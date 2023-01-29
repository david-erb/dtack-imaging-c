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
| umdemo - client/server IP address mapper
| 
|.SYNOPSIS
|:umdemo [params] -screen spec -input file
|
|.SUMMARY
| Mpeg playback client and server.
| 
|.PARAMETERS
| :-screen:		screen spec (required)
| :-input:		input file (required)
| :-reps:		number of times to repeat (default 0 means forever)
|dtinclude(params1.dx)
|
|.DESCRIPTION
| This program operates as either a server or a client.
| Client operation is assumed unless the -op parameter is "server".
| Otherwise the -op parameter specifies the client operation to perform.
| 
| The following values for the -op parameter are recognized:
| server	act as server
| file		read and play the file
| send		send a file's contents to a server
| tell		tell a server to prepare to read a file
| play		tell a server to play
| stop		tell a server to stop
| pause		tell a server to pause
|
| The -connect parameter always specifies the server's listen address.
| When a server, the program listens at this address.
| When a client, the program attempts to find a server at the address
|
| The -input parameter is used to specify the file to read or send.
| 
| The -variant parameter specifies the umdemo device to play to.
|
| The -screen parameter specifies the display device.
| It is only need for certain variants.
| 
| Each -op parameter value requires certain other parameters.
| This table summarizes these requirements:
| server	connect, variant
| file		input, variant
| send		input, connect
| tell		input, connect
| play		connect
| stop		connect
| pause		connect
|
|.EXAMPLES
| Client:
|:  umdemo -screen hws,x -input myfile.mpg
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <umdemo.h>
U_R("umdemo $RCSfile: app/umdemo.c,v $ $Revision: 1.8 $");
#include <dtack/main.h>

#include <umdemo.use>

static const char *ops[] = {			/* values for -op parameter 		*/
  "fifo",
  "stuff",
  "server",
  "control",
  "file",
  "tell",
  NULL
};

typedef 
u_rc_e 
U_API
umdemo_op_f(							/* an -op parameter function 		*/
  dtparam_t *param);

static umdemo_op_f *op_func[] = {		/* functions for -op parameter 		*/
  umdemo_op_fifo,
  umdemo_op_stuff,
  umdemo_op_server,
  umdemo_op_control,
  umdemo_op_file,
  umdemo_op_tell,
  NULL
};

typedef struct {						/* application structure			*/
  int op;
} umdemo_app_t;

#define u_interactive_parse				/* don't use dialog 				*/\
  dt_enter_parse

#define PARAM_CHECK(WHICH, WANTED) \
{ \
  U_Q(umdemo_param,(param, \
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

/*..........................................................................
 *..........................................................................*/

static
void
umdemo_stamp(							/* output debug message				*/
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
umdemo_dbg(								/* output debug message				*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->dbg = NULL;						/* don't come back here 			*/
  umdemo_stamp(ctl, caller, 
    s, sizeof(s));
  u_dbg_va(ctl, s, class_bits,			/* let u_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = umdemo_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

u_rc_e
U_API
umdemo_err(								/* handle errors					*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  umdemo_t *umdemo = 
    (umdemo_t *)ctl->user;
  u_rc_e rc;
  char s[64];
  ctl->err = NULL;						/* don't come back here 			*/
  umdemo_stamp(ctl, caller, 
    s, sizeof(s));

  if (umdemo != NULL &&
	  !umdemo->response[0])				/* no error yet? 					*/
    dtstr_printf_va(ctl,				/* keep first error 				*/
      umdemo->response, 
      sizeof(umdemo->response),
      format, ap);

  rc = u_err_va(ctl, s,					/* log the error 					*/
    format, ap);
  ctl->err = umdemo_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
U_API
umdemo_say(								/* output console message			*/
  u_ctl_t *ctl,							/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->say = NULL;						/* don't come back here 			*/
  umdemo_stamp(ctl, caller, 
    s, sizeof(s));

  u_say_va(ctl, s,
    format, ap);
  ctl->say = umdemo_say;				/* come back here next time 		*/
}

/*..........................................................................
 *..........................................................................*/

static
u_rc_e 
umdemo_param(
  dtparam_t *param,
  const char *value,
  const char *desc,
  dtparam_flag_t flags,
  int wanted,
  char *message,
  int mmax)
{
  U_F("umdemo_param");
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
umdemo_arg_check(						/* check args and params			*/
  u_ctl_t *ctl,
  void *vapp,							/* app structure 					*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  const char *F = NULL;
  umdemo_app_t *app = (umdemo_app_t *)vapp;

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
	PARAM_CHECK(connect, 1);
	PARAM_CHECK(variant, 1);
	PARAM_CHECK(output, 0);
	PARAM_CHECK(path, 0);
  }

  if (!strcmp(ops[app->op], "fifo"))	/* be the uplay replacement			*/
  {
	PARAM_CHECK(input, 1);				/* data fifo name 					*/
	PARAM_CHECK(connect, 1);			/* command fifo name 				*/
	PARAM_CHECK(variant, 1);			/* umdec spec 						*/
	PARAM_CHECK(output, 0);
	PARAM_CHECK(path, 1);				/* reply fifo name					*/
  }

  if (!strcmp(ops[app->op], "stuff"))	/* stuff data into fifo				*/
  {
	PARAM_CHECK(input, 1);				/* test data filename				*/
	PARAM_CHECK(connect, 1);			/* command fifo name 				*/
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(output, 1);				/* data fifo name					*/
	PARAM_CHECK(path, 1);				/* reply fifo name 					*/
  }

  if (!strcmp(ops[app->op], "file"))	/* do the file directly 			*/
  {
	PARAM_CHECK(input, 1);
	PARAM_CHECK(connect, 0);
	PARAM_CHECK(variant, 1);
	PARAM_CHECK(output, 0);
	PARAM_CHECK(path, 0);
  }

  if (!strcmp(ops[app->op], "tell"))	/* tell server to do the file 		*/
  {
	PARAM_CHECK(input, 1);
	PARAM_CHECK(connect, 1);
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(output, 0);
	PARAM_CHECK(path, 0);
  }

  if (!strcmp(ops[app->op], "control"))	/* send keyboard commands to server	*/
  {
	PARAM_CHECK(input, 1);
	PARAM_CHECK(connect, 1);
	PARAM_CHECK(variant, 0);
	PARAM_CHECK(output, 0);
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
  
DTMAIN1(umdemo)
{
  U_F("umdemo");
  u_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  umdemo_app_t umdemo_app;
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
      "",
    &param->screen,						/* display spec 					*/
      "Screen spec", NULL, 
      "",
    &param->variant,					/* umdemo device spec 				*/
      "Umdemo spec", NULL, 
      "",
    &param->output,						/* output fifo						*/
      "Data output", NULL, 
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
    &param->n, 
      "Frame max", NULL, 
      "0",
    &param->reps, 
      "Repeat", NULL, 
      "1",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  U_C(dt_enter_parse,(					/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, umdemo_arg_check, &umdemo_app));

  ctl->dbg = umdemo_dbg;				/* special logging needs 			*/
  ctl->err = umdemo_err;
  ctl->say = umdemo_say;
 
										/* ................................ */
  for (rep=0; 
	   param->reps == 0 ||
	   rep<param->reps; 
	   rep++)
  {
	U_C((*op_func[umdemo_app.op]),
      (param));
  }	
										/* ................................ */
cleanup:
  U_I(dt_leave,(param));				/* clean up before leaving			*/
    
  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

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
