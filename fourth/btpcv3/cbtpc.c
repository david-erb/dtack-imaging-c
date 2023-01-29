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
// Eight+one band binary tree predictive encoder
//
// Version 3. Includes static huffman coding and new tree structure
//
// Copyright (c) John A Robinson. 1994, 1995.
//
#include <fstream.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <bcompactr.h>

void rgb_ycc(unsigned char ***components,int ymax,int xmax, int smooth);
int makescratch(const int& xmax, const int& ymax);
int deletescratch(const int& ymax);
void btpc_enc_plane(unsigned char **in, const int& xmax, const int& ymax,
const int& fromlevel, const int& tolevel, const int *spacing, 
Compactr *codeout, const int& clipval);

//
// Calculation of quantization levels from quality parameter
//
int calcq(int maxval, int quality, int *qvals) {
	if ((quality > 100) || (quality < 0))
		return -1;
	int topval = (100-quality)/2;
	// topval will be the quantizer spacing on the finest band
	topval *= maxval;
	topval /= 255;
	if (!topval) {	// Quality == 100 or scaling makes it so
		for (int i = 0; i < 8; i++)
			*qvals++ = 1;
		return 1;
		}
	float qratio;
	if (topval >= 40)
		{
		topval += (topval-25) + 3*(topval-40);
		qratio = 0.75;
		}
	else if (topval > 25)
		{
		topval += (topval-25);
		qratio = 0.75;
		}
	else
		qratio = 0.8;
	qvals[7] = topval;
	for (int i = 6; i >= 0; i--)
		qvals[i] = (int)((((float)qvals[i+1])*qratio)+0.99999);
	return 1;
	}



void pgmword(ifstream& in, char *cbuf)
// The PGM man page says you have to be as lenient as possible about pgm
// files, so this is my general-purpose lenient pgm word reader
// Read next PGM word, ignoring comments and whitespace
	{
	char c;
	while(1) {
	    do 
		in.get(c);
	    while(c == ' ' || c == '\t' || c == '\n' || c == '\r');
	    if (c == '#') {
		do
			in.get(c);
		while (c != '\n');
		}
	    else break;
	    }
	*cbuf++ = c;
	while ((c = in.peek())!= ' ' && c != '\t' && c != '\n' && c != '\r'
	  && c != '#')
	    in.get(*cbuf++);
	*cbuf = '\0';
	}

int readpnmandpad(char *pgmname, int& rows, int& cols, 
	int& maxval, unsigned char ***pbuf)
// Read picture in "P5" or "P6" format into arrays. Pad the picture size up to
// the next multiple of 16.
	{
	char cbuf[32];
	unsigned char c;
	int retval = 0;
	ifstream in(pgmname);
	if (!in) {
		cerr << "Cannot open " << pgmname << '\n';
		return 0;
		}
	in.get(cbuf[0]);
	in.get(cbuf[1]);
	cbuf[2] = 0;
	if (!strcmp(cbuf,"P5")) 
		// PGM image
		retval = 1;
	else if (!strcmp(cbuf,"P6"))
		// PPM image
		retval = 3;
	else {
		cerr << "Sorry. This version of cbtpc only works with ";
		cerr << "raw format (P5 or P6) PGM or PPM files.\n";
		cerr << pgmname << " is not a valid raw format file\n";
		return 0;
		}
	pgmword(in, cbuf);
	cols = atoi(cbuf);
	pgmword(in,cbuf);
	rows = atoi(cbuf);
	pgmword(in,cbuf);
	maxval = atoi(cbuf);
	if (maxval > 255)	// Shouldn't happen in a valid raw pgm file
		maxval = 255;	// ... but just in case it does
	// absorb final whitespace char before start of data
	in.get(c);
	// Make xmax and ymax equal to next highest multiple of 16
	int ymax = ((rows+15)>>4)<<4;
	int xmax = ((cols+15)>>4)<<4;
	if (retval == 1)	// PGM straightforward
	    {
	    unsigned char **buf;
	    *pbuf = buf = new unsigned char *[ymax];
	    for (int i = 0; i < rows; i++) {
		buf[i] = new unsigned char[xmax];
		in.read(buf[i], cols);
		for (int j = cols; j < xmax; j++)	// Pad image by
			buf[i][j] = buf[i][cols-1];	// copying last pel
		}
	    // Vertical padding
	    for (i = rows; i < ymax; i++) {
		buf[i] = new unsigned char[xmax];	// Pad image by
		memcpy(buf[i], buf[rows-1], xmax);	// copying last line
		}
	    }
	else			// PPM
	    {
	    unsigned char **bufr, **bufg, **bufb;
	    unsigned char *tempbuf = new unsigned char[3*xmax];
	    unsigned char *pr, *pg, *pb, *ptemp;
	    *pbuf++ = bufg = new unsigned char *[ymax];
	    *pbuf++ = bufr = new unsigned char *[ymax];
	    *pbuf = bufb = new unsigned char *[ymax];
	    // This loop incorporates vertical padding
	    for (int i = 0; i < ymax; i++){
		pg = bufg[i] = new unsigned char[xmax];
		pr = bufr[i] = new unsigned char[xmax];
		pb = bufb[i] = new unsigned char[xmax];
		if (i < rows)
			in.read(tempbuf,cols*3);
		ptemp = tempbuf;
		for (int j = 0; j < cols; j++) {
			*pr++ = *ptemp++;
			*pg++ = *ptemp++;
			*pb++ = *ptemp++;
			}
		for (j = cols; j < xmax; j++) {	// Pad
			*pr++ = bufr[i][cols-1];
			*pg++ = bufg[i][cols-1];
			*pb++ = bufb[i][cols-1];
			}
		}
	    delete [] tempbuf;
	    }
	in.close();
	return retval;
	}


int main(int argc, char *argv[])
	{
	if ((argc != 3)&&(argc != 4))
		{
		cerr << "Usage: cbtpc pgm_inpic code_file [quality]\n";
		return -1;
		}
	unsigned char **components[3];
	unsigned char **in;
	int truerows, truecols;
	int maxval;
	int pnmtype;
	char coltype = 'G';	// GRB or monochrome
	char uvfilter = '0';	// No downsampling
	// Get picture into unsigned char arrays
	pnmtype = readpnmandpad(argv[1],truerows,truecols,maxval,components);
	if (!pnmtype)
		return -1;
	// Make xmax and ymax equal to next highest multiple of 16
	int ymax = ((truerows+15)>>4)<<4;
	int xmax = ((truecols+15)>>4)<<4;
	int spacing[9];	// Quantizer spacings
	int quality = 75;
 	if (argc == 4)
		quality = atoi(argv[3]);
	if (calcq(maxval, quality, spacing+1) < 0) {
		cerr << "Quality parameter must be between 0 and 100\n";
		return -1;
		}

  int i;
  printf("spacing: quality=%d", quality);
  for (i=1; i<=8; i++)
    printf(" 0x%02x", spacing[i]);
  printf("\n");

	if ((pnmtype == 3)&&(spacing[8] >= 2))
		{
		coltype = 'Y';
		uvfilter = 'B';	// Box filter
		rgb_ycc(components,ymax,xmax,1);
		}
	FILE *cfp = fopen(argv[2],"w");
	if (cfp == 0)
		{
		cerr << "Can't open " << argv[2] << "\n";
		return -1;
		}
	// Write header
	if (pnmtype == 1)	
		fprintf(cfp,"btpc 3.a14%cG0\n%d\n%d\n", 
			NUMCODERS+'0', truerows, truecols);
	else
		fprintf(cfp,"btpc 3.a34%c%c%c\n%d\n%d\n", 
			NUMCODERS+'0', coltype, uvfilter, truerows, truecols);
	// Output all the quantizer spacing parameters
	for (i = 1; i <=8; i++)
		fputc((unsigned char) spacing[i], cfp);
	// And the maxval parameter
	fputc((unsigned char) maxval, cfp);
	// Set up the huffman coders
	cfpio fio(cfp);
	Compactr codeout[NUMCODERS];
	for (i = 0; i < NUMCODERS; i++)
		codeout[i].attach(&fio);
	// Encode each plane of the picture
	makescratch(xmax, ymax);
	for (int compnum = 0; compnum < pnmtype; compnum++) {
	    int tolevel = (compnum && (uvfilter != '0'));
	    int clipval = tolevel ? 255 : maxval;
	    btpc_enc_plane(components[compnum],xmax,ymax,4,
		tolevel, spacing, codeout, clipval);
	    }
	fio.flush();
	fclose(cfp);
	// And finally, clean up
	for (compnum = 0; compnum < pnmtype; compnum++) {
	    in = components[compnum];
	    for (i = 0; i < ymax; i++)
		delete [] in[i];
	    delete [] in;
	    }
	deletescratch(ymax);
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
