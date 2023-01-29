/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)Month.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }
#include "Month.h"

unsigned short
MonthMMMtoMM(const char *mmm)
{
	unsigned short month=0;
	switch (tolower(mmm[0])) {
		case 'a':
			switch (tolower(mmm[1])) {
				case 'p':	month=4; break;
				case 'u':	month=8; break;
			}
			break;
		case 'd':	month=12; break;
		case 'f':	month=2; break;
		case 'j':
			switch (tolower(mmm[1])) {
				case 'a':	month=1; break;
				case 'u':
					switch (tolower(mmm[2])) {
						case 'l':
							month=7;
							break;
						case 'n':
							month=6;
							break;
					}
					break;
			}
			break;
		case 'm':
			switch (tolower(mmm[2])) {
				case 'r':	month=3; break;
				case 'y':	month=5; break;
			}
			break;
		case 'n':	month=11; break;
		case 'o':	month=10; break;
		case 's':	month=9; break;
	}
	return month;
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
