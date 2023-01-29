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

#include <wrangler.h>
DT_RCSID("app/wrangler $RCSfile: server.c,v $ $Revision: 1.8 $");
#include <dtack/liner.h>
#include <dtack/str.h>

#define CR (0x0d)
#define LF (0x0a)

static const char *requests[] = {
  "start",
  "cgi",
  "docs",
  "state",
  "exit",
  NULL
};
static dtdg_server_request_f *funcs[] = {
  wrangler_cmd_start,
  wrangler_cmd_cgi,
  wrangler_cmd_docs,
  wrangler_cmd_state,
  wrangler_cmd_exit,
  NULL
};

#define BSIZE (8192)

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
wrangler_goodbye(
  dt_ctl_t *ctl,
  void *goodbye_arg)
{
  DT_F("wrangler_goodbye");
  wrangler_t *wrangler = (wrangler_t *)goodbye_arg;
  dt_rc_e rc;
  
  if (wrangler->connected)
  {
	DT_G(dtipc_done,(ctl,				/* release child's end of socket	*/
      wrangler->dgserver.ipc));
  }
  
  DT_G(dtdg_server_uninit,(ctl,			/* quit being a datagram server 	*/
    &wrangler->dgserver));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_cmd_start(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("wrangler_cmd_start");
  wrangler_t *wrangler = (wrangler_t *)arg;
  char *sessid = NULL;
  char *streamid = NULL;
  char *client = NULL;
  char *limit = NULL;
  char *start = NULL;
  char *done = NULL;
  char *audio = NULL;
  char *drone_args = NULL;
  double port;
  int i, n;
  dtfd_t fd;
  dt_rc_e rc = DT_RC_GOOD;

  fd.fd = -1;

  DT_C(dtdg_server_header_plain,(ctl,	/* give standard plain header 		*/
    dgserver, wrangler->ident));

  DT_C(dtdg_server_response,(ctl,		/* ident ourselves in response 		*/
    dgserver, 
    DTDG_RESPONSE_IDENT
    ": %s", wrangler->ident));

  DT_C(wrangler_verify,(wrangler,		/* verify parent authorization 		*/
    dgserver->ipc));

  dtstr_tokenize(ctl, rest, '&', &n);	/* split up rest of the line		*/
  for (i=0; i<n; i++)
  {
#   define CUT(FIELD) \
	if (DT_ABBREV(rest, #FIELD "=")) \
	  FIELD = strchr(rest, '=') + 1
	CUT(sessid);
	CUT(streamid);
	CUT(client);
	CUT(limit);
	CUT(start);
	CUT(done);
	CUT(audio);
	rest = rest + strlen(rest) + 1;
  }

# define CHECK(FIELD) \
  if (FIELD == NULL) \
  { \
    DT_C(dtdg_server_response,(ctl, \
      dgserver, \
      DTDG_RESPONSE_ERROR \
      ": missing %s", #FIELD)); \
	goto cleanup; \
  }
  
  CHECK(sessid);						/* make sure we got all our fields 	*/
  CHECK(streamid);
  CHECK(client);
  CHECK(limit);
  CHECK(start);
  CHECK(done);
  CHECK(audio);

  DT_C(dtfd_open,(ctl, &fd,				/* open drone arg template file 	*/
    wrangler->drone_template,
    DTCONFIG_FOPEN_RTEXT));	

  DT_C(dtliner_fd_string,(ctl, &fd,		/* make string from file 			*/
    "#*;", " \t", 
	&wrangler->drone_args));

  n = strlen(wrangler->drone_args) +
    1024;

  DT_C(dtos_malloc2,(ctl, 
    (void **)&drone_args, n,
    F, "drone args"));

  DT_C(dt1d_ran_uniform,(				/* random port number 				*/
    &wrangler->random, &port));
  port = wrangler->min_port + port * 
    (wrangler->max_port -
	 wrangler->min_port);
  
  dt_dbg(ctl, F, DT_DBG_MASK_USR2, 
    "sessid=%s, port=%ld", 
    sessid, (long)port);

  
#ifdef DTCONFIG_IMPLIED_UNIXLIKE
  DT_C(dtstr_printf,(ctl,				/* create connect string 			*/
    drone_args, n,
    wrangler->drone_args,				/* this is a format string			*/
    (long)port, 
    sessid,
    streamid,
    client,
    limit,
    strlen(start)? start: "\"\"",
    strlen(done)? done: "\"\"",
    strlen(audio)? audio: "\"\""));
#else
  DT_C(dtstr_printf,(ctl,				/* create connect string 			*/
    drone_args, n,
    wrangler->drone_args,				/* this is a format string			*/
    wrangler->exe,						/* argv[0]							*/
    (long)port, 
    sessid,
    streamid,
    client,
    limit,
    strlen(start)? start: "\"\"",
    strlen(done)? done: "\"\"",
    strlen(audio)? audio: "\"\""));
#endif

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,   
    "%s", drone_args);

  DT_C(dtprocs_start,(ctl,				/* start drone 						*/
    &wrangler->procs, sessid,
    wrangler->executable,
    drone_args,
    -1, -1,
    wrangler_goodbye, (void *)wrangler,
    NULL));
  
  DT_I(dtdg_server_response,(ctl,
    dgserver,
    "port"
    ": %ld", (long)port));

cleanup:
  if (drone_args != NULL)
	DT_I(dtos_free2,(ctl, drone_args,
      F, "substituted drone args"));

  if (wrangler->drone_args != NULL)
  {
	DT_I(dtos_free2,(ctl,
      wrangler->drone_args,
      F, "drone args"));
	wrangler->drone_args = NULL;
  }

  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));

  
  if (rc != DT_RC_GOOD)
  {
    DT_I(dtdg_server_response,(ctl,
      dgserver,
      DTDG_RESPONSE_ERROR
      ": could not start drone"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_cmd_cgi(						/* start cgi program 				*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("wrangler_cmd_cgi");
  wrangler_t *wrangler = (wrangler_t *)arg;
  unsigned int count;
  char *drone_args = NULL;
  int i, n;
  dtfd_t fd;
  dt_rc_e rc = DT_RC_GOOD;
  
  fd.fd = -1;

  DT_C(dtlist_count,(ctl,				/* how many children already? 		*/
      &wrangler->procs.list, &count));

  if (count >= (unsigned int)			/* can't start another child? 		*/
	    wrangler->max_children)
  {
	DT_C(wrangler_cmd_docs,(ctl,		/* serve an image anyway 			*/
      dgserver, arg,
      "docs", "nomore.jpg",
      message, message_max));
	goto cleanup;
  }

  {
	char *s;
	DT_C(dtos_strdup3,(ctl, 
      "QUERY_STRING", "=", rest, &s));
	putenv(s);							/* give the cgi args to the child 	*/
	DT_C(dtos_free2,(ctl, s, F, s));
  }

  DT_C(dtfd_open,(ctl, &fd,				/* open drone arg template file 	*/
    wrangler->drone_template,
    DTCONFIG_FOPEN_RTEXT));	

  DT_C(dtliner_fd_string,(ctl, &fd,		/* make string from file 			*/
    "#*;", " \t", 
	&wrangler->drone_args));

  n = strlen(wrangler->drone_args) +	/* space to subsitute into 			*/
	strlen(wrangler->executable)+
    256;

  DT_C(dtos_malloc2,(ctl, 
    (void **)&drone_args, n,
    F, "drone args"));
  
  DT_C(dtstr_printf,(ctl,				/* create connect string 			*/
    drone_args, n,
    wrangler->drone_args,				/* this is a format string			*/
    wrangler->executable));			 	/* argv[0]							*/

  DT_C(dtprocs_start,(ctl,				/* start cgi drone 					*/
    &wrangler->procs, "cgi drone",
    wrangler->executable,
    drone_args,
    dgserver->ipc->datahandle,
    dgserver->ipc->datahandle,
    wrangler_goodbye, (void *)wrangler,
    NULL));

  DT_C(dtipc_done,(ctl,					/* close server end of data socket 	*/
    dgserver->ipc));
  wrangler->connected = 0;				/* emit no more on data socket 		*/
  
cleanup:
  if (drone_args != NULL)
	DT_I(dtos_free2,(ctl, drone_args,
      F, "substituted drone args"));

  if (wrangler->drone_args != NULL)
  {
	DT_I(dtos_free2,(ctl,
      wrangler->drone_args,
      F, "drone args"));
	wrangler->drone_args = NULL;
  }

  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));
  
  if (rc != DT_RC_GOOD)
  {
    DT_I(dtdg_server_response,(ctl,
      dgserver,
      DTDG_RESPONSE_ERROR
      ": could not start drone"));
  }
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_cmd_docs(						/* serve file to browser			*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("wrangler_cmd_docs");
  wrangler_t *wrangler = (wrangler_t *)arg;
  const char *mimetype;
  char *p;
  dtfd_t _fd, *fd = &_fd;
  unsigned char *b = NULL;
  long total;
  int output_header = 0;
  dt_rc_e rc;

  fd->fd = -1;

  p = strrchr(rest, '.');
  
# define MIMETYPE(SUFFIX, MIMETYPE)		\
  if (p != NULL &&						\
	  !strcmp(p+1, SUFFIX))				\
	mimetype = MIMETYPE;				\
  else

  MIMETYPE("html", "text/html")
  MIMETYPE("htm", "text/html")
  MIMETYPE("txt", "text/plain")
  MIMETYPE("gif", "image/gif")
  MIMETYPE("jpg", "image/jpeg")
  MIMETYPE("class", "text/plain")
  mimetype = "application/octet-stream";

  DT_C(dtos_malloc2,(ctl, (void **)&b,	/* space for buffer 				*/
    BSIZE, F, "copy buffer"));

  DT_C(dtstr_printf,(ctl, b, BSIZE,		/* prepend path 					*/
    "%s/%s", wrangler->docpath,
     rest[0]=='/'? rest+1: rest));
  DT_C(dtfd_open,(ctl, fd, b, "r"));	/* open local file for reading 		*/
   
  DT_C(dtfd_seek,(ctl, fd, 0,			/* get file size 					*/
    DTFD_WHENCE_END, &total));
  
  DT_C(dtfd_seek,(ctl, fd, 0,			/* return to start of file 			*/
    DTFD_WHENCE_SET, NULL));

	   
  while(1)
  {
	long got;
	DT_C(dtfd_read,(ctl, fd,			/* read from local file				*/
      b, BSIZE, &got));
	if (got == 0)						/* end of file? 					*/
	  break;

	if (!output_header)
	{
	  DT_C(dtdg_server_response,(ctl,	
        dgserver, 
        "HTTP/1.0 200 OK"));
      output_header =  1;
      DT_C(dtdg_server_response,(ctl,	
        dgserver, 
        "Content-type: %s", mimetype));
      DT_C(dtdg_server_response,(ctl,	
        dgserver, 
        "Content-length: %ld", total));
      DT_C(dtdg_server_response,(ctl,	
        dgserver, 
        ""));
	}

	DT_C(dtipc_write_all,(ctl,	 		/* write to socket					*/
      dgserver->ipc, b, got));
  }

cleanup:
  if (rc != DT_RC_GOOD)
  {
    if (!output_header)
    {
      DT_I(dtdg_server_response,(ctl,	
        dgserver, 
        "HTTP/1.0 %s %s",
  	    (fd->fd == -1)? "404": "500",
	    (fd->fd == -1)? "not found": "server error"));
      DT_I(dtdg_server_response,(ctl,	
        dgserver, 
        ""));
      DT_I(dtdg_server_response,(ctl,	
        dgserver, 
        "Sorry, error %s the requested file.",
	    (fd->fd == -1)? "finding": "reading"));
	}
  }

  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "copy buffer"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_cmd_state(						/* reply our state to browser 		*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("wrangler_cmd_state");
  wrangler_t *wrangler = (wrangler_t *)arg;
  dtos_time_t now;
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdg_server_header_plain,(ctl,	/* give standard plain header 		*/
    dgserver, wrangler->ident));

  DT_C(dtdg_server_response,(ctl,		/* ident ourselves in response 		*/
    dgserver, 
    DTDG_RESPONSE_IDENT
    ": %s", wrangler->ident));

  DT_C(dtos_time,(ctl, &now));
  
  elem = DTLIST_ELEM_HEAD;				/* start at top of list 			*/
  while (1)
  {
	dtprocs_proc_t *proc;
	DT_C(dtlist_next,(ctl,				/* get next proc list entry 		*/
      &wrangler->procs.list, 
      elem, &elem));
	if (elem == NULL)
	  break;
	proc = (dtprocs_proc_t *)			/* address the proc entry 			*/
      elem->user;
	DT_C(dtdg_server_response,(ctl, 
      dgserver, 
      "running=%d sessid=%s elapsed=%0.2f sec",
      proc->running,
      proc->name,
      now.wall-proc->start_time.wall));
  }


cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_cmd_exit(						/* exit this server					*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("wrangler_cmd_exit");
  wrangler_t *wrangler = (wrangler_t *)arg;
  dtos_time_t now;
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdg_server_header_plain,(ctl,	/* give standard plain header 		*/
    dgserver, wrangler->ident));

  DT_C(dtdg_server_response,(ctl,		/* ident ourselves in response 		*/
    dgserver, 
    DTDG_RESPONSE_IDENT
    ": %s", wrangler->ident));
  
  DT_C(dtdg_server_response,(ctl,		/* ident ourselves in response 		*/
    dgserver, 
    "now exiting"));

cleanup:
  return DT_RC_STOP;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
wrangler_server_transact(
  wrangler_t *wrangler,
  dtdg_server_t *dgserver)
{
  DT_F("wrangler_server_transact");
  dt_ctl_t *ctl = wrangler->ctl;
  char *request = NULL;
  dt_rc_e rc;

  DT_C(dtdg_server_get_request,(		/* receive the datagram request		*/
    ctl, dgserver, &request));

  wrangler->connected = 1;

  if (request == NULL)					/* no request available? 			*/
    goto cleanup;

  DT_G(dtdg_server_request,(ctl,		/* generate datagram response		*/
    dgserver, request,
    (void *)wrangler,
    requests, funcs,
    wrangler->message, 
    sizeof(wrangler->message)));

  if (rc == DT_RC_STOP)
	goto cleanup;

  if (!wrangler->connected)				/* socket got usurped? 				*/
	goto cleanup;

  if (wrangler->message[0])
  {
    DT_C(dtdg_server_response,(ctl,
      dgserver,
      DTDG_RESPONSE_ERROR
      ": %s", wrangler->message));
  }
	
  DT_C(dtdg_server_trailer_plain,(ctl,	/* give html trailer 				*/
    dgserver));

cleanup:
  if (wrangler->connected)
  {
	dt_rc_e rc;
	DT_G(dtdg_server_done,(ctl,			/* finish response to the datagram	*/
      dgserver));
  }

  wrangler->connected = 0;

  if (request != NULL)
	DT_I(dtos_free2,(ctl, request,		/* free the datagram command 		*/
      F, "request"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_server(
  dtparam_t *param)
{
  DT_F("wrangler_server");
  dt_ctl_t *ctl = &param->ctl;
  int major, minor;
  long date;
  wrangler_t _wrangler, *wrangler = &_wrangler;
  dtdg_server_t *dgserver = &wrangler->dgserver;
  dt_rc_e rc;

  dgserver->initialized = 0;

  DT_C(wrangler_init,(wrangler,			/* init wrangler structure 			*/
    param));

  DT_C(dtdg_server_init,(ctl, dgserver,	/* become a datagram server 		*/
    param->connect));

  DT_C(wrangler_rev,(ctl, NULL,
    &major, &minor, &date));

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "%02d-%02d (%ld) ready to start drones",
    major, minor, date);

  while(1)
  {
	int is_request;

	DT_C(dtdg_server_is_request,(ctl,	/* check state of command input 	*/
      dgserver, &is_request));
	if (is_request)						/* client is connecting? 			*/
	{
	  DT_C(wrangler_server_transact,(	/* transact							*/
        wrangler, dgserver));
	}
	else
	  DT_C(dt_yield,(ctl,				/* check for async messages 		*/
        "polling for connections"));
	
	while(1)
	{
	  dtprocs_proc_t *proc;
	  dtos_time_t now;
	  char s[256];
	  DT_C(dtprocs_reap,(ctl,
        &wrangler->procs, &proc));
	  if (proc == NULL)					/* no more to reap? 				*/
		break;
	  DT_C(dtos_time,(ctl, &now));
	  DT_C(dtprocs_proc_describe,(ctl,
        &wrangler->procs, proc, 
        s, sizeof(s)));
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "reaped (%0.2f sec) %s",
        now.wall-proc->start_time.wall,
        s);
	  DT_C(dtos_free2,(ctl, proc,
        F, "reaped proc structure"));
	}

	DT_C(dtos_delay,(ctl, param->dt));	/* poll delay						*/
  }
  
cleanup:
  DT_I(dtdg_server_uninit,(ctl,			/* quit being a datagram server 	*/
    dgserver));
 
  DT_I(wrangler_uninit,(wrangler));		/* free wrangler resources 			*/

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
