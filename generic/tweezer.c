/*
 * Fun with Objects
 *
 * Copyright (C) 2000, 2004 by Karl Lehenbauer, All Rights Reserved
 *
 * Freely redistributable under the Tcl license.
 *
 */

#include <tcl.h>


/*
 *----------------------------------------------------------------------
 *
 * sc_TweezerObjCmd --
 *
 *	This procedure is invoked to process the "tweezer" command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
sc_TweezerObjCmd(dummy, interp, objc, objv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int objc;				/* Number of arguments. */
    Tcl_Obj   *CONST objv[];
{
    Tcl_Obj *objp;
    Tcl_Obj *resultPtr = Tcl_GetObjResult (interp);
    int      optIndex;
    char          *typeName;
    Tcl_ObjType   *typeObjType;


    static CONST char *options[] = {
        "types", "type", "shimmer", "refcount", "makeref", "duplicate", "incr_refcount", "decr_refcount", (char *)NULL
    };

    enum options
    {
        OPT_TYPES, OPT_TYPE, OPT_SHIMMER, OPT_REFCOUNT, OPT_MAKEREF, OPT_DUPLICATE, OPT_INCRREF, OPT_DECRREF
    };

    if (objc < 2 || objc > 4) {
      err:
	Tcl_WrongNumArgs (interp, 1, objv, "subcommand ?object? ?args?");
	return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj(interp, objv[1], options, "option", TCL_EXACT,
        &optIndex) != TCL_OK)
    {
	    return TCL_ERROR;
    }

    switch ((enum options) optIndex)
    {
        case OPT_TYPE:
	    objp = objv[2];

	    if (objc != 3) goto err;

	    /* if typePtr is null, the object has no internal representation */
	    if (objp->typePtr == NULL) {
		return TCL_OK;
	    }

	    /* typePtr is not null, return the name of the type of the
	     * object's cached internal representation */
	    Tcl_SetStringObj (resultPtr, objp->typePtr->name, -1);
	    return TCL_OK;

	case OPT_TYPES:
	    if (objc != 2) goto err;

	    /* return a list of all of the types.  wasn't this C API handy? */
	    return (Tcl_AppendAllObjTypes (interp, resultPtr));

	case OPT_REFCOUNT:
	    if (objc != 3) goto err;

	    objp = objv[2];
	    Tcl_SetIntObj (resultPtr, objp->refCount);
	    return TCL_OK;

	case OPT_SHIMMER:
	    objp = objv[2];

	   if (objc != 4) goto err;

	   /* get the name of the type they want to shimmer the object
	    * to and locate the matching object type */
	   typeName = Tcl_GetStringFromObj (objv [3], NULL);

	   if ((typeObjType = Tcl_GetObjType (typeName)) == NULL) {
	       Tcl_AddObjErrorInfo (interp, "unknown type '", -1);
	       Tcl_AddObjErrorInfo (interp, typeName, -1);
	       Tcl_AddObjErrorInfo (interp, "'", 1);
	       return TCL_ERROR;
	   }

	   /* attempt the type conversion.  if it fails, return an error */
	   if (Tcl_ConvertToType (interp, objp, typeObjType) == TCL_ERROR) {
	       Tcl_AddObjErrorInfo (interp, "while shimmering type", 1);
	       return TCL_ERROR;
	   }

	   /* type conversion succeeded */
	   return TCL_OK;

	case OPT_MAKEREF:
	    if (objc != 3) goto err;

	    /* return a reference to an object.  dangerous. */
	    Tcl_SetObjResult (interp, objv[2]);
	    return TCL_OK;

	case OPT_DUPLICATE:
	    if (objc != 3) goto err;

	    /* duplicate object */
	    Tcl_SetObjResult (interp, Tcl_DuplicateObj (objv[2]));
	    return TCL_OK;

	case OPT_INCRREF:
	    if (objc != 3) goto err;
	    objp = objv[2];

	    /* increment reference count and return it */
	    Tcl_IncrRefCount (objp);
	    Tcl_SetIntObj (resultPtr, objp->refCount);
	    return TCL_OK;


	case OPT_DECRREF:
	    if (objc != 3) goto err;
	    objp = objv[2];

	    /* decrement reference count and return it.  very dangerous. */
	    Tcl_DecrRefCount (objp);
	    Tcl_SetIntObj (resultPtr, objp->refCount);
	    return TCL_OK;
    }

    panic("never expected to get here");
    return TCL_ERROR;
}

