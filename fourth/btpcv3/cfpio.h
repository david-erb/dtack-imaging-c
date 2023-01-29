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
//
// Binary Tree Predictive Coding.
// John Robinson, 1994, 1995.
//
// Header file for static huffman coder.
//

const int TABLEBITS = 12;
const int TABLESIZE = (1<<TABLEBITS);

#define MYPUTC(BYTE, FILE) \
{ \
  if (FILE != NULL) \
    putc(BYTE, FILE); \
  else \
    outbuf[nout] = BYTE; \
  nout++; \
}

class cfpio {
public:
	FILE	*cfp;
    unsigned char *outbuf;
// Output works a byte at a time and is therefore simple
	int	outbyte;
	int	outbits; // Number of output bits
// Input is double buffered: first get a chunk into buf, then get bits into
// bitbuf
	int charsleft;
	unsigned long	bitbuf;
	int valid;	// bits left in bitbuf
	unsigned char *pbuf;
	unsigned char *buf;
	int constructor_failed;
int 
fillbuf();
void 
loadforlook();
int 
lookbits(const int& bits, const int& mask); 
void 
skipbits(const int bits);
int 
get(unsigned char *declength, unsigned char *decode, 
	    register int bits, const int mask); 

int nout;								// tells how many bytes were output
cfpio(								// constructor where buffer is in memory
      unsigned char *initbuf, 
      int length);
cfpio(FILE *file) ;
void 
putbit(int val);
void 
putbits(int val, int numbits) ;
void 
putbyte(unsigned char val);
void 
flush();
int 
getbit();
~cfpio();
};





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
