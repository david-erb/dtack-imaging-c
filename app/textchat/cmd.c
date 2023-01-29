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
| Introduction - intro
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: cmd.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_ident(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_ident");
  textchat_t *textchat = (textchat_t *)arg;
  dt_rc_e rc = DT_RC_GOOD;
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "server_ident: %s", textchat->ident));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_state(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_state");
  textchat_t *textchat = (textchat_t *)arg;
  char t[16];
  dt_rc_e rc = DT_RC_GOOD;
  DT_Q(dtstr_printchar,(ctl,
    (char)textchat->state,
    t, sizeof(t)));
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "server_state: %s", t));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_talk(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_talk");
  textchat_t *textchat = (textchat_t *)arg;

  if (strlen(							/* no partner chosen yet? 			*/
      textchat->partner_contents) == 0)
    return dt_err(ctl, F,
      TEXTCHAT_M_PARTNERFIRST);

  DT_Q(textchat_converse,(textchat,		/* start conversation going 		*/
    textchat->ident,					/* we talk 							*/
    textchat->partner_contents));		/* they listen 						*/
  
  DT_Q(dtdg_server_response,(ctl, 
    dgserver,
    DTDG_RESPONSE_RESULT
    ": now talking to %s", 
    textchat->partner_contents));

  textchat->state = TEXTCHAT_STATE_TALK;

  textchat->message_contents[0] = '\0';
  textchat->update = 1;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_listen(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_listen");
  textchat_t *textchat = (textchat_t *)arg;

  if (strlen(							/* no partner chosen yet? 			*/
      textchat->partner_contents) == 0)
    return dt_err(ctl, F,
      "Please choose partner.");

  DT_Q(textchat_converse,(textchat,		/* start conversation going 		*/
    textchat->partner_contents,			/* they talk 						*/
    textchat->ident));					/* we listen 						*/

  textchat->state = TEXTCHAT_STATE_LISTEN;

  textchat->message_contents[0] = '\0';
  textchat->update = 1;
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_idle(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_idle");
  textchat_t *textchat = (textchat_t *)arg;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  char request[256];
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;

  DT_C(dtstr_printf,(ctl,				/* build the cease request			*/
    request, sizeof(request),
    "GET /cease"));

  DT_C(textchat_ask,(textchat,			/* cease our buddy					*/
    textchat->ident, request,
    dgclient));

  textchat->state = TEXTCHAT_STATE_IDLE;

cleanup:

  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, dgclient));

  textchat->message_contents[0] = '\0';
  textchat->update = 1;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_privacy(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_privacy");
  textchat_t *textchat = (textchat_t *)arg;
  dt_rc_e rc = DT_RC_GOOD;
  textchat->state = TEXTCHAT_STATE_PRIVACY;

  textchat->message_contents[0] = '\0';
  textchat->update = 1;
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
textchat_cmd_partner(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("textchat_cmd_partner");
  textchat_t *textchat = (textchat_t *)arg;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  const char *ident = rest;
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;

#ifdef NONO
  DT_Q(dtstr_printf,(ctl,				/* copy partner contents to spec 	*/
    spec, sizeof(spec),
    "%s", textchat->partner_contents));
  
  textchat->partner_contents[0] = '\0';
  
  DT_Q(textchat_ident,(textchat,		/* find ident for spec				*/
    spec,
	textchat->partner_contents,
    sizeof(textchat->partner_contents)));
#endif

  if (!strcmp(ident, textchat->ident))	/* trying to talk to ourselves? 	*/
  {
	rc = dt_err(ctl, F,
      TEXTCHAT_M_TALKTOSELF);
	goto cleanup;
  }

  DT_C(textchat_ask,(textchat,			/* see if partner can respond		*/
    ident,
    DTDG_REQUEST_STATE,
    dgclient));

  DT_C(dtstr_printf,(ctl,
	textchat->partner_contents,
    sizeof(textchat->partner_contents),
    "%s", ident));
  
  DT_Q(dtdg_server_response,(ctl, dgserver,
    DTDG_RESPONSE_RESULT
    ": contacted partner %s", ident));

cleanup:
  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, 
      dgclient));

  textchat->message_contents[0] = '\0';
  textchat->update = 1;
										/* ................................ */
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
