/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>							/* NCI fundamental declarations 	*/
#include <umdec.h>						/* public to the world 				*/

#include <dtack/os.h>						/* utility OS functions 			*/
#include <dtack/str.h>						/* utility string functions 		*/

#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/color.h>
#include <umdecscr.h>					/* screen support for display 		*/

#include <dtack/prj.h>

DT_RCSID("source/mpeg $RCSfile: umdecssg.c,v $ $Revision: 1.7 $");

#define DESC "mpeg software simulation"

#if DTPRJ_MPEG_SSG == DT_1

#ifndef DTDEPEND
#  include <ssg.h>
#endif

typedef struct {
  dt_ctl_t *ctl;
  char *spec;
  char *output_type;
  const char *scr_spec;
  int keep_picture_coding;
  umdec_frame_callback_f *frame_callback;
  void *frame_arg;
  umdec_scr_t umdec_scr;
  int umdec_scr_initted;
  double frame;
} umdec_ssg_priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_ssg_release(						/* release resources 				*/
  dt_ctl_t *ctl,
  umdec_ssg_priv_t *priv)
{
  DT_F("umdec_ssg_release");
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
	if (priv->umdec_scr_initted)
      DT_I(umdec_scr_destroy,(ctl,
        &priv->umdec_scr));
	if (priv->spec != NULL)
      DT_I(dtos_free2,(ctl, priv->spec,
        F, "priv->spec"));
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_ssg_uninit(						/* close object						*/
  dt_ctl_t *ctl,
  umdec_t *umdec)
{
  DT_F("umdec_ssg_uninit");
  umdec_ssg_priv_t *priv =
    umdec->priv;
  DT_Q(umdec_ssg_release,(ctl, priv));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_ssg_get_status(					/* get operation parameters 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* decoder object structure 		*/
  umdec_status_t *status)				/* returned status					*/
{
  DT_F("umdec_ssg_get_status");
  umdec_ssg_priv_t *priv =
    umdec->priv;
  umdec->status.frame = priv->frame;
  umdec->status.pts = priv->frame * 90000.0;
  return DT_RC_GOOD;
}

/*..........................................................................
 * this is a umdec frame callback
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_ssg_frame_callback(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  umdec_frametype_e frametype)			/* type of frame being given 		*/
{
  DT_F("umdec_ssg_frame_callback");
  umdec_ssg_priv_t *priv =
    umdec->priv;
  dt_rc_e rc;

  DT_Q(umdec_scr_callback,(ctl,			/* display the image 				*/
    umdec, arg, ssg_frametype,
    ssg_src, ssg_xe, ssg_ye));

  if (ssg_frametype ==
	  SSG_FRAMETYPE_YCBCR_420 ||
	  ssg_frametype ==
	  SSG_FRAMETYPE_PSEUDO)
  {
	priv->frame++;						/* increment local frame count 		*/

	DT_Q((*priv->frame_callback),(ctl,		/* notify caller 					*/
      umdec, priv->frame_arg, 
      UMDEC_FRAMETYPE_FRAMEINC));
  }

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
umdec_ssg_go(							/* start playing					*/
  dt_ctl_t *ctl,
  umdec_t *umdec,
  umdec_control_t *control)
{
  DT_F("umdec_ssg_go");
  umdec_ssg_priv_t *priv =
    umdec->priv;
  dt_rc_e rc = DT_RC_GOOD;

  priv->frame_callback =				/* userp pointers 					*/
    umdec->frame_callback;
  umdec->frame_callback =
    umdec_ssg_frame_callback;
  
  priv->frame_arg =
    umdec->frame_arg;
  umdec->frame_arg = 
    (void *)&priv->umdec_scr;

  DT_C(ssg_main,(ctl, umdec,				/* start playing immediately 		*/
    priv->keep_picture_coding,
    priv->output_type));
  
cleanup:
  umdec->frame_callback =   
    priv->frame_callback;
  umdec->frame_arg =   
    priv->frame_arg;
  
  return rc;
}

#endif

/*..........................................................................
| NAME
| umdec_ssg_init() - initialize ssg decoder object
|
| SUMMARY
| Initialize the decoder object ~mdec~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_ssg_init(
  dt_ctl_t *ctl,
  umdec_t *umdec,
  const char *spec)
{
  DT_F("umdec_ssg_init");
# if DTPRJ_MPEG_SSG == DT_1
  umdec_ssg_priv_t *priv = NULL;
  int ntokens;
  char *p;
  dt_rc_e rc;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL mdecssg spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(umdec_ssg_priv_t),
    F, "priv"));
  memset(priv, 0,						/* clear private structure 			*/
    sizeof(umdec_ssg_priv_t));

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));

  p = priv->spec;

  priv->output_type = "raw";
  if (ntokens >= 1)
  {
	if (strcmp(p, SSG_OUTPUT_RAW) &&
		strcmp(p, SSG_OUTPUT_DITHER))
	{
	  rc = dt_err(ctl, F,
        "\"%s\" invalid in umdec spec,"
        " must be %s or %s", p,
		SSG_OUTPUT_RAW,
		SSG_OUTPUT_DITHER);
	  goto cleanup;
	}
	priv->output_type = p;
	p += strlen(p) + 1;
  }
  
  priv->keep_picture_coding = 0;		/* let ssg choose default 			*/
  if (ntokens >= 2)
  {
	if (!strcmp(p, "I"))				/* keep I frames only? 				*/
  	  priv->keep_picture_coding = 1;
	else
	if (!strcmp(p, "P"))
  	  priv->keep_picture_coding = 2;
	else
	if (!strcmp(p, "B"))
  	  priv->keep_picture_coding = 3;
	else
	if (!strcmp(p, "D"))
  	  priv->keep_picture_coding = 4;
	else
	if (!strcmp(p, ""))					/* keep all? 						*/
  	  priv->keep_picture_coding = 4;
	else
	{
	  rc = dt_err(ctl, F,
        "\"%s\" invalid in umdec spec,"
        " must be I, P, or B", p);
	  goto cleanup;
	}
	p += strlen(p) + 1;
  }

  DT_C(dtprj_param_default,(ctl,			/* look up screen default string	*/
    "-screen", &priv->scr_spec));
  if (ntokens >= 3 &&					/* caller is giving screen spec? 	*/
	  strlen(p) > 0)
  {
	int i;
	priv->scr_spec = p;
	for (i=3; i<ntokens; i++)
	{
	  p += strlen(p);
	  *p++ = ',';						/* replace the comma 				*/
	}
  }

  priv->ctl = ctl;

  umdec->priv          = priv;
  umdec->uninit        = umdec_ssg_uninit;
  umdec->go            = umdec_ssg_go;
  umdec->get_status    = umdec_ssg_get_status;

  DT_C(umdec_scr_create,(ctl,			/* create screen access 			*/
    &priv->umdec_scr, priv->scr_spec));
  priv->umdec_scr_initted = 1;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	DT_I(umdec_ssg_release,(ctl, priv));
  }

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_ssg_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "umdec_ssg lib";
  if (rev_major != NULL)
    *rev_major = 2;
  if (rev_minor != NULL)
    *rev_minor = 0;
  if (rev_date != NULL)
    *rev_date = 19970218;
  return DT_RC_GOOD;
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
