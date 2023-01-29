/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("app/tableau $RCSfile: rlerb.c,v $ $Revision: 1.8 $");

#define CBITS        (0x07)				/* the control bits 				*/
#define CBITS_ZSMALL (0x00)				/* 1-32 0's 						*/
#define CBITS_ZLARGE (0x01)				/* 33-8192 0's (2 bytes)			*/
#define CBITS_D1     (0x02)
#define CBITS_D2     (0x03)
#define CBITS_D3     (0x04)
#define CBITS_D4     (0x05)
#define CBITS_D5     (0x06)
#define CBITS_DN     (0x07)				/* 6-256 data's (2 bytes)			*/

#define ZERO (128)

#ifdef NONO
static int nprint = 0;
#define DP(P) if (nprint++ >= 0) P
#define D0 nprint = 0
#else
#define DP(P)
#define D0
#endif

/*..........................................................................
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_rlerb_encode(
  const void *vin,
  dt1d_i_t nin,
  void *vout,
  dt1d_i_t maxout,
  dt1d_i_t *nout)
{
#define EMIT(D) if (n == maxout) break; else out[n++] = (D)
  const unsigned char *in =
    (const unsigned char *)vin;
  unsigned char *out = 
    (unsigned char *)vout;
  unsigned char v;
  dt1d_i_t i = 0;
  dt1d_i_t n = 0;

  D0;

  maxout -= 2;							/* safety margin 					*/
  do {
	unsigned int l = 1;
	unsigned int m = in[i] != ZERO? 0x100: 0x2000;
    while (i<nin)
	{
	  i++;
	  if (i<nin && in[i] == in[i-1] && l<m)
        l++;
	  else
	    break;
	}

	v = in[i-1] & ~CBITS;				/* clear bottom three bits		    */

    DP(printf("dt1d_rle_encode:"
      " v=0x%02x, l=%4d, i=%6d\n",
      v, l, i));

    if (v == ZERO)						/* encoding a zero? 				*/
	{
	  if (l <= 0x20)					/* short run of zeros? 				*/
	  {
	    EMIT(CBITS_ZSMALL | ((l-1) << 3));
	  }
	  else								/* long run of zeros? 				*/
	  {
	    EMIT(CBITS_ZLARGE | (((l-1) >> 5) & ~CBITS));
	    EMIT((l-1) & 0xff);
	  }
	}
	else								/* encoding non-zero data? 			*/
	{			
	  if (l == 1)
	    EMIT(CBITS_D1 | v);
	  else
	  if (l == 2)
	    EMIT(CBITS_D2 | v);
	  else
	  if (l == 3)
	    EMIT(CBITS_D3 | v);
	  else
	  if (l == 4)
	    EMIT(CBITS_D4 | v);
	  else
	  if (l == 5)
	    EMIT(CBITS_D5 | v);
	  else
	  {
	    EMIT(CBITS_DN | v);
	    EMIT(l - 1);					/* output length 					*/
	  }
	}
  } while (i<nin);

  if (nout != NULL)
    *nout = n;

# undef EMIT

  return DT1D_RC_GOOD;		
}

/*..........................................................................
 *..........................................................................*/

dt1d_rc_e
DT1D_API
dt1d_rlerb_decode(
  const void *vin,
  dt1d_i_t nin,
  void *vout,
  dt1d_i_t maxout,
  dt1d_i_t *nout)
{
  const unsigned char *p =
    (const unsigned char *)vin;
  unsigned char *out =
    (unsigned char *)vout;
  unsigned char v;
  dt1d_i_t i;
  dt1d_i_t inc;
  dt1d_i_t n = 0;
  dt1d_rc_e rc = DT1D_RC_GOOD;

  D0;

  for (i=0; i<nin; i+=inc)
  {
	int f = p[i] & CBITS;				/* get the control bits 			*/
	int l;
	inc = 1;
	if (f == CBITS_ZSMALL)
	{
	  v = ZERO;
	  l = (p[i] & ~CBITS) >> 3;
	}
	else
	if (f == CBITS_ZLARGE)
	{
	  v = ZERO;
	  l = ((p[i] & ~CBITS) << 5) | p[i+1];
      inc = 2;
	}
	else								/* run of data? 					*/
	{
	  v = p[i] & ~CBITS;
	  if (f == CBITS_D1)
	    l = 0;
	  else
	  if (f == CBITS_D2)
	    l = 1;
	  else
	  if (f == CBITS_D3)
	    l = 2;
	  else
	  if (f == CBITS_D4)
	    l = 3;
	  else
	  if (f == CBITS_D5)
	    l = 4;
	  else
	  {
		l = p[i+1];
        inc = 2;
	  }
	}

    DP(printf("dt1d_rle_decode:"
      " v=0x%02x, l=%4d, n=%6d\n", 
      v, l, n));

	if (n+l+1 > maxout)					/* overrunning the output buffer? 	*/
	{
	  rc = DT1D_RC_BAD;
	  break;
	}
	else								/* still room in the output buffer? */
	{
	  int j;
	  for (j=0; j<=l; j++)
	    out[j] = v;
	}

	n += l + 1;
	out += l + 1;
  }
  
  if (nout != NULL)
    *nout = n;
  
  return rc;
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
