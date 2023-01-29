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
  20000508 DE conditional unregister of ipc
  20000508 DE mark structure initialized
  19991226 DE free body list at uninit
  19991207 DE added ability to read posted data
 *..........................................................................*/


/*..........................................................................
| NAME
| :dtdg_server: Function Group - datagram server object
|
| SUMMARY
| The datagram server object ~dgserver~ encapsulates the ability
| of a process to receive datagram requests.
|
| A companion object called ~dgclient~ provides the ability for a process
| to make requests of a ~dgserver~.
| 
| From the server's point of view, the conversation goes like this:
| 1.	call dtdg_server_init()
| 2.	call dtdg_server_is_request() to poll for incoming requests
| 3.	call dtdg_server_get_request() to get incoming request string
| 4.	call dtdg_server_response() multiple times until response is complete
| 5.	call dtdg_server_done() to finish response
| 6.	go back to dtdg_server_is_request() step
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: dgserver.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/liner.h>
#include <dtack/dg.h>
#include <dtack/str.h>
#include <dtack/os.h>

#define CR 0x0d
#define LF 0x0a

/*..........................................................................
| NAME
| dtdg_url_to_spec() - convert url to ipc spec
|
| SUMMARY
| Analyze ~url~ and convert to a dtipc spec.
| Return ~spec~ as NULL if ~url~ is not of a recognized form.
| Expect the caller to free space for ~spec~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_url_to_spec(						/* convert url to ipc spec			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *url,						/* given possible url 				*/
  char **spec)							/* converted spec					*/
{
  DT_F("dtdg_url_to_spec");

  *spec = NULL;

  if (strncmp(url, "http://", 7))		/* doesn't look like http?			*/
    return DT_RC_GOOD;

  DT_Q(dtos_strdup3,(ctl, "rocket,",	/* http is always rocket 			*/
    url+7, "", spec));
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_server_init() - initialize datagram server object
|
| SUMMARY
| Initialize the datagram server object ~dgserver~.
| Use provided ipc connection.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_use(						/* instantiate dtdg_server object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  dtipc_t *ipc)							/* existing ipc connection 			*/
{
  DT_F("dtdg_server_use");
  dt_rc_e rc = DT_RC_GOOD;
  
  DT_MEMSET(dgserver, 0,				/* clear structure 					*/
    sizeof(*dgserver));

  dgserver->ipc = ipc;					/* use provided ipc 				*/

  DT_C(dtlist_create,(ctl,				/* create list for response header 	*/
    &dgserver->header_list,
    "header_list"));

  dgserver->initialized = 1;			/* mark structure initialized		*/
  
cleanup:
  if (rc != DT_RC_GOOD)
	DT_MEMSET(dgserver, 0,				/* clear structure 					*/
      sizeof(*dgserver));
  return rc;
}

/*..........................................................................
| NAME
| dtdg_server_init() - initialize datagram server object
|
| SUMMARY
| Initialize the datagram server object ~dgserver~.
| Expect ~spec~ to the of the form #scheme,carrier#.
| Have an error if #scheme# is not one of a predefined set.
| Have an error if there is already a server at #carrier#.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_init(						/* instantiate dtdg_server object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  const char *spec)						/* spec identifying this server 	*/
{
  DT_F("dtdg_server_init");
  int ipc_initialized = 0;
  int header_list_initialized = 0;
  int body_list_initialized = 0;
  char *spec2 = NULL;
  dt_rc_e rc;
  DT_MEMSET(dgserver, 0,				/* clear structure 					*/
    sizeof(*dgserver));
  DT_C(dtdg_url_to_spec,(ctl, spec,		/* make spec from url 				*/
    &spec2));
  if (spec2 != NULL)					/* spec was url? 					*/
    spec = spec2;
  dgserver->ipc = &dgserver->_ipc;		/* use built-in ipc space  			*/
  
  DT_C(dtipc_register,(ctl,				/* register as ipc server 			*/
    dgserver->ipc, spec, 0, 0));
  ipc_initialized = 1;
  dgserver->initialized = 1;

  DT_C(dtlist_create,(ctl,				/* create list for request header 	*/
    &dgserver->header_list,
    "header_list"));
  header_list_initialized = 1;

  DT_C(dtlist_create,(ctl,				/* create list for post body		*/
    &dgserver->body_list,
    "body_list"));
  body_list_initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (body_list_initialized)
	  DT_I(dtlist_destroy,(ctl,
        &dgserver->body_list));

	if (header_list_initialized)
	  DT_I(dtlist_destroy,(ctl,
        &dgserver->header_list));

	if (ipc_initialized)
	  DT_I(dtipc_unregister,(ctl,
        dgserver->ipc));

	DT_MEMSET(dgserver, 0,				/* clear structure 					*/
      sizeof(*dgserver));
  }

  if (spec2 != NULL)
    DT_I(dtos_free2,(ctl, spec2,
      F, "spec-from-url"));
  return rc;
}

/*..........................................................................
| NAME
| dtdg_server_is_request() - check if client request pending
|
| SUMMARY
| Return ~is_request~ as non-zero if a client is currently attempting
| to send a request.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_is_request(					/* check for client request pending */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  int *is_request)
{
  DT_F("dtdg_server_is_request");
  dt_rc_e rc;

  DT_C(dtipc_poll,(ctl,
    dgserver->ipc,
    DTIPC_POLL_ACCEPT,
    0, "connection from client",
    is_request));

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdg_server_read_request2() - receive incoming client request with timeout
|
| SUMMARY
| Allocate space for the client' request at ~*request~
| and place the request string there.
| Assume socket is connected and ready to read.
| If no line is available or timeout occurs, return ~request~ as NULL.
| Have no error in this case.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_read_request2(				/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,							/* timeout 							*/
  char **request)						/* the returned request string		*/
{
  DT_F("dtdg_server_read_request2");
  int l;
  dt_rc_e rc;

  *request = NULL;

  DT_C(dtdg_get_line2,(ctl, dgserver,	/* read the request line 			*/
    timeout, 
    DTDG_SERVER_REQUEST_MAXLEN,			/* max bytes to read 				*/
    request, NULL));

cleanup:
  if (rc != DT_RC_GOOD &&
	  *request != NULL)
  {
	DT_I(dtos_free2,(ctl, *request,
      F, "request"));
	*request = NULL;
  }

  return rc;
}

/*..........................................................................
| NAME
| dtdg_server_read_request() - receive incoming client request
|
| SUMMARY
| Allocate space for the client' request at ~*request~
| and place the request string there.
| Assume socket is connected and ready to read.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_read_request(				/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  char **request)						/* the returned request string		*/
{
  DT_F("dtdg_server_read_request");
  DT_Q(dtdg_server_read_request2,(
    ctl, dgserver, 
    -1,									/* no timeout 						*/
    request));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_server_get_request() - receive incoming client request
|
| SUMMARY
| Allocate space for the client' request at ~*request~
| and place the request string there.
| If no client is currently attempting to send a request, block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_get_request(				/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  char **request)						/* the returned request string		*/
{
  DT_F("dtdg_server_get_request");
  dt_rc_e rc;

  DT_C(dtipc_wait,(ctl,					/* accept the connection 			*/
    dgserver->ipc));

  DT_C(dtdg_server_read_request,(ctl,	/* read the request 				*/
    dgserver, request));

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdg_server_wait_request() - wait for client request 
|
| SUMMARY
| Allocate space for and received the next client ~*request~.
| Return NULL ~*request~ if nothing is available in ~timeout~ milliseconds.
| Expect the caller to free space allocated for ~*request~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_wait(						/* wait for client request			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  char **request)						/* returned request or NULL 		*/
{
  DT_F("dtdg_server_wait");
  int is_request;

  DT_Q(dtipc_poll,(ctl,					/* wait for connection 				*/
    dgserver->ipc,
    DTIPC_POLL_ACCEPT,
    timeout, "connection from client",
    &is_request));

  if (is_request)						/* connection available? 			*/
    DT_Q(dtdg_server_get_request,(ctl,	/* get the request 					*/
      dgserver, request));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_server_done() - end response
|
| SUMMARY
| Indicate end of the response to the client.
| No further response lines or data may be sent after this call.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_done(						/* finish response 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver)				/* this dtdg_server object 			*/
{
  DT_F("dtdg_server_done");
  char junk[256];
  if (dgserver->ipc->datahandle == -1)
	return DT_RC_GOOD;
#ifdef NONO
  while (1)
  {
	junk[0] = '\0';
	DT_Q(dtipc_read_line,(ctl,
      dgserver->ipc,
      junk, sizeof( junk)));
	if (strlen(junk) <= 2)				/* until final line has only CRLF	*/
  	  break;
  }
#endif
  DT_Q(dtipc_done,(ctl, 
    dgserver->ipc));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_server_uninit() - release server object
|
| SUMMARY
| Terminate server actions.
| Accept no further client requests.
| Cause all clients currently attempting to send requests to have errors.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_uninit(						/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver)				/* this dtdg_server object 			*/
{
  DT_F("dtdg_server_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (dgserver->initialized)
  {
	DT_I(dtlist_free,(ctl,				/* free posted body lines			*/
      &dgserver->body_list));
	DT_I(dtlist_free,(ctl,				/* free header lines 				*/
      &dgserver->header_list));
	DT_I(dtlist_destroy,(ctl,
      &dgserver->body_list));
	DT_I(dtlist_destroy,(ctl,
      &dgserver->header_list));
	if (dgserver->ipc)					/* supposed to release this? 		*/
	  DT_I(dtipc_unregister,(ctl, 
        dgserver->ipc));
  }
  DT_MEMSET(dgserver, 0, sizeof(*dgserver));
  return rc;
}

/*..........................................................................
| NAME
| dtdg_server_wait_line() - wait for post line
|
| SUMMARY
| Wait for next post line from server.
| Wait a maximum of ~milliseconds~.
| Return ~*is_ready~ non-zero if a line can be read.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_wait_line(					/* receive single post line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,							/* maximum milliseconds to wait 	*/
  int *is_available)					/* returned ready status 			*/
{
  DT_F("dtdg_server_wait_line");

  DT_Q(dtipc_poll,(ctl,					/* check for input 					*/
    dgserver->ipc,
    DTIPC_POLL_READ,
    timeout, 
    "post line from server",
    is_available));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtdg_server_get_post() - receive post body
|
| SUMMARY
| Read all post body lines.
| Expect the body to end when data is exhausted.
| Store the cgi argument fields within the object for later
| parsing and extraction.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_get_post(					/* receive post body				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  long timeout,
  long maxlines,						/* maximum number of lines 			*/
  long *pgot)							/* number of lines gotten 			*/
{
  DT_F("dtdg_server_get_post");
  long maxbytes;
  long gotlines;
  long gotbytes;
  char *line = NULL;
  const char *length;
  char *t;
  int i, n;
  dt_rc_e rc;

  DT_C(dtdg_header_find,(ctl,			/* how many bytes to extract 		*/
    dgserver,
    "content-length", &length));
  DT_C(dtstr_to_long,(ctl, length, 
    &maxbytes));

  DT_C(dtlist_free,(ctl,				/* free existing body lines		*/
    &dgserver->body_list));
  
  for (gotlines=0, gotbytes=0; 
	   gotbytes < maxbytes &&
	   (maxlines==0 || gotlines<maxlines);
	   gotlines++)
  {
	int ready;
	long nbytes;
	DT_C(dtdg_server_wait_line,(ctl,
      dgserver, timeout, &ready));
	if (!ready)
	{
	  rc = dt_err(ctl, F, 
        "timeout waiting for"
        " post line");
	  goto cleanup;
	}

	DT_C(dtdg_get_line2,(ctl,			/* get next line from server 		*/
      dgserver, timeout,
      maxbytes-gotbytes, 
      &line, &nbytes));
	
	gotbytes += nbytes;					/* total bytes read from connection */
	
	if (line == NULL)					/* server has ended conversation? 	*/
   	  break;

	t = line;
	DT_C(dtstr_tokenize,(ctl, t,
      '&', &n));
	for (i=0; i<n; i++)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "%s", t);
	  DT_C(dtliner_line_list,(ctl,		/* add it to the post list		*/
        t, "", "=", 
        &dgserver->body_list));
	  t = t + strlen(t) + 1;
	}


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
    *pgot = gotlines;					/* return lines got 				*/
  return rc;
}

/*..........................................................................
 * Have an error if field ~name~ is not found in post body.
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_server_post_search(				/* search field value for post		*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  const char *name,
  const char **value)
{
  DT_F("dtdg_server_post_search");
  dtlist_elem_t *elem;

  DT_Q(dtlist_search,(ctl,				/* search for state field 			*/
    &dgserver->body_list,
    name, &elem));
  if (elem == NULL)						/* no state in post? 				*/
	*value = NULL;
  else
    *value = (const char *)elem->user;

  return DT_RC_GOOD;
}

/*..........................................................................
 * Have an error if field ~name~ is not found in post body.
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtdg_server_post_find(					/* find field value in post			*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  const char *name,
  const char **value)
{
  DT_F("dtdg_server_post_find");
  dtlist_elem_t *elem;

  DT_Q(dtlist_search,(ctl,				/* search for field 				*/
    &dgserver->body_list,
    name, &elem));
  if (elem == NULL)						/* no such field in post? 			*/
	return dt_err(ctl, F,
      "no %s field in post",
      name);
  *value = (const char *)elem->user;

  return DT_RC_GOOD;
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
