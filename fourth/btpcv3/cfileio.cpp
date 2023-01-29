//
//
// Binary Tree Predictive Coding.
// John Robinson, 1994, 1995.
//

#ifndef DTDEPEND
#include <stdio.h>
#endif

#include <bcompact.h>

cfileio::cfileio(						// constructor where buffer is in memory
      unsigned char *initbuf, 
      int length)
	{ 
	  cfp = NULL; 
	  outbyte = 0; 
	  outbits = 0;
	  charsleft = length; 
	  pbuf = initbuf;
      outbuf = initbuf;
      nout = 0;
	  valid = 0;
    }
cfileio::
cfileio(FILE *file) 
    {
 
        nout = 0;
		cfp = file; outbyte = 0; outbits = 0;
		charsleft = 0; valid = 0; 
	}
void 
cfileio::
putbit(int val)
	{
		outbyte <<= 1;
		if (val)
			outbyte++;
		outbits++;
		if ((outbits &= 0x7) == 0)
			MYPUTC(outbyte, cfp);
	}
void 
cfileio::
putbits(int val, int numbits) 
    {
		int space = 8-outbits;
		if (numbits > space) 
        {
			outbyte <<= space;
			outbyte |= val>>(numbits = numbits-space);
			MYPUTC(outbyte, cfp);
			outbits = 0;
			space = 8;
			val &= (1<<numbits) - 1;
			if (numbits > space) 
            { 
			// Now space = 8 and outbyte is empty
			    outbyte = val>>(numbits = numbits-space);
			    MYPUTC(outbyte, cfp);
			    val &= (1<<numbits) - 1;
			}
		}
		// Now guaranteed that numbits <= space
		outbyte <<= numbits;
		outbyte |= val;
		outbits += numbits;
		if ((outbits &= 0x7) == 0)
			MYPUTC(outbyte, cfp);
    }
void 
cfileio::
putbyte(unsigned char val) {
		int thisbyte = 8 - outbits;
		outbyte <<= thisbyte;
		outbyte |= (val >> outbits);
		MYPUTC(outbyte, cfp);
		outbyte = val;
		}
void 
cfileio::
flush() {
		if (outbits) {
			outbyte <<= 8 - outbits;
			MYPUTC(outbyte ,cfp);
			outbits = 0;
			}
		fflush(cfp);
		}
int 
cfileio::
getbit() {
		if (!valid) {
			bitbuf = (charsleft-- ? *pbuf++ : 
				(fillbuf() && charsleft--) ? *pbuf++ : 0);
			valid = 8;
			}
		return (int) ((bitbuf >> --valid) & 1);
		}
cfileio::
~cfileio() {}
int 
cfileio::
fillbuf() {
		return(charsleft = (cfp == NULL)? 0: fread((pbuf = buf),1,2048,cfp));
		}
void 
cfileio::
loadforlook()
        {
		bitbuf = (bitbuf<<8) + (charsleft-- ? *pbuf++ : 
			(fillbuf() && charsleft--) ? *pbuf++ : 0);
		}
int 
cfileio::
lookbits(const int& bits, const int& mask) 
        {
		if (valid < bits)
			{
			loadforlook();
			valid += 8;
			}
		if (valid < bits)
			{
			loadforlook();
			valid += 8;
			}
		return (int) ((bitbuf >> (valid-bits)) & mask);
		}
void 
cfileio::
skipbits(const int bits) 
        {
		valid -= bits;
		}
int 
cfileio::
get(unsigned char *declength, unsigned char *decode, 
	    register int bits, const int mask) 
        {
		register int v = valid;
		if (v < bits) {
			loadforlook();
			if ((v += 8) < bits) {
				loadforlook();
				v += 8;
				}
			}
		bits = (int) ((bitbuf >> (v-bits)) & mask);
		valid = v - declength[bits];
		return decode[bits];
		}

