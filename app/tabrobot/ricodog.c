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
| ricodog - client/server IP address mapper
| 
|.SYNOPSIS
|:ricodog [params] -connect host:port
|
|.SUMMARY
| Handle specialized PTZ socket commands.
| 
|.PARAMETERS
| :-connect:	server to try to connect to
| :-output:		serial port where duet is
| :-variant:	serial port setup string
| :-n:			duet OFF byte
| :-y:			duet ON byte
| :-epsilon:	connect timeout (seconds)
| :-dx:			dwell time while duet is off (seconds)
| :-dt:			time between polls (seconds)
|dtinclude(params1.dx)
|
|.EXAMPLES
| Server:
|:  ricodog -connect 156.46.122.16:5790
| 
| Client:
|:  ricodog -connect 156.46.122.16:5300 -name myname
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("ricodog $RCSfile: ricodog.c,v $ $Revision: 1.8 $");
#include <dtserial.h>
#include <dtack/rev.h>

#include <dtack/main.h>

#include <ricodog.use>

#define dt_interactive_parse			/* don't use dialog 				*/\
  dt_enter_parse

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
ricodog_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "ricodog app";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 1;
  if (rev_date != NULL)
    *rev_date = 19980503;
  return DT_RC_GOOD;
}

static
dtrev_f *revs[] = {
  ricodog_rev,
  dtipc_rocket_rev,
  NULL
};

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ricodog_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("ricodog_arg_check");

  if (param->op != NULL &&				/* op is given? 					*/
	  !strcmp(param->op, "rev"))		/* user just wants revision? 		*/
	return DT_RC_GOOD;

  if (param->connect == NULL ||
	  strlen(param->connect) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\" (-connect)",
      param->connect_desc);
    *start = param->connect_desc;
	return DT_RC_GOOD;
  }
  
  if (param->output == NULL ||
	  strlen(param->output) == 0)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\" (-output)",
      param->output_desc);
    *start = param->output_desc;
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
  
DTMAIN1(ricodog)
{
  char F[32];
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtserial_t serial;
  dt_rc_e rc = DT_RC_GOOD;
  
  serial.initialized = 0;

  DTMAIN_NAME(F);						/* get program name from argv[0]	*/

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
    &param->connect, "IPC spec",         NULL, "",
    &param->output,  "Serial spec",      NULL, "",
    &param->variant, "Serial setup",     NULL, "",
    &param->op,      "Operation",        NULL, "monitor",
    &param->n,       "Duet OFF code",    NULL, "0x00",
    &param->y,       "Duet ON code",     NULL, "0x40",
    &param->epsilon, "Connect timeout",  NULL, "60000",
    &param->dx,      "Duet off dwell",   NULL, "10000",
    &param->dt,      "Poll seconds",     NULL, "60",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */
 
  DT_C(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, ricodog_arg_check, NULL));
										/* ................................ */
  ctl->dbg = tabrobot_dbg;				/* special logging needs 			*/
  ctl->err = tabrobot_err;
  ctl->say = tabrobot_say;

  if (!strcmp(param->op, "rev"))		/* op is to say rev?				*/
  {
	DT_C(dtrev_say,(ctl, revs)); 
  }
  else
  if (!strcmp(param->op, "monitor"))	/* op is to monitor connection		*/
  {
	char off = (char)param->n;
	char on = (char)param->y;
	
	DT_C(dtserial_open,(ctl, &serial,	/* open serial port 				*/
      param->output));
	if (param->variant &&				/* special setup required? 			*/
		param->variant[0])
	  DT_C(dtserial_setup,(ctl,			/* set up serial port 				*/
        &serial, param->variant));

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "monitoring \"%s\""
      " at %g-second intervals",
      param->connect, param->dt);
	
	while(1)
	{
	  dtipc_t ipc;
	  double t;

	  for (t=0; t<param->dt; t++)
	  {
		DT_C(dt_yield,(ctl, "polling"));
		DT_C(dtos_delay,(ctl,			/* poll period 						*/
          1000.0));
	  }

	  DT_G(dtipc_find,(ctl,				/* make connection 					*/
        &ipc,
        param->connect,
        0, 0, param->epsilon));
	  if (rc == DT_RC_GOOD)
	  {
		DT_G(dtipc_unfind,(ctl, &ipc));	/* release connection 				*/
	  }
	  else
	  {
		DT_C(dtserial_write,(ctl,		/* write the OFF code 				*/
          &serial, &off, 1));
		DT_C(dtos_delay,(ctl,			/* delay a bit 						*/
          param->dx));
		DT_C(dtserial_write,(ctl,		/* write the ON code 				*/
          &serial, &on, 1));
		dt_dbg(ctl, F, DT_DBG_MASK_USR2,
          "duet at %s power-cycled",
          param->output);
	  }

	}
  }
  else
	rc = dt_err(ctl, F,
      "invalid op \"%s\"", param->op);
										/* ................................ */
cleanup:
  if (serial.initialized)
	DT_I(dtserial_close,(ctl,
      &serial));

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
