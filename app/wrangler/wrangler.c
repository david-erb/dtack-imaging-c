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
| perwrangler - video server dispatch
| 
| END
 *..........................................................................*/

/*..........................................................................
| USAGE
| perwrangler - video server dispatch
| 
|.SYNOPSIS
|:perwrangler [params] -connect host:port -ident identity -config dir -path drone
|:or
|:perwrangler @argfile
|
|.SUMMARY
| Accept HTTP commands from trusted server to launch and monitor video push drones.
| 
|.PARAMETERS
| :-connect:	IPC spec to listen on
| :-ident:		identity for debug messages
| :-parent:		trusted IP address (default rocket,0.0.0.0:0 meaning trust anyone)
| :-path:		drone executable name
| :-config:		drone arg file
| :-datadir:	directory to serve documents from
| :-n:			maximum number of simultaneous drones (100)
| :-min:		minimum port (default 10000)
| :-max:		maximum port (default 20000)
| :-dt:			poll time in milliseconds (default 500)
| :-op:			operation, either "normal" or "rev" (default normal)
|dtinclude(params1.dx)
|
|.DESCRIPTION
| The wrangler receives HTTP CGI querystring commands to launch drones.
| It manufactures a random port and starts the drone on that port.
| When the drone is launched and ready, the wrangler returns the port to the caller.
| 
|.CGI SYNTAX
| The wrangler responds to the following CGI commands:
| :?start:	start a drone
| :?state:	return a text status report
| :?exit:	stop wrangler
| 
| For the :?start: GGI command, the following CGI arguments are required:
| 
| :&sessid:		session identifier
| :&streamid:	stream to push
| :&client:		client to accept
| :&limit:		time limit in seconds
| :&start:		url to hit before pushing video
| :&done:		url to get after pushing video
| :&audio:		whether audio is desired
| 
| All CGI arguments are passed to the drone without change.
| This is accomplished by substituting strings for "%s" tokens in
| the drone's arg file given by the :-config: command line parameter.
| 
| Here is a wrangler CGI start example (line broken for readbility):
| 
|:   http://205.251.105.8:5770/start?client=0.0.0.0:0&sessid=ABCD1234
|:     &streamid=jpeg/cap01/cam0.%08ld.rgb888.enc&limit=0
|:     &start=rocket,205.251.105.85:80/start.asp?
|:     &done=rocket,205.251.105.85:80/done.asp?
|
| The wrangler responds to the CGI start command with at least a line of 
| form:
| 
|:   port: 12345
| 
| The port is the random port on which the new drone is listening.
|
|.DRONE INTERFACE
| The wrangler expects the command line parameter :-path: to be a drone arg file. 
| This file must contain special symbols for substitution.
| The substitutions are made strictly in order.
| The order of symbol substitution is:
| 
| 1.	port to listen on
| 2.	sessid to be passed to start and done urls
| 3.	streamid pattern of files to push
| 4.	client expected client spec, i.e. IP address
| 5.	max time to push in secionds
| 6.	start url
| 7.	done url
| 8.	audio suffix 
|
| Here is a valid example of a drone arg file.
| The tokens beginning with '%' are substitution symbols.
| 
|:   -connect rocket,206.251.105.8:%d
|:   -ident %s
|:   -input %s
|:   -name %s
|:   -max %s
|:   -pre %s
|:   -output %s
|:   -variant %s
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
| The following constitutes an example config file:
| 
|:   -connect rocket,0:5718
|:   -ident wrangler
|:   -parent rocket,0.0.0.0:0
|:
|:   -path perdrone/02-13/perdrona
|:   -config config/perdrona.template
|:   
|:   -datadir .
|:   -min 30000
|:   -max 40000
|:   -dt 500
|:   
|:   -D usr1+usr2+command+reply
|:   
|:   -errto logs/perwrangler.log
|:   -dbgto logs/perwrangler.log
|:   -sayto logs/perwrangler.log
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <wrangler.h>
DT_RCSID("wrangler $RCSfile: wrangler.c,v $ $Revision: 1.8 $");
#include <dtack/rev.h>

#include <dtack/main.h>

#include <wrangler.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

static
dtrev_f *revs[] = {
  wrangler_rev,
  dtdg_rev,
  NULL
};

/*..........................................................................
 *..........................................................................*/

static
void
wrangler_stamp(							/* output debug message				*/
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
wrangler_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  wrangler_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = wrangler_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
wrangler_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  wrangler_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, s,				/* log the error 					*/
    format, ap);
  ctl->err = wrangler_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
wrangler_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  wrangler_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = wrangler_say;				/* come back here next time 		*/
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
wrangler_arg_check(						/* check args and params			*/
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
  if (param->ident == NULL ||
	  strlen(param->ident) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->ident_desc);
    *start = param->ident_desc;
	return DT_RC_GOOD;
  }
  if (param->path == NULL ||
	  strlen(param->path) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->path_desc);
    *start = param->path_desc;
	return DT_RC_GOOD;
  }
  if (param->config == NULL ||
	  strlen(param->config) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->config_desc);
    *start = param->config_desc;
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
  
DTMAIN1(wrangler)
{
  DT_F("wrangler");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  long rep;
  int op;
  dt_rc_e rc = DT_RC_GOOD;

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
    &param->ident, 
      "Ident", NULL, 
      "",
    &param->datadir, 
      "Doc path", NULL, 
      "",
    &param->path, 
      "Drone exe", NULL, 
      "",
    &param->config, 
      "Drone arg", NULL, 
      "",
    &param->parent, 
      "Parent ipc spec", NULL, 
      "rocket,0.0.0.0:0",
    &param->n,  
      "Max children", NULL, 
      "100",
    &param->min,  
      "Min port", NULL, 
      "10000",
    &param->max,  
      "Max port", NULL, 
      "20000",
    &param->dt, 
      "Poll time", NULL, 
      "500",
    NULL));

  DT_GI(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op, 
      "Operation", NULL, 
      "server",
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
 
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, "", usage, param,
    arg, wrangler_arg_check, NULL));
										/* ................................ */
  ctl->dbg = wrangler_dbg;				/* special logging needs 			*/
  ctl->err = wrangler_err;
  ctl->say = wrangler_say;


  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
  {
	DT_C(dtrev_say,(ctl, revs)); 
  }
  else
  if (DT_ABBREV(param->op, "server"))	/* op is server? 					*/
  {
	DT_C(wrangler_server,(param));
  }
  else
  {
#ifdef NONO
	DT_C(wrangler_client,(param,
      param->op));
#endif
  }
										/* ................................ */
cleanup:
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
