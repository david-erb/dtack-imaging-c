/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <sys/name.h>
#include <sys/vc.h>
#include <sys/kernel.h>
#include <sys/sendmx.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/timers.h>
#include <sys/times.h>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <dtack/base.h>

#include <qbaby.h>

#define REPLYQ(CODE, BUF, N, Q) \
  {qbaby_reply(vid, QBABY_REPLY_ ## CODE, BUF, N, Q);}
#define REPLY(CODE, BUF, N) \
  REPLYQ(CODE, BUF, N, NULL);

#define REPLYRQ(CODE, BUF, N, Q) \
  {qbaby_reply(vid, QBABY_REPLY_ ## CODE, BUF, N, Q); return;}
#define REPLYR(CODE, BUF, N) \
  REPLYRQ(CODE, BUF, N, NULL)

#define ERROR qbaby_error
#define DEBUG qbaby_debug
#define ERRNO_FMT " errno %d (%s)"
#define ERRNO_ARG errno, strerror(errno)
										/* debug levels: 					*/
										/* 1 - errors 						*/
										/* 2 - system resources 			*/
										/* 3 - connections, deaths			*/
										/* 5 - timeout						*/
										/* 6 - request/reply/condition		*/
										/* 7 - data							*/

#define CASE(ENUM, FUNCTION) 			\
  case QBABY_REQUEST_ ## ENUM:			\
    FUNCTION(vid, &r, q, &pending);		/* try to do the function 			*/\
    if (pending)						/* request still pending? 			*/\
	  qbaby_condition_new(vid, &r, q,	/* make a new condition 			*/\
        FUNCTION);						\
    break

typedef struct {						/* buffer descriptor 				*/
  char *wp;
  char *ep;
  int active;
  char sp[QBABY_BUFFER_MAX];
} qbaby_buffer_t;

typedef struct {						/* connection descriptor 			*/
  long serial;
  nid_t server_nid;
  pid_t server_pid;
  pid_t server_vid;
  nid_t client_nid;
  pid_t client_pid;
  pid_t client_vid;
  int server_closing;
  int client_closing;
  int active;
  int id;
  char name[QBABY_NAME_MAX];
  qbaby_buffer_t server_fill_buf;
  qbaby_buffer_t client_fill_buf;
} qbaby_connection_t;

typedef
void
qbaby_handler_f(						/* condition handler 				*/
  pid_t vid, 
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending);

typedef struct {						/* condition descriptor 			*/
  pid_t vid;
  qbaby_request_t r;
  qbaby_connection_t *q;
  qbaby_handler_f *handler;
} qbaby_condition_t;

										/* ................................ */
#define QBABY_CONDITION_MAX (12)
#define QBABY_CONNECTION_MAX (10)

static qbaby_condition_t *clist;
static qbaby_connection_t *qlist;
static long serial = 1;
static int debug_level = 0;

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_error(
  const char *format,
  ...)
{
  va_list ap;
  va_start(ap, format);
  fprintf(stderr, "qbaby: ");
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_debug(
  int level,
  const char *format,
  ...)
{
  va_list ap;
  if (level > debug_level)
    return;
  va_start(ap, format);
  fprintf(stderr, "qbaby: ");
  vfprintf(stderr, format, ap);
  fprintf(stderr, "\n");
  va_end(ap);
}

/*..........................................................................
 * see if given nid, pid is alive
 *..........................................................................*/
static
int
qbaby_alive(
  qbaby_connection_t *q,
  nid_t nid,
  pid_t pid)
{
  pid_t vid = qnx_vc_attach(			/* try to make a vc to the pid		*/
    nid, pid, 0, 0);
  if (vid == (pid_t)(-1))				/* could not make a vc?				*/
  {
	const char *it;
	if (nid == q->server_nid &&
		pid == q->server_pid)
	  it = "server";
	else
	if (nid == q->client_nid &&
		pid == q->client_pid)
	  it = "client";
	else
  	  it = "unknown";
	DEBUG(3, "<%s> %s (nid=%d, pid=%d) has died",
      q->name, it, nid, pid);
    return 0;
  }
  else									/* vc got made ok?					*/
  {
	if (vid != pid)
      qnx_vc_detach(vid);
    return 1;
  }
}

/*..........................................................................
 * create new condition
 *..........................................................................*/
static
void
qbaby_condition_new(
  pid_t vid,
  qbaby_request_t *r,
  qbaby_connection_t *q,
  qbaby_handler_f *handler)
{
  int i;
  for (i=0; i<QBABY_CONDITION_MAX; i++)	/* scan condition list 				*/
  {
	qbaby_condition_t *c = &clist[i];
    if (c->handler == NULL)				/* found an unused one? 			*/
	{
	  c->vid = vid;
	  c->r = *r;
	  c->q = q;
	  c->handler = handler;
	  DEBUG(6, "<%s> added condition slot %d", q->name, i);
	  return;
	}
  }
  REPLY(LISTFULL, NULL, 0)
}

/*..........................................................................
 * create new connection
 *..........................................................................*/
static
qbaby_connection_t *
qbaby_connection_new(					/* create connection 				*/
  pid_t vid,
  qbaby_request_t *r)
{
  int i;
  for (i=0; i<QBABY_CONNECTION_MAX; i++)
  {
	qbaby_connection_t *q = &qlist[i];
    if (!q->active)
	{
	  DEBUG(3, "<%s> connection being created", r->name);
	  memset(q, 0, sizeof(*q));
	  q->id = qnx_name_attach(0, r->name);
	  if (q->id == -1)
	  {
		DEBUG(1, "qnx_name_attach(0, \"%s\")"
          ERRNO_FMT, r->name, ERRNO_ARG);
	    REPLY(ATTACHERROR, NULL, 0);
		return NULL;
	  }
	  strcpy(q->name, r->name);
	  q->active = 1;
	  q->server_nid = r->nid;
	  q->server_pid = r->pid;
	  q->server_vid = vid;
	  
	  q->server_fill_buf.wp =
  	    q->server_fill_buf.sp;
	  q->server_fill_buf.ep =
  	    q->server_fill_buf.sp +
        sizeof(q->server_fill_buf.sp);
	  
	  q->client_fill_buf.wp =
  	    q->client_fill_buf.sp;
	  q->client_fill_buf.ep =
  	    q->client_fill_buf.sp +
        sizeof(q->client_fill_buf.sp);
	  return q;
	}
  }
  REPLY(LISTFULL, NULL, 0);
  return NULL;
}

/*..........................................................................
 * delete a connection
 *..........................................................................*/
static
void
qbaby_connection_delete(
  qbaby_connection_t *q)
{
  if (q->id != 0 &&
	  q->id != -1)
  {
	DEBUG(3, "<%s> connection being deleted", q->name);
    qnx_name_detach(0, q->id);
  }
  
  memset(q, 0, sizeof(*q));
}

/*..........................................................................
 * find existing connection
 *..........................................................................*/
static
qbaby_connection_t *
qbaby_connection_find(
  qbaby_request_t *r)
{
  int i;
  for (i=0; i<QBABY_CONNECTION_MAX; i++)
  {
	qbaby_connection_t *q = &qlist[i];
    if (q->active &&					/* this slot is active? 			*/
		!strcmp(q->name, r->name))		/* and name matches request? 		*/
	{
	  if (qbaby_alive(q,				/* server is still alive? 			*/
			q->server_nid,
			q->server_pid))
	    return q;
	  else								/* server has died? 				*/
	    qbaby_connection_delete(q);		/* delete connection 				*/
	}
  }
  return NULL;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_reply(
  pid_t vid, 
  qbaby_reply_e reply,
  void *buffer, 
  int size,
  qbaby_connection_t *q)
{
  qbaby_reply_t r;
  struct _mxfer_entry mx[2];
  int n;
  int rc;

  r.reply = reply;
  r.size = size;
  
  _setmx(&mx[0], &r, sizeof(r));
  n = 1;
  
  if (size > 0 && buffer != NULL)		/* buffer to be returned? 			*/
  {
    _setmx(&mx[1], buffer, size);
	n++;
  }

  rc = Replymx(vid, n, mx);
  if (q != NULL && rc == -1)			/* reply failed? 					*/
    q->serial = 0;						/* disconnect 						*/

  DEBUG( 
    reply != QBABY_REPLY_OK? 4: 6,
    "<%s> reply %s to vid %d size=%d",
    q != NULL? q->name: "",
    QBABY_REPLY_TO_STRING(&r), vid,
    size);
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_register(
  pid_t vid, 
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  if (q != NULL && q->active)			/* connection already active? 		*/
    REPLYR(EXISTS, NULL, 0);
  q = qbaby_connection_new(vid, r);
  if (q == NULL)
    return;
  REPLYQ(OK, NULL, 0, q);
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_wait(
  pid_t vid, 
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);

  if (vid != q->server_vid)				/* we are not the server? 			*/
    REPLYR(REJECTED, NULL, 0);

  if (q->serial > 0)					/* currently connected? 			*/
  {
	REPLYRQ(OK, NULL, q->serial, q);
  }
  else									/* not currently connected?			*/
    *pending = 1;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_find(
  pid_t vid, 
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);
 
  if (q->serial == 0)					/* not currently connected? 		*/
  {
	q->client_closing = 0;
	q->server_closing = 0;
	q->serial = serial++;
	q->client_nid = r->nid;
	q->client_pid = r->pid;
	q->client_vid = vid;
	REPLYRQ(OK, NULL, q->serial, q);
  }
  else									/* currently connected?				*/
    *pending = 1;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_fill(
  pid_t vid, 
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  qbaby_buffer_t *b;
  int other_closing;
  int n;

  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);

  if (vid == q->server_vid)
  {
    b = &q->server_fill_buf;
	if (q->server_closing)				/* we are already closing?			*/
      REPLYR(CLOSING, NULL, 0);
	other_closing = q->client_closing;
  }
  else
  if (vid == q->client_vid)
  {
    if (q->serial != r->serial)
      REPLYR(NOTCONNECTED, NULL, 0);
	if (q->client_closing)				/* we are already closing?			*/
      REPLYR(CLOSING, NULL, 0);
    b = &q->client_fill_buf;
	other_closing = q->server_closing;
  }
  else
    REPLYR(REJECTED, NULL, 0);

  if (other_closing)					/* connection is closing? 			*/
    REPLYR(SHUTDOWN, NULL, 0);
  
  n = b->ep - b->wp;					/* space available 					*/
  if (n > 0)							/* buffer not full? 				*/
  {
	if (n > r->size)					/* can hold more than we want? 		*/
	  n = r->size;
	n = Readmsg(vid, sizeof(*r),		/* read from the message 			*/
      b->wp, n);

	if (n == -1)
	{
      qbaby_connection_delete(q);		/* this connection is toast 		*/
	  REPLYRQ(READERROR, NULL, 0, q);
	}

	b->wp += n;
	REPLYR(OK, NULL, n);
  }
  else									/* buffer full? 					*/
    *pending = 1;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_drain(
  pid_t vid,
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  qbaby_buffer_t *b;
  int other_closing;
  int n;

  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);
  if (q->serial != r->serial)			/* connection is not ours? 			*/
    REPLYR(NOTCONNECTED, NULL, 0);

  if (vid == q->server_vid)
  {
	if (q->server_closing)				/* we are already closing?			*/
      REPLYR(CLOSING, NULL, 0);
    b = &q->client_fill_buf;
	other_closing = q->client_closing;
  }
  else
  if (vid == q->client_vid)
  {
    if (q->serial != r->serial)
      REPLYR(NOTCONNECTED, NULL, 0);
	if (q->client_closing)				/* we are already closing?			*/
      REPLYR(CLOSING, NULL, 0);
    b = &q->server_fill_buf;
	other_closing = q->server_closing;
  }
  else
    REPLYR(REJECTED, NULL, 0);
  
  n = b->wp - b->sp;					/* bytes available 					*/

  if (n == 0 && other_closing)			/* buffer empty and closing? 		*/
  {
    DEBUG(6, "<%s> data exhausted",
      q->name);
    REPLYRQ(OK, NULL, 0, q);			/* return 0 bytes read 				*/
  }

  if (n > 0)							/* buffer not empty?				*/
  {
	if (n > r->size)					/* got more than we want? 			*/
      n = r->size;
	REPLYQ(OK, b->sp, n, q);

	memmove(b->sp, b->sp+n,				/* shuffle unread bytes down 		*/
      b->wp-b->sp-n);
	b->wp -= n;
  }
  else
    *pending = 1;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_done(
  pid_t vid,
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);

  if (q->serial != r->serial)			/* connection is not ours? 			*/
  {
    REPLYR(NOTCONNECTED, NULL, 0);
  }
  
  if (vid != q->server_vid)				/* we are not the server? 			*/
    REPLYR(REJECTED, NULL, 0);

  if (q->server_closing &&
	  q->client_closing)
    q->serial = 0;						/* mark it unconnected 				*/

  q->server_closing = 1;				/* server is closing 				*/

  if (q->client_closing == 1)			/* client has closed also? 			*/
  {
    REPLYR(OK, NULL, 0);
  }
  else
    *pending = 1;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_unfind(
  pid_t vid,
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);
  if (q->serial != r->serial)			/* connection is not ours? 			*/
    REPLYR(NOTCONNECTED, NULL, 0);

  if (vid != q->client_vid)				/* we are not the client? 			*/
    REPLYR(REJECTED, NULL, 0);

  if (q->server_closing &&
	  q->client_closing)
    q->serial = 0;						/* mark it unconnected 				*/

  q->client_closing = 1;				/* client is closing 				*/

  if (q->server_closing == 1)			/* server has closed also? 			*/
  {
	REPLY(OK, NULL, 0);
  }
  else
    *pending = 1;
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_unregister(
  pid_t vid, 
  qbaby_request_t *r,
  qbaby_connection_t *q,
  int *pending)
{
  if (q == NULL || !q->active)			/* connection not active? 			*/
    REPLYR(NOTFOUND, NULL, 0);

  if (vid != q->server_vid)				/* we are not the server? 			*/
    REPLYR(REJECTED, NULL, 0);

  qbaby_connection_delete(q);			/* this connection is toast 		*/

  REPLY(OK, NULL, 0);
}

/*..........................................................................
 *..........................................................................*/
static
void
qbaby_timer(
  timer_t id,
  long milliseconds)
{
  struct itimerspec timer;
  int sysrc;
  timer.it_interval.tv_sec = 0;			/* only fire once 					*/
  timer.it_interval.tv_nsec = 0;
  timer.it_value.tv_sec = milliseconds/1000;  
  timer.it_value.tv_nsec = (milliseconds%1000)*1000000;
  sysrc = timer_settime(id, 0, &timer, NULL);
  if (sysrc == -1)
  {
	ERROR("unable to set time"
      ", errno %d (%s)",
      errno, strerror(errno));
	exit(1);
  }
}


/*..........................................................................
 *..........................................................................*/

static 
long
qbaby_now()
{
  static long time0 = 0;
  static double tick;
  clock_t now;
  struct tms tmsbuffer;

  now = times(&tmsbuffer);
  if (now == (clock_t)(-1))
  {
	ERROR("times(&tmsbuffer) failed"
      " errno %d (%s)",
      errno, strerror(errno));
	exit(1);
  }
  if (time0 == 0)
  {
    time0 = now;
	tick = sysconf(_SC_CLK_TCK);
	if (tick == 0.0)
      tick = 1.0;
  }

  return 0.5 + (double)(now-time0) * 1000.0 / tick;
}


/*..........................................................................
 *..........................................................................*/

int
main(
  int argc,
  char *argv[])
{
  pid_t proxy;
  struct sigevent event;
  timer_t id;
  int sysrc;
  int sleep = -1;
  int i;

  if (argc < 2)
  {
    ERROR("missing command line argument");
	exit(1);
  }

  sysrc = qnx_name_attach(getnid(),		/* register our name locally 		*/
    argv[1]);
  if (sysrc == -1)
  {
	ERROR("qnx_name_attach(%d, \"%s\""
      ", errno %d (%s)",
      getnid(), argv[1],
      errno, strerror(errno));
    exit(1);
  }

  if (argc >= 3)
    debug_level = atoi(argv[2]);

  DEBUG(1, "registered %s, debug_level=%d",
    argv[1], debug_level);
										/* ................................ */

  proxy = qnx_proxy_attach(
    0, NULL, 0, -1);
  if (proxy == -1)
  {
	ERROR("unable to attach proxy"
      ", errno %d (%s)",
      errno, strerror(errno));
	exit(1);
  }
										/* ................................ */
  memset(&event, 0, sizeof(event));
  event.sigev_signo = -proxy;
  id = timer_create(CLOCK_REALTIME, 
    &event);
  if(id == (timer_t)(-1))
  {
	ERROR("unable to create timer"
      ", errno %d (%s)",
      errno, strerror(errno));
	exit(1);
  }
										/* ................................ */
  clist = (qbaby_condition_t *)
    calloc(QBABY_CONDITION_MAX *
      sizeof(*clist), 1);
  qlist = (qbaby_connection_t *)
    calloc(QBABY_CONNECTION_MAX *
      sizeof(*qlist), 1);

  DEBUG(1, "allocated %d bytes for internal tables",
    QBABY_CONDITION_MAX *
      sizeof(*clist) +
    QBABY_CONNECTION_MAX *
      sizeof(*qlist));
										/* ................................ */

  serial = 1;							/* initial serial number 			*/
  while (1)
  {
	pid_t vid;
	qbaby_request_t r;
	int removed;

	if (sleep != -1)
	{
	  DEBUG(5, "setting wakeup proxy at %ld ms", sleep);
	  qbaby_timer(id, sleep);
	}

	vid = Receive(0, &r, sizeof(r));	/* wait for work 					*/
	if (vid == proxy)					/* proxy? 							*/
	  DEBUG(5, "proxy triggered");
	else
	if (vid != -1)						/* not signal?						*/
	{
	  qbaby_connection_t *q;
	  int pending = 0;
	  DEBUG(6, "<%s> request %s from vid=%d (%d/%d) size=%d, timeout=%d", 
        r.name,
        QBABY_REQUEST_TO_STRING(&r), 
        vid, r.nid, r.pid, 
        r.size, r.timeout);
	  if (r.timeout != -1)
	    r.timeout += qbaby_now();
	  q = qbaby_connection_find(&r);	/* see if connection exists 		*/

	  switch(r.request)					/* handle new request 				*/
	  {
		CASE(REGISTER,   qbaby_register);
		CASE(WAIT,       qbaby_wait);
		CASE(FIND,       qbaby_find);
		CASE(FILL,       qbaby_fill);
		CASE(DRAIN,      qbaby_drain);
		CASE(DONE,       qbaby_done);
		CASE(UNFIND,     qbaby_unfind);
		CASE(UNREGISTER, qbaby_unregister);
		default: REPLY(BADCMD, NULL, 0);
	  }
	}
										/* ................................ */
	for (i=0;							/* scan the connections 			*/
		 i<QBABY_CONNECTION_MAX; 
		 i++)
	{
	  qbaby_connection_t * q =			/* for short 						*/
        &qlist[i];
	  if (q->active)					/* connection is active? 			*/
	  {
		if (!qbaby_alive(q,				/* server has died? 				*/
			  q->server_nid, 
              q->server_pid))
		  qbaby_connection_delete(q);	/* nuke the connection 				*/
		else							/* server still alive? 				*/
		if (q->serial != 0 &&			/* client has connected? 			*/
			!qbaby_alive(q,				/* client has died? 				*/
			  q->client_nid, 
              q->client_pid))
		  q->serial = 0;				/* end the conversation 			*/
	  }
	}
										/* ................................ */
	do									/* cycle condition queue 			*/
	{
	  long now = qbaby_now();			/* current time 					*/
	  removed = 0;
	  sleep = -1;
	  for (i=0;							/* scan the conditions 				*/
		   i<QBABY_CONDITION_MAX; 
		   i++)
	  {
		qbaby_condition_t *c =			/* for short 						*/
          &clist[i];
		if (c->handler != NULL)			/* this condition is active? 		*/
		{
		  int pending = 0;
		  DEBUG(6, "handling condition in slot %d", i);
		  c->handler(c->vid, 
            &c->r, c->q, &pending);

		  if (pending &&				/* this condition not solved? 		*/
			  c->r.timeout != -1)		/* and a timeout is set?		    */
		  {
			long remain =				/* time remaining until timeout 	*/
              c->r.timeout - now;
			if (remain <= 0)			/* request timed out? 				*/
			{
			  pid_t vid = c->vid;
			  REPLYQ(TIMEOUT, NULL, 0,	/* tell sender about timeout 		*/
                c->q);
			  pending = 0;
			}
			else						/* request not timed out yet? 		*/
			if (sleep == -1)
			  sleep = remain;
			else
			if (remain < sleep)
			  sleep = remain;
		  }

		  if (pending &&				/* this condition not solved? 		*/
			  sleep == -1 ||			/* and no timeout is set?		    */
			  sleep > QBABY_DEATH_SLEEP)
		    sleep = QBABY_DEATH_SLEEP;	/* check for death anyway 			*/
		   
		  if (!pending)					/* we solved this condition? 		*/
		  {
			c->handler = NULL;			/* remove condition 				*/
			removed++;
		  }
		}
	  }
	  if (removed)
	    DEBUG(6, "removed %d conditions",
          removed);
	} while(removed);
  }
  return 0;
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
