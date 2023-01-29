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

#include <dtack/t.h>
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/ipc.h>
#include <dtack/prj.h>
#include <dtack/label.h>

DT_RCSID("test $RCSfile: dttipc2.c,v $ $Revision: 1.6 $");

#define USAGE "[-Dnnnn] ipctype [token]"

static const char *ops[] = {
  "wait",
  NULL};

#define QNX4_IPC   "qnx4"
#define SOCKET_IPC "socket"

#ifdef DTCONFIG_IMPLIED_UNIXLIKE

#ifdef DTCONFIG_IMPLIED_QNX4
#  include <time.h>						/* for localtime() call 			*/
#endif

#ifdef DTCONFIG_IMPLIED_SUNOS_OLD		/* not in this guy's header files!	*/
extern int strftime(char *, int, 
  const char *, const struct tm *);
#endif

#define VARIANT(N) \
  (!strcmp(param->variant, #N))

#define CAPTURE_MAX (256)				/* max size of error capture 		*/

/*..........................................................................
 *..........................................................................*/

static
void
dttipc_stamp(							/* output debug message				*/
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
 
  dtos_getpid(NULL, &pid);

  dtstr_printf(ctl,						/* tack on the caller name and pid	*/
    s+strlen(s), lmax-strlen(s),
    " [%5d]%s%s", pid, 
    caller? " ": "",
    caller? caller: "");
}

/*..........................................................................
 *..........................................................................*/

static
void
DTCONFIG_API2
dttipc_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  dttipc_stamp(NULL, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = dttipc_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

static
dt_rc_e
DTCONFIG_API2
dttipc_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  dttipc_stamp(NULL, caller, 
    s, sizeof(s));
  ctl->err = NULL;						/* don't come back here 			*/
  rc = dt_err_va(ctl, caller,
    format, ap);
  ctl->err = dttipc_err;				/* come back here next time 		*/
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

static
dt_rc_e
DTCONFIG_API2
dttipc_err_capture(						/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char *s = (char *)ctl->user;
  dtstr_printf_va(NULL,					/* capture the error message 		*/
    ctl->user, CAPTURE_MAX,
    format, ap);
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

static
void
DTCONFIG_API2
dttipc_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  dttipc_stamp(NULL, caller, 
    s, sizeof(s));
  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, caller,				/* let dt_say_va() do all the work 	*/
    format, ap);
  ctl->say = dttipc_say;				/* come back here next time 		*/
}

/*..........................................................................*
 *..........................................................................*/
#else
#define dttipc_dbg NULL
#define dttipc_err NULL
#define dttipc_say NULL
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_server_transact(
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long size,
  double max,
  int *assert_fail)
{
  DT_F("dttipc_server_transact");
  void *b = NULL;
  dt_rc_e rc = DT_RC_GOOD;
 
  DT_Q(dtipc_wait,(ctl, ipc));			/* wait for client to do find	    */

  if (size)								/* packet size for timing 			*/
  {
    double total = 0;
	dtos_time_t mark1, mark2;
    long i;
	char f[80];
	DT_C(dtos_malloc,(ctl, &b, size));	/* get space for buffer 			*/
    for (i=0; i<size; i++)
      ((char *)b)[(int)i] = (char)i;
	DT_C(dtos_time,(ctl, &mark1));
    while(total < max)					/* loop until we are satisfied 		*/
	{
      DT_C(dtipc_send,(ctl, ipc,		/* blast stuff to client			*/
        b, size));
	  total += size;
	}
	DT_C(dtos_time,(ctl, &mark2));
	if (mark2.wall > mark1.wall)
  	  dtstr_printf(ctl, f, sizeof(f),
        "%8.0f bytes in %8.3f seconds"
        " = %6.1fK bytes/sec",
        total, mark2.wall-mark1.wall,
        total / (mark2.wall-mark1.wall) /
        1000.0);
	else
  	  dtstr_printf(ctl, f, sizeof(f),
        "%8.0f bytes in no elapsed time!",
        total);
    dt_dbg(ctl, F, DT_DBG_MASK_TIME,
      "%s", f);
  }
  else									/* size 0 means do cute strings */
  {
    int i;
    char s[32], *r;
    for (i=0; i<10; i++)
    {
      DT_Q(dtipc_recv,(ctl, ipc,		/* first receive from client	    */
        (void **)&r, sizeof(s))); 

      sprintf(s, "olive %3d", i);
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl,
          !strcmp(r, s),
          F, "got \"%s\" expected \"%s\"",
          r, s);
	
  	  DT_Q(dtos_free2,(ctl, r, 
        F, "received data"));

      DT_Q(dtipc_recv,(ctl, ipc,		/* second receive from client	    */
        (void **)&r, sizeof(s))); 

      sprintf(s, "ocher %3d", i);
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl,
          !strcmp(r, s),
          F, "got \"%s\" expected \"%s\"",
          r, s);
	
  	  DT_Q(dtos_free2,(ctl, r, 
        F, "received data"));

      sprintf(s, "orange %3d", i);
      DT_Q(dtipc_send,(ctl, ipc,		/* server can send to client	    */
        s, sizeof(s)));
	}
  }

  DT_Q(dtipc_done,(ctl, ipc));			/* server finishes transaction	    */

cleanup:
  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "buffer"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_client_transact(
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long size,
  double max,
  int *assert_fail)
{
  DT_F("dttipc_client_transact");
  void *b = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  if (size)								/* packet size for timing 			*/
  {
    double total = 0;
	dtos_time_t mark1, mark2;
	char f[80];
	DT_C(dtos_time,(ctl, &mark1));
    while(total < max)					/* loop until we are satisfied 		*/
	{
      DT_C(dtipc_recv,(ctl, ipc,		/* get stuff from server			*/
        &b, size));
      DT_C(dtos_free,(ctl, b));
      b = NULL;
	  total += size;
	}
	DT_C(dtos_time,(ctl, &mark2));
	if (mark2.wall > mark1.wall)
  	  dtstr_printf(ctl, f, sizeof(f),
        "%8.0f bytes in %8.3f seconds"
        " = %6.1fK bytes/sec",
        total, mark2.wall-mark1.wall,
        total / (mark2.wall-mark1.wall) /
        1000.0);
	else
  	  dtstr_printf(ctl, f, sizeof(f),
        "%8.0f bytes in no elapsed time!",
        total);
    dt_dbg(ctl, F, DT_DBG_MASK_TIME,
      "%s", f);
  }
  else									/* size 0 means do cute strings */
  {
	int i;
	char s[32], *r;
	for (i=0; i<10; i++)
	{
	  sprintf(s, "olive %3d", i);
	  DT_Q(dtipc_send,(ctl, ipc,		/* send first synchronous msg	    */
        s, sizeof(s))); 

	  sprintf(s, "ocher %3d", i);
	  DT_Q(dtipc_send,(ctl, ipc,		/* send second synchronous msg	    */
        s, sizeof(s))); 

	  DT_Q(dtipc_recv,(ctl, ipc,		/* third synchronous msg is reply   */
        (void **)&r, sizeof(s))); 

      sprintf(s, "orange %3d", i);
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl,
          !strcmp(r, s),
          F, "got \"%s\" expected \"%s\"",
          r, s);
	
  	  DT_Q(dtos_free2,(ctl, r, 
        F, "received data"));
	}
  }
  
cleanup:
  if (b != NULL)
    DT_I(dtos_free,(ctl, b));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_server");
  dtipc_t ipc;
  int isbackground = !strchr(arg, 'f');	/* any 'f' character means fore 	*/

  if (isback)
    *isback = 0;

  DT_Q(dtipc_register,(ctl, &ipc, 
    name,
    DTIPC_RECV_MAX_DEFAULT, 
	isbackground?
      DTIPC_FLAG_SELF_BACKGROUND: 0));

  if (!isbackground ||					/* we did not ask for background? 	*/
      ipc.flags &						/* or the background is returning? 	*/
      DTIPC_FLAG_SELF_BACKGROUND)
  {
 
    DT_Q(dttipc_server_transact,(ctl,	/* do server-type things 			*/
      &ipc, size, max,
      assert_fail));
	
    DT_Q(dtipc_unregister,(ctl, &ipc));

    if (isback)
      *isback = 1;
  }
  else									/* non-server fork returning? 		*/
    DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_client");
  dtipc_t ipc;
 
  DT_Q(dtipc_find,(ctl, &ipc, 
    name,
    DTIPC_RECV_MAX_DEFAULT, 
    DTIPC_FLAG_SIGNAL_ON_FIND, -1)); 

  DT_Q(dttipc_client_transact,(ctl,
    &ipc, size, max, assert_fail));

  DT_Q(dtipc_unfind,(ctl, &ipc));

  DT_Q(dtos_delay,(ctl, (long)delay));	/* wait til server is good and gone	*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * register but never accept
 *..........................................................................*/

dt_rc_e
dttipc_tardy_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_tardy_server");
  dtipc_t ipc;
  int isbackground = !strchr(arg, 'f');	/* any 'f' character means fore 	*/

  if (isback)
    *isback = 0;

  DT_Q(dtipc_register,(ctl, &ipc, 
    name,
    DTIPC_RECV_MAX_DEFAULT, 
	isbackground?
      DTIPC_FLAG_SELF_BACKGROUND: 0));

  if (!isbackground ||					/* we did not ask for background? 	*/
      ipc.flags &						/* or the background is returning? 	*/
      DTIPC_FLAG_SELF_BACKGROUND)
  {
	DT_Q(dtos_delay,(ctl, 2000));		/* be tardy 						*/
    DT_Q(dtipc_unregister,(ctl, &ipc));
    if (isback)
      *isback = 1;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_impatient_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_intention_client");
  dt_err_f *old_err = ctl->err;
  dtipc_t ipc;
  char capture[CAPTURE_MAX];
  dt_rc_e rc;

  ctl->err = dttipc_err_capture;		/* substitute error capture routine */
  ctl->user = (void *) capture;			/* where to capture the message 	*/

  DT_G(dtipc_find,(ctl, &ipc, 
    name,
    DTIPC_RECV_MAX_DEFAULT, 
    DTIPC_FLAG_SIGNAL_ON_FIND, 
    1000)); 

  ctl->err = old_err;					/* replace error handling function 	*/

  if (rc == DT_RC_GOOD)
  {
	*assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 0,
        F, "client did not time out");
	DT_Q(dtipc_unfind,(ctl, &ipc));
  }
  else
  {
	if (DT_LABEL(DT_LABEL_TIMEOUT, capture))
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 1,
          F, "client correctly timed out");
	else
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0,
          F, "unexpected error: %s",
          ctl->user);
  }

  DT_Q(dtos_delay,(ctl, (long)delay));	/* wait til server is good and gone	*/

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_test1(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_test1");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_server,(ctl, ipcspec,	/* start server in background 		*/
      "", param->n, param->max,
      isback, assert_fail));
	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_client,(ctl, ipcspec,	/* initiate client actions 			*/
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_server,(ctl, ipcspec, 	/* just start server in background 	*/
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_server,(ctl, ipcspec,	/* start server in foreground 		*/ 
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_client,(ctl, ipcspec,	/* presume server running already 	*/
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_test2(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_test2");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_tardy_server,(			/* start server in background 		*/
      ctl, ipcspec,
      "", param->n, param->max,
      isback, assert_fail));
	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_impatient_client,(	/* initiate client actions 			*/
        ctl, ipcspec,
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_tardy_server,(			/* just start server in background 	*/
      ctl, ipcspec, 
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_tardy_server,(			/* start server in foreground 		*/ 
      ctl, ipcspec,
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_impatient_client,(		/* presume server running already 	*/
      ctl, ipcspec,
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
DTCONFIG_API1
dt_rc_e
dttipc_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttipc_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttipc)
{
  DT_F("dttipc");
  dt_ctl_t *ctl = &param->ctl;
  char ipcspec[64];
  dt_enter_arg_t arg[2];
  int assert_fail = 0;
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking             */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want   */
    &param->op, NULL, NULL, "auto",
    &param->variant, NULL, NULL, "1",
    &param->max, "Bytes to exchange", NULL, "0",
    &param->n, "Packet size", NULL, "0",
    &param->dt, "Delay to close", NULL, "200",
    &param->reps, NULL, NULL, "5",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "IPC Spec", "", 
      ipcspec, sizeof(ipcspec),
    NULL));

  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttipc_arg_check, NULL));

  ctl->dbg = dttipc_dbg;
  ctl->err = dttipc_err;
  ctl->say = dttipc_say;
                                        /* ................................ */
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
	  int isback = 0;
	  if (strlen(ipcspec) != 0)			/* explicit ipc wanted? 			*/
	  {
		if (VARIANT(1))
	    DT_G(dttipc_test1,(param,
          ipcspec,
          &isback, &assert_fail))
		else
	    DT_G(dttipc_test2,(param,
          ipcspec,
          &isback, &assert_fail));
	  }
	  else								/* no explicit ipc? 				*/
	  {
		if (!isback)
	    if (dtprj_socket)				/* we can do sockets? 				*/
		{
		  if (VARIANT(1))
  	        DT_G(dttipc_test1,(param,
              SOCKET_IPC ","
              DTPRJ_SOCKET_HOST2 ":"
              DTPRJ_SOCKET_PORT2,
              &isback, &assert_fail))
		    else
  	        DT_G(dttipc_test2,(param,
              SOCKET_IPC ","
              DTPRJ_SOCKET_HOST2 ":"
              DTPRJ_SOCKET_PORT2,
              &isback, &assert_fail));
		}
      
		if (!isback)
	    if (dtprj_qnx4_ipc)				/* we can do qnx4 ipc? 				*/
		{
		if (VARIANT(1))
	      DT_G(dttipc_test1,(param,
            QNX4_IPC ","
            DTPRJ_QNX4_IPC_NAME2,
            &isback, &assert_fail))
		  else
	      DT_G(dttipc_test2,(param,
            QNX4_IPC ","
            DTPRJ_QNX4_IPC_NAME2,
            &isback, &assert_fail));
		}
	  }
	  if (isback)
        break;
	}
  }

                                        /* ................................ */
cleanup:

  DT_I(dt_leave,(param));				/* clean up before leaving          */

  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
