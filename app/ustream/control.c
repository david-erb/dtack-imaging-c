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
DT_RCSID("app/ustream $RCSfile: control.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e 
DTCONFIG_API1
ustream_op_control(
  dtparam_t *param)
{
  DT_F("ustream_op_control");
  dt_ctl_t *ctl = &param->ctl;
  char *request = NULL;
  int request_length;
  dtdgclient_t _dgclient, *dgclient = &_dgclient;
  char *line;
  long mask = DT_DBG_MASK_REPLY;
  int l, n;
  dt_rc_e rc;

  request_length = 32 + 
    strlen(param->input);

  DT_C(dtos_malloc2,(ctl,				/* space for request string 		*/
    (void **)&request, 
    request_length, 
    F, "request"));

  DT_C(dtstr_printf,(ctl,				/* prepend http GET 				*/
    request, request_length,
    "GET /%s", param->input));

  DT_C(dtdgclient_init,(ctl, dgclient,	/* send request to server 			*/
    param->connect, request));
  
  DT_C(dtdgclient_get_header,(ctl,		/* pick off the header 				*/
    dgclient));

  n = 0;
  do {
	DT_C(dtdgclient_get_line,(ctl,
      dgclient, &line));
	l = strlen(line);
	if (l > 0)
	{
  	  dt_dbg(ctl, F, mask, "%s", line);
	  n++;
	}
    DT_I(dtos_free2,(ctl, line,
      F, "line"));
  } while (l > 0);

  if (n == 0)
    dt_dbg(ctl, F, mask,
      "%s", "no lines in response");

cleanup:
  if (request != NULL)
    DT_I(dtos_free2,(ctl, request,
      F, "request"));
  DT_I(dtdgclient_uninit,(ctl,
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
