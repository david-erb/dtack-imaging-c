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
| dt_wes78 command - Levine&Nazif 1985
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

static char rcsid[] = "util $RCSfile: cmd.c,v $ $Revision: 1.1 $";

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/1d.h>
#include <dt_wes78.h>
#include <dt_sez90.h>

#define USAGE "[params] image"

#define BACKGROUND (0)					/* background of thresh image	    */
#define FOREGROUND (255)				/* foreground of thresh image	    */

#define NGRAY (256)						/* gray levels to deal with		    */

#define BAR_COLOR (255)					/* white for busyness bars		    */
#define BAR_SUFFIX ".wes78"
#define PDS_COLOR 1						/* blue for pds curve				*/
#define BOX_COLOR 4						/* green for boxes around peaks	    */
#define MAX_COLOR 2						/* red for maxima of peaks		    */

/*..........................................................................*/
dt_rc_e 
dtimage_plot_xcurve_double(				/* plot curve, value at each x		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  double *vals,							/* array of values				    */
  long n,								/* number of bars				    */
  long valzero,							/* where minimum value goes			*/
  long valsize,							/* size of maximum value		    */
  double color)
{
  static char *F = "dtimage_plot_xcurve_double";
  long min;								/* min data on plot				    */
  long max;								/* max data on plot				    */
  double scale;
  long i;

  n = DT_MIN(n, image->xe);

  DT_Q(dt1d_range_double,(ctl, vals, n,	/* get range of values				*/
    &min, &max));

  scale = (double)(valsize-1) /			/* scale data to fit on plot	    */
        (vals[max] - vals[min]);

  for (i=1; i<n; i++)					/* for all array values			    */
    DT_Q(dtimage_plot_line,(ctl,
      image,
      i, image->ye - 1 - (vals[i-1]-vals[min]) * scale + valzero,
      i, image->ye - 1 - (vals[i]-  vals[min]) * scale + valzero,
      color));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dt_wes78_all(
  dtparam_t *param,
  dtimage_t *input,
  double *busyness)
{
  static char *F = "dt_wes78_all";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t cooc, A, B;
  int n, t;
  double total;
  dt_rc_e oprc, rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    input, "input", NULL, F);
  if (rc != DT_RC_GOOD)
    return rc;

  n = 256;

  cooc.xe = 0;
  DT_GI(dtimage_create,(ctl,
    &cooc, n, n, DT_NTYPE_B32));
  A.xe = 0;
  DT_GI(dtimage_create,(ctl,
    &A, n, n, DT_NTYPE_B32));
  B.xe = 0;
  DT_GI(dtimage_create,(ctl,
    &B, n, n, DT_NTYPE_B32));

  DT_GI(dtimage_cooc_set,(ctl,			/* compute and sum cooc matrices   	*/
    input, 1,  0, &cooc));
  DT_GI(dtimage_cooc_sum,(ctl,
    input, 0,  1, &cooc));
  DT_GI(dtimage_cooc_sum,(ctl,
    input, 1,  1, &cooc));
  DT_GI(dtimage_cooc_sum,(ctl,
    input, 1, -1, &cooc));

  DT_GI(dtimage_sum_cumulative,(		/* shaded area B				    */
    ctl, &cooc, &B));
  DT_GI(dtimage_sum_cumulative_rev,(	/* shaded area A				    */
    ctl, &cooc, &A));

  if (rc == DT_RC_GOOD)
  {
    total = A.row[0].b32[0];			/* sum of coocurrences			    */
    busyness[0] = 0.0;
    for (t=1; t<n; t++)
	{
      busyness[t] = 1.0 -				/* all the pixels				    */
        (B.row[t-1].b32[t-1] +			/* except in shaded area B (Fig 1)	*/
         A.row[t].b32[t]) / total;		/* and in shaded area A			    */
	}
  }
  oprc = rc;

  if (B.xe)								/* free the images				    */
  DT_G(dtimage_free,(ctl, &B));
  if (A.xe)
  DT_G(dtimage_free,(ctl, &A));
  if (cooc.xe)
  DT_G(dtimage_free,(ctl, &cooc));

  return oprc == DT_RC_GOOD?
    rc: oprc;
}

/*..........................................................................*/

static char *F = "dt_wes78_cmd";

int
dt_wes78_cmd(
  int argc,
  char *argv[])
{
  dtimage_t input, plot;				/* image structures					*/
  dtparam_t param;						/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  double busyness[NGRAY];
  double pds[NGRAY];
  char *plot_name = NULL;
  dtos_time_t mark1, mark2;
  int n;
  int t;
  dt_rc_e rc;

  DT_G(dtparam_init,(&param));			/* init param structure             */
  DTPARAM_DEFAULT_P(&param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(&param, n, 15);
  DT_GI(dt_interactive_parse,(			/* go parse args					*/
    argc, argv, F, USAGE, &param, &n));

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc != 1)						/* check remaining arg(s)			*/
    rc = dt_interactive_usage(
      &param, USAGE);

  input.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get input image					*/
    &input, argv[0]));

  DT_GI(dtos_malloc2,(ctl,				/* space for plot image's name	    */
    (void **)&plot_name,
    strlen(argv[0])+
    strlen(BAR_SUFFIX)+1,
    F, "plot_name"));
  if (rc == DT_RC_GOOD)
  sprintf(plot_name, "%s%s",			/* name for plot image			    */
    argv[0], BAR_SUFFIX);
  plot.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for plot image			    */
    &plot, NGRAY, 128,
    DT_NTYPE_B08));
  DT_GI(dtimage_constant,(ctl,			/* clear plot image				    */
    &plot, 0.0));


  DT_GI(dt_wes78_all,(					/* calculate busyness array		    */
    &param, &input, busyness));
 

  if (plot_name)						/* output plotting is enabled?	    */
  {
    DT_GI(dtimage_plot_xbars_double,(	/* make bar plot					*/
      ctl, &plot, busyness, NGRAY,
      0, plot.ye, BAR_COLOR));
  }

  if (rc == DT_RC_GOOD)
  for (t=0; t<NGRAY; t++)				/* for all busyness values		    */
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,	/* print busyness				    */
      "%3d. %2.5lf",
      (int)t, busyness[t]);

  {
    long min, max;
    DT_GI(dt1d_mult_scalar_double,(ctl,	/* invert so valleys become peaks	*/
      busyness, NGRAY, -1.0));
    DT_GI(dt1d_range_double,(ctl,
      busyness, NGRAY, &min, &max));
    DT_GI(dt1d_add_scalar_double,(ctl,	/* make all values positive again	*/
      busyness, NGRAY, -busyness[min]));
  }


  DT_GI(dt_sez90_pds,(					/* calculate Sezan pds signal	    */
    &param, busyness, NGRAY, pds));

  if (plot_name)						/* output plotting is enabled?	    */
  {
    DT_GI(dtimage_plot_xcurve_double,(
      ctl, &plot, pds, NGRAY,
      0, plot.ye, PDS_COLOR));

    DT_GI(dt_sez90_plot_box,(&param,	/* plot boxes around peaks		    */
      busyness, NGRAY, pds,
      &plot, BOX_COLOR));

    DT_GI(dt_sez90_plot_max,(&param,	/* plot maximums of peaks		    */
      busyness, NGRAY, pds,
      &plot, MAX_COLOR));

    DT_GI(dtimage_export,(ctl,			/* output plot						*/
      &plot, plot_name,
      -1, -1));
  }

  if (plot.xe)
  DT_G(dtimage_free,(ctl,				/* free images						*/
    &plot));

  if (plot_name)
  DT_G(dtos_free2,(ctl, plot_name,
    F, "plot_name"));

  if (input.xe)
  DT_G(dtimage_free,(ctl,	
    &input));

  DT_Q(dtparam_free,(&param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
