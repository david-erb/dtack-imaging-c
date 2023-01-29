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

#include <dtack/base.h>
#include <cfpio.h>

cfpio::cfpio(
  FILE *file)
{
  buf = (unsigned char *)malloc(2048);
  if (buf == NULL)						/* memory object failed to alloc? 	*/
    this->constructor_failed = 1;
  else
    this->constructor_failed = 0;
  nout = 0;
  cfp = file; outbyte = 0; outbits = 0;
  charsleft = 0; valid = 0; 
}

cfpio::cfpio(	
  unsigned char *initbuf, 
  int length)
{ 
  this->constructor_failed = 0;
  buf = NULL;
  cfp = NULL; 
  outbyte = 0; 
  outbits = 0;
  charsleft = length; 
  pbuf = initbuf;
  outbuf = initbuf;
  nout = 0;
  valid = 0;
}

void 
cfpio::
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
cfpio::
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
cfpio::
putbyte(unsigned char val) {
		int thisbyte = 8 - outbits;
		outbyte <<= thisbyte;
		outbyte |= (val >> outbits);
		MYPUTC(outbyte, cfp);
		outbyte = val;
		}
void 
cfpio::
flush() {
		if (outbits) {
			outbyte <<= 8 - outbits;
			MYPUTC(outbyte ,cfp);
			outbits = 0;
			}
		fflush(cfp);
		}
int 
cfpio::
getbit() {
		if (!valid) {
			bitbuf = (charsleft-- ? *pbuf++ : 
				(fillbuf() && charsleft--) ? *pbuf++ : 0);
			valid = 8;
			}
		return (int) ((bitbuf >> --valid) & 1);
		}
cfpio::
~cfpio() {
  if (this->buf != NULL)
    free(this->buf);
}
int 
cfpio::
fillbuf() {
		return(charsleft = (cfp == NULL)? 0: fread((pbuf = buf),1,2048,cfp));
		}
void 
cfpio::
loadforlook()
        {
		bitbuf = (bitbuf<<8) + (charsleft-- ? *pbuf++ : 
			(fillbuf() && charsleft--) ? *pbuf++ : 0);
		}
int 
cfpio::
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
cfpio::
skipbits(const int bits) 
        {
		valid -= bits;
		}
int 
cfpio::
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






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
