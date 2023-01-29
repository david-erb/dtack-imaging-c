/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tsync.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */


#ifndef _DC3tsync_HEADER_
#define _DC3tsync_HEADER_

#define	DC3DefaultTransferSyntax	DC3ImplicitVRLittleEndianTransferSyntax

#ifdef USEOLDDRAFTMETAHEADERTRANSFERSYNTAX
#define	DC3MetaHeaderTransferSyntax	DC3ImplicitVRLittleEndianTransferSyntax
#else  // USEOLDDRAFTMETAHEADERTRANSFERSYNTAX
#define	DC3MetaHeaderTransferSyntax	DC3ExplicitVRLittleEndianTransferSyntax
#endif // USEOLDDRAFTMETAHEADERTRANSFERSYNTAX

#define	DC3ImplicitVRLittleEndianTransferSyntax		"1.2.840.10008.1.2"
#define	DC3ExplicitVRLittleEndianTransferSyntax		"1.2.840.10008.1.2.1"
#define	DC3ExplicitVRBigEndianTransferSyntax		"1.2.840.10008.1.2.2"

#define	DC3DefaultLosslessJPEGTransferSyntax	DC3JPEGProcess14SV1TransferSyntax
#define	DC3DefaultLossy8BitJPEGTransferSyntax	DC3JPEGProcess1TransferSyntax
#define	DC3DefaultLossy12BitJPEGTransferSyntax	DC3JPEGProcess4TransferSyntax

#define	DC3JPEGProcess1TransferSyntax		"1.2.840.10008.1.2.4.50"
#define	DC3JPEGProcess2_4TransferSyntax		"1.2.840.10008.1.2.4.51"
#define	DC3JPEGProcess3_5TransferSyntax		"1.2.840.10008.1.2.4.52"
#define	DC3JPEGProcess6_8TransferSyntax		"1.2.840.10008.1.2.4.53"
#define	DC3JPEGProcess7_9TransferSyntax		"1.2.840.10008.1.2.4.54"
#define	DC3JPEGProcess10_12TransferSyntax	"1.2.840.10008.1.2.4.55"
#define	DC3JPEGProcess11_13TransferSyntax	"1.2.840.10008.1.2.4.56"
#define	DC3JPEGProcess14TransferSyntax		"1.2.840.10008.1.2.4.57"
#define	DC3JPEGProcess15TransferSyntax		"1.2.840.10008.1.2.4.58"
#define	DC3JPEGProcess16_18TransferSyntax	"1.2.840.10008.1.2.4.59"
#define	DC3JPEGProcess17_19TransferSyntax	"1.2.840.10008.1.2.4.60"
#define	DC3JPEGProcess20_22TransferSyntax	"1.2.840.10008.1.2.4.61"
#define	DC3JPEGProcess21_23TransferSyntax	"1.2.840.10008.1.2.4.62"
#define	DC3JPEGProcess24_26TransferSyntax	"1.2.840.10008.1.2.4.63"
#define	DC3JPEGProcess25_27TransferSyntax	"1.2.840.10008.1.2.4.64"
#define	DC3JPEGProcess28TransferSyntax		"1.2.840.10008.1.2.4.65"
#define	DC3JPEGProcess29TransferSyntax		"1.2.840.10008.1.2.4.66"
#define	DC3JPEGProcess14SV1TransferSyntax	"1.2.840.10008.1.2.4.70"

#endif /* _DC3tsync_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
