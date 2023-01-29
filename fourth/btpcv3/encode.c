/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <btpcv3.h>

//
// Eight+one band binary tree predictive encoder
//
// Version 3. Includes static huffman coding and new tree structure
//
// Copyright (c) John A Robinson. 1994, 1995.
//
#ifndef DTDEPEND
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#endif

#include <cfpio.h>

#include <compactr.h>

#include <internal.p>

#include <colmap.p>

#include <btpc.h>


//
// Calculation of quantization levels from quality parameter
//
static
int 
calcq(int maxval, int quality, int *qvals) 
{
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


/*..........................................................................
 * xe and ye must be multiples of 16
 * if picture is color and there must be three arrays of row pointers
 *..........................................................................*/

btpc_rc_e
DTCONFIG_API1
btpc_encode(
  unsigned char ***rows,
  int xe, 
  int ye, 
  int maxval, 
  int pnmtype, 
  int quality,
  char *b,				// buffer containing file
  int length,				// buffer length
  int *used)
{
  char coltype = 'G';			// GRB or monochrome
  char uvfilter = '0';			// No downsampling
  int i;

  int spacing[9];			// Quantizer spacings
  if (calcq(maxval, quality, spacing+1) < 0) 
    return BTPC_RC_BADQUALITY;

#ifdef NONO
  printf("spacing: quality=%d", quality);
  for (i=1; i<=8; i++)
    printf(" 0x%02x", spacing[i]);
  printf("\n");
#endif

  if (pnmtype == 3 &&
      spacing[8] >= 2)
  {
    coltype = 'Y';
    uvfilter = 'B';			// Box filter
    rgb_ycc(rows, ye, xe, 1);
  }

  // Write header
  sprintf(b, "btpc 3.a%d4%c%c%c\n%d\n%d\n", 
    pnmtype,
    NUMCODERS+'0', 
    coltype, uvfilter, 
    ye, xe);

  length -= strlen(b);
  *used = strlen(b);
  b += strlen(b);

  // Output all the quantizer spacing parameters
  for (i=1; i<=8; i++)
    b[i-1] = spacing[i];
  
  // And the maxval parameter
  b[i-1] = maxval;

  length -= 9;
  *used = *used + 9;
  b += 9;
  
  // Set up the huffman coders
  cfpio fio((unsigned char *)b, length);
  if (fio.constructor_failed)
    return BTPC_RC_BADMALLOC;

  class Compactr codeout[NUMCODERS];
  for (i = 0; i < NUMCODERS; i++)
  {
	if (codeout[i].constructor_failed)
	  return BTPC_RC_BADMALLOC;
    codeout[i].attach(&fio);
  }
  
  // Encode each plane of the picture
  makescratch(xe, ye);

  for (int compnum = 0; compnum < pnmtype; compnum++)
  {
    int tolevel = (compnum && (uvfilter != '0'));
    int clipval = tolevel ? 255 : maxval;
    btpc_enc_plane(rows[compnum],xe, ye, 4,
      tolevel, spacing, codeout, clipval);
  }	

  fio.flush();

  // And finally, clean up
  deletescratch(ye);

  *used = *used + fio.nout;

  return BTPC_RC_GOOD;
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
