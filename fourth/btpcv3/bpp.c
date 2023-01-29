/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




//
// Simple calculation of bits per pel
// John Robinson, 1995
// Uses fact that the image is a P5 or P6 pnm file to decide which argument
// is the image. This is handy because I often forget the argument order.
//
#include <fstream.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
void pgmword(ifstream *pin, char *cbuf)
// Read next PGM word, ignoring comments and whitespace
	{
	char c;
	while(1) {
	    do 
		pin->get(c);
	    while(c == ' ' || c == '\t' || c == '\n' || c == '\r');
	    if (c == '#') {
		do
			pin->get(c);
		while (c != '\n');
		}
	    else break;
	    }
	*cbuf++ = c;
	while ((c = pin->peek())!= ' ' && c != '\t' && c != '\n' && c != '\r'
	  && c != '#')
	    pin->get(*cbuf++);
	*cbuf = '\0';
	}
ifstream *readpgmheader(char *pgmname, int& rows, int& cols, int& comps)
// Read "P5" pgm or "P6" ppm format header.
	{
	char cbuf[32];
	int maxval;	// Absorbs maxval parameter
	unsigned char c;
	ifstream *pin = new ifstream(pgmname);
	if (pin->bad()) {
		cerr << "Cannot open " << pgmname << '\n';
		comps = 0;
		return 0;
		}
	pin->get(cbuf[0]);
	pin->get(cbuf[1]);
	cbuf[2] = 0;
	if (!strcmp(cbuf,"P5")) 
		comps = 1;
	else if (!strcmp(cbuf,"P6"))
		comps = 3;
	else {
		// This one is not the pnm file
		comps = 0;
		return pin;
		}
	pgmword(pin, cbuf);
	cols = atoi(cbuf);
	pgmword(pin,cbuf);
	rows = atoi(cbuf);
	pgmword(pin,cbuf);
	maxval = atoi(cbuf);
	// absorb final whitespace char before start of data
	pin->get(c);
	return pin;
	}
int main(int argc, char *argv[]) {
	if (argc != 3) {
		cerr << "Usage: bpp orig.pnm compressed\n";
		return -1;
		}
	int rows, cols;
	int components;
	ifstream *in, *out;
	ifstream *comp;
	in = readpgmheader(argv[1],rows,cols,components);
	if (!in)
		return 0;
	int temprows, tempcols, tempcomponents;
	comp = out = readpgmheader(argv[2],temprows,tempcols,tempcomponents);
	if (!out)
		return 0;
	if (!components){	// In wasn't the pnm file
		if (!tempcomponents) {
			cerr << "Neither argument was a pnm file\n";
			return 0;
			}
		rows = temprows;
		cols = tempcols;
		components = tempcomponents;
		comp = in;
		}
	else {
		if (tempcomponents) {
			cerr << "Both arguments were pnm files\n";
			return 0;
			}
		}
	comp->seekg(0, istream::end);
	float compsize = comp->tellg();
	cout << compsize*8.0/((float)rows*cols);
	// Note no division by number of components in previous line
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
