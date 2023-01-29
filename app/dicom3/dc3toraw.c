/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)dc3toraw.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include "tattrrd.h"
#include "ttag.h"
#include "tfstream.h"
#include "tddict.h"
#include "tddictc.h"

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
	int 	doconvert = 0;
	char *	insyntaxUID  = 0;
	char *	outsyntaxUID = DC3DefaultTransferSyntax;

	GetOpt getopt(argc,argv,"bci:o:vw");
	int c;
	int err=0;
	while ((c=getopt()) != -1 && !err) {
		switch (c) {
			case 'c':	// Convert signed to unsigned
				doconvert=1;
				break;
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
			case 'o':	// Output transfer syntax
				outsyntaxUID=getopt.optarg;
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
		cerr << "Usage: " << argv[0]
		     << " -c -v -w -b -i syntax -o syntax "
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

	DC3Tag oldtag (0,0);
	DC3Tag pixeltag (DC3PixelData_GROUP,DC3PixelData_ELEMENT);
	DC3Tag rowstag (DC3Rows_GROUP,DC3Rows_ELEMENT);
	DC3Tag colstag (DC3Columns_GROUP,DC3Columns_ELEMENT);
	DC3Tag pixreptag (DC3PixelRepresentation_GROUP,
		DC3PixelRepresentation_ELEMENT);
	DC3Tag highbittag (DC3HighBit_GROUP,DC3HighBit_ELEMENT);
	DC3Attribute *pixelattr;
	Uint16 pixrep = 0;
	Uint16 highbit = 15;

	while (1) {
		DC3Attribute *attr;
		DC3Tag newtag (0,0);
		int err;
		DC3AttributeRead(instream,reportlevel,
			&newtag,&attr,&err);
		if (!instream.good() || instream.eof()
				     || (stopbad && !attr)) break;
		if (newtag <= oldtag) {
			cerr << "Tags out of sequence\n"
			     << flush;
			break;
		}
		if (attr) {
			if (newtag == rowstag) {
				if (verbose) attr->put(cerr);
				delete attr;
			}
			else if (newtag == colstag) {
				if (verbose) attr->put(cerr);
				delete attr;
			}
			else if (newtag == pixeltag) {
				pixelattr=attr;
			}
			else if (newtag == pixreptag) {
				Xint value;
				if (attr->SingleValueNumeric(value) != -1)
					pixrep=(Uint16)value;
			}
			else if (newtag == highbittag) {
				Xint value;
				if (attr->SingleValueNumeric(value) != -1)
					highbit=(Uint16)value;
			}
			else {
				delete attr;
			}
		}
		oldtag=newtag;
	}

	int convert;
	if (doconvert)
		convert = (pixrep != 0) ? highbit : 0;	// Force unsigned
	else
		convert=0;

	if (verbose)
		cerr << "**************** Writing **************\n" << flush;

	instream.clear();	// Clear EOF state to allow seeking/reading

	DC3ofstream outstream(outname,outsyntaxUID);
	if (!outstream.is_open()) {
		cerr << "Can't open <" << outname << "> for write\n" << flush;
		return 1;
	}

	pixelattr->writedata(outstream,convert);
	delete pixelattr;

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
