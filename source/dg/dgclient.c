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
  CHANGES
  19981203 DE added ability to send extra header fields
 *..........................................................................*/


/*..........................................................................
| NAME
| :dtdg_client: Function Group - datagram client object
|
| SUMMARY
| The datagram server object ~dgclient~ encapsulates the ability
| of a process to send a datagram request and get the reply.
|
| A companion object called ~dgserver~ provides the ability for a process
| to act as a server.
|
| Each instantiation of a client object represents a single datagram
| conversation.
| 
| From the client's point of view, the conversation goes like this:
| 1.	call dtdg_client_init() with the server spec and the request string
| 2.	call dtdg_client_get_header() to wait for incoming response header
| 3.	call dtdg_client_get_line() to get incoming data lines
| 		(assuming the data is formatted as text)
| 6.	go back to dtdg_client_uninit() to finish the conversation
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: dgclient.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/liner.h>
#include <dtack/str.h>
#include <dtack/os.h>
  
#define CR 0x0d
#define LF 0x0a
static char nl[3] = {CR, LF, '\0'};

/*..........................................................................
| NAME
| dtdg_client_init2() - initialize datagram client object
|
| SUMMARY
| Initialize the datagram client object ~dgclient~.
| Expect ~spec~ to the of the form #scheme,carrier#.
| Have an error if #scheme# is not one of a predefined set.
| Have an error if there is no server at #carrier# or the server
| refuses to talk.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_client_init2(						/* instantiate dtdg_client object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  const char *spec,						/* spec identifying server 			*/
  const char *request,					/* the request string 				*/
  dtlist_t *request_header_list,		/* list of request header fields 	*/
  long timeout)							/* maximum time to wait for accept 	*/
{
  DT_F("dtdg_client_init");
  int ipc_initialized = 0;
  int body_list_initialized = 0;
  int header_list_initialized = 0;
  dt_rc_e rc;

  DT_MEMSET(dgclient, 0, sizeof(*dgclient));
  dgclient->ipc = &dgclient->_ipc;		/* use built-in ipc space  			*/
  
  DT_C(dtipc_find,(ctl,					/* connect to server 				*/
    dgclient->ipc, spec, 0, 0, 
    timeout));
  ipc_initialized = 1;

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "request \"%s\"", request);

  DT_C(dtipc_write_all,(ctl,			/* emit the line 					*/
    dgclient->ipc,
    request, strlen(request)));

  DT_C(dtipc_write_all,(ctl,			/* emit the line termination 		*/
    dgclient->ipc,
    nl, strlen(nl)));

  if (request_header_list)				/* other header fields exist? 		*/
  {
	char line[1024];
	dtlist_elem_t *elem = 
	  DTLIST_ELEM_HEAD;
	while(1)
	{
	  DT_C(dtlist_next,(ctl,
        request_header_list,
        elem, &elem));
	  if (elem == NULL)
		break;
	  DT_C(dtstr_printf,(ctl, 
        line, sizeof(line)-2, "%s: %s",
        elem->name, elem->user));
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "request header \"%s\"", line);
	  strcat(line, nl);
	  DT_C(dtipc_write_all,(ctl,		/* emit the line 					*/
        dgclient->ipc,
        line, strlen(line)));
	}
  }
  
  DT_C(dtipc_write_all,(ctl,			/* emit the request termination		*/
    dgclient->ipc,
    nl, strlen(nl)));

  DT_C(dtlist_create,(ctl,				/* create list for response header 	*/
    &dgclient->header_list,
    "header_list"));
  header_list_initialized = 1;

  DT_C(dtlist_create,(ctl,				/* create list for response body 	*/
    &dgclient->body_list,
    "body_list"));
  body_list_initialized = 1;

  dgclient->initialized = 1;
cleanup:
  if (rc != DT_RC_GOOD)
  {
	dgclient->error = 1;				/* mark error on this connection 	*/
	if (body_list_initialized)
	  DT_I(dtlist_destroy,(ctl,
        &dgclient->body_list));
	if (header_list_initialized)
	  DT_I(dtlist_destroy,(ctl,
        &dgclient->header_list));
	if (ipc_initialized)
	  DT_I(dtipc_unfind,(ctl,
        dgclient->ipc));
	DT_MEMSET(dgclient, 0, sizeof(*dgclient));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_client_init(						/* instantiate dtdg_client object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  const char *spec,						/* spec identifying server 			*/
  const char *request,					/* the request string 				*/
  long timeout)							/* maximum time to wait for accept 	*/
{
  DT_F("dtdg_client_init");
  DT_Q(dtdg_client_init2,(ctl,
    dgclient, spec, request, 
    NULL,								/* no extra header fields 			*/
    timeout));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_client_wait_line() - wait for response line
|
| SUMMARY
| Wait for next response line from server.
| Wait a maximum of ~milliseconds~.
| Return ~*is_ready~ non-zero if a line can be read.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_client_wait_line(					/* receive single response line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  int *is_available)					/* returned ready status 			*/
{
  DT_F("dtdg_client_wait_line");

  DT_Q(dtipc_poll,(ctl,					/* check for input 					*/
    dgclient->ipc,
    DTIPC_POLL_READ,
    timeout, 
    "response line from server",
    is_available));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_client_wait_header() - wait for response header
|
| SUMMARY
| Wait for a complete response header from server.
| Wait a maximum of ~milliseconds~.
| Return ~*is_ready~ non-zero if the header is complete.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_client_wait_header(				/* wait for header to start			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  int *is_available)					/* returned ready status 			*/
{
  DT_F("dtdg_client_wait_header");

  DT_Q(dtipc_poll,(ctl,					/* check for input 					*/
    dgclient->ipc,
    DTIPC_POLL_READ,
    timeout, 
    "response header from server",
    is_available));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_client_get_response() - receive response body
|
| SUMMARY
| Read all response body lines.
| Expect the body to end when data is exhausted.
| Store the header fields within the object for later
| parsing and extraction.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_client_get_response(				/* receive response body			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient,				/* this dtdg_client object 			*/
  long timeout,
  long max,								/* maximum number of lines 			*/
  long *pgot)							/* number of lines gotten 			*/
{
  DT_F("dtdg_client_get_response");
  long got;
  char *line = NULL;
  dt_rc_e rc;

  for (got=0; max==0 || got<max; got++)
  {
	int ready;
	DT_C(dtdg_client_wait_line,(ctl,	/* wait for a new line to come in	*/
      dgclient, timeout, &ready));
	if (!ready)
	{
	  rc = dt_err(ctl, F, 
        "timeout waiting for"
        " response line");
	  goto cleanup;
	}
	DT_C(dtdg_get_line,(ctl,			/* get next line from server 		*/
      dgclient, &line));
	
	if (line == NULL)					/* server has ended conversation? 	*/
   	  break;

	DT_C(dtliner_line_list,(ctl,		/* add it to the response list		*/
      line, "", ":", 
      &dgclient->body_list));

	dt_dbg(ctl, F, DT_DBG_MASK_REPLY,
      "%s", line);

	DT_C(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
	line = NULL;
  }

cleanup:
  if (line != NULL)
	DT_I(dtos_free2,(ctl, line,			/* free line space 					*/
      F, "line"));
  if (rc == DT_RC_GOOD &&				/* good return code? 				*/
	  pgot != NULL)						/* and caller wants line count? 	*/
    *pgot = got;						/* return line got 					*/
  return rc;
}

/*..........................................................................
 * Have an error if field ~name~ is not found in response body.
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_client_response_search(			/* search field value for response	*/
  dt_ctl_t *ctl,
  dtdg_client_t *dgclient,
  const char *name,
  const char **value)
{
  DT_F("dtdg_client_response_search");
  dtlist_elem_t *elem;

  DT_Q(dtlist_search,(ctl,				/* search for state field 			*/
    &dgclient->body_list,
    name, &elem));
  if (elem == NULL)						/* no state in response? 			*/
	*value = NULL;
  else
    *value = (const char *)elem->user;

  return DT_RC_GOOD;
}

/*..........................................................................
 * Have an error if field ~name~ is not found in response body.
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_client_response_find(				/* find field value in response		*/
  dt_ctl_t *ctl,
  dtdg_client_t *dgclient,
  const char *name,
  const char **value)
{
  DT_F("dtdg_client_response_find");
  dtlist_elem_t *elem;

  DT_Q(dtlist_search,(ctl,				/* search for field 				*/
    &dgclient->body_list,
    name, &elem));
  if (elem == NULL)						/* no such field in response? 		*/
	return dt_err(ctl, F,
      "no %s field in response",
      name);
  *value = (const char *)elem->user;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_client_uninit() - release client object
|
| SUMMARY
| Terminate the conversation.
| Accept no further response header lines or data.
| Cause the server to be notified that the conversation is over.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_client_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_client_t *dgclient)				/* this dtdg_client object 			*/
{
  DT_F("dtdg_client_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (dgclient->initialized)
  {
	DT_I(dtlist_free,(ctl,				/* free response lines 				*/
      &dgclient->body_list));
	DT_I(dtlist_destroy,(ctl,
      &dgclient->body_list));
	DT_I(dtlist_free,(ctl,				/* free header lines 				*/
      &dgclient->header_list));
	DT_I(dtlist_destroy,(ctl,
      &dgclient->header_list));
#ifdef NONO
	DT_I(dtipc_write_all,(ctl,			/* emit the blank line				*/
      &dgclient->ipc,
      nl, strlen(nl)));
#endif
	DT_I(dtipc_unfind,(ctl, 
      dgclient->ipc));
  }
  DT_MEMSET(dgclient, 0, sizeof(*dgclient));
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
