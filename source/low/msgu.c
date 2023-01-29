/*..........................................................................
| NAME
| :dtmsg: Function Group - semaphore operations
|
| SUMMARY
| The message object ~sem~ encapsulates the ability
| of a process to send and receive IPC messages.
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: msg.c,v $ $Revision: 1.8 $");
#include <dtack/msg.h>
#include <dtack/os.h>
#include <dtack/errno.h>

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*..........................................................................
  CHANGES
  19990102 DE don't give error on receive truncation
  19990102 DE added const to post and added flush
 *..........................................................................*/

/*..........................................................................
| NAME
| dtmsg_create() - create message object
|
| SUMMARY
| Create the message queue object ~msg~.
| If ~id~ is 0, the message queue will be private.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_create(							/* create new message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 		*/
  int key)								/* system-wide message id 			*/
{
  DT_F("dtmsg_create");
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)
	key = (int)IPC_PRIVATE;

  memset(msg, 0, sizeof(*msg));			/* clear message structure 			*/
  
  sysrc = msgget(						/* get new message 					*/
    (key_t)key, 
    0666 | IPC_CREAT);
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "msgget(%d, 0666|IPC_CREAT)" 
      DTERRNO_FMT,
      key,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  msg->key = key;
  msg->sid = sysrc;						/* the queue id for the queue		*/
  msg->created = 1;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "created key=%d sid=%d", 
    msg->key, msg->sid);

  msg->initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtmsg_destroy,(ctl, msg));

  return rc;
}

/*..........................................................................
| NAME
| dtmsg_destroy() - destroy message object
|
| SUMMARY
| Free all resources associated with ~msg~ object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_destroy(							/* release msg object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg)							/* msg object 						*/
{
  DT_F("dtmsg_destroy");
  dt_rc_e rc = DT_RC_GOOD;
  if (msg->created)						/* msg object got created? 			*/
  {
	int sysrc;
	int e;

	sysrc = msgctl(						/* remove message 					*/
      msg->sid,
      IPC_RMID,							/* cmd 								*/
      NULL);							/* arg 								*/
	e = errno;
	if (sysrc == -1)					/* system call failed? 				*/
    {
	  rc = dt_err(ctl, F,
        "msgctl(%d, IPC_RMID, NULL)"
        DTERRNO_FMT,
        msg->sid,
        DTERRNO_ARG(e));
    }
	else
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "destroyed key=%d sid=%d", 
        msg->key, msg->sid);
  }
  memset(msg, 0, sizeof(*msg));
  
  return rc;
}

/*..........................................................................
| NAME
| dtmsg_access() - access message object
|
| SUMMARY
| Access the message queue object ~msg~.
| If ~id~ is 0, the message queue will be private.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_access(							/* access new message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 		*/
  int key)								/* system-wide message id 			*/
{
  DT_F("dtmsg_access");
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)
	DT_CLEANUP((ctl, F,
      "invalid key %d", key));

  memset(msg, 0, sizeof(*msg));			/* clear message structure 			*/
  
  sysrc = msgget(						/* access message 					*/
    (key_t)key, 0); 
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
	DT_CLEANUP((ctl, F,
      "msgget(%d, 0)"
      DTERRNO_FMT,
      key,
      DTERRNO_ARG(e)));

  msg->key = key;
  msg->sid = sysrc;						/* the queue id for the queue		*/
  msg->accessed = 1;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "accessed key=%d sid=%d", 
    msg->key, msg->sid);

  msg->initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtmsg_release,(ctl, msg));

  return rc;
}

/*..........................................................................
| NAME
| dtmsg_release() - release message object
|
| SUMMARY
| Free all resources associated with ~msg~ object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_release(							/* release msg object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg)							/* msg object 						*/
{
  DT_F("dtmsg_release");
  dt_rc_e rc = DT_RC_GOOD;
  if (msg->accessed)					/* msg object got accessed? 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "released key=%d sid=%d", 
      msg->key, msg->sid);
  }
  memset(msg, 0, sizeof(*msg));
  
  return rc;
}

/*..........................................................................
| NAME
| dtmsg_post() - post message object
|
| SUMMARY
| Increment message count.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_post(								/* post msg object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* msg object 						*/
  const void *data,
  long length)
{
  DT_F("dtmsg_post");
  struct msgbuf *buf = NULL;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (!msg->initialized)				/* msg object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");
  if (length <= 0)
	return dt_err(ctl, F,
      "invalid length %ld", length);

  DT_C(dtos_malloc2,(ctl,				/* space for the message 			*/
    (void **)&buf, 
    sizeof(buf->mtype)+length,
    F, "buf"));

  buf->mtype = 1;						/* message type 					*/
  
  DT_MEMCPY(buf->mtext, data, length);	/* copy data into the message 		*/

  sysrc = msgsnd(msg->sid,				/* send the message increment */
    buf, length, IPC_NOWAIT);	   
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "msgsnd(%d, buf, %ld, IPC_NOWAIT)" 
      DTERRNO_FMT,
      msg->sid, 
      length,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  if (DT_DBG(ctl, DT_DBG_MASK_COMMAND) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "posted %ld bytes to msg queue %d(%d)",
    length, msg->key, msg->sid);

cleanup:
  if (buf != NULL)
    DT_I(dtos_free2,(ctl, buf, F, "buf"));
  
  return rc;
}

/*..........................................................................
| NAME
| dtmsg_wait() - wait for message
|
| SUMMARY
| Wait for message.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_wait(								/* wait msg object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* msg object 						*/
  long max,
  void *data,
  long *length)
{
  DT_F("dtmsg_wait");
  struct msgbuf *buf = NULL;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (!msg->initialized)				/* msg object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");
  if (max <= 0)
	return dt_err(ctl, F,
      "invalid length %ld", max);

  DT_C(dtos_malloc2,(ctl,				/* space for the message 			*/
    (void **)&buf, 
    sizeof(buf->mtype)+max,
    F, "buf"));

  buf->mtype = 1;						/* message type 					*/

  sysrc = msgrcv(msg->sid,				/* wait for the message				*/
    buf, max, 
    0,									/* any mtype 						*/
    MSG_NOERROR |						/* no error on truncation 			*/
    0);									/* no IPC_NOWAIT flag means wait	*/
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "msgrcv(%d, buf, %ld, IPC_NOWAIT)" 
      DTERRNO_FMT,
      msg->sid, 
      max,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  DT_MEMCPY(data, buf->mtext, sysrc);	/* copy data from the message 		*/
  if (length != NULL)
	*length = sysrc;					/* actual length of message 		*/

  if (DT_DBG(ctl, DT_DBG_MASK_COMMAND) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "received %d bytes from msg queue %d(%d)",
    sysrc, msg->key, msg->sid);

cleanup:
  if (buf != NULL)
    DT_I(dtos_free2,(ctl, buf, F, "buf"));
  
  return rc;
}

/*..........................................................................
| NAME
| dtmsg_get_count() - return number of messages in queue
|
| SUMMARY
| Return current message count.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_get_count(						/* get current message count		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 	*/
  int *count)
{
  DT_F("dtmsg_get_count");
  int sysrc;
  int e;
  struct msqid_ds stat;
  dt_rc_e rc = DT_RC_GOOD;

  if (!msg->initialized)				/* msg object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");

  sysrc = msgctl(						/* initialize new message 		*/
    msg->sid,
    IPC_STAT,
    &stat);
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "msgctl(%d, IPC_STAT, &stat"
      DTERRNO_FMT,
      msg->sid,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  *count = stat.msg_qnum;

  if (DT_DBG(ctl, DT_DBG_MASK_COMMAND) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "%d messages are in msg queue %d(%d)",
    *count, msg->key, msg->sid);

cleanup:

  return rc;
}

/*..........................................................................
| NAME
| dtmsg_exists() - return true if message object exists
|
| SUMMARY
| Return true if the message queue object ~key~ exists.
| Possibly return information about msg queue in ~msg~.
| Do not create or access the msg queue.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_exists(							/* access new message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg,							/* returned message structure 		*/
  int key,								/* system-wide message id 			*/
  int *exists)
{
  DT_F("dtmsg_exists");
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)
	DT_CLEANUP((ctl, F,
      "invalid key %d", key));

  memset(msg, 0, sizeof(*msg));			/* clear message structure 			*/
  
  *exists = 1;
  sysrc = msgget(						/* access message 					*/
    (key_t)key, 0); 
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	if (e = ENOENT)
	  *exists = 0;
	else
	  DT_CLEANUP((ctl, F,
        "msgget(%d, 0)"
        DTERRNO_FMT,
        key,
        DTERRNO_ARG(e)));
  }

  msg->key = key;
  msg->sid = sysrc;						/* the queue id for the queue		*/

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "key=%d sid=%d", 
    msg->key, msg->sid);

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtmsg_release,(ctl, msg));

  return rc;
}

/*..........................................................................
| NAME
| dtmsg_flush() - clear all messages from queue
|
| SUMMARY
| Clear all messages from queue by receiving them.
| Discard all contents.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_flush(							/* flush all messages				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtmsg_t *msg)							/* returned message structure 		*/
{
  DT_F("dtmsg_flush");
  int i, n;
  char t[256];
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtmsg_get_count,(ctl, msg, &n));	/* check for incoming messages 		*/
   
  for (i=0; i<n; i++)
  {
	char t[1];
	DT_C(dtmsg_wait,(ctl, msg,			/* pick off the queued message 		*/
      sizeof(t), t, NULL));
  }

cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsg_rev(								/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtmsg message queue lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 2;
  if (rev_date != NULL)
    *rev_date = 19990102;
  return DT_RC_GOOD;
}


#endif
