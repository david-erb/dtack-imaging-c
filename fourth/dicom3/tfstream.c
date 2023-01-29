/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tfstream.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <ctype.h>
#include <string.h>

#include "tfstream.h"
#include "tsyn.h"

static int
checkBigBad(char *buffer)
{
	// DICOM PS3.5 says big-endian is msb first, lsb last
	// However have encountered files where a 32 bit value is sent
	// as big-endian lsb 16 bits, then big-endian msb 16 bits :(

	Uint32 goodvalue;
	Uint32 badvalue;

	goodvalue =  (Uint32)buffer[0];
	goodvalue <<= 8;
	goodvalue |= (Uint32)buffer[1];
	goodvalue <<= 8;
	goodvalue |= (Uint32)buffer[2];
	goodvalue <<= 8;
	goodvalue |= (Uint32)buffer[3];

	badvalue  =  (Uint32)buffer[2];
	badvalue  <<= 8;
	badvalue  |= (Uint32)buffer[3];
	badvalue  <<= 8;
	badvalue  |= (Uint32)buffer[0];
	badvalue  <<= 8;
	badvalue  |= (Uint32)buffer[1];

	// assume that the lower value is the correct one
	// (the check is applyed to the first entry which
	// should be a group length (IdentifyingGroup) that
	// is VERY unlikely to be long enough to break this

	return badvalue < goodvalue;
}

DC3TransferSyntax *
DC3ifstream::GuessTransferSyntax(ostream *verbose)
{
	// if	1st 4 bytes == "DICM"           -> DICOM metaheader
	// else	skip 128 then 4 bytes == "DICM" -> DICOM metaheader
	//	metaheader has specified syntax, defines rest of file
	//      in old part 10 drafts was default implicit vr little endian
	//      subsequently changed to explicit vr - test for either
	// else no DICOM metaheader:
	//	check for leading garbage (eg. Siemens SPI 128 bytes preamble)
	//	if 1st 2 bytes != 0x0008 or 0x0800 skip 128 bytes & look again
	//		if next 2 bytes != 0x0008 or 0x0800 then back to start
	//
	//	assume only valid groups are >= 0x0008 <= 0x07ff
	//		if 1st byte >07 then little-endian else big-endian
	//		if 5,6th bytes uppercase then explicit else implicit
	//		(ie. all valid VR tags are 2 letter uppercase ascii
	//		 and implicit lengths are long so usually zero here)

	// if can't decide, uses default implicit little endian

	int metaheader		= 0;
	int skip128		= 0;
	int startswith_0008	= 0;
	int bigendian		= 0;
	int bigbad		= 0;
	int explicitvr		= 0;

	char b[8];
	read(b,8);
	if (strncmp(b,"DICM",4) == 0) {
		metaheader=1;
	}
	else if (b[0] == 0x00 && b[1] == 0x08 
	      || b[1] == 0x00 && b[0] == 0x08) {
		startswith_0008=1;
		if (b[0] < 0x08) bigendian=1;
		if (isupper(b[4]) && isupper(b[5])) explicitvr=1;
		else bigbad=checkBigBad(b+4);
	}


	if (!metaheader && !startswith_0008) {
		// skip potential 128 byte preamble (DICOM or Siemens SPI)
		if (rdbuf()->seekoff(128,ios::beg,ios::in|ios::out) != EOF) {
			read(b,8);
			if (strncmp(b,"DICM",4) == 0) {
				metaheader=1;
			}
			else if (b[0] == 0x00 && b[1] == 0x08 
			      || b[1] == 0x00 && b[0] == 0x08) {
				skip128=1;
				if (b[0] < 0x08) bigendian=1;
				if (isupper(b[4]) && isupper(b[5]))
					explicitvr=1;
				else bigbad=checkBigBad(b+4);
			}
			// else use existing best guess
		}
	}

	clear();	// Clear EOF state in case too short to seek

	DC3TransferSyntax *ts;

	if (metaheader) {
		bigendian=0;
		// check what metaheader transfer syntax to use and
		// set transfer syntax ... will be changed later
		ts = new DC3TransferSyntax(
			DC3ExplicitVRLittleEndianTransferSyntax);
		explicitvr=1;
		if (rdbuf()->seekoff(128+4,ios::beg,ios::in|ios::out) != EOF) {
			read(b,6);
			if (!(isupper(b[4]) && isupper(b[5]))) { // old (draft)
				delete ts;
				ts = new DC3TransferSyntax(
				    DC3ImplicitVRLittleEndianTransferSyntax);
				explicitvr=0;
			}
		}
			
		// leaves positioned at start of metaheader
		rdbuf()->seekoff(128+4,ios::beg,ios::in|ios::out);
	}
	else {
		enum DC3Endian endian =
			bigendian ? (bigbad ? BIGBAD : BIG)
				  : LITTLE;
		enum DC3VRType vrtype = explicitvr ? EXPLICIT : IMPLICIT;
		enum DC3Encapsulated encap = NOTENCAPSULATED;

		ts = new DC3TransferSyntax(endian,vrtype,encap);

		if (skip128) 	rdbuf()->seekoff(128,ios::beg,ios::in|ios::out);
		else 		rdbuf()->seekoff(0,ios::beg,ios::in|ios::out);
	}
	// now positioned at start of valid DICOM attributes

	if (verbose) {
		*verbose << "MetaHeader: "
			 << (metaheader ? "present" : "absent") << "\n";
		*verbose << "Skip: "
			 << (skip128 ? (metaheader ? 128 : 132) : 0) << "\n";
		*verbose << (metaheader ? "MetaHeader " : "") 
			 << "Endian: "
			 << (bigendian ? "big" : "little")
			 << (bigbad    ? " (bad)" : "")
			 << "\n";
		*verbose << (metaheader ? "MetaHeader " : "") 
			 << "ValueRepresentation: "
			 << (explicitvr ? "explicit" : "implicit") << "\n";
		*verbose << (metaheader ? "MetaHeader " : "") 
			 << "Encapsulated: no\n";
	}

	return ts;
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
