/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tsyn.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tsyn_HEADER_
#define _DC3tsyn_HEADER_

#include "tsync.h"

enum DC3Endian {LITTLE,BIG,BIGBAD};
enum DC3VRType {IMPLICIT,EXPLICIT};
enum DC3Encapsulated {ENCAPSULATED,NOTENCAPSULATED};

class DC3TransferSyntax {
	char *			UID;
	char *			Description;
	enum DC3Endian 		Endian;
	enum DC3VRType 		VRType;
	enum DC3Encapsulated 	Encapsulated;
public:
	DC3TransferSyntax(enum DC3Endian endian,
			  enum DC3VRType vrtype,
			  enum DC3Encapsulated encapsulated)
		{
			UID="Automatically determined";
			Description="Automatically determined";
			Endian=endian;
			VRType=vrtype;
			Encapsulated=encapsulated;
		}

	DC3TransferSyntax(const char *UID);

	~DC3TransferSyntax(void);

	char *	Describe(void)		{ return Description; }
	int	isLittleEndian(void)	{ return Endian == LITTLE; }
	int	isBigEndian(void)	{ return Endian == BIG; }
	int	isBigBadEndian(void)	{ return Endian == BIGBAD; }
	int	isImplicitVR(void)	{ return VRType == IMPLICIT; }
	int	isExplicitVR(void)	{ return VRType == EXPLICIT; }
	int	isEncapsulated(void)
		{ return Encapsulated == ENCAPSULATED; }
	int	isNotEncapsulated(void)
		{ return Encapsulated == NOTENCAPSULATED; }
	int	isValid(void)		{ return UID != 0; }
};

#endif /* _DC3tsyn_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
