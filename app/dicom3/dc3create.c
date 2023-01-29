/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)dc3create.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include "GetOpt.h"
#include "MainCmd.h"

#include <iostream.h>
#include <iomanip.h>
#include <strstream.h>
#include <string.h>
#include <ctype.h>

typedef unsigned short Uint16;
typedef unsigned long  Uint32;

static void
write32(ostream& out,Uint32 u,int big)
{
	// cerr << "write32: " << u << "\n" << flush;
	unsigned char buffer[4];
	if (big) {
		buffer[0]=(unsigned char)(u>>24);
		buffer[1]=(unsigned char)(u>>16);
		buffer[2]=(unsigned char)(u>>8);
		buffer[3]=(unsigned char)u;
	}
	else {
		buffer[3]=(unsigned char)(u>>24);
		buffer[2]=(unsigned char)(u>>16);
		buffer[1]=(unsigned char)(u>>8);
		buffer[0]=(unsigned char)u;
	}
	out.write(buffer,4);
}

static void
write16(ostream& out,Uint16 u,int big)
{
	// cerr << "write16: " << u << "\n" << flush;
	unsigned char buffer[4];
	if (big) {
		buffer[0]=(unsigned char)(u>>8);
		buffer[1]=(unsigned char)u;
	}
	else {
		buffer[1]=(unsigned char)(u>>8);
		buffer[0]=(unsigned char)u;
	}
	out.write(buffer,2);
}

/* Recognized commands are:

(hex group,hex element) anything VR=<vr> VL=<hex vl> [hexvals]
(hex group,hex element) anything VR=<vr> VL=<hex vl> <strvals>
%implicit
%explicit
%little
%big
datavalues

VL will be replaced with actual VL unless OB,OW or SQ

*/

static void
parseError(char *str,char c)
{
	cerr << "Parse error in entry - \"" << str 
	     << "\" expected, got \'" << c << "\'\n" << flush;
}

static int
getCommand(istream& in)
{
	int command=0;
	const unsigned buflng = 10;
	char buffer[buflng];
		char c;
		in >> c;
		if (!in.good()) return 0;;
		switch (c) {
			case '(':
				command='(';
				break;
			case '%':
				in >> setw(buflng-1) >> buffer;
				if (!in.good()) break;
				if (strcmp(buffer,"implicit") == 0) command='i';
				if (strcmp(buffer,"explicit") == 0) command='e';
				if (strcmp(buffer,"little") == 0)   command='l';
				if (strcmp(buffer,"big") == 0)      command='b';
				if (strcmp(buffer,"byte") == 0)     command='B';
				if (strcmp(buffer,"short") == 0)    command='S';
				if (strcmp(buffer,"word") == 0)     command='S';
				if (strcmp(buffer,"long") == 0)     command='L';
				break;
			default:
				if (isdigit(c)) {
					command=c;
					in.putback(c);
				}
				else {
					parseError("attribute,command or value",
						c);
				}
				break;
		}
	return command;
}

static void
getHex32(istream& in,Uint32& v)
{
	char c;
	int i;

	in >> hex >> v;
	if (v == 0 && (i=in.peek()) == 'x') {
		in.get(c);
		in >> hex >> v;
	}
	if (!in.good()) {
		parseError("hex number",0);
	}
	else {
		// cerr << "getHex32: " << v << "\n" << flush;
	}
}

static void
getHex16(istream& in,Uint16& v)
{
	Uint32 v32;
	getHex32(in,v32);
	v=(Uint16)v32;
}


static int
getParsedEntry(istream& in,Uint16& group,Uint16& element,
	char *VR,Uint32& VL,int& valuetype,char **valuestring)
{
	char c;
	const unsigned buflng = 1024;
	char buffer[buflng];	// enough for longest value string
	char *p;

	// Group,Element,VL are ALWAYS hex regardless of what they look like.

	// First '(' has already been read as command

	// Find "nnnn,nnnn)" ...

	getHex16(in,group);
	if (!in.good()) {
		return -1;	// Failure
	}
	in >> c;
	if (c != ',') {
		parseError(",",c);
		return -1;	// Failure
	}
	getHex16(in,element);
	if (!in.good()) {
		return -1;	// Failure
	}
	in >> c;
	if (c != ')') {
		parseError(")",c);
		return -1;	// Failure
	}
	// Skip any fields (eg. from dc3dump) found "VR=<xx>" ...
	do {
		in >> buffer;
	} while (in.good() && strncmp(buffer,"VR",2) != 0);

	if (!in.good()) {
		parseError("VR",0);
		return -1;	// Failure
	}
	for (p=buffer; *p && *p != '<';++p);
	strncpy(VR,p+1,2);
	if (*(p+3) != '>') {
		parseError("> after VR",*(p+3));
		return -1;	// Failure
	}

	// Find "VL=<nn>" ... 	(nn is in hex)

	do {
		in >> buffer;
	} while (in.good() && strncmp(buffer,"VL",2) != 0);

	if (!in.good()) {
		parseError("VL",0);
		return -1;	// Failure
	}
	for (p=buffer; *p && *p != '<';++p);
	istrstream istr(p+1);
	getHex32(istr,VL);
	if (!in.good()) {
		return -1;	// Failure
	}
 	istr >> c;
	if (c != '>') {
		parseError("> after VL",c);
		return -1;	// Failure
	}

	in >> c;
	char traildelim;
	switch (c) {
		case '<':
			traildelim='>';
			break;
		case '[':
			traildelim=']';
			break;
		default:
			parseError("<values> or [values]",c);
			return -1;	// Failure
	}
	valuetype=c;
	for (p=buffer; p<buffer+buflng; ++p) {
		in.get(c);	// Don't skip whitespace
		if (!in.good()) {
			parseError("> or ] after value",0);
			return -1;	// Failure
		}
		else {
			if (c == traildelim) {
				*p=0;
				break;
			}
			else {
				*p=c;
			}
		}
	}
	*valuestring=new char[strlen(buffer)+1];
	strcpy(*valuestring,buffer);

	return 0;		// Success
}

static void
writeAttribute(ostream& out,Uint16 group,Uint16 element,
	char *vr,Uint32 vl,int type,char *string,
	int explicitvr,int big)
{
	write16(out,group,big);
	write16(out,element,big);

	Uint32 actualvl;
	switch (type) {
		case '<':	// string
			if ((actualvl=strlen(string))%2) ++actualvl;
			break;
		case '[':	// hex
			if (*string) {
				char *p;
				for (actualvl=1,p=string; *p; ++p)
					if (!isxdigit(*p) && *p != 'x') 
						++actualvl;
			}
			else actualvl=0;
			actualvl*=(vr[1] == 'L')?4:2;
			break;
	}

	if (actualvl != vl) {
		if (strcmp(vr,"OB") == 0
		 || strcmp(vr,"OW") == 0
		 || strcmp(vr,"SQ") == 0) {
			actualvl=vl;
		}
		else {
			cerr << "Specified VL " << vl 
			     << " wrong, using " << actualvl
			     << "\n" << flush;
		}
	}

	if (explicitvr) {
		out.write(vr,2);			// 2 byte explicitvr vr
		if (strcmp(vr,"OB") == 0
		 || strcmp(vr,"OW") == 0
		 || strcmp(vr,"SQ") == 0) {
			write16(out,0,big);		// 2 byte reserved
			write32(out,actualvl,big);	// 4 byte value length
		}
		else {
			write16(out,(Uint16)actualvl,big);	// 2 byte length
		}
	}
	else {
		write32(out,actualvl,big);
	}

	char pad = (strcmp(vr,"UI") == 0) ? 0 : ' ';
	switch (type) {
		case '<':	// string
			out << string;
			if (strlen(string)%2) out << pad;
			break;
		case '[':	// hex
			if (*string) {
				istrstream istr(string);
				while (istr.good()) {
					Uint32 u;
					char c;
					getHex32(istr,u);
					istr >> c;		// delimiter
					if (vr[1] == 'L')
						write32(out,u,big);
					else
						write16(out,(Uint16)u,big);
				}
			}
			break;
	}
}

int
main(int argc, char *argv[])
{
	CCOMMAND(argc,argv);

	cerr.setf(ios::showbase,ios::showbase);

	int explicitvr=0;
	int big=0;
	int datawordlength=1;

	GetOpt getopt(argc,argv,"be");
	int c;
	int err=0;
	while ((c=getopt()) != -1 && !err) {
		switch (c) {
			case 'e':
				explicitvr=1;
				break;
			case 'b':
				big=1;
				explicitvr=1;	// no such thing as implicit big
				break;
			case '?':
			default:
				err=1;
		}
	}

	if (getopt.optind != argc) err=1;

	if (err) {
		cerr << "Usage: " << argv[0] << " [-b][-e] <infile >outfile\n"
		     << flush;
		return 1;
	}

	int command;
	while (command=getCommand(cin)) {
		Uint16 group;
		Uint16 element;
		char vr[3];
		Uint32 vl;
		int type;
		char *string;
		switch (command) {
			case '(':
				if (getParsedEntry(cin,group,element,
						vr,vl,type,&string) == 0) {
					writeAttribute(cout,group,element,
						vr,vl,type,string,
						explicitvr,big);
				}
				if (strcmp(vr,"OB") == 0) datawordlength=1;
				if (strcmp(vr,"OW") == 0) datawordlength=2;
				break;
			case 'i':
				explicitvr=0;
				break;
			case 'e':
				explicitvr=1;
				break;
			case 'l':
				big=0;
				break;
			case 'b':
				big=1;
				explicitvr=1;	// no such thing as implicit big
				break;
			case 'B':
				datawordlength=1;
				break;
			case 'S':
				datawordlength=2;
				break;
			case 'L':
				datawordlength=4;
				break;
			default:
				if (isdigit(command)) {
					Uint32 value;
					cin >> resetiosflags(ios::basefield)
					    >> value;
					switch (datawordlength) {
						case 1:
							cout.put((char)value);
							break;
						case 2:
							write16(cout,
							    (Uint16)value,big);
							break;
						case 4:
							write32(cout,value,big);
							break;
					}
				}
				break;
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
