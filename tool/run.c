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
| dttool_run utility - make run of row or column
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

DT_RCSID("tool $RCSfile: run.c,v $ $Revision: 1.6 $");

#define USAGE "[params] source h|v dest"

static dt_rc_e static_parse(			/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

dt_rc_e dttool__run(						/* make run plot				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  int dir,
  dtxy_t barsize,
  dtimage_t *plot,
  dt_ntype_e ntype,
  double foreground,
  double background);

/*..........................................................................*/

DTTOOL_MAIN(dttool_run)
{
  dtimage_t image;						/* image structure					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int n;
  char *arg_input;
  char *arg_dir;
  char *arg_output;
  dtimage_t plot;
  dt_ntype_e ntype;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(static_parse,(param,			/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check args						*/
    DT_GI(dttool_usage,(param, USAGE));

  arg_input = argv[0];					/* get positionals				    */
  arg_dir = argv[1];
  arg_output = argv[2];

  if (strcmp(arg_dir, "h") &&			/* check direction validity		    */
      strcmp(arg_dir, "v"))
    DT_GI(dttool_usage,(param, USAGE));

  image.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
    &image, arg_input));

  {
	char message[80];
    DT_GI(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, sizeof(message)));
    if (message[0])
      rc = dt_err(ctl, F, "%s", message);
  }
  if (ntype == DT_NTYPE_UNKNOWN)
    ntype = image.ntype;

  plot.xe = 0;
  DT_GI(dttool__run,(ctl,				/* make run image of image   		*/
    &image, arg_dir[0],
    (dtxy_t)(arg_dir[0]=='h'?param->h:param->w),
    &plot, ntype,
    param->foreground,
    param->background));

  DT_GI(dtimage_export,(ctl,			/* output run plot					*/
    &plot, arg_output, DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (plot.xe != 0)						/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free run plot					*/
    &plot));

  if (image.xe != 0)					/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free image						*/
    &image));

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/
static dt_rc_e static_parse(			/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(h, 64);				/* height of run display		    */
  DTPARAM_DEFAULT(w, 64);				/* height of run display		    */
  DTPARAM_DEFAULT(foreground, 255);		/* color for drawing bars		    */
  DTPARAM_DEFAULT(background, 0);
  DTPARAM_DEFAULT(dbg, 0);
  DTPARAM_DEFAULT(precision, "same");

  DT_Q(dttool_parse,(param, F, USAGE,		/* go parse args					*/
    argc, argv, n));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e dttool__run(					/* make run plot					*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  int dir,
  dtxy_t barsize,
  dtimage_t *plot,
  dt_ntype_e ntype,
  double foreground,
  double background)
{
  dtrun_len_t *run;						/* run array						*/
  dtntype_s4_t *runlong;
  unsigned int i, n;
  const char *temp = "run array";
  const char *templong = "run array (long)";
  dt_rc_e rc = DT_RC_GOOD;

#define H (dir == 'h')

  n = H? image->xe: image->ye;			/* number of bars				    */

  run = NULL;
  DT_GI(dtos_malloc2,(ctl,				/* space for run					*/
    (void **)&run,
    n * sizeof(*run), F, temp));

  runlong = NULL;
  DT_GI(dtos_malloc2,(ctl,				/* space for run					*/
    (void **)&runlong,
    n * sizeof(*runlong), F, templong));


  plot->xe = 0;
  if (H)								/* get space for run plot			*/
  {
    n = image->xe;
    DT_GI(dtimage_create,(ctl,			/* create output plot image		    */
      plot, n, barsize, ntype));
    DT_GI(dtrun_cols_max,(ctl,			/* find maximum run on each col	    */
      image, run, n));
    if (rc == DT_RC_GOOD)
	  for (i=0; i<n; i++)
        runlong[i] = run[i];
	DT_Q(dtimage_constant,(ctl, plot,	/* clear plot 						*/
      background));
    DT_GI(dtimage_plot_xbars_sint32,(ctl,	/* plot runs as bars			    */
      plot, runlong, n,
      0, barsize, foreground));
  }
  else
  {
    n = image->ye;
    DT_GI(dtimage_create,(ctl,			/* create output plot image		    */
      plot, barsize, n, ntype));
    DT_GI(dtrun_rows_max,(ctl,			/* find maximum runs on each row    */
      image, run, n));
    if (rc == DT_RC_GOOD)
	  for (i=0; i<n; i++)
        runlong[i] = run[i];
	DT_Q(dtimage_constant,(ctl, plot,	/* clear plot 						*/
      background));
    DT_GI(dtimage_plot_ybars_sint32,(ctl,	/* plot runs as bars			    */
      plot, runlong, n,
      0, barsize, foreground));
  }

  if (runlong != NULL)
    DT_Q(dtos_free2,(ctl, runlong,		/* free run							*/
      F, templong));
  if (run != NULL)
    DT_Q(dtos_free2,(ctl, run,			/* free run							*/
      F, temp));

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
