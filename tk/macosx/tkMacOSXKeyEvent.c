/*
 * tkMacOSXKeyEvent.c --
 *
 *	This file implements functions that decode & handle keyboard events
 *      on MacOS X.
 *
 *      Copyright 2001, Apple Computer, Inc.
 *
 *      The following terms apply to all files originating from Apple
 *      Computer, Inc. ("Apple") and associated with the software
 *      unless explicitly disclaimed in individual files.
 *
 *
 *      Apple hereby grants permission to use, copy, modify,
 *      distribute, and license this software and its documentation
 *      for any purpose, provided that existing copyright notices are
 *      retained in all copies and that this notice is included
 *      verbatim in any distributions. No written agreement, license,
 *      or royalty fee is required for any of the authorized
 *      uses. Modifications to this software may be copyrighted by
 *      their authors and need not follow the licensing terms
 *      described here, provided that the new terms are clearly
 *      indicated on the first page of each file where they apply.
 *
 *
 *      IN NO EVENT SHALL APPLE, THE AUTHORS OR DISTRIBUTORS OF THE
 *      SOFTWARE BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
 *      INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF
 *      THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
 *      EVEN IF APPLE OR THE AUTHORS HAVE BEEN ADVISED OF THE
 *      POSSIBILITY OF SUCH DAMAGE.  APPLE, THE AUTHORS AND
 *      DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
 *      BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS
 *      SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, AND APPLE,THE
 *      AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
 *      MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *      GOVERNMENT USE: If you are acquiring this software on behalf
 *      of the U.S. government, the Government shall have only
 *      "Restricted Rights" in the software and related documentation
 *      as defined in the Federal Acquisition Regulations (FARs) in
 *      Clause 52.227.19 (c) (2).  If you are acquiring the software
 *      on behalf of the Department of Defense, the software shall be
 *      classified as "Commercial Computer Software" and the
 *      Government shall have only "Restricted Rights" as defined in
 *      Clause 252.227-7013 (c) (1) of DFARs.  Notwithstanding the
 *      foregoing, the authors grant the U.S. Government and others
 *      acting in its behalf permission to use and distribute the
 *      software in accordance with the terms specified in this
 *      license.
 */

#include "tkMacOSXInt.h"
#include "tkPort.h"
#include "tkMacOSXEvent.h"

typedef struct {
    WindowRef   whichWindow;
    Point       global;
    Point       local;
    int         state;
    char        ch;
    UInt32      keyCode;
    UInt32      keyModifiers;
    UInt32      message;  
} KeyEventData;

static Tk_Window gGrabWinPtr = NULL;     /* Current grab window, NULL if no grab. */
static Tk_Window gKeyboardWinPtr = NULL; /* Current keyboard grab window. */

/*
 * Declarations for functions used only in this file.
 */
 
static int GenerateKeyEvent _ANSI_ARGS_(( EventKind eKind, 
        KeyEventData * e, 
        Window window, 
        UInt32 savedKeyCode,
        UInt32 savedModifiers));
/*
 *----------------------------------------------------------------------
 *
 * TkMacOSXProcessKeyboardEvent --
 *
 *        This routine processes the event in eventPtr, and
 *        generates the appropriate Tk events from it.
 *
 * Results:
 *        True if event(s) are generated - false otherwise.
 *
 * Side effects:
 *        Additional events may be place on the Tk event queue.
 *
 *----------------------------------------------------------------------
 */

int TkMacOSXProcessKeyboardEvent(
        TkMacOSXEvent * eventPtr, 
        MacEventStatus * statusPtr)
{
    static UInt32 savedKeyCode = 0;
    static UInt32 savedModifiers = 0;
    OSStatus     status;
    KeyEventData keyEventData;
    Window    window;
    MenuRef   menuRef;
    MenuItemIndex menuItemIndex;
    int eventGenerated;
    
    statusPtr->handledByTk = 1;
    keyEventData.whichWindow = FrontNonFloatingWindow();
    if (keyEventData.whichWindow == NULL) {
        return 0;
    }
    GetMouse(&keyEventData.local);
    keyEventData.global = keyEventData.local;
    LocalToGlobal(&keyEventData.global);
    keyEventData.state = TkMacOSXButtonKeyState();
#if 0
    /*
     * This block of code seems like a good idea, to trap
     * key-bindings which point directly to menus, but it
     * has a number of problems:
     * (1) when grabs are present we definitely don't want
     * to do this.
     * (2) Tk's semantics define accelerator keystrings in
     * menus as a purely visual adornment, and require that
     * the developer create separate bindings to trigger
     * them.  This breaks those semantics.  (i.e. Tk will
     * behave differently on Aqua to the behaviour on Unix/Win).
     * (3) Tk's bindings depend on the current window's bindtags,
     * which may be completely different to what happens to be
     * in some global menu (agreed, it shouldn't be that different,
     * but it often is).
     * 
     * While a better middleground might be possible, the best, most
     * compatible, approach at present is to disable this block.
     */
    if (IsMenuKeyEvent(NULL, eventPtr->eventRef, 
            kNilOptions, &menuRef, &menuItemIndex)) {
        int    oldMode;
        MenuID menuID;
        KeyMap theKeys;
        int    selection;
        
        menuID = GetMenuID(menuRef);
        selection = (menuID << 16 ) | menuItemIndex;
    
        GetKeys(theKeys);
        oldMode = Tcl_SetServiceMode(TCL_SERVICE_ALL);
        TkMacOSXClearMenubarActive();
 
        /*
         * Handle -postcommand
         */
         
         TkMacOSXPreprocessMenu();
         TkMacOSXHandleMenuSelect(selection, theKeys[1] & 4);
         Tcl_SetServiceMode(oldMode);
         return 0; /* TODO: may not be on event on queue. */
    }
#endif

    status = GetEventParameter(eventPtr->eventRef, 
            kEventParamKeyMacCharCodes,
            typeChar, NULL,
            sizeof(keyEventData.ch), NULL,
            &keyEventData.ch);
    if (status != noErr) {
        fprintf(stderr, "Failed to retrieve KeyMacCharCodes\n" );
        statusPtr->err = 1;
        return 1;
    } 
    status = GetEventParameter(eventPtr->eventRef, 
            kEventParamKeyCode,
            typeUInt32, NULL,
            sizeof(keyEventData.keyCode), NULL,
            &keyEventData.keyCode);
    if (status != noErr) {
        fprintf(stderr, "Failed to retrieve KeyCode\n" );
        statusPtr->err = 1;
        return 1;
    }
    status = GetEventParameter(eventPtr->eventRef, 
            kEventParamKeyModifiers,
            typeUInt32, NULL,
            sizeof(keyEventData.keyModifiers), NULL,
            &keyEventData.keyModifiers);
    if (status != noErr) {
        fprintf(stderr, "Failed to retrieve KeyModifiers\n" );
        statusPtr->err = 1;
        return 1;
    }
    keyEventData.message = keyEventData.ch|(keyEventData.keyCode << 8);

    window = TkMacOSXGetXWindow(keyEventData.whichWindow);
    
    eventGenerated = GenerateKeyEvent(eventPtr->eKind, &keyEventData,
				      window, savedKeyCode, savedModifiers);
    savedModifiers = keyEventData.keyModifiers;
    
    if (eventGenerated == 0) {
	savedKeyCode = keyEventData.message;
	return false;
    } else if (eventGenerated == -1) {
	savedKeyCode = 0;
	return false;
    } else {
	savedKeyCode = 0;
	return true;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * GenerateKeyEvent --
 *
 *        Given Macintosh keyUp, keyDown & autoKey events this function
 *        generates the appropiate X key events.  The window that is passed
 *        should represent the frontmost window - which will recieve the
 *        event.
 *
 * Results:
 *        1 if an event was generated, 0 if we are waiting for another
 *        byte of a multi-byte sequence, and -1 for any other error.
 *
 * Side effects:
 *        Additional events may be place on the Tk event queue.
 *
 *----------------------------------------------------------------------
 */

int
GenerateKeyEvent( EventKind eKind, 
        KeyEventData * e, 
        Window window, 
        UInt32 savedKeyCode,
        UInt32 savedModifiers )
{
    Tk_Window tkwin;
    XEvent event;
    unsigned char byte;
    char buf[16];
    TkDisplay *dispPtr;
    
    /*
     * The focus must be in the FrontWindow on the Macintosh.
     * We then query Tk to determine the exact Tk window
     * that owns the focus.
     */

    dispPtr = TkGetDisplayList();
    tkwin = Tk_IdToWindow(dispPtr->display, window);
    
    if (tkwin == NULL) {
        fprintf(stderr,"tkwin == NULL, %d\n", __LINE__);
        return -1;
    }
    
    tkwin = (Tk_Window) ((TkWindow *) tkwin)->dispPtr->focusPtr;
    if (tkwin == NULL) {
        fprintf(stderr,"tkwin == NULL, %d\n", __LINE__);
        return -1;
    }
    byte = (e->message&charCodeMask);
    if (byte == 0) {
        /* 
         * Either we have a pure-modifier change, or perhaps
         * a dead-key (e.g. opt-e) was pressed.  In the former case we do
         * want to generate an event, in the latter I'm not sure
         * what to do.
         */
        if (eKind == kEventRawKeyModifiersChanged) {
            /* Drop through to the event code below */
        } else {
	    /* 
	     * What shall we do here?  We certainly aren't dealing
	     * with deadkeys at present.  Is this where they come?
	     */
            return 0;
        }
    } else if ((savedKeyCode == 0) &&
            (Tcl_ExternalToUtf(NULL, TkMacOSXCarbonEncoding, 
			       (char *) &byte, 1, 0, NULL, 
                        buf, sizeof(buf), NULL, NULL, NULL) != TCL_OK)) {
        /*
         * This event specifies a lead byte.  Wait for the second byte
         * to come in before sending the XEvent.
         */
        fprintf(stderr,"Failed %02x\n", byte);
        return 0;
    }   

    event.xany.send_event = False;
    event.xkey.same_screen = true;
    event.xkey.subwindow = None;
    event.xkey.time = TkpGetMS();

    event.xkey.x_root = e->global.h;
    event.xkey.y_root = e->global.v;
    Tk_TopCoordsToWindow(tkwin, e->local.h, e->local.v, 
            &event.xkey.x, &event.xkey.y);
    
    /* 
     * Now, we may have a problem here.  How do we handle 'Option-char'
     * keypresses?  The problem is that we might want to bind to some of
     * these (e.g. Cmd-Opt-d is 'uncomment' in Alpha), but Option-d
     * generates a 'delta' symbol with some keycode unrelated to 'd', and so
     * the binding never triggers.  In any case, the delta that is produced
     * is never mapped to an 'XK_Greek_DELTA' keysym so bindings on that
     * won't work either (a general KeyPress binding will of course trigger,
     * but a specific binding on XK_Greek_DELTA will not).
     * 
     * I think what we want is for the event to contain information on
     * both the 'Opt-d' side of things and the 'delta'.  Then a binding
     * on Opt-d will trigger, but the ascii/string representation of the
     * event will be a delta.
     * 
     * A different way to look at this is that 'Opt-d' is delta, but that
     * Command-Opt-d is nothing to do with delta, but I'm not sure that is
     * helpful.
     * 
     * Also some keypresses (Opt-e) are dead-keys to add accents to
     * letters.  We don't handle them yet.
     * 
     * Help needed!
     */
    event.xkey.keycode = byte |
        ((savedKeyCode & charCodeMask) << 8) |
        ((e->message&keyCodeMask) << 8);
   
    event.xany.serial = Tk_Display(tkwin)->request;
    event.xkey.window = Tk_WindowId(tkwin);
    event.xkey.display = Tk_Display(tkwin);
    event.xkey.root = XRootWindow(Tk_Display(tkwin), 0);
    event.xkey.state =  e->state;

    switch(eKind) {
        case kEventRawKeyDown:
            event.xany.type = KeyPress;
            Tk_QueueWindowEvent(&event, TCL_QUEUE_TAIL);
            break;
        case kEventRawKeyUp:
            event.xany.type = KeyRelease;
            Tk_QueueWindowEvent(&event, TCL_QUEUE_TAIL);
            break;
        case kEventRawKeyRepeat:
            event.xany.type = KeyRelease;
            Tk_QueueWindowEvent(&event, TCL_QUEUE_TAIL);
            event.xany.type = KeyPress;
            Tk_QueueWindowEvent(&event, TCL_QUEUE_TAIL);
            break;
        case kEventRawKeyModifiersChanged:
            if (savedModifiers > e->keyModifiers) {
                event.xany.type = KeyRelease;
            } else {
                event.xany.type = KeyPress;
            }
	    /* 
	     * Use special '-1' to signify a special keycode to
	     * our platform specific code in tkMacOSXKeyboard.c.
	     * This is rather like what happens on Windows.
	     */
            event.xany.send_event = -1;
	    /* Set keycode (which was zero) to the changed modifier */
            event.xkey.keycode = (e->keyModifiers ^ savedModifiers);
            Tk_QueueWindowEvent(&event, TCL_QUEUE_TAIL);
            break;
        default:
            break;
    } 
    return 1;
}

/*
 *----------------------------------------------------------------------
 *
 * XGrabKeyboard --
 *
 *        Simulates a keyboard grab by setting the focus.
 *
 * Results:
 *        Always returns GrabSuccess.
 *
 * Side effects:
 *        Sets the keyboard focus to the specified window.
 *
 *----------------------------------------------------------------------
 */

int
XGrabKeyboard(
    Display* display,
    Window grab_window,
    Bool owner_events,
    int pointer_mode,
    int keyboard_mode,
    Time time)
{
    gKeyboardWinPtr = Tk_IdToWindow(display, grab_window);
    return GrabSuccess;
}

/*
 *----------------------------------------------------------------------
 *
 * XUngrabKeyboard --
 *
 *        Releases the simulated keyboard grab.
 *
 * Results:
 *        None.
 *
 * Side effects:
 *        Sets the keyboard focus back to the value before the grab.
 *
 *----------------------------------------------------------------------
 */

void
XUngrabKeyboard(
    Display* display,
    Time time)
{
    gKeyboardWinPtr = NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * TkMacOSXGetCapture --
 *
 * Results:
 *      Returns the current grab window
 * Side effects:
 *        None.
 *
 */
Tk_Window
TkMacOSXGetCapture()
{
    return gGrabWinPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * TkpSetCapture --
 *
 *        This function captures the mouse so that all future events
 *        will be reported to this window, even if the mouse is outside
 *        the window.  If the specified window is NULL, then the mouse
 *        is released. 
 *
 * Results:
 *        None.
 *
 * Side effects:
 *        Sets the capture flag and captures the mouse.
 *
 *----------------------------------------------------------------------
 */

void
TkpSetCapture(
    TkWindow *winPtr)                        /* Capture window, or NULL. */
{
    while ((winPtr != NULL) && !Tk_IsTopLevel(winPtr)) {
        winPtr = winPtr->parentPtr;
    }
    gGrabWinPtr = (Tk_Window) winPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Tk_SetCaretPos --
 *
 *	This enables correct placement of the XIM caret.  This is called
 *	by widgets to indicate their cursor placement, and the caret
 *	location is used by TkpGetString to place the XIM caret.
 *
 * Results:
 *	None
 *
 * Side effects:
 *	None
 *
 *----------------------------------------------------------------------
 */

void
Tk_SetCaretPos(tkwin, x, y, height)
    Tk_Window tkwin;
    int	      x;
    int	      y;
    int	      height;
{
}
