/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)GetOpt.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }


#include <iostream.h>
#include <stdlib.h>		// for atoi()
#include <string.h>		// for strchr()
//#include <strings.h>		// for index()

#include "GetOpt.h"

GetOpt::GetOpt(int argc,char *argv[],const char *optstring)
{
	Argc=argc;
	Argv=argv;
	Optstring=optstring;
	Optind=1;
	opterr=1;
}

int
GetOpt::operator()(void)
{
	int rvalue;
	if (Optind >= Argc) {
		rvalue=-1;
		optind=Optind;
	}
	else {
		char *ptr;
		ptr=Argv[Optind];
		if (*ptr == '-') {
			++ptr;
			if (*ptr == '-') {
				++ptr;
				if (*ptr == 0) {	// "--" means stop
					rvalue=-1;
				}
				else {			// "--something" :(
					if (opterr)
						 cerr << Argv[0]
						      << ": characters present after '--'\n" << flush;
					rvalue='?';	// Error
				}
				++Optind;
				optind=Optind;
			}
			else {
				if (*ptr == 0) {	// "-" alone
					rvalue='-';	// ??????????????? :( ? should be error ?
					++Optind;	// skip the "-"
					optind=Optind;
				}
				else {			// "-something"
					const char *isopt=strchr(Optstring,*ptr);
					if (isopt) {
						int argn;
						int argreqd;
						switch(*(isopt+1)) {
							case ':':
								if (*(isopt+2) == ':') {
									argn=1;		// Optional argument
									argreqd=0;
								}
								else {
									argn=1;		// Mandatory argument
									argreqd=1;
								}
								break;
							case '[':
								argn=atoi(isopt+2);	// Not checking for end ']'
								if (argn < 0) argn=0;	// Just in case !!!
								argreqd=argn;
								break;
							default:
								argn=0;			// No arguments
								argreqd=0;
								break;
						}
						optnum=0;
						optarg=0;
						rvalue=*ptr;
						if (*(ptr+1)) {
							if (argn) {	// get an argument from the option
								optarg=ptr+1;
								++optnum;
								--argn;
								if (argreqd) --argreqd;
							}
							else {		// "-osomething" and no args wanted !
								if (opterr)
						 			cerr << Argv[0]
								    	 << ": option is too long '-"
								    	 << *ptr << "'\n" << flush;
								rvalue='?';
							}
						}
						++Optind;
						optind=Optind;
						while (argn && Optind<Argc && *Argv[Optind] != '-') {
							if (!optarg) {
								optarg=Argv[Optind];	// The "first" argument
								optind=Optind+1;	// The "second" argument
							}
							++Optind;
							++optnum;
							--argn;
							if (argreqd) --argreqd;
						}
						if (argreqd) {
							if (opterr)
						 		cerr << Argv[0]
								     << ": insufficient arguments for '-"
								     << *ptr << "'\n" << flush;
							rvalue='?';	// Error - not enough args
						}
					}
					else {
						if (opterr)
						 	cerr << Argv[0]
							     << ": unrecognized option '-"
							     << *ptr << "'\n" << flush;
						rvalue='?';
						++Optind;
						optind=Optind;
					}
				}
			}
		}
		else {			// "something" so stop
			rvalue=-1;
			optind=Optind;
		}
	}
	return rvalue;
}






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
