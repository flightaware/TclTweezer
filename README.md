This is Tweezer, a scanning tunneling electron microscope for Tcl objects.

Tweezer is a hackish tool for doing dangerous but potentially interesting things to Tcl objects... things that may surprise and delight...  or cause your interpreter to dump core...

Using Tweezer you can sniff any Tcl object that you can contrive for the tweezer to be able to see, such as a variable, proc, array element, element of a list within a list, etc, find out its data type (internal representation), look at and manipulate its reference count, generate a new reference, and even force an object to shimmer to a different internal representation, which may actually be useful for something.

It kind of lifts the veil on the objects that underly Tcl, that are normally invisible to Tcl programs.

This extension is mainly for the purpose of experimentation.  However it can be used to compile procs without executing them, which could be really useful.

I strongly recommend against using it for anything real unless you are absolutely sure about what you're doing.

I welcome suggestions on how to improve this tool and creative flames on what a horrible abomination it is.

This package is a freely available open source package.  You can do virtually anything you like with it, such as modifying it, redistributing it, and selling it either in whole or in part.  See the file "license.terms" for complete information.

Tweezer was written by Karl Lehenbauer.

Using the Tweezer
---

```tcl
package require tweezer

% set a 10
% tweezer type $a
int

% tweezer shimmer $a double
% tweezer type $a
double
% puts $a
10
% tweezer type $a
double

% set a "a b c d e f g h i j k l m n o p"
a b c d e f g h i j k l m n o p
% tweezer type $a
% lindex $a 4
e
% tweezer type $a
list
% tweezer shimmer [lindex $a 4] list
% tweezer type [lindex $a 4]
list


% proc die {} {if}
% tweezer shimmer [info body die] bytecode
wrong # args: no expression after "if" argument

% tweezer types
boolean index double end-offset wideInt list cmdName bytecode nsName procbody bytearray int {array search} string

foreach proc [info procs] {
    tweezer convert [info body $proc] bytecode
}
```

...pretty cool, huh?

Tweezer Quick Reference
---

* **tweezer types**

    Return a list of all of the defined object types within Tcl.

* **tweezer type** *object*

    Return the internal data type of an object.  If the object has no current internal representation, return nothing.
    
    To see the data type of a variable, reference the variable's contents

```tcl
tweezer type $var

tweezer type $array(element)
```
 To see a proc body, use info body...

```tcl
tweezer type [info body myproc]
```

 To see the type of the fourth element of a list

```tcl
tweezer type [lindex $list 3]
```

* **tweezer shimmer** *object* *type*

 This shimmers an object to a different type.  For instance to force the fifth element of a list to be an integer

```tcl
    tweezer shimmer [lindex $list 4] int
```

 Attempting to shimmer objects to strange object types can cause coredumps.

 The name shimmer is possibly dumb.  It might be reasonable to just have "tweezer type" set the type if 4 arguments are present.

* **tweezer refcount** *object*

 Return the object's reference count.

* **tweezer makeref** *object*

 Create and return a reference to an object without incrementing the object's reference count.  Dangerous.

* **tweezer duplicate** *object*

 Duplicate an object and return the new object.  This shouldn't be dangerous but may be dumb.

* **tweezer incr_refcount** *object*

 ...increment an object's reference count.  Stupid name.

* **tweezer decr_refcount** *object*

 Decrement an object's reference count.  Another stupid name plus dangerous.

Bugs
---

After changing the type of a proc body to bytecode, it doesn't show up as bytecode.  This used to work, maybe in earlier versions of Tcl 8.  It does still appear to force compilation of the proc.

Contents
---

Makefile.in	Makefile template.  The configure script uses this file to
		produce the final Makefile.

README		This file

aclocal.m4	Generated file.  Do not edit.  Autoconf uses this as input
		when generating the final configure script.  See "tcl.m4"
		below.

configure	Generated file.  Do not edit.  This must be regenerated
		anytime configure.in or tclconfig/tcl.m4 changes.

configure.in	Configure script template.  Autoconf uses this file as input
		to produce the final configure script.

generic/tweezer.c	Tcl object tweezer and interface source code
generic/tcltweezer.c

generic/tweezer.h	Header files for the tweezer.


tclconfig/	This directory contains various template files that build
		the configure script.  They should not need modification.

	install-sh	Program used for copying binaries and script files
			to their install locations.

	tcl.m4		Collection of Tcl autoconf macros.  Included by
			aclocal.m4 to define SC_* macros.

UNIX build
---

Building under most UNIX systems is easy, just run the configure script
and then run make. 

```sh
cd tweezer
./configure
make
make install
```

Windows build
---

Nobody has yet attempted to build the tweezer under Windows.

The recommended method to build extensions under windows is to use the Msys + Mingw build process. This provides a Unix-style build while generating native Windows binaries. Using the Msys + Mingw build tools means that you can use the same configure script as per the Unix build to create a Makefile.

If you have VC++ then you may wish to use the files in the win subdirectory and build the extension using just VC++. These files have been designed to be as generic as possible but will require some additional maintenance by the project developer to synchronise with the TEA configure.in and Makefile.in files. Instructions for using the VC++ makefile are written in the first part of the Makefile.vc file.

Installation
---

The tweezers install like so:

         $exec_prefix
          /       \
        lib       bin
         |         |
   PACKAGEx.y   (dependent .dll files on Windows)
         |
  pkgIndex.tcl (.so|.dll files)

The main .so|.dll library file gets installed in the versioned PACKAGE directory, which is OK on all platforms because it will be directly referenced with by 'load' in the pkgIndex.tcl file.  Dependent DLL files on Windows must go in the bin directory (or other directory on the user's PATH) in order for them to be found.
