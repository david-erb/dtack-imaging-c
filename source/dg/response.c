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

#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: response.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/str.h>

#define CR 0x0d
#define LF 0x0a
static char nl[3] = {CR, LF, '\0'};

/*..........................................................................
| NAME
| dtdg_server_response() - send response line to client
|
| SUMMARY
| Build a response line according to ~format~ and variable argument list ~ap~.
| Send the response line to the client.
| This function may be called multiple times as part of a single conversation.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtdg_server_response(					/* emit response line 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  const char *format,
  ...)
{
  DT_F("dtdg_server_response");
  va_list ap;
  va_start(ap, format);
  DT_Q(dtdg_server_response_va,(ctl,
    dgserver, format, ap));
  va_end(ap);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_response_va(				/* emit response line 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_server_t *dgserver,				/* this dtdg_server object 			*/
  const char *format,
  va_list ap)
{
  DT_F("dtdg_server_response_va");
  char line[256];
  dt_rc_e rc;

  if (dgserver == NULL)
    return DT_RC_GOOD;

  if (dgserver->error)					/* previous error?					*/
	return DT_RC_GOOD;

  DT_Q(dtstr_printf_va,(ctl,
    line, sizeof(line)-strlen(nl),
    format, ap));

  strcat(line, nl);						/* tack on line termination 		*/
  
  DT_G(dtipc_write_all,(ctl,			/* emit the line 					*/
    dgserver->ipc,
    line, strlen(line)));

  if (rc != DT_RC_GOOD)					/* error writing? 					*/
	dgserver->error = 1;				/* mark error on this connection 	*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_header_plain(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver,
  const char *title)
{
  DT_F("dtdg_server_header_plain");
  DT_Q(dtdg_server_response,(ctl, dgserver, "HTTP/1.0 200 OK"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "Content-type: text/plain"));
  DT_Q(dtdg_server_response,(ctl, dgserver, ""));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_header_html(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver,
  const char *title)
{
  DT_F("dtdg_server_header_html");
  DT_Q(dtdg_server_response,(ctl, dgserver, "HTTP/1.0 200 OK"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "Server: dtdgserver"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "Content-type: text/html"));
  DT_Q(dtdg_server_response,(ctl, dgserver, ""));
  DT_Q(dtdg_server_response,(ctl, dgserver, "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "<html>"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "<head>"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "<title>"));

  DT_Q(dtdg_server_response,(ctl, dgserver, "%s", title));
  DT_Q(dtdg_server_response,(ctl, dgserver, "</title>"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "</head>"));

  DT_Q(dtdg_server_response,(ctl, dgserver, "<body bgcolor=e5e5d0>"));

  DT_Q(dtdg_server_response,(ctl, dgserver, "<font size=+2>"
    "page title: %s"
    "</font><br>",
    title));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_trailer_plain(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_trailer_html(
  dt_ctl_t *ctl,				   
  dtdg_server_t *dgserver)
{
  DT_F("dtdg_server_trailer_html");
  DT_Q(dtdg_server_response,(ctl, dgserver, "<p>"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "<a href=\"/\" target=\"_top\">"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "Return to Main Page"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "</a>"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "</body>"));
  DT_Q(dtdg_server_response,(ctl, dgserver, "</html>"));

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
