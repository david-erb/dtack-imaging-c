/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* GetOpt.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */


#ifndef _GetOpt_HEADER_
#define _GetOpt_HEADER_

class GetOpt {
	int 		Argc;
	char **		Argv;
	const char *	Optstring;
	int		Optind;
public:
	char *	optarg;		// Single or first argument returned here
	int	optind;		// Index of next entry to be processed
				// Changes by user are IGNORED TOTALLY
	int	optnum;		// Number of arguments returned
	int	opterr;		// Caller sets to zero to keep quiet on error

	GetOpt(int argc,char **argv,const char *optstring);

	int	operator()(void);	// Returns next option or -1
};

// The option string:
//
//	c	- expect "-c"
//	c:	- expect "-c argument" or "-cargument"
//	c::	- expect "-c [argument]"	ie. optional argument
//	c[n]	- expect "-c arg0 ... argn-1"	ie. sequence of n arguments
//
// The option must be a single character, ie. "-cxxx" causes error unless
// expecting arguments, in which cases "xxx" is taken as the (first) arg.
//
// When a single arg is required, a pointer to its start (which may be part
// way through the option argv[]) is returned in optarg. If multiple args
// were required, the second arg is indexed by optind. These will automagically
// be skipped on the next operator()(void) call. If not enough arguments were
// found, then an error occurs. If an argument is expected and anything starting
// with '-' is found, an error occurs.
//
// A new public int optnum returns the number of arguments found
//
// Encountering '-' alone returns '-' but doesn't stop ? is this right ? :(
//
// Encountering the end of argv or a non-option when an argument is
// not expected or "--" alone causes return with EOF and optind set
// pointing at the next meaningful entry (or == argc if no more left)
//
// Errors are flagged by the return of '?' and an error message is printed
// unless opterr has been set to zero
//
// This routine ONLY HANDLES ONE OPTION PER ARGV COMPONENT at present :(

#endif /* _GetOpt_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
