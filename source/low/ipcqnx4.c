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
| DESCRIPTION
| As yet un<documented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: qnx4.c,v $ $Revision: 1.1 $");
#include <dtack/ipc.h>

#include <dtack/os.h>
#include <dtack/str.h>

#ifdef DTCONFIG_IMPLIED_QNX4

#ifndef DTDEPEND
#include <sys/name.h>
#include <sys/vc.h>
#include <sys/kernel.h>
#include <sys/sendmx.h>
#include <sys/select.h>
#endif

#define MSG_MAX (16384)
#endif

typedef struct {
  char *bytes;
  long have;
  long offset;
  long name_id;
  char name[64];
} priv_t;

#if DTPRJ_QNX4_IPC != DT_1
/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_qnx4_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
{
  DT_F("dtipc_qnx4_init");

  return dt_err_notlinked(ctl, F,
    "qnx4 ipc");
}

#else

/*..........................................................................
 * timeout of -1 means wait forever
 *..........................................................................*/
static
dt_rc_e
dtipc_qnx4_poll(						/* wait for socket to be active		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  dtipc_poll_e what,
  long milliseconds,
  const char *waiting_for,
  int *isready)
{
  DT_F("dtipc_qnx4_poll");
#ifdef NONO
  fd_set fds;
  int in = what == DTIPC_POLL_ACCEPT || 
           what == DTIPC_POLL_READ;
  int sockhandle;
  int first = -1;
  int ready = -1;

  if (what == DTIPC_POLL_ACCEPT)
  {
    return DT_RC_GOOD;
  }
  if (what == DTIPC_POLL_CONNECT)
  {
    return DT_RC_GOOD;
  }
  if (what == DTIPC_POLL_READ)
  {
    return DT_RC_GOOD;
  }
  if (what == DTIPC_POLL_WRITE)
  {
    return DT_RC_GOOD;
  }

  if (what == DTIPC_POLL_ACCEPT ||
	  what == DTIPC_POLL_CONNECT)
    sockhandle = ipc->selectfd;
  else
    sockhandle = ipc->datahandle;

  if (sockhandle == -1)
    return dt_err(ctl, F,
      "invalid sockhandle -1:"
      " probably wrong poll type %d",
      what);

  do {
	struct timeval timeout;
	int sysrc;

	if (first == 1 && 
		DT_DBG(ctl, DT_DBG_MASK_IPC))
	{	
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "waiting for %s on sockhandle %d",
        waiting_for, sockhandle);
	  first = 0;
	}

	if (first != -1)
	{
	  DT_Q(dt_yield,(ctl, waiting_for));
	}

	if (milliseconds == 0)				/* no waiting?						*/
	{
	  timeout.tv_sec = 0;				/* loop poll time	 				*/
	  timeout.tv_usec = 0;
	}
	else
	if (milliseconds == -1)				/* waiting forever? 				*/
	{
	  timeout.tv_sec = 1;				/* loop poll time one second		*/
	  timeout.tv_usec = 0;
	}
	else
	if (milliseconds < 1000)			/* very short wait?					*/
	{
	  timeout.tv_sec = 
        milliseconds/1000;
	  timeout.tv_usec = 
        (milliseconds%1000)*1000;
	}
	else								/* waiting longer than one sec?	    */
	{
	  timeout.tv_sec = 1;				/* loop poll time one second		*/
	  timeout.tv_usec = 0;
	}

	FD_ZERO(&fds);
	FD_SET(sockhandle, &fds);			/* wait for activity				*/
	sysrc = select(sockhandle+1,
      in? &fds: NULL, 
      in? NULL: &fds, 
      NULL, &timeout);
	if (sysrc == -1 &&
		errno == EINTR)
	{	
	  if (signal_sigchld == 1)			/* a child process died?		    */
	  {
		signal_sigchld = 0;
		DT_Q(dtipc_qnx4_child,(ctl,
          ipc));
	    continue;
	  }
	  else								/* some other signal? 				*/
        return dt_err(ctl, F,
	      "%s got unexpected signal",
          priv->name);
	}
	if (sysrc == -1)					/* other failure in select()?	    */
      return dt_err(ctl, F,
        "%s select(%d, fds,"
        " NULL, NULL, (%ld,%ld))"
        " waiting for %s"
        " gave errno %d",
        priv->name,
        sockhandle+1,
        timeout.tv_sec, timeout.tv_usec,
        waiting_for,
        LASTERROR);

	ready = FD_ISSET(sockhandle, &fds);

	if (first == -1)
	  first = 1;

	if (milliseconds == 0)				/* this is a poll?				    */
	  break;

	if (milliseconds != -1)				/* not waiting forever?			    */
	{
	  milliseconds -= 1000;
	  if (milliseconds <= 0)			/* waited long enough?			    */
	    break;
	}

  } while(!ready);						/* input not yet available? 		*/

  if (isready != NULL)					/* caller wants to know? 			*/
    *isready = ready;
#endif
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_register(					/* register service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name)
{
  DT_F("dtipc_qnx4_register");
  priv_t *priv = (priv_t *)ipc->priv;
  int sysrc;
  pid_t ppid;
  char c;

  if (name == NULL)
    return dt_err(ctl, F,
      "NULL registered name");
  else
  if (strlen(name) == 0)
    return dt_err(ctl, F,
      "zero-length registered name");

  if (strlen(name) > sizeof(priv->name))
    return dt_err(ctl, F,
      "ipc name \"%s\" too long",
      name);

  strcpy(priv->name, name);              /* remember the ipc name            */
  ipc->type = DTIPC_TYPE_QNX4;

  if (ipc->flags &						/* supposed to background ourself?	*/
      DTIPC_FLAG_SELF_BACKGROUND)
  {
    sysrc = fork();
    if (sysrc == -1)
      return dt_err(ctl, F,
        "fork() failed errno %d (%s)",
        errno, strerror(errno));
    else
    if (sysrc != 0)						/* we are the parent?			    */
    {
      pid_t child = sysrc;
      sysrc = Receive(child, &c, 1);	/* sit around waiting for child		*/
      if (sysrc == -1)
        return dt_err(ctl, F,
          "%s Receive(%d, &c, 1)"
          " from child failed errno %d (%s)",
		  priv->name,
          child, errno, strerror(errno));
      if (sysrc != child)
        return dt_err(ctl, F,
          "%s Receive(%d, &c, 1)"
          " from child gave rc %d",
		  priv->name,
          child, sysrc);
      sysrc = Reply(child, &c, 1);
      if (sysrc == -1)
        return dt_err(ctl, F,
          "%s Reply(%d, &c, 1) to child failed errno %d (%s)",
  		  priv->name,
          child, errno, strerror(errno));

	  if (DT_DBG(ctl, DT_DBG_MASK_IPC))
	  {
  	    DT_Q(dtos_delay,(ctl, 1000));	/* keep output from interleaving 	*/
		dt_dbg(ctl, F, DT_DBG_MASK_IPC,
          "%s parent (%d) now has"
          " confirmation of child (%d)",
          priv->name, getpid(), child);
	  }

      ipc->flags &=						/* let caller know this is stub	    */
        ~DTIPC_FLAG_SELF_BACKGROUND;

      return DT_RC_GOOD;
    }

    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "%s child (%d) taking over",
      priv->name, getpid());
  }

  sysrc = qnx_name_attach(0, name);

  if (sysrc == -1)
    return dt_err(ctl, F, 
      "qnx_name_attach(0, \"%s\") failed errno %d (%s)",
      name, errno, strerror(errno));

  priv->name_id = sysrc;

  ipc->selectfd = -1;					/* we can't select for ipc input	*/

  if (ipc->flags &						/* we backgrounded ourselves?		*/
      DTIPC_FLAG_SELF_BACKGROUND)
  {
    ppid = getppid();
    sysrc = Send(ppid, &c, &c, 1, 1);	/* tell parent it can leave         */
    if (sysrc == -1)
      return dt_err(ctl, F,
        "%s Send(%d, &c, &c, 1, 1) to child failed errno %d (%s)",
	    priv->name,
        ppid, errno, strerror(errno));
    else
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "%s Send to parent %d is done",
	    priv->name,
        ppid);
  }

  return DT_RC_GOOD;
}


/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_release(						/* release allocated resources		*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtipc_qnx4_release");
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
	if (priv->bytes != NULL)
      DT_I(dtos_free2,(ctl, priv->bytes,
        F, "received data"));
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_unregister(					/* unregister service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_qnx4_unregister");
  priv_t *priv = (priv_t *)ipc->priv;

  if (priv->name_id != 0)
  {
    int sysrc;
    sysrc = qnx_name_detach(0, 
      priv->name_id);
    if (sysrc == -1)
      dt_err(ctl, F, 
        "qnx_name_detach(0, 0x%08lx)"
        " failed errno %d (%s)",
        priv->name_id, errno, strerror(errno));
    else
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "\"%s\" unregistered",
        priv->name);
  }

  DT_MEMSET(priv->name, 0,					/* clear name					    */
    sizeof(priv->name));

  ipc->type = DTIPC_TYPE_NONE;

  DT_Q(dtipc_qnx4_release,(ctl, priv));	/* release resources 				*/
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_unfind(						/* unfind registered service name	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_qnx4_unfind");
  priv_t *priv = (priv_t *)ipc->priv;
  int rc;

  rc = qnx_vc_detach(ipc->handle.id);
#ifdef NONO
  if (rc == -1)
    dt_err(ctl, F, 
      "qnx_vc_detach(%ld) failed errno %d (%s)",
      ipc->handle.id, 
      errno, strerror(errno));
#endif

  DT_MEMSET(priv->name, 0,					/* clear name					    */
    sizeof(priv->name));

  ipc->type = DTIPC_TYPE_NONE;

  DT_Q(dtipc_qnx4_release,(ctl, priv));	/* release resources 				*/

  return DT_RC_GOOD;
}


/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_done(						/* server is done with connection   */
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_qnx4_done");
  int rc;

  rc = qnx_vc_detach(ipc->handle.id);
  
#ifdef NONO
  if (rc == -1)
    dt_err(ctl, F, 
      "qnx_vc_detach(%ld) failed errno %d (%s)",
      ipc->handle.id, 
      errno, strerror(errno));
#endif

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_isipc(							/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  unsigned long id,
  dtipc_header_t *h,
  int *isipc)
{
  DT_F("dtipc_qnx4_isipc");

  if (h->magic != DTIPC_MAGIC)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "bad header magic 0x%08lx,"
      " presuming message not coming from dtipc",
      h->magic);
    *isipc = 0;
  }
  else
  if (h->total != 0)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "header total %ld is not 0,"
      " presuming message not coming from dtipc",
      h->total);
    *isipc = 0;
  }
  else
  if (h->length != 0)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "header length %ld is not 0",
      h->length);
    *isipc = 0;
  }
  else
  {
    dtipc_ack_t ack = DTIPC_ACK;
    int sysrc = Reply(id, &ack, sizeof(ack));
    if (sysrc == -1)
    {
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "Reply errno %d (%s)",
        errno, strerror(errno));
      *isipc = 0;
    }
    else
    {
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "ipc find from id 0x%08lx", id);
      ipc->handle.id = id;
      ipc->type = DTIPC_TYPE_QNX4;
      *isipc = 1;
    }
  }

  return DT_RC_GOOD;
}


/*..........................................................................
| This function should not be called by a server unless only dtipc
| connect requests are expected.
| Any kind of incoming message which is not recognized as dtipc
| is discarded, with no reply to the sender.
| This case actually occurs in the :dttwin: test program.
| It turns out that the unrecognized messages are actually proxies from
| QNX windows and can be safely ignored.
 *..........................................................................*/
static
dt_rc_e
dtipc_qnx4_wait(						/* wait for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_qnx4_wait");
  dtipc_header_t h;
  pid_t id;
  int isipc;

  do {
    id = Receive(0, &h, sizeof(h));

    if (id == (pid_t)(-1))
      return dt_err(ctl, F, 
        "Receive(0, &h, sizeof(h))"
        " failed errno %d (%s)",
        errno, strerror(errno));

    DT_Q(dtipc_qnx4_isipc,(ctl, ipc,	/* see if it's one of ours			*/
      id, &h, &isipc));
    if (!isipc)							/* not one of ours?					*/
      dt_dbg(ctl, F, DT_DBG_MASK_IPC,
        "ignoring a non-dtipc message");
  } while (!isipc);

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_find(						/* find registered service name		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long timeout)
{
  DT_F("dtipc_qnx4_find");
  priv_t *priv = ipc->priv;
  dtipc_header_t h;
  int sysrc;
  const char *namepart;
  long node;
  dtipc_ack_t ack;

  if (name == NULL)
    return dt_err(ctl, F,
      "NULL registered name");
  else
  if (strlen(name) == 0)
    return dt_err(ctl, F,
      "zero-length registered name");

  namepart = strchr(name, ':');			/* see if there is a colon 			*/
  if (namepart == NULL)					/* no colon? 						*/
  {
	namepart = name;					/* expect whole name registered 	*/
	node = 0;							/* rely on nameloc if not local 	*/
  }
  else
  {
	int l;
	char nodepart[32];
	int isnum;
	
    if (strlen(namepart) == 1)
      return dt_err(ctl, F,
        "zero-length name part"
        " after colon in \"%s\"", 
        name);
	l = (int)(namepart-name);
	namepart++;							/* point past the colon 			*/
	if (l > sizeof(nodepart)-1)
      return dt_err(ctl, F,
        "node part too long"
        " before colon in \"%s\"", 
        name);
	strncpy(nodepart, name, l);			/* get just the number part 		*/
	nodepart[l] = '\0';
    
	DT_Q(dtstr_to_long2,(ctl,			/* see if name is a node number 	*/
      nodepart, &node, &isnum));
	
	if (!isnum)							/* not a port number? 				*/
      return dt_err(ctl, F,
        "node part not a number"
        " before colon in \"%s\"", 
        name);
  }

  ipc->handle.id = qnx_name_locate(		/* attempt to find the name 		*/
    (nid_t)node, namepart, 0, NULL);

  if (ipc->handle.id == -1)
  {
    if (errno == ESRCH)
      return dt_err(ctl, F,
        "dtipc qnx4 process registering"
        " \"%s\" on node %ld"
        " is not running",
        namepart, (long)node);
    else
      return dt_err(ctl, F, 
        "qnx_name_locate(%ld,"
        " \"%s\", 0, NULL)"
        " failed errno %d (%s)",
        (long)node, name, errno, strerror(errno));
  }

  h.magic = DTIPC_MAGIC;
  h.total = 0;
  h.length = 0;
  h.pid = getpid();
  sysrc = Send(ipc->handle.id, &h, &ack, sizeof(h), sizeof(ack));
  if (sysrc == -1)
    return dt_err(ctl, F, "Send errno %d (%s)",
      errno, strerror(errno));

  if (ack != DTIPC_ACK)
    return dt_err(ctl, F,
      "ack 0x%08lx does not match 0x%08lx",
      ack, DTIPC_ACK);

  strcpy(priv->name, name);              /* remember the ipc name            */

  ipc->type = DTIPC_TYPE_QNX4;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_send(						/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length)
{
  DT_F("dtipc_qnx4_send");
  struct _mxfer_entry smx[2], rmx[1];
  dtipc_header_t h;
  long sofar, l;
  dtipc_ack_t ack;
  int rc;

  if (ipc->type != DTIPC_TYPE_QNX4)
    return dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_QNX4);

  h.magic = DTIPC_MAGIC;
  h.total = length;
  for (sofar=0; sofar<length; sofar+=l)
  {
    l = DT_MIN(length - sofar, MSG_MAX);
    h.length = l;
    _setmx(&smx[0], &h, sizeof(h));
    _setmx(&smx[1], (char *)data+sofar, h.length);
    _setmx(&rmx[0], &ack, sizeof(ack));

    rc = Sendmx(ipc->handle.id, 2, 1, smx, rmx);

    if (rc == -1)
      return dt_err(ctl, F, "Sendmx errno %d (%s) at %ld of %ld sent",
        errno, strerror(errno),
        sofar, length);

    if (ack != DTIPC_ACK)
      return dt_err(ctl, F,
        "ack 0x%08lx does not match 0x%08lx at %ld of %ld sent",
        ack, DTIPC_ACK,
        sofar, length);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * if ipc->handle.id is 0, receives from anybody and returns with new ipc->handle.id 
 * if ipc->handle.id is not 0, accepts message only from client on it
 * length of message received must match exactly what is expected
 * always allocates space for new data
 *..........................................................................*/
static
dt_rc_e
dtipc_qnx4_recv(						/* receive data on connection		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length)
{
  DT_F("dtipc_qnx4_recv");
  struct _mxfer_entry mx[2];
  dtipc_header_t h;
  dtipc_ack_t ack;
  pid_t handle;
  long got;
  dt_rc_e rc;

  if (ipc->type != DTIPC_TYPE_QNX4)
    return dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_QNX4);

  if (ipc->handle.id == 0)
    return dt_err(ctl, F,
      "caller gave invalid ipc->handle.id (0x%08lx)",
      ipc->handle.id);

  if (length > ipc->recv_max)
    return dt_err(ctl, F,
      "caller requesting length %ld greater than recv_max %ld",
      length, ipc->recv_max);

  DT_G(dtos_malloc2,(ctl, data, length, F, "received data"));
  if (rc != DT_RC_GOOD)
    return rc;

  got = 0;
  do {
    _setmx(&mx[0], &h, sizeof(h));
    _setmx(&mx[1], (char *)(*data)+got, length-got);

    handle = Receivemx(ipc->handle.id, 2, mx);

    if (handle == (pid_t)(-1))
      return dt_err(ctl, F, "Receivemx errno %d (%s)",
        errno, strerror(errno));

    if (ipc->handle.id != handle)
      return dt_err(ctl, F,
        "handle 0x%08lx does not match 0x%08lx at got %ld of %ld",
        (unsigned long)handle, ipc->handle.id,
        got, length);
    if (h.magic != DTIPC_MAGIC)
      return dt_err(ctl, F,
        "header magic 0x%08lx does not match 0x%08lx at got %ld of %ld",
        h.magic, DTIPC_MAGIC,
        got, length);
    if (h.total != length)
      return dt_err(ctl, F,
        "header total %ld does not match %ld at got %ld of %ld",
        h.total, length,
        got, length);
  
    ack = DTIPC_ACK;
    Reply(handle, &ack, sizeof(ack));

    got += h.length;
    ipc->handle.id = handle;			/* further i/o with this client		*/
  } while (got < length);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_qnx4_recv2(						/* receive data on connection		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long *length)
{
  DT_F("dtipc_qnx4_recv2");
  dtipc_header_t h;
  dtipc_ack_t ack;
  pid_t handle;
  dt_rc_e rc;


  *data = NULL;

  if (ipc->type != DTIPC_TYPE_QNX4)
  {
    rc = dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_QNX4);
	goto cleanup;
  }

  if (ipc->handle.id == 0)
  {
    rc = dt_err(ctl, F,
      "caller gave invalid"
      " ipc->handle.id (0x%08lx)",
      ipc->handle.id);
	goto cleanup;
  }

  handle = Receive(ipc->handle.id,		/* wait for partner to send 		*/
    (void *)&h, sizeof(h));

  if (handle == (pid_t)(-1))
  {
    rc = dt_err(ctl, F, 
      "Receive errno %d (%s)",
      errno, strerror(errno));
	goto cleanup;
  }

  if (ipc->handle.id != handle)			/* somebody else sent? 				*/
  {
    rc = dt_err(ctl, F,
      "handle 0x%08lx does not"
      " match 0x%08lx",
      (unsigned long)handle, 
      ipc->handle.id);
	goto cleanup;
  }
  if (h.magic != DTIPC_MAGIC)			/* invalid header? 					*/
  {
    rc = dt_err(ctl, F,
      "header magic 0x%08lx does not"
      " match 0x%08lx",
      h.magic, DTIPC_MAGIC);
	goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* get space for data body			*/
    data, h.total+1,
    F, "received data"));

  *length = Readmsg(ipc->handle.id,		/* get the data body 				*/
    sizeof(h), *data, h.total+1);

  if (*length != h.total)				/* data body length wrong? 			*/
  {
	rc = dt_err(ctl, F,
      "message length %ld"
      " but Readmsg gave %ld",
      *length, h.total);
	goto cleanup;
  }

cleanup:
  if (rc != DT_RC_GOOD &&
	  *data != NULL)
  {
    DT_I(dtos_free2,(ctl, *data,
      F, "received data"));
	*data = NULL;
  }
  
  if (handle != (pid_t)(-1))			/* we received a message? 			*/
  {
	if (rc == DT_RC_GOOD)
	  ack = DTIPC_ACK;
	else
	  ack = DTIPC_NACK;
	Reply(handle, &ack, sizeof(ack));
  }
  
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtipc_qnx4_read(						/* read data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void *data,
  long maxlength,
  long *length)
{
  DT_F("dtipc_qnx4_read");
  priv_t *priv = (priv_t *)ipc->priv;
  dt_rc_e rc = DT_RC_GOOD;

  if (ipc->type != DTIPC_TYPE_QNX4)
  {
    rc = dt_err(ctl, F, 
      "ipc type %d does not match %d",
      ipc->type, DTIPC_TYPE_QNX4);
	goto cleanup;
  }

  if (priv->have == 0)					/* input buffer exhausted? 			*/
  {
	if (priv->bytes != NULL)			/* we have old data? 				*/
      DT_C(dtos_free2,(ctl,				/* nuke old data 					*/
        priv->bytes,				
        F, "received data"));
      
	DT_C(dtipc_qnx4_recv2,(ctl, ipc,	/* wait for new data 				*/
      (void **)&priv->bytes, 
      &priv->have));
  }
  
  *length = DT_MIN(maxlength,			/* how much data to copy 			*/
    priv->have);
  DT_MEMCPY(data,							/* copy data to user buffer 		*/
    priv->bytes+priv->offset,
    *length);

  priv->have -= *length;
  priv->offset += *length;

cleanup:
  return rc;
}


/*..........................................................................*/
static
dt_rc_e
dtipc_qnx4_write(						/* write data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length,
  long *wrote)
{
  DT_F("dtipc_qnx4_write");
  DT_Q(dtipc_qnx4_send,(ctl, ipc,
    data, length));
  *wrote = length;
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_qnx4_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
{
  DT_F("dtipc_qnx4_init");
  priv_t *priv = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  DT_MEMSET(ipc, 0, sizeof(*ipc));

  if (recv_max == 0)
    ipc->recv_max = DTIPC_RECV_MAX_DEFAULT;
  else
    ipc->recv_max = recv_max;

  ipc->flags = flags;

  DT_C(dtos_malloc2,(ctl,
    (void **)&priv, sizeof(*priv),
    F, "priv"));
  DT_MEMSET(priv, 0, sizeof(*priv));

  ipc->priv = priv;
  ipc->rregister = dtipc_qnx4_register;
  ipc->unregister = dtipc_qnx4_unregister;
  ipc->poll = dtipc_qnx4_poll;
  ipc->wait = dtipc_qnx4_wait;
  ipc->isipc = dtipc_qnx4_isipc;
  ipc->find = dtipc_qnx4_find;
  ipc->unfind = dtipc_qnx4_unfind;
  ipc->done = dtipc_qnx4_done;
  ipc->send = dtipc_qnx4_send;
  ipc->recv = dtipc_qnx4_recv;
  ipc->recv2 = dtipc_qnx4_recv2;
  ipc->write = dtipc_qnx4_write;
  ipc->read = dtipc_qnx4_read;

cleanup:
  if (rc != DT_RC_GOOD)
    DT_I(dtipc_qnx4_release,(ctl,
      priv));
    
  return DT_RC_GOOD;
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
