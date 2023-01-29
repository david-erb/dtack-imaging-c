/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* dgtypes.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3dgtypes_HEADER_
#define _DC3dgtypes_HEADER_

#include <math.h>
#include <string.h>
#include <stdlib.h>	// for atoi()

#include <fstream.h>

#include "ttypes.h"

/* ************************ Data General Classes ************************ */

class DGR {
	double		value;

	unsigned char	sign;
	Uint16		exponent;
	Uint32		mantissa;

	char		sh[20];	// should be enough for DG Real !
	unsigned 	shmax;
	char		*toString(void);
public:
	DGR(unsigned l)
		{
			if (l != 1) {
				cerr << "DGI:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh[0]='\0';
			shmax=20;	// CC-3.01 doesn't like initializers :(
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDS(void)	{ return toString(); }
	double	todouble(void)	{ return value; }
};

class DGI {
	Uint16	u;
	char	sh[10];		// should be enough for Uint16 string
	unsigned shmax;
	char	*toString(void);
public:
	DGI(unsigned l)
		{
			if (l != 1) {
				cerr << "DGI:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh[0]='\0';
			shmax=10;	// CC-3.01 doesn't like initializers :(
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
};

class DGI4 {
	Uint32	u;
	char	sh[20];		// should be enough for Uint32 string
	unsigned shmax;
	char	*toString(void);
public:
	DGI4(unsigned l)
		{
			if (l != 1) {
				cerr << "DGI:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh[0]='\0';
			shmax=20;	// CC-3.01 doesn't like initializers :(
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toSH(void)	{ return toString(); }
	char *	toIS(void)	{ return toString(); }
	char *	toDS(void)	{ return toString(); }

	Uint16	toUS(void)	{ return (Uint16)u; }
	Uint32	toUL(void)	{ return (Uint32)u; }
	Int16	toSS(void)	{ return (Int16)u; }
	Int32	toSL(void)	{ return (Int32)u; }
};

class DGDD {
	Uint16	month;
	Uint16	day;
	Uint16	year;
	char	sh[11];		// should be enough for date string
	unsigned shmax;
	char	*toString(void);
public:
	DGDD(unsigned l)
		{
			if (l != 1) {
				cerr << "DGI:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh[0]='\0';
			shmax=11;	// CC-3.01 doesn't like initializers :(
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDA(void)	{ return toString(); }
	char *	toSH(void)	{ return toString(); }
};

class DGTT {
	Uint16	hours;
	Uint16	mins;
	Uint16	secs;
	char	sh[17];		// should be enough for time string
	unsigned shmax;
	char	*toString(void);
public:
	DGTT(unsigned l)
		{
			if (l != 1) {
				cerr << "DGI:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh[0]='\0';
			shmax=17;	// CC-3.01 doesn't like initializers :(
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toTM(void)	{ return toString(); }
	char *	toSH(void)	{ return toString(); }
};

class DGA {
	char *str;
	unsigned inlength;	// inlength is in bytes (cf. DGA2)
public:
	DGA(unsigned l)
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

	~DGA(void)
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
	Uint16	toUS(void)	{ return atoi(str); }
};

class DGA2 {
	char *str;
	unsigned inlength;	// inlength is in DG words ie. 2 bytes
public:
	DGA2(unsigned l)
		{
			str=new char[l*2+1];
			if (str) {
				inlength=l;
				*str='\0';
			}
			else {
				inlength=0;
			}
		}

	~DGA2(void)
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
	Uint16	toUS(void)	{ return atoi(str); }
};

#endif /* _DC3dgtypes_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
