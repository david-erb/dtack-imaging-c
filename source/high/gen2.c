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
#include <dtack/lut8.h>

DT_RCSID("source/high $RCSfile: gen2.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_replicate(
  dt_ctl_t *ctl,
  dtimage_t *image,
  const dt_ntype_f2_t * const kernel,
  const dtxy_t kx,
  const dtxy_t ky)
{
  DT_F("dtimage_gen_replicate");
  dt_ntype_f2_t *work = NULL;
  const dtxy_t xe = image->xe;
  dtxy_t y;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for the promoted row 		*/
    (void **)&work,
    xe * sizeof(DT_NTYPE_F2),
    F, "work row"));

  for (y=0; y<image->ye; y++)
  {
    const dt_ntype_f2_t * const k =		/* point into kernel buffer 		*/
      kernel + (y%ky) * kx;
	dtxy_t x;
	void *p;
    for (x=0; x<xe; x++)				/* replicate the pattern 			*/
	  work[x] = k[x%kx];
	DTIMAGE_MAPROW2(DT_C, ctl,			/* access the row 					*/
      image, y, &p);
	dt1d_cast(work, xe, DT_NTYPE_F2,	/* convert the row 					*/
      p, xe, image->ntype);
	DTIMAGE_PUTROW2(DT_C, ctl,			/* release and write a row 			*/
      image, y);
  }

cleanup:
  if (work != NULL)
    DT_I(dtos_free2,(ctl, work,
      F, "work row"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_interlock16(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground)
{
  DT_F("dtimage_gen_interlock16");
  dt_ntype_f2_t *kernel = NULL;
#define KX 16
#define KY 16
  char pattern[KX*KY] = {
    1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
    1,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,
    1,0,2,0,1,1,1,1,1,1,1,1,1,0,2,0,
    1,0,2,0,1,0,0,0,0,0,0,0,1,0,2,0,
    1,0,2,0,1,0,2,2,2,2,2,0,1,0,2,0,
    1,0,2,0,1,0,2,0,0,0,2,0,1,0,2,0,
    1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,
    1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,
    1,0,2,0,1,0,2,0,1,0,2,0,1,0,2,0,
    1,0,2,0,1,0,0,0,1,0,2,0,1,0,2,0,
    1,0,2,0,1,1,1,1,1,0,2,0,1,0,2,0,
    1,0,2,0,0,0,0,0,0,0,2,0,1,0,2,0,
    1,0,2,2,2,2,2,2,2,2,2,0,1,0,2,0,
    1,0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,0,2,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  const dtxy_t n = KX * KY;
  dtxy_t i;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for the promoted kernel	*/
    (void **)&kernel,
    n * sizeof(DT_NTYPE_F2),
    F, "kernel"));

  for (i=0; i<n; i++)					/* promote the kernel 				*/
    kernel[i] = pattern[i]?
  	  foreground: background;

  DT_C(dtimage_gen_replicate,(ctl,		/* replicate the kernel 			*/
    image, kernel, KX, KY));

cleanup:
  if (kernel != NULL)
    DT_I(dtos_free2,(ctl, kernel,
      F, "kernel"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

#define F "dtimage_gen_rect_tree"

static
dt_rc_e
gen_rect_tree(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double color,
  int depth,
  int maxdepth)
{
  dtxy_t xhalf;
  dtxy_t yhalf;
  dtxy_t xmid;
  dtxy_t ymid;
  dt_rc_e rc;

  if (depth > maxdepth) return DT_RC_GOOD;

  xhalf = (xmax - xmin) / 2;
  yhalf = (ymax - ymin) / 2;
  xmid = xmin + xhalf;
  ymid = ymin + yhalf;

  DT_Q(dtimage_plot_line,(ctl, image, xmid, ymin, xmid, ymax, color));
  DT_Q(dtimage_plot_line,(ctl, image, xmin, ymid, xmax, ymid, color));

  if (xhalf < 5 || yhalf < 5) return DT_RC_GOOD;
  rc = gen_rect_tree(ctl, image,
     xmid, ymin+2, xmax-2, ymid-2, color, depth+1, maxdepth);
  if (rc != DT_RC_GOOD) return rc;
  rc = gen_rect_tree(ctl, image,
     xmid, ymid+2, xmax-2, ymax-2, color, depth+1, maxdepth);
  if (rc != DT_RC_GOOD) return rc;
  rc = gen_rect_tree(ctl, image,
     xmin+2, ymid+2, xmid, ymax-2, color, depth+1, maxdepth);
  if (rc != DT_RC_GOOD) return rc;
  rc = gen_rect_tree(ctl, image,
     xmin+2, ymin+2, xmid, ymid-2, color, depth+1, maxdepth);
  if (rc != DT_RC_GOOD) return rc;

  return DT_RC_GOOD;
}

dt_rc_e
DTCONFIG_API1
dtimage_gen_rect_tree(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int maxdepth)
{
  dt_rc_e rc;

  DT_Q(dtimage_constant,(ctl, image, background));

  rc = gen_rect_tree(ctl, image,
    2, 0, image->xe-3, image->ye-3,
    foreground,
    0, maxdepth);

  return rc;
}
#undef F

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_rect_spiral(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int separation)
{
  DT_F("dtimage_gen_rect_spiral");
  dtxy_t x0, y0;
  dtxy_t x1, y1;
  dtxy_t w, h;
  int sign;

  DT_Q(dtimage_constant,(ctl, image, background));

  x1 = separation;
  y1 = separation;
  sign = 1;
  w = image->xe - 2 * separation;
  h = image->ye - 2 * separation;
  do {
    x0 = x1;
    y0 = y1;
    x1 += sign * w;
    w -= separation;
    DT_Q(dtimage_plot_line,(ctl, image, x0, y0, x1, y1, foreground));

    x0 = x1;
    y0 = y1;
    y1 += sign * h;
    h -= separation;
    DT_Q(dtimage_plot_line,(ctl, image, x0, y0, x1, y1, foreground));

    sign = -sign;
  } while (w > 0 && h > 0);

  return DT_RC_GOOD;
}


/*..........................................................................*/
static
void
wave(
  double a,
  double w,
  double p,
  double *y,
  int n)
{
  int i;
  double inc = 2.0 * DT_PI / (double)n;
  for (i=0; i<n; i++)
  {
    y[i] = a * sin(w * (double)i * inc + p) + a;
  }
}

/*..........................................................................
 * sine waves horizontal
 * param->max amplitude in image units
 * param->omega frequency in number of wavelengths per image
 * param->phi phase shift
 * param->foreground bar color
 * param->background background color
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_xsine(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dtimage_gen_xsine");
  double *a = NULL;
  dt_rc_e rc;
  dtxy_t param_max = DT_CAST(dtxy_t,
    DTPARAM_GET_VALUE(max,
      image->xe * 0.8));
  double param_omega =
    DTPARAM_GET_VALUE(omega, DT_PI*2.0);
  double param_phi =
    DTPARAM_GET_VALUE(phi, 0.0);

  DT_Q(dtos_malloc2,(ctl,
    (void **)&a, 
    image->ye * sizeof(*a),
    F, "array"));
										/* ................................ */
  wave(1.0, param_omega, param_phi,
    a, image->ye);

  DT_C(dtimage_constant,(ctl,			/* clear output image 				*/
    image, param->background));
 
  DT_C(dtimage_plot_ybars_double,(		/* make bar plot of histogram		*/
    ctl, image, a, image->ye,
    (image->ye-param_max)/2,
    param_max, param->foreground));

cleanup:
  DT_Q(dtos_free2,(ctl, a,
    F, "array"));
  return rc;
}

/*..........................................................................
 * sine waves
 * param->max amplitude in image units
 * param->omega frequency in radians
 * param->phi phase shift
 * param->foreground bar color
 * param->background background color
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_ysine(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dtimage_gen_ysine");
  double *a = NULL;
  int base;
  dtimage_t overlay;
  dt_rc_e rc;
  dtxy_t param_max = DT_CAST(dtxy_t,
    DTPARAM_GET_VALUE(max,
      (double)image->xe * 0.8));
  double param_omega =
    DTPARAM_GET_VALUE(omega, DT_PI*4.0);
  double param_phi =
    DTPARAM_GET_VALUE(phi, 0.0);

  overlay.xe = 0;

  DT_Q(dtos_malloc2,(ctl,
    (void **)&a, 
    image->ye * sizeof(*a),
    F, "array"));
										/* ................................ */
  wave(1.0, param_omega, param_phi,
    a, image->ye);

  DT_C(dtimage_constant,(ctl,			/* clear output image 				*/
    image, param->background));
 
  base = (int)(image->xe-param_max)/2;
  DT_C(dtimage_plot_ybars_double,(		/* make bar plot of histogram		*/
    ctl, image, a, image->ye, base,
    param_max, param->foreground));

  DT_C(dtimage_overlay,(ctl, image,
    &overlay, 0, 0, base, image->ye));

  DT_C(dtimage_constant,(ctl,			/* clear output image 				*/
    &overlay, param->foreground));

cleanup:
  if (overlay.xe)
    DT_I(dtimage_free,(ctl, &overlay));
  if (a != NULL)
    DT_I(dtos_free2,(ctl, a,
      F, "array"));
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
