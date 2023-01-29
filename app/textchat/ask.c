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
DT_RCSID("app/textchat $RCSfile: ask.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 * ask audio server to do something and check the response
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_ask(							/* ask audio server something		*/
  textchat_t *textchat,
  const char *ident,					/* audio client ident 				*/
  const char *request,					/* the given request string 		*/
  dtdg_client_t *dgclient)				/* client object holding response 	*/
{
  DT_F("textchat_ask");
  dt_ctl_t *ctl = textchat->ctl;
  char spec[TEXTCHAT_SPEC_MAXLEN];
  int identified = 0;
  int is_available;
  const char *response;
  dt_rc_e rc;

  dgclient->initialized = 0;			/* we init this structure 			*/

  DT_C(textchat_ident_to_spec,(			/* make spec from ident 			*/
    textchat, ident, 
    spec, sizeof(spec)));
  identified = 1;
  
  DT_C(dtdg_client_init,(ctl,			/* send request to server 			*/
    dgclient, spec, request,
    TEXTCHAT_CONNECT_TIMEOUT));

  DT_C(dtdg_client_wait_header,(ctl,	/* wait for header to appear		*/
    dgclient,
    TEXTCHAT_HEADER_TIMEOUT,
    &is_available));
  if (!is_available)
  {
	rc = dt_err(ctl, F,
      "ustream %s (%s) not responding",
      ident, spec);
	goto cleanup;
  }
  
  DT_C(dtdg_client_get_header,(ctl,		/* pick off the header 				*/
    dgclient));

  DT_C(dtdg_client_get_response,(ctl,	/* pick off the response			*/
    dgclient, 
    TEXTCHAT_RESPONSE_TIMEOUT));

										/* ................................ */
  DT_C(dtdg_client_response_search,(	/* see if response has error		*/
    ctl, dgclient, DTDG_RESPONSE_ERROR,
    &response));
  
  if (response != NULL)					/* there is a server error? 		*/
  {
	rc = dt_err(ctl, F,
      DTDG_RESPONSE_ERROR ": %s",
      response);
	goto cleanup;
  }

										/* ................................ */
  DT_C(dtdg_client_response_search,(	/* see if response has error		*/
    ctl, dgclient, DTDG_RESPONSE_RC,
    &response));
  
  if (response != NULL &&
	  strcmp(response, "0"))			/* there is a non-zero rc?			*/
  {
	rc = dt_err(ctl, F,
      DTDG_RESPONSE_RC ": %s",
      response);
	goto cleanup;
  }

										/* ................................ */
  DT_C(dtdg_client_response_search,(	/* see if response has result		*/
    ctl, dgclient, DTDG_RESPONSE_RESULT,
    &response));
  
  if (response != NULL)					/* there is a server result? 		*/
	dt_dbg(ctl, F,  
      DT_DBG_MASK_REPLY,
      DTDG_RESPONSE_RESULT ": %s",
      response);

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (dgclient->initialized)
      DT_I(dtdg_client_uninit,(ctl,
        dgclient));

	if (identified)						/* partner is known? 				*/
	{
	  if (strstr(textchat->message,		/* it looks like a busy signal? 	*/
			"busy"))
  	    DT_I(dtstr_printf,(ctl,			/* reformat busy message			*/
          textchat->message,
          sizeof(textchat->message),
          TEXTCHAT_M_BUSY, ident))
	  else
  	    DT_I(dtstr_printf,(ctl,			/* just couldn't talk to it			*/
          textchat->message,
          sizeof(textchat->message),
          TEXTCHAT_M_OFFLINE, ident));
	}
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
