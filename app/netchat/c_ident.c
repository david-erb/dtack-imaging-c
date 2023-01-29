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
DT_RCSID("app/netchat $RCSfile: c_ident.c,v $ $Revision: 1.8 $");
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
      "Please choose partner.");

  DT_Q(netchat_converse,(netchat,		/* start conversation going 		*/
    netchat->ident,					/* we talk 							*/
    netchat->partner_contents));		/* they listen 						*/

  netchat->state = NETCHAT_STATE_TALK;

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
  dt_rc_e rc = DT_RC_GOOD;
  netchat->state = NETCHAT_STATE_IDLE;
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
  dt_rc_e rc = DT_RC_GOOD;
  netchat->state = NETCHAT_STATE_PRIVACY;
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
  dt_rc_e rc = DT_RC_GOOD;

  dgclient->initialized = 0;

#ifdef NONO
  DT_Q(dtstr_printf,(ctl,				/* copy partner contents to spec 	*/
    spec, sizeof(spec),
    "%s", netchat->partner_contents));
  
  netchat->partner_contents[0] = '\0';
  
  DT_Q(netchat_ident,(netchat,		/* find ident for spec				*/
    spec,
	netchat->partner_contents,
    sizeof(netchat->partner_contents)));
#endif


  DT_C(netchat_make_request,(			/* see if partner can respond		*/
    netchat,
    netchat->partner_contents,
    DTDG_REQUEST_STATE,
    dgclient));

cleanup:
  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl, 
      dgclient));
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
