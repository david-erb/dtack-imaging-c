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
| dttool fft - fft transform
| 
|.SYNOPSIS
|:dttool fft [parameters] input output
| 
|.SUMMARY
| FFT transform ~input~ image.
| ~Output~ image will contain the complex magnitude of the result.
| 
|.ARGUMENTS
| ~input~	name of the image to be fft
| ~output~	name of output image to be created
|
|.PARAMETERS
| :-precision:	desired precision of ~output~
|dtinclude(paramxy.dx)
|dtinclude(params1.dx)
| 
|.DESCRIPTION
| The ~input~ image is fft filtered under a window size of
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
DT_RCSID("tool $RCSfile: fft.c,v $ $Revision: 1.6 $");
#include <dtack/os.h>

#define USAGE "fft input output"

typedef struct {
  dt_ntype_e ntype;
} app_t;

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_fft_arg_check(				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_fft_arg_check");
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

DTTOOL_MAIN(dttool_fft)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dtimage_t input;						/* image structure					*/
  dtimage_t condition;
  dtimage_t output;
  dtxy_t x, y;
  dt_rc_e rc;
  
  input.xe = 0;
  condition.xe = 0;
  output.xe = 0;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, NULL,
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
    arg, dttool_fft_arg_check, (void *)&a));

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));

  DT_C(dtimage_create,(ctl,				/* create upcast conditioned image 	*/
    &condition,
    input.xe, input.ye, DTNTYPE_F2));

  DT_C(dtimage_cast,(ctl, &input,		/* copy input to conditioned image 	*/
    &condition));
  
  for (y=0; y<condition.ye; y+=2)		/* condition fft to be centered		*/
  {
	dtntype_f2_t *p;
	p = condition.row[y].f2;
    for (x=1; x<condition.xe; x+=2)		/* even rows odd pixels 			*/
	  p[x] = -p[x];
	p = condition.row[y+1].f2;
    for (x=0; x<condition.xe; x+=2)		/* odd rows even pixels 			*/
	  p[x] = -p[x];
  }
	  
  output.xe = 0;
  if (a.ntype != DT_NTYPE_UNKNOWN)		/* precision given?					*/
    DT_C(dtimage_create,(ctl,			/* space for output image		    */
      &output, input.xe, input.ye,
      a.ntype));

  DT_C(dtimage_fft_magnitude,(ctl,
    &condition, &output));

  DT_C(dtimage_export,(ctl,				/* output resulting image			*/
    &output, output_name, 
    DT_CAST(dtxy_t,						/* image exported x position 		*/
      DTPARAM_GET_VALUE(x,				/* from -x arg 						*/
        DTIMAGE_NOPOS)),				/* or use window system default 	*/
    DT_CAST(dtxy_t,						/* image eyported y position 		*/
      DTPARAM_GET_VALUE(y,				/* from -y arg 						*/
        DTIMAGE_NOPOS))));				/* or use window system default 	*/

cleanup:
	   
  if (output.xe)						/* output image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free output image				*/
      &output));

  if (condition.xe)						/* condition image got allocated?   */
    DT_I(dtimage_free,(ctl,				/* free condition image				*/
      &condition));

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
