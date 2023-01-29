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
| dt_sez90_samp2
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

static char rcsid[] = "sez90 $RCSfile: samp2.c,v $ $Revision: 1.1 $";

#include <dtack/base.h>
#include <dtwin.h>
#include <dt_sez90.h>

#define USAGE "[params] image plot"

#define NGRAY (256)

#define BAR_COLOR (0)					/* black for hist plot			    */
#define BACKGROUND_COLOR (228)			/* white for background				*/

/*..........................................................................*/

static char *F = "dt_sez90_samp2";

int
main(
  int argc,
  char *argv[])
{
  dtimage_t input = {0}, plot = {0};	/* image structures					*/
  dtparam_t param;						/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  double hist[256];
  char mark[256];
  long start, maximum, end;
  int n;
  dt_rc_e rc;
										/* -------------------------------- */
  DT_G(dtparam_init,(&param));			/* init param structure             */
  DTPARAM_DEFAULT_P(&param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(&param, n,          DT_SEZ90_DEFAULT_N);
  DTPARAM_DEFAULT_P(&param, dx,         DT_SEZ90_DEFAULT_CLOSENESS);

  DT_C(dt_interactive_parse,(			/* go parse args					*/
    argc, argv, F, USAGE, &param, &n));

  argc -= n;
  argv += n;
  if (argc < 1 || argc > 2)				/* check remaining arg(s)			*/
    DT_C(dt_interactive_usage,(
      &param, USAGE));

										/* -------------------------------- */
  input.xe = 0;
  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, argv[0]));

  plot.xe = 0;
  DT_C(dtimage_create,(ctl,				/* space for plot image			    */
    &plot, NGRAY, 128,
    DT_NTYPE_B08));
  DT_C(dtimage_constant,(ctl,			/* clear plot image				    */
    &plot, BACKGROUND_COLOR));
										/* ................................ */
  DT_C(dtimage_hist,(ctl,				/* make unscaled histogram 			*/
    &input, hist, NGRAY, 0.0));

  DT_C(dtimage_plot_xbars_double,(		/* make bar plot of histogram		*/
    ctl, &plot, hist, NGRAY,
    0, plot.ye, BAR_COLOR));

  DT_C(dt_sez90,(						/* mark peaks in signal				*/
    &param, hist, NGRAY, mark));

  end = 0;
  do {
    DT_C(dt_sez90_peak,(&param,			/* find next peak				    */
      mark, NGRAY, end,
      &start, &maximum, &end));
    if (start > 0 &&					/* peak doesn't start at 0?		    */
        end < NGRAY)					/* peak is identifiable?		    */
	{
#     define VLINE(X, C) \
	  DT_C(dtimage_plot_line,(ctl, \
        &plot, X, 0, X, plot.ye-1, C));

      if (start > 0)
      VLINE(start-1, BACKGROUND_COLOR);
      VLINE(start, BAR_COLOR);
      if (start < plot.xe-1)
      VLINE(start+1, BACKGROUND_COLOR);
      dt_say(ctl, F, "start %3ld  maximum %3ld  end %3ld",
        start, maximum, end);
	}
  } while (end < NGRAY);

  DT_C(dtimage_export,(ctl,				/* output plot						*/
    &plot, argv[1], -1, -1));

cleanup:

  if (plot.xe)
    dtimage_free(ctl, &plot);			/* free images						*/

  if (input.xe)
    dtimage_free(ctl, &input);

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
