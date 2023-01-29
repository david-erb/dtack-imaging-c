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
| dt_sez90_samp1b
| 
| DESCRIPTION
| For figure in manual.
| Make sine-wave "argv[1].hist" with just middle peak boxed.
| Make sine-wave "argv[1].ends" with all peaks boxed.
| Draw pds curve in separate image "argv[1].pds"
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

#include <dtack/base.h>
DT_RCSID("sez90 $RCSfile: samp_1b.c,v $ $Revision: 1.4 $");
#include <dt_sez90.h>

#define USAGE "[params] plot"

#define MAXGRAY (256)
#define NGRAY ((int)param.w)
#define PLOT_HEIGHT (param.h)

#define BAR_COLOR (param.foreground)
#define BOX_COLOR (param.foreground)
#define PDS_COLOR (param.foreground)
#define ZERO_COLOR (param.foreground)
#define BACKGROUND_COLOR (param.background)

/*..........................................................................*/
static
void
sinewave(
  double a,
  double w,
  double p,
  double *y,
  int n)
{
  int i;
  double inc = 2.5 * DT_PI / (double)n;
  for (i=0; i<n; i++)
  {
    y[i] = a * sin(w * (double)i * inc + p) + a;
  }
}
/*..........................................................................*/

static char *F = "dt_sez90_samp1b";

int
main(
  int argc,
  char *argv[])
{
  dtimage_t plott = {0};				/* plott is plot plus small border  */
  dtimage_t plot = {0};					/* plot is NGRAY wide			    */
  dtparam_t param;						/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  dt1d_ran_t ran;
  double wave[MAXGRAY];
  double random[MAXGRAY];
  double hist[MAXGRAY];
  double pds[MAXGRAY];
  char marks[MAXGRAY+2];
  char *mark = marks;
  char plotname[128];
  int n;
  dt_rc_e rc;
  *mark++ = '\0'; *mark++ = '\0';		/* trick for box background		    */
										/* ................................ */
  DT_G(dtparam_init,(&param));			/* init param structure             */
  DTPARAM_DEFAULT_P(&param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(&param, w,          MAXGRAY);
  DTPARAM_DEFAULT_P(&param, h,          128);
  DTPARAM_DEFAULT_P(&param, omega,      2);
  DTPARAM_DEFAULT_P(&param, phi,        0);
  DTPARAM_DEFAULT_P(&param, sigma,      0.15);
  DTPARAM_DEFAULT_P(&param, mu,         0);
  DTPARAM_DEFAULT_P(&param, n,          DT_SEZ90_DEFAULT_N);
  DTPARAM_DEFAULT_P(&param, dx,         DT_SEZ90_DEFAULT_CLOSENESS);
  DTPARAM_DEFAULT_P(&param, foreground, 0);
  DTPARAM_DEFAULT_P(&param, background, 228);

  DT_C(dt_enter_param,(					/* go parse args					*/
    argc, argv, F, USAGE, &param, &n));

  argc -= n;
  argv += n;
  if (argc != 1)						/* check remaining arg(s)			*/
    DT_C(dt_usage_say,(
      &param, USAGE));

										/* ................................ */
  plot.xe = 0;
  DT_C(dtimage_create,(ctl,				/* space for plot image			    */
    &plott, NGRAY+2, PLOT_HEIGHT,
    DT_NTYPE_B08));
  DT_C(dtimage_overlay,(ctl,
    &plott, &plot, 1, 0, NGRAY,
    plott.ye));
										/* ................................ */
  sinewave(1.0, param.omega, param.phi,	/* make sine wave				    */
    wave, NGRAY);
  DT_C(dt1d_ran_gauss_init,(			/* get ready to do gauss noise		*/
    &ran, 0));
  DT_C(dt1d_random_gauss_double,(		/* make random vector			    */
    &ran, random, NGRAY));
  DT_C(dt1d_mult_scalar_double,(		/* scale random vector			    */
    random, NGRAY, param.sigma));
  DT_C(dt1d_add_scalar_double,(			/* offset random vector			    */
    random, NGRAY, param.mu));
  DT_C(dt1d_add_double,(				/* add noise to sine wave			*/
    wave, random, NGRAY, 
    hist, NGRAY));

										/* now we have a "histogram"	    */

  DT_C(dt_sez90_pds,(&param,			/* calculate pds					*/
    hist, NGRAY, pds));

  DT_C(dt_sez90_mark,(&param,			/* mark peaks in signal				*/
    hist, NGRAY, pds, mark));

										/* ................................ */
  DT_C(dtimage_constant,(ctl,			/* clear plot image				    */
    &plott, BACKGROUND_COLOR));

  {
    dt1d_i_t min, max, t, t0, t1;
	double pmin, pmax;

    DT_C(dt1d_range_double,(			/* highest pds point for scaling 	*/
      pds, NGRAY, &min, &max));
    pmin = pds[min];
    pmax = pds[max];
    if (pmin > 0)						/* make zero coincide on graph	    */
      pmin = -pmax;
    else
    if (pmax < 0)
      pmax = -pmin;
    else
    if (pmax > -pmin)
      pmin = -pmax;
    else
      pmax = -pmin;

#   define Y(U) ((dtxy_t)(plot.ye-1-(plot.ye-2)*((U-pmin)/(pmax-pmin))))

	DT_C(dtimage_plot_line,(ctl,		/* draw 0 line					    */
      &plot,
      0, Y(0.0), NGRAY-1, Y(0.0),
      ZERO_COLOR));

    t0 = (int)param.n / 2 + 1;
    t1 = NGRAY - (int)param.n / 2 - 1;
    for (t=t0; t<t1; t++)				/* for all pds values			    */
	  DT_C(dtimage_plot_line,(ctl,
        &plot,
        t, Y(pds[t]),
        t, Y(pds[t-1]), PDS_COLOR));
  }

  DT_C(dt_sez90_plot_boxes,(&param,		/* box around peak				    */
    mark, NGRAY, &plot, BOX_COLOR));

  sprintf(plotname, "%s.pds",			/* name for pds plot			    */
    argv[0]);
  DT_C(dtimage_export,(ctl,				/* output plot						*/
    &plot, plotname, -1, -1));

										/* ................................ */
  DT_C(dtimage_constant,(ctl,			/* clear plot image				    */
    &plott, BACKGROUND_COLOR));

  DT_C(dtimage_plot_xbars_double,(		/* make bar plot of histogram		*/
    ctl, &plot, hist, NGRAY,	
    4, plot.ye-8, BAR_COLOR));

  DT_C(dt_sez90_plot_boxes,(&param,		/* background for box				*/
    mark-2, NGRAY+2, &plott,
    BACKGROUND_COLOR));
  DT_C(dt_sez90_plot_boxes,(&param,
    mark, NGRAY, &plott,
    BACKGROUND_COLOR));
  DT_C(dt_sez90_plot_boxes,(&param,		/* box around peak				    */
    mark, NGRAY, &plot, BOX_COLOR));

  sprintf(plotname, "%s.hist",			/* name for histogram plot		    */
    argv[0]);
  DT_C(dtimage_export,(ctl,				/* output plot						*/
    &plot, plotname, -1, -1));
										/* ................................ */

  DT_C(dt_sez90_ends,(&param,			/* mark peaks on ends			  	*/
    hist, NGRAY, mark));

  DT_C(dt_sez90_plot_boxes,(&param,		/* background for boxes				*/
    mark-2, NGRAY+2, &plott,
    BACKGROUND_COLOR));
  DT_C(dt_sez90_plot_boxes,(&param,
    mark, NGRAY, &plott,
    BACKGROUND_COLOR));
  DT_C(dt_sez90_plot_boxes,(&param,		/* boxes around peaks			    */
    mark, NGRAY, &plot, BOX_COLOR));

  sprintf(plotname, "%s.ends",			/* name for histogram plot		    */
    argv[0]);
  DT_C(dtimage_export,(ctl,				/* output plot						*/
    &plot, plotname, -1, -1));

cleanup:

  if (plot.xe)
    dtimage_free(ctl, &plot);

  if (plott.xe)
    dtimage_free(ctl, &plott);

  DT_Q(dtparam_free,(&param));			/* free param space					*/

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
