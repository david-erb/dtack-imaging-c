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
| USAGE
| reverb - client/server IP address mapper
| 
|.SYNOPSIS
|:reverb [params] -connect host:port -cache file -ident name
|
|.SUMMARY
| Stream load and show full-motion video from the Reverb Internet server.
| 
|.PARAMETERS
| :-connect:	connection spec (required for either client or server)
| :-cache:		cache file (required for server)
| :-ident:		cient name (required for client)
| :-reps:		number of times to repeat (default 0 means forever)
| :-dt:			delay in seconds between repeats (default 30)
|dtinclude(params1.dx)
|
|.EXAMPLES
| Server:
|:  reverb -connect 156.46.122.16:5300 -cache /var/reverb.cache
| 
| Client:
|:  reverb -connect 156.46.122.16:5300 -name myname
|
|.ADVANTAGES
| -	Does not disturb the existing DNS server.
| 
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <reverbi.h>
DT_RCSID("reverb $RCSfile: reverb.c,v $ $Revision: 1.8 $");

#include <dtack/main.h>

#include <reverb.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define CR (0x0d)
#define LF (0x0a)

static char nl[2] = {'\n', '\0'};

#define REQUEST_MAX (8192)

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

static
int
reverb_split(							/* separate string into tokens 		*/
  char *p,								/* string changed in place 			*/
  char c)								/* separator character 				*/
{
  char *q = p;
  int i;
  int l = strlen(p) + 1;				/* q needs the EOS too 				*/
  int n = 0;
  for (i=0; i<l; i++)
  {
    if (p[i] == c)
	{
      *q++ = '\0';
	  n++;
	}
	else
    if (p[i] == '+')
	{
      *q++ = ' ';
	}
	else
    if (p[i] == '%' && p[i+1] != '%')
	{
	  char t[3];
	  t[0] = p[i+1];
	  t[1] = p[i+2];
	  t[2] = '\0';
      *q++ = (char)strtol(t, 0, 16);
	  i += 2;
	}
	else
    if (p[i] == '%' && p[i+1] == '%')
	{
      *q++ = '%';
	  i += 1;
	}
	else
	  *q++ = p[i];
  }
  return n + 1;
}

/*..........................................................................
 *..........................................................................*/

static
void
reverb_stamp(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  char *s,
  int lmax)
{
  int pid;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/
  strftime(s, lmax,						/* express as string 				*/
    "%Y%m%d.%H%M.%S", t);

  dtos_getpid(ctl, &pid);

  dtstr_printf(ctl,						/* tack on the caller name and pid	*/
    s+strlen(s), lmax-strlen(s),
    " [%5d]%s%s", pid, 
    caller? " ": "",
    caller? caller: "");
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
reverb_dbg(								/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  reverb_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = reverb_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
reverb_err(								/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  reverb_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,
    format, ap);
  ctl->err = reverb_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
reverb_say(								/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  reverb_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = reverb_say;				/* come back here next time 		*/
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_entry_alloc(
  dtparam_t *param,
  reverb_entry_t **entry)
{
  DT_F("reverb_entry_alloc");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dtos_malloc,(ctl,
    (void **)entry, sizeof(**entry)));
  memset(*entry, 0, sizeof(**entry));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_cache_read(
  dtparam_t *param,
  const char *cachespec,
  dtlist_t *list)
{
  DT_F("reverb_cache_read");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  reverb_entry_t *entry;
  int n, l, m, i;
  char s[REVERB_NAME_MAX+32];
  dt_rc_e rc;

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd, cachespec,	/* open cache file for reading 		*/
    "r"));
  l = 0;
  m = 0;
  while(1)
  {
	char ipaddr[REVERB_IPADDR_MAX];
	DT_C(dtfd_read_line,(ctl, fd,		/* read line from file 				*/
      s, sizeof(s)));
	if (strlen(s) == 0)					/* end of file? 					*/
	  break;

	l++;								/* increment line count				*/

	for (i=0; isspace(s[i]); i++);		/* skip leading white space 		*/
	if (s[i] == ';' ||					/* weed out comments 				*/
		s[i] == '#' ||
		s[i] == '*')
	  continue;
	
	DT_C(reverb_entry_alloc,(param,		/* make a new entry					*/
      &entry));
	n = sscanf(s, "%s %s %ld %ld %ld %ld %ld\n",
      entry->name, 
      entry->ipaddr,
      &entry->created_time,
      &entry->recreated_time,
      &entry->recreated_count,
      &entry->accessed_time,
      &entry->accessed_count);
	if (n != 7)
	{
	  rc = dt_err(ctl, F,
        "invalid cache data in %s"
        " on line %d",
        cachespec, l);
	  goto cleanup;
	}
	if (s[strlen(s)-1] != '\n')
	{
	  rc = dt_err(ctl, F,
        "line too long in %s"
        " on line %d",
        cachespec, l);
	  goto cleanup;
	}

	DT_C(dtlist_after,(ctl, list,		/* add new association to list	    */
      DTLIST_ELEM_HEAD, 
      entry->name, (void *)entry,
      NULL));
	
	m++;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "read %d associations from %s",
    m, cachespec);

cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));
  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_cache_write(
  dtparam_t *param,
  const char *cachespec,
  dtlist_t *list)
{
  DT_F("reverb_cache_write");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  char s[256];
  dtlist_elem_t *elem;
  reverb_entry_t *entry;
  dt_rc_e rc;

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd, cachespec,	/* open cache file for writing 		*/
    "w"));
  dtstr_printf(ctl, s, sizeof(s),
    "%20s %15s  %10s %21s  %21s\n",
    "#name", "ipaddr",
    "created",
    "recreated time/count",
    "accessed time/count");
  DT_C(dtfd_write_all,(ctl,				/* output header line 				*/
    fd, s, strlen(s)));

  elem = DTLIST_ELEM_HEAD;				/* start list at head 				*/
  while (1)
  {
	DT_C(dtlist_next,(ctl,				/* fetch list element 				*/
      list, elem, &elem));
	if (elem == NULL)					/* end of list? 					*/
	  break;
	entry = (reverb_entry_t *)			/* address the structure 			*/
      elem->user;
	dtstr_printf(ctl, s, sizeof(s),		/* format file line 				*/
      "%20s %16s %10ld %10ld %10ld %10ld %10ld\n",
      entry->name,
	  entry->ipaddr,
      entry->created_time,
      entry->recreated_time,
      entry->recreated_count,
      entry->accessed_time,
      entry->accessed_count);
	DT_C(dtfd_write_all,(ctl,			/* output file line 				*/
      fd, s, strlen(s)));
  }
cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));
  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_reply(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *format,
  ...)
{
  dt_ctl_t *ctl = &param->ctl;
  char line[512];
  va_list arglist;
  va_start(arglist, format);

  if (ctl->user !=						/* previous reply failed? 			*/
	  (void *)(DT_RC_GOOD))
    return DT_RC_GOOD;

  dtstr_printf_va(ctl, line,			/* make a complete string 			*/
    sizeof(line)-sizeof(nl),
    format, arglist);
  va_end(arglist);

  strcat(line, nl);
  ctl->user = (void *)
    dtipc_write_all(ctl, ipc,			/* emit to socket					*/
      (void *)line, strlen(line));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_reply_header(
  dtparam_t *param,
  dtipc_t *ipc)
{

  reverb_reply(param, ipc, "HTTP/1.0 200 OK");
  reverb_reply(param, ipc, "Content-type: text/html");
  reverb_reply(param, ipc, "");
  reverb_reply(param, ipc, "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">");
  reverb_reply(param, ipc, "<html>");
  reverb_reply(param, ipc, "<head>");
  reverb_reply(param, ipc, "<title>");

  reverb_reply(param, ipc, "Tableau Web Reverb Server");
  reverb_reply(param, ipc, "</title>");
  reverb_reply(param, ipc, "<link rev=\"made\" href=\"MAILTO:webmaster@dtack.com\">");
  reverb_reply(param, ipc, "</head>");

  reverb_reply(param, ipc, "<body bgcolor=e5e5d0>");

  reverb_reply(param, ipc, "<font size=+2>"
    "Tableau Web Reverb Server"
    "</font><br>");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_reply_trailer(
  dtparam_t *param,
  dtipc_t *ipc)
{

  reverb_reply(param, ipc, "<hr>");
  reverb_reply(param, ipc, "With questions or comments please email");
  reverb_reply(param, ipc, "<a href=\"mailto:tableau@dtack.com\">"
    "tableau@dtack.com</a>.");
  reverb_reply(param, ipc, "<p>");
  reverb_reply(param, ipc, "Copyright (c) 1996 Tableau Unlimited, All Rights Reserved");

  reverb_reply(param, ipc, "</body>");
  reverb_reply(param, ipc, "</html>");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_reply_message(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *message)
{
  DT_F("reverb_reply");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(reverb_reply_header,(param, ipc));

  reverb_reply(param, ipc, 
    "<p><b><font size=+2>"
    "%s"
    "</font></b><p>",
    message);

  DT_Q(reverb_reply_trailer,(param, ipc));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
reverb_report(
  dtparam_t *param,
  dtipc_t *ipc,
  dtlist_t *list)
{
  DT_F("reverb_report");
  dt_ctl_t *ctl = &param->ctl;
  char created_string[32];
  char recreated_string[32];
  char accessed_string[32];
  dtlist_elem_t *elem;
  reverb_entry_t *entry;

  DT_Q(reverb_reply_header,(param, ipc));	/* html header 						*/

  reverb_reply(param, ipc, "<pre>");
  reverb_reply(param, ipc,
    "%20s %16s %16s  %27s  %s",
    "#name", "ipaddr",
    "created",
    "recreated time/count",
    "accessed time/count");

  elem = DTLIST_ELEM_HEAD;				/* start list at head 				*/
  while (1)
  {
	DT_Q(dtlist_next,(ctl,				/* fetch list element 				*/
      list, elem, &elem));
	if (elem == NULL)					/* end of list? 					*/
	  break;
	entry = (reverb_entry_t *)			/* address the structure 			*/
      elem->user;
	{
	  struct tm *t = localtime(
        &entry->created_time);
	  strftime(created_string,
        sizeof(created_string),
        "%Y%m%d.%H%M.%S", t);
	}
	{
	  struct tm *t = localtime(
        &entry->recreated_time);
	  strftime(recreated_string,
        sizeof(recreated_string),
        "%Y%m%d.%H%M.%S", t);
	}
	{
	  struct tm *t = localtime(
        &entry->accessed_time);
	  strftime(accessed_string,
        sizeof(accessed_string),
        "%Y%m%d.%H%M.%S", t);
	}
	reverb_reply(param, ipc,
      "%20s %16s"
      " %s,"
      " %s %10ld,"
      " %s %10ld",
      entry->name,
      entry->ipaddr,
      created_string,
      recreated_string,
      entry->recreated_count,
      accessed_string,
      entry->accessed_count);
  }
  
  reverb_reply(param, ipc, "</pre>");

  DT_Q(reverb_reply_trailer,(param, ipc));	/* html trailer 					*/
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
reverb_add(
  dtparam_t *param,
  dtipc_t *ipc,
  dtlist_t *list,
  const char *path,
  const char *ipaddr,
  int *changed,
  char *message,
  int mmax)
{
  DT_F("reverb_add");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_elem_t *elem;
  reverb_entry_t *entry;
  
  DT_Q(dtlist_search,(ctl, list,		/* see if we already have it 		*/
    path, &elem));
  if (elem == NULL)						/* no associating exists yet? 		*/
  {
	dtstr_printf(ctl, message, mmax,
      "\"%s\" now %s",
      path, ipaddr);
	DT_Q(reverb_entry_alloc,(			/* make a new entry					*/
      param, &entry));
	entry->created_time = time(NULL);
  }
  else									/* associating already exists? 		*/
  {
	entry = (reverb_entry_t *)
      elem->user;
	dtstr_printf(ctl, message, mmax,
      "\"%s\" now %s (formerly %s)",
      path, ipaddr, entry->ipaddr);

	if (strcmp(ipaddr, entry->ipaddr))	/* association is different? 		*/
	{
  	  DT_Q(dtlist_delete,(ctl, list,	/* delete old association 			*/
        elem));
	  entry->recreated_time = time(NULL);
	  entry->recreated_count++;
	}
  }

  if (strcmp(ipaddr, entry->ipaddr))	/* association is different? 		*/
  {
	dtstr_printf(ctl, entry->name,		/* silently truncate 				*/
      sizeof(entry->name), "%s",
      path);
	strcpy(entry->ipaddr, ipaddr);

    DT_Q(dtlist_after,(ctl, list,		/* add new association to list	    */
      DTLIST_ELEM_HEAD, 
      entry->name, (void *)entry,
      NULL));
	
	*changed = 1;
  }
  else
    *changed = 0;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
reverb_lookup(
  dtparam_t *param,
  dtipc_t *ipc,
  dtlist_t *list,
  char *path,
  char *message,
  int mmax)
{
  DT_F("reverb_lookup");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_elem_t *elem;
  int n;
  n = reverb_split(path, '?');			/* split path from actual URL	    */
  DT_Q(dtlist_search,(ctl, list,		/* do the lookup				    */
    path, &elem));
  if (elem == NULL)						/* didn't find the name? 			*/
    dtstr_printf(ctl, message, mmax,
	  "Sorry,"
      " name \"%s\" is not found.",
      path);
  else									/* we found the name? 				*/
  {
	reverb_entry_t *entry =
	  (reverb_entry_t *)				/* reference the entry 				*/
        elem->user;
	entry->accessed_time = time(NULL);
	entry->accessed_count++;
	if (n == 1)							/* no destination requested?		*/
	{
	  dtstr_printf(ctl, message, mmax,
        "\"%s\" redirected to %s",
        path, entry->ipaddr);
	}
	else								/* specific destination requested? 	*/
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "\"%s\" redirected to %s%s",
        path, entry->ipaddr,
        path + strlen(path) + 1);

	  path += strlen(path) + 1;			/* skip the name and the '?' 		*/
	  reverb_reply(param, ipc, 
        "HTTP/1.0 302 Moved Temporarily");
	  reverb_reply(param, ipc, 
        "Location: http://%s%s",
	    entry->ipaddr, path);
	  reverb_reply(param, ipc, "");
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
reverb_transact(
  dtparam_t *param,
  dtipc_t *ipc,
  dtlist_t *list,
  long *updated)
{
  DT_F("reverb_transact");
  dt_ctl_t *ctl = &param->ctl;
  char *request = NULL;
  int l;
  char message[256];
  int mmax = sizeof(message);
  char *path;
  int n;
  dt_rc_e rc;

  message[0] = '\0';

  DT_C(dtos_malloc2,(ctl,
    (void **)&request, REQUEST_MAX,
    F, "request"));

  DT_C(dtipc_read_line,(ctl, ipc,		/* read the request line 			*/
    request, REQUEST_MAX));
  l = strlen(request);
  if (request[l-1] != '\n')
  {
	rc = dt_err(ctl, F,
      "incoming request too long,"
      " max is %d", REQUEST_MAX);
	goto cleanup;
  }

  request[l-1] = '\0';					/* nuke the newline 				*/

  n = reverb_split(request, ' ');		/* split input into tokens 			*/
  path = request+strlen(request)+1;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,		/* debug the request 				*/
    "%s path=%s", request, path);

  if (n == 0)							/* no input tokens? 				*/
	dtstr_printf(ctl, message, mmax,
	  "Sorry,"
      " no command received.");
  else									/* ................................ */
  if (strcmp(request, "GET"))			/* command is not GET? 				*/
	dtstr_printf(ctl, message, mmax,
	  "Sorry,"
      " unrecognized command \"%s\".", 
      request);
  else									/* ................................ */
  if (n == 1)							/* no path after GET? 				*/
    dtstr_printf(ctl, message, mmax,
	  "Sorry,"
      " no path received.");
  else									/* ................................ */
  if (!strncmp(path, "/add?", 5))		/* request to add? 					*/
  {
	int changed;
	const char *s;
	char *p;
	char ipaddr[REVERB_IPADDR_MAX];

	s = strchr(ipc->client_spec, ',');	/* find the comma					*/
	if (s == NULL)
      s = ipc->client_spec;
	else
      s++;
    DT_C(dtstr_printf,(ctl,				/* use host from control connection	*/
      ipaddr, sizeof(ipaddr), "%s", s));
	p = strchr(ipaddr, ':');				/* find the colon					*/
    if (p != NULL)
      *p++ = '\0';

	DT_C(reverb_add,(param, ipc,
      list,
      path+5, ipaddr,
      &changed, message, mmax));
	if (changed)
  	  *updated = 0;						/* force cache update			    */
  }
  else									/* ................................ */
  if (!strncmp(path, "/report?", 8))	/* request to report? 				*/
  {
	DT_C(reverb_report,(param, ipc,
      list));
  }
										/* ................................ */
  else									/* request for lookup? 				*/
  {
	DT_C(reverb_lookup,(param, ipc,
      list,
      path+1, message, mmax));
  }

  if (message[0] != '\0')				/* some reply is needed? 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "%s", message);
	DT_C(reverb_reply_message,(param,	/* get HTTP reply 					*/
      ipc, message));
  }
  
cleanup:
										/* this stuff is critical 			*/
  if (request != NULL)
  {
	while(1)							/* read trailing lines 				*/
	{
	  request[0] = '\0';
	  DT_I(dtipc_read_line,(ctl, ipc,		
        request, REQUEST_MAX));
	  if (strlen(request) <= 2)			/* final line has only CRLF 		*/
	    break;
	}
	DT_I(dtos_free2,(ctl, request,
      F, "request"));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
reverb_server(
  dtparam_t *param,
  const char *ipcspec,
  const char *cachespec)
{
  DT_F("reverb_server");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  dtlist_t _list, *list = &_list;
  time_t updated = 0;
  dt_rc_e rc;

  ipc->type = DTIPC_TYPE_NONE;

  DT_C(dtlist_create,(ctl, list,		/* create list 						*/
    "list"));
  
  DT_C(reverb_cache_read,(param,		/* suck in cache file 				*/
    cachespec, list));
  
  DT_C(dtipc_register,(ctl, ipc,		/* register as a server 			*/
    ipcspec, 0, 0));

  while(1)
  {
    DT_C(dtipc_wait,(ctl, ipc));		/* wait for connect					*/

	ctl->user = (void *)DT_RC_GOOD;		/* re-enable output 				*/

	reverb_transact(param, ipc, list,	/* handle transaction, ignore rc 	*/
      &updated);

	if (time(NULL) - updated > 60)		/* time to update cache? 			*/
	{
  	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,	/* debug the request 				*/
        "updating cache");
	  DT_C(reverb_cache_write,(param,	/* update cache file 				*/
        cachespec, list));
	  updated = time(NULL);
	}

    DT_C(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
  }
  
cleanup:
  if (ipc->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unregister,(ctl, ipc));	/* release resources				*/

  DT_I(dtlist_destroy,(ctl, list));

  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
reverb_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{

  if (param->connect == NULL ||
	  strlen(param->connect) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->connect_desc);
    *start = param->connect_desc;	
  }
  
  if (strlen(param->cache) != 0 &&	
	  strlen(param->ident) != 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, -cache and -ident are"
      " mutually exclusive.");
    *start = param->cache_desc;			
  }
    
  if (strlen(param->cache) == 0 &&		
	  strlen(param->ident) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, one of -cache and -ident is"
      " required.");
    *start = param->cache_desc;
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(reverb)
{
  DT_F("reverb");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char *ipcspec = NULL;
  long rep;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect, 
      "IPC Spec", NULL, 
      "",
    &param->cache, 
      "Server cache", NULL, 
      "",
    &param->ident, 
      "Client ident", NULL, 
      "",
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->reps, 
      "Repeat", NULL, 
      "0",
    &param->dt, 
      "Repeat delay seconds", NULL, 
      "30",
    NULL));

  DT_Q(dtparam_set,(param,				/* hide params we always parse for	*/
    DTPARAM_FLAG_QUATERNARY,
    &param->dialog,   NULL, NULL, NULL,
    &param->keyboard, NULL, NULL, NULL,
    &param->dbgto,    NULL, NULL, NULL,
    &param->errto,    NULL, NULL, NULL,
    &param->sayto,    NULL, NULL, NULL,
    &param->mem,      NULL, NULL, NULL,
    &param->dbg,      NULL, NULL, NULL,
    &param->yield,    NULL, NULL, NULL,
    &param->win,      NULL, NULL, NULL,
    &param->help,     NULL, NULL, NULL,
    &param->HELP,     NULL, NULL, NULL,
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, "", usage, param,
    arg, reverb_arg_check, NULL));
 
										/* ................................ */
  ctl->dbg = reverb_dbg;				/* special logging needs 			*/
  ctl->err = reverb_err;
  ctl->say = reverb_say;

# define L (strlen("socket,"))
  DT_C(dtos_malloc,(ctl,
    (void **)&ipcspec,
    strlen(param->connect) + L + 1));
  strcpy(ipcspec, "socket,");			/* prepend socket before ipc spec 	*/
  strcat(ipcspec, param->connect);
  
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	if (strlen(param->cache) != 0)		/* we are the server? 				*/
	{
	  DT_C(reverb_server,(param, 
        ipcspec, param->cache));
	}
	else
	if (strlen(param->ident) != 0)		/* we are the client? 				*/
    {
	  DT_C(reverb_client,(ctl,
        ipcspec, param->ident,
        DTIPC_FLAG_TRY_DELAY_30 |
        DTIPC_FLAG_TRY_FOREVER, 
        NULL, 0)); 
	}
	if (rep < param->reps-1 &&			/* don't wait the last time		    */
		param->dt != 0)
  	  DT_C(dtos_delay,(ctl, 
        param->dt*1000L));
  }	
										/* ................................ */
cleanup:
  if (ipcspec != NULL)
    DT_I(dtos_free,(ctl, ipcspec));

  DT_I(dt_leave,(param));				/* clean up before leaving			*/
    
  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

  {
	int check_fail = 0;
    DT_I(dt_heapmon_uninit,(ctl,		/* check all memory is freed        */
      &check_fail));
    if (rc == DT_RC_GOOD &&
		check_fail)						/* any failed assertions?           */
      rc = DT_RC_BAD;
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
