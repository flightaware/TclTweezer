/*
 * tcltweezer.c --
 *
 * $Id: tcltweezer.c,v 1.3 2004/12/01 16:28:17 karl Exp $
 *
 *
 */

#include <tcl.h>
#include "tweezer.h"


/*
 *----------------------------------------------------------------------
 *
 * Tweezer_Init --
 *
 *	Initialize the Tweezer.  The string "Tweezer" in the
 *	function name must match the PACKAGE declaration at the top of
 *	configure.in.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The tweezer package is created.
 *	One new command "tweezer" is added to the Tcl interpreter.
 *
 *----------------------------------------------------------------------
 */

int
Tweezer_Init(Tcl_Interp *interp)
{
    /*
     * This may work with 8.0, but we are using strictly stubs here,
     * which requires 8.1.
     */
    if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
	return TCL_ERROR;
    }

    if (Tcl_PkgRequire(interp, "Tcl", "8.1", 0) == NULL) {
	return TCL_ERROR;
    }

    if (Tcl_PkgProvide(interp, "tweezer", PACKAGE_VERSION) != TCL_OK) {
	return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "tweezer", (Tcl_ObjCmdProc *) sc_TweezerObjCmd,
	    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    return TCL_OK;
}


/*
 *----------------------------------------------------------------------
 *
 * Tweezer_SafeInit --
 *
 *	Initialize the Tweezer in a safe interpreter.
 *
 *      We think the tweezer may be a bit dangerous for safe interpreters,
 *      so we don't provide it.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	Very little
 *
 *----------------------------------------------------------------------
 */

int
Tweezer_SafeInit(Tcl_Interp *interp)
{
    return TCL_OK;
}

