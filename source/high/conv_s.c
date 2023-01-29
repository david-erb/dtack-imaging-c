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
DT_RCSID("source/high $RCSfile: conv_s.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>

#define YIELD(Y, Y0, YE) DT_YIELD_ROW(DT_Q, "convolving", (Y)-(Y0), (YE)-(Y0))
#define GETIN(Y, P) DTIMAGE_GETROW(ctl, input, Y, (void **)&(P))
#define GETOUT(Y, P) DTIMAGE_GETROW(ctl, output, Y, (void **)&(P))
#define UNMAPIN(Y) DTIMAGE_UNMAPROW(ctl, input, Y);
#define PUTOUT(Y) DTIMAGE_PUTROW(ctl, output, Y);

static dtntype_s4_t kernel_3x3_ave[] = {1,1,1, 1,1,1, 1,1,1};
static dtntype_s4_t kernel_3x3_laplace[] = {0,-1,0, -1,4,-1, 0,-1,0};

/*..........................................................................
 * direct method, actually works, but slower than sdmk even for 3x3
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_convolve_bbn(					/* b08 b08 any long kernel 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_bbn");
  dtxy_t x, y;
  int kx, ky;
  long a;
# define KMAX (33)
  dt_ntype_b08_t *kq[KMAX];				/* enough for most kernels		    */
  dt_ntype_b08_t **q;
  const dtntype_s4_t *k;

  if (ky2-ky1 > KMAX)
    return dt_err(ctl, F,
      "kernel height %d too big",
      ky2-ky1);

  q = kq + KMAX / 2;
  for (ky=ky1; ky<ky2-1; ky++)			/* map first rows into memory	    */
    GETIN(y1+ky, q[ky]);

  for (y=y1; y<y2; y++)					/* for all input rows			    */
  {
    dt_ntype_b08_t *p;
    YIELD(y, y1, y2);
    GETIN(y+ky2-1, q[ky2-1]);
    GETOUT(y, p);
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      a = 0;
      k = kernel;
      for (ky=ky1; ky<ky2; ky++)
	  {
        const dt_ntype_b08_t *q2 = q[ky];
        for (kx=kx1; kx<kx2; kx++)
          a += (*k++ * q2[x+kx]);
	  }
      p[x] = (dt_ntype_b08_t)a;
	}
    PUTOUT(y);
    UNMAPIN(y+ky1);						/* release topmost row			    */
    for (ky=ky1+1; ky<ky2; ky++)		/* bubble row pointers up		    */
      q[ky-1] = q[ky];
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_convolve_bb3(					/* b08 b08 any 3x3 long kernel		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  long * const mul,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_bb3");
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
    YIELD(y, y1, y2);
    GETIN(y-1, p1);
    GETIN(y,   p2);
    GETIN(y+1, p3);
    GETOUT(y, q);
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      q[x] = (dt_ntype_b08_t)(
        mul[R(0)+p1[0]] + mul[R(1)+p1[1]] + mul[R(2)+p1[2]] + 
        mul[R(3)+p2[0]] + mul[R(4)+p2[1]] + mul[R(5)+p2[2]] + 
        mul[R(6)+p3[0]] + mul[R(7)+p3[1]] + mul[R(8)+p3[2]]);
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
dtimage_convolve_bb3_laplace(			/* b08 b08 laplace kernel 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  long * const mul4,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_bb3_laplace");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2, *p3, *q;
  int i;
  dt_dbg(ctl, F, DT_DBG_MASK_IMG, "");

  for (i=0; i<256; i++)
    mul4[i] = 4 * i;

  for (y=y1; y<y2; y++)					/* for all input rows			    */
  {
    YIELD(y, y1, y2);
    GETIN(y-1, p1);
    GETIN(y,   p2);
    GETIN(y+1, p3);
    GETOUT(y, q);
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      q[x] = (dt_ntype_b08_t)
             (      -p1[x] -
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
dtimage_convolve_bb3_ave(				/* b08 b08 average kernel 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_bb3_ave");
  dtxy_t x, y;
  dt_ntype_b08_t *p1, *p2, *p3, *q;
  dtxy_t s, s1, s2, s3;

  dt_dbg(ctl, F, DT_DBG_MASK_IMG, "");

  for (y=y1; y<y2; y++)					/* for all input rows			    */
  {
    YIELD(y, y1, y2);
    GETIN(y-1, p1);
    GETIN(y,   p2);
    GETIN(y+1, p3);
    GETOUT(y, q);
    p1++;
    p2++;
    p3++;
    s1 = 0;
    s2 = p1[-1] + p2[-1] + p3[-1];
    s3 = p1[ 0] + p2[ 0] + p3[ 0];
    s = s2 + s3;
    for (x=x1; x<x2; x++)				/* for all input pixels			    */
	{
      s -= s1;
      s1 = s2;
      s2 = s3;
      q[x] = (dt_ntype_b08_t)(s += s3 = p1[x] + p2[x] + p3[x]);
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
dtimage_convolve_bb(					/* special b08 b08 cases 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_bb");
  static dt_ntype_e input_ntypes[] = {
    DT_NTYPE_B08, DT_NTYPE_NULL};
  static dt_ntype_e output_ntypes[] = {
    DT_NTYPE_B08, DT_NTYPE_NULL};
  dtxy_t xmax, ymax;
  dtxy_t x1, y1;
  dtxy_t x2, y2;
  int kx1, ky1;
  int kx2, ky2;
  long cmp;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check input data type		    */
    input, "input",
    input_ntypes,
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check output data type		    */
    output, "output",
    output_ntypes, 
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  xmax = DT_MIN(input->xe, output->xe);
  ymax = DT_MIN(input->ye, output->ye);

  kx1 = -kxsize / 2;
  ky1 = -kysize / 2 ;
  kx2 = kxsize + kx1;
  ky2 = kysize + ky1;
  x1 = -kx1;
  y1 = -ky1;
  x2 = xmax - kx2 + 1;
  y2 = ymax - ky2 + 1;

  switch(DT_NTYPE_PAIR(input->ntype,
           output->ntype))
  {
    case DT_NTYPE_PAIR(DT_NTYPE_B08, DT_NTYPE_B08):
    {
      if (kxsize == 3 && kysize == 3)
	  {
        DT_Q_1D(dt1d_cmp,(				/* b08,b08 and 3x3 average kernel   */
          DTNTYPE_S4,
          kernel, 9,
          kernel_3x3_ave, 9, 0, &cmp));
        if (cmp == 0)
		{
          DT_Q(dtimage_convolve_bb3_ave,
            (ctl,
             input, x1, y1, x2, y2,
             kernel, kx1, ky1, kx2, ky2,
             output));
		  break;
		}

        DT_Q_1D(dt1d_cmp,(				/* b08,b08 and 3x3 laplace			*/
          DTNTYPE_S4,
          kernel, 9,
          kernel_3x3_laplace, 9, 0, &cmp));
        if (cmp == 0)
		{
          long *mul;
          dt_rc_e rc;
          DT_Q(dtos_malloc2,(ctl,
            (void **)&mul, 256*sizeof(long),
            F, "mul buffer"));
          DT_G(dtimage_convolve_bb3_laplace,
            (ctl,
             input, x1, y1, x2, y2,
             kernel, kx1, ky1, kx2, ky2,
             mul, output));
          DT_Q(dtos_free2,(ctl, mul,
            F, "mul buffer"));
          if (rc != DT_RC_GOOD)
            return rc;
		  break;
		}
										/* b08,b08 and 3x3 unknown kernel	*/
        {
          long *mul;
          dt_rc_e rc;
          DT_Q(dtos_malloc2,(ctl,
            (void **)&mul, 256*9*sizeof(long),
            F, "mul buffer"));
          DT_G(dtimage_convolve_bb3,
            (ctl,
             input, x1, y1, x2, y2,
             kernel, kx1, ky1, kx2, ky2,
             mul, output));
          DT_Q(dtos_free2,(ctl, mul,
            F, "mul buffer"));
          if (rc != DT_RC_GOOD)
            return rc;
  		  break;
        }
	  }
      else								/* b08,b08 but not 3x3			    */
	  {
        DT_G(dtimage_convolve_bbn,
          (ctl,
           input, x1, y1, x2, y2,
           kernel, kx1, ky1, kx2, ky2,
           output));
		break;
	  }
    }
    default:
      return dt_err(ctl, F, "shouldn't happen");
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
