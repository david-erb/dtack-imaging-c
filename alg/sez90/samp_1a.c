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
| dt_sez90_samp1
| 
| DESCRIPTION
| Make sine-wave "histogram". 
| Overlay pds curve.
| Draw colored boxes.
| Print out sine numbers and pds numbers to be used as test data input.
|
| For test5x series:
| sez901a -Dpixel -w 64 -n 15 :plot
| For test6x series:
| sez901a -Dpixel -w 128 -n 7 -omega 6 :plot
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
DT_RCSID("sez90 $RCSfile: samp_1a.c,v $ $Revision: 1.5 $");

#include <dtack/lut8.h>
#include <dt_sez90.h>

#define USAGE "[params] plot"

#define MAXGRAY (256)
#define NGRAY ((int)param.w)
#define PLOT_HEIGHT (param.h)

#define BAR_COLOR  DTLUT8_WHITE			/* white for hist plot			    */
#define PDS_COLOR  DTLUT8_BLUE			/* blue for pds plot			    */
#define BOX_COLOR  DTLUT8_GREEN			/* green for boxes around peaks	    */
#define MAX_COLOR  DTLUT8_RED			/* red for maxima of peaks		    */
#define ZERO_COLOR DTLUT8_YELLOW		/* yellow for line at y=0.0			*/
#define BACKGROUND_COLOR DTLUT8_BLACK	/* black for background				*/

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
/*..........................................................................*/

static char *F = "dt_sez90_samp1";

int
main(
  int argc,
  char *argv[])
{
  dtimage_t plot = {0};
  dtparam_t param = {0};				/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  double hist[MAXGRAY];
  double pds[MAXGRAY];
  char mark[MAXGRAY];
  int n;
  int i;
  dt_rc_e rc;
										/* ................................ */
  DT_G(dtparam_init,(&param));			/* init param structure             */
  DTPARAM_DEFAULT_P(&param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(&param, w,          MAXGRAY);
  DTPARAM_DEFAULT_P(&param, h,          128);
  DTPARAM_DEFAULT_P(&param, omega,      2);
  DTPARAM_DEFAULT_P(&param, phi,        0);
  DTPARAM_DEFAULT_P(&param, n,          DT_SEZ90_DEFAULT_N);
  DTPARAM_DEFAULT_P(&param, dx,         DT_SEZ90_DEFAULT_CLOSENESS);

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
    &plot, NGRAY, PLOT_HEIGHT,
    DT_NTYPE_B08));
  DT_C(dtimage_constant,(ctl,			/* clear plot image				    */
    &plot, BACKGROUND_COLOR));
										/* ................................ */
  wave(1.0, param.omega, param.phi,
    hist, NGRAY);

  DT_C(dt_sez90_pds,(&param,			/* calculate pds					*/
    hist, NGRAY, pds));

  if (ctl->dbg_mask & DT_DBG_MASK_PIXEL)
  {
	for (i=0; i<NGRAY; i++)
      printf("%8.5f%s", hist[i],
        i%8==7||i==NGRAY-1? ",\n":",");
	for (i=0; i<NGRAY; i++)
      printf("%8.5f%s", pds[i],
        i%8==7||i==NGRAY-1? ",\n":",");
  }

  DT_C(dt_sez90_mark,(&param,			/* mark peaks in signal				*/
    hist, NGRAY, pds, mark));

  DT_C(dt_sez90_ends,(&param,			/* mark ends in signal				*/
    hist, NGRAY, mark));

  dt_sez90_mark_dbg(&param,				/* print out values				    */
    hist, NGRAY, mark, 
    F, DT_DBG_MASK_ANY);

  DT_C(dtimage_plot_xbars_double,(		/* make bar plot of histogram		*/
    ctl, &plot, hist, NGRAY,
    0, plot.ye, BAR_COLOR));

  {
    dt1d_i_t min, max;
	double pmin, pmax;
    dtxy_t t;

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

    for (t=0; t<NGRAY; t++)				/* for all pds values			    */
	  DT_C(dtimage_plot_line,(ctl,
        &plot,
        t, Y(pds[t]),
        t, Y(pds[t-1]), PDS_COLOR));
  }

  DT_C(dt_sez90_plot_boxes,(&param,		/* plot boxes around peaks		    */
    mark, NGRAY, &plot, BOX_COLOR));

  DT_C(dt_sez90_plot_maxes,(&param,		/* plot maxes of peaks			    */
    mark, NGRAY, &plot, MAX_COLOR));

  DT_C(dtimage_export,(ctl,				/* output plot						*/
    &plot, argv[0], -1, -1));

cleanup:

  if (plot.xe)
    dtimage_free(ctl, &plot);			/* free images						*/

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
