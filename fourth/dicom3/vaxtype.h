/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* vaxtype.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3vaxtype_HEADER_
#define _DC3vaxtype_HEADER_

#include <math.h>
#include <string.h>
#include <stdlib.h>	// for atoi()

#include <fstream.h>

#include "ttypes.h"

/* ************************ Vax Classes ************************ */

class VAX_float_f {
	double		value;

	char		*sh;
	char		*toString(void);
public:
	VAX_float_f(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
		}

	void	read(ifstream& instream,unsigned block,unsigned offset);

	char *	toDS(void)	{ return toString(); }
	double	todouble(void)	{ return value; }
};

class VAX_byte {
	signed char	u;
	char	*sh;
	char	*toString(void);
public:
	VAX_byte(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
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

class VAX_u_byte {
	unsigned char	u;
	char	*sh;
	char	*toString(void);
public:
	VAX_u_byte(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
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

class VAX_short {
	Int16	u;
	char	*sh;
	char	*toString(void);
public:
	VAX_short(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
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

class VAX_u_short {
	Uint16	u;
	char	*sh;
	char	*toString(void);
public:
	VAX_u_short(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
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

class VAX_int {
	Int32	u;
	char	*sh;
	char	*toString(void);
public:
	VAX_int(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
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

class VAX_u_int {
	Uint32	u;
	char	*sh;
	char	*toString(void);
public:
	VAX_u_int(unsigned l)
		{
			if (l != 1) {
				cerr << "VAX:: Only length 1 supported, not "
				     << l << "\n" << flush;
			}
			sh=0;
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

class VAX_char {
	char *str;
	unsigned inlength;	// inlength is in bytes
public:
	VAX_char(unsigned l)
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

	~VAX_char(void)
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

#endif /* _DC3vaxtype_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
