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
| dttool convolve - convolve image with kernel
| 
|.SYNOPSIS
|:dttool convolve [parameters] input kernel output
| 
|.SUMMARY
| Convolve ~input~ image with ~kernel~ to produce ~output~ image.
| 
|.ARGUMENTS
| ~input~	name of the image to be convolved
| ~kernel~	name of a predefined kernel, or name of a file
| ~output~	name of output image to be created
|
|.PARAMETERS
| :-w:			kernel width if ~kernel~ is not a file (default 3)
| :-h:			kernel height if ~kernel~ is not a file (default 3)
|dtinclude(parampr.dx)
|dtinclude(paramxy.dx)
|dtinclude(params1.dx)
| 
|.DESCRIPTION
| ~Output~ image is created to be smaller than ~input~ by size of
| ~kernel~ less one so no undefined border area is present.
| 
| The name ~kernel~ is first searched for in a list of predefined kernel names.
| If found, the kernel is created with the size given by
| ~param->w~ and ~param->h~ and precision either
| :DT_NTYPE_F2: or :DT_NTYPE_S4: (double or long),
| depending on ~param->precision~ if given, or ~input->ntype~.
| if ~kernel~ is not a predefined name,
| then it is assumed to be the name of a file containing
| an image specifying the kernel.
|
|dtinclude(kernels.dx)
|  
|.RETURN VALUES
|dtinclude(return2.dx)
| 
|.ERRORS
| -	~input~ not found
| -	~kernel~ not a predefined name and not a filename
| -	~output~ not a valid file or window name
| -	not enough memory to hold ~input~, ~output~,
|	and convolution work buffers at the same time
|
|dtinclude(params2.dx)
| 
| END
 *..........................................................................*/

#include <dttool.h>
DT_RCSID("tool $RCSfile: convolve.c,v $ $Revision: 1.6 $");
#include <dtack/os.h>

#include <convolve.use>

typedef struct {
  dt_ntype_e ntype;
  int ktype;
} app_t;

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttool_convolve_arg_check(				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_convolve_arg_check");
  app_t *a = (app_t *)app;
#define KERNEL arg[1]
										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a source image? 			*/
    arg, 0, 
    NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a kernel name? 			*/
    arg, 1, 
    NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;
  
  DT_Q(dtstr_choice,(ctl,				/* check kernel name  				*/
    dtimage_gen_kernel_names,			/* against list of predefined names */
    KERNEL.val,	&a->ktype));

  if (a->ktype < 0)						/* not a predefined name? 			*/
  {
	int isreadable;
	DT_Q(dtos_isreadable,(ctl,			/* see if it is a file 				*/
      KERNEL.val, &isreadable));
	if (isreadable)
      a->ktype = -1;
    else
      a->ktype = -2;
  }

  if (a->ktype < -1)					/* did not resolve a kernel name? 	*/
  {
    dtstr_printf(ctl, msg,
	  DT_ENTER_MSG_MAXLEN,
      "Sorry, \"%s\" is not a"
      " predefined name nor a"
      " readable file name.",
      KERNEL.val);
    *start = KERNEL.desc;
    return DT_RC_GOOD;
  }

										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a destination image? 	*/
    arg, 2, 
    NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

										/* ................................ */
										/* ................................ */
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

static
dt_rc_e
dttool_convolve_work(
  dtparam_t *param,
  const char *input_name,
  const char *kernel_name,
  const char *output_name,
  app_t *a)
{
  DT_F("dttool_convolve_work");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;
  dtimage_t input;
  dtimage_t kernel;
  dtimage_t output;
  dtimage_t cut;
  
  input.xe = 0;
  kernel.xe = 0;
  output.xe = 0;
  cut.xe = 0;

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));
 
  if (a->ntype == DT_NTYPE_UNKNOWN)		/* no precision given?				*/
	a->ntype = input.ntype;
  
  if (a->ktype < 0)						/* not a predefined name? 			*/
  {
	DT_C(dtimage_import,(ctl,			/* get kernel from file 			*/
      &kernel, kernel_name));
  }
  else									/* this is a predefined name? 		*/
  {
	DT_C(dtimage_create,(ctl,			/* space for kernel image 			*/
      &kernel,
      DT_CAST(dtxy_t, param->w),
      DT_CAST(dtxy_t, param->h),
      DTNTYPE_ISFLOAT(a->ntype)?		/* precision is floating? 			*/
        DTNTYPE_F2:						/* use a double kernel 				*/
        DTNTYPE_S4));					/* else use a long kernel 			*/
    DT_C(dtimage_gen_kernel,(ctl,		/* generate predefined kernel 		*/
      &kernel, kernel_name));
  }

  DT_C(dtimage_create,(ctl,				/* space for output image		    */
    &output, input.xe, input.ye,
    a->ntype));

  DT_C(dtimage_constant,(ctl,			/* clear output image			    */
    &output, 0.0));

  DT_C(dtimage_convolve,(ctl,			/* convolve input to output			*/
    &input, &kernel, &output));

  DT_C(dtimage_overlay,(ctl,			/* chop out the dead pixels 		*/
    &output, &cut,
    kernel.xe/2, 
    kernel.ye/2,
    output.xe-(kernel.xe-1),
    output.ye-(kernel.ye-1)));

  DT_C(dtimage_export,(ctl,				/* output resulting image			*/
    &cut, output_name, 
    DT_CAST(dtxy_t,						/* image exported x position 		*/
      DTPARAM_GET_VALUE(x,				/* from -x arg 						*/
        DTIMAGE_NOPOS)),				/* or use window system default 	*/
    DT_CAST(dtxy_t,						/* image exported y position 		*/
      DTPARAM_GET_VALUE(y,				/* from -y arg 						*/
        DTIMAGE_NOPOS))));				/* or use window system default 	*/

cleanup:
  if (cut.xe)							/* cut image got allocated?	    	*/
    DT_I(dtimage_free,(ctl,				/* free cut image					*/
      &cut));
	   
  if (output.xe)						/* output image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free output image				*/
      &output));

  if (kernel.xe)						/* kernel image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free kernel image				*/
      &kernel));

  if (input.xe)							/* input image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free input image					*/
      &input));

  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_convolve)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char kernel_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[4];
  app_t a;
  dt_rc_e rc = DT_RC_GOOD;
  
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param,				/* tell what parameters we want		*/
    DTPARAM_FLAG_PRIMARY,
    &param->w,          "Kernel width",      NULL, "3",
    &param->h,          "Kernel height",     NULL, "3",
    &param->precision,  "Output image type", NULL, NULL,
    &param->x,          NULL,                NULL, NULL,
    &param->y,          NULL,                NULL, NULL,
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
    "Kernel", NULL, 
      kernel_name, sizeof(kernel_name),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttool_convolve_arg_check, &a));

										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
    DT_G(dttool_convolve_work,(			/* do the hard work 				*/
      param,
      input_name,
      kernel_name,
      output_name,      
      &a));
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dttool_convolve_arg_check, NULL, rc));
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
