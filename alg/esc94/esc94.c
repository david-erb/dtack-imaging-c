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
| TITLE
| Algorithm: dt_esc94
| 
|!Abstract
| This is an implementation of an error diffusion algorithm.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("alg/esc94 $RCSfile: esc94.c,v $ $Revision: 1.3 $");
#include <dtack/os.h>
#include <dt_esc94.h>

#include <dtack/str.h>
#include <dtack/mem.h>


static const char *variants[] = {
# define VARIANT_EASY 0
  "easy",
# define VARIANT_FAST 1
  "fast",
  NULL
};

#define CELLY (6)
#define CELLX (6)

static const int cell_next[6] = {1, 2, 3, 4, 5, 0};

static double cell[CELLY][CELLX] = {
 { 9,11,10, 8, 6, 7 },
 {12,17,16, 5, 0, 1 },
 {13,14,15, 4, 3, 2 },
 { 8, 6, 7, 9,11,10 },
 { 5, 0, 1,12,17,16 },
 { 4, 3, 2,13,14,15 } 
};

/*..........................................................................
| NAME
| dt_esc94() - error diffusion
| 
| DESCRIPTION
| dt_esc94() generates an error diffusion output image.
|
| This is the main call.  It primarily branches on variant.
|
| END
 *..........................................................................*/

dt_rc_e
dt_esc94(								/* error diffusion					*/
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_esc94";
  dt_ctl_t *ctl = &param->ctl;
  int variant;

  DT_Q(dtstr_choice,(ctl,				/* choose variant				    */
    variants, param->variant, 
    &variant));

  switch(variant)
  {
	case VARIANT_EASY: 
      DT_Q(dt_esc94_easy,(param,		/* do it the easy way				*/
        srcimg, dstimg));
    break;
	case VARIANT_FAST: 
      DT_Q(dt_esc94_fast,(param,		/* do it the fast way				*/
        srcimg, dstimg));
    break;
	case -1:
	  return dt_err(ctl, F,
        "unknown variant \"%s\"",
        param->variant);
	default:
	  return dt_err(ctl, F,
        "ambiguous variant \"%s\"",
        param->variant);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_esc94() - error diffusion
| 
| DESCRIPTION
| dt_esc94() produces an two-valued output image by error diffusion.
|
| END
 *..........................................................................*/

dt_rc_e
dt_esc94_easy(				
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_esc94_easy";
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dt_esc94_easy_any,(
    param, srcimg, dstimg));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_esc94() - error diffusion
| 
| DESCRIPTION
| dt_esc94() produces an two-valued output image by error diffusion.
|
| END
 *..........................................................................*/

dt_rc_e
dt_esc94_fast(				
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_esc94_fast";
  dt_ctl_t *ctl = &param->ctl;

  switch(DT_NTYPE_PAIR(
           srcimg->ntype,
           dstimg->ntype))
  {
    case DT_NTYPE_PAIR(					/* input b08 and output b08?	    */
           DT_NTYPE_B08,
           DT_NTYPE_B08):
      DT_Q(dt_esc94_fast_b08_b08,(
        param, srcimg, dstimg));
    break;
    default:							/* any other combination of types?  */
      DT_Q(dt_esc94_easy_any,(
        param, srcimg, dstimg));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_esc94() - error diffusion
| 
| DESCRIPTION
| dt_esc94() produces an two-valued output image by error diffusion.
|
| END
 *..........................................................................*/

dt_rc_e
dt_esc94_fast_b08_b08(				
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_esc94_b08_b08";
  dt_ctl_t *ctl = &param->ctl;
  dtxy_t xe, ye;
  typedef long k_t;
  k_t *kp0, *kp1, *ktmp;
  dtxy_t x, y;
  int i;
  dt_rc_e rc = DT_RC_GOOD;
  unsigned int n;
  unsigned int datasize;
  dt_ntype_b08_t *srcdata = NULL;
  dtxy_t srcbeg = 0;
  dtxy_t srcend = 0;
  dt_ntype_b08_t *dstdata = NULL;
  dtxy_t dstbeg = 0;
  dtxy_t dstend = 0;
# define KSHIFT (8)
# define KMULT (256)
  const long param_max = (long)(param->max * KMULT + 0.5);
  const long param_min = (long)(param->min * KMULT + 0.5);
  const dt_ntype_b08_t foreground = (dt_ntype_b08_t)param->foreground;
  const dt_ntype_b08_t background = (dt_ntype_b08_t)param->background;
  long param_k_imn[256];
  long lambda_cell[CELLY][CELLX];
  static dt_ntype_e ntypes[] = {
    DT_NTYPE_B08, DT_NTYPE_NULL};

  for (i=0; i<256; i++)					/* precompute k * imn table	   		*/
    param_k_imn[i] = (long)(
      param->k * i * KMULT + 0.5);

  for (y=0; y<CELLY; y++)				/* precompute cell weights		  	*/
    for (x=0; x<CELLX; x++)
      lambda_cell[y][x] = (long)(
        param->lambda * cell[y][x] *
		KMULT + 0.5);

  rc = dtimage_check_2f(ctl,			/* check data types				    */
    srcimg, "srcimg",
    dstimg, "dstimg",
    ntypes, DTIMAGE_FLAG_PAGED, F,
    &xe, &ye);
  if (rc != DT_RC_GOOD)
    return rc;

  n = (unsigned int)
    (DTCONFIG_MALLOC_MAX /				/* max error objects we could get	*/
    (long)sizeof(*kp0));					
  if (xe+2 > n)
    return dt_err(ctl, F,
      "image too wide (max %u)", n);

  n = sizeof(*kp0) * (xe + 2);			/* space we need for error objects	*/

  DT_Q(dtos_malloc2,(ctl,				/* two one-line diffusion buffers	*/
    (void **)&kp0, n,
    F, "diffusion buffer"));
  DT_Q(dtos_malloc2,(ctl, 
    (void **)&kp1, n,
    F, "diffusion buffer"));

  kp0 += 1;                             /* account for left edge            */
  kp1 += 1;
  memset(kp0-1, 0, n);
  for (y=0; y<ye; y++)					/* loop over rows of image		    */
  {
    const long *lambda_cell_row = 
      &lambda_cell[y%CELLY][0];
    int cellx = 0;
    if (y >= srcend)					/* exhausted input block?		    */
	{
      if (srcdata)						/* we have a previous block?	    */
        DTIMAGE_UNMAPROW(ctl, srcimg,	/* release it					    */
          srcbeg);
      DTIMAGE_GETROWS(ctl, srcimg, y,	/* map the next block               */
        (void **)&srcdata, &datasize);
      srcbeg = y;						/* where this block starts		    */
      srcend += datasize /				/* last y in this block				*/
        srcimg->rowsize;
	}

    if (y >= dstend)					/* exhausted output block?		    */
	{
      if (dstdata)						/* we have a previous block?	    */
        DTIMAGE_PUTROW(ctl, dstimg,		/* release it					    */
          dstbeg);
      DTIMAGE_MAPROWS(ctl, dstimg, y,	/* map the next block               */
        (void **)&dstdata, &datasize);
      dstbeg = y;						/* where this block starts		    */
      dstend += datasize /				/* last y in this block				*/
        dstimg->rowsize;
	}

    memset(kp1-1, 0, n);				/* clear bottom-most error buffer	*/

    for (x=0; x<xe; x++)				/* for all columns				    */
	{
      const int raw = srcdata[x];		/* get desired brightness		    */
      const k_t imn = raw << KSHIFT;	/* scale to fixed point				*/
      const k_t sigma = (kp0[x] >> 4) +	/* error contribution to this pixel	*/
        ((kp0[x] & 0x8) == 0x8);
      int step;
	  k_t e;

      step = -sigma >=					/* equation 3						*/
        lambda_cell_row[cellx] -		/* periodic threshold modulation    */
        param_k_imn[raw];				/* input-dependent modulation	    */ 
      cellx = cell_next[cellx];

      if (step)							/* need to light the pixel?		    */
	  {
        e = param_max - imn + sigma;	/* equation 2						*/
        dstdata[x] = foreground;
	  }
      else								/* need to make the pixel dark?	    */
	  {
        e = param_min - imn + sigma;	/* equation 2						*/
        dstdata[x] = background;
	  }


										/* diffuse the new error		    */
      {
        k_t * const kpx = kp1 + x - 1;
        kp0[x+1] += 7 * e;
        kpx[0] += 3 * e;
        kpx[1] += 5 * e;
        kpx[2] +=     e;
	  }
	}

    srcdata += srcimg->xe;				/* point to next row			    */
    dstdata += dstimg->xe;

    ktmp = kp0;							/* swap line buffers			    */
    kp0 = kp1;
    kp1 = ktmp;
  }

  if (srcdata)							/* we have an input block?	    	*/
    DTIMAGE_PUTROW(ctl, srcimg,			/* release it					    */
      srcbeg);

  if (dstdata)							/* we have an output block?	    	*/
    DTIMAGE_PUTROW(ctl, dstimg,			/* release it					    */
      dstbeg);

  DT_Q(dtos_free2,(ctl,
    kp1-1, F, "diffusion buffer"));
  DT_Q(dtos_free2,(ctl,
    kp0-1, F, "diffusion buffer"));

  return rc;
}

/*..........................................................................
| NAME
| dt_esc94() - error diffusion
| 
| DESCRIPTION
| dt_esc94() produces an two-valued output image by error diffusion.
|
| END
 *..........................................................................*/

dt_rc_e
dt_esc94_easy_any(				
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_esc94_easy_any";
  dt_ctl_t *ctl = &param->ctl;
  dtxy_t xe, ye;
  double *kp0, *kp1, *ktmp;
  dtxy_t x, y;
  dt_rc_e rc = DT_RC_GOOD;
  unsigned int n;

  rc = dtimage_check_2f(ctl,
    srcimg, "srcimg",
    dstimg, "dstimg",
    NULL, DTIMAGE_FLAG_PAGED, F,
    &xe, &ye);
  if (rc != DT_RC_GOOD)
    return rc;

  n = (unsigned int)
    (DTCONFIG_MALLOC_MAX /				/* max error objects we could get	*/
    (long)sizeof(*kp0));					
  if (xe+2 > n)
    return dt_err(ctl, F,
      "image too wide (max %u)", n);

  n = sizeof(*kp0) * (xe + 2);			/* space we need for error objects	*/

  DT_Q(dtos_malloc2,(ctl,				/* two one-line diffusion buffers	*/
    (void **)&kp0, n,
    F, "diffusion buffer"));
  DT_Q(dtos_malloc2,(ctl, 
    (void **)&kp1, n,
    F, "diffusion buffer"));

  kp0 += 1;                             /* account for left edge            */
  kp1 += 1;
  memset(kp0-1, 0, n);
  for (y=0; y<ye; y++)					/* loop over rows of image		    */
  {
    memset(kp1-1, 0, n);				/* clear bottom-most error buffer	*/

    for (x=0; x<xe; x++)				/* for all columns				    */
	{
      double imn, bmn, tmn;
      const double sigma = kp0[x]/16.0;	/* error contribution to this pixel	*/
      int step;
	  double e;

      DT_Q(dtimage_get_pixel,(ctl,		/* get desired brightness 			*/
        srcimg, x, y, &imn));

      tmn =								/* part of equation 5			    */
        param->lambda * 
        cell[y%CELLY][x%CELLX] -		/* periodic threshold modulation    */
        (param->k-1) * imn;				/* input-dependent modulation	    */ 

      step = imn - sigma >= tmn;		/* equation 3						*/

      if (step)							/* need to light the pixel?		    */
        bmn = param->max;
      else								/* need to make the pixel dark?	    */
        bmn = param->min;

      e = bmn - imn + sigma;			/* equation 2						*/

      kp0[x+1] += 7 * e;				/* diffuse the new error		    */
      kp1[x-1] += 3 * e;
      kp1[x  ] += 5 * e;
      kp1[x+1] +=     e;

      DT_Q(dtimage_set_pixel,(ctl,		/* put on/off pixel in output	   	*/
        dstimg, x, y,
        step?
          param->foreground:
          param->background));
	}

    ktmp = kp0;							/* swap line buffers			    */
    kp0 = kp1;
    kp1 = ktmp;
  }

  DT_Q(dtos_free2,(ctl,
    kp1-1, F, "diffusion buffer"));
  DT_Q(dtos_free2,(ctl,
    kp0-1, F, "diffusion buffer"));

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
