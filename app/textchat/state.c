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
DT_RCSID("app/textchat $RCSfile: state.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 * get audio buddy server's current state
 * don't update screen, just mark it if it needs redrawing
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_state(							/* get current audio state 			*/
  textchat_t *textchat)
{
  DT_F("textchat_state");
  dt_ctl_t *ctl = textchat->ctl;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  int is_available;
  const char *state;
  const char *stream_spec;
  const char *permit;
  const char *last_error;
  dt_rc_e rc;
  
  ctl->user = (void *)textchat;			/* capture errors 					*/

  DT_C(textchat_ask,(textchat,			/* request state 					*/
    textchat->ident,
    DTDG_REQUEST_STATE, 
    dgclient));

  DT_C(dtdg_client_response_find,( ctl,	/* find state in response 			*/
    dgclient, DTDG_RESPONSE_STATE,
    &state));

  if (DT_ABBREV(state,					/* server is idle? 					*/
        DTDG_STATE_WAITING))
  {
	if (textchat->state !=
          TEXTCHAT_STATE_IDLING)
	{
	  textchat->state =					/* server must be idling 			*/
        TEXTCHAT_STATE_IDLING;
	  textchat->update = 1;				/* redraw screen 					*/
	}
  }
  else									/* server is not idle? 				*/
  if (DT_ABBREV(state,
        DTDG_STATE_SHOVELING))
  {
    DT_C(dtdg_client_response_find,(	/* find server's output spec		*/
      ctl,
      dgclient, 
      DTDG_RESPONSE_OSTREAM_SPEC,
      &stream_spec));
	if (DT_ABBREV(stream_spec, "audio")) /* output spec is audio? 			*/
	{
	  if (textchat->state !=
            TEXTCHAT_STATE_LISTENING)
	  {
	    textchat->state =				/* server must be listening 		*/
          TEXTCHAT_STATE_LISTENING;
		textchat->update = 1;			/* redraw screen 					*/
	  }
	}
	else								/* output spec is not audio? 		*/
	{
	  DT_C(dtdg_client_response_find,(	/* find server's input spec 		*/
        ctl, dgclient, 
        DTDG_RESPONSE_ISTREAM_SPEC,
        &stream_spec));
	  if (DT_ABBREV(stream_spec,		/* input spec is audio? 			*/
            "audio"))	
	  {
		if (textchat->state !=
              TEXTCHAT_STATE_TALKING)
		{
		  textchat->state =				/* server must be talking 		*/
            TEXTCHAT_STATE_TALKING;
		  textchat->update = 1;			/* redraw screen 					*/
	    }
	  }
	  else
	  {
		rc = dt_err(ctl, F, 
          "%s is not an audio server",
          textchat->ident);
		goto cleanup;
	  }
	}
  }
										/* ................................ */
  DT_C(dtdg_client_response_find,(ctl,	/* find the partner					*/
    dgclient, DTDG_RESPONSE_PERMIT,
    &permit));

  if (strcmp(permit,					/* partner has changed? 			*/
        textchat->partner_contents) &&
	  strlen(permit) > 0)
  {
	DT_C(dtstr_printf,(ctl,				/* copy current partner 			*/
      textchat->partner_contents,
      sizeof(textchat->partner_contents),
      "%s", permit));
	textchat->update = 1;				/* cause screen to redraw 			*/
  }
										/* ................................ */
  DT_C(dtdg_client_response_search,(	/* find last error, if any			*/
    ctl, dgclient, 
    DTDG_RESPONSE_LASTERROR,
    &last_error));

  if (last_error != NULL &&				/* there is a last error? 			*/
	  strcmp(last_error,				/* and it is a new one? 			*/
        textchat->message_contents) &&
	  strcmp(last_error, "none") &&		/* and it's not something benign 	*/
	  strcmp(last_error, ""))
  {
	if (strstr(last_error, "busy"))		/* this is a busy message? 			*/
	{
	  char caller[32];
	  char *p = strchr(last_error, ' ');
	  int l = DT_MIN(sizeof(caller)-1,
        p != NULL? 
          p-last_error: 
          strlen(last_error));
	  memcpy(caller, last_error, l);
	  caller[l] = '\0';
  	  DT_C(dtstr_printf,(ctl,			/* display error on screen			*/
        textchat->message_contents,
        sizeof(textchat->message_contents),
        TEXTCHAT_M_CALLWAITING, caller));
	}
	else
  	  DT_C(dtstr_printf,(ctl,			/* display error on screen			*/
        textchat->message_contents,
        sizeof(textchat->message_contents),
        "%s", last_error));
	textchat->update = 1;				/* cause screen to redraw 			*/
  }

cleanup:
  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl,
      dgclient));

  ctl->user = NULL;						/* quit capturing errors 			*/

  if (rc != DT_RC_STOP)					/* ignore errors from this function */
    rc = DT_RC_GOOD;

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
