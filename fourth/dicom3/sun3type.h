/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* sun3type.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3sun3type_HEADER_
#define _DC3sun3type_HEADER_

#include <math.h>
#include <string.h>
#include <stdlib.h>	// for atoi()

#include <fstream.h>

#include "ttypes.h"

/* ************************ Sun3 Classes ************************ */

class SUN3_float {
	double		value;

	char		*sh;
	char		*toString(void);
public:
	SUN3_float(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDS(void)	{ return toString(); }
	double	todouble(void)	{ return value; }
};

class SUN3_double {
	double		value;

	char		*sh;
	char		*toString(void);
public:
	SUN3_double(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDS(void)	{ return toString(); }
	double	todouble(void)	{ return value; }
};

class SUN3_short {
	Int16	u;
	char	*sh;
	char	*toString(void);
public:
	SUN3_short(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
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

class SUN3_u_short {
	Uint16	u;
	char	*sh;
	char	*toString(void);
public:
	SUN3_u_short(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
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

class SUN3_int {
	Int32	u;
	char	*sh;
	char	*toString(void);
public:
	SUN3_int(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
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

class SUN3_u_int {
	Uint32	u;
	char	*sh;
	char	*toString(void);
public:
	SUN3_u_int(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
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

class SUN3_char {
	char *str;
	unsigned inlength;	// inlength is in bytes
public:
	SUN3_char(unsigned l)
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

	~SUN3_char(void)
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
};

class SUN3_datetime {
	Uint32	SecondsSinceEpoch;
	unsigned long	Year;
	unsigned long	Month;
	unsigned long	Day;
	unsigned long	Hours;
	unsigned long	Minutes;
	unsigned long	Seconds;
	char *dash;
	char *tmsh;
	char *dtsh;
public:
	SUN3_datetime(unsigned l)
		{
			if (l != 1) {
				cerr << "SUN3:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			dash=0;
			tmsh=0;
			dtsh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDA(void);
	char *	toTM(void);
	char *	toDT(void);
};

#endif /* _DC3sun3type_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
