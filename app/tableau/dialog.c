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
DT_RCSID("app/tableau $RCSfile: dialog.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_dialog(							/* hit the done url					*/
  dtparam_t *param,
  const char *mode,						/* what's happening 				*/
  const char *client,
  long seconds,
  long frames,
  long kbytes,
  const char *keyword)					/* keyword we need to go 			*/
{
  DT_F("tableau_dialog");
  dt_ctl_t *ctl = &param->ctl;
  char *format = NULL;
  char *request = NULL;
  int request_length;
  char *url = NULL;
  char *filepart;
  char *donestring = "/done?";			/* default url file part 			*/
  dtdg_client_t _dgclient;
  dtdg_client_t *dgclient = &_dgclient;
  int client_initialized = 0;
  int is_available;
  long mask = DT_DBG_MASK_REPLY;
  int i;
  int pid;
  dt_rc_e rc = DT_RC_GOOD;

  if (param->dialog == NULL ||			/* no done url specified? 			*/
	  strlen(param->dialog) == 0)
	goto cleanup;

  request_length = 256 +				/* extra space for HTTP stuff 		*/
    strlen(param->dialog);

  DT_C(dtos_malloc2,(ctl,				/* space for request string 		*/
    (void **)&request, 
    request_length, 
    F, "request"));

  DT_C(dtos_strdup,(ctl, param->dialog, /* a place to edit 					*/
    &url));
  filepart = strchr(url, '/');			/* search for slash					*/
  if (filepart == NULL)					/* no slash? 						*/
	filepart = donestring;				/* default CGI string 				*/

  DT_C(dtos_strdup3,(ctl,
    "GET ", filepart, " HTTP/1.0",
    &format));
  DT_C(dtos_getpid,(ctl, &pid));
  DT_C(dtstr_printf,(ctl,				/* prepend http GET 				*/
    request, request_length,
    format,
    mode,
    param->ident? param->ident: "unknown",
    client? client: "unknown",
    seconds, frames, kbytes, pid));

  if (filepart != donestring)			/* need to split url? 				*/
    *filepart = '\0';

  DT_C(dtdg_client_init,(ctl, dgclient,	/* send request to server 			*/
    url, request, param->epsilon)); 

  DT_C(dtdg_client_wait_header,(ctl,	/* wait for header to appear		*/
    dgclient, param->epsilon, 
    &is_available));
  client_initialized = 1;
  if (!is_available)
  {
	rc = dt_err(ctl, F,
      "server %s not responding", url);
	goto cleanup;
  }
  
  DT_C(dtdg_header_get,(ctl,			/* pick off the header 				*/
    dgclient, -1));
  
  DT_C(dtdg_client_get_response,(ctl,	/* pick off the response			*/
    dgclient, param->dt,
    100, NULL));

  if (keyword)
  {
	const char *vgot;

    DT_C(dtdg_client_response_search,(ctl, 
      dgclient, keyword, &vgot));

	if (!vgot)
    DT_C(dtdg_client_response_search,(ctl, 
      dgclient, keyword, &vgot));
	
	if (!vgot)
	{
	  rc = dt_err(ctl, F,
        "keyword \"%s\" does not appear"
        " in response header or body",
        keyword);
	  goto cleanup;
	}
	  
  }
  
  if (!strcmp(mode, TABLEAU_DIALOG_MODE_STOP))
  dt_dbg(ctl, NULL, DT_DBG_MASK_USR1,
    "%s %lds %ldf %ldkb",
    param->ident,
    seconds, frames, kbytes);

cleanup:
  if (format != NULL)
    DT_I(dtos_free2,(ctl, format,
      F, format));
  if (request != NULL)
    DT_I(dtos_free2,(ctl, request,
      F, request));
  if (url != NULL)
    DT_I(dtos_free2,(ctl, url,
      F, url));
  if (client_initialized)
    DT_I(dtdg_client_uninit ,(ctl,
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
