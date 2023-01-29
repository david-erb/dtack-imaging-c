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
| netchat_* - netchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <netchat.h>
DT_RCSID("app/netchat $RCSfile: request.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

static const char *requests[] = {
  "ident",
  "state",
  "talk",
  "listen",
  "idle",
  "privacy",
  "partner",
  "exit",
  NULL
};

static dtdg_server_request_f *funcs[] = {
  netchat_cmd_ident,
  netchat_cmd_state,
  netchat_cmd_talk,
  netchat_cmd_listen,
  netchat_cmd_idle,
  netchat_cmd_privacy,
  netchat_cmd_partner,
  dtdg_server_exit,
  NULL
};

/*..........................................................................
 * handle incoming request from ipc
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_request_handle(
  netchat_t *netchat,
  dtdg_server_t *dgserver,
  char *request)
{
  DT_F("netchat_request_handle");
  dt_ctl_t *ctl = netchat->ctl;
  const char *s = NULL;
  char request2[256];
  char *message = netchat->message;
  int max = sizeof(netchat->message);
  dt_rc_e rc, rc2;

										/* ................................ */
										/* handle toggle requests 			*/
# define IS_REQUEST(S) \
    DT_ABBREV(request+strlen("GET /"), \
      (S))

  if (IS_REQUEST("talk"))
  {
  	if (netchat->state ==	
		NETCHAT_STATE_TALKING)
	  s = "idle";
	else
	if (netchat->state ==	
		NETCHAT_STATE_LISTENING)
	  s = "talk";
	else
	if (netchat->state ==	
		NETCHAT_STATE_IDLING)
	  s = "talk";
  }
  else
  if (IS_REQUEST("listen"))
  {
	if (netchat->state ==	
		NETCHAT_STATE_LISTENING)
	  s = "idle";
	else
	if (netchat->state ==	
		NETCHAT_STATE_TALKING)
	  s = "listen";
	else
	if (netchat->state ==	
		NETCHAT_STATE_IDLING)
	  s = "listen";
  }

  if (s != NULL)
    DT_Q(dtstr_printf,(ctl,
      request2, sizeof(request2),
      "GET /%s", s))
  else
    DT_Q(dtstr_printf,(ctl,
      request2, sizeof(request2),
      "%s", request));

										/* ................................ */
  ctl->user = (void *)netchat;			/* capture errors 					*/

  if (dgserver != NULL)
  DT_C(dtdg_server_header_plain,(ctl,	/* give http header 				*/
    dgserver, "Netchat"));

  message[0] = '\0';					/* clear previous message 			*/

  DT_G(dtdg_server_request,(ctl,		/* generate datagram response		*/
    dgserver, request2,
    (void *)netchat,
    requests, funcs,
    message, max));

  rc2 = rc;

  if (message[0] != '\0')				/* got an error message? 			*/
  {
    DT_C(dtdg_server_response,(ctl,
      dgserver,
      DTDG_RESPONSE_ERROR
      ": %s", message));
	DT_C(dtdg_server_response,(ctl, 
      dgserver,
      DTDG_RESPONSE_RC
      ": %d", DT_RC_BAD));
    DT_I(dtstr_printf,(ctl,				/* copy message to display 			*/
      netchat->message_contents,
      sizeof(netchat->message_contents),
      "%s", message));
    netchat->update = 1;				/* redraw screen 					*/
  }
  else
  {
	DT_C(dtdg_server_response,(ctl, dgserver,
      DTDG_RESPONSE_RC
      ": %d", DT_RC_GOOD));
  }

  if (dgserver != NULL)
  DT_C(dtdg_server_trailer_plain,(ctl,	/* give http trailer 				*/
    dgserver));

  if (dgserver != NULL)
  DT_C(dtdg_server_done,(ctl,			/* finish response to the datagram	*/
    dgserver));

cleanup:
  ctl->user = NULL;						/* quit capturing errors 			*/

  if (rc2 == DT_RC_STOP)
    rc = rc2;
  else
    rc = DT_RC_GOOD;
  return rc;
}

/*..........................................................................
 * poll for events and pass them to the appropriate handler function
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_request_poll(
  netchat_t *netchat,
  dtdg_server_t *dgserver)
{
  DT_F("netchat_request_poll");
  dt_ctl_t *ctl = netchat->ctl;
  char *request = NULL;
  dt_rc_e rc;
	  
  do {
	if (request != NULL)				/* previous request allocated? 		*/
	{
	  DT_C(dtos_free2,(ctl, request,
        F, "request"));
	  request = NULL;
	}
										/* ................................ */
	if (request == NULL)
	  DT_C(netchat_dialog_poll,(		/* poll for keyboard input			*/
        netchat, &request)); 

	if (request == NULL)
	  DT_C(netchat_graphics_poll,(		/* poll for graphics event input	*/
        netchat, &request));

	if (request != NULL)				/* there is a user event?			*/
	{
	  DT_C(netchat_request_handle,(	/* handle the user request			*/
        netchat, NULL, request));
	}
										/* ................................ */
	else								/* no user event? 					*/
	{
	  int is_request;
	  DT_C(dtdg_server_is_request,(ctl,	/* poll for dg command input		*/
        dgserver, &is_request));
	  
	  if (is_request)					/* there is a request for us? 		*/
	  {
  	    DT_C(dtdg_server_get_request,(	/* receive the datagram request		*/
          ctl, dgserver, &request));
		if (request != NULL)			/* a request is available? 			*/
  	      DT_C(netchat_request_handle,(	/* handle the ipc request			*/
            netchat, dgserver, 
            request));
	  }
	}
										/* ................................ */
  } while(request != NULL);				/* handle all pending requests 		*/

cleanup:
  if (request != NULL)
	DT_I(dtos_free2,(ctl, request,
      F, "request"));
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
