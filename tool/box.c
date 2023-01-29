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
| USAGE
| dttool_box - draw box on image
| 
|.SYNOPSIS
|:dttool box [parameters] input xmin ymin xmax ymax output
| 
|.SUMMARY
| Draw rectangular box on ~input~ image and copy to ~output~.
| 
|.ARGUMENTS
| ~input~	name of the input image file or window
| ~xmin~	left
| ~ymin~	top
| ~xmax~	right
| ~ymax~	bottom
| ~output~	name of the output image file or window,
| 			may be the same image as ~input~
|
|.PARAMETERS
|dtinclude(paramcl.dx)
|dtinclude(paramxy.dx)
|dtinclude(params1.dx)
|  
|.RETURN VALUES
|dtinclude(return2.dx)
| 
|.ERRORS
| - ~input~ or ~output~ image not found
| -	invalid box position
| -	invalid parameter value
|
|dtinclude(params2.dx)
| 
| END
 *..........................................................................*/

#include <dttool.h>
DT_RCSID("tool $RCSfile: box.c,v $ $Revision: 1.5 $");
#include <dtack/lut8.h>
#include <dtack/mem.h>

#include <box.use>

typedef struct {
  double color;
  long xmin;
  long ymin;
  long xmax;
  long ymax;
} app_t;

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttool_box_cmd_check(					/* check params for this program	*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_box_cmd_check");
  app_t *a = (app_t *)app;
						
  DT_Q(dttool_parse_arg_string,(param,	/* import image						*/
    arg, 0, NULL,
    msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_arg_long,(param,
    arg, 1, NULL,
    &a->xmin, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_arg_long,(param,
    arg, 2, NULL,
    &a->ymin, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_arg_long,(param,
    arg, 3, NULL,
    &a->xmax, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_arg_long,(param,
    arg, 4, NULL,
    &a->ymax, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;
  
  DT_Q(dttool_parse_arg_string,(param,	/* export image						*/
    arg, 5, NULL,
    msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_param_color,(param,	/* drawing color					*/
    "white", &a->color, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
cmd_box(
  dtparam_t *param,
  const char *input_name,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double color,
  const char *output_name)
{
  DT_F("dttool_cmd_box");
  dtimage_t image;						/* image structure					*/
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  image.xe = 0;
  DT_C(dtimage_import,(ctl,				/* get image from file or window	*/
    &image, input_name));

  DT_Q(dtimage_plot_line,(ctl, &image, xmin, ymin, xmax, ymin, color));
  DT_Q(dtimage_plot_line,(ctl, &image, xmax, ymin, xmax, ymax, color));
  DT_Q(dtimage_plot_line,(ctl, &image, xmax, ymax, xmin, ymax, color));
  DT_Q(dtimage_plot_line,(ctl, &image, xmin, ymax, xmin, ymin, color));

  DT_C(dtimage_export,(ctl,				/* output image						*/
    &image, output_name, 
    DT_CAST(dtxy_t,						/* image exported x position 		*/
      DTPARAM_GET_VALUE(x,				/* from -x arg 						*/
        DTIMAGE_NOPOS)),				/* or use window system default 	*/
    DT_CAST(dtxy_t,						/* image exported y position 		*/
      DTPARAM_GET_VALUE(y,				/* from -y arg 						*/
        DTIMAGE_NOPOS))));				/* or use window system default 	*/

cleanup:
  if (image.xe != 0)
    DT_I(dtimage_free,(ctl,				/* free image						*/
      &image));

  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_box)
{
  dt_ctl_t *ctl = &param->ctl;
  char input_name[DTTOOL_NAME_MAXLEN];
  char xmin_string[DTTOOL_NUMBER_MAXLEN];
  char ymin_string[DTTOOL_NUMBER_MAXLEN];
  char xmax_string[DTTOOL_NUMBER_MAXLEN];
  char ymax_string[DTTOOL_NUMBER_MAXLEN];
  char output_name[DTTOOL_NAME_MAXLEN];
  dt_enter_arg_t arg[7];
  app_t app;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,				/* set up keywords 					*/
    DTPARAM_FLAG_PRIMARY,
    &param->color, NULL, NULL, "white",
    NULL));
					 
  DT_GI(dtparam_set,(param,				/* set up keywords 					*/
    DTPARAM_FLAG_SECONDARY,
    &param->x,          NULL,            NULL, "-1",
    &param->y,          NULL,            NULL, "-1",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Left (xmin)", NULL, 
      xmin_string, sizeof(xmin_string),
    "Top (ymin)", NULL, 
      ymin_string, sizeof(ymin_string),
    "Right (xmax)", NULL, 
      xmax_string, sizeof(xmax_string),
    "Bottom (ymax)", NULL, 
      ymax_string, sizeof(ymax_string),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttool_box_cmd_check, &app));

										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
	DT_G(cmd_box,(param,
      input_name,
      DT_CAST(long, app.xmin), 
      DT_CAST(long, app.ymin), 
      DT_CAST(long, app.xmax), 
      DT_CAST(long, app.ymax),
      app.color,
      output_name));
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dttool_box_cmd_check, &app, rc));
  } 

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

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
