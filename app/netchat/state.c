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
DT_RCSID("app/netchat $RCSfile: state.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/label.h>

/*..........................................................................
 * get audio buddy server's current state
 * don't update screen, just mark it if it needs redrawing
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_state(							/* get current audio state 			*/
  netchat_t *netchat)
{
  DT_F("netchat_state");
  dt_ctl_t *ctl = netchat->ctl;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  int is_available;
  const char *state;
  const char *stream_spec;
  const char *permit;
  const char *last_error;
  dt_rc_e rc;
  
  ctl->user = (void *)netchat;			/* capture errors 					*/

  DT_C(netchat_ask,(netchat,			/* request state 					*/
    netchat->ident,
    DTDG_REQUEST_STATE, 
    dgclient));

  DT_C(dtdg_client_response_find,(ctl,	/* find state in response 			*/
    dgclient, DTDG_RESPONSE_STATE,
    &state));

  if (DT_ABBREV(state,					/* server is locked?				*/
        DTDG_STATE_LOCKED))
  {
	if (netchat->state !=				/* first we heard about this 		*/
          NETCHAT_STATE_PRIVACY)
	{
	  netchat->state =					/* change display to show privacy	*/
        NETCHAT_STATE_PRIVACY;
	  netchat->update = 1;				/* redraw screen 					*/
	}
  }
  else									/* server is not locked?			*/
  if (DT_ABBREV(state,					/* server is idle? 					*/
        DTDG_STATE_WAITING))
  {
	if (netchat->state !=
          NETCHAT_STATE_IDLING)
	{
	  netchat->state =					/* server must be idling 			*/
        NETCHAT_STATE_IDLING;
	  netchat->update = 1;				/* redraw screen 					*/
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
	  if (netchat->state !=
            NETCHAT_STATE_LISTENING)
	  {
	    netchat->state =				/* server must be listening 		*/
          NETCHAT_STATE_LISTENING;
		netchat->update = 1;			/* redraw screen 					*/
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
		if (netchat->state !=
              NETCHAT_STATE_TALKING)
		{
		  netchat->state =				/* server must be talking 		*/
            NETCHAT_STATE_TALKING;
		  netchat->update = 1;			/* redraw screen 					*/
	    }
	  }
	  else
	  {
		rc = dt_err(ctl, F, 
          "%s is not an audio server",
          netchat->ident);
		goto cleanup;
	  }
	}
  }
										/* ................................ */
  DT_C(dtdg_client_response_find,(ctl,	/* find the partner					*/
    dgclient, DTDG_RESPONSE_PERMIT,
    &permit));

  if (strcmp(permit,					/* partner has changed? 			*/
        netchat->partner_contents) &&
	  strlen(permit) > 0)
  {
	DT_C(dtstr_printf,(ctl,				/* copy current partner 			*/
      netchat->partner_contents,
      sizeof(netchat->partner_contents),
      "%s", permit));
	netchat->update = 1;				/* cause screen to redraw 			*/
  }
										/* ................................ */
  DT_C(dtdg_client_response_search,(	/* find last error, if any			*/
    ctl, dgclient, 
    DTDG_RESPONSE_LASTERROR,
    &last_error));

  if (last_error != NULL &&				/* there is a last error? 			*/
	  strcmp(last_error,				/* and it is a new one? 			*/
        netchat->last_error) &&
	  strcmp(last_error, "none") &&		/* and it's not something benign 	*/
	  strcmp(last_error, ""))
  {
	char caller[32];
	char *p = strchr(last_error, ' ');	/* extract caller from message 		*/
	int l = DT_MIN(sizeof(caller)-1,
      p != NULL? 
        p-last_error: 
        strlen(last_error));
	memcpy(caller, last_error, l);
	caller[l] = '\0';

	if (strstr(last_error, "busy") ||	/* this is a busy message? 			*/
		strstr(last_error, "locked"))	/* this is a privacy message?		*/
  	  DT_C(dtstr_printf,(ctl,			/* translate error for display		*/
        netchat->message_contents,
        sizeof(netchat->message_contents),
        NETCHAT_M_CALLWAITING, caller))
	else
	if (strstr(last_error,				/* other end hung up?				*/
          DT_LABEL_HANGUP))
  	  DT_C(dtstr_printf,(ctl,			/* translate error for display		*/
        netchat->message_contents,
        sizeof(netchat->message_contents),
        NETCHAT_M_HANGUP, caller))
	else
  	  DT_C(dtstr_printf,(ctl,			/* display error on screen			*/
        netchat->message_contents,
        sizeof(netchat->message_contents),
        "anomalous condition logged"));

	DT_C(dtstr_printf,(ctl,				/* remember the last error			*/
      netchat->last_error,
      sizeof(netchat->last_error),
        "%s", last_error));
	netchat->update = 1;				/* cause screen to redraw 			*/
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
