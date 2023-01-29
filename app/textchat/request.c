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
DT_RCSID("app/textchat $RCSfile: request.c,v $ $Revision: 1.8 $");
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
  textchat_cmd_ident,
  textchat_cmd_state,
  textchat_cmd_talk,
  textchat_cmd_listen,
  textchat_cmd_idle,
  textchat_cmd_privacy,
  textchat_cmd_partner,
  dtdg_server_exit,
  NULL
};

/*..........................................................................
 * handle incoming request from ipc
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_request_handle(
  textchat_t *textchat,
  dtdg_server_t *dgserver,
  char *request)
{
  DT_F("textchat_request_handle");
  dt_ctl_t *ctl = textchat->ctl;
  char *message = textchat->message;
  int max = sizeof(textchat->message);
  dt_rc_e rc, rc2;
  
  ctl->user = (void *)textchat;			/* capture errors 					*/

#if 0
  if (dgserver != NULL)
  DT_C(dtdg_server_header,(ctl,			/* give html header 				*/
    dgserver, "Textchat"));
#else
  DT_C(dtdg_server_response,(ctl, 
    dgserver, ""));
#endif

  message[0] = '\0';					/* clear previous message 			*/

  DT_G(dtdg_server_request,(ctl,		/* generate datagram response		*/
    dgserver, request,
    (void *)textchat,
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
      textchat->message_contents,
      sizeof(textchat->message_contents),
      "%s", message));
    textchat->update = 1;				/* redraw screen 					*/
  }
  else
  {
	DT_C(dtdg_server_response,(ctl, dgserver,
      DTDG_RESPONSE_RC
      ": %d", DT_RC_GOOD));
  }

#if 0
  DT_C(dtdg_server_trailer,(ctl,		/* give html trailer 				*/
    dgserver));
#else
  DT_C(dtdg_server_response,(ctl, 
    dgserver, "</html>"));
  DT_C(dtdg_server_response,(ctl, 
    dgserver, ""));
#endif

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
textchat_request_poll(
  textchat_t *textchat,
  dtdg_server_t *dgserver)
{
  DT_F("textchat_request_poll");
  dt_ctl_t *ctl = textchat->ctl;
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
	  DT_C(textchat_dialog_poll,(		/* poll for keyboard input			*/
        textchat, &request)); 

	if (request == NULL)
	  DT_C(textchat_graphics_poll,(		/* poll for graphics event input	*/
        textchat, &request));

	if (request != NULL)				/* there is a user event?			*/
	{
	  DT_C(textchat_request_handle,(	/* handle the user request			*/
        textchat, NULL, request));
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
	    DT_C(textchat_request_handle,(	/* handle the ipc request			*/
          textchat, dgserver, request));
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
