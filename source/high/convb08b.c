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
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/yield.h>
#include <dtack/mem.h>

DT_RCSID("base $RCSfile: convb08b.c,v $ $Revision: 1.1 $");

#define YIELD(Y) 
#define GETIN(Y, P) DTIMAGE_GETROW(ctl, input, Y, (void **)&(P))
#define MAPOUT(Y, P) DTIMAGE_MAPROW(ctl, output, Y, (void **)&(P))
#define UNMAPIN(Y) DTIMAGE_UNMAPROW(ctl, input, Y);
#define PUTOUT(Y) DTIMAGE_PUTROW(ctl, output, Y);

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_convolve_long_b08_b08_3x3_any(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const long *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  const long gain,
  long * const mul,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_long_b08_b08_3x3_any");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2, *p3, *q;
#define R(K) (K<<8)						/* 256 entries in each of 9 tables	*/
  int k, i;

  dt_dbg(ctl, F, DT_DBG_MASK_IMG, "");

  for (k=0; k<9; k++)
    for (i=0; i<256; i++)
      mul[R(k)+i] = kernel[k] * i;

  for (y=y1; y<y2; y++)					/* for all input rows			    */
  {
    YIELD(y)
    GETIN(y-1, p1);
    GETIN(y,   p2);
    GETIN(y+1, p3);
    MAPOUT(y, q);
    if (gain != 1)
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      q[x] = (
        mul[R(0)+p1[0]] + mul[R(1)+p1[1]] + mul[R(2)+p1[2]] + 
        mul[R(3)+p2[0]] + mul[R(4)+p2[1]] + mul[R(5)+p2[2]] + 
        mul[R(6)+p3[0]] + mul[R(7)+p3[1]] + mul[R(8)+p3[2]]) / gain;
      p1++; p2++; p3++;
	}
    else
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      q[x] =
        mul[R(0)+p1[0]] + mul[R(1)+p1[1]] + mul[R(2)+p1[2]] + 
        mul[R(3)+p2[0]] + mul[R(4)+p2[1]] + mul[R(5)+p2[2]] + 
        mul[R(6)+p3[0]] + mul[R(7)+p3[1]] + mul[R(8)+p3[2]];
      p1++; p2++; p3++;
	}
    PUTOUT(y);
    UNMAPIN(y+1);
    UNMAPIN(y);
    UNMAPIN(y-1);
  }
  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_convolve_long_b08_b08_3x3_laplace(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const long *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  const long gain,
  long * const mul4,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_long_b08_b08_3x3_laplace");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2, *p3, *q;
  int i;
  dt_dbg(ctl, F, DT_DBG_MASK_IMG, "");

  for (i=0; i<256; i++)
    mul4[i] = 4 * i;

  for (y=y1; y<y2; y++)					/* for all input rows			    */
  {
    YIELD(y)
    GETIN(y-1, p1);
    GETIN(y,   p2);
    GETIN(y+1, p3);
    MAPOUT(y, q);
    if (gain != 1)
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      q[x] = (      -p1[x] -
        p2[0] + mul4[p2[1]] - p2[2] -
                     p3[x]) / gain;
      p2++;
	}
    else
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      q[x] = (      -p1[x] -
        p2[0] + mul4[p2[1]] - p2[2] -
                     p3[x]);
      p2++;
	}
    PUTOUT(y);
    UNMAPIN(y+1);
    UNMAPIN(y);
    UNMAPIN(y-1);
  }
  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_convolve_long_b08_b08_3x3_ave(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const long *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  const long gain,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_long_b08_b08_3x3_ave");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2, *p3, *q;
  dtxy_t s, s1, s2, s3;

  dt_dbg(ctl, F, DT_DBG_MASK_IMG, "");

  for (y=y1; y<y2; y++)					/* for all input rows			    */
  {
    YIELD(y)
    GETIN(y-1, p1);
    GETIN(y,   p2);
    GETIN(y+1, p3);
    MAPOUT(y, q);
    p1++;
    p2++;
    p3++;
    s1 = 0;
    s2 = p1[-1] + p2[-1] + p3[-1];
    s3 = p1[ 0] + p2[ 0] + p3[ 0];
    s = s2 + s3;
    if (gain != 1)
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      s -= s1;
      s1 = s2;
      s2 = s3;
      q[x] = (s += s3 = p1[x] + p2[x] + p3[x]) / gain;
	}
    else
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      s -= s1;
      s1 = s2;
      s2 = s3;
      q[x] = (s += s3 = p1[x] + p2[x] + p3[x]);
	}
    PUTOUT(y);
    UNMAPIN(y+1);
    UNMAPIN(y);
    UNMAPIN(y-1);
  }
  return DT_RC_GOOD;
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
