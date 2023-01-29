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
| Algorithm: dt_flo76
| 
|!Abstract
| This is an implementation of an error diffusion algorithm.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/1d.h>
#include <dt_flo76.h>

#include <dtack/str.h>

DT_RCSID("alg/flo76 $RCSfile: flo76.c,v $ $Revision: 1.1 $");

static const char *variants[] = {
# define VARIANT_EASY 0
  "easy",
# define VARIANT_FAST 1
  "fast",
  NULL
};

/*..........................................................................
| NAME
| dt_flo76() - peak detection
| 
| DESCRIPTION
| dt_flo76() produces an array in which peaks are marked.
|
| END
 *..........................................................................*/

dt_rc_e
dt_flo76(								/* calculate pds and mark peaks		*/
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_flo76";
  dt_ctl_t *ctl = &param->ctl;
  int variant;

  DT_Q(dtstr_choice,(ctl,				/* choose variant				    */
    variants, param->variant, 
    &variant));

  switch(variant)
  {
	case VARIANT_EASY: 
      DT_Q(dt_flo76_easy,(param,
        srcimg, dstimg));
    break;
	case VARIANT_FAST: 
      DT_Q(dt_flo76_fast,(param,
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
| dt_flo76() - peak detection
| 
| DESCRIPTION
| dt_flo76() produces an two-valued output image by error diffusion.
|
| END
 *..........................................................................*/

dt_rc_e
dt_flo76_easy(				
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  static char *F = "dt_flo76_easy";
  dt_ctl_t *ctl = &param->ctl;
  const double half = (param->max+param->min) / 2.0;
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
    (DTCONFIG_MALLOC_MAX /				/* max double objects we could get	*/
    (long)sizeof(double));					
  if (xe+2 > n)
    return dt_err(ctl, F,
      "image too wide (max %u)", n);

  n = sizeof(double) * (xe + 2);		/* space we need for double objects	*/

  DT_Q(dtos_malloc2,(ctl,				/* two one-line diffusion buffers	*/
    (void **)&kp0, n,
    F, "diffusion buffer"));
  DT_Q(dtos_malloc2,(ctl, 
    (void **)&kp1, n,
    F, "diffusion buffer"));


  kp0 += 1;								/* account for left edge			*/
  kp1 += 1;
  memset(kp0-1, 0, n);
  for (y=0; y<ye; y++)					/* loop over rows of image		    */
  {
    memset(kp1-1, 0, n);				/* clear bottom-most error buffer	*/

    for (x=0; x<xe; x++)				/* for all columns				    */
	{
      double desired, modified, actual;
	  double e;
      DT_Q(dtimage_get_pixel,(ctl,		/* get desired brightness 			*/
        srcimg, x, y, &desired));
      modified = desired -				/* modify desired brightness	    */
        kp0[x] / 16.0;	
      if (modified > half)				/* need to light the pixel?		    */
        actual = param->max;
      else								/* need to make the pixel dark?	    */
        actual = param->min;
      e = actual - modified;			/* error introduced				    */

      kp0[x+1] += 7 * e;				/* diffuse the new error		    */
      kp1[x-1] += 3 * e;
      kp1[x  ] += 5 * e;
      kp1[x+1] +=     e;

      DT_Q(dtimage_set_pixel,(ctl,		/* put on/off pixel in output	   	*/
        dstimg, x, y,
        actual == param->max?
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

/*..........................................................................
| NAME
| dt_flo76() - peak detection
|
| DESCRIPTION
| dt_flo76() produces an two-valued output image by error diffusion.
|
| END
 *..........................................................................*/

dt_rc_e
dt_flo76_fast(
  dtparam_t *param,
  dtimage_t *srcimg,
  dtimage_t *dstimg)
{
  return dt_flo76_easy(param, srcimg, dstimg);
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
