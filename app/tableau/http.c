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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>
DT_RCSID("app/tableau $RCSfile: put_http.c,v $ $Revision: 1.8 $");

#define HTTP_CONNECT_TIMEOUT (param->epsilon)
#define HTTP_HEADER_TIMEOUT (param->epsilon)
#define HTTP_RESPONSE_TIMEOUT (param->epsilon)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_http_start(
  dtparam_t *param,
  const char *url,
  char **spec)
{
  DT_F("tableau_http_start");
  dt_ctl_t *ctl = &param->ctl;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  char *p;
  char request[128];
  int is_available;
  const char *response;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  dgclient->initialized = 0;			/* we init this structure 			*/

  DT_C(dtdg_url_to_spec,(ctl, url, 
    spec));

  p = strchr(*spec, '/');				/* find path part of url 			*/

  if (p == NULL)						/* no path part? 					*/
  {
	rc = dt_err(ctl, F,
      "no path in url %s", url);
	goto cleanup;
  }

  *p++ = '\0';							/* separate ipc spec from path 		*/

  DT_C(dtstr_printf,(ctl,				/* create client request 			*/
    request, sizeof(request),
    "GET /%s HTTP/1.0",
    p));

  DT_C(dtdg_client_init,(ctl,			/* send request to server 			*/
    dgclient, *spec, request,
    HTTP_CONNECT_TIMEOUT));

  DT_C(dtdg_client_wait_header,(ctl,	/* wait for header to appear		*/
    dgclient,
    HTTP_HEADER_TIMEOUT,
    &is_available));
  if (!is_available)
  {
	rc = dt_err(ctl, F,
      "url %s not responding", url);
	goto cleanup;
  }
  
										/* ................................ */
  DT_C(dtdg_header_get,(ctl,			/* pick off the header 				*/
    dgclient, HTTP_RESPONSE_TIMEOUT));
  DT_C(dtlist_next,(ctl,				/* examine first header line 		*/
    &dgclient->header_list,
    DTLIST_ELEM_HEAD, &elem));

  if (!strstr(elem->name, "200"))		/* invalid header? 					*/
  {
	rc = dt_err(ctl, F,
      "\"%s\" got response \"%s\"",
      p, elem->name);
	goto cleanup;
  }
										/* ................................ */

  DT_C(dtdg_client_get_response,(ctl,	/* pick off first response line		*/
    dgclient, 
    HTTP_RESPONSE_TIMEOUT, 1, NULL));

  DT_C(dtdg_client_response_search,(	/* see if response has error			*/
    ctl, dgclient, DTDG_RESPONSE_ERROR,
    &response));
  if (response != NULL)
  {
	rc = dt_err(ctl, F,
      "server says \"%s\"", response);
	goto cleanup;
  }

  DT_C(dtdg_client_response_find,(		/* see if response has spec			*/
    ctl, dgclient, "server_spec",
    &response));

  DT_C(dtos_free2,(ctl, *spec,
    F, "spec from url"));

  *spec = NULL;

  DT_C(dtos_strdup,(ctl, response,
    spec));

cleanup:
  if (dgclient->initialized)
    DT_I(dtdg_client_uninit,(ctl,
      dgclient));

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
