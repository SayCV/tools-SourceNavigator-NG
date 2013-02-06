/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1998,2007 Oracle.  All rights reserved.
 *
 * $Id: xa.c,v 12.15 2007/06/29 00:25:02 margo Exp $
 */

#include "db_config.h"

#include "db_int.h"
#include "dbinc/txn.h"

static int __db_xa_close __P((char *, int, long));
static int __db_xa_commit __P((XID *, int, long));
static int __db_xa_complete __P((int *, int *, int, long));
static int __db_xa_end __P((XID *, int, long));
static int __db_xa_forget __P((XID *, int, long));
static int __db_xa_open __P((char *, int, long));
static int __db_xa_prepare __P((XID *, int, long));
static int __db_xa_recover __P((XID *, long, int, long));
static int __db_xa_rollback __P((XID *, int, long));
static int __db_xa_start __P((XID *, int, long));
static int __xa_put_txn __P((DB_ENV *, DB_TXN *));

/*
 * Possible flag values:
 *	Dynamic registration	0 => no dynamic registration
 *				TMREGISTER => dynamic registration
 *	Asynchronous operation	0 => no support for asynchrony
 *				TMUSEASYNC => async support
 *	Migration support	0 => migration of transactions across
 *				     threads is possible
 *				TMNOMIGRATE => no migration across threads
 */
const struct xa_switch_t db_xa_switch = {
	 "Berkeley DB",		/* name[RMNAMESZ] */
	 TMNOMIGRATE,		/* flags */
	 0,			/* version */
	 __db_xa_open,		/* xa_open_entry */
	 __db_xa_close,		/* xa_close_entry */
	 __db_xa_start,		/* xa_start_entry */
	 __db_xa_end,		/* xa_end_entry */
	 __db_xa_rollback,	/* xa_rollback_entry */
	 __db_xa_prepare,	/* xa_prepare_entry */
	 __db_xa_commit,	/* xa_commit_entry */
	 __db_xa_recover,	/* xa_recover_entry */
	 __db_xa_forget,	/* xa_forget_entry */
	 __db_xa_complete	/* xa_complete_entry */
};

/*
 * If you want your XA server to be multi-threaded, then you must (at least)
 * edit this file and change:
 *	#undef  XA_MULTI_THREAD
 * to:
 *	#define XA_MULTI_THREAD 1
 */
#undef XA_MULTI_THREAD

/*
 * __xa_get_txn --
 *	Return a pointer to the current transaction structure for the
 * designated environment.  If do_init is non-zero and we don't find a
 * structure for the current thread, then create a new structure for it.
 *
 * PUBLIC: int __xa_get_txn __P((DB_ENV *, DB_TXN **, int));
 */
int
__xa_get_txn(dbenv, txnp, do_init)
	DB_ENV *dbenv;
	DB_TXN **txnp;
	int do_init;
{
#ifdef XA_MULTI_THREAD
	DB_TXN *t;
	DB_TXNMGR *mgr;
	TXN_DETAIL *td;
	db_threadid_t tid;
	pid_t pid;
#endif
	int ret;

	ret = 0;

#ifdef XA_MULTI_THREAD
	dbenv->thread_id(dbenv, &pid, &tid);
	*txnp = NULL;

	DB_ASSERT(dbenv, dbenv->tx_handle != NULL);
	mgr = dbenv->tx_handle;

	/*
	 * We need to protect the xa_txn linked list, but the environment does
	 * not have a mutex.  Since we are in an XA transaction environment,
	 * we know there is a transaction structure, we can use its mutex.
	 */
	MUTEX_LOCK(dbenv, mgr->mutex);
	TAILQ_FOREACH(t, &dbenv->xa_txn, xalinks) {
		td = t->td;
		if (td->pid != pid)
			continue;
#ifdef HAVE_SIMPLE_THREAD_TYPE
		if (t->tid == tid) {
			*txnp = t;
			break;
		}
#else
		if (memcmp(&t->tid, &tid, sizeof(tid)) == 0) {
			*txnp = t;
			break;
		}
#endif
	}
	MUTEX_UNLOCK(dbenv, mgr->mutex);

	if (*txnp == NULL) {
		if (!do_init)
			ret = EINVAL;
		else if ((ret =
		    __os_malloc(dbenv, sizeof(DB_TXN), txnp)) == 0) {
			(*txnp)->tid = tid;
			MUTEX_LOCK(dbenv, mgr->mutex);
			TAILQ_INSERT_HEAD(&dbenv->xa_txn, *txnp, xalinks);
			MUTEX_UNLOCK(dbenv, mgr->mutex);
		}
	}
#else
	COMPQUIET(do_init, 0);

	*txnp = TAILQ_FIRST(&dbenv->xa_txn);
	if (*txnp == NULL &&
	    (ret = __os_calloc(dbenv, 1, sizeof(DB_TXN), txnp)) == 0) {
		(*txnp)->txnid = TXN_INVALID;
		TAILQ_INSERT_HEAD(&dbenv->xa_txn, *txnp, xalinks);
	}
#endif

	return (ret);
}

static int
__xa_put_txn(dbenv, txnp)
	DB_ENV *dbenv;
	DB_TXN *txnp;
{
#ifdef XA_MULTI_THREAD
	DB_TXNMGR *mgr;
	mgr = dbenv->tx_handle;

	MUTEX_LOCK(dbenv, mgr->mutex);
	TAILQ_REMOVE(&dbenv->xa_txn, txnp, xalinks);
	MUTEX_UNLOCK(dbenv, mgr->mutex);
	__os_free(dbenv, txnp);
#else
	COMPQUIET(dbenv, NULL);
	txnp->txnid = TXN_INVALID;
#endif
	return (0);
}

#ifdef XA_MULTI_THREAD
#define	XA_FLAGS \
	(DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | \
	DB_INIT_TXN | DB_THREAD)
#else
#define	XA_FLAGS \
	(DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | \
	DB_INIT_TXN)
#endif

/*
 * __db_xa_open --
 *	The open call in the XA protocol.  The rmid field is an id number
 * that the TM assigned us and will pass us on every xa call.  We need to
 * map that rmid number into a dbenv structure that we create during
 * initialization.  Since this id number is thread specific, we do not
 * need to store it in shared memory.  The file xa_map.c implements all
 * such xa->db mappings.
 *	The xa_info field is instance specific information.  We require
 * that the value of DB_HOME be passed in xa_info.  Since xa_info is the
 * only thing that we get to pass to db_env_create, any config information
 * will have to be done via a config file instead of via the db_env_create
 * call.
 */
static int
__db_xa_open(xa_info, rmid, arg_flags)
	char *xa_info;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	u_long flags;

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);
	if (flags != TMNOFLAGS)
		return (XAER_INVAL);

	/* Verify if we already have this environment open. */
	if (__db_rmid_to_env(rmid, &dbenv) == 0)
		return (XA_OK);

	/* Open a new environment. */
	if (db_env_create(&dbenv, 0) != 0)
		return (XAER_RMERR);
	if (dbenv->open(dbenv, xa_info, XA_FLAGS, 0) != 0)
		goto err;

	/* Create the mapping. */
	if (__db_map_rmid(rmid, dbenv) != 0)
		goto err;

	/* Allocate space for the current transaction. */
	TAILQ_INIT(&dbenv->xa_txn);

	return (XA_OK);

err:	(void)dbenv->close(dbenv, 0);

	return (XAER_RMERR);
}

/*
 * __db_xa_close --
 *	The close call of the XA protocol.  The only trickiness here
 * is that if there are any active transactions, we must fail.  It is
 * *not* an error to call close on an environment that has already been
 * closed (I am interpreting that to mean it's OK to call close on an
 * environment that has never been opened).
 */
static int
__db_xa_close(xa_info, rmid, arg_flags)
	char *xa_info;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	DB_TXN *t;
	int ret, t_ret;
	u_long flags;

	COMPQUIET(xa_info, NULL);

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);
	if (flags != TMNOFLAGS)
		return (XAER_INVAL);

	/* If the environment is closed, then we're done. */
	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XA_OK);

	/* Check if there are any pending transactions. */
	if ((t = TAILQ_FIRST(&dbenv->xa_txn)) != NULL &&
	    t->txnid != TXN_INVALID)
		return (XAER_PROTO);

	/* Destroy the mapping. */
	ret = __db_unmap_rmid(rmid);

	/* Discard space held for the current transaction. */
	while ((t = TAILQ_FIRST(&dbenv->xa_txn)) != NULL) {
		TAILQ_REMOVE(&dbenv->xa_txn, t, xalinks);
		__os_free(dbenv, t);
	}

	/* Close the environment. */
	if ((t_ret = dbenv->close(dbenv, 0)) != 0 && ret == 0)
		ret = t_ret;

	return (ret == 0 ? XA_OK : XAER_RMERR);
}

/*
 * __db_xa_start --
 *	Begin a transaction for the current resource manager.
 */
static int
__db_xa_start(xid, rmid, arg_flags)
	XID *xid;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	DB_TXN *txnp;
	TXN_DETAIL *td;
	roff_t off;
	u_long flags;
	int is_known;

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

#define	OK_FLAGS	(TMJOIN | TMRESUME | TMNOWAIT | TMASYNC | TMNOFLAGS)
	if (LF_ISSET(~OK_FLAGS))
		return (XAER_INVAL);

	if (LF_ISSET(TMJOIN) && LF_ISSET(TMRESUME))
		return (XAER_INVAL);

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);

	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	is_known = __db_xid_to_txn(dbenv, xid, &off) == 0;

	if (is_known && !LF_ISSET(TMRESUME) && !LF_ISSET(TMJOIN))
		return (XAER_DUPID);

	if (!is_known && LF_ISSET(TMRESUME | TMJOIN))
		return (XAER_NOTA);

	/*
	 * This can't block, so we can ignore TMNOWAIT.
	 *
	 * Other error conditions: RMERR, RMFAIL, OUTSIDE, PROTO, RB*
	 */
	if (is_known) {
		td = R_ADDR(&dbenv->tx_handle->reginfo, off);
		if (td->xa_status == TXN_XA_SUSPENDED &&
		    !LF_ISSET(TMRESUME | TMJOIN))
			return (XAER_PROTO);
		if (td->xa_status == TXN_XA_DEADLOCKED)
			return (XA_RBDEADLOCK);
		if (td->xa_status == TXN_XA_ABORTED)
			return (XA_RBOTHER);

		/* Now, fill in the global transaction structure. */
		if (__xa_get_txn(dbenv, &txnp, 1) != 0)
			return (XAER_RMERR);
		if (__txn_continue(dbenv, txnp, td) != 0)
			return (XAER_RMERR);
		td->xa_status = TXN_XA_STARTED;
	} else {
		if (__xa_get_txn(dbenv, &txnp, 1) != 0)
			return (XAER_RMERR);
		if (__txn_xa_begin(dbenv, txnp))
			return (XAER_RMERR);
		(void)__db_map_xid(dbenv, xid, txnp->td);
		td = txnp->td;
		td->xa_status = TXN_XA_STARTED;
	}
	return (XA_OK);
}

/*
 * __db_xa_end --
 *	Disassociate the current transaction from the current process.
 */
static int
__db_xa_end(xid, rmid, flags)
	XID *xid;
	int rmid;
	long flags;
{
	DB_ENV *dbenv;
	DB_TXN *txn;
	TXN_DETAIL *td;
	roff_t off;

	if (flags != TMNOFLAGS && !LF_ISSET(TMSUSPEND | TMSUCCESS | TMFAIL))
		return (XAER_INVAL);

	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	if (__db_xid_to_txn(dbenv, xid, &off) != 0)
		return (XAER_NOTA);

	if (__xa_get_txn(dbenv, &txn, 0) != 0)
		return (XAER_RMERR);

	td = R_ADDR(&dbenv->tx_handle->reginfo, off);
	if (td != txn->td)
		return (XAER_PROTO);

	if (td->xa_status == TXN_XA_DEADLOCKED)
		return (XA_RBDEADLOCK);

	if (td->status == TXN_ABORTED)
		return (XA_RBOTHER);

	if (td->xa_status != TXN_XA_STARTED)
		return (XAER_PROTO);

	/*
	 * If we ever support XA migration, we cannot keep SUSPEND/END
	 * status in the shared region; it would have to be process local.
	 */
	if (LF_ISSET(TMSUSPEND))
		td->xa_status = TXN_XA_SUSPENDED;
	else
		td->xa_status = TXN_XA_ENDED;

	/*
	 * XXX
	 * This can fail in XA_MULTI_THREAD mode.
	 */
	(void)__xa_put_txn(dbenv, txn);
	return (XA_OK);
}

/*
 * __db_xa_prepare --
 *	Sync the log to disk so we can guarantee recoverability.
 */
static int
__db_xa_prepare(xid, rmid, arg_flags)
	XID *xid;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	DB_TXN *txnp;
	TXN_DETAIL *td;
	roff_t off;
	u_long flags;

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);
	if (flags != TMNOFLAGS)
		return (XAER_INVAL);

	/*
	 * We need to know if we've ever called prepare on this.
	 * As part of the prepare, we set the xa_status field to
	 * reflect that fact that prepare has been called, and if
	 * it's ever called again, it's an error.
	 */
	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	if (__db_xid_to_txn(dbenv, xid, &off) != 0)
		return (XAER_NOTA);
	td = R_ADDR(&dbenv->tx_handle->reginfo, off);
	if (td->xa_status == TXN_XA_DEADLOCKED)
		return (XA_RBDEADLOCK);

	if (td->xa_status != TXN_XA_ENDED && td->xa_status != TXN_XA_SUSPENDED)
		return (XAER_PROTO);

	/* Now, fill in the global transaction structure. */
	if (__xa_get_txn(dbenv, &txnp, 0) != 0)
		return (XAER_PROTO);
	if (__txn_continue(dbenv, txnp, td) != 0)
		return (XAER_RMERR);

	if (txnp->prepare(txnp, (u_int8_t *)xid->data) != 0)
		return (XAER_RMERR);

	td->xa_status = TXN_XA_PREPARED;

	/*
	 * XXX
	 * This can fail in XA_MULTI_THREAD mode.
	 */
	(void)__xa_put_txn(dbenv, txnp);
	return (XA_OK);
}

/*
 * __db_xa_commit --
 *	Commit the transaction
 */
static int
__db_xa_commit(xid, rmid, arg_flags)
	XID *xid;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	DB_TXN *txnp;
	TXN_DETAIL *td;
	roff_t off;
	u_long flags;

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);
#undef	OK_FLAGS
#define	OK_FLAGS	(TMNOFLAGS | TMNOWAIT | TMONEPHASE)
	if (LF_ISSET(~OK_FLAGS))
		return (XAER_INVAL);

	/*
	 * We need to know if we've ever called prepare on this.
	 * We can verify this by examining the xa_status field.
	 */
	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	if (__db_xid_to_txn(dbenv, xid, &off) != 0)
		return (XAER_NOTA);

	td = R_ADDR(&dbenv->tx_handle->reginfo, off);
	if (td->xa_status == TXN_XA_DEADLOCKED)
		return (XA_RBDEADLOCK);

	if (td->xa_status == TXN_XA_ABORTED)
		return (XA_RBOTHER);

	if (LF_ISSET(TMONEPHASE) &&
	    td->xa_status != TXN_XA_ENDED && td->xa_status != TXN_XA_SUSPENDED)
		return (XAER_PROTO);

	if (!LF_ISSET(TMONEPHASE) && td->xa_status != TXN_XA_PREPARED)
		return (XAER_PROTO);

	/* Now, fill in the global transaction structure. */
	if (__xa_get_txn(dbenv, &txnp, 0) != 0)
		return (XAER_RMERR);
	if (__txn_continue(dbenv, txnp, td) != 0)
		return (XAER_RMERR);

	if (txnp->commit(txnp, 0) != 0)
		return (XAER_RMERR);

	/*
	 * XXX
	 * This can fail in XA_MULTI_THREAD mode.
	 */
	(void)__xa_put_txn(dbenv, txnp);
	return (XA_OK);
}

/*
 * __db_xa_recover --
 *	Returns a list of prepared and heuristically completed transactions.
 *
 * The return value is the number of xids placed into the xid array (less
 * than or equal to the count parameter).  The flags are going to indicate
 * whether we are starting a scan or continuing one.
 */
static int
__db_xa_recover(xids, count, rmid, flags)
	XID *xids;
	long count, flags;
	int rmid;
{
	DB_ENV *dbenv;
	u_int32_t newflags;
	long rval;

	/* If the environment is closed, then we're done. */
	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	if (LF_ISSET(TMSTARTRSCAN))
		newflags = DB_FIRST;
	else if (LF_ISSET(TMENDRSCAN))
		newflags = DB_LAST;
	else
		newflags = DB_NEXT;

	rval = 0;
	if (__txn_get_prepared(dbenv, xids, NULL, count, &rval, newflags) != 0)
		return (XAER_RMERR);
	else
		return (rval);
}

/*
 * __db_xa_rollback
 *	Abort an XA transaction.
 */
static int
__db_xa_rollback(xid, rmid, arg_flags)
	XID *xid;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	DB_TXN *txnp;
	TXN_DETAIL *td;
	roff_t off;
	u_long flags;

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);
	if (flags != TMNOFLAGS)
		return (XAER_INVAL);

	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	if (__db_xid_to_txn(dbenv, xid, &off) != 0)
		return (XAER_NOTA);

	td = R_ADDR(&dbenv->tx_handle->reginfo, off);
	if (td->xa_status == TXN_XA_DEADLOCKED)
		return (XA_RBDEADLOCK);

	if (td->xa_status == TXN_XA_ABORTED)
		return (XA_RBOTHER);

	if (td->xa_status != TXN_XA_ENDED &&
	    td->xa_status != TXN_XA_SUSPENDED &&
	    td->xa_status != TXN_XA_PREPARED)
		return (XAER_PROTO);

	/* Now, fill in the global transaction structure. */
	if (__xa_get_txn(dbenv, &txnp, 0) != 0)
		return (XAER_RMERR);
	if (__txn_continue(dbenv, txnp, td) != 0)
		return (XAER_RMERR);
	if (txnp->abort(txnp) != 0)
		return (XAER_RMERR);

	/*
	 * XXX
	 * This can fail in XA_MULTI_THREAD mode.
	 */
	(void)__xa_put_txn(dbenv, txnp);
	return (XA_OK);
}

/*
 * __db_xa_forget --
 *	Forget about an XID for a transaction that was heuristically
 * completed.  Since we do not heuristically complete anything, I
 * don't think we have to do anything here, but we should make sure
 * that we reclaim the slots in the txnid table.
 */
static int
__db_xa_forget(xid, rmid, arg_flags)
	XID *xid;
	int rmid;
	long arg_flags;
{
	DB_ENV *dbenv;
	roff_t off;
	u_long flags;

	flags = (u_long)arg_flags;	/* Conversion for bit operations. */

	if (LF_ISSET(TMASYNC))
		return (XAER_ASYNC);
	if (flags != TMNOFLAGS)
		return (XAER_INVAL);

	if (__db_rmid_to_env(rmid, &dbenv) != 0)
		return (XAER_PROTO);

	/*
	 * If mapping is gone, then we're done.
	 */
	if (__db_xid_to_txn(dbenv, xid, &off) != 0)
		return (XA_OK);

	__db_unmap_xid(dbenv, xid, off);

	/* No fatal value that would require an XAER_RMFAIL. */
	return (XA_OK);
}

/*
 * __db_xa_complete --
 *	Used to wait for asynchronous operations to complete.  Since we're
 *	not doing asynch, this is an invalid operation.
 */
static int
__db_xa_complete(handle, retval, rmid, flags)
	int *handle, *retval, rmid;
	long flags;
{
	COMPQUIET(handle, NULL);
	COMPQUIET(retval, NULL);
	COMPQUIET(rmid, 0);
	COMPQUIET(flags, 0);

	return (XAER_INVAL);
}
