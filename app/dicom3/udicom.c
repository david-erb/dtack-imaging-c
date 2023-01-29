/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)dc3topnm.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include "tattrrd.h"
#include "tlist.h"
#include "tfstream.h"

#include "GetOpt.h"
#include "MainCmd.h"

#include <dtack/base.h>

#define GOOD (0)						/* return codes 					*/
#define BAD (1)

/*..........................................................................
 * takes a list of attributes read from a dicom file
 *..........................................................................*/

static
int
udicom_image_to_window(
  DC3ifstream &instream,				/* input file stream 				*/
  DC3ListofAttributes &list,			/* list of attributes from file 	*/
  const char *outname,					/* output window name 				*/
  int verbose)
{
	instream.clear();	// Clear EOF state to allow seeking/reading

	int failed = 0;
	int fatal = 0;

	Uint16 SamplesPerPixel = 1;
	failed |= list.getSingleValueUint16(
			DC3TagFromName(SamplesPerPixel),
			SamplesPerPixel,verbose);

	char * PhotometricInterpretation = "MONOCHROME2";
	failed |= list.getSingleValueStringCopy(
			DC3TagFromName(PhotometricInterpretation),
			PhotometricInterpretation,verbose);

	Uint16 Rows;
	fatal |= list.getSingleValueUint16(
			DC3TagFromName(Rows),
			Rows,verbose);

	Uint16 Columns;
	fatal |= list.getSingleValueUint16(
			DC3TagFromName(Columns),
			Columns,verbose);

	Uint16 BitsAllocated = 16;
	failed |= list.getSingleValueUint16(
			DC3TagFromName(BitsAllocated),
			BitsAllocated,verbose);

	Uint16 BitsStored = 16;
	failed |= list.getSingleValueUint16(
			DC3TagFromName(BitsStored),
			BitsStored,verbose);

	Uint16 HighBit = 15;
	failed |= list.getSingleValueUint16(
			DC3TagFromName(HighBit),
			HighBit,verbose);

	Uint16 PixelRepresentation = 0;	// EnumValues= (0000H=unsigned integer,
					                //   	      	0001H=2s complement)
	failed |= list.getSingleValueUint16(
			DC3TagFromName(PixelRepresentation),
			PixelRepresentation,verbose);

	DC3Attribute *PixelData;
	fatal |= ((PixelData=list.find(DC3TagFromName(PixelData))) == 0);

	if (fatal) {
		cerr << "Mandatory attributes missing in Image Pixel module"
		     << " - cannot proceed\n" << flush;
		return BAD;
	}

	if (failed) {
		cerr << "Mandatory attributes missing in Image Pixel module"
		     << " - will assume defaults\n" << flush;
		return BAD;
	}

	if (verbose)
		cerr << "**************** Writing **************\n" << flush;

	instream.clear();	// Clear EOF state to allow seeking/reading

	if (SamplesPerPixel != 1) {
		cerr << "Unsupported SamplesPerPixel = " << SamplesPerPixel
		     << "\n" << flush;
		return BAD;
	}
	if (strncmp(PhotometricInterpretation,"MONOCHROME1",11) != 0
	 && strncmp(PhotometricInterpretation,"MONOCHROME2",11) != 0
	) {
		cerr << "Unsupported PhotometricInterpretation = " 
		     << PhotometricInterpretation
		     << "\n" << flush;
		return BAD;
	}
	if (HighBit+1-BitsStored != 0) {
		cerr << "Unsupported - data not packed in low part of word"
		     << "\n" << flush;
		return BAD;
	}

    DT_RCSID("dicom3 $RCSfile: udicom.c,v $ $Revision: 1.8 $");
    DT_F("udicom");
    dt_ctl_t *ctl = NULL;
    dtimage_t wordimage;
    DT_Q(dtimage_create,(ctl,			/* get space for word image			*/
      &wordimage, Columns, Rows,
      DT_NTYPE_B16));

    PixelData->readdatastart();			/* position for reading 			*/
    PixelData->readdatanext(			/* suck all data from file 			*/
      Columns * Rows,
      wordimage.row[0].b08);

    dtimage_t byteimage;
    DT_Q(dtimage_create,(ctl,			/* get space for byte image			*/
      &byteimage, Columns, Rows,
      DT_NTYPE_B08));

    DT_Q(dtimage_cast,(ctl,				/* convert word image to byte		*/
      &wordimage, &byteimage));

    DT_Q(dtimage_export,(ctl,			/* throw byte image to window 		*/
      &byteimage, outname, 0, 0));
    DT_Q(dtimage_free,(ctl,				/* free image resources 			*/
      &byteimage));
    DT_Q(dtimage_free,(ctl,
      &wordimage));

	return GOOD;
}


/*..........................................................................
 *..........................................................................*/

int
main(int argc, char *argv[])
{
	CCOMMAND(argc,argv);

	cerr.setf(ios::showbase,ios::showbase);

	char *	stamp = 0;
	char *	inname = 0;
	char *	outname = 0;
	int 	verbose = 0;
	int 	reportlevel = 1;
	int 	stopbad = 1;
	char *	insyntaxUID  = 0;
	char *	outsyntaxUID = DC3DefaultTransferSyntax;

	// NB. Extended PGM/PPM raw word format is little-endian

	GetOpt getopt(argc,argv,"bi:vw");
	int c;
	int err=0;
	while ((c=getopt()) != -1 && !err) {
		switch (c) {
			case 'v':	// Verbose
				verbose=1;
				break;
			case 'w':	// Warnings displayed
				reportlevel=2;
				break;
			case 'b':	// Don't stop on bad attributes
				stopbad=0;
				break;
			case 'i':	// Input transfer syntax
				insyntaxUID=getopt.optarg;
				break;
			case '?':
			default:
				err=1;
		}
	}

	if (getopt.optind+2 == argc) {
		inname=argv[getopt.optind];
		outname=argv[getopt.optind+1];
	}
	else err=1;

	if (err) {
		cerr << "Usage: " << argv[0] << " -v -w -b -i syntax "
		     << "infile outfile\n"
		     << flush;
		return BAD;
	}

	if (insyntaxUID) {
		DC3TransferSyntax intest(insyntaxUID);
		if (!intest.isValid()) {
			cerr << "Input Transfer Syntax UID <"
			     << insyntaxUID << "> is invalid\n";
			return BAD;
		}
	}
	DC3TransferSyntax outtest(outsyntaxUID);
	if (!outtest.isValid()) {
		cerr << "Output Transfer Syntax UID <"
		     << outsyntaxUID << "> is invalid\n";
		return BAD;
	}

	DC3ifstream instream(inname,insyntaxUID);
	if (!instream.is_open()) {
		cerr << "Can't open <" << inname << "> for read\n" << flush;
		return BAD;
	}

	if (verbose)
		cerr << "**************** Reading **************\n" << flush;

	DC3ListofAttributes list;
	DC3Tag oldtag (0,0);
	while (1) {
		DC3Attribute *attr;
		DC3Tag newtag (0,0);
		int err;
		DC3AttributeRead(instream,reportlevel,
			&newtag,&attr,&err);
		if (!instream.good() || instream.eof()
				     || (stopbad && !attr)) break;
		if (newtag <= oldtag) {
			cerr << "Stop - tags out of order or trailing garbage\n"
			     << flush;
			break;
		}
		if (attr) {
			list.insert(attr);
		}
		oldtag=newtag;
	}


    int rc;
    rc = udicom_image_to_window(		/* copy image part to window 		*/
      instream,							/* the opened stream we are reading */
      list,								/* list of attributes we built 		*/
      outname,							/* output window name 				*/
      verbose);

	return rc;
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
