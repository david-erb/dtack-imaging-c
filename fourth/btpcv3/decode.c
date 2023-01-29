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
// Eight+one-band binary tree predictive decoder
//
// Version 3.
//
// Copyright (c) John Robinson. 1994, 1995.
//

#ifndef DTDEPEND
#include <stdio.h>
#include <string.h>
#endif

#include <cfpio.h>
#include <compactr.h>

#include <internal.p>

#include <colmap.p>

#include <btpc.h>

/*..........................................................................
 * check for signature
 * return error code or GOOD
 *..........................................................................*/

int
DTCONFIG_API1
btpc_decode_issig(
  char *b,				// buffer containing file
  int length)				// buffer length
{
  if (length < 7 ||
      memcmp(b, BTPC_SIG3, 7))
    return 0;
  else
    return 1;
}

/*..........................................................................
 * scan header already in memory
 * return error code or GOOD
 *..........................................................................*/

btpc_rc_e
DTCONFIG_API1
btpc_decode_header(
  char *b,				// buffer containing file
  int length,				// buffer length
  int *cols, 
  int *rows, 
  int *maxval, 
  int *pnmtype, 
  int *numlevels, 
  char *coltype, 
  char *uvfilter, 
  int *spacing,
  int *got)
{
  char cbuf[12];
  char newline;
  int n_coders;
  char *b0 = b;

  cbuf[7] = 0;
  if (memcmp(b, BTPC_SIG3, 7))
    return BTPC_RC_BADSIG;

  b += 7;
  int n;
  n = sscanf(b, "%s %d %d%c",
    cbuf, 
    rows,
    cols,
    &newline);
  if (n != 4)
    return BTPC_RC_BADHEAD;

  b = strchr(b, '\n') + 1;
  b = strchr(b, '\n') + 1;
  b = strchr(b, '\n') + 1;

  if (cbuf[0] == 'a')
  {
    *pnmtype = cbuf[1] - '0';		// 1 for PGM, 3 for PPM
    *numlevels = cbuf[2] - '0';
    n_coders = cbuf[3] - '0';
    *coltype = cbuf[4];			// 'G'=GRB, 'Y'=YUV
    *uvfilter = cbuf[5];		// 'B'=box, '0'=none
    if (*numlevels > 4)
      return BTPC_RC_BADLEVELS;
    if (n_coders != NUMCODERS)
      return BTPC_RC_BADCODERS;
  }
  else
    return BTPC_RC_BADVERSION;
  for (int i = 1; i <= *numlevels*2; i++)
    spacing[i] = *b++ & 0xff;
  *maxval = *b++ & 0xff;
  
  *got = b - b0;
  return BTPC_RC_GOOD;
}

/*..........................................................................
 * xe and ye must be multiples of 16
 * if picture is color and there must be three arrays of row pointers
 *..........................................................................*/

btpc_rc_e
DTCONFIG_API1
btpc_decode(
  char *b,				// buffer containing file
  int length,				// buffer length
  int xe, 
  int ye, 
  int maxval, 
  int pnmtype, 
  int numlevels, 
  char coltype, 
  char uvfilter, 
  int *spacing,
  unsigned char ***rows)
{
  // Set up coders
  class cfpio fio((unsigned char *)b, length);
  class Compactr codein[NUMCODERS];
 
  int compnum;
  int i;
  for (i = 0; i < NUMCODERS; i++)
    codein[i].attach(&fio);

  int xe2 = ((xe+15)>>4)<<4;
  int ye2 = ((ye+15)>>4)<<4;

  for (compnum = 0; compnum < pnmtype; compnum++)
  {
    int tolevel = (compnum && (uvfilter != '0'));
    int clipval = tolevel ? 255 : maxval;
    // Above line ensures that UV components are correctly clipped
    btpc_dec_plane(rows[compnum],
      xe2, ye2,
      numlevels,
      tolevel, spacing, codein, clipval);
  }

  // Convert colourspace back if necessary
  if (coltype == 'Y') 
  {
    unsigned char *clip = initclip(maxval);
    ycc_rgb(rows,
      ye, xe,
      clip, uvfilter);
  }

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
