/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: reply.c,v $ $Revision: 1.8 $");

static char nl[3] = {0x0d, 0x0a, '\0'};

/*..........................................................................
| NAME
| Introduction - intro
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabchief_reply(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *format,
  ...)
{
  dt_ctl_t *ctl = &param->ctl;
    tabchief_t *tabchief = 
      (tabchief_t *)ctl->user;
  char line[512];
  dt_rc_e rc;

  va_list arglist;
  va_start(arglist, format);

  if (tabchief != NULL &&
	  tabchief->reply_rc != DT_RC_GOOD)	/* previous reply failed? 			*/
    return DT_RC_GOOD;

  if (ipc == NULL)
    return DT_RC_GOOD;

  dtstr_printf_va(ctl, line,			/* make a complete string 			*/
    sizeof(line)-sizeof(nl),
    format, arglist);
  va_end(arglist);

  strcat(line, nl);
  rc = dtipc_write_all(ctl, ipc,		/* emit to socket					*/
    line, strlen(line));
  if (tabchief != NULL)
    tabchief->reply_rc = rc;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabchief_reply_header(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *title,
  const char *host)
{
  dt_ctl_t *ctl = &param->ctl;
  char ipaddr[32];

  if (host[0] != '\0')
    dtstr_printf(ctl, ipaddr, sizeof(ipaddr),
      " at %s", host);
  else
    ipaddr[0] = '\0';

  tabchief_reply(param, ipc, "HTTP/1.0 200 OK");
  tabchief_reply(param, ipc, "Server: Tabchief by Tableau Web");
  tabchief_reply(param, ipc, "Content-type: text/html");
  tabchief_reply(param, ipc, "");
  tabchief_reply(param, ipc, "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">");
  tabchief_reply(param, ipc, "<html>");
  tabchief_reply(param, ipc, "<head>");
  tabchief_reply(param, ipc, "<title>");

  tabchief_reply(param, ipc, "Video Caesar %s%s", title, ipaddr);
  tabchief_reply(param, ipc, "</title>");
  tabchief_reply(param, ipc, "</head>");

  tabchief_reply(param, ipc, "<body bgcolor=e5e5d0>");

  tabchief_reply(param, ipc, "<a href=\"/\" target=\"_top\">");
  tabchief_reply(param, ipc, "<img align=middle"
    " src=\"/gif?tableau1.gif\" alt=\"Video Caesar Home\">");
  tabchief_reply(param, ipc, "</a>");

  tabchief_reply(param, ipc, "<font size=+2>"
    "Video Caesar %s%s"
    "</font><br>",
    title, ipaddr);
				 

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
tabchief_reply_trailer(
  dtparam_t *param,
  dtipc_t *ipc)
{
  tabchief_reply(param, ipc, "<p>");
  tabchief_reply(param, ipc, "<a href=\"/\" target=\"_top\">");
  tabchief_reply(param, ipc, "Return to Main Page");
  tabchief_reply(param, ipc, "</a>");
  tabchief_reply(param, ipc, "</body>");
  tabchief_reply(param, ipc, "</html>");
  tabchief_reply(param, ipc, "");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
tabchief_reply_message(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *message)
{
  tabchief_reply(param, ipc, 
    "<p><b><font size=+2>"
    "%s"
    "</font></b><p>",
    message);
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
