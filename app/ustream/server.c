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

#include <ustream.h>
DT_RCSID("app/ustream $RCSfile: server.c,v $ $Revision: 1.8 $");

static const char *requests[] = {
  "ident",
  "state",
  "stream",
  "lock",
  "cease",
  "exit",
  NULL
};
static dtdg_server_request_f *funcs[] = {
  ustream_cmd_ident,
  ustream_cmd_state,
  ustream_cmd_stream,
  ustream_cmd_lock,
  ustream_cmd_cease,
  dtdg_server_exit,
  NULL
};
  
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_server_transact(
  ustream_t *ustream,
  dtdg_server_t *dgserver)
{
  DT_F("ustream_server_transact");
  dt_ctl_t *ctl = ustream->ctl;
  char *request = NULL;
  dt_rc_e rc;
  dt_rc_e rc2 = DT_RC_GOOD;

  DT_C(dtdg_server_get_request,(		/* receive the datagram request		*/
    ctl, dgserver, &request));

  if (request == NULL)					/* no request available? 			*/
    goto cleanup;

  DT_C(dtdg_server_header_plain,(ctl,	/* give standard plain header 		*/
    dgserver, "Ustream"));

  if (ustream->message[0] != '\0')		/* asynchronous message pending? 	*/
    strcpy(ustream->last_message,		/* report at next state request 	*/
      ustream->message);

  DT_C(dtdg_server_response,(ctl,		/* ident ourselves in response 		*/
    dgserver, 
    DTDG_RESPONSE_IDENT
    ": %s", ustream->ident));

  DT_G(dtdg_server_request,(ctl,		/* generate datagram response		*/
    dgserver, request,
    (void *)ustream,
    requests, funcs,
    ustream->message, 
    sizeof(ustream->message)))

  rc2 = rc;

  if (ustream->message[0] != '\0')		/* error handling request?			*/
  {
    DT_C(dtdg_server_response,(ctl,		/* tack on end of request 			*/
      dgserver, 
      DTDG_RESPONSE_ERROR
      ": %s", ustream->message));
    DT_C(dtdg_server_response,(ctl,		/* tack on end of request 			*/
      dgserver, 
      DTDG_RESPONSE_RC
      ": %d", DT_RC_BAD));
    strcpy(ustream->last_message,		/* report at next state request 	*/
      ustream->message);
	ustream->message[0] = '\0';
  }
  else
    DT_C(dtdg_server_response,(ctl,		/* tack on end of request 			*/
      dgserver, 
      DTDG_RESPONSE_RC
      ": %d", DT_RC_GOOD));

  DT_C(dtdg_server_trailer_plain,(ctl,	/* give html trailer 				*/
    dgserver));

  DT_C(dtdg_server_done,(ctl,			/* finish response to the datagram	*/
    dgserver));

cleanup:
  if (request != NULL)
	DT_I(dtos_free2,(ctl, request,		/* free the datagram command 		*/
      F, "request"));

  if (rc2 == DT_RC_STOP)
    rc = rc2;
  else
    rc = DT_RC_GOOD;
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_server_check(
  ustream_t *ustream,
  dtdg_server_t *dgserver)
{
  DT_F("ustream_server_check");
  dt_ctl_t *ctl = ustream->ctl;
  int is_request;
  dt_rc_e rc;

  while (1)
  {
	DT_C(dtdg_server_is_request,(ctl,	/* check state of command input 	*/
      dgserver, &is_request));
	if (!is_request)					/* there is work for us? 			*/
	  break;
	DT_C(ustream_server_transact,(		/* handle the command 				*/
      ustream, dgserver));
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_op_server(
  dtparam_t *param)
{
  DT_F("ustream_shovel");
  dt_ctl_t *ctl = &param->ctl;
  ustream_t _ustream, *ustream = &_ustream;
  dtdg_server_t _dgserver, *dgserver = &_dgserver;
  dt_rc_e rc;

  dgserver->initialized = 0;

  DT_C(ustream_init,(ustream, param));

  DT_C(dtdg_server_init,(ctl, dgserver,	/* become a datagram server 		*/
    ustream->dgserver_spec));

  DT_C(dtos_time,(ctl, 
    &ustream->time));
  ustream_state(ustream,
    DTDG_STATE_WAITING);
  
  ctl->user = (void *)ustream;			/* catch errors 					*/

  while(1)
  {
	DT_C(ustream_server_check,(ustream,	/* handle datagram commands			*/
      dgserver));
	if (ustream->flag_shovel)			/* supposed to start shoveling? 	*/
	{
	  while (ustream->flag_shovel)
		DT_C(ustream_shovel,(ustream,	/* shovel until streams end			*/
          dgserver));
	}
	else								/* no stream data available? 		*/
      DT_C(dtos_delay,(ctl, 
        ustream->poll_period));
  }
  
cleanup:
  ctl->user = NULL;						/* quit grabbing error messages		*/
  
  DT_I(dtdg_server_uninit,(ctl,			/* quit being a datagram server 	*/
    dgserver));
  DT_I(ustream_uninit,(ustream));
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
