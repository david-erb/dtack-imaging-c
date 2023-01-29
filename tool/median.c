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
| dttool median - median filter
| 
|.SYNOPSIS
|:dttool median [parameters] input output
| 
|.SUMMARY
| Median filter ~input~ image to produce ~output~ image.
| ~Output~ image is created to be smaller than ~input~ by size of
| window less one so no undefined border area is present.
| 
|.ARGUMENTS
| ~input~	name of the image to be mediand
| ~output~	name of output image to be created
|
|.PARAMETERS
| :-w:			width of window
| :-h:			height of window
| :-precision:	desired precision of ~output~
|dtinclude(paramxy.dx)
|dtinclude(params1.dx)
| 
|.DESCRIPTION
| The ~input~ image is median filtered under a window size of
| ~param->w~ and ~param->h~.
|  
|.RETURN VALUES
|dtinclude(return2.dx)
| 
|.ERRORS
| - ~input~ not found
| - ~output~ not a valid file or window name
| - not enough memory to hold ~input~, ~output~,
|	and convolution work buffers at the same time
|
|dtinclude(params2.dx)
| 
| END
 *..........................................................................*/

#include <dttool.h>
DT_RCSID("tool $RCSfile: median.c,v $ $Revision: 1.6 $");
#include <dtack/os.h>

#define USAGE "median input output"

typedef struct {
  dt_ntype_e ntype;
} app_t;

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_median_arg_check(				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_median_arg_check");
  app_t *a = (app_t *)app;

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

DTTOOL_MAIN(dttool_median)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dtimage_t input;						/* image structure					*/
  dtimage_t output;
  dtimage_t cut;
  dt_rc_e rc;
  
  input.xe = 0;
  output.xe = 0;
  cut.xe = 0;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, NULL,
    &param->w,  NULL, NULL, "3",
    &param->h,  NULL, NULL, "3",
    &param->x,  NULL, NULL, NULL,
    &param->y,  NULL, NULL, NULL,
    NULL));
 
  dt_dbg_trace(ctl, F,      
    DT_RCSID_STRING, __LINE__, 
    "we heah");

  DT_Q(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_Q(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_median_arg_check, (void *)&a));

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));

  if (a.ntype == DT_NTYPE_UNKNOWN)		/* no precision given?				*/
	a.ntype = input.ntype;

  DT_C(dtimage_create,(ctl,				/* space for output image		    */
    &output, input.xe, input.ye,
    a.ntype));

  {

	dtxy_t kx = DT_CAST(dtxy_t, param->w);
	dtxy_t ky = DT_CAST(dtxy_t, param->h);

    DT_C(dtimage_median,(ctl,			/* median input to output			*/
      &input, kx, ky, &output))
						
    DT_C(dtimage_overlay,(ctl,			/* chop out the dead pixels 		*/
      &output, &cut,
      kx / 2,
      ky / 2,
      output.xe-(kx-1),
      output.ye-(ky-1)));
  }
	   
  DT_C(dtimage_export,(ctl,				/* output resulting image			*/
    &cut, output_name, 
    DT_CAST(dtxy_t,						/* image exported x position 		*/
      DTPARAM_GET_VALUE(x,				/* from -x arg 						*/
        DTIMAGE_NOPOS)),				/* or use window system default 	*/
    DT_CAST(dtxy_t,						/* image eyported y position 		*/
      DTPARAM_GET_VALUE(y,				/* from -y arg 						*/
        DTIMAGE_NOPOS))));				/* or use window system default 	*/

cleanup:
  if (cut.xe)							/* cut image got allocated?	    	*/
    DT_I(dtimage_free,(ctl,				/* free cut image					*/
      &cut));
	   
  if (output.xe)						/* output image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free output image				*/
      &output));

  if (input.xe)							/* input image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free input image					*/
      &input));

  DT_I(dt_leave,(param));				/* clean up before leaving          */

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
