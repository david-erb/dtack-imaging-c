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
  19991124 DE allow more data types
 *..........................................................................*/



/*..........................................................................
| NAME
| dttoolgen utility - generate image
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: gen.c,v $ $Revision: 1.13 $");

#define USAGE "[params] source dest"

typedef struct {
  int pattern;
  dtntype_e ntype;
} app_t;

typedef dt_rc_e
gen_f(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param);

#define LIST \
L("1",		  dttool_gen__1)											  \
L("constant", dttool_gen__1)											  \
L("2v",       dttool_gen__2v)			/* vertical split				*/\
L("vsplit",	  dttool_gen__2v)											  \
L("2h",       dttool_gen__2h)			/* horizontal split				*/\
L("hsplit",	  dttool_gen__2h)\
L("3v",       dttool_gen__3v)			/* vertical ramp				*/\
L("vramp",	  dttool_gen__3v)\
L("3h",		  dttool_gen__3h)\
L("hramp",    dttool_gen__3h)\
L("4",		  dttool_gen__4)			/* x + y						*/\
L("x+y",      dttool_gen__4)\
L("5",        dttool_gen__5)			/* x * y						*/\
L("x*y",	  dttool_gen__5)\
L("6",        dttool_gen__6)			/* gaussian lighting			*/\
L("gausslight", dttool_gen__6)\
L("7",		  dttool_gen__7)			/* bit stipple					*/\
L("stipple",  dttool_gen__7)\
L("8",		  dttool_gen__8)			/* uniform random noise			*/\
L("uniform",  dttool_gen__8)\
L("9",		  dttool_gen__9)			/* gaussian random noise		*/\
L("gaussnoise", dttool_gen__9)\
L("10",		  dttool_gen__10)			/* space filling tree			*/\
L("tree",	  dttool_gen__10)\
L("11",		  dttool_gen__11)			/* rectangular spiral			*/\
L("spiral",	  dttool_gen__11)\
L("12",		  dttool_gen__12)			/* regularly spaced islands		*/\
L("islands",  dttool_gen__12)                                             \
L("13",		  dttool_gen__13)			/* checkerboard					*/\
L("checkerboard",dttool_gen__13)                                          \
L("test1",    dttool_gen__test1)		/* grayscale with X on it		*/\
L("test2",	  dttool_gen__test2)		/* grayscale with X on it		*/\
L("test3",	  dttool_gen__test3)		/* grayscale with X on it		*/\
L("color256", dttool_gen__color256)		/* 256 colors in rectangles		*/\
L("grid1",	  dttool_gen__grid1)		/* regular grid					*/\
L("grid2",	  dttool_gen__grid2)		/* arithmetic sequential grid	*/\
L("xsine",	  dttool_gen__xsine)		/* sine wave horizontal			*/\
L("ysine",	  dttool_gen__ysine)		/* sine wave vertical			*/\
L("kernel",	  dttool_gen__kernel)		/* standard convolution kernel	*/\
L("interlock16",dttool_gen__interlock16)


#define L(PATTERN, FUNC) static gen_f FUNC;
LIST
#undef  L

static gen_f *func[41] = {
#define L(PATTERN, FUNC) FUNC,
LIST
#undef  L
NULL};

static const char *patterns[41] = {
#define L(PATTERN, FUNC) PATTERN,
LIST
#undef  L
NULL};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_gen_arg_check(					/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_gen_arg_check");
  app_t *a = (app_t *)app;

  DT_Q(dttool_parse_arg_string,(param,	/* we have a source pattern? 		*/
    arg, 0, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dtstr_choice,(ctl, patterns,		/* make sure pattern is understood	*/
    arg[0].val, &a->pattern));

  if (a->pattern == -1)					/* source pattern is understood? 	*/
  {
	dtstr_printf(ctl, 
      msg, DT_ENTER_MSG_MAXLEN,
      "Sorry, \"%s\""
      " is not understood.", 
      arg[0].val);
    *start = arg[0].desc;
    return DT_RC_GOOD;
  }

  if (a->pattern < -1)					/* source pattern not ambiguous? 	*/
  {
	dtstr_printf(ctl, 
      msg, DT_ENTER_MSG_MAXLEN,
      "Sorry, \"%s\" is ambiguous.",
      arg[0].val);
    *start = arg[0].desc;
    return DT_RC_GOOD;
  }

  DT_Q(dttool_parse_arg_string,(param,	/* we have a destination image? 	*/
    arg, 1, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  if (param->precision != NULL)
  {
    DT_Q(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &a->ntype,
      msg, DT_ENTER_MSG_MAXLEN));
	if (msg[0] != '\0')					/* bad precision? 					*/
	  *start = param->precision;
  }
  else
    a->ntype = DTNTYPE_B08;			/* default precision 				*/
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_gen)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 48
  char pattern_name[ARG_MAXLEN];
  char image_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dt_rc_e rc = DT_RC_GOOD;
  

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->op,        NULL, NULL, NULL,
    &param->n,         NULL, NULL, NULL,
    NULL));

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_SECONDARY,
    &param->w,          NULL,                    NULL, "256",
    &param->h,          NULL,                    NULL, "256",
    &param->dx,         NULL,                    NULL, NULL,
    &param->dy,         NULL,                    NULL, NULL,
    &param->foreground, NULL,                    NULL, "192",
    &param->background, NULL,                    NULL, "64",
    &param->precision,  NULL,                    NULL, "8bit",
    &param->sigma,      NULL,                    NULL, NULL,
    &param->mu,         NULL,                    NULL, NULL,
    &param->omega,      NULL,                    NULL, NULL,
    &param->phi,        NULL,                    NULL, NULL,
    &param->max,        NULL,                    NULL, NULL,
    &param->x,          NULL,                    NULL, "-1",
    &param->y,          NULL,                    NULL, "-1",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Pattern", NULL, 
      pattern_name, sizeof(pattern_name),
    "Output image", NULL, 
      image_name, sizeof(image_name),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_gen_arg_check, &a));

										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
	dtimage_t image;					/* image structure					*/
    image.xe = 0;
    DT_GI(dtimage_create,(ctl, &image,	/* get space for image			    */
      param->w, param->h, a.ntype));

    DT_GI((func[a.pattern]),(ctl,		/* call function to gen pattern	    */
      &image, param));				

    DT_GI(dtimage_export,(ctl,			/* put image into file or window	*/
      &image, image_name,
      (dtxy_t)param->x,
      (dtxy_t)param->y));

    if (image.xe != 0)
      DT_GI(dtimage_free,(ctl,			/* free image						*/
        &image));
	image.xe = 0;
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, USAGE, arg,
      dttool_gen_arg_check, NULL, rc));
  } 
										/* ................................ */

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dtparam_free,(param));			/* free param space					*/

  return rc;
}


/*..........................................................................
 *      00000000
 *      00000000
 *      00000000
 *      00000000
 *..........................................................................*/
static
dt_rc_e
dttool_gen__1(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__1");

  DT_Q(dtimage_constant,(ctl, image,
    param->background));
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *      00001111
 *      00001111
 *      00001111
 *      00001111
 * width of left half is given by param->dx
 *..........................................................................*/
static
dt_rc_e
dttool_gen__2v(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__2v");
  dtxy_t half = DT_CAST(dtxy_t, DTPARAM_GET_VALUE(dx, image->xe / 2));
  dtimage_t overlay;
  dt_rc_e rc;
  
  overlay.xe = 0;
  DT_C(dtimage_overlay,(ctl,			/* left half					    */
    image, &overlay,
    0, 0, 
    half, image->ye));
  DT_C(dtimage_constant,(ctl, &overlay,
    param->background));
  DT_Q(dtimage_free,(ctl, &overlay));
  
  overlay.xe = 0;
  DT_C(dtimage_overlay,(ctl,			/* left half					    */
    image, &overlay,
    half, 0, 
    image->xe-half, image->ye));
  DT_C(dtimage_constant,(ctl, &overlay,
    param->foreground));
  
cleanup:
  if (overlay.xe != 0)
  DT_I(dtimage_free,(ctl, &overlay));

  return rc;
}

/*..........................................................................
 *      00000000
 *      00000000
 *      11111111
 *      11111111
 * width of top half is given by param->dy
 *..........................................................................*/

static
dt_rc_e
dttool_gen__2h(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__2h");
  dtxy_t half = DT_CAST(dtxy_t, DTPARAM_GET_VALUE(dy, image->ye / 2));
  dtimage_t overlay;
  dt_rc_e rc;
  
  overlay.xe = 0;
  DT_C(dtimage_overlay,(ctl,			/* left half					    */
    image, &overlay,
    0, 0, 
    image->xe, half));
  DT_C(dtimage_constant,(ctl, &overlay,
    param->background));
  DT_Q(dtimage_free,(ctl, &overlay));
  
  overlay.xe = 0;
  DT_C(dtimage_overlay,(ctl,			/* left half					    */
    image, &overlay,
    0, half,
    image->xe, image->ye-half));
  DT_C(dtimage_constant,(ctl, &overlay,
    param->foreground));
  
cleanup:
  if (overlay.xe != 0)
  DT_I(dtimage_free,(ctl, &overlay));

  return rc;
}

/*..........................................................................
 *      00001111
 *      00001111
 *      00001111
 *      00001111
 *..........................................................................*/
static
dt_rc_e
dttool_gen__3v(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__3v");
  dtxy_t x, y;
  dt_rc_e rc;
  dtntype_e *ntypes = NULL;

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<image->ye; y++)
  {
    for (x=0; x<image->xe; x++)
      DT_Q(dtimage_set_pixel,(ctl, image, x, y, x));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *      00000000
 *      00000000
 *      11111111
 *      11111111
 *..........................................................................*/

static
dt_rc_e
dttool_gen__3h(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__3h");
  dtxy_t x, y;
  dt_rc_e rc;
  dtntype_e ntypes[] =
    {DTNTYPE_B08, DTNTYPE_NULL};

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<image->ye; y++)
  {
    for (x=0; x<image->xe; x++)
      image->row[y].b08[x] = y;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *      01234567
 *      12345678
 *      23456789
 *      3456789a
 *..........................................................................*/

static
dt_rc_e
dttool_gen__4(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__4");
  dtxy_t x, y;
  dt_rc_e rc;
  dtntype_e ntypes[] =
    {DTNTYPE_B08, DTNTYPE_NULL};

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<image->ye; y++)
  {
    for (x=0; x<image->xe; x++)
      image->row[y].b08[x] = x + y;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *      0000000
 *      0123456
 *      02468ae
 *      0369cf+
 *..........................................................................*/

static
dt_rc_e
dttool_gen__5(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__5");
  dtxy_t x, y;
  dtxy_t max = image->xe * image->ye;
  dt_rc_e rc;
  dtntype_e ntypes[] =
    {DTNTYPE_B08, DTNTYPE_NULL};

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0; y<image->ye; y++)
  {
    for (x=0; x<image->xe; x++)
      image->row[y].b08[x] = (256L * x * y) / max;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *      exp(-((x-x0) * (x-x0) + (y-y0) + (y-y0)) / (sigma * sigma))
 *..........................................................................*/

static
dt_rc_e
dttool_gen__6(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__6");
  dtxy_t x, y;
  dtxy_t xmax = image->xe - 1;
  dtxy_t ymax = image->ye - 1;
  dtxy_t x0 = image->xe / 2;
  dtxy_t y0 = image->ye / 2;
  double sigma_squared;
  double maxval;
  dt_rc_e rc;
  dtntype_e ntypes[] =
    {DTNTYPE_B08, DTNTYPE_NULL};

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;


  if (param->sigma != 0.0)				/* caller specified a sigma?	    */
    sigma_squared =
      param->sigma * param->sigma;
  else									/* no specified sigma?			    */
    sigma_squared = (x0 / 2)*(y0 / 2);	/* make up a nice one			    */

  maxval = dt_ntype_maxval[DT_NTYPE_B08];
  for (y=0; y<=y0; y++)
  {
    for (x=0; x<=x0; x++)
	{
      dt_ntype_b08_t d =				/* compute the upper left quadrant  */ 
        (dtntype_b08_t)
          (exp(-(double)((x-x0) * (x-x0) +
                 (y-y0) * (y-y0)) /
             sigma_squared) * maxval);
      image->row[y     ].b08[x]     =d;	/* axially symmetric			    */
      image->row[ymax-y].b08[xmax-x]=d; 
      image->row[y     ].b08[xmax-x]=d;
      image->row[ymax-y].b08[x]     =d;
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttool_gen__7(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__7");
  dtxy_t x, y;
  dtxy_t xmax = image->xe / 8;
  dtntype_b01_t val;
  dt_rc_e rc;
  dtntype_e ntypes[] =
    {DTNTYPE_B01, DTNTYPE_NULL};

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", ntypes, F);
  if (rc != DT_RC_GOOD)
    return rc;


  for (y=0; y<image->ye; y++)
  {
    if (y & 0x04)
      val = 0x0f;
    else
      val = 0xf0;
    for (x=0; x<xmax; x++)
      image->row[y].b01[x] = val;
  }
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttool_gen__8(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__8");
  dtimage_t random;
  dt1d_ran_t ran;
  dtxy_t y;
  double max, min;
  double scale;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", NULL, F);
  if (rc != DT_RC_GOOD)
    return rc;

  random.xe = 0;
  DT_C(dtimage_create,(ctl,
    &random,
    image->xe, image->ye,
    DTNTYPE_F2));

  DT_C_1D(dt1d_ran_uniform_init,(&ran,
    param->n));

  for (y=0; y<random.ye; y++)
  {
    DT_C_1D(dt1d_random_uniform_double,(	/* fill image with random noise	    */
      &ran,
      random.row[y].f2,
      random.xe));
  }

    min = dt_ntype_minval[image->ntype];
    max = dt_ntype_maxval[image->ntype];
    scale = max - min + 1.0;
    DT_C(dtimage_arith,(ctl,			/* scale to output				    */
      &random, scale,
      &random, 0.0, 0.0, DT_OP_ADD, 0,
      image));

cleanup:
  if (random.xe)
  DT_Q(dtimage_free,(ctl, &random));

  return rc;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttool_gen__9(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__9");
  dt1d_ran_t ran;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check output image type			*/
    image, "image", NULL, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtimage_constant,(ctl, image, 0.0));

  DT_Q_1D(dt1d_ran_uniform_init,(&ran, param->n));

  DT_Q(dtimage_noise_gauss_add,(ctl, image, &ran, param->mu, param->sigma));
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttool_gen__10(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__10");
  DT_Q(dtimage_gen_rect_tree,(ctl,
    image,
    param->background,
    param->foreground,
    param->n > 0? (int)param->n: 999));
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttool_gen__11(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__11");
  DT_Q(dtimage_gen_rect_spiral,(ctl,
    image,
    param->background,
    param->foreground,
    param->n > 0? (int)param->n: 2));
  return DT_RC_GOOD;
}


/*..........................................................................
 * 10101010
 * 00000000
 * 10101010
 * 00000000
 * 10101010
 * 00000000
 * pattern element width is param->dx
 * pattern element height is param->dy
 *..........................................................................*/
static
dt_rc_e
dttool_gen__12(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__12");
  dtxy_t dx = DT_CAST(dtxy_t, DTPARAM_GET_VALUE(dx, 1));
  dtxy_t dy = DT_CAST(dtxy_t, DTPARAM_GET_VALUE(dy, 1));
  DT_Q(dtimage_gen_rect_islands,(ctl,
    image,
    param->background,
    param->foreground,
    param->n > 0? (int)param->n: 1,
    dx, dy));
  return DT_RC_GOOD;
}


/*..........................................................................
 * 10101010
 * 01010101
 * 10101010
 * 01010101
 * 10101010
 * 01010101
 * pattern element width is param->dx
 * pattern element height is param->dy
 *..........................................................................*/
static
dt_rc_e
dttool_gen__13(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__13");
  dtxy_t dx = DT_CAST(dtxy_t, DTPARAM_GET_VALUE(dx, 1));
  dtxy_t dy = DT_CAST(dtxy_t, DTPARAM_GET_VALUE(dy, 1));
  DT_Q(dtimage_gen_checkerboard,(ctl,
    image,
    param->background,
    param->foreground,
    dx, dy)); 
  return DT_RC_GOOD;
}


/*..........................................................................
 * grayscale background with border and X on it
 *..........................................................................*/
static
dt_rc_e
dttool_gen__test1(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__test1");
  DT_Q(dtimage_gen_test1,(ctl, image));
  return DT_RC_GOOD;
}

/*..........................................................................
 * grayscale background with border and X on it
 *..........................................................................*/
static
dt_rc_e
dttool_gen__test2(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__test2");
  DT_Q(dtimage_gen_test2,(ctl, image));
  return DT_RC_GOOD;
}

/*..........................................................................
 * grayscale background with border and X on it
 *..........................................................................*/
static
dt_rc_e
dttool_gen__test3(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__test3");
  DT_Q(dtimage_gen_test3,(ctl, image));
  return DT_RC_GOOD;
}

/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__color256(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__color256");
  DT_Q(dtimage_gen_color256,(ctl, image));
  return DT_RC_GOOD;
}


/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__grid1(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__grid1");
  DT_Q(dtimage_gen_grid1,(ctl, image, param));
  return DT_RC_GOOD;
}

/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__grid2(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__grid2");
  DT_Q(dtimage_gen_grid2,(ctl, image, param));
  return DT_RC_GOOD;
}


/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__xsine(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__xsine");
  DT_Q(dtimage_gen_xsine,(ctl, image, param));
  return DT_RC_GOOD;
}


/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__ysine(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{

  DT_F("dttool_gen__ysine");
  DT_Q(dtimage_gen_ysine,(ctl, image, param));
  return DT_RC_GOOD;
}


/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__kernel(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dttool_gen__kernel");
  const char *kernel_name = DTPARAM_GET_VALUE(op, "average");
  DT_Q(dtimage_gen_kernel,(ctl, image, kernel_name));
  return DT_RC_GOOD;
}


/*..........................................................................
 *
 *..........................................................................*/
static
dt_rc_e
dttool_gen__interlock16(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{

  DT_F("dttool_gen__interlock16");
  DT_Q(dtimage_gen_interlock16,(ctl, 
    image, 
    param->background,
    param->foreground));
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
