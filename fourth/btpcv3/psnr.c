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
// Calculation of psnr between two images
// John Robinson, 1995
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
// Read "P5" pgm format header.
	{
	char cbuf[32];
	int maxval;	// Absorbs maxval parameter
	unsigned char c;
	ifstream *pin = new ifstream(pgmname);
	if (pin->bad()) {
		cerr << "Cannot open " << pgmname << '\n';
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
		cerr << pgmname << " is not a valid raw format PGM/PPM file\n";
		return 0;
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
		cerr << "Usage: psnr inpic outpic\n";
		return -1;
		}
	int rows, cols;
	int components;
        unsigned char *inbuf;
        unsigned char *outbuf;
	ifstream *in, *out;
	in = readpgmheader(argv[1],rows,cols,components);
	if (!in)
		return 0;
	int temprows, tempcols, tempcomponents;
	out = readpgmheader(argv[2],temprows,tempcols,tempcomponents);
	if (!out)
		return 0;
	if ((tempcols != cols)||(temprows != rows)) {
		cerr << "Images are not the same size\n";
		return 0;
		}
	if (components != tempcomponents) {
		cerr << "One image PPM, one PGM: Comparing first component\n";
		return 0;
		}
        inbuf = new unsigned char[cols*components];
        outbuf = new unsigned char[cols*components];
	float mse = 0.0;
	for (int i = 0; i < rows; i++) {
		unsigned char *p = inbuf;
		unsigned char *q = outbuf;
		int temp;
		int accum = 0;
		in->read(inbuf,cols*components);
		out->read(outbuf,cols*components);
		for (int j = 0; j < cols*components; j++)
			{
			temp = *p++ - *q++;
			accum += temp*temp;
			}
		mse += (float) accum;
		}
	mse /= rows*cols*components;
	float psnr;
	psnr = 255.0*255.0/mse;
	psnr = 10*log10(psnr);
	cout << ' ' << psnr << '\n';
	in->close();
	out->close();
	delete in;
	delete out;
	return 1;
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
