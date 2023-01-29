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
// Eight+one-band binary tree predictive decoder
//
// Version 3.
//
// Copyright (c) John Robinson. 1994, 1995.
//
#include <stdlib.h>
#include <string.h>
#include <compactr.h>
unsigned char *initclip(const int& topval);
void ycc_rgb(unsigned char ***components, const int& rows, const int& cols,
const unsigned char *clip, const char& expand);
void btpc_dec_plane(unsigned char **out,const int& XMAX, const int& YMAX,
const int& fromlevel, const int& tolevel, const int *spacing,
Compactr *codein, const int& clipval);


int writepnm(const char *pgmname, const unsigned char ***pbuf, 
const int& rows, const int& cols, const int& maxval, const int& pnmtype)
// Write picture to "P5" pgm or "P6" ppm format
	{
	FILE *out = fopen(pgmname, "w");
	if (!out) {
		fprintf(stderr,"Cannot open %s\n", pgmname);
		exit (0);
		}
	if (pnmtype == 1) {	// PGM
	    fprintf(out,"P5\n%d %d\n%d\n", cols, rows, maxval);
	    for (int i = 0; i < rows; i++)
		fwrite(pbuf[0][i],1,cols,out);
	    }
	else { // PPM
	    fprintf(out,"P6\n%d %d\n%d\n", cols, rows, maxval);
	    unsigned char *temp = new unsigned char[3*cols];
	    for (int i = 0; i < rows; i++) {
		unsigned char *ptemp = temp;
		const unsigned char *pg = pbuf[0][i];
		const unsigned char *pr = pbuf[1][i];
		const unsigned char *pb = pbuf[2][i];
		for (int j = 0; j < cols; j++) {
			*ptemp++ = *pr++;
			*ptemp++ = *pg++;
			*ptemp++ = *pb++;
			}
		fwrite(temp,1,cols*3,out);
		}
	    delete [] temp;
	    }
	fclose(out);
	return 0;
	}

int btpc_read_header(FILE *cfp, int& pnmtype, int& rows, int& cols, 
int& maxval, int& numlevels, char& coltype, char& uvfilter, int *spacing) {
	char cbuf[12];
	char newline;
	int n_coders;
	fread(cbuf,1,7,cfp);
	cbuf[7] = 0;
	if (strcmp(cbuf,"btpc 3.")) {
		fprintf(stderr, "This is not a valid btpc 3 file\n");
		return -1;
		}
	fscanf(cfp,"%s %d %d%c",cbuf, &rows,&cols,&newline);
	if (cbuf[0] == 'a') {
		pnmtype = cbuf[1] - '0';// 1 for PGM, 3 for PPM
		numlevels = cbuf[2] - '0';
		n_coders = cbuf[3] - '0';
		coltype = cbuf[4];	// 'G'=GRB, 'Y'=YUV
		uvfilter = cbuf[5];	// 'B'=box, '0'=none
		if (numlevels > 4) {
			fprintf(stderr, "Sorry. This picture is coded with ");
			fprintf(stderr, "%d levels. I can handle ", numlevels);
			fprintf(stderr, "up to 4 only\n");
			return -1;
			}
		if (n_coders != NUMCODERS) {
			fprintf(stderr, "Sorry. This picture is coded with ");
			fprintf(stderr, "%d coders. I can handle ", n_coders);
			fprintf(stderr, "%d coders only\n",NUMCODERS);
			return -1;
			}
		}
	else {
		fprintf(stderr, "This is a version of btpc 3 that I don't ");
		fprintf(stderr, "recognize.\nI'm going to guess some ");
		fprintf(stderr, "parameters to try\n");
		fprintf(stderr, "to recover the monochrome component.\n");
		pnmtype = 1;
		numlevels = 4;
		coltype = 'G';
		uvfilter = '0';
		}
	for (int i = 1; i <= numlevels*2; i++)
		spacing[i] = fgetc(cfp);
	maxval = fgetc(cfp);
	return 0;
	}

int main(int argc, char *argv[])
	{
	if (argc != 3) {
		fprintf(stderr, "Usage: dbtpc code_file pgm_outpic\n");
		return -1;
		}
	// Use old stdlib for input because it seems to be more efficient
	FILE *cfp = fopen(argv[1],"r");
	if (!cfp) {
		fprintf(stderr, "Can't open %s\n", argv[1]);
		return -1;
		}
	int pnmtype, truerows, truecols, maxval, numlevels;
	char coltype, uvfilter;
	int spacing[65];
	if (btpc_read_header(cfp, pnmtype, truerows, truecols,  maxval, 
	    numlevels, coltype, uvfilter, spacing))
		return - 1;
	// Make XMAX and YMAX equal to next highest multiple of 16
	int YMAX = ((truerows+15)>>4)<<4;
	int XMAX = ((truecols+15)>>4)<<4;
	// Set up coders
	cfpio fio(cfp);
	Compactr codein[NUMCODERS];
	for (int i = 0; i < NUMCODERS; i++)
		codein[i].attach(&fio);
	// Allocate space for decoded picture
	// If decoding into an existing array (e.g. for immediate display)
	// create line pointers out[i], and point at rows of array.
	unsigned char **components[3];
	unsigned char **out;
	for (int compnum = 0; compnum < pnmtype; compnum++)
	    {
	    components[compnum] = out = new unsigned char*[YMAX];
            for (i = 0; i < YMAX; i++) {
                out[i] = new unsigned char[XMAX];
            	memset(out[i],0,XMAX);
		}
	    }
	// Decode each plane of the picture
	for (compnum = 0; compnum < pnmtype; compnum++){
	    int tolevel = (compnum && (uvfilter != '0'));
	    int clipval = tolevel ? 255 : maxval;
	    // Above line ensures that UV components are correctly clipped
printf("%d %d %d %d\n", numlevels, tolevel, codein, clipval);
	    btpc_dec_plane(components[compnum],XMAX,YMAX,numlevels,
		tolevel, spacing, codein, clipval);
	    }
	// Convert colourspace back if necessary
	if (coltype == 'Y') {
		unsigned char *clip = initclip(maxval);
		ycc_rgb(components,truerows,truecols,clip,uvfilter);
		}
	writepnm(argv[2], components, truerows, truecols, maxval,pnmtype);
	for (compnum = 0; compnum < pnmtype; compnum++) {
		out = components[compnum];
		for (i = 0; i < YMAX; i++) 
			delete [] out[i];
		delete [] out;
		}
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
