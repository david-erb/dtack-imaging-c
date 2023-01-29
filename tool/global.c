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
| dt_lev85 command - Levine&Nazif 1985
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
#include <dtack/os.h>
#include <dt_lev85.h>

DT_RCSID("tool $RCSfile: global.c,v $ $Revision: 1.4 $");

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

#define USAGE "[params] image [output]"

#define BACKGROUND (0)					/* background of thresh image	    */
#define FOREGROUND (255)				/* foreground of thresh image	    */

#define COLOR0 2						/* red								*/
#define COLOR1 4						/* green						    */
#define COLOR2 7						/* white						    */
/*..........................................................................*/

dt_rc_e
dttool_global_uniformity(
  dtparam_t *param,
  dtimage_t *gray,
  dtimage_t *segmented,
  double thresh,
  double *u0,
  long *n0,
  double *u1,
  long *n1,
  double *u2,
  long *n2)
{
  DT_F("dttool_global_uniformity");
  dt_ctl_t *ctl = &param->ctl;
  dtos_time_t mark1, mark2;

  DTPARAM_VALUE(background,				/* use default if no background 	*/
    BACKGROUND);
  DTPARAM_VALUE(foreground,				/* use default if no foreground 	*/
    FOREGROUND);

  DT_Q(dtimage_thresh_scalar,(ctl,		/* threshold the image			    */
    gray, thresh,
    param->background,
    param->foreground,
    segmented));

  DT_Q(dtos_time,(ctl, &mark1));
  DT_Q(dt_lev85_3,(						/* compute uniformities				*/
    param, gray, segmented, 
	param->background,
    u0, n0, u1, n1, u2, n2));
  DT_Q(dtos_time,(ctl, &mark2));

  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "user %1.2f, system %1.2f, wall %1.2f seconds",
    mark2.user - mark1.user,
    mark2.system - mark1.system,
    mark2.wall - mark1.wall);

  return DT_RC_GOOD;
}
/*..........................................................................*/

DTTOOL_MAIN(dttool_global)
{
  dtimage_t gray, segmented, plot;		/* image structures					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int n;
  double u0, last_u0 = 0;
  double u1, last_u1 = 0;
  double u2, last_u2 = 0;
  double max_u0, max_u0_thresh;
  double max_u1, max_u1_thresh;
  double max_u2, max_u2_thresh;
  long n0;
  long n1;
  long n2;
  int i;
  char *output_thresh_name;
  char output_plot_name[64];
  double thresh, thresh0, dthresh;
  int first;
  dt_rc_e rc;

  rc = parse(param, argc, argv, &n);	/* parse parameters				   	*/
  if (rc != DT_RC_GOOD) return rc;

  argc -= n;
  argv += n;
  if (argc < 1 || argc > 2)				/* check remaining args				*/
    rc = dt_usage_say(
      param, USAGE);

  gray.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get gray image					*/
    &gray, argv[0]));

  segmented.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for thresholded output		*/
    &segmented, gray.xe, gray.ye,
    DT_NTYPE_B08));

  plot.xe = 0;
  if (argc >= 2)						/* going to be exporting images?	*/
  {
    output_thresh_name = argv[1];		/* name for segmented output	    */
    sprintf(output_plot_name,			/* name for plotted output		    */
      "%s.plot",				
      output_thresh_name);
    DT_GI(dtimage_create,(ctl,			/* space for plot				    */
      &plot, 256, 128,
      DT_NTYPE_B08));
	DT_GI(dtimage_constant,(ctl,		/* clear plot image				    */
      &plot, 0.0));
  }
  else
  {
    output_thresh_name = "";
    output_plot_name[0] = '\0';
  }

  first = 1;
  thresh0 = param->t;
  dthresh = param->dt;
  if (rc == DT_RC_GOOD)
  for (i=0; i<param->n; i++)
  {
    thresh = thresh0 + i * dthresh;

    if (thresh >						/* threshold beyond gray range?	    */
        dt_ntype_maxval[gray.ntype])
      break;

	DT_GI(dttool_global_uniformity,(
      param, &gray, &segmented,
      thresh, 
      &u0, &n0, &u1, &n1, &u2, &n2));

    if (strlen(output_thresh_name))
    DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
      &segmented, output_thresh_name,
      DTIMAGE_NOPOS, DTIMAGE_NOPOS));

    if (rc == DT_RC_GOOD)
      dt_say(ctl, F,					/* print uniformities			    */
      "%3d. %1.5lf (%5ld)  %1.5lf (%5ld)  %1.5lf (%5ld)",
        (int)thresh,
        u0, n0,
        u1, n1,
        u2, n2);
    if (strlen(output_plot_name) &&		/* output plotting is enabled?	    */
        !first)
	{
#define Y(U) (plot.ye-1-(plot.ye-2)*U)	/* convert uniformity to plot y	    */
	  DT_GI(dtimage_plot_line,(ctl,
        &plot,
        thresh-dthresh, Y(last_u0),
        thresh, Y(u0), COLOR0));
	  DT_GI(dtimage_plot_line,(ctl,
        &plot,
        thresh-dthresh, Y(last_u1),
        thresh, Y(u1), COLOR1));
	  DT_GI(dtimage_plot_line,(ctl,
        &plot,
        thresh-dthresh, Y(last_u2),
        thresh, Y(u2), COLOR2));
      DT_GI(dtimage_export,(ctl,		/* output plot thus far				*/
        &plot, output_plot_name,
        DTIMAGE_NOPOS, DTIMAGE_NOPOS));
	}
    if (first)
	{
	  max_u0 = u0;
	  max_u1 = u1;
	  max_u2 = u2;
	  max_u0_thresh = thresh;
	  max_u1_thresh = thresh;
	  max_u2_thresh = thresh;
	  first = 0;
	}
	else
	{
	  if (u0 > max_u0)					/* remember absolute curve max's	*/
	  {
		max_u0 = u0;
		max_u0_thresh = thresh;
	  }
	  if (u1 > max_u1)
	  {
		max_u1 = u1;
		max_u1_thresh = thresh;
	  }
	  if (u2 > max_u2)
	  {
		max_u2 = u2;
		max_u2_thresh = thresh;
	  }
	}
    last_u0 = u0;
    last_u1 = u1;
    last_u2 = u2;
  }

  if (rc == DT_RC_GOOD)
  {
	dt_say(ctl, F, "max background uniformity %1.5f at thresh %3.2f",
      max_u0, max_u0_thresh);
	dt_say(ctl, F, "max foreground uniformity %1.5f at thresh %3.2f",
      max_u1, max_u1_thresh);
	dt_say(ctl, F, "max combined   uniformity %1.5f at thresh %3.2f",
      max_u2, max_u2_thresh);
  }

  if (plot.xe)
  DT_G(dtimage_free,(ctl,				/* free images						*/
    &plot));

  if (segmented.xe)
  DT_G(dtimage_free,(ctl,				/* free images						*/
    &segmented));

  if (gray.xe)
  DT_G(dtimage_free,(ctl,	
    &gray));

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(t, 0);				/* first step					    */
  DTPARAM_DEFAULT(dt, 5);				/* delta in each step			    */
  DTPARAM_DEFAULT(n, 1000);				/* number of steps to do		    */
  DTPARAM_DEFAULT(dbg, 0);

  rc = dt_enter_param	(argc, argv,	/* go parse args					*/
    F, USAGE, param, n);

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
