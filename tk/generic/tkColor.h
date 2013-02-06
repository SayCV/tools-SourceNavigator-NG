/*
 * tkColor.h --
 *
 *	Declarations of data types and functions used by the
 *	Tk color module.
 *
 * Copyright (c) 1996-1997 by Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: tkColor.h 144 2003-02-05 10:56:26Z mdejong $
 */

#ifndef _TKCOLOR
#define _TKCOLOR

#include <tkInt.h>

#ifdef BUILD_tk
# undef TCL_STORAGE_CLASS
# define TCL_STORAGE_CLASS DLLEXPORT
#endif

/* CYGNUS LOCAL.  */

/* A linked list of GC structures.  */

typedef struct TkGCList {
    /* Next item on list.  */
    struct TkGCList *next;
    /* The display for the GC.  */
    Display *display;
    /* The GC.  */
    GC gc;
    /* GCForeground or GCBackground.  */
    unsigned long mask;
} TkGCList;

/* END CYGNUS LOCAL */

/*
 * One of the following data structures is used to keep track of
 * each color that is being used by the application; typically there
 * is a colormap entry allocated for each of these colors.
 */

#define TK_COLOR_BY_NAME	1
#define TK_COLOR_BY_VALUE	2

#define COLOR_MAGIC ((unsigned int) 0x46140277)

typedef struct TkColor {
    XColor color;		/* Information about this color. */
    unsigned int magic;		/* Used for quick integrity check on this
				 * structure.   Must always have the
				 * value COLOR_MAGIC. */
    GC gc;			/* Simple gc with this color as foreground
				 * color and all other fields defaulted.
				 * May be None. */
    Screen *screen;		/* Screen where this color is valid.  Used
				 * to delete it, and to find its display. */
    Colormap colormap;		/* Colormap from which this entry was
				 * allocated. */
    Visual *visual;             /* Visual associated with colormap. */
    int resourceRefCount;	/* Number of active uses of this color (each
				 * active use corresponds to a call to
				 * Tk_AllocColorFromObj or Tk_GetColor).
				 * If this count is 0, then this TkColor
				 * structure is no longer valid and it isn't
				 * present in a hash table: it is being
				 * kept around only because there are objects
				 * referring to it.  The structure is freed
				 * when resourceRefCount and objRefCount
				 * are both 0. */
    int objRefCount;		/* The number of Tcl objects that reference
				 * this structure. */
    int type;			/* TK_COLOR_BY_NAME or TK_COLOR_BY_VALUE */
    Tcl_HashEntry *hashPtr;	/* Pointer to hash table entry for this
				 * structure. (for use in deleting entry). */
    struct TkColor *nextPtr;	/* Points to the next TkColor structure with
				 * the same color name.  Colors with the
				 * same name but different screens or
				 * colormaps are chained together off a
				 * single entry in nameTable.  For colors in
				 * valueTable (those allocated by
				 * Tk_GetColorByValue) this field is always
				 * NULL. */
    /* CYGNUS LOCAL */
    TkGCList *gcList;		/* List of GCs which use this color.  */
} TkColor;

/*
 * Common APIs exported from all platform-specific implementations.
 */

#ifndef TkpFreeColor
EXTERN void		TkpFreeColor _ANSI_ARGS_((TkColor *tkColPtr));
#endif
EXTERN TkColor *	TkpGetColor _ANSI_ARGS_((Tk_Window tkwin,
			    Tk_Uid name));
EXTERN TkColor *	TkpGetColorByValue _ANSI_ARGS_((Tk_Window tkwin,
			    XColor *colorPtr));	

/* CYGNUS LOCAL.  */
EXTERN void		TkMapOverColors _ANSI_ARGS_((void (*) (TkColor *)));
EXTERN void		TkColorChanged _ANSI_ARGS_((TkColor *)); 

# undef TCL_STORAGE_CLASS
# define TCL_STORAGE_CLASS DLLIMPORT

#endif /* _TKCOLOR */

