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
| dttoolhist utility - make histogram of image
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
DT_RCSID("tool $RCSfile: hist.c,v $ $Revision: 1.4 $");
#include <dtack/os.h>
#include <dtack/lut8.h>

static const char *ops[] = {
  "both", "plot", "print",
  NULL
};

static const char *types[] = {
  "=gaussian", 
  "=uniform",
  NULL
};

static dt_double_f *funcs[] = {
  dthist_bin_gaussian, 
  dthist_bin_uniform
};

#define USAGE "[params] source dest"

#define HMAX (256)
#define VMAX (128)

#define BACKGROUND_COLOR  64			/* gray for plot background		    */
#define BAR_COLOR         DTLUT8_WHITE	/* white for histogram bars		    */

typedef struct {
  dt_ntype_e ntype;
  int type;
} app_t;

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttool_hist_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_hist_arg_check");
  app_t *a = (app_t *)app;

  DT_Q(dttool_parse_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));
  
  DT_Q(dttool_parse_arg_string,(param,	/* we have a source image? 			*/
    arg, 0, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  a->type = -1;
  if (arg[0].val[0] ==					/* input starts with special char? 	*/
	  DT_SEPARATOR_ENUM)
  {
    DT_Q(dtstr_choice2,(ctl, types, 
      arg[0].val, &a->type,
      msg, DT_ENTER_MSG_MAXLEN));
	if (msg[0] != '\0')					/* bad string? 						*/
	{
	  *start = arg[0].val;
	  return DT_RC_GOOD;
	}
  }

  if (DT_ABBREV("plot", param->op))
  {
    DT_Q(dttool_parse_arg_string,(		/* we have a destination image? 	*/
      param, arg, 1, NULL, msg, start));
    if (msg[0] != '\0')
      return DT_RC_GOOD;
  }

  if (param->precision != NULL)			/* precision specified? 			*/
  {
    DT_Q(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &a->ntype,
      msg, DT_ENTER_MSG_MAXLEN));
	if (msg[0] != '\0')					/* bad precision? 					*/
	  *start = param->precision;
  }
  else
    a->ntype = DT_NTYPE_UNKNOWN;		/* default precision 				*/
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
dttool_hist_doit(
  dtparam_t *param,
  app_t *a,
  const char *input_name,
  const char *output_name)
{
  DT_F("dttool_hist_doit");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t image;
  dtimage_t hplot;
  dthist_t hist;
  dt_rc_e rc;

  image.xe = 0;
  hist.width = 0;
  hplot.xe = 0;

  if (a->type == -1)					/* input is an image				*/
  {
	DT_C(dtimage_import,(ctl,			/* get image from file or window	*/
      &image, input_name));
	DT_Q(dthist_alloc,(ctl,				/* space for histogram				*/
      &hist, 256,
      dt_ntype_minval[image.ntype],
      dt_ntype_maxval[image.ntype] +
      !DTNTYPE_ISFLOAT(image.ntype)));	/* plus 1 if integer type 			*/
	DT_C(dtimage_hist,(ctl,				/* make histogram from an image		*/
      &image, &hist));
  }
  else
  {
	dtstats_t stats;
	DT_Q(dthist_alloc,(ctl,				/* space for histogram				*/
      &hist, 256,
      dt_ntype_minval[a->ntype] - 0.5 *
        !DTNTYPE_ISFLOAT(a->ntype),
      dt_ntype_maxval[a->ntype] + 0.5 *
        !DTNTYPE_ISFLOAT(a->ntype)));
	DT_MEMSET(&stats, 0, sizeof(stats));
	stats.variance = 
	  param->sigma * param->sigma;
	stats.mean = param->mu;				/* sets scale for uniform			*/
	stats.sum = 256 * 256;				/* sets scale for Gaussian 			*/
	DT_C(dthist_gen,(ctl,				/* generate histogram 				*/
      &hist, funcs[a->type],
      (void *)&stats));
  }

  if (DT_ABBREV("print", param->op) ||
	  DT_ABBREV("both", param->op))
  {
	unsigned int i;
	for (i=0; i<hist.width; i++)
	{
	  dthist_freq2_t *freq = (dthist_freq2_t *)hist.freq;
	  double min, max;
	  DT_C(dthist_bin_to_value,(ctl,
        &hist, i, &min, &max));
	  if (freq[i] >= param->t)			/* or this value is big enough? 	*/
	    dt_say(ctl, F,
          "%4d. (%8g -> %8g) %8lu",
          i, min, max, freq[i]);
	}
  }

  if (DT_ABBREV("plot", param->op) ||
	  DT_ABBREV("both", param->op))
  {
    DT_C(dtimage_create,(ctl,			/* get space for histogram plot		*/
      &hplot, HMAX, VMAX,
      DT_NTYPE_B08));

    DT_C(dtimage_constant,(ctl,			/* clear histogram plot				*/
      &hplot, param->background));

    DT_C(dtimage_hist_plot_horiz,(ctl,	/* make bar plot of histogram		*/
      &hplot, &hist,
      2, hplot.ye-4, 
      param->foreground));

    DT_C(dtimage_export,(ctl,			/* output histogram plot			*/
      &hplot, output_name, 
      DTIMAGE_NOPOS, DTIMAGE_NOPOS));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_STATS))
  {
	dtstats_t stats;
	DT_MEMSET(&stats, 0, sizeof(stats));
	stats.want =
	  DTSTATS_AREA |
	  DTSTATS_SUM |
	  DTSTATS_MEAN |
	  DTSTATS_VARIANCE |
	  DTSTATS_STANDARD_DEVIATION;
	DT_C(dthist_stats,(ctl, &hist,
      &stats));

	if (stats.have & DTSTATS_AREA)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %ld",
        "area", stats.area);
	}

	if (stats.have & DTSTATS_SUM)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %g",
        "sum", stats.sum);
	}

	if (stats.have & DTSTATS_MEAN)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %g",
        "mean", stats.mean);
	}

	if (stats.have & DTSTATS_VARIANCE)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %g",
        "variance", stats.variance);
	}

	if (stats.have & DTSTATS_STANDARD_DEVIATION)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_STATS,
        "%-20s = %g",
        "standard deviation", stats.standard_deviation);
	}
  }
  
cleanup:
  if (hplot.xe != 0)
  DT_I(dtimage_free,(ctl, &hplot));		/* free histogram plot				*/

  if (hist.width != 0)
  DT_I(dthist_free,(ctl, &hist));		/* free histogram					*/

  if (image.xe != 0)
  DT_I(dtimage_free,(ctl, &image));		/* free image						*/
    
  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_hist)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[3];
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  int assert_fail = 0;					/* failed assertion count		    */
  app_t a;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "plot",
    &param->foreground, NULL, NULL, "64",
    &param->background, NULL, NULL, "192",
    &param->t, "Print threshold", NULL, "0",	/* minimum value printed	*/
    &param->mu, "Generated mean", NULL, "128",
    &param->sigma, "Generated stddev", NULL, "20",
    &param->precision, "Generated precision", NULL, "8bit",
    NULL));
  
  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Histogram plot", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_hist_arg_check, &a));
  
  while (rc == DT_RC_GOOD)
  {
	DT_G(dttool_hist_doit,(param, &a,	/* do the work 						*/
      input_name, output_name));

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttool_hist_arg_check, 
      &a, rc));
  }

										/* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dtparam_free,(param));			/* free param space					*/

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
