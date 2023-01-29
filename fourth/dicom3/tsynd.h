/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tsynd.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tsynd_HEADER_
#define _DC3tsynd_HEADER_

#include "tsyn.h"

static struct DC3TransferSyntaxTableEntry {
	char *			UID;
	char *			Description;
	enum DC3Endian 		Endian;
	enum DC3VRType 		VRType;
	enum DC3Encapsulated 	Encapsulated;
	unsigned int		JPEGProcess8;
	unsigned int		JPEGProcess12;
	unsigned 		Supported;
} DC3TransferSyntaxTable[] =
{	DC3ImplicitVRLittleEndianTransferSyntax,
		"Implicit VR Little Endian",
		LITTLE, IMPLICIT, NOTENCAPSULATED, 0,0, 1,
	DC3ExplicitVRLittleEndianTransferSyntax,
		"Explicit VR Little Endian",
		LITTLE, EXPLICIT, NOTENCAPSULATED, 0,0, 1,
	DC3ExplicitVRBigEndianTransferSyntax,
		"Explicit VR Big Endian",
		BIG, EXPLICIT, NOTENCAPSULATED, 0,0, 1,
	DC3JPEGProcess1TransferSyntax,
		"JPEG Baseline",
		LITTLE, EXPLICIT, ENCAPSULATED, 1,1, 0,
	DC3JPEGProcess2_4TransferSyntax,
		"JPEG Extended",
		LITTLE, EXPLICIT, ENCAPSULATED, 2,4, 0,
	DC3JPEGProcess3_5TransferSyntax,
		"JPEG Extended",
		LITTLE, EXPLICIT, ENCAPSULATED, 3,5, 0,
	DC3JPEGProcess6_8TransferSyntax,
		"JPEG Spectral Selection, Non-hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 6,8, 0,
	DC3JPEGProcess7_9TransferSyntax,
		"JPEG Spectral Selection, Non-hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 7,9, 0,
	DC3JPEGProcess10_12TransferSyntax,
		"JPEG Full Progression, Non-hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 10,12, 0,
	DC3JPEGProcess11_13TransferSyntax,
		"JPEG Full Progression, Non-hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 11,13, 0,
	DC3JPEGProcess14TransferSyntax,
		"JPEG Lossless, Non-hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 14,14, 0,
	DC3JPEGProcess15TransferSyntax,
		"JPEG Lossless, Non-hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 15,15, 0,
	DC3JPEGProcess16_18TransferSyntax,
		"JPEG Extended, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 16,18, 0,
	DC3JPEGProcess17_19TransferSyntax,
		"JPEG Extended, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 17,19, 0,
	DC3JPEGProcess20_22TransferSyntax,
		"JPEG Spectral Selection, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 20,22, 0,
	DC3JPEGProcess21_23TransferSyntax,
		"JPEG Spectral Selection, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 21,23, 0,
	DC3JPEGProcess24_26TransferSyntax,
		"JPEG Full Progression, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 24,26, 0,
	DC3JPEGProcess25_27TransferSyntax,
		"JPEG Full Progression, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 25,27, 0,
	DC3JPEGProcess28TransferSyntax,
		"JPEG Lossless, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 28,28, 0,
	DC3JPEGProcess29TransferSyntax,
		"JPEG Lossless, Hierarchical",
		LITTLE, EXPLICIT, ENCAPSULATED, 29,29, 0,
	DC3JPEGProcess14SV1TransferSyntax,
		"JPEG Lossless, Hierarchical, 1st Order Prediction",
		LITTLE, EXPLICIT, ENCAPSULATED, 14,14, 0,
	0,0,(DC3Endian)0,(DC3VRType)0,(DC3Encapsulated)0,0,0,0
};


#endif /* _DC3tsynd_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
