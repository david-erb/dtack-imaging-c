/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* gnerror.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _GNError_HEADER_
#define _GNError_HEADER_

#include <iostream.h>

#ifdef INSTANTIATEGLOBALS
#define EXTERN
#else /* INSTANTIATEGLOBALS */
#define EXTERN extern 
#endif /* INSTANTIATEGLOBALS */

#define DEFAULTERRORBEHAVIOUR	GNErrorVERBOSE
#define DEFAULTERRORSTREAM	(&cerr)

enum GNErrorBehaviour {GNErrorIGNORE,GNErrorVERBOSE};

class GNError {
	enum GNErrorBehaviour behaviour;
	ostream *ost;
public:
	GNError(void)
		{
			behaviour=DEFAULTERRORBEHAVIOUR;
			ost=DEFAULTERRORSTREAM;
		}
	GNError(enum GNErrorBehaviour b)
		{
			behaviour=b;
			ost=DEFAULTERRORSTREAM;
		}
	GNError(enum GNErrorBehaviour b,ostream *o)
		{
			behaviour=b;
			ost=o;
		}
	GNError(ostream *o)
		{
			behaviour=DEFAULTERRORBEHAVIOUR;
			ost=o;
		}
	
	void error(char *comment)
		{
			switch (behaviour) {
				case GNErrorIGNORE:
					break;
				case GNErrorVERBOSE:
					*ost << comment << "\n" << flush;
					break;
			}
		}

	void setIgnore(void)	{ behaviour=GNErrorIGNORE; }
	void setVerbose(void)	{ behaviour=GNErrorVERBOSE; }
};

EXTERN class GNError GNErrorGlobal;

#undef DEFAULTERRORBEHAVIOUR
#undef DEFAULTERRORSTREAM

#endif /* _GNError_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
