/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/t.h>
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/ipc.h>
#include <dtack/prj.h>
#include <dtack/label.h>

DT_RCSID("test $RCSfile: dttipc.c,v $ $Revision: 1.6 $");

#define USAGE "[-Dnnnn] ipctype [token]"

#define WTO_TIMEOUT (1000)
#define FTO_TIMEOUT (1000)
#define CTO_TIMEOUT (500)
#define CTO_DELAY (2000)
#define RTO_TIMEOUT (1000)
#define RTO_DELAY   (2000)
#define PTO_TIMEOUT (500)
#define PTO_DELAY   (2000)
#define SSUICIDE_TIMEOUT (3000)			/* client max wait for suicide 		*/
#define SSUICIDE_DELAY   (500)			/* server delay before suicide 		*/
#define CSUICIDE_TIMEOUT (3000)			/* client max wait for suicide 		*/
#define CSUICIDE_DELAY   (500)			/* server delay before suicide 		*/

static const char *ops[] = {
  "auto",
  "server",
  "fore",
  "client",
  NULL};

#define QNX4_IPC   "qnx4"
#define SOCKET_IPC "socket"

#define VARIANT(N) \
  (!strcmp(param->variant, #N))

#define CAPTURE_MAX (256)				/* max size of error capture 		*/

#ifdef DTCONFIG_IMPLIED_QNX4
#  include <time.h>						/* for localtime() call 			*/
#endif

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

void
DTCONFIG_API1
dttipc_dbg(								/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->dbg = NULL;						/* don't come back here 			*/
  dttipc_stamp(NULL, caller, 
    s, sizeof(s));
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = dttipc_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dttipc_err(								/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  ctl->err = NULL;						/* don't come back here 			*/
  dttipc_stamp(NULL, caller, 
    s, sizeof(s));
  rc = dt_err_va(ctl, s,
    format, ap);
  ctl->err = dttipc_err;				/* come back here next time 		*/
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
dttipc_err_capture(						/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc = DT_RC_BAD;
  char *s = (char *)ctl->user;
  dtstr_printf_va((dt_ctl_t *)NULL,		/* capture the error message 		*/
    (char *)ctl->user, CAPTURE_MAX,
    format, ap);
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
dttipc_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  ctl->say = NULL;						/* don't come back here 			*/
  dttipc_stamp(NULL, caller, 
    s, sizeof(s));
  dt_say_va(ctl, caller,				/* let dt_say_va() do all the work 	*/
    format, ap);
  ctl->say = dttipc_say;				/* come back here next time 		*/
}

/*..........................................................................*
 *..........................................................................*/

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
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
    if (isback)
      *isback = 1;

    DT_Q(dttipc_server_transact,(ctl,	/* do server-type things 			*/
      &ipc, size, max,
      assert_fail));
	
    DT_Q(dtipc_unregister,(ctl, &ipc));
  }
  else									/* non-server fork returning? 		*/
  {
    DT_Q(dtipc_unregister,(ctl, &ipc));
  }

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
dttipc_wto_server(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_wto_server");
  dt_ctl_t *ctl = &param->ctl;
  dt_err_f *old_err;
  dtipc_t ipc;
  dtos_time_t beg, end;
  int isready;

  if (isback)
    *isback = 0;

  DT_Q(dtipc_register,(ctl, &ipc,		/* register our name 				*/
    ipcspec,
    DTIPC_RECV_MAX_DEFAULT, 0)); 

  DT_Q(dtos_time,(ctl, &beg));
  DT_Q(dtipc_poll,(ctl, &ipc,			/* wait for non-existent client		*/
    DTIPC_POLL_ACCEPT, WTO_TIMEOUT,
    "nonexistent client", 
    &isready));
  DT_Q(dtos_time,(ctl, &end));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, !isready && 
      (end.wall-beg.wall>1.0),
      F, "wait ready %d (%d) after %g (%g)",
      isready, 0,
      end.wall-beg.wall, 1.0);
  
  DT_Q(dtipc_unregister,(ctl, &ipc));	/* clean up 						*/

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_fto_client(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_fto_client");
  dt_ctl_t *ctl = &param->ctl;
  dt_err_f *old_err;
  dtipc_t ipc;
  dtos_time_t beg, end;
  int isready;

  if (isback)
    *isback = 0;

  DT_Q(dtipc_register,(ctl, &ipc,		/* register our name 				*/
    ipcspec,
    DTIPC_RECV_MAX_DEFAULT, 0)); 

  DT_Q(dtos_time,(ctl, &beg));
  DT_Q(dtipc_poll,(ctl, &ipc,			/* wait for non-existent client		*/
    DTIPC_POLL_ACCEPT, FTO_TIMEOUT,
    "nonexistent client", 
    &isready));
  DT_Q(dtos_time,(ctl, &end));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, !isready && 
      (end.wall-beg.wall>1.0),
      F, "find ready %d (%d) after %g (%g)",
      isready, 0,
      end.wall-beg.wall, 1.0);
  
  DT_Q(dtipc_unregister,(ctl, &ipc));	/* clean up 						*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * register but never accept
 *..........................................................................*/

dt_rc_e
dttipc_cto_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_cto_server");
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
    if (isback)
      *isback = 1;
	DT_Q(dtos_delay,(ctl,				/* be tardy 						*/
      CTO_DELAY));	
  }
  
  DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_cto_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_cto_client");
  dt_err_f *old_err = ctl->err;
  dtipc_t *ipc = NULL;
# define TRY_MAX (10)
  char capture[CAPTURE_MAX];
  int n = 0;
  int i;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl, 
    (void **)&ipc, 
    TRY_MAX *sizeof(*ipc),
    F, "ipc structures"));

  ctl->err = dttipc_err_capture;		/* substitute error capture routine */
  ctl->user = (void *) capture;			/* where to capture the message 	*/

  for (n=0; n<TRY_MAX; n++)
  {
    DT_G(dtipc_find,(ctl, &ipc[n],		/* second find fails 				*/
      name,
      DTIPC_RECV_MAX_DEFAULT, 
      DTIPC_FLAG_SIGNAL_ON_FIND, 
      CTO_TIMEOUT));
	if (rc != DT_RC_GOOD)
	  break;
  }

  ctl->err = old_err;					/* replace error handling function 	*/

  if (rc == DT_RC_GOOD)
  {
	*assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 0,
        F, "client did not time out");
  }
  else
  {
	if (DT_LABEL_IS(DT_LABEL_TIMEOUT, capture))
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 1,
          F, "client correctly timed out");
	else
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0,
          F, "unexpected error: %s",
          ctl->user);
  }

  DT_Q(dtos_delay,(ctl, CTO_DELAY));	/* wait til server is good and gone	*/
  
cleanup:
  if (ipc != NULL)
  {
    for (i=n-1; i>=0; i--)
      DT_I(dtipc_unfind,(ctl, &ipc[i]));
	DT_I(dtos_free2,(ctl, ipc,
      F, "ipc structures"));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_cto(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_cto");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_cto_server,(			/* start server in background 		*/
      ctl, ipcspec,
      "", param->n, param->max,
      isback, assert_fail));
	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_cto_client,(			/* initiate client actions 			*/
        ctl, ipcspec,
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_cto_server,(			/* just start server in background 	*/
      ctl, ipcspec, 
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_cto_server,(			/* start server in foreground 		*/ 
      ctl, ipcspec,
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_cto_client,(			/* presume server running already 	*/
      ctl, ipcspec,
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
/*..........................................................................
 * register and accept but never write
 *..........................................................................*/

dt_rc_e
dttipc_rto_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_rto_server");
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
    if (isback)
      *isback = 1;

	DT_Q(dtipc_wait,(ctl, &ipc));		/* accept connection 				*/
	
	DT_Q(dtos_delay,(ctl,				/* never write						*/
      RTO_DELAY));	
  }
  
  DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_rto_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_rto_client");
  dtipc_t ipc;
  int ipc_initialized = 0;
  char capture[CAPTURE_MAX];
  dtos_time_t beg, end;
  long elapsed;
  int isready;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, &ipc,			/* connect to server				*/
    name,
    DTIPC_RECV_MAX_DEFAULT, 0, -1));
  ipc_initialized = 1;

  DT_C(dtos_time,(ctl, &beg));
  DT_C(dtipc_poll,(ctl, &ipc,			/* wait for non-existent data		*/
    DTIPC_POLL_READ, RTO_TIMEOUT,
    "nonexistent data", 
    &isready));
  DT_C(dtos_time,(ctl, &end));

  elapsed = (long)(1.0 + 1000.0 *		/* elapsed milliseconds				*/
    (end.wall - beg.wall));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, !isready && 
      elapsed > RTO_TIMEOUT,
      F, "read ready %d (%d)"
      " after %d (%d)",
      isready, 0,
      elapsed, RTO_TIMEOUT);

  DT_Q(dtos_delay,(ctl, RTO_DELAY));	/* wait til server is good and gone	*/
  
cleanup:
  if (ipc_initialized)
    DT_I(dtipc_unfind,(ctl, &ipc));	 

  return rc;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_rto(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_rto");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_rto_server,(			/* start server in background 		*/
      ctl, ipcspec,
      "", param->n, param->max,
      isback, assert_fail));
	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_rto_client,(			/* initiate client actions 			*/
        ctl, ipcspec,
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_rto_server,(			/* just start server in background 	*/
      ctl, ipcspec, 
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_rto_server,(			/* start server in foreground 		*/ 
      ctl, ipcspec,
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_rto_client,(			/* presume server running already 	*/
      ctl, ipcspec,
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
/*..........................................................................
 * register and accept but never read
 *..........................................................................*/

dt_rc_e
dttipc_pto_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_pto_server");
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
    if (isback)
      *isback = 1;

	DT_Q(dtipc_wait,(ctl, &ipc));		/* accept connection 				*/
	
	DT_Q(dtos_delay,(ctl,				/* never write						*/
      PTO_DELAY));	
  }
  
  DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_pto_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_pto_client");
  dtipc_t ipc;
  int ipc_initialized = 0;
  char capture[CAPTURE_MAX];
# define BUFSIZE (8000)					/* not multiple of QBABY_BUFSIZE 	*/
  char *buf = NULL;
  long wrote;
  dtos_time_t beg, end;
  long elapsed;
  int isready;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, &ipc,			/* connect to server				*/
    name,
    DTIPC_RECV_MAX_DEFAULT, 0, -1));
  ipc_initialized = 1;

  DT_C(dtos_malloc2,(ctl,
    (void **)&buf, BUFSIZE,
    F, "buf"));
  
  do {									/* stuff the pipe full 				*/
	DT_C(dtipc_write_some,(ctl, &ipc,
      buf, BUFSIZE, &wrote));
  } while(wrote == BUFSIZE);

  DT_C(dtos_time,(ctl, &beg));
  DT_C(dtipc_poll,(ctl, &ipc,			/* wait for pipe to empty			*/
    DTIPC_POLL_WRITE, PTO_TIMEOUT,
    "nonexistent read", 
    &isready));
  DT_C(dtos_time,(ctl, &end));

  elapsed = (long)(1.0 + 1000.0 *		/* elapsed milliseconds				*/
    (end.wall - beg.wall));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, !isready && 
      elapsed > PTO_TIMEOUT,
      F, "read ready %d (%d)"
      " after %d (%d)",
      isready, 0,
      elapsed, PTO_TIMEOUT);

  DT_Q(dtos_delay,(ctl, PTO_DELAY));	/* wait til server is good and gone	*/
  
cleanup:
  if (buf != NULL)
    DT_I(dtos_free2,(ctl, buf,
      F, "buf"));
  if (ipc_initialized)
    DT_I(dtipc_unfind,(ctl, &ipc));	 

  return rc;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_pto(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_pto");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_pto_server,(			/* start server in background 		*/
      ctl, ipcspec,
      "", param->n, param->max,
      isback, assert_fail));
	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_pto_client,(			/* initiate client actions 			*/
        ctl, ipcspec,
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_pto_server,(			/* just start server in background 	*/
      ctl, ipcspec, 
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_pto_server,(			/* start server in foreground 		*/ 
      ctl, ipcspec,
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_pto_client,(			/* presume server running already 	*/
      ctl, ipcspec,
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
/*..........................................................................
 * register and accept then die
 *..........................................................................*/

dt_rc_e
dttipc_ssuicide_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_ssuicide_server");
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
	DT_Q(dtipc_wait,(ctl, &ipc));		/* accept connection 				*/
	
	DT_Q(dtos_delay,(ctl,				/* never write						*/
      SSUICIDE_DELAY));

	exit(0);							/* die 								*/
  }
  
  DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_ssuicide_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_ssuicide_client");
  dt_err_f *old_err = ctl->err;
  dtipc_t ipc;
  int ipc_initialized = 0;
  char capture[CAPTURE_MAX];
  int isready;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, &ipc,			/* connect to server				*/
    name,
    DTIPC_RECV_MAX_DEFAULT, 0, -1));
  ipc_initialized = 1;

  ctl->err = dttipc_err_capture;		/* substitute error capture routine */
  ctl->user = (void *) capture;			/* where to capture the message 	*/

  DT_G(dtipc_poll,(ctl, &ipc,			/* wait for non-existent data		*/
    DTIPC_POLL_READ, 
    SSUICIDE_TIMEOUT,
    "server to die", 
    &isready));

  ctl->err = old_err;					/* replace error handling function 	*/

  if (rc == DT_RC_GOOD)
  {
	*assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 0,
        F, "server did not die");
  }
  else
  {
	if (DT_LABEL_IS(DT_LABEL_NOTFOUND, capture))
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 1,
          F, "server correctly died");
	else
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0,
          F, "unexpected error: %s",
          ctl->user);
  }

  rc = DT_RC_GOOD;

cleanup:
  if (ipc_initialized)
    DT_I(dtipc_unfind,(ctl, &ipc));	 

  return rc;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_ssuicide(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_ssuicide");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_ssuicide_server,(			/* start server in background 		*/
      ctl, ipcspec,
      "", param->n, param->max,
      isback, assert_fail));
	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_ssuicide_client,(			/* initiate client actions 			*/
        ctl, ipcspec,
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_ssuicide_server,(			/* just start server in background 	*/
      ctl, ipcspec, 
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_ssuicide_server,(			/* start server in foreground 		*/ 
      ctl, ipcspec,
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_ssuicide_client,(			/* presume server running already 	*/
      ctl, ipcspec,
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
/*..........................................................................
 * register and accept then die
 *..........................................................................*/

dt_rc_e
dttipc_csuicide_server(
  dt_ctl_t *ctl,
  const char *name,
  char *arg,
  long size,
  double max,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_csuicide_server");
  dtipc_t ipc;
  int isbackground = !strchr(arg, 'f');	/* any 'f' character means fore 	*/
  dt_err_f *old_err = ctl->err;
  char capture[CAPTURE_MAX];
  int isready;
  dt_rc_e rc;

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
    if (isback)
      *isback = 1;

	DT_Q(dtipc_wait,(ctl, &ipc));		/* accept connection 				*/
	
	ctl->err = dttipc_err_capture;		/* substitute error capture routine */
	ctl->user = (void *) capture;		/* where to capture the message 	*/

	DT_G(dtipc_poll,(ctl, &ipc,			/* wait for non-existent data		*/
      DTIPC_POLL_READ, 
      CSUICIDE_TIMEOUT,
      "client to die", 
      &isready));

	ctl->err = old_err;					/* replace error handling function 	*/

	if (rc == DT_RC_GOOD)
	{
	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0,
          F, "server did not die");
	}
	else
	{
	  if (DT_LABEL_IS(DT_LABEL_NOTFOUND, capture))
  	    *assert_fail += DT_RC_GOOD !=
         dt_assert(ctl, 1,
           F, "server correctly died");
	  else
  	    *assert_fail += DT_RC_GOOD !=
          dt_assert(ctl, 0,
            F, "unexpected error: %s",
            ctl->user);
	}
  }
  
  DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttipc_csuicide_client(
  dt_ctl_t *ctl,
  const char *name,
  long size,
  double max,
  double delay,
  int *assert_fail)
{
  DT_F("dttipc_csuicide_client");
  dtipc_t ipc;
  int ipc_initialized = 0;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, &ipc,			/* connect to server				*/
    name,
    DTIPC_RECV_MAX_DEFAULT, 0, -1));
  ipc_initialized = 1;
  
  DT_Q(dtos_delay,(ctl,					/* never write						*/
    CSUICIDE_DELAY));

  exit(0);								/* die 								*/

cleanup:
  if (ipc_initialized)
    DT_I(dtipc_unfind,(ctl, &ipc));	 

  return rc;
}

/*..........................................................................*/
static
dt_rc_e 
dttipc_csuicide(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_csuicide");
  dt_ctl_t *ctl = &param->ctl;

  if (!strcmp(param->op, "auto"))
  {
	DT_Q(dttipc_csuicide_server,(		/* start server in background 		*/
      ctl, ipcspec,
      "", param->n, param->max,
      isback, assert_fail));

	if (*isback == 0)					/* we aren't the background server?	*/
  	  DT_Q(dttipc_csuicide_client,(		/* initiate client actions 			*/
        ctl, ipcspec,
	    param->n, param->max, 
        param->dt,						/* delay before close			    */
        assert_fail))
  }

  if (!strcmp(param->op, "server"))
  {
	DT_Q(dttipc_csuicide_server,(			/* just start server in background 	*/
      ctl, ipcspec, 
      "", param->n, param->max, 
      isback, assert_fail));
  }

  if (!strcmp(param->op, "fore"))	
  {
	DT_Q(dttipc_csuicide_server,(			/* start server in foreground 		*/ 
      ctl, ipcspec,
      "f", param->n, param->max, 
      NULL, assert_fail));
  }

  if (!strcmp(param->op, "client"))
  {
	DT_Q(dttipc_csuicide_client,(			/* presume server running already 	*/
      ctl, ipcspec,
      param->n, param->max,
      param->dt,						/* delay before close			    */
      assert_fail));
  }

  return DT_RC_GOOD;
}

#ifdef NONO
/*..........................................................................*/
static
dt_rc_e 
dttipc_wait_xxxxx(
  dtparam_t *param,
  const char *ipcspec,
  int *isback,
  int *assert_fail)
{
  DT_F("dttipc_wait_xxxxx");
  dt_ctl_t *ctl = &param->ctl;
  dt_err_f *old_err;
  dtipc_t ipc;
  char capture[CAPTURE_MAX];
  int isready;

  if (isback)
    *isback = 0;

  DT_Q(dtipc_register,(ctl, &ipc,		/* register our name 				*/
    ipcspec,
    DTIPC_RECV_MAX_DEFAULT, 0)); 

  ctl->user = (void *)capture;			/* where to capture the message 	*/
  old_err = ctl->err;
  ctl->err = dttipc_err_capture;		/* substitute error capture routine */

  DT_Q(dtipc_poll,(ctl, &ipc,			/* wait for non-existent client		*/
    DTIPC_POLL_ACCEPT, 1000,
    "nonexistent client", 
    &isready));

  ctl->err = old_err;					/* replace error handling function 	*/

  if (rc == DT_RC_GOOD)					/* wait call didn't fail? 			*/
  {
	*assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 0,
        F, "wait did not time out");
	DT_Q(dtipc_unfind,(ctl, &ipc));
  }
  else									/* wait call failed? 				*/
  {
	if (DT_LABEL_IS(DT_LABEL_TIMEOUT,	/* error label is right? 			*/
		  capture))
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 1,
          F, "wait correctly timed out");
	else								/* error label is wrong? 			*/
  	  *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0,
          F, "unexpected error: %s",
          ctl->user);
  }
  
  DT_Q(dtipc_unregister,(ctl, &ipc));

  return DT_RC_GOOD;
}
#endif
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
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
		if (VARIANT(wto))
	      DT_G(dttipc_wto_server,(
            param, ipcspec,
            &isback, &assert_fail))
		else
		if (VARIANT(fto))
	      DT_G(dttipc_fto_client,(
            param, ipcspec,
            &isback, &assert_fail))
		else
		if (VARIANT(cto))
  	      DT_G(dttipc_cto,( 
            param, ipcspec,
            &isback, &assert_fail))
		else
		if (VARIANT(rto))
  	      DT_G(dttipc_rto,( 
            param, ipcspec,
            &isback, &assert_fail))
		else
		if (VARIANT(pto))
  	      DT_G(dttipc_pto,( 
            param, ipcspec,
            &isback, &assert_fail))
		else
		if (VARIANT(ssuicide))
  	      DT_G(dttipc_ssuicide,( 
            param, ipcspec,
            &isback, &assert_fail))
		else
		if (VARIANT(csuicide))
  	      DT_G(dttipc_csuicide,( 
            param, ipcspec,
            &isback, &assert_fail))
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
		}
      
		if (!isback)
	    if (dtprj_qnx4_ipc)				/* we can do qnx4 ipc? 				*/
		{
		if (VARIANT(1))
	      DT_G(dttipc_test1,(param,
            QNX4_IPC ","
            DTPRJ_QNX4_IPC_NAME2,
            &isback, &assert_fail))
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
