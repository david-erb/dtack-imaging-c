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
| dttool_copy - copy image
| 
|.SYNOPSIS
|:dttool copy [parameters] input output
| 
|.SUMMARY
| Copy ~input~ image to ~output~ image.
| 
|.ARGUMENTS
| ~input~	name of the input image file or window
| ~output~	name of the output image file or window
|
|.PARAMETERS
| :-w:			width of output (default size of input)
| :-h:			height of output (default size of input)
| :-dx:			input start column (default 0)
| :-dy:			input start row (default 0)
|dtinclude(parampr.dx)
|dtinclude(paramxy.dx)
|dtinclude(params1.dx)
|  
|.RETURN VALUES
|dtinclude(return2.dx)
| 
|.ERRORS
| - ~input~ or ~output~ image not found
| -	invalid parameter value
|
|dtinclude(params2.dx)
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: copy.c,v $ $Revision: 1.9 $");

#include <copy.use>

typedef struct {
  dt_ntype_e ntype;
} app_t;

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_copy_arg_check(					/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_copy_arg_check");
  app_t *a = (app_t *)app;

  DT_Q(dttool_parse_arg_string,(param,	/* we have a source image? 			*/
    arg, 0, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  DT_Q(dttool_parse_arg_string,(param,	/* we have a output image? 			*/
    arg, 1, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

  if (param->precision != NULL)			/* precision specified? 			*/
  {
    DT_Q(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &a->ntype,
      msg, DT_ENTER_MSG_MAXLEN));
	if (msg[0] != '\0')					/* bad precision? 					*/
	  *start = param->precision;
  }
  else
    a->ntype = DT_NTYPE_UNKNOWN;		/* default precision 				*/
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_copy)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dt_rc_e rc = DT_RC_GOOD;
  
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->w,          "Output width",  NULL, NULL,
    &param->h,          "Output height", NULL, NULL,
    &param->dx,         "Start x",       NULL, NULL,
    &param->dy,         "Start y",       NULL, NULL,
    &param->precision,  NULL,            NULL, "8bit",
    &param->x,          NULL,            NULL, "-1",
    &param->y,          NULL,            NULL, "-1",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttool_copy_arg_check, &a));

										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
    dtimage_t input;					/* image structure					*/
    dtimage_t cropped;
	dtxy_t xe, ye;
    input.xe = 0;
    cropped.xe = 0;						/* in case of error				    */

    DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
      &input, input_name));

    if (rc == DT_RC_GOOD)
    if (param->dx < 0 ||
        param->dy < 0 ||
        param->dx >= input.xe ||
        param->dy >= input.ye)
    {
      rc = dt_err(ctl, F,
        "copy start point (%ld,%ld)"
        " is outside image (%ld,%ld)",
        (long)param->dx, 
        (long)param->dy,
        input.xe, input.ye);
    }

    xe = (dtxy_t)
      DTPARAM_GET_VALUE(w,				/* width not specified?			    */
        input.xe - param->dx);			/* use all we can				    */

    ye = (dtxy_t)
      DTPARAM_GET_VALUE(h,				/* height not specified?		    */
        input.ye - param->dy);			/* use all we can				    */

    DT_GI(dtimage_overlay,(ctl,			/* crop input to intermediate image	*/
      &input, &cropped,
      (dtxy_t)param->dx,
      (dtxy_t)param->dy, xe, ye));

    if (a.ntype == DT_NTYPE_UNKNOWN)	/* no precision given?				*/
	  a.ntype = input.ntype;

    if (a.ntype == input.ntype)			/* no precision change?			    */
    {
      DT_GI(dtimage_export,(ctl,		/* put image into file or window	*/
        &cropped, output_name,
        DT_CAST(dtxy_t,					/* image exported x position 		*/
          DTPARAM_GET_VALUE(x,			/* from -x arg 						*/
            DTIMAGE_NOPOS)),			/* or use window system default 	*/
        DT_CAST(dtxy_t,					/* image exported y position 		*/
          DTPARAM_GET_VALUE(y,			/* from -y arg 						*/
            DTIMAGE_NOPOS))));			/* or use window system default 	*/
    } 
    else								/* precision change specified?	    */
    {
      dtimage_t output;
	  output.xe = 0;
	  DT_GI(dtimage_create,(ctl,		/* make space for output image	    */
        &output, cropped.xe, 
        cropped.ye, a.ntype));
	  DT_GI(dtimage_cast,(ctl,			/* change precision				    */
        &cropped, &output));
	  DT_GI(dtimage_export,(ctl,		/* put image into file or window	*/
        &output, output_name,
        DT_CAST(dtxy_t,					/* image exported x position 		*/
          DTPARAM_GET_VALUE(x,			/* from -x arg 						*/
            DTIMAGE_NOPOS)),			/* or use window system default 	*/
        DT_CAST(dtxy_t,					/* image exported y position 		*/
          DTPARAM_GET_VALUE(y,			/* from -y arg 						*/
            DTIMAGE_NOPOS))));			/* or use window system default 	*/

      if (output.xe != 0)				/* we actually allocated it?	    */
        DT_I(dtimage_free,(ctl,			/* free output image				*/
            &output));
    }

    if (cropped.xe != 0)				/* we actually allocated it?	    */
      DT_I(dtimage_free,(ctl,			/* free cropped image				*/
        &cropped));

    if (input.xe != 0)					/* we actually allocated it?	    */
      DT_I(dtimage_free,(ctl,			/* free input image					*/
        &input));
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dttool_copy_arg_check, NULL, rc));
  } 
										/* ................................ */

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
