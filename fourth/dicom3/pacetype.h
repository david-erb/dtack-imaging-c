/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* pacetype.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3pacetype_HEADER_
#define _DC3pacetype_HEADER_

#include <math.h>
#include <string.h>
#include <stdlib.h>	// for atoi()

#include <fstream.h>
#include <strstream.h>

#include "ttypes.h"

/* ************************ Pace Classes ************************ */

class PACE_float32 {
	double		value;

	char		*sh;
	char		*toString(void);
public:
	PACE_float32(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDS(void)	{ return toString(); }
	double	todouble(void)	{ return value; }
};

class PACE_int16 {
	Int16	u;
	char	*sh;
	char	*toString(void);
public:
	PACE_int16(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }
	char *	toLO(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
	Uint16	toXS(void)	{ return (Uint16)u; }
};

class PACE_u_int16 {
	Uint16	u;
	char	*sh;
	char	*toString(void);
public:
	PACE_u_int16(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }
	char *	toLO(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
	Uint16	toXS(void)	{ return (Uint16)u; }
};

class PACE_int32 {
	Int32	u;
	char	*sh;
	char	*toString(void);
public:
	PACE_int32(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }
	char *	toLO(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
	Uint16	toXS(void)	{ return (Uint16)u; }
};

class PACE_u_int32 {
	Uint32	u;
	char	*sh;
	char	*toString(void);
public:
	PACE_u_int32(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }
	char *	toLO(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
	Uint16	toXS(void)	{ return (Uint16)u; }
};

class PACE_string {
	char *str;
	unsigned inlength;	// inlength is in bytes
public:
	PACE_string(unsigned l)
		{
			str=new char[l+1];
			if (str) {
				inlength=l;
				*str='\0';
			}
			else {
				inlength=0;
			}
		}

	~PACE_string(void)
		{
			if (str) delete[] str;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toLO(void)	{ return str; }
	char *	toPN(void)	{ return str; }
	char *	toSH(void)	{ return str; }
	char *	toCS(void)	{ return str; }
	char *	toLT(void)	{ return str; }
	char *	toIS(void)	{ return str; }
	char *	toDS(void)	{ return str; }
	Uint16	toUS(void)	{ return atoi(str); }
	Int16	toSS(void)	{ return atoi(str); }
	double	todouble(void)
		{ 
			double d;
			istrstream(str) >> d;
			return d;
		}
};

class PACE_byte {
	Uint16	u;
	char	*sh;
	char	*toString(void);
public:
	PACE_byte(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }
	char *	toLO(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
	Uint16	toXS(void)	{ return (Uint16)u; }
};

class PACE_string_date  {
	union {
		char str[11];
		struct {
			char yyyy[4];
			char delim1;
			char mm[2];
			char delim2;
			char dd[2];
			char terminator;
		} date;
	} u;
	char *dash;
public:
	PACE_string_date(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			dash=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDA(void);
};

class PACE_string_time {
	union {
		char str[13];
		struct {
			char hh[2];
			char delim1;
			char mm[2];
			char delim2;
			char ssxxx[5];
			char terminator;
		} date;
	} u;
	char *dash;
public:
	PACE_string_time(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			dash=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toTM(void);
};

class PACE_int_date {
	Uint32	DaysSinceEpoch;
	unsigned long	Year;
	unsigned long	Month;
	unsigned long	Day;
	char *dash;
public:
	PACE_int_date(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			dash=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDA(void);
};

class PACE_int_time {
	Uint32	ticks;
	unsigned long	Hours;
	unsigned long	Minutes;
	unsigned long	Seconds;
	unsigned long	Milliseconds;
	char *tmsh;
public:
	PACE_int_time(unsigned l)
		{
			if (l != 1) {
				cerr << "PACE:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			tmsh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toTM(void);
};


#endif /* _DC3pacetype_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
