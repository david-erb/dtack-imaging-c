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

#include <reverbi.h>
DT_RCSID("app/reverb $RCSfile: client.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
reverb_client(
  dt_ctl_t *ctl,
  const char *ipcspec,
  const char *ident,
  long timeout,
  char *host,
  int max)
{
  DT_F("reverb_client");
  dtipc_t _ipc, *ipc = &_ipc;
  char request[256];
  char reply[256];
  long got;
  dt_rc_e rc;

  ipc->type = DTIPC_TYPE_NONE;

  DT_C(dtipc_find,(ctl, ipc,			/* contact the server				*/
    ipcspec, 0, 0, timeout));

  dtstr_printf(ctl,						/* make request string 				*/
    request, sizeof(request),	
    "GET /add?%s HTTP/1.0\n\n", ident);

  DT_C(dtipc_write_all,(ctl, ipc,		/* output request string 			*/
    request, strlen(request)));

  DT_C(dtipc_read_line,(ctl, ipc,		/* read first line of reply 		*/
     reply, sizeof( reply)));

  if (reply[strlen(reply)-1] == '\n')	/* nuke the newline				    */
    reply[strlen( reply)-1] = '\0';

# define GOODNESS "HTTP/1.0 200"

  if (strncmp(reply, GOODNESS,			/* invalid return code? 			*/
        strlen(GOODNESS)))
    dt_err(ctl, F,
      "unexpected reply \"%s\"", reply);

  if (host != NULL && max != 0)
  {
	const char *q;
	q = strchr(ipc->client_spec, ',');	/* find the comma					*/
	if (q != NULL)
      q = ipc->client_spec;
	else
      q++;
	DT_C(dtstr_printf,(ctl,				/* use host from control connection	*/
      host, max, "%s", q));
  }
  else
    host = (char *)ipc->client_spec;

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "reverb server %s acknowledged"
    " %s->%s",
    ipcspec, ident, host);

  do {									/* politely listen to everything 	*/
	DT_C(dtipc_read_some,(ctl, ipc,		
      reply, sizeof(reply), &got));
  } while (got > 0);
 
cleanup:
  if (ipc->type != DTIPC_TYPE_NONE)
  {
    DT_I(dtipc_unfind,(ctl, ipc));		/* release resources				*/
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
