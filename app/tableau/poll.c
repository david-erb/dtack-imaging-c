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

#include <tableau.h>
DT_RCSID("app/tableau $RCSfile: server.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_poll_ident(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("tableau_poll_ident");
  tableau_t *tableau = (tableau_t *)arg;
  const char *rev_string;
  int rev_major;
  int rev_minor;
  long rev_date;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(tableau_rev,(ctl,
    &rev_string,
    &rev_major, &rev_minor,
    &rev_date));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "revision: %02d.%02d %d",
    rev_major, rev_minor,
    rev_date));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "-input: %s", tableau->param->input));
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "-screen: %s", tableau->param->screen));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_poll_state(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("tableau_poll_state");
  tableau_t *tableau = (tableau_t *)arg;
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "last_frame: %ld", tableau->last_frame));
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "last_size: %ld", tableau->last_size));
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "last_time: %0.1f", tableau->last_time));
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "last_cam: %s", tableau->last_cam));
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "current_quality: %g", tableau->param->t));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_poll_quality(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("tableau_poll_quality");
  tableau_t *tableau = (tableau_t *)arg;
  double quality;
  dt_rc_e rc = DT_RC_GOOD;
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "last_quality: %g", tableau->param->t));
  DT_Q(dtstr_to_double,(ctl, rest, &quality));
  tableau->param->t = quality;
  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    "current_quality: %g", tableau->param->t));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_poll_source(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("tableau_poll_source");
  tableau_t *tableau = (tableau_t *)arg;
  if (tableau == NULL)
    return dt_err(ctl, F, "tableau is NULL");

  if (tableau->get == NULL)
    return dt_err(ctl, F, "tableau->get is NULL");

  if (tableau->get->type != TABLEAU_GET_FB)
    return dt_err(ctl, F, "\"%s\" not valid for tableau->get.type %d",
      command, tableau->get->type);

  DT_Q(dtfb_source,(ctl, &tableau->get->u.fb.fb, rest));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, "new_source: %s", rest));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static const char *requests[] = {
  "ident",
  "state",
  "quality",
  "source",
  "exit",
  NULL
};

static dtdg_server_request_f *funcs[] = {
  tableau_poll_ident,
  tableau_poll_state,
  tableau_poll_quality,
  tableau_poll_source,
  dtdg_server_exit,
  NULL
};
  
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
tableau_poll_transact(
  tableau_t *tableau,
  dtdg_server_t *dgserver,
  char *request)
{
  DT_F("tableau_poll_transact");
  dt_ctl_t *ctl = tableau->ctl;
  dt_rc_e rc;
  dt_rc_e rc2 = DT_RC_GOOD;

  DT_C(dtdg_server_header_plain,(ctl,	/* give standard plain header 		*/
    dgserver, "Tableau"));

  ctl->user = (void *)tableau;			/* catch errors 					*/

  if (tableau->message[0] != '\0')		/* asynchronous message pending? 	*/
    strcpy(tableau->last_message,		/* report at next state request 	*/
      tableau->message);

  DT_G(dtdg_server_request,(ctl,		/* generate datagram response		*/
    dgserver, request,
    (void *)tableau,
    requests, funcs,
    tableau->message, 
    sizeof(tableau->message)))

  rc2 = rc;

  if (tableau->message[0] != '\0')		/* error handling request?			*/
  {
    DT_C(dtdg_server_response,(ctl,		/* tack on end of request 			*/
      dgserver, 
      DTDG_RESPONSE_ERROR
      ": %s", tableau->message));
    DT_C(dtdg_server_response,(ctl,		/* tack on end of request 			*/
      dgserver, 
      DTDG_RESPONSE_RC
      ": %d", DT_RC_BAD));
    strcpy(tableau->last_message,		/* report at next state request 	*/
      tableau->message);
	tableau->message[0] = '\0';
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
  ctl->user = NULL;						/* quit grabbing error messages		*/

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
tableau_poll(
  tableau_t *tableau,
  dtdg_server_t *dgserver,
  long delay)
{
  DT_F("tableau_poll");
  dt_ctl_t *ctl = tableau->ctl;
  dtos_time_t start;
  long elapsed;
  char *request = NULL;
  dt_rc_e rc;

  DT_C(dtos_time,(ctl, &start));
  
  do {
	dtos_time_t now;
	DT_C(dtos_time,(ctl, &now));		/* time at this moment 				*/
	elapsed = (now.wall - start.wall) *	/* elapsed since function called 	*/
      1000.0 + 0.5;
	if (elapsed > delay)				/* delayed enough? 					*/
	  elapsed = delay;					/* one final poll 					*/

	DT_C(dtdg_server_wait,(ctl,			/* wait for command input 			*/
      dgserver, delay-elapsed,
      &request));

	if (request != NULL)				/* there is work for us? 			*/
  	  DT_C(tableau_poll_transact,(		/* handle the command 				*/
        tableau, dgserver, request));
  } while(elapsed > delay);				/* delayed enough? 					*/

cleanup:
  if (request != NULL)
	DT_I(dtos_free2,(ctl, request,		/* free the datagram command 		*/
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
