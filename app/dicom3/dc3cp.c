/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)dc3cp.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include "tattrrd.h"
#include "tlist.h"
#include "ttag.h"
#include "tfstream.h"
#include "tddict.h"

#include "GetOpt.h"
#include "MainCmd.h"

int
main(int argc, char *argv[])
{
	CCOMMAND(argc,argv);

	cerr.setf(ios::showbase,ios::showbase);

	char *	stamp 		= 0;
	char *	inname 		= 0;
	char *	outname 	= 0;
	int 	verbose 	= 0;
	int 	reportlevel 	= 1;
	int 	stopbad 	= 1;
	int 	privateremove 	= 0;
	int 	metaheader 	= 0;
	char *	insyntaxUID  	= 0;
	char *	outsyntaxUID 	= DC3DefaultTransferSyntax;

	GetOpt getopt(argc,argv,"bd:i:mo:pr[2]s:vw");
	int c;
	int err=0;
	DC3ListofAttributes deletelist;
	DC3ListofAttributes replacelist;
	while ((c=getopt()) != -1 && !err) {
		switch (c) {
			case 'p':	// Private attributes removed
				privateremove=1;
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
			case 'd':	// Delete attribute keyword
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
			case 'i':	// Input transfer syntax
				insyntaxUID=getopt.optarg;
				break;
			case 'o':	// Output transfer syntax
				outsyntaxUID=getopt.optarg;
				break;
			case 's':	// Stamp with specified date/time
				stamp=getopt.optarg;
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
		cerr << "Usage: " << argv[0] << " [-i syntax] [-o syntax] [-m] "
		     << "[-d key] [-r key value] [-s stamp] [-p] "
		     << "[-v] [-w] [-b] "
		     << "infile outfile\n"
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
			if (verbose) {
				attr->put(cerr);
			}
			list.insert(attr);
		}
		oldtag=newtag;
	}

	if (verbose)
		cerr << "******** Removing Old Metaheader *******\n" << flush;

	list.RemoveMetaHeaderGroup();

	if (verbose)
		cerr << "**************** Deleting **************\n" << flush;

	list.Delete(deletelist);

	if (verbose)
		cerr << "**************** Replacing **************\n" << flush;

	list.Replace(replacelist);

	if (privateremove) {
		if (verbose)
			cerr << "************* Remove Private ************\n" 
			     << flush;
		list.PrivateRemove();
	}

	if (verbose)
		cerr << "**************** Fixing **************\n" << flush;

	list.Fix(outsyntaxUID,metaheader);

	if (verbose)
		cerr << "**************** Writing **************\n" << flush;

	instream.clear();	// Clear EOF state to allow seeking/reading

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
