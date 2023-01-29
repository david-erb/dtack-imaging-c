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
| dttool morph - morph image with kernel
| 
|.SYNOPSIS
|:dttool morph [parameters] input kernel output
| 
|.SUMMARY
| Morph ~input~ image with ~kernel~ to produce ~output~ image.
| ~Output~ image is created to be smaller than ~input~ by size of
| ~kernel~ less one so no undefined border area is present.
| ~Kernel~ may be created from one of a set of known types,
| or may be read from a file.
| 
|.ARGUMENTS
| ~input~	name of the image to be morphd
| ~kernel~	name of a predefined kernel, or name of a file
| ~output~	name of output image to be created
|
|.PARAMETERS
| :-w:			width of kernel, ignored if ~kernel~ is a file
| :-h:			height of kernel, ignored if ~kernel~ is a file
| :-precision:	desired precision of ~output~
|dtinclude(paramxy.dx)
|dtinclude(params1.dx)
| 
|.DESCRIPTION
| The name ~kernel~ is first searched for in a list of predefined
| kernel names.
| If found, the kernel is created with the size given by
| ~param->w~ and ~param->h~ and precision either
| :DT_NTYPE_F2: or :DT_NTYPE_S4: (double or long),
| depending on ~param->precision~ if given, or ~input->ntype~.
| if ~kernel~ is not a predefined name,
| then it is assumed to be the name of a file containing
| an image specifying the kernel.
|  
|.RETURN VALUES
|dtinclude(return2.dx)
| 
|.ERRORS
| - ~input~ not found
| - ~kernel~ not a predefined name and not a filename
| - ~output~ not a valid file or window name
| - not enough memory to hold ~input~, ~output~,
|	and convolution work buffers at the same time
|
|dtinclude(params2.dx)
| 
| END
 *..........................................................................*/

#include <dttool.h>
DT_RCSID("tool $RCSfile: morph.c,v $ $Revision: 1.6 $");
#include <dtack/os.h>

#define USAGE "morph [-op dilate|erode] input kernel output"

static const char *ops[] = {
  "dilate",
  "erode",
  NULL
};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_morph_arg_check(					/* check args and params            */
  dt_ctl_t *ctl,
  void *appstruct,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_morph_arg_check");

  DT_Q(dttool_parse_op,(ctl,			/* make sure op is understood       */
    param, ops, msg, start));

  DT_Q(dttool_parse_precision,(ctl,		/* check precision					*/
    param, NULL, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_morph)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char kernel_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[4];
  dtimage_t input;						/* image structure					*/
  dtimage_t kernel;
  dtimage_t output;
  dtimage_t cut;
  dt_ntype_e ntype;
  int choice;
  dt_rc_e rc;
  
  input.xe = 0;
  kernel.xe = 0;
  output.xe = 0;
  cut.xe = 0;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, NULL,
    &param->op, NULL, NULL, "dilate",
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
    "Kernel image", NULL, 
      kernel_name, sizeof(kernel_name),
    "Output image", NULL, 
      output_name, sizeof(output_name),
    NULL));

  DT_Q(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_morph_arg_check, NULL));

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));

  if (param->precision != NULL)
  {
	char message[80];
    DT_GI(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, sizeof(message)));
    if (message[0])
	{
      rc = dt_err(ctl, F, "%s", message);
	  goto cleanup;
	}
  }
  else
    ntype = input.ntype;
  
  DT_C(dtstr_choice,(ctl,				/* check kernel name  				*/
    dtimage_gen_kernel_names,			/* against list of predefined names */
    kernel_name,
    &choice));
  if (choice >= 0)						/* it is a predefined name? 		*/
  {
	DT_C(dtimage_create,(ctl, &kernel,	/* space for kernel image 			*/
      DT_CAST(dtxy_t, param->w),
      DT_CAST(dtxy_t, param->h),
      DTNTYPE_ISFLOAT(ntype)?			/* precision is floating? 			*/
        DTNTYPE_F2:						/* use a double kernel 				*/
        DTNTYPE_S4));					/* else use a long kernel 			*/
	DT_C(dtimage_gen_kernel,(ctl,
      &kernel, kernel_name));
  }
  else									/* not a predefined name? 			*/
  {
	int isreadable;
	DT_C(dtos_isreadable,(ctl,			/* see if it is a file 				*/
      kernel_name, &isreadable));
	if (isreadable)
	  DT_C(dtimage_import,(ctl,
        &kernel, kernel_name));
  }
  if (kernel.xe == 0)					/* did not resolve a kernel name? 	*/
  {
    rc = dt_err(ctl, F, 
      "Sorry, \"%s\" is not a"
      " predefined name nor a"
      " readable file name",
      &kernel_name);
	goto cleanup;
  }

  DT_C(dtimage_create,(ctl,				/* space for output image		    */
    &output, input.xe, input.ye,
    ntype));

  DT_C(dtimage_constant,(ctl,			/* clear output image			    */
    &output, 0.0));

  if (DT_ABBREV("dilate", param->op))
    DT_C(dtimage_dilate3x3,(ctl,		/* dilate input to output			*/
      &input, &output))
  else
  if (DT_ABBREV("erode", param->op))
    DT_C(dtimage_erode3x3,(ctl,			/* erode input to output			*/
      &input, &output))

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

  if (kernel.xe)						/* kernel image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free kernel image				*/
      &kernel));

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
