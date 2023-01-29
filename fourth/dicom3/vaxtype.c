/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)vaxtype.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <strstream.h>
#include <iomanip.h>

#include "vaxtype.h"

/* ************************ Vax Methods ************************ */

void
VAX_float_f::read(ifstream& instream,unsigned block,unsigned offset)
{
	// cerr << "float::read - start\n" << flush;
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_float_f::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		if (instream) {
			typedef struct {
				unsigned	mantissalow : 16;
				unsigned	sign : 1;
				unsigned	exponent : 8;
				unsigned	mantissahigh : 7;
			} VAX_FLOAT_F;

			VAX_FLOAT_F number;

			// Vax is a Little Endian machine ...

			{ unsigned char *numberp=(unsigned char *)&number; int i=4; while (i) *numberp++=buffer[--i]; }

			unsigned char	sign             = number.sign;
			Uint16		exponent         = number.exponent;
			Uint16		mantissalow      = number.mantissalow;
			Uint16		mantissahigh     = number.mantissahigh;
			double		mantissavalue    = (double)mantissahigh * pow(2.0,(long)16) + (double)mantissalow;
			double		mantissafraction = mantissavalue/pow(2.0,(long)23);

			//cerr	<< "sign is " << dec << (unsigned)sign << "\n";
			//cerr	<< "exponent is " << dec << exponent << "\n";
			//cerr	<< "mantissalow is 0x" << hex << mantissalow << "\n";
			//cerr	<< "mantissahigh is 0x" << hex << mantissahigh << "\n";
			//cerr	<< "mantissavalue is " << dec << mantissavalue << "\n";
			//cerr	<< "mantissafraction is " << dec << mantissafraction << "\n";

			if (exponent == 0 && mantissalow == 0 && mantissahigh == 0) {
					// Zero
					value=0;
			}
			else {
				// Always normalized ... the 1.0 is the "hidden" bit in VaxSpeak
				// the exponent is excess 128, eg 10000010 is +2
				// but refers to a binary point to the LEFT of the hidden bit
				// hence the -129 rather than -128
				value=(1.0+mantissafraction)*pow(2.0,(long)(exponent)-129);
				value = (sign == 0) ? value : -value;
			}

			//cerr << "float::read - value=" << value << "\n"
			//      << flush;
		}
		else {
			cerr << "VAX_float_f::read read failed\n" << flush;
			value=0;
		}
	}	
}

char *
VAX_float_f::toString(void)	
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
	ostrstream ost;
	ost << dec << setiosflags(ios::fixed) << setiosflags(ios::showpoint)
	    << setprecision(6) << value << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_byte::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_byte::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[1];
		instream.read(buffer,1);
		u =  (signed char)buffer[0];
		if (!instream) {
			cerr << "VAX_byte::read read failed\n" << flush;
		}
	}	
}

char *
VAX_byte::toString(void)	
{
	ostrstream ost;
	ost << dec << (signed short)u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_u_byte::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_u_byte::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[1];
		instream.read(buffer,1);
		u =  (unsigned char)buffer[0];
		if (!instream) {
			cerr << "VAX_u_byte::read read failed\n" << flush;
		}
	}	
}

char *
VAX_u_byte::toString(void)	
{
	ostrstream ost;
	ost << dec << (unsigned short)u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_short::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_short::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// Vax is a Little Endian machine
		u =  (Uint16)buffer[1];
		u <<= 8;
		u |= (Uint16)buffer[0];
		if (!instream) {
			cerr << "VAX_short::read read failed\n" << flush;
		}
	}	
}

char *
VAX_short::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_u_short::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_u_short::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// Vax is a Little Endian machine
		u =  (Uint16)buffer[1];
		u <<= 8;
		u |= (Uint16)buffer[0];
		if (!instream) {
			cerr << "VAX_u_short::read read failed\n" << flush;
		}
	}	
}

char *
VAX_u_short::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_int::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_int::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Vax is a Little Endian machine
		u =  (Uint32)buffer[3];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[0];
		if (!instream) {
			cerr << "VAX_int::read read failed\n" << flush;
		}
	}	
}

char *
VAX_int::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_u_int::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_u_int::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Vax is a Little Endian machine
		u =  (Uint32)buffer[3];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[0];
		if (!instream) {
			cerr << "VAX_u_int::read read failed\n" << flush;
		}
	}	
}

char *
VAX_u_int::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
VAX_char::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "VAX_char::read seek failed\n" << flush;
	}
	else {
		if (str) {
			instream.read(str,inlength);
			str[inlength]='\0';	// Just in case
			if (!instream) {
				cerr << "VAX_char::read read failed\n"
				     << flush;
			}
		}
		else {
			cerr << "VAX_char::read string not allocated\n"
			     << flush;
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
