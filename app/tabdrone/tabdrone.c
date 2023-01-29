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
| TITLE
| perdrone - video push drone
| 
| END
 *..........................................................................*/

/*..........................................................................
| USAGE
| perdrone - video push drone
| 
|.SYNOPSIS
|:perdrone [params] -connect host:port
|
|.SUMMARY
| Push video stream to single client.
| 
|.PARAMETERS
| :-connect:	connection spec (required for either client or server)
| :-ident:		session id sent to the start and done urls
| :-name:		expected client spec or rocket,0.0.0.0:0 for any
| :-max:		time limit in seconds (default 0 meaning unlimited)
| :-input:		frame source (default tabgrab.dat)
| :-variant:	audio suffix or blank for no audio (default .au)
| :-pre:		url to hit after client connects but before video starts
| :-output:		url to hit when video is done
| :-dt:			time to wait for client to connect in ms (default 3000)
| :-op:			operation, either "normal" or "rev" (default normal)
|dtinclude(params1.dx)
|
|.DESCRIPTION
| Tabdrone is an HTTP-compliant web server.
| It is intended to serve a single video stream then die.
| 
| Tabdrone has a number of command line parameters which control its function.
| Alternatively, tabdrone can read its parameters from a config file.
| In normal operation, a separate program builds tabdrone's config file on the fly.
| One such program is wrangler.
|
| When it is started, tabdrone listens for a client connection.
| Optionally, tabdrone can refuse any connection not from a specified IP address.
| Tabdrone talks "HTTP" with the connecting client.
| This allows Netscape to talk to tabdrone from an :<img>: tag.
| 
| Even though it is an HTTP-protocol conversation,
| the connecting client need specify no "file" part of the URL.
| This is because the tabdrone instance has all the parameters it needs in its
| config file.
|
| After the client connects,
| before starting the video stream, tabdrone hits a "start" url.
| This "start" url is intended to notify a tracking system of video start.
| Tabdrone ignores the response to the "start" url.
| However, tabdrone must receive a standard HTTP response or it dies with an error.
| 
| Tabdrone reads motion jpeg files and sends them to a browser.
| The input jpeg files are stored on disk as individual files.
| The individual filenames correspond to a pattern.
| The pattern contains an 8-digit number which increases with each frame.
| 
| When sending to a browser, it is in the form of a "server push" stream.
| More exactly, the MIME type is "multipart/x-mixed-replace.
| 
| Tabdrone hits a "done" url when video stops.
| This "done" url is intended to notify a tracking system of the video time used.
| Tabdrone ignores the response to the "done" url.
| However, tabdrone must receive a standard HTTP response or it dies with an error.
|
|.START URL
| After the client connects, but before video is pumped,
| the drone hits the start url.
| The start url is formed by appending the string:
| 
|:   sessid=XXXXXXXX
| 
| to the end of the ~-pre~ command line parameter.
| The ~XXXXXXXX~ is replaced  by the contents of the ~-ident~ parameter.
|
| For example, if the command line or arg file had:
| 
|:   -ident ABCD1234
|:   -pre rocket,206.251.105.36:80/start.asp?
|
| then the drone would hit the start url:
| 
|:   http://206.251.105.36:80/start.asp?sessid=ABCD1234
| 
|.DONE URL
| After the video session is ended for any reason,
| the drone hits the done url.
| The done url is formed by appending the string:
| 
|:   sessid=XXXX&time=FF.FF&frames=NNNN
| 
| to the end of the ~-output~ command line parameter.
| The ~XXXXXXXX~ is replaced  by the contents of the ~-ident~ parameter.
| The ~FF.FF~ is replaced by the number of seconds actually used.
| The ~NNNN~ is replaced by the number of frames pumped.
|
| For example, if the command line or arg file had:
| 
|:   -ident ABCD1234
|:   -output rocket,206.251.105.36:80/done.asp?
|
| then the drone would hit the done url:
| 
|:   http://206.251.105.36:80/done.asp?sessid=ABCD1234&time=33.12&frames=17
|
|.VIDEO FILES
| The drone looks for files whose names fit the pattern
| given by the ~-input~ command line parameter.
| The pattern must contain exactly one "%08ld" substring.
| For example,
| 
|:   -input jpeg/cam0.%08ld.rgb888.enc
| 
| would match the following filenames:
| 
|:   jpeg/cam0.00000001.rgb888.enc
|:   jpeg/cam0.00023431.rgb888.enc
| 
| and so on.
| When the drone first starts, it looks for the highest numbered file.
| As its first frame, it pushes the file previous to that one.
| Then it begins to loop.
| In each loop iteration,
| it waits for the next highest numbered file to arrive.
| It always pushes the file previous to the highest number available.
| This is because it is possible that the highest numbered file is
| only partially complete.
|
|.DIAGNOSTIC OUTPUT
| Tabdrone writes all its output to its log file.
| The log filename is specified in the ~-dbgto~ and ~-errto~ parameters.
| Depending on the debug level, more or less output will be written.
| The debug level is specified with the ~-D~ parameter.
| The ~-D~ parameter's value contains tokens separated by '+' signs.
|
| The most useful debug tokens, in order of usefulness and verbosity are:
|
| :usr1:	program start and stop and critical messages
| :ipc:		socket actions such as connect and close
| :command:	commands received or sent
| :reply:	replys from commands
| :usr2:	one line for each frame transmitted
| 
|.EXAMPLES
| Here is an example arg file:
| 
|:   -connect rocket,206.251.105.8:13745
|:   -ident ABCD1234
|:   -input jpeg/room1/cam0.%08ld.rgb888.enc
|:   -name rocket,129.241.102.2:0
|:   -max 100
|:   -pre rocket,206.251.105.36:80/start.asp?
|:   -output rocket,206.251.105.36:80/done.asp?
|:   -variant .au
|:   
|:   -dt 60000
|:   -t 25
|:   -w 2000
|:   -D usr1
|:   
|:   -dbgto logs/perdrona.log
|:   -errto logs/perdrona.log
|:   -sayto logs/perdrona.log
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <tabdrone.h>
DT_RCSID("tabdrone $RCSfile: tabdrone.c,v $ $Revision: 1.8 $");
#include <dtack/rev.h>

#include <dtack/main.h>
#include <dtack/errno.h>
#include <dtack/licens.h>

#include <tabdrone.use>

#include <dtack/list.h>						
#include <dtack/dg.h>						/* for rev only 					*/

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#ifndef DTDEPEND
#include <signal.h>
#endif
#endif

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

#define APPNAME_TRIM(S)                                                       \
  {                                                                           \
    char *p;                                                                  \
	p = strrchr((S), '.');				/* chop off filetype				*/\
	if (p != NULL)                                                            \
	  *p = '\0';                                                              \
	p = strrchr((S), '/');				/* trim off path before '/'			*/\
	if (p != NULL)                                                            \
	  memmove((char *)(S), p+1,                                               \
        strlen(p));                                                           \
	p = strrchr((S), '\\');				/* trim off path before '\'			*/\
	if (p != NULL)                                                            \
	  memmove((char *)(S), p+1,                                               \
        strlen(p));                                                           \
  } 

static
dtrev_f *revs[] = {
  tabdrone_rev,
  dtdg_rev,
  NULL
};

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtprocs_notify(
  dt_ctl_t *ctl,
  void *procs)
{
  DT_F("dtprocs_notify");
  int sysrc;
  int e;

  sysrc = kill(getppid(), SIGUSR1);
  e = errno;
  if (sysrc != 0)
	return dt_err(ctl, F,
      "kill(%d, %d)"
      DTERRNO_FMT,
      getppid(), SIGUSR1,
      DTERRNO_ARG(e));
  return DT_RC_GOOD;
										/* ................................ */
#ifdef DTCONFIG_IMPLIED_MSW32
  HANDLE lock = NULL;
  char s[256];
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  *s = '\0';
  GetEnvironmentVariable(
    "DTPROCS_LOCK", s, sizeof(s));
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "DTPROCS_LOCK=\"%s\"", s);

  if (s != NULL &&
	  *s != '\0')
  {
	lock = OpenSemaphore(				/* access the semaphore				*/
      SEMAPHORE_ALL_ACCESS, FALSE,
      s);
	if (lock == NULL)					/* could not get semaphore?			*/
	{
	  DWORD e = GetLastError();
	  char s[256];
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
	    "OpenSemaphore("
        "SEMAPHORE_ALL_ACCESS, FALSE, "
        "\"%s\")"
        DTERRNO_LASTFS,
        s,
        DTERRNO_LASTES(e, s));
	  goto cleanup;
	}
  
	ReleaseSemaphore(lock, 1,			/* release access					*/
	  NULL);
  }
  
cleanup:
  if (lock != NULL)
	CloseHandle(lock);
  return rc;
#endif
}

/*..........................................................................
 *..........................................................................*/

static
void
tabdrone_stamp(							/* output debug message				*/
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
tabdrone_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabdrone_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = tabdrone_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
tabdrone_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  tabdrone_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,				/* log the error 					*/
    format, ap);
  ctl->err = tabdrone_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
tabdrone_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  tabdrone_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = tabdrone_say;				/* come back here next time 		*/
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
tabdrone_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  if (!strcmp(param->op, "rev"))
	return DT_RC_GOOD;
  
  if (param->connect == NULL ||
	  strlen(param->connect) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->connect_desc);
    *start = param->connect_desc;
	return DT_RC_GOOD;
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(tabdrone)
{
  DT_F("tabdrone");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char *appname = NULL;
  dtlicense_t license;
  long rep;
  int op;
  dt_rc_e rc = DT_RC_GOOD;

  license.initialized = 0;

  {
	int pid;
	ctl->progname = argv[0];			/* make sure our name is correct 	*/
	DT_Q(dtos_getpid,(ctl, &pid));
  }

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* listen address 					*/
      "IPC spec", NULL, 
      "",
    &param->config,						/* config (license) directory		*/
      "Config dir", NULL, 
      ".",
    &param->ident,						/* client cookie (sessid)			*/
      "Client cookie", NULL, 
      "cookie",
    &param->name,						/* client IP address (client)		*/
      "Client ipaddr", NULL, 
      "0.0.0.0",
    &param->max,						/* max seconds or frames (limit)	*/
      "Frame/time limit", NULL, 
      "0",
    &param->input,						/* source for frames (streamid)		*/
      "Frame source", NULL, 
      "tabgrab.dat",
    &param->variant,					/* audio filetype suffix with dot	*/
      "Audio suffix", NULL, 
      ".au",
    &param->precision,					/* URL to hit when start		*/
      "Start URL", NULL, 
      "",
    &param->output,						/* URL to hit when done (done)		*/
      "Done URL", NULL, 
      "",
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->t, 
      "Jpeg quality", NULL, 
      "25",
    &param->dt, 
      "Accept timeout", NULL, 
      "3000",
    &param->w,							/* 100% wait milliseconds 			*/
      "Full wait", NULL, 
      "2000",
    &param->op,
      "Operation", NULL, 
      "normal",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dtos_strdup,(ctl, argv[0], 
    &appname));
  APPNAME_TRIM(appname);
  
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, appname, usage, param,
    arg, tabdrone_arg_check, NULL));
										/* ................................ */
  ctl->dbg = tabdrone_dbg;				/* special logging needs 			*/
  ctl->err = tabdrone_err;
  ctl->say = tabdrone_say;

  DT_C(dtprocs_notify,(ctl, NULL));		/* notify parent 					*/

  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
  {
	DT_C(dtrev_say,(ctl, revs)); 
  }
  else									/* op is to be a server? 			*/
  {
	DT_C(dtlicense_request,(ctl,		/* request a tableau license 		*/
      &license, param->config, 
      ctl->progname));
    DT_C(tabdrone_server,(param));
  }
										/* ................................ */
cleanup:
  DT_I(dtlicense_release,(ctl,			/* release any license 				*/
    &license));

  if (appname != NULL)
	DT_I(dtos_free2,(ctl, appname,
      F, "appname"));
  
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

  dt_dbg(ctl, F, 0, NULL);				/* close files 						*/
  dt_say(ctl, F, NULL);
  dt_err(ctl, F, NULL);
  
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
