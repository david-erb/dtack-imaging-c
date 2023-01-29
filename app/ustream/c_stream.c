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
DT_RCSID("app/ustream $RCSfile: c_stream.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

#include <dtstream.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
ustream_cmd_stream(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("ustream_cmd_stream");
  ustream_t *ustream = (ustream_t *)arg;
  int n;
  char *istream_spec;
  char *ostream_spec;
  char *permit;
  dt_rc_e rc = DT_RC_GOOD;

  ustream->message[0] = '\0';			/* clear error message 				*/
  ustream->last_message[0] = '\0';
										/* ................................ */
  dtstr_tokenize(ctl, rest, '&', &n);	/* split up rest of the line		*/
  if (n != 2 && n != 3)
  {
	rc = dt_err(ctl, F,
      "invalid argument \"%s\"",
      rest);
	goto cleanup;
  }

  istream_spec = rest;					/* get pieces from request 			*/
  rest = rest + strlen(rest) + 1;
  ostream_spec = rest;
  rest = rest + strlen(rest) + 1;
  if (n == 3)
	permit = rest;
  else
    permit = "none";

										/* ................................ */
  if (ustream->flag_locked)				/* we are locked? 					*/
  {
	rc = dt_err(ctl, F,
      "%s call rejected because locked",
      permit);
	goto cleanup;
  }

										/* ................................ */
  if (ustream->flag_shovel)				/* we are currently shoveling? 		*/
  {
    if (strcmp(ustream->permit, 		/* not same partner?				*/
          permit))
	{
	  rc = dt_err(ctl, F,
        "%s call rejected because busy"
        " with %s",
        permit, ustream->permit);
	  goto cleanup;
	}
	else								/* same partner? 					*/
	{
	  ustream->flag_shovel = 2;			/* freshen connection 				*/
	  DT_C(dtdg_server_response,(ctl, 
        dgserver,
        DTDG_RESPONSE_RESULT
        ": freshening from %s to %s,"
        " permitting %s", 
        ustream->istream_spec,
        ustream->ostream_spec,
        ustream->permit));
	}
  }
  else									/* not currently shoveling? 		*/
  {
	ustream->flag_shovel = 1;			/* start shoveling loop 			*/
	DT_C(dtdg_server_response,(ctl, 
      dgserver,
      DTDG_RESPONSE_RESULT
      ": will shovel from %s to %s,"
      " permitting %s", 
      ustream->istream_spec,
      ustream->ostream_spec,
      ustream->permit));
  }
  
										/* ................................ */
  if (!strncmp(istream_spec, "ipc", 3))	/* looks like input is ipc? 		*/
  {
	DT_C(dtstr_printf,(ctl,				/* ignore requested ipc spec		*/
      ustream->istream_spec,
      sizeof(ustream->istream_spec),
      "%s", 
      ustream->istream_preference));	/* in favor of given preference		*/

    DT_C(dtdg_server_response,(ctl,		/* reply where we will listen 		*/
      dgserver,
      DTDG_RESPONSE_ISTREAM_SPEC
      ": %s",
      ustream->istream_spec));
  }
  else									/* input is not ipc? 				*/
  {
	DT_C(dtstr_printf,(ctl,				/* keep copy of istream spec 		*/
      ustream->istream_spec,
      sizeof(ustream->istream_spec),
      "%s", istream_spec));
  }

										/* ................................ */
  DT_C(dtstr_printf,(ctl,				/* keep copy of ostream spec 		*/
    ustream->ostream_spec,
    sizeof(ustream->ostream_spec),
    "%s", ostream_spec));
  
  DT_C(dtstr_printf,(ctl,				/* keep copy of permit 				*/
    ustream->permit, 
    sizeof(ustream->permit),
    "%s", permit));

cleanup:

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
