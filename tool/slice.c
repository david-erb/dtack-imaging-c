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
| dttool_slice utility - slice gray level range
| 
| DESCRIPTION
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: slice.c,v $ $Revision: 1.9 $");

#define USAGE "[params] source dest"

typedef struct {
  int dummy;
} app_t;

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttool_slice_arg_check(					/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_stats_arg_check");
  app_t *a = (app_t *)app;

  DT_Q(dttool_parse_arg_string,(param,	/* we have a source image? 			*/
    arg, 0, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_arg_string,(param,	/* we have a destination image? 	*/
    arg, 1, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_slice)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dtimage_t input;	
  double thresh[2];
  double values[3];
  dt_rc_e rc = DT_RC_GOOD;
  
  input.xe = 0;

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->min, NULL,            NULL, "128",
    &param->max, NULL,            NULL, "256",
    &param->background, NULL,            NULL, "0",
    &param->foreground, NULL,            NULL, "255",
    &param->x,          NULL,            NULL, "-1",
    &param->y,          NULL,            NULL, "-1",
    NULL));

  DT_C(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_C(dt_enter_parse,(					/* parse command line				*/
    argc, argv, F, USAGE, param,
    arg, dttool_slice_arg_check, &a));

  DT_C(dtimage_import,(ctl,				/* get image from file or window	*/
    &input, input_name));

  values[0] = param->background;
  thresh[0] = param->min;
  values[1] = param->foreground;
  thresh[1] = param->max;
  values[2] = param->background;
  
  DT_GI(dtimage_thresh_multi,(ctl,		/* threshold image in place			*/
    &input, thresh, 2, values,
    &input));

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &input, output_name, 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));
										/* ................................ */

cleanup:
  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

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
