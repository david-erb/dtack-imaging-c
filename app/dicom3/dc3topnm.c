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
		return 1;
	}

	if (insyntaxUID) {
		DC3TransferSyntax intest(insyntaxUID);
		if (!intest.isValid()) {
			cerr << "Input Transfer Syntax UID <"
			     << insyntaxUID << "> is invalid\n";
			return 1;
		}
	}
	DC3TransferSyntax outtest(outsyntaxUID);
	if (!outtest.isValid()) {
		cerr << "Output Transfer Syntax UID <"
		     << outsyntaxUID << "> is invalid\n";
		return 1;
	}

	DC3ifstream instream(inname,insyntaxUID);
	if (!instream.is_open()) {
		cerr << "Can't open <" << inname << "> for read\n" << flush;
		return 1;
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
					//	      	0001H=2s complement)
	failed |= list.getSingleValueUint16(
			DC3TagFromName(PixelRepresentation),
			PixelRepresentation,verbose);


	DC3Attribute *PixelData;
	fatal |= ((PixelData=list.find(DC3TagFromName(PixelData))) == 0);

	if (failed) {
		cerr << "Mandatory attributes missing in Image Pixel module"
		     << " - will assume defaults\n" << flush;
	}

	if (fatal) {
		cerr << "Mandatory attributes missing in Image Pixel module"
		     << " - cannot proceed\n" << flush;
		return 1;
	}

	if (verbose)
		cerr << "**************** Writing **************\n" << flush;

	instream.clear();	// Clear EOF state to allow seeking/reading

	DC3ofstream outstream(outname,outsyntaxUID);
	if (!outstream.is_open()) {
		cerr << "Can't open <" << outname << "> for write\n" << flush;
		return 1;
	}

	if (SamplesPerPixel != 1) {
		cerr << "Unsupported SamplesPerPixel = " << SamplesPerPixel
		     << "\n" << flush;
		return 1;
	}
	if (strncmp(PhotometricInterpretation,"MONOCHROME1",11) != 0
	 && strncmp(PhotometricInterpretation,"MONOCHROME2",11) != 0
	) {
		cerr << "Unsupported PhotometricInterpretation = " 
		     << PhotometricInterpretation
		     << "\n" << flush;
		return 1;
	}
	if (HighBit+1-BitsStored != 0) {
		cerr << "Unsupported - data not packed in low part of word"
		     << "\n" << flush;
		return 1;
	}

	// PNM is unsigned, so ...
	int convert = (PixelRepresentation != 0) ? HighBit : 0;

#define PNMBIGRAW 1
#if	PNMBIGRAW
	{
		unsigned long maxval = (1<<BitsStored)-1;
		ostrstream ost;
		ost << "P5\n" << Columns << " " << Rows << "\n"
		    << maxval << "\n" << '\0';
		char *str=ost.str();
		outstream.write(str,strlen(str));
		delete str;
	}
	cerr << "PixelData->writedata(outstream,convert)" << flush;

	PixelData->writedata(outstream,convert);

#else /* PNMBIGRAW */
	Compile Time Error - only extended raw PNM output supported
#endif /* PNMBIGRAW */

	outstream.close();
	return 0;
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
