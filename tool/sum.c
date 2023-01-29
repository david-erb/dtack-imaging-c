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
| dttool_sum utility - make sum of row or column
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
#include <dtack/mem.h>

DT_RCSID("tool $RCSfile: sum.c,v $ $Revision: 1.6 $");

#define USAGE "[params] source h|v dest"

#define FLAG_PLOT (0x01)
#define FLAG_H    (0x02)
#define FLAG_V    (0x04)

/*..........................................................................*/
static dt_rc_e dttool_sum_parse(		/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  DT_F("dttool_sum_parse");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(h, 64);				/* height of sum display	    */
  DTPARAM_DEFAULT(w, 64);				/* height of sum display	    */
  DTPARAM_DEFAULT(foreground, 255);		/* color for drawing bars		    */
  DTPARAM_DEFAULT(background, 0);
  DTPARAM_DEFAULT(dbg, 0);
  DTPARAM_DEFAULT(precision, "same");
  DTPARAM_DEFAULT(op, "h");

  DT_Q(dttool_parse,(param, F, USAGE,		/* go parse args					*/
    argc, argv, n));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e dttool__sum(						/* make sum plot					*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned long flags,
  dtxy_t  barsize,
  dtimage_t *plot,
  dt_ntype_e ntype,
  double foreground,
  double background)
{
  DT_F("dttool__sum");
  double *sum;							/* sum array						*/
  double smin;							/* min data on plot				    */
  double smax;							/* max data on plot				    */
  int i, j, n;
  const char *temp = "sum array";
  dt_rc_e rc;

  sum = NULL;
  plot->xe = 0;

  if (flags & FLAG_H)					/* sum along columns? 				*/
  {
    n = image->xe;						/* number of bars 					*/
	if (flags & FLAG_PLOT)				/* will be making plot? 			*/
      DT_C(dtimage_create,(ctl,			/* create output plot image		    */
        plot, n, barsize, ntype))
    DT_C(dtos_malloc2,(ctl,				/* space for sum					*/
      (void **)&sum,
      n * sizeof(*sum), F, temp));
    DT_C(dtimage_sum_cols,(ctl,			/* make sum array 					*/
      image, sum, n));
  }
  else									/* sum down rows?					*/
  {
    n = image->ye;
	if (flags & FLAG_PLOT)
      DT_C(dtimage_create,(ctl,			/* create output plot image		    */
        plot, barsize, n, ntype));
    DT_C(dtos_malloc2,(ctl,				/* space for sum					*/
      (void **)&sum,
	  n * sizeof(*sum), F, temp));
    DT_C(dtimage_sum_rows,(ctl,			/* make sum array 					*/
      image, sum, n));
  }
  
  if (plot->xe == 0)					/* not a plot operation? 			*/
  {
    DT_C(dtimage_create,(ctl,			/* create output histogram as		*/
      plot, 1, n, DT_NTYPE_F2));		/* special single-column image 		*/
	for (i=0; i<n; i++)					/* copy sum array to hist image 	*/
  	  DTIMAGE_SETPIX1(DT_Q, ctl,
        plot, 0, i, sum[i]);
	goto cleanup;
  }

  smin = sum[0];
  smax = sum[0];

  for (i=1; i<n; i++)					/* find plot limits					*/
  {
    smin = DT_MIN(sum[i], smin);
    smax = DT_MAX(sum[i], smax);
  }

  for (i=0; i<n; i++)					/* scale data for window		    */
    sum[i] = (barsize-1) *			
      (sum[i]-smin) / (smax-smin);

  DT_C(dtimage_constant,(ctl,			/* clear sum plot					*/
    plot, background));

  for (i=0; i<n; i++)					/* for each bar					    */
  {
    if (flags && FLAG_H)
	{
      int bar = barsize-1-(int)sum[i];	/* bar height						*/
	  for (j=barsize-1; j>=bar; j--)
  	  {
  	    DT_C(dtimage_set_pixel,(ctl,
          plot, i, j, foreground))
	  }
	}
    else
	{
      int bar = (int)sum[i];			/* bar width						*/
	  for (j=0; j<=bar; j++)
  	  {
  	    DT_C(dtimage_set_pixel,(ctl,
          plot, j, i, foreground))
	  }
	}
  }

cleanup:
  if (sum != NULL)
    DT_I(dtos_free2,(ctl, sum,			/* free sum							*/
      F, temp));

  if (rc != DT_RC_GOOD &&				/* we failed?					    */
      plot->xe != 0)					/* but an image got allocated?	    */
    DT_I(dtimage_free,(ctl, plot));

  return rc;
}




/*..........................................................................*/

DTTOOL_MAIN(dttool_sum)
{
  dtimage_t image;						/* image structure					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int n;
  char *arg_input;
  char *arg_flags;
  char *arg_output;
  dtimage_t plot;
  dt_ntype_e ntype;
  unsigned long flags;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dttool_sum_parse,(param,				/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check args						*/
    DT_GI(dttool_usage,(param, USAGE));

  arg_input = argv[0];					/* get positionals				    */
  arg_flags = argv[1];
  arg_output = argv[2];

  if (!strcmp(arg_flags, "h"))			/* check direction validity		    */
    flags = FLAG_H | FLAG_PLOT;
  else
  if (!strcmp(arg_flags, "hH"))
    flags = FLAG_H;
  else
  if (!strcmp(arg_flags, "v"))
    flags = FLAG_V | FLAG_PLOT;
  else
  if (!strcmp(arg_flags, "vH"))
    flags = FLAG_V;
  else
    DT_GI(dttool_usage,(param, USAGE));

  image.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
    &image, arg_input));

  if (!strcmp(param->precision, "same"))
  {
	char message[80];
    DT_GI(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, sizeof(message)));
    if (message[0])
      rc = dt_err(ctl, F, "%s", message);
  }
  else
    ntype = image.ntype;

  plot.xe = 0;
  DT_GI(dttool__sum,(ctl,				/* make sum image of image   		*/
    &image, flags,
    (dtxy_t)(flags&FLAG_H?param->h:param->w),
    &plot, ntype,
    param->foreground,
    param->background));

  DT_GI(dtimage_export,(ctl,			/* output sum plot					*/
    &plot, arg_output,
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (plot.xe != 0)						/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free sum plot					*/
    &plot));

  if (image.xe != 0)					/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free image						*/
    &image));

  DT_Q(dtparam_free,(param));			/* free param space					*/

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
