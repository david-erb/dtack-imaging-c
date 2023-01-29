/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)dc3xdisp.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#define INSTANTIATEGLOBALS

#include "tattrrd.h"
#include "tlist.h"
#include "tfstream.h"

#include "gnerror.h"
#include "vgdisplay.h"

#include "GetOpt.h"
#include "MainCmd.h"

int
main(int argc, char *argv[])
{
	CCOMMAND(argc,argv);

	cerr.setf(ios::showbase,ios::showbase);

	char *	inname = 0;
	int 	verbose = 0;
	int 	reportlevel = 1;
	int 	stopbad = 1;
	char *	insyntaxUID  = 0;
	char	windowing = 's';

	GetOpt getopt(argc,argv,"bi:vwsf");
	int c;
	int err=0;
	DC3ListofAttributes deletelist;
	DC3ListofAttributes replacelist;
	while ((c=getopt()) != -1 && !err) {
		switch (c) {
			case 'v':	// Verbose
				verbose=1;
				break;
			case 'i':	// Input transfer syntax
				insyntaxUID=getopt.optarg;
				break;
			case 'w':	// Warnings displayed
				reportlevel=2;
				break;
			case 'b':	// Don't stop on bad attributes
				stopbad=0;
				break;
			case 's':	// slow - image windowing
			case 'f':	// fast - map windowing
				windowing=c;
				break;
			case '?':
			default:
				err=1;
		}
	}

	if (getopt.optind+1 == argc) {
		inname=argv[getopt.optind];
	}
	else err=1;

	if (err) {
		cerr << "Usage: " << argv[0]
		     << " [-i syntax] [-v] [-w] [-b]  [-f|s]"
		     << " infile\n"
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

	DC3ifstream instream(inname,insyntaxUID);
	if (!instream.is_open()) {
		cerr << "Can't open <" << inname << "> for read\n" << flush;
		return 1;
	}

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
			cerr << "Tags out of sequence\n"
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
	}
	else {
		VGDisplay display;
		VGWindow window(&display,Columns,Rows);

		VGWindowImage wimage(&window);

		VGColormap map(&display);
		VGColormap tmpmap(&display);

		VGImage *image = 0;

		switch (BitsStored) {
			case 8:
				image = new VGImage_8bit (Columns,Rows);
				break;
			case 12:
				image = new VGImage_12bit (Columns,Rows);
				break;
			case 16:
				image = new VGImage_16bit (Columns,Rows);
				break;
			default:
				cerr << "Unimplemented BitsStored = "
				     << BitsStored << "\n" << flush;
				return 1;
		}

		switch (BitsStored) {
			case 8:
			case 12:
			case 16:
				map.setLinearGray(1<<BitsStored);
				if (windowing == 'f')
					tmpmap.setLinearGray(1<<BitsStored);
				break;
			default:
				cerr << "Unimplemented BitsStored = "
				     << BitsStored << "\n" << flush;
				return 1;
		}

		Uint32 minval=0xffff;
		Uint32 maxval=0;
		{
			unsigned r;
			unsigned rowlength=Columns*(BitsAllocated/8);
			unsigned char *buffer =
				new unsigned char [rowlength];
			Uint16 signmask=1<<HighBit;
			PixelData->readdatastart();
			// Won't handle 8 bits if VR OW :(
			for (r=0; r<Rows; ++r) {
				PixelData->readdatanext(Columns,buffer);
				unsigned count=Columns;
				if (BitsAllocated > 8) {
					Uint16 *ubuffer = (Uint16 *)buffer;
					while (count--) {
						if (PixelRepresentation) *ubuffer^=signmask;
						if (*ubuffer > maxval) maxval=*ubuffer;
						if (*ubuffer < minval) minval=*ubuffer;
						++ubuffer;
					}
				}
				else {
					unsigned char *ubuffer = (unsigned char *)buffer;
					while (count--) {
						if (PixelRepresentation) *ubuffer^=signmask;
						if (*ubuffer > maxval) maxval=*ubuffer;
						if (*ubuffer < minval) minval=*ubuffer;
						++ubuffer;
					}
				}
				image->setRow(r,buffer);
			}
			delete[] buffer;
		}

		if (verbose) {
			cerr << "Minimum unsigned value " << dec << minval << "\n" << flush;
			cerr << "Maximum unsigned value " << dec << maxval << "\n" << flush;
		}

		//int maplevel = PixelRepresentation ? (1<<HighBit) : 500;
		//int mapwidth = 500;
		int maplevel = (int)((minval+maxval)/2);
		int mapwidth = (int)((maxval-minval)/2);

		if (verbose) {
			cerr << "Initial level " << dec << maplevel << "\n" << flush;
			cerr << "Initial width " << dec << mapwidth << "\n" << flush;
		}

		if (windowing == 's') {
			map.setImageLevelWidth(maplevel,mapwidth);
			window.setColormap(&map);
		}
		else {
			tmpmap.setMapLevelWidth(maplevel,mapwidth);
			window.setColormap(&tmpmap);
		}
		wimage.Load(image,&map);
		window.SelectInput();
		window.Map();

		while (1) {
			XEvent report = display.NextEvent();
			switch (report.type) {
				int lastx,lasty;
				case Expose:
					if (report.xexpose.count != 0) break;
					wimage.Put();
					break;
				case ButtonPress:
					if (report.xbutton.button == Button1) {
						lastx=report.xbutton.x;
						lasty=report.xbutton.y;
					}
					break;
				case ButtonRelease:
					// up=increase level,down=decrease
					// left=narrow window,right=widen
					if (report.xbutton.button == Button1) {
						int deltax=(int)
							report.xbutton.x-lastx;
						int deltay=(int)
							report.xbutton.y-lasty;
						if (verbose) cerr << "delta "
						     << dec << deltax << ","
						     << dec << deltay << "\n" << flush;
						mapwidth+=deltax;
						if (mapwidth<0) mapwidth=0;
						maplevel-=deltay;
						if (maplevel<0) maplevel=0;
						if (verbose) cerr
						     << "width " << dec << mapwidth 
						     << " level " << dec << maplevel 
						     << "\n" << flush;
						if (windowing == 's') {
							map.setImageLevelWidth(
								maplevel,
								mapwidth);
							wimage.Load(image,&map);
							wimage.Put();
						}
						else {
							tmpmap.setMapLevelWidth(
								maplevel,
								mapwidth);
							window.setColormap(
								&tmpmap);
						}
					}
					break;
				default:
					break;
			}
		}
	}

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
