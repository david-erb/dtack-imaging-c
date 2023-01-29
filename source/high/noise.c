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
  CHANGES
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/



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

DT_RCSID("source/high $RCSfile: noise.c,v $ $Revision: 1.1 $");

#define FUNC(NAME, PIXEL_TYPE, CLIP) \
static void NAME( \
  dt_ctl_t *ctl, \
  PIXEL_TYPE *p, \
  dtxy_t xmax, \
  double vmin, \
  double vmax, \
  double *random) \
{ \
  dtxy_t x; \
  for (x=0; x<xmax; x++) \
    random[x] += (double)p[x]; \
  if (vmin != vmax) \
    CLIP(random, xmax, p, xmax, vmin, vmax); \
}

FUNC(noise_gauss_add_b08, dt_ntype_b08_t, dt1d_clip_double_uchar)
FUNC(noise_gauss_add_b16, dt_ntype_b16_t, dt1d_clip_double_ushort)
FUNC(noise_gauss_add_b32, dt_ntype_b32_t, dt1d_clip_double_uint32)
FUNC(noise_gauss_add_s1,  dt_ntype_s1_t,  dt1d_clip_double_char)
FUNC(noise_gauss_add_s2,  dt_ntype_s2_t,  dt1d_clip_double_short)
FUNC(noise_gauss_add_s4,  dt_ntype_s4_t,  dt1d_clip_double_sint32)
FUNC(noise_gauss_add_f1,  dt_ntype_f1_t,  dt1d_clip_double_float)
FUNC(noise_gauss_add_f2,  dt_ntype_f2_t,  dt1d_clip_double_double)

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtimage_noise_gauss_add(				/* add gaussian random values		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dt1d_ran_t *ran,
  double mean,
  double sigma)							/* stddev i.e. sqrt(variance)		*/
{
  DT_F("dtimage_noise_gauss_add");
  double *random = NULL;
  dtxy_t y;
  const dt_ntype_e *ntypes =			/* should handle any datatype	    */
    dt_ntype_list;
  double vmin, vmax;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    image, "image",
    ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_C(dtos_malloc2,(ctl,				/* space for random row vector	    */
    (void **)&random, 
    image->xe * sizeof(*random),
    F, "random buffer"));

  vmin = dt_ntype_minval[image->ntype];
  vmax = dt_ntype_maxval[image->ntype];

  for (y=0; y<image->ye; y++)			/* for each row					    */
  {
    DT_C_1D(dt1d_random_gauss_double,(	/* compute new row vector		    */
      ran, random, image->xe));

    if (sigma != 1.0 ||
		mean != 0.0)
    DT_C_1D(dt1d_add,(
      DTNTYPE_F2,
      random, image->xe, sigma,
      random, image->xe, 0.0,
      mean,
      random, image->xe));

    if (rc == DT_RC_GOOD)
    switch(image->ntype)
    {
      case DT_NTYPE_B08:
        noise_gauss_add_b08(ctl,
          image->row[y].b08, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_B16:
        noise_gauss_add_b16(ctl,
          image->row[y].b16, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_B32:
        noise_gauss_add_b32(ctl,
          image->row[y].b32, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_S1:
        noise_gauss_add_s1(ctl,
          image->row[y].s1, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_S2:
        noise_gauss_add_s2(ctl,
          image->row[y].s2, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_S4:
        noise_gauss_add_s4(ctl,
          image->row[y].s4, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_F1:
        noise_gauss_add_f1(ctl,
          image->row[y].f1, image->xe,
          vmin, vmax, random);
      break;
      case DT_NTYPE_F2:
        noise_gauss_add_f2(ctl,
          image->row[y].f2, image->xe,
          vmin, vmax, random);
      break;
	  default:
        rc = dtimage_err_ntype(ctl,		/* can't do op for ntype			*/
          image, "image",
          ntypes, F);
    }
    if (rc != DT_RC_GOOD)
      break;
  }

cleanup:
  if (random != NULL)
  dtos_free2(ctl, random,
    F, "random buffr");
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
