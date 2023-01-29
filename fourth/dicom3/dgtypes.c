/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)dgtypes.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <strstream.h>
#include <iomanip.h>

#include "dgtypes.h"

/* ************************ Data General Methods ************************ */

void
DGR::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGR::read seek failed\n" << flush;
	}
	else {
		typedef struct {
			unsigned	sign : 1;
			unsigned	exponent : 7;
			unsigned	mantissa : 24;
		} DG_FLOAT;

		DG_FLOAT number;

		unsigned char buffer[4];
		instream.read(buffer,4);
		if (instream) {
			// DataGeneral is a Big Endian machine
			memcpy ((char *)(&number),buffer,4);
			sign     = number.sign;
			exponent = number.exponent;
			mantissa = number.mantissa;

			value = (double) mantissa / (1 << 24) *
				pow (16.0, (long)(exponent) - 64);
			value = (sign == 0) ? value : -value;
		}
		else {
			cerr << "DGR::read read failed\n" << flush;
			value=0;
		}
	}	
}

char *
DGR::toString(void)	
{
// Different ios libraries handle the precision differently:
// 	SC says - the default precision is 6, but this is 6 non-zero digits
//		  ie. 0.0320512 not 0.032051
//		- turning on setiosflags(ios::fixed) ALWAYS outputs 6 digits
//		  even if lsb are zero, eg. 1.500000, though it now does
//		  0.032051 not 0.0320512
//		- presumably (ios::showpoint) is on by default
//		- resetiosflags(ios::showpoint) doesn't make trailing 0 go :(
//
// 	GNU g++ says same as SC:
//		- the default precision is 6, but this is 6 non-zero digits
//		  ie. 0.0320512 not 0.032051
//		- turning on setiosflags(ios::fixed) ALWAYS outputs 6 digits
//		  even if lsb are zero, eg. 1.500000, though it now does
//		  0.032051 not 0.0320512
//		- presumably (ios::showpoint) is on by default
//		- resetiosflags(ios::showpoint) doesn't make trailing 0 go :(
//
//      Symantec says:
// 		- the default precision is 6, but this is 6 including 0
//		  ie. 0.032051 not 0.0320512
//		- turning on setiosflags(ios::fixed) DOESN'T ALWAYS output 6
//		  ie. 1.5 is 1.5 not 1.500000
//		- presumably (ios::showpoint) is not on by default
//		- turning it on makes 1.5 into 1.500000
//
// This combination makes comparing the binary output feasible:
//
	ostrstream ost(sh,shmax);
	ost << dec << setiosflags(ios::fixed) << setiosflags(ios::showpoint)
	    << setprecision(6) << value << '\0';
	return sh;
}

void
DGI::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGI::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// DataGeneral is a Big Endian machine
		u =  (Uint16)buffer[0];
		u <<= 8;
		u |= (Uint16)buffer[1];
		if (!instream) {
			cerr << "DGI::read read failed\n" << flush;
		}
	}	
}

char *
DGI::toString(void)	
{
	ostrstream ost(sh,shmax);
	ost << dec << u << '\0';
	return sh;
}

void
DGI4::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGI::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// DataGeneral is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "DGI::read read failed\n" << flush;
		}
	}	
}

char *
DGI4::toString(void)	
{
	ostrstream ost(sh,shmax);
	ost << dec << u << '\0';
	return sh;
}

void
DGDD::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGDD::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[6];
		instream.read(buffer,6);
		// DataGeneral is a Big Endian machine
		month =  (Uint16)buffer[0];
		month <<= 8;
		month |= (Uint16)buffer[1];
		day   =  (Uint16)buffer[2];
		day   <<= 8;
		day   |= (Uint16)buffer[3];
		year  =  (Uint16)buffer[4];
		year  <<= 8;
		year  |= (Uint16)buffer[5];
		if (!instream) {
			cerr << "DGDD::read read failed\n" << flush;
		}
	}	
}

char *
DGDD::toString(void)	
{
	ostrstream ost(sh,shmax);
	ost << dec << setfill('0')
	    << "19" << setw(2) << year
	    << setw(2) << month
	    << setw(2) << day
	    << '\0' << setfill(' ');
	return sh;
}

void
DGTT::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGTT::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[6];
		instream.read(buffer,6);
		// DataGeneral is a Big Endian machine
		hours =  (Uint16)buffer[0];
		hours <<= 8;
		hours |= (Uint16)buffer[1];
		mins   =  (Uint16)buffer[2];
		mins   <<= 8;
		mins   |= (Uint16)buffer[3];
		secs  =  (Uint16)buffer[4];
		secs  <<= 8;
		secs  |= (Uint16)buffer[5];
		if (!instream) {
			cerr << "DGTT::read read failed\n" << flush;
		}
	}	
}

char *
DGTT::toString(void)	
{
	ostrstream ost(sh,shmax);
	ost << dec << setfill('0')
	    << setw(2) << hours
	    << setw(2) << mins
	    << setw(2) << secs
	    << '\0' << setfill(' ');
	return sh;
}

void
DGA::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGA::read seek failed\n" << flush;
	}
	else {
		if (str) {
			instream.read(str,inlength);
			str[inlength]='\0';	// Just in case
			if (!instream) {
				cerr << "DGA::read read failed\n" << flush;
			}
		}
		else {
			cerr << "DGA::read string not allocated\n" << flush;
		}
	}	
}

void
DGA2::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block*512+offset*2,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "DGA2::read seek failed\n" << flush;
	}
	else {
		if (str) {
			instream.read(str,inlength*2);
			str[inlength*2]='\0';	// Just in case
			if (!instream) {
				cerr << "DGA2::read read failed\n" << flush;
			}
		}
		else {
			cerr << "DGA2::read string not allocated\n" << flush;
		}
	}	
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
