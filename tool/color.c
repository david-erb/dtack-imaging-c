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
| dttool color - color converter
| 
| END
 *..........................................................................*/

#include <dttool.h>
DT_RCSID("tool $RCSfile: color.c,v $ $Revision: 1.6 $");
#include <dtack/os.h>
#include <dtack/color.h>
#include <dtack/lut8.h>

#define USAGE "color filename image-spec"

typedef struct {
  dt_color_org_t in_org;
  dt_color_org_t out_org;
} app_t;

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttool_color_arg_check(					/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttool_color_arg_check");
  app_t *a = (app_t *)app;
										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a source filename?		*/
    arg, 0, 
    NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

										/* ................................ */
  DT_Q(dttool_parse_arg_string,(param,	/* we have a output name? 			*/
    arg, 1, 
    NULL, msg, start));
  if (msg[0] != '\0')
    return DT_RC_GOOD;

										/* ................................ */
  DT_Q(dt_color_org_parse,(ctl, 
    &a->in_org,
    param->color,
    msg, DT_ENTER_MSG_MAXLEN));
  if (msg[0] != '\0')					/* bad color spec? 					*/
  {
	*start = param->color;				/* where the bad field is 			*/
	return DT_RC_GOOD;
  }
										/* ................................ */
  DT_Q(dt_color_org_parse,(ctl, 
    &a->out_org,
    param->output,
    msg, DT_ENTER_MSG_MAXLEN));
  if (msg[0] != '\0')					/* bad color spec? 					*/
  {
	*start = param->output;				/* where the bad field is 			*/
	return DT_RC_GOOD;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_color)
{
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char input_name[ARG_MAXLEN];
  char output_name[ARG_MAXLEN];
  dt_enter_arg_t arg[3];
  app_t a;
  dtfd_t fd;
  unsigned int npixels;
  unsigned int nbytes;
  dt_color_org_t *in_org = &a.in_org;
  dt_color_org_t *out_org = &a.out_org;
  void *in_buff = NULL;
  dtos_time_t mark1, mark2;
  long n;
  dt_color_convert_t convert;
  dtxy_t xe, ye;
  dtimage_t output;
  dt_rc_e rc;

  DT_MEMSET(&a, 0, sizeof(a));
  fd.fd = -1;

  output.xe = 0;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->precision,  NULL, NULL, NULL,
    &param->w,  NULL, NULL, "516",
    &param->h,  NULL, NULL, "452",
    &param->x,  NULL, NULL, NULL,
    &param->y,  NULL, NULL, NULL,
    &param->n,  "timing loops", NULL, "1",
    &param->color,  NULL, NULL, "rgb888",
    &param->output,  NULL, NULL, "pseudo",
    NULL));

  DT_Q(dt_enter_arg2,(ctl,				/* set up positionals			    */
    arg,
    "Input filename", NULL, 
      input_name, sizeof(input_name),
      DTTOOL_PARSE_FLAG_NEED,
    "Output image", NULL, 
      output_name, sizeof(output_name),
      DTTOOL_PARSE_FLAG_NEEDEXPORT,
    NULL));

  DT_Q(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttool_color_arg_check, (void *)&a));

  npixels = DT_CAST(long,				/* pixels expected 					*/
    param->w * param->h);
  
										/* ................................ */
  DT_C(dt_color_org_size,(ctl, in_org,	/* bytes expected in the input file	*/
    npixels, &nbytes));

  DT_C(dtos_malloc2,(ctl,
    &in_buff, nbytes, F, "in buff"));

  DT_C(dtfd_open,(ctl, &fd,				/* open input file 					*/
    input_name, "r"));
  DT_C(dtfd_read_all,(ctl, &fd,			/* read entire contents 			*/
    in_buff, nbytes));
  DT_C(dtfd_close,(ctl, &fd));			/* close input file 				*/
  fd.fd = -1;

										/* ................................ */
  DT_C(dt_color_org_size,(ctl, out_org,	/* bytes expected in output file	*/
    10, &nbytes));

  if (out_org->org ==					/* 24->8?							*/
	  DT_COLOR_ORG_PSEUDO ||
	  out_org->org ==					/* just luminance?					*/
	  DT_COLOR_ORG_LUMINANCE)
  {
	xe = param->w;
	ye = param->h;
  }
  else
  if (out_org->org ==					/* interleave by plane? 			*/
	  DT_COLOR_ORG_R_G_B ||
	  out_org->org == 
	  DT_COLOR_ORG_Y_CB_CR ||
	  out_org->org == 
	  DT_COLOR_ORG_Y_CB_CR_411)
  {
	xe = param->w;
	ye = (param->h * nbytes) / 10;
  }
  else									/* interleave by byte? 				*/
  {
	xe = (param->w * nbytes) / 10;
	ye = param->h;
  }

  DT_C(dtimage_create,(ctl,				/* space for output image		    */
    &output, xe, ye, 
    DT_NTYPE_B08))

  DT_C(dt_color_convert_init,(ctl,		/* prepare conversion 				*/
    &convert, in_org, out_org));

  DT_C(dtos_time,(ctl, &mark1));
  for (n=0; n<param->n; n++)
  {
    DT_C(dt_color_convert,(ctl,			/* convert 							*/
      &convert, 
      in_buff, in_org,
      DT_CAST(dtxy_t, param->w),						 
      DT_CAST(dtxy_t, param->h), 
      output.row[0].dvoid, out_org));
  }
  DT_C(dtos_time,(ctl, &mark2));
  DT_C(dtos_time_report,(ctl,	
    &mark1, &mark2, "conversions"));
  
  DT_C(dt_color_convert_uninit,(ctl,	/* free conversion resources 		*/
    &convert, in_org, out_org));
	   
										/* ................................ */
  DT_C(dtimage_export,(ctl,				/* output resulting image			*/
    &output, output_name, 
    DT_CAST(dtxy_t,						/* image exported x position 		*/
      DTPARAM_GET_VALUE(x,				/* from -x arg 						*/
        DTIMAGE_NOPOS)),				/* or use window system default 	*/
    DT_CAST(dtxy_t,						/* image eyported y position 		*/
      DTPARAM_GET_VALUE(y,				/* from -y arg 						*/
        DTIMAGE_NOPOS))));				/* or use window system default 	*/
  
  if (out_org->org ==					/* 24->8?							*/
	  DT_COLOR_ORG_PSEUDO)
  {
	dtlut8_t *lut8 = (dtlut8_t *)		/* pseudo has lut at end 			*/
      (output.row[output.ye-1].b08 +
       output.xe);
	DT_C(dtlut8_export,(ctl, lut8,		/* throw the lut into the window 	*/
      output_name, 0, 256));
  }

cleanup:
  if (output.xe)						/* output image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free output image				*/
      &output));

  if (in_buff != NULL)
    DT_I(dtos_free2,(ctl,
      &in_buff, F, "in buff"));

  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

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
