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
| textchat_* - textchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: init.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/liner.h>

/*..........................................................................
 * initialize textchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_init(
  textchat_t **ptextchat,
  dtparam_t *param)
{
  DT_F("textchat_init");
  dt_ctl_t *ctl = &param->ctl;
  textchat_t *textchat = NULL;
  dtfd_t fd;
  dt_rc_e rc = DT_RC_GOOD;

  fd.fd = -1;

  DT_C(dtos_malloc2,(ctl,
    (void **) &textchat,
    sizeof(*textchat),
    F, "textchat structure"));
  
  memset(textchat, 0, 
    sizeof(*textchat));

  textchat->ctl = &param->ctl;
  textchat->ident = param->ident;		/* absolutely required 				*/
  textchat->poll_period = param->dt;
  textchat->state = 
    TEXTCHAT_STATE_STARTING;

  DT_C(textchat_log_setup,(textchat));	/* set up file logging 				*/

  DT_C(ustate_init_config,(ctl,			/* get ustate database 				*/
    &textchat->ustate, param->config));

  DT_C(ustate_find,(ctl,				/* get our dgserver listen address	*/
    &textchat->ustate,
    param->ident, "netchat",
    "dgserver_spec", 
    param->connect,
    &textchat->dgserver_spec));

										/* ................................ */
  if (strstr(param->dialog, ","))		/* looks like a dialog spec? 		*/
  {
	DT_C(textchat_dialog_init,(
      textchat, param));
  }
  else									/* else presume graphics spec 		*/
  if (strcmp(param->dialog, "none"))	/* unless it is none 				*/
  {
	DT_C(textchat_graphics_init,(
      textchat, param));
  }

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  if (rc != DT_RC_GOOD)
  {
    DT_I(textchat_uninit,(textchat));
	DT_I(textchat_log_quit,(textchat));	/* quit with the file logging 		*/
  }
  else
    *ptextchat = textchat;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_uninit(
  textchat_t *textchat)
{
  DT_F("textchat_uninit");
  dt_rc_e rc = DT_RC_GOOD;

  if (textchat != NULL)
  {
	dt_ctl_t *ctl = textchat->ctl;

	if (textchat->dialog_initialized)
	  DT_I(textchat_dialog_uninit,(
        textchat));

	if (textchat->graphics_initialized)
	  DT_I(textchat_graphics_uninit,(
        textchat));

	DT_I(ustate_uninit,(ctl,
      &textchat->ustate));
  
	DT_I(textchat_log_quit,(textchat));	/* quit with the file logging 		*/

	DT_I(dtos_free2,(ctl, textchat,
      F, "textchat structure"));
  }

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
