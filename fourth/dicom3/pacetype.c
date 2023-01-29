/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)pacetype.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <strstream.h>
#include <iomanip.h>

#include "pacetype.h"

/* ************************ Pace Methods ************************ */

void
PACE_float32::read(ifstream& instream,unsigned block,unsigned offset)
{
	// cerr << "float::read - start\n" << flush;
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_float32::read seek failed\n" << flush;
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
			// Pace is a Big Endian machine
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
			cerr << "PACE_float32::read read failed\n" << flush;
			value=0;
		}
	}	
}

char *
PACE_float32::toString(void)	
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
PACE_int16::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_int16::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// Pace is a Big Endian machine
		u =  (Uint16)buffer[0];
		u <<= 8;
		u |= (Uint16)buffer[1];
		if (!instream) {
			cerr << "PACE_int16::read read failed\n" << flush;
		}
	}	
}

char *
PACE_int16::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
PACE_u_int16::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_u_int16::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[2];
		instream.read(buffer,2);
		// Pace is a Big Endian machine
		u =  (Uint16)buffer[0];
		u <<= 8;
		u |= (Uint16)buffer[1];
		if (!instream) {
			cerr << "PACE_u_int16::read read failed\n" << flush;
		}
	}	
}

char *
PACE_u_int16::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
PACE_int32::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_int32::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Pace is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "PACE_int32::read read failed\n" << flush;
		}
	}	
}

char *
PACE_int32::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
PACE_u_int32::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_u_int32::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Pace is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "PACE_u_int32::read read failed\n" << flush;
		}
	}	
}

char *
PACE_u_int32::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
PACE_string::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_string::read seek failed\n" << flush;
	}
	else {
		if (str) {
			instream.read(str,inlength);
			str[inlength]='\0';	// Just in case
			if (!instream) {
				cerr << "PACE_string::read read failed\n"
				     << flush;
			}
		}
		else {
			cerr << "PACE_string::read string not allocated\n"
			     << flush;
		}
	}	
}

void
PACE_byte::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_byte::read seek failed\n" << flush;
	}
	else {
		unsigned char buffer[1];
		instream.read(buffer,1);
		u =  (Uint16)buffer[0];
		if (!instream) {
			cerr << "PACE_u_int16::read read failed\n" << flush;
		}
	}	
}

char *
PACE_byte::toString(void)	
{
	ostrstream ost;
	ost << dec << u << '\0';
	if (sh) delete sh;
	sh=ost.str();
	return sh;
}

void
PACE_string_date::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_string_date::read seek failed\n" << flush;
	}
	else {
		instream.read(u.str,10);
		u.date.delim1='\0';
		u.date.delim2='\0';
		u.date.terminator='\0';
		if (!instream) {
			cerr << "PACE_string_date::read read failed\n"
			     << flush;
		}
	}	
}

char *
PACE_string_date::toDA(void)	
{
	// str is of form yyyy/mm/dd

	ostrstream ost;
	ost << u.date.yyyy
	    << u.date.mm
	    << u.date.dd
	    << ends;
	if (dash) delete dash;
	dash=ost.str();
	return dash;
}

void
PACE_string_time::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_string_time::read seek failed\n" << flush;
	}
	else {
		instream.read(u.str,12);
		u.date.delim1='\0';
		u.date.delim2='\0';
		u.date.terminator='\0';
		if (!instream) {
			cerr << "PACE_string_time::read read failed\n"
			     << flush;
		}
	}	
}

char *
PACE_string_time::toTM(void)	
{
	// str is of form hh/mm/ss.xxx

	ostrstream ost;
	ost << u.date.hh
	    << u.date.mm
	    << u.date.ssxxx
	    << ends;
	if (dash) delete dash;
	dash=ost.str();
	return dash;
}

void
PACE_int_date::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_int_date::read seek failed\n" << flush;
	}
	else {
		Uint32 u;
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Pace is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "PACE_int_date::read read failed\n" << flush;
		}
		else {
			#define isleap(year) (year%4 == 0 && year%20 != 0)

			#define DaysPerYear(year)	(isleap(year)? \
				DaysPerLeapYear:DaysPerNormalYear)

			#define DaysPerMonth(year)	(isleap(year)? \
				DaysPerMonthLeap:DaysPerMonthNormal)

			const unsigned long	DaysPerNormalYear= 365;
			const unsigned long	DaysPerLeapYear  = 366;

			static unsigned DaysPerMonthNormal [13] = {
				0,
				31,
				28,
				31,
				30,
				31,
				30,
				31,
				31,
				30,
				31,
				30,
				31
			};
			static unsigned DaysPerMonthLeap [13] = {
				0,
				31,
				29,
				31,
				30,
				31,
				30,
				31,
				31,
				30,
				31,
				30,
				31
			};

			// epoch is 0 Jan 1980

			DaysSinceEpoch=u-1;
			unsigned long Days = DaysSinceEpoch;
			Year 	= 1980;
			while (Days >= DaysPerYear(Year)) {
				Days-=DaysPerYear(Year);
				++Year;
			}

			Month = 0;
			while (Days >= DaysPerMonth(Year)[Month]) {
				Days-=DaysPerMonth(Year)[Month];
				++Month;
			}

			Day=Days;
		}
	}	
}

char *
PACE_int_date::toDA(void)	
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

void
PACE_int_time::read(ifstream& instream,unsigned block,unsigned offset)
{
	if (instream.rdbuf()->seekoff
			(block+offset,ios::beg,ios::in|ios::out)
			== streampos(EOF)) {
		cerr << "PACE_int_time::read seek failed\n" << flush;
	}
	else {
		Uint32 u;
		unsigned char buffer[4];
		instream.read(buffer,4);
		// Pace is a Big Endian machine
		u =  (Uint32)buffer[0];
		u <<= 8;
		u |= (Uint32)buffer[1];
		u <<= 8;
		u |= (Uint32)buffer[2];
		u <<= 8;
		u |= (Uint32)buffer[3];
		if (!instream) {
			cerr << "PACE_int_time::read read failed\n" << flush;
		}
		else {
			// time seems to be in units of mS

			ticks=u;
			Milliseconds=ticks%1000;
			Seconds=(ticks/1000)%60;
			Minutes=(ticks/(1000*60))%60;
			Hours  =(ticks/(1000*60*60))%60;
		}
	}	
}

char *
PACE_int_time::toTM(void)	
{
	ostrstream ost;
	ost << dec << setfill('0')
	    << setw(2) << Hours
	    << setw(2) << Minutes
	    << setw(2) << Seconds
	    << "."
	    << setw(3) << Milliseconds
	    << '\0';
	if (tmsh) delete tmsh;
	tmsh=ost.str();
	return tmsh;
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
