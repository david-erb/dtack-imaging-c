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
| umplay - MPEG player
| 
|.SYNOPSIS
|:umplay -op [fifo | stuff | server | control | file] [params]
|
|.SUMMARY
| Mpeg playback client and server.  Can also do interactive file playback.
| 
|.PARAMETERS
| :-op:			operation
| :-input:		MPEG input file
| :-connect:	ipc spec to adopt or seek
| :-variant:	umdec spec
| :-path:		fifo names in form datafifo,cmdfifo,repfifo
| :-config:		optional config file
| :-screen:		screen spec for software display
| :-keyboard:	keyboard spec (default is operating system specific)
| :-reps:		number of times to repeat (default 1)
|dtinclude(params1.dx)
|
|.DESCRIPTION
| The program does one operation per invocation.
| The operations are:
| 
| :-op fifo:	become the 3-fifo server (rundos uplay replacement)
| :-op stuff:	test fifo server by stuffing data and commands into it
| :-op deccmd:	test fifo server by stuffing commands into it
| :-op decdata:	test fifo server by stuffing data into it
| :-op server:	become an ipc-based server
| :-op control:	send commands to an ipc-based server
| :-op file:	interactively play a file
| :-op probe:	just check for hardware
| :-op rev:		print version and quit
|
|.FIFO OPERATION
| In this operation, the program opens two fifos
| for reading and one for writing.
| It then functions as Uniquant's mpeg player server.
| This server can be tested with :umplay -op stuff:.
| 
| The required other parameters for this operation are:
| :-connect:	listen ipc spec
| :-path:		fifo names in form datafifo,cmdfifo,repfifo
| :-variant:	the umdec spec
|
|.STUFF, DECCMD AND DECDATA OPERATIONS
| In these operation, the program opens the complementary fifos to :-op fifo:.
| It then stuffs commands and data into the fifos which cause the server
| to perform specific operations which can be verified visually.
| The script #umplay.sh# runs these operations in the right way.
|
|.SERVER OPERATION
| In this operation, the program listens at an ipc address for commands.
| A corresponding client to this server is :umplay -op control:.
| 
| The required other parameters for this operation are:
| :-connect:	listen ipc address
| :-variant:	the umdec spec
|
|.CONTROL OPERATION
| In this operation, the program acts as client to :umplay -op server:.
| The client gets commands from the command line, the keyboard, or a file.
| Each command is given to the server as a separate conversation.
| This makes it possible to send commands sequentially from a shell script.
| 
| The required other parameters for this operation are:
| :-connect:	ipc address where server is listening
| :-input:		if "key" use keyboard; if a known command send it;
| 				otherwise interpret as a file with lines containing commands
|
| The known commands are:
| :input:		tell server name of file to read
| :forward:		play forward normally
| :fforward:	play forward fast
| :sforward:	play forward slow
| :pause:		pause or single step
| :quit:		stop and close input stream
| :exit:		server should exit
| :status:		return current status string
| 
| If :-input: is "key" and :-keyboard: is not "none", 
| you may interact during the playing.
| Each command for the server is indicated by a single keystroke.
| :i:	tell server name of file to read
| :f:	play forward normally
| :+:	play forward fast
| :-:	play forward slow
| :p:	pause or single step
| :q:	stop and close input stream
| :x:	server should exit
| :s:	return current status string
| :.:	quit client but leave server running
|
|.FILE OPERATION
| In this operation, the program simply reads and plays an mpeg file.
| The required other parameters for this operation are:
| :-input:		name of the file
| :-variant:	the umdec spec
| 
| If :-keyboard: is not "none", you may interact during the playing.
| Each command is indicated by a single keystroke.
| :f:	play forward normally
| :+:	play forward fast
| :-:	play forward slow
| :p:	pause or single step
| :q:	stop and close input stream
| :s:	return current status string
|
|.UMDEC SPEC
| Whenever this program must actually decode an mpeg string,
| it requires a umdec spec.
| This spec is always given as a :-variant: parameter.
| The umdec document gives the full syntax of this spec.
| However, here are some common examples:
|:-variant vp			Videoplex
|:-variant ssg,raw		software I frames only on standard display
|:-variant ssg,P,hws,x	software I and P frames on X Window display
| 
|.CONFIG FILE
| An optional config file may be specified with the :-config: parameter.
| This file consists of lines of the form key:value.
| Empty lines and lines beginning with pound-sign are ignored.
| White space before and after the keys and values is stripped off.
| 
| The program responds only to certain keys.  The others it ignores.
| The set of valid keys is:
| umplay.overlay.text.string		text string to be placed in overlay
| umplay.overlay.text.x0			upper left column for string
| umplay.overlay.text.y0			upper left row for string
| umplay.overlay.text.height		text height in pixels
| umplay.overlay.text.foreground	text foreground like 0x00RRGGBB
| umplay.overlay.text.background	text background like 0x00RRGGBB
| umplay.overlay.image.vp.filename	name of .osd file for Videoplex
| umplay.overlay.image.x0			upper left column for image
| umplay.overlay.image.y0			upper left row for image
| 
|.EXAMPLES
|:umplay -op fifo -path fifo/dat,fifo/cmd,fifo/rep -variant vp
|:umplay -op stuff -path fifo/data,fifo/cmd,fifo/rep
|:umplay -op server -conn rocket,localhost:6111 -variant vp
|:umplay -op control -conn rocket,localhost:6111 -input key
|:umplay -op control -conn rocket,localhost:6111 -input forward
|:umplay -op file -input myfile.mpeg -variant vp
| 
|.REVISION HISTORY
| #19970302 1.2#
| -	fixed bug causing mottled background in text
| - allow very black text background
| -	fixed bug causing initial speed to be ignored
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#ifdef __USAGE
usage:   umplay -op [fifo | stuff | server | control | file] [params]
  Mpeg playback client and server.  Can also do interactive 
  file playback.  
PARAMETERS
  -op             operation 
  -input          MPEG input file 
  -connect        ipc spec to adopt or seek 
  -variant        umdec spec 
  -path           fifo names in form datafifo,cmdfifo,repfifo 
  -config         optional config file 
  -screen         screen spec for software display 
  -keyboard       keyboard spec (default is operating system 
                  specific) 
  -reps           number of times to repeat (default 1) 
for more help, please use -HELP (capitalized)
#endif

#include <umplay.h>
U_R("umplay $RCSfile: app/umplay.c,v $ $Revision: 1.8 $");
#include <dtack/main.h>
#include <dtack/rev.h>

#include <umplay.use>

typedef struct {						/* application structure			*/
  int op;
} umplay_app_t;

#define u_interactive_parse				/* don't use dialog 				*/\
  dt_enter_parse

static
dtrev_f *revs[] = {
  umplay_rev,
  umdec_vp_rev,
  NULL
};

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  U_Q(umplay_param,(param, \
     param->WHICH, \
     param->WHICH ## _desc, \
     param->WHICH ## _flag, \
     WANTED, message, DT_ENTER_MSG_MAXLEN)); \
   if (message[0]) \
   { \
	 *start = param->op_desc; \
	 return U_RC_GOOD; \
   } \
 }

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_op_probe(
  dtparam_t *param)
{
  U_F("umplay_op_probe");
  u_ctl_t *ctl = &param->ctl;
  umdec_t _umdec, *umdec = &_umdec;
  U_Q(umdec_init,(ctl, umdec,			/* create the umdec object			*/
    param->variant, 0L));
  U_Q(umdec_uninit,(ctl, umdec));		/* clean up the umdec object 		*/
  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_op_rev(
  dtparam_t *param)
{
  U_F("umplay_op_rev");
  u_ctl_t *ctl = &param->ctl;
  DT_Q(dtrev_say,(ctl, revs)); 
  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

static const char *ops[] = {			/* values for -op parameter 		*/
  "fifo",
  "stuff",
  "server",
  "control",
  "deccmd",
  "decdata",
  "file",
  "probe",
  "rev",
  NULL
};

typedef 
u_rc_e 
U_API
umplay_op_f(							/* an -op parameter function 		*/
  dtparam_t *param);

static umplay_op_f *op_func[] = {		/* functions for -op parameter 		*/
  umplay_op_fifo,
  umplay_op_stuff,
  umplay_op_server,
  umplay_op_control,
  umplay_op_deccmd,
  umplay_op_decdata,
  umplay_op_file,
  umplay_op_probe,
  umplay_op_rev,
  NULL
};

/*..........................................................................
 *..........................................................................*/

static
void
umplay_stamp(							/* output debug message				*/
  u_ctl_t *ctl,							/* environment control 				*/
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
U_API
umplay_dbg(								/* output debug message				*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->dbg = NULL;						/* don't come back here 			*/
  umplay_stamp(ctl, caller, 
    s, sizeof(s));
  u_dbg_va(ctl, s, class_bits,			/* let u_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = umplay_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

u_rc_e
U_API
umplay_err(								/* handle errors					*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  umplay_t *umplay = 
    (umplay_t *)ctl->user;
  u_rc_e rc;
  char s[64];
  ctl->err = NULL;						/* don't come back here 			*/
  umplay_stamp(ctl, caller, 
    s, sizeof(s));

  if (umplay != NULL &&
	  !umplay->response[0])				/* no error yet? 					*/
  {
    dtstr_printf_va(ctl,				/* keep first error 				*/
      umplay->response,
      sizeof(umplay->response),
      format, ap);
    rc = u_err(ctl, s,					/* log the error 					*/
      "%s", umplay->response);
  }
  else									/* not first error? 				*/
    rc = u_err_va(ctl, s,				/* log the error 					*/
      format, ap);
  ctl->err = umplay_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
U_API
umplay_say(								/* output console message			*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->say = NULL;						/* don't come back here 			*/
  umplay_stamp(ctl, caller, 
    s, sizeof(s));

  u_say_va(ctl, s,
    format, ap);
  ctl->say = umplay_say;				/* come back here next time 		*/
}

/*..........................................................................
 *..........................................................................*/

static
u_rc_e 
umplay_param(
  dtparam_t *param,
  const char *value,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  U_F("umplay_param");
  u_ctl_t *ctl = &param->ctl;
  if (flags & DTPARAM_FLAG_VALUE)
  {
  	if (!wanted)
  	  U_Q(dtstr_printf,(ctl, message, mmax,
        "%s is not allowed for the %s operation.",
        desc, param->op));
  }
  else
  	if (wanted)
  	  U_Q(dtstr_printf,(ctl, message, mmax,
        "%s is required for the %s operation.",
        desc, param->op));

  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_arg_check(						/* check args and params			*/
  u_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  umplay_app_t *app = (umplay_app_t *)vapp;

  if (param->op == NULL ||
	  strlen(param->op) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->op_desc);
    *start = param->op_desc;
	return U_RC_GOOD;
  }

  U_Q(dtstr_choice2,(ctl, ops,
    param->op, &app->op,
    message, DT_ENTER_MSG_MAXLEN));
  if (message[0])
  {
    *start = param->op_desc;
	return U_RC_GOOD;
  }

  if (!strcmp(ops[app->op], "server"))	/* be the ipc server				*/
  {
	PARAM_CHECK(input, 0);
	PARAM_CHECK(connect, 1);			/* command ipc spec 				*/
	PARAM_CHECK(variant, 1);
	PARAM_CHECK(path, 0);
  }

  if (!strcmp(ops[app->op], "fifo"))	/* be the uplay replacement			*/
  {
	PARAM_CHECK(input, 0);
	PARAM_CHECK(variant, 1);			/* umdec spec 						*/
	PARAM_CHECK(path, 1);				/* fifo names						*/
  }

  if (!strcmp(ops[app->op], "stuff"))	/* stuff data and cmds into fifos	*/
  {
	PARAM_CHECK(config, 1);				/* list of commands to do 			*/
	PARAM_CHECK(input, 1);				/* mpeg data filename				*/
	PARAM_CHECK(connect, 0);
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(path, 1);				/* fifo names 						*/
  }

  if (!strcmp(ops[app->op], "deccmd"))	/* stuff into command fifo			*/
  {
	PARAM_CHECK(config, 1);				/* list of commands to do 			*/
	PARAM_CHECK(input, 0);				/* mpeg data filename				*/
/*	PARAM_CHECK(connect, 1);			/* ipc name of decdata process 		*/
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(path, 1);				/* fifo names 						*/
  }

  if (!strcmp(ops[app->op], "decdata"))	/* stuff into data fifo				*/
  {
	PARAM_CHECK(config, 0);		
	PARAM_CHECK(input, 1);				/* mpeg data filename				*/
/*	PARAM_CHECK(connect, 1);			/* our server ipc address 			*/
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(path, 1);				/* fifo names 						*/
  }

  if (!strcmp(ops[app->op], "file"))	/* do the file directly 			*/
  {
	PARAM_CHECK(input, 1);
	PARAM_CHECK(connect, 0);
	PARAM_CHECK(variant, 1);
	PARAM_CHECK(path, 0);
  }

  if (!strcmp(ops[app->op], "control"))	/* send keyboard commands to server	*/
  {
	PARAM_CHECK(input, 1);
	PARAM_CHECK(connect, 1);
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(path, 0);
  }

  if (!strcmp(ops[app->op], "probe"))	/* just check for hardware			*/
  {
	PARAM_CHECK(input, 0);
	PARAM_CHECK(connect, 0);
	PARAM_CHECK(variant, 1);
	PARAM_CHECK(path, 0);
  }

  return U_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(umplay)
{
  U_F("umplay");
  u_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  umplay_app_t umplay_app;
  long rep;
  u_rc_e rc = U_RC_GOOD;

  U_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  U_C(dtparam_init,(param));			/* init param structure             */

  U_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op,							/* program operation				*/
      "Operation", NULL, 
      "",
    &param->input,						/* input file						*/
      "Input spec", NULL, 
      "",
    &param->connect,					/* ipc spec							*/
      "Ipc spec", NULL, 
      NULL,
    &param->variant,					/* umplay device spec 				*/
      "Umplay spec", NULL, 
      "",
    &param->path,						/* reply fifo						*/
      "Reply path", NULL, 
      "",
    NULL));

  U_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->keyboard, 
      "Keybd spec", NULL, 
      "",
    &param->config, 
      "Config file", NULL, 
      "",
    &param->reps, 
      "Repeat", NULL, 
      "1",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  U_C(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, umplay_arg_check, &umplay_app));

  ctl->dbg = umplay_dbg;				/* special logging needs 			*/
  ctl->err = umplay_err;
  ctl->say = umplay_say;

										/* ................................ */
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	U_C((*op_func[umplay_app.op]),
      (param));
  }	
										/* ................................ */
cleanup:
  U_I(dt_leave,(param));				/* clean up before leaving			*/
    
  ctl->dbg_string = NULL;
  ctl->err_string = NULL;
  ctl->say_string = NULL;
  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

  if (rc == U_RC_STOP)					/* we were told to stop?			*/
    rc = U_RC_GOOD;
  
  {
	int check_fail = 0;
    U_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed        */
      &check_fail));
    if (rc == U_RC_GOOD &&
		check_fail)						/* any failed assertions?           */
      rc = U_RC_BAD;
  }

  u_dbg(ctl, F, 0, NULL);				/* close files 						*/
  u_say(ctl, F, NULL);
  u_err(ctl, F, NULL);

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
