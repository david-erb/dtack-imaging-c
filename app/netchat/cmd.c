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

#include <netchat.h>
DT_RCSID("app/netchat $RCSfile: cmd.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
netchat_cmd_ident(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_ident");
  netchat_t *netchat = (netchat_t *)arg;
  dt_rc_e rc = DT_RC_GOOD;
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "server_ident: %s", netchat->ident));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
netchat_cmd_state(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_state");
  netchat_t *netchat = (netchat_t *)arg;
  char t[16];
  dt_rc_e rc = DT_RC_GOOD;
  DT_Q(dtstr_printchar,(ctl,
    (char)netchat->state,
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
netchat_cmd_talk(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_talk");
  netchat_t *netchat = (netchat_t *)arg;

  if (strlen(							/* no partner chosen yet? 			*/
      netchat->partner_contents) == 0)
    return dt_err(ctl, F,
      NETCHAT_M_PARTNERFIRST);

  DT_Q(netchat_converse,(netchat,		/* start conversation going 		*/
    netchat->ident,					/* we talk 							*/
    netchat->partner_contents));		/* they listen 						*/
  
  DT_Q(dtdg_server_response,(ctl, 
    dgserver,
    DTDG_RESPONSE_RESULT
    ": now talking to %s", 
    netchat->partner_contents));

  netchat->state = NETCHAT_STATE_TALK;

  netchat->message_contents[0] = '\0';
  netchat->update = 1;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
netchat_cmd_listen(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_listen");
  netchat_t *netchat = (netchat_t *)arg;

  if (strlen(							/* no partner chosen yet? 			*/
      netchat->partner_contents) == 0)
    return dt_err(ctl, F,
      "Please choose partner.");

  DT_Q(netchat_converse,(netchat,		/* start conversation going 		*/
    netchat->partner_contents,			/* they talk 						*/
    netchat->ident));					/* we listen 						*/

  netchat->state = NETCHAT_STATE_LISTEN;

  netchat->message_contents[0] = '\0';
  netchat->update = 1;
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
netchat_cmd_idle(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_idle");
  netchat_t *netchat = (netchat_t *)arg;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  char request[256];
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;

  DT_C(dtstr_printf,(ctl,				/* build the cease request			*/
    request, sizeof(request),
    "GET /cease"));

  DT_C(netchat_ask,(netchat,			/* cease our buddy					*/
    netchat->ident, request,
    dgclient));

  netchat->state = NETCHAT_STATE_IDLE;

cleanup:

  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, dgclient));

  netchat->message_contents[0] = '\0';
  netchat->update = 1;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
netchat_cmd_privacy(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_privacy");
  netchat_t *netchat = (netchat_t *)arg;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  char request[256];
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;

  DT_C(dtstr_printf,(ctl,				/* build the privacy request		*/
    request, sizeof(request),
    "GET /lock"));

  DT_C(netchat_ask,(netchat,			/* lock our buddy					*/
    netchat->ident, request,
    dgclient));

  netchat->state = NETCHAT_STATE_PRIVACY;

cleanup:

  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, dgclient));

  netchat->message_contents[0] = '\0';
  netchat->update = 1;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
netchat_cmd_partner(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("netchat_cmd_partner");
  netchat_t *netchat = (netchat_t *)arg;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  const char *ident = rest;
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "ident=\"%s\"", ident);

  if (!strcmp(ident, netchat->ident))	/* trying to talk to ourselves? 	*/
  {
	rc = dt_err(ctl, F,
      NETCHAT_M_TALKTOSELF);
	goto cleanup;
  }

  DT_C(netchat_ask,(netchat,			/* see if partner can respond		*/
    ident,
    DTDG_REQUEST_STATE,
    dgclient));

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "ask succeeded, ident=\"%s\"", ident);

  DT_C(dtstr_printf,(ctl,
	netchat->partner_contents,
    sizeof(netchat->partner_contents),
    "%s", ident));
  
  DT_Q(dtdg_server_response,(ctl, dgserver,
    DTDG_RESPONSE_RESULT
    ": contacted partner %s", ident));

cleanup:
  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, 
      dgclient));

  netchat->message_contents[0] = '\0';
  netchat->update = 1;
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
