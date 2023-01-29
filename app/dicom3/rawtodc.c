/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)rawtodc.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#ifdef __GNUG__
#include <builtin.h>
#endif
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <strstream.h>
#include <iomanip.h>

#include "tlist.h"
#include "tfstream.h"
#include "tddictc.h"

#include "GetOpt.h"
#include "MainCmd.h"

typedef unsigned short Uint16;

static istream	*instream;
static Uint16 	width;
static Uint16 	height;
static Uint16 	depth;
static char 	endian;
static Uint16 	*linebuffer;

static Uint16
readUint16(istream *instream)
{
	Uint16 u;
	unsigned char buffer[2];
	instream->read(buffer,2);
	if (endian == 'l') {
		u =  (Uint16)buffer[1];
		u <<= 8;
		u |= (Uint16)buffer[0];
	}
	else {
		u =  (Uint16)buffer[0];
		u <<= 8;
		u |= (Uint16)buffer[1];
	}
	return u;
}

static int
readrawlinestart(void)
{
	// instream is already open

	linebuffer=new Uint16[width];
	if (!linebuffer) {
		cerr << "Couldn't allocate linebuffer\n" << flush;
		return -1;
	}

	return 0;
}

static int
readrawlinedone(void)
{
	// leave instream open - closed in main routine

	if (linebuffer)	delete [] linebuffer;

	return 0;
}

static int
readrawline(Uint16 **buffer)
{
	Uint16 *ptr = linebuffer;

	unsigned col;
	for (col=0; col<width; ++col) {
		Uint16 u=readUint16(instream);
		if (!instream->good()) break;
		*ptr++=u;
	}

	if (!instream->good()) {
		cerr << "Line read failed\n" << flush;
		return -1;
	}

	*buffer=linebuffer;		// return pointer to data read
	return width;			// return length of data (1 line)
}

int
main(int argc,char *argv[])
{
	CCOMMAND(argc,argv);

	char *	stamp		= 0;
	char *	inname		= 0;
	char *	outname		= 0;
	int 	verbose		= 0;
	int 	metaheader 	= 0;
	char *	outsyntaxUID	= DC3DefaultTransferSyntax;
	unsigned	usecin = 0;

	width = 0;
	height = 0;
	depth = 0;
	endian = 0;

	GetOpt getopt(argc,argv,"lbw:h:d:x:mo:r[2]s:v");
	int c;
	int err=0;
	DC3ListofAttributes deletelist;
	DC3ListofAttributes replacelist;
	while ((c=getopt()) != -1 && !err) {
		switch (c) {
			case '-':	// use standard input
				usecin=1;
				break;
			case 'v':	// Verbose
				verbose=1;
				break;
			case 'x':	// Delete attribute keyword
				{
					char *keyword=getopt.optarg;
					char *values="";
					DC3Attribute *attr;
					DC3AttributeGet(keyword,values,
						&attr,&err);
					if (!attr || err) {
						cerr << "Couldn't handle <"
						     << keyword
						     << "> attribute\n"
						     << flush;
					}
					else {
						deletelist.insert(attr);
					}
				}
				break;
			case 'r':	// Replace attribute keyword & value
				{
					char *keyword=getopt.optarg;
					char *values=argv[getopt.optind];
					DC3Attribute *attr;
					DC3AttributeGet(keyword,values,
						&attr,&err);
					if (!attr || err) {
						cerr << "Couldn't handle <"
						     << keyword
						     << "> attribute\n"
						     << flush;
					}
					else {
						replacelist.insert(attr);
					}
				}
				break;
			case 'm':	// Generate metaheader
				metaheader=1;
				break;
			case 'o':	// Output transfer syntax
				outsyntaxUID=getopt.optarg;
				break;
			case 's':	// Stamp with specified date/time
				stamp=getopt.optarg;
				break;
			case 'w':	// width
				width=atoi(getopt.optarg);
				break;
			case 'h':	// height
				height=atoi(getopt.optarg);
				break;
			case 'd':	// depth
				depth=atoi(getopt.optarg);
				break;
			case 'l':	// little endian
			case 'b':	// big endian
				endian=c;
				break;
			case '?':
			default:
				err=1;
		}
	}

	switch (depth) {
#ifdef CRAP
		case 8:
			if (endian) {
				cerr << "Can't have byte order for 8 bits !\n"
				     << flush;
				err=1;
			}
			break;
#endif // CRAP
		case 12:
		case 16:
			if (!endian) {
				cerr << "Must specify byte order for "
				     << depth << " bit data\n"
				     << flush;
				err=1;
			}
			break;
		default:
#ifdef CRAP
			cerr << "Must specify depth of 8,12 or 16" << "\n"
			     << flush;
#else  // CRAP
			cerr << "Must specify depth of 12 or 16" << "\n"
			     << flush;
#endif // CRAP
			err=1;
			break;
	}

	if (!width || !height) {
		cerr << "Must specify width & height\n" << flush;
		err=1;
	}

	if (getopt.optind+1 == argc) {
		inname=0;
		outname=argv[getopt.optind];
	}
	else {
		if (usecin) {
			cerr << "Specify outfile alone if using stdin\n"
			     << flush;
			err=1;
		}
		else {
			if (getopt.optind+2 == argc) {
				inname=argv[getopt.optind];
				outname=argv[getopt.optind+1];
			}
			else err=1;
		}
	}

	if (err) {
		cerr << "Usage: " << argv[0] << " "
		     << "-w nn -h nn -d nn [-l|b] [-m]"
		     << "[-o syntax] [-x key] [-r key value] [-s stamp] [-v] "
		     << "[infile|-] outfile [<infile]\n"
		     << flush;
		return 1;
	}

	if (verbose) {
		cerr << "************** Deletions ************\n" << flush;
		deletelist.first();
		while (deletelist.ismore()) {
			DC3Attribute *attr = deletelist.value();
			attr->put(cerr);
			deletelist.next();
		}
		cerr << "************** Replacements ************\n" << flush;
		replacelist.first();
		while (replacelist.ismore()) {
			DC3Attribute *attr = replacelist.value();
			attr->put(cerr);
			replacelist.next();
		}
	}

	if (inname) {
		ifstream *infstream = new ifstream(inname,ios::in);
		if (!infstream->rdbuf()->is_open()) {
			cerr << "Can't open <" << inname << "> for read\n" 
			     << flush;
			return 1;
		}
		instream=infstream;
	}
	else instream=&cin;

	DC3ListofAttributes list;

	{
		// Rows = height

		DC3Tag tag (DC3Rows_GROUP,
			    DC3Rows_ELEMENT);
		Uint16 i = height;
		DC3UnspecifiedShort *attribute =
			new DC3UnspecifiedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// Columns = width

		DC3Tag tag (DC3Columns_GROUP,
			    DC3Columns_ELEMENT);
		Uint16 i = height;
		DC3UnspecifiedShort *attribute =
			new DC3UnspecifiedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// PixelPaddingValue = 0

		DC3Tag tag (DC3PixelPaddingValue_GROUP,
			    DC3PixelPaddingValue_ELEMENT);
		Uint16 i = 0;
		DC3UnspecifiedShort *attribute =
			new DC3UnspecifiedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// BitsAllocated = 16

		DC3Tag tag (DC3BitsAllocated_GROUP,
			    DC3BitsAllocated_ELEMENT);
		Uint16 i = 16;
		DC3UnsignedShort *attribute =
			new DC3UnsignedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// HighBit = depth-1

		DC3Tag tag (DC3HighBit_GROUP,
			    DC3HighBit_ELEMENT);
		Uint16 i = depth-1;
		DC3UnsignedShort *attribute =
			new DC3UnsignedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// BitsStored = depth

		DC3Tag tag (DC3BitsStored_GROUP,
			    DC3BitsStored_ELEMENT);
		Uint16 i = depth;
		DC3UnsignedShort *attribute =
			new DC3UnsignedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// PixelRepresentation = 0 (unsigned)

		DC3Tag tag (DC3PixelRepresentation_GROUP,
			    DC3PixelRepresentation_ELEMENT);
		Uint16 i = 0;
		DC3UnsignedShort *attribute =
			new DC3UnsignedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// SamplesPerPixel = 1

		DC3Tag tag (DC3SamplesPerPixel_GROUP,
			    DC3SamplesPerPixel_ELEMENT);
		Uint16 i = 1;
		DC3UnsignedShort *attribute =
			new DC3UnsignedShort(tag,2,&i);
		list.insert(attribute);
	}
	{
		// PhotometricInterpretation = MONOCHROME2

		DC3Tag tag (DC3PhotometricInterpretation_GROUP,
			    DC3PhotometricInterpretation_ELEMENT);
		char *str = "MONOCHROME2";
		DC3CodeString *attribute =
			new DC3CodeString(tag,strlen(str),str);
		list.insert(attribute);
	}

	if (verbose)
		cerr << "**************** Reading **************\n" << flush;

	{
		// Add a PixelData attribute ...

		DC3Tag tag (DC3PixelData_GROUP,DC3PixelData_ELEMENT);
		DC3OtherWordString *attr = 
			new DC3OtherWordString(tag,
				width*height*2,
				readrawlinestart,
				readrawline,
				readrawlinedone);
		list.insert(attr);
	}

	if (verbose)
		cerr << "**************** Deleting **************\n" << flush;

	list.Delete(deletelist);

	if (verbose)
		cerr << "**************** Replacing **************\n" << flush;

	list.Replace(replacelist);

	if (verbose)
		cerr << "**************** Fixing **************\n" << flush;

	list.Fix(outsyntaxUID,metaheader);

	if (verbose)
		cerr << "**************** Writing **************\n" << flush;

	DC3TransferSyntax outtest(outsyntaxUID);
	if (!outtest.isValid()) {
		cerr << "Output Transfer Syntax UID <"
		     << outsyntaxUID << "> is invalid\n";
		return 1;
	}

	DC3ofstream outstream(outname,outsyntaxUID);
	if (!outstream.is_open()) {
		cerr << "Can't open <" << outname << "> for write\n" << flush;
		return 1;
	}

	list.first();
	while (list.ismore()) {
		DC3Attribute *attr = list.value();
		if (verbose) {
			attr->put(cerr);
		}
		attr->write(outstream);
		list.next();
	}

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
