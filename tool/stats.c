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
| dttoolor utility -
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

DT_RCSID("tool $RCSfile: stats.c,v $ $Revision: 1.4 $");

#define USAGE "[params] image"
#define OP_DEFAULT ""

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_stats_arg_check(					/* check args and params            */
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_stats)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 48
  char image_name[ARG_MAXLEN];
  dt_enter_arg_t arg[2];
  dtimage_t image;						/* image structure					*/
  double sum;
  double variance;
  double mean;
  double imin, imax;
  dt_rc_e rc = DT_RC_GOOD;
  
  image.xe = 0;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->op, NULL, NULL, OP_DEFAULT,
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      image_name, sizeof(image_name),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_stats_arg_check, NULL));


										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
	DT_GI(dtimage_import,(ctl,			/* get input image					*/
      &image, image_name));

    DT_GI(dtimage_sum,(ctl,
      &image, &sum));

#if 0
    DT_GI(dtimage_range,(ctl,
      &image, &imin, &imax));
 
    DT_GI(dtimage_variance,(ctl,
      &image, &variance, &mean));
#endif

	if (rc == DT_RC_GOOD)
	{
      dt_say(ctl, F, "width         %u", image.xe);
      dt_say(ctl, F, "height        %u", image.ye);
      dt_say(ctl, F, "type          %s", dt_ntype_string[image.ntype]);
      dt_say(ctl, F, "size of pixel %u bits", 
        DT_CAST(unsigned int, 8.0 * dt_ntype_sizeof[image.ntype]));
	  dt_say(ctl, F, "size of row   %u bytes", image.rowsize);
	  dt_say(ctl, F, "min           %g", imin);
	  dt_say(ctl, F, "max           %g", imax);
	  dt_say(ctl, F, "sum           %g", sum);
	  //	  dt_say(ctl, F, "area          %g", area);
	  dt_say(ctl, F, "mean          %g", mean);
	  dt_say(ctl, F, "variance      %g", variance);
	  dt_say(ctl, F, "std deviation %g",
        variance > 0.0? sqrt(variance): 0.0);
	}
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, USAGE, arg,
      dttool_stats_arg_check, NULL, rc));
  } 

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  if (image.xe)							/* image got allocated?	    		*/
    DT_I(dtimage_free,(ctl, &image));	/* free image						*/

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
