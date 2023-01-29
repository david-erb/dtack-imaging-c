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
| dttoolmultithresh utility - multiple threshold gray level display
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
DT_RCSID("tool $RCSfile: multithr.c,v $ $Revision: 1.4 $");
#include <dtack/lut8.h>

#define USAGE "[params] source thresh [thresh...] dest"

#define IMAX (256)						/* max number of thresholds 	    */
#define BACKGROUND_COLOR  64			/* gray for plot background		    */
#define BAR_COLOR         DTLUT8_WHITE	/* white for histogram bars		    */
#define THRESH_COLOR      DTLUT8_GREEN	/* green for threshold locations	*/

/*..........................................................................*/

dt_rc_e
dttool_multithresh_plot(				/* make annotated histogram plot 	*/
  dtparam_t *param,
  dtimage_t *input,
  double *thresh,
  dtxy_t n,
  const char *plot_name)
{
  DT_F("dttool_multithresh_plot");
  dt_ctl_t *ctl = &param->ctl;
  dthist_t hist;
  dtimage_t plot;
  dtxy_t i;
  dt_rc_e rc = DT_RC_GOOD;

  hist.freq = NULL;
  plot.xe = 0;
  if (plot_name && *plot_name)			/* not null or empty name?		    */
  {
	DT_C(dthist_alloc,(ctl,				/* space for histogram				*/
      &hist, 256,
      dt_ntype_minval[input->ntype],
      dt_ntype_maxval[input->ntype]));

    DT_C(dtimage_hist,(ctl,				/* make histogram  					*/
      input, &hist));

    DT_C(dtimage_create,(ctl,			/* space for plot image			    */
      &plot, hist.width, 128,
      DT_NTYPE_B08));

    DT_C(dtimage_constant,(ctl,			/* clear plot image				    */
      &plot, BACKGROUND_COLOR));		
	
    DT_C(dtimage_hist_plot_horiz,(ctl,	/* make bar plot of histogram		*/
      &plot, &hist,
      2, plot.ye-4, BAR_COLOR));
    for (i=0; i<n; i++)					/* for all threshold values		    */
	  DT_C(dtimage_plot_line,(ctl,
        &plot,
        DT_CAST(dtxy_t, thresh[i]),
        0,
        DT_CAST(dtxy_t, thresh[i]),
        plot.ye-1, 
        THRESH_COLOR));
    DT_C(dtimage_export,(ctl,			/* output plot						*/
      &plot, (char *)plot_name,
      -1, -1));
  }

cleanup:
  if (hist.freq != NULL)
    DT_I(dthist_free,(ctl, &hist));		/* free histogram					*/
  if (plot.xe)
    DT_I(dtimage_free,(ctl, &plot));	/* free plot image					*/

  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_multithresh)
{
  dtimage_t input;						/* image structure					*/
  
  dt_ctl_t *ctl = &param->ctl;
  char *arg_input;
  char *arg_output;
  double thresh[IMAX];
  double values[IMAX+1];
  double inc;
  int isnumber;
  dt_rc_e rc;
  int i, n;
										/* -------------------------------- */
  DT_G(dtparam_init,(param));			/* init param structure             */
  DTPARAM_DEFAULT_P(param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(param,
    background, 0);	
  DTPARAM_DEFAULT_P(param,
    foreground, 255);	
  DTPARAM_DEFAULT_P(param,
    output, NULL);	
  DT_GI(dt_enter_param,(					/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc < 3)							/* check positional args			*/
    rc = dt_usage_say(
      param, USAGE);

  arg_input = argv[0];					/* get positionals				    */
  arg_output = argv[argc-1];

  n = argc - 2;							/* how many thresholds we got	    */
  if (n > IMAX)
    rc = dt_err(ctl, F,
      "%d thresholds on command line is too many",
      n);

  argv++;
  for (i=0; i<n; i++)					/* for all thesholds on line	    */
  {
    DT_GI(dtstr_to_double2,(ctl,		/* try to convert thresh to number	*/
      argv[i], &thresh[i],
      &isnumber));
    if (rc == DT_RC_GOOD && !isnumber)
      rc = dt_err(ctl, F,
        "threshold %d \"%s\" is not a number",
        i, argv[i]);
  }

  DT_GI(dtimage_import,(ctl,			/* get input image					*/
    &input, arg_input));

  if (rc == DT_RC_GOOD)
  {
    inc = (param->foreground -			/* delta gray level per threshold   */
           param->background) / 
           (double)(n + 1);
    for (i=0; i<n; i++)					/* assign values equidistantly	    */
      values[i] = param->background +
        i * inc;
    values[i] = param->foreground;		/* in case of non-integrality	    */
  }

  DT_GI(dttool_multithresh_plot,(param,	/* make optional histogram plot 	*/
    &input, thresh, n, param->output));

  DT_GI(dtimage_thresh_multi,(ctl,		/* threshold image in place			*/
    &input, thresh, n, values,
    &input));

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &input, arg_output, 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (input.xe)
  DT_Q(dtimage_free,(ctl,				/* free input image					*/
    &input));

  DT_Q(dtparam_free,(param));			/* free param space					*/

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
