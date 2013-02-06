/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996,2007 Oracle.  All rights reserved.
 *
 * $Id: os_region.c,v 12.11 2007/05/17 15:15:46 bostic Exp $
 */

#include "db_config.h"

#include "db_int.h"

/*
 * __os_r_attach --
 *	Attach to a shared memory region.
 *
 * PUBLIC: int __os_r_attach __P((DB_ENV *, REGINFO *, REGION *));
 */
int
__os_r_attach(dbenv, infop, rp)
	DB_ENV *dbenv;
	REGINFO *infop;
	REGION *rp;
{
	int ret;

	/*
	 * All regions are created on 8K boundaries out of sheer paranoia,
	 * so we don't make some underlying VM unhappy. Make sure we don't
	 * overflow or underflow.
	 */
#define	OS_VMPAGESIZE		(8 * 1024)
#define	OS_VMROUNDOFF(i) {						\
	if ((i) <							\
	    (UINT32_MAX - OS_VMPAGESIZE) + 1 || (i) < OS_VMPAGESIZE)	\
		(i) += OS_VMPAGESIZE - 1;				\
	(i) -= (i) % OS_VMPAGESIZE;					\
}
	OS_VMROUNDOFF(rp->size);

#ifdef DB_REGIONSIZE_MAX
	/* Some architectures have hard limits on the maximum region size. */
	if (rp->size > DB_REGIONSIZE_MAX) {
		__db_errx(dbenv, "region size %lu is too large; maximum is %lu",
		    (u_long)rp->size, (u_long)DB_REGIONSIZE_MAX);
		return (EINVAL);
	}
#endif

	/*
	 * If a region is private, malloc the memory.
	 *
	 * !!!
	 * If this fails because the region is too large to malloc, mmap(2)
	 * using the MAP_ANON or MAP_ANONYMOUS flags would be an alternative.
	 * I don't know of any architectures (yet!) where malloc is a problem.
	 */
	if (F_ISSET(dbenv, DB_ENV_PRIVATE)) {
#if defined(HAVE_MUTEX_HPPA_MSEM_INIT)
		/*
		 * !!!
		 * There exist spinlocks that don't work in malloc memory, e.g.,
		 * the HP/UX msemaphore interface.  If we don't have locks that
		 * will work in malloc memory, we better not be private or not
		 * be threaded.
		 */
		if (F_ISSET(dbenv, DB_ENV_THREAD)) {
			__db_errx(dbenv, "%s",
    "architecture does not support locks inside process-local (malloc) memory");
			__db_errx(dbenv, "%s",
    "application may not specify both DB_PRIVATE and DB_THREAD");
			return (EINVAL);
		}
#endif
		if ((ret = __os_malloc(
		    dbenv, sizeof(REGENV), &infop->addr)) != 0)
			return (ret);

		infop->max_alloc = rp->size;
	} else {
		/*
		 * If the user replaced the map call, call through their
		 * interface.
		 */
		if (DB_GLOBAL(j_map) != NULL && (ret = DB_GLOBAL(j_map)
		    (infop->name, rp->size, 1, 0, &infop->addr)) != 0)
			return (ret);

		/* Get some space from the underlying system. */
		if ((ret = __os_r_sysattach(dbenv, infop, rp)) != 0)
			return (ret);
	}

	/*
	 * We may require alignment the underlying system or heap allocation
	 * library doesn't supply.  Align the address if necessary, saving
	 * the original values for restoration when the region is discarded.
	 */
	infop->addr_orig = infop->addr;
	infop->addr = ALIGNP_INC(infop->addr_orig, sizeof(size_t));

	rp->size_orig = rp->size;
	if (infop->addr != infop->addr_orig)
		rp->size -= (roff_t)
		    ((u_int8_t *)infop->addr - (u_int8_t *)infop->addr_orig);

	return (0);
}

/*
 * __os_r_detach --
 *	Detach from a shared memory region.
 *
 * PUBLIC: int __os_r_detach __P((DB_ENV *, REGINFO *, int));
 */
int
__os_r_detach(dbenv, infop, destroy)
	DB_ENV *dbenv;
	REGINFO *infop;
	int destroy;
{
	REGION *rp;

	rp = infop->rp;

	/* Restore any address/size altered for alignment reasons. */
	if (infop->addr != infop->addr_orig) {
		infop->addr = infop->addr_orig;
		rp->size = rp->size_orig;
	}

	/* If a region is private, free the memory. */
	if (F_ISSET(dbenv, DB_ENV_PRIVATE)) {
		__os_free(dbenv, infop->addr);
		return (0);
	}

	/* If the user replaced the map call, call through their interface. */
	if (DB_GLOBAL(j_unmap) != NULL)
		return (DB_GLOBAL(j_unmap)(infop->addr, rp->size));

	return (__os_r_sysdetach(dbenv, infop, destroy));
}
