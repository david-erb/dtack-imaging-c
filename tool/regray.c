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
| dttool regray utility - 
| 
| DESCRIPTION
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: regray.c,v $ $Revision: 1.6 $");

#define USAGE "[params] input ave|laplace output"

typedef struct {
  dt_ntype_e ntype;
} app_t;

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_regray_arg_check(				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_regray_arg_check");
  app_t *a = (app_t *)app;
										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a source image? 			*/
    arg, 0, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a destination image? 	*/
    arg, 1, NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

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
dttool_regray_work(
  dtparam_t *param,
  const char *input_name,
  const char *output_name,
  app_t *a)
{
  DT_F("dttool_regray_work");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;						/* image structure					*/
  dtimage_t output;
  dt_rc_e rc;
  
  input.xe = 0;
  output.xe = 0;

  DT_C(dtimage_import,(ctl,				/* get input image					*/
    &input, input_name));
 
  if (a->ntype == DT_NTYPE_UNKNOWN)		/* no precision given?				*/
	a->ntype = input.ntype;				/* use same as input 				*/

  DT_C(dtimage_create,(ctl,				/* space for output image		    */
    &output, input.xe, input.ye,
    a->ntype));

  {
    double imin, imax;
	double omin, omax;
	double gain, offset;
    DT_C(dtimage_range,(ctl, &input,	/* find input dynamic range 		*/
      &imin, &imax));

    omin = DTPARAM_GET_VALUE(min,
      dt_ntype_minval[output.ntype]);
    omax = DTPARAM_GET_VALUE(max,
      dt_ntype_maxval[output.ntype]);

    if (imax == imin)
	  gain = 0;
	else
      gain = (omax - omin) /			/* compute scaling 					*/
             (imax - imin);
    offset = -imin * gain + omin;		/* compute offset 					*/

    DT_C(dtimage_arith,(ctl,			/* convert to new range 			*/
      &input, gain, 
      &input, 0, offset, 
      DT_OP_ADD, 0,
      &output));

    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "input dynamic range %g to %g"
      " mapped into %g to %g",
      imin, imax, 
      imin * gain + offset,
      imax * gain + offset);

    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "gain %g, offset %g", gain, offset);
  }

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

  if (input.xe)							/* input image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free input image					*/
      &input));

  return rc;
}
/*..........................................................................*/

DTTOOL_MAIN(dttool_regray)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dt_rc_e rc;
  
  DT_G(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,				/* tell what parameters we want		*/
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  "Output image type", NULL, NULL,
    &param->w,          "Kernel width",      NULL, "3",
    &param->h,          "Kernel height",     NULL, "3",
    &param->x,          NULL,                NULL, NULL,
    &param->y,          NULL,                NULL, NULL,
    NULL));

  DT_GI(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, "b08",
    &param->min,  "Output gray minimum", NULL, NULL,
    &param->max,  "Output gray maximum", NULL, NULL,
    &param->x,    NULL,                  NULL, NULL,
    &param->y,    NULL,                  NULL, NULL,
    NULL));

  DT_GI(dt_enter_arg2,(ctl,				/* set up positionals			    */
    arg,
    "Input image", NULL, 
      input_name, sizeof(input_name),
      DTTOOL_PARSE_FLAG_NEEDIMPORT,
    "Output image", NULL, 
      output_name, sizeof(output_name),
      DTTOOL_PARSE_FLAG_NEEDEXPORT,
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_regray_arg_check, &a));

										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
    DT_G(dttool_regray_work,(			/* do the hard work 				*/
      param,
      input_name,
      output_name,      
      &a));
	
    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, USAGE, arg,
      dttool_regray_arg_check, NULL, rc));
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
