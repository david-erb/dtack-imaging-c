/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)syttype.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <strstream.h>
#include <iomanip.h>

#include "syttype.h"

/* ************************ Sytec Methods ************************ */

void
SYTEC_float32::read(ifstream& instream,unsigned block,unsigned offset)
{
	// cerr << "float::read - start\n" << flush;
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_float32::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		if (instream) {
			unsigned char	sign;
			Uint16		exponent;
			Uint32		mantissa;

			typedef struct {
				unsigned	sign : 1;
				unsigned	exponent : 8;
				unsigned	mantissa : 23;
			} IEEE_FLOAT_SINGLE;

			IEEE_FLOAT_SINGLE number;
			// Sytec is a Big Endian machine
			memcpy ((char *)(&number),buffer,4);
			sign     = number.sign;
			exponent = number.exponent;
			mantissa = number.mantissa;

			if (exponent == 0) {
				if (mantissa == 0) {
					// Zero
					value=0;
				}
				else {
					// Not normalized
					value=((double)mantissa/(1<<23))*pow(2.0,(long)-126);
					value = (sign == 0) ? value : -value;
				}
			}
			else if (exponent == 255) {
				if (mantissa) {
					// Not a number ... use infinity anyway
					value=HUGE_VAL;		// POSIX, ? use infinity() for SYSV ?
				}
				else {
					// Infinity
					value=HUGE_VAL;		// POSIX, ? use infinity() for SYSV ?
				}
			}
			else {
				// Normalized
				value=(1.0+(double)mantissa/(1<<23))*pow(2.0,(long)(exponent)-127);
				value = (sign == 0) ? value : -value;
			}

			// cerr << "float::read - value=" << value << "\n"
			//      << flush;
		}
		else {
			cerr << "SYTEC_float32::read read failed\n" << flush;
			value=0;
		}
	}	
}

char *
SYTEC_float32::toString(void)	
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
	// cerr << "float::toString - start\n" << flush;
	ostrstream ost;
	// cerr << "float::toString - filling\n" << flush;
	ost << dec << setiosflags(ios::fixed) << setiosflags(ios::showpoint)
	    << setprecision(6) << value << '\0';
	// cerr << "float::toString - going to delete\n" << flush;
	if (sh) delete sh;
	// cerr << "float::toString - going to freeze\n" << flush;
	sh=ost.str();
	// cerr << "float::toString - end\n" << flush;
	return sh;
}

void
SYTEC_int16::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_int16::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// Sytec is a Big Endian machine
		u =  (Uint16)buffer[0];
		u <<= 8;
		u |= (Uint16)buffer[1];
		if (!instream) {
			cerr << "SYTEC_int16::read read failed\n" << flush;
		}
	}	
}

char *
SYTEC_int16::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
SYTEC_u_int16::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_u_int16::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// Sytec is a Big Endian machine
		u =  (Uint16)buffer[0];
		u <<= 8;
		u |= (Uint16)buffer[1];
		if (!instream) {
			cerr << "SYTEC_u_int16::read read failed\n" << flush;
		}
	}	
}

char *
SYTEC_u_int16::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
SYTEC_int32::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_int32::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Sytec is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "SYTEC_int32::read read failed\n" << flush;
		}
	}	
}

char *
SYTEC_int32::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
SYTEC_u_int32::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_u_int32::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Sytec is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "SYTEC_u_int32::read read failed\n" << flush;
		}
	}	
}

char *
SYTEC_u_int32::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
SYTEC_string::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_string::read seek failed\n" << flush;
	}
	else {
		if (str) {
			instream.read(str,inlength);
			str[inlength]='\0';	// Just in case
			if (!instream) {
				cerr << "SYTEC_string::read read failed\n"
				     << flush;
			}
		}
		else {
			cerr << "SYTEC_string::read string not allocated\n"
			     << flush;
		}
	}	
}

void
SYTEC_byte::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_byte::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[1];
		instream.read(buffer,1);
		u =  (Uint16)buffer[0];
		if (!instream) {
			cerr << "SYTEC_u_int16::read read failed\n" << flush;
		}
	}	
}

char *
SYTEC_byte::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
SYTEC_datetime::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "SYTEC_datetime::read seek failed\n" << flush;
	}
	else {
		Uint32 u;
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Sytec is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "SYTEC_datetime::read read failed\n" << flush;
		}
		else {
			#define isleap(year) (year%4 == 0 && year%20 != 0)

			#define SecondsPerYear(year)	(isleap(year)? \
				SecondsPerLeapYear:SecondsPerNormalYear)

			#define SecondsPerMonth(year)	(isleap(year)? \
				SecondsPerMonthLeap:SecondsPerMonthNormal)

			const unsigned long	SecondsPerNormalYear= \
							60*60*24*365;
			const unsigned long	SecondsPerLeapYear  = \
							60*60*24*366;
			const unsigned long	SecondsPerDay=60*60*24;
			const unsigned long	SecondsPerHour=60*60;
			const unsigned long	SecondsPerMinute=60;

			static unsigned SecondsPerMonthNormal [13] = {
				0,
				31*SecondsPerDay,
				28*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay
			};
			static unsigned SecondsPerMonthLeap [13] = {
				0,
				31*SecondsPerDay,
				29*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay,
				30*SecondsPerDay,
				31*SecondsPerDay
			};

			// epoch is 00:00:00 GMT 1 Jan 1970

			SecondsSinceEpoch=u;
			Seconds = SecondsSinceEpoch;
			Year 	= 1970;
			while (Seconds >= SecondsPerYear(Year)) {
				Seconds-=SecondsPerYear(Year);
				++Year;
			}

			Month = 0;
			while (Seconds >= SecondsPerMonth(Year)[Month]) {
				Seconds-=SecondsPerMonth(Year)[Month];
				++Month;
			}

			Day=Seconds/SecondsPerDay;
			Seconds-=Day*SecondsPerDay;
			Hours=Seconds/SecondsPerHour;
			Seconds-=Hours*SecondsPerHour;
			Minutes=Seconds/SecondsPerMinute;
			Seconds-=Minutes*SecondsPerMinute;
		}
	}	
}

char *
SYTEC_datetime::toDA(void)	
{
	ostrstream ost;
	ost << dec << setfill('0')
	    << setw(4) << Year
	    << setw(2) << Month
	    << setw(2) << Day
	    << '\0' << setfill(' ');
	if (dash) delete dash;
	dash=ost.str();
	return dash;
}

char *
SYTEC_datetime::toTM(void)	
{
	ostrstream ost;
	ost << dec << setfill('0')
	    << setw(2) << Hours
	    << setw(2) << Minutes
	    << setw(2) << Seconds
	    << '\0' << setfill(' ');
	if (tmsh) delete tmsh;
	tmsh=ost.str();
	return tmsh;
}

char *
SYTEC_datetime::toDT(void)	
{
	ostrstream ost;
	ost << dec << setfill('0')
	    << setw(4) << Year
	    << setw(2) << Month
	    << setw(2) << Day
	    << setw(2) << Hours
	    << setw(2) << Minutes
	    << setw(2) << Seconds
	    << '\0' << setfill(' ');
	if (dtsh) delete dtsh;
	dtsh=ost.str();
	return dtsh;
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
