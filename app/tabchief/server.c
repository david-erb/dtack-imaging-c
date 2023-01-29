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

#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: server.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>
#include <reverb.h>
#include <dtack/liner.h>
#define DT_RC_SHUTDOWN ((dt_rc_e)(0xdeadfeeb))

/*..........................................................................
 *..........................................................................*/

#define CR (0x0d)
#define LF (0x0a)

#define FLAG_NOHTML      (0x0001)
#define FLAG_LOG1        (0x0002)
#define FLAG_SKIPTRAILER (0x0004)
#define FLAG_DANGEROUS   (0x0008)

#define REQUEST_MAX (8192)

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_run(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_run");
  dt_ctl_t *ctl = &param->ctl;
  tabchief_t *tabchief = 
    (tabchief_t *)ctl->user;
  char *line = NULL;
  int winrc;
  dt_rc_e rc;

  DT_C(dtos_strdup,(ctl, arg, &line));

  dsched_split(line, '\0');

  tabchief_reply(param, ipc, "<p>");

#ifdef DTCONFIG_IMPLIED_MSW
    winrc = WinExec(line, SW_SHOWNORMAL);
# else
    winrc = -1;
# endif

  if (winrc <= 31)
  {
	DT_C(dtstr_printf,(ctl,
      tabchief->response,
      sizeof(tabchief->response),
      "Command launch failed, rc=%d.",
      winrc));
  }
  else
  {
	DT_C(dtstr_printf,(ctl,
      tabchief->response,
      sizeof(tabchief->response),
      "Command launch succeeded."));
  }

cleanup:
  if (line != NULL)
    DT_I(dtos_free2,(ctl, line,
      F, "line"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static const char *choices[] = {
  "/",
  TABCHIEF_COMMAND_SCHEDULE,
  "show",
  "tail",
  "html",
  "gif",
  "xbm",
  "edit",
  "save",
  "run",
  "restart",
  "copyto",
  "copyfrom",
  NULL
};
static const char *titles[] = {
  "Main",
  "Scheduler",
  "Show File",
  "Tail File",
  "Html File",
  "Gif File",
  "Xbm File",
  "Edit File",
  "Save File",
  "Run",
  "Restart",
  "Copyto",
  "Copyfrom",
  NULL
};
static tabchief_f *methods[] = {
  tabchief_index,
  dsched,
  tabchief_server_showfile,
  tabchief_server_tailfile,
  tabchief_server_html,
  tabchief_server_gif,
  tabchief_server_gif,
  tabchief_server_editfile,
  tabchief_server_savefile,
  tabchief_server_run,
  tabchief_restart,
  tabchief_server_copyto,
  tabchief_server_copyfrom,
  NULL
};
static int flags[] = {
  FLAG_SKIPTRAILER,						/* main 							*/
  FLAG_LOG1,							/* schedule 						*/
  FLAG_DANGEROUS,						/* show 							*/
  FLAG_DANGEROUS,						/* tail 							*/
  FLAG_NOHTML,							/* html	 							*/
  FLAG_NOHTML,							/* gif	 							*/
  FLAG_NOHTML,							/* xbm	 							*/
  FLAG_DANGEROUS|FLAG_LOG1,				/* edit 							*/
  FLAG_DANGEROUS|FLAG_LOG1,				/* save 							*/
  FLAG_DANGEROUS|FLAG_LOG1,				/* run	 							*/
  FLAG_DANGEROUS|FLAG_LOG1,				/* restart 							*/
  FLAG_DANGEROUS|FLAG_LOG1|FLAG_NOHTML,	/* copyto 							*/
  FLAG_DANGEROUS|FLAG_LOG1|FLAG_NOHTML	/* copyfrom 						*/
};

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_server_transact(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *host)
{
  DT_F("tabchief_server_transact");
  dt_ctl_t *ctl = &param->ctl;
  tabchief_t _tabchief, *tabchief = &_tabchief;
  char *request = NULL;
  int l;
  char *path;
  int ishtml = 1;
  int skiptrailer = 0;
  int header_done = 0;
  int shutdown = 0;
  int n;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,
    (void **)&request, REQUEST_MAX,
    F, "request"));

  memset(tabchief, 0, sizeof(*tabchief));
  ctl->user = tabchief;

  tabchief->response[0] = '\0';			/* clear error response 			*/

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

  dtstr_tokenize(ctl, request, ' ', 	/* split input into tokens 			*/
    &n);
  path = request+strlen(request)+1;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,		/* debug the request 				*/
    "%s path=%s", request, path);

  if (n == 0)							/* no input tokens? 				*/
    dt_err(ctl, F,
	  "Sorry, no command received.");
  else									/* ................................ */
  if (strcmp(request, "GET"))			/* command is not GET? 				*/
    dt_err(ctl, F,
	  "Sorry,"
      " unrecognized command \"%s\".", 
      request);
  else									/* ................................ */
  if (n == 1)							/* no path after GET? 				*/
    dt_err(ctl, F,
	  "Sorry, no path received.");
										/* ................................ */
  else									/* request for work? 				*/
  {
	int choice;
	char *arg;
	dtstr_tokenize(ctl, path, '?', &n);	/* split off args after the '?'		*/
	if (n == 1)							/* no question mark?				*/
	{
#     define DOC "/doc/"
	  if (!strncmp(path, DOC,			/* leader is doc? 					*/
			 strlen(DOC)))		
	  {
		char *suffix;
		arg = path + 1;					/* keep doc as part of the arg		*/
		suffix = strchr(arg, '.');
		if (suffix == NULL)
		  suffix = "";
        if (!strcmp(suffix, ".xbm"))
		  path = "/xbm";
		else
        if (!strcmp(suffix, ".gif"))
		  path = "/gif";
        else
		  path = "/html";
	  }
	  else								/* leader is not doc? 				*/
	    arg = "";						/* whole thing is the command 		*/
	}
	else								/* there is a question mark? 		*/
	{
      arg = path+strlen(path)+1;		/* arg is everything after 			*/
	}
	DT_C(dtstr_choice2,(ctl,			/* look up function 				*/
      choices, path+1, &choice,
      tabchief->response, 
      sizeof(tabchief->response)));
	if (choice >= 0)
	{
	  {
		char t[64];
		dtstr_printf(ctl, 
          t, sizeof(t)-4,
          "%s", arg);
	    if (strlen(arg) > sizeof(t)-4)
          strcat(t, "...");
	    dt_dbg(ctl, NULL,
          flags[choice] & FLAG_LOG1?
            DT_DBG_MASK_ANY:
            DT_DBG_MASK_USR2,
          "(from %s) %s?%s", 
          ipc->client_spec,
          choices[choice], t);
	  }
	  ishtml = !(flags[choice] &
		FLAG_NOHTML);
	  skiptrailer = (flags[choice] &
		FLAG_SKIPTRAILER);
	  if (ishtml)
	  {
		DT_C(tabchief_reply_header,(	/* give html header 				*/
          param, ipc, 
          titles[choice], host));
		header_done = 1;
	  }
	  if (!strcmp(param->op, "mockup") &&
		  flags[choice] & FLAG_DANGEROUS)
	  {
		dtstr_printf(ctl,
          tabchief->response,
		  sizeof(tabchief->response),
          "Sorry, we decline to run"
          " \"%s\" on this mockup server.",
          choices[choice]);
	  }
	  else
	  {
		DT_G((*methods[choice]),(param,	/* call appropriate function 		*/
          ipc, path+1, arg));
		if (rc == DT_RC_STOP ||
			rc == DT_RC_SHUTDOWN)
		  shutdown = 1;
	  }
	}
	else								/* not a valid choice 				*/
	  (void)dt_err(ctl, F,
        "invalid path \"%s\"",
        path);
  }

  if (ishtml)
  {
	if (tabchief->response[0] != '\0')	/* some reply is needed? 			*/
	{
	  if (!header_done)
  	    DT_C(tabchief_reply_header,(	/* give html header 				*/
          param, ipc, "Error", host));
	  DT_C(tabchief_reply_message,(		/* get HTTP reply 					*/
        param, ipc, 
        tabchief->response));
	}

	if (!skiptrailer)
	DT_C(tabchief_reply_trailer,(		/* give html trailer 				*/
      param, ipc));
  }
  else
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "response \"%s\"",
      tabchief->response);
	if (strlen(tabchief->response) > 0)
	DT_C(dtipc_write_all,(ctl, ipc,		/* give response 					*/
      tabchief->response,
      strlen(tabchief->response)));
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

  if (shutdown)
    rc = DT_RC_SHUTDOWN;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_reverb(
  dtparam_t *param,
  char *host,
  int max)
{
  DT_F("tabchief_reverb");
  dt_ctl_t *ctl = &param->ctl;
  char *reverb = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  if (strlen(param->ident) != 0 &&		/* supposed to use reverb server?	*/
	  strcmp(param->ident, "none"))
  {
	char *ident;

#   define P "rocket,"
#   define L (strlen(P))
	DT_C(dtos_malloc,(ctl,
      (void **)&reverb,
      strlen(param->ident) + L + 1));
	strcpy(reverb, P);					/* prepend socket before ipc spec 	*/
	strcat(reverb, param->ident);
  
	ident = strchr(reverb, '+');
	if (ident == NULL)					/* reverb name not given? 			*/
	{
	  (void)dt_err(ctl, F,
        "no ident given in reverb"
        " %s", reverb);
	}
	else								/* reverb name given 				*/
	{
	  *ident++ = '\0';
	  DT_C(reverb_client,(ctl,			/* contact reverb server 			*/
        reverb, ident, param->max, 
        host, max));
	}
  }
  else
    host[0] = '\0';
  
cleanup:
  if (reverb != NULL)					/* reverb string got allocated? 	*/
    DT_I(dtos_free2,(ctl, reverb,
      F, "reverb"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_init_proc(
  dtparam_t *param)
{
  DT_F("tabchief_init_proc");
  dt_ctl_t *ctl = &param->ctl;
  char filename[256];
  dtfd_t _fd, *fd = &_fd;
  int list_created = 0;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtlist_create,(ctl,
    &tabchief_proclist, "proclist"));
  list_created = 1;
  
  DT_C(dtstr_printf,(ctl,
    filename, sizeof(filename),
    "%s/%s.arg", 
    param->config, param->name));		/* tabstart.arg, usually		    */
  DT_C(dtfd_open,(ctl, fd, 
    filename, "r"));
  DT_C(dtliner_fd_list,(				/* make list from lines in file		*/
    ctl, fd, 
    "#*;",								/* string of comment characters		*/
    " ",								/* the key:value separator 			*/
    &tabchief_proclist));

  elem = DTLIST_ELEM_HEAD;
  while(1)
  {
	char *p;
	DT_C(dtlist_next,(ctl,				/* fetch proc elem 					*/
      &tabchief_proclist,
      elem, &elem));
	if (elem == NULL)
	  break;
	p = strchr(elem->name, '=');		/* chop off src_filename 			*/
	if (p != NULL)
	  *p = '\0';
  }
  
cleanup:
  if (fd->fd != -1)
	DT_I(dtfd_close,(ctl, fd));
  if (list_created && rc != DT_RC_GOOD)
  {
	DT_I(dtlist_free,(ctl,
      &tabchief_proclist));
	DT_I(dtlist_destroy,(ctl,
      &tabchief_proclist));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_uninit_proc(
  dtparam_t *param)
{
  DT_F("tabchief_init_proc");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(dtlist_free,(ctl,
    &tabchief_proclist));
  DT_I(dtlist_destroy,(ctl,
    &tabchief_proclist));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabchief_server(
  dtparam_t *param,
  const char *ipcspec)
{
  DT_F("tabchief_server");
  dt_ctl_t *ctl = &param->ctl;
  int init_proc = 0;
  dtipc_t _ipc, *ipc = &_ipc;
  dtos_time_t now, last;
  double elapsed;
  int n;
  char host[64];
  dt_rc_e rc;

  ipc->type = DTIPC_TYPE_NONE;

  if (!(param->config_flag &	
	   DTPARAM_FLAG_VALUE))
  {
	rc = dt_err(ctl, F,
      "need -config");
	goto cleanup;
  }
  
  DT_C(tabchief_init_proc,(param));
  init_proc = 1;

  DT_C(dtipc_register,(ctl, ipc,		/* register as a server 			*/
    ipcspec, 0, 0));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "ready, config is \"%s\"",
    param->config);

  host[0] = '\0';
  last.wall = 0.0;
  while(1)
  {
	int isready;

	DT_C(dtos_time,(ctl, &now));		/* current time 					*/
	elapsed = (now.wall-last.wall) *	/* elapsed since last contact 		*/
      1000.0;
	if (param->omega > 0.0 &&
		(last.wall == 0.0 ||
		 elapsed > param->omega))		/* time to re-reverb? 				*/
	{
      DT_G(tabchief_reverb,(param,		/* contact reverb server 			*/
        host, sizeof(host)));			/* catch failure 					*/
	  if (rc == DT_RC_STOP ||
		  rc == DT_RC_SHUTDOWN)
		goto cleanup;
      if (rc == DT_RC_GOOD)				/* reverb contact successful?		*/
		n = 0;
	  else								/* reverb contact failed?			*/
		n++;
	  if (n > param->n)					/* tired of reverb failing?		    */
		goto cleanup;
	  DT_C(dtos_time,(ctl, &last));
	}

	DT_C(dtipc_poll,(ctl, ipc,			/* wait for incoming connection		*/
      DTIPC_POLL_ACCEPT, 
      (long)param->omega+500,
      "client connect", &isready));

	if (isready)						/* client is connecting? 			*/
	{
	  DT_C(dtipc_wait,(ctl, ipc));		/* wait for connect					*/

	  DT_G(tabchief_server_transact,(	/* transact							*/
        param, ipc, host));
	  DT_I(dtipc_done,(ctl, ipc));		/* server finishes transaction	    */
	  if (rc == DT_RC_STOP ||
		  rc == DT_RC_SHUTDOWN)
		goto cleanup;

	  DT_C(dtos_time,(ctl, &last));		/* remember time of last contact 	*/
	}
  }
  
cleanup:
  
  if (ipc->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unregister,(ctl, ipc));	/* release resources				*/

  if (init_proc)
	DT_I(tabchief_uninit_proc,(param));

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
