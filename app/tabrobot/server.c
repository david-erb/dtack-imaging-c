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
  CHANGES

/*..........................................................................
  CHANGES
  20000720 DE added read poll, check for negative delay
  20000720 DE add delay scale factor param->b
  20000223 DE added param->epsilon
  19990122 DE check for client NULL in server_poll
 *..........................................................................*/

#define CR (0x0d)
#define LF (0x0a)


/*..........................................................................
| NAME
| Introduction - intro
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("app/tabrobot $RCSfile: server.c,v $ $Revision: 1.8 $");
#include <dtptz.h>

typedef struct {
  dtipc_t ipc;
  dtos_time_t time;
  int bad;
  int extended;
} connection_t;

#define FORMAT "position %d, max wait %d:%02d:%02d\r\n"
#define ACK0 "no one waiting, %d:%02d:%02d used\r\n"
#define ACK1 "%d waiting, %d:%02d:%02d left\r\n"
#define ACK2 "%d waiting, %d:%02d:%02d left\r\n"
#define GO "go\r\n"
#define CRLF "\r\n"

typedef struct {
  char name[64];
  int datahandle;
} priv_t;

extern
dt_rc_e
DTCONFIG_API1
dtipc_write_all2(						/* write all data with timeout		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  long timeout,							/* max total time to wait 			*/
  const void *buf,						/* buffer holding data 				*/
  long want);							/* number of bytes to write 		*/

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
_rtrim(
  char *t)
{
  int l = strlen(t);
  while (l > 0 &&						/* nuke the crlf's 					*/
		 t[l-1] == LF ||
		 t[l-1] == CR)
	t[--l] = '\0';
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabrobot_server_count(
  dtparam_t *param,
  dtlist_t *list,
  int *n)
{
  DT_F("tabrobot_server_count");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_elem_t *elem;
  connection_t *connection;
  dt_rc_e rc;

  DT_C(dtlist_next,(ctl, list,			/* get list head 					*/
    DTLIST_ELEM_HEAD, &elem));

  *n = 0;
  while(elem != NULL)
  {
	connection = (connection_t *)
      elem->user;
	if (!connection->bad)				/* not a bad connection? 			*/
	  *n = *n + 1;
    DT_C(dtlist_next,(ctl, list,		/* get next list entry				*/
      elem, &elem));
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabrobot_server_done(
  dtparam_t *param,
  dtlist_t *list,
  dtipc_t **client,
  dtptz_t *ptz,
  dtos_time_t *notified)
{
  DT_F("tabrobot_server_done");
  dt_ctl_t *ctl = &param->ctl;
  dtlist_elem_t *elem;
  connection_t *connection;
  dt_rc_e rc;

  DT_C(dtipc_done,(ctl, *client));		/* server ends conversation 		*/
  DT_C(dtlist_next,(ctl, list,			/* get list head 					*/
    DTLIST_ELEM_HEAD, &elem));
  connection = (connection_t *)
	elem->user;
  DT_C(dtipc_unfind,(ctl,				/* release ipc channel 				*/
    &connection->ipc));
  DT_C(dtos_free2,(ctl, connection,		/* free connection structure 		*/
    F, "connection structure"));
  DT_C(dtos_free2,(ctl,					/* free fake name 					*/ 
    (void *)elem->name,
    F, "name"));
  DT_C(dtlist_delete,(ctl, list,		/* delete list head 				*/
    elem));
  *client = NULL;						/* invalidate current client 		*/
  notified->wall = 0;					/* notify everyone 					*/
  
#if 0
  DT_C(dtptz_stop,(ctl, ptz));			/* give stop to ptz 				*/
#endif

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabrobot_server_poll(
  dtparam_t *param,
  dtipc_t *listen,
  dtlist_t *list,
  dtipc_t **client,
  dtptz_t *ptz,
  dtos_time_t *notified)
{
  DT_F("tabrobot_server_poll");
  dt_ctl_t *ctl = &param->ctl;
	dtos_time_t now;
  const double min = param->min/1000.0;
  const double max = param->max/1000.0;
  unsigned int n0;
  int n1;
  long timeout;
  int isready;
  char *name;
  dtlist_elem_t *elem;
  connection_t *connection;
  dt_rc_e rc;

  DT_C(dtos_time,(ctl, &now));

  DT_C(dtlist_count,(ctl, list, &n0));	/* check queue count 				*/
  if (n0 == 0)							/* queue is empty? 					*/
  {
	timeout = 1000;						/* wait one second					*/
  }
  else									/* queue is not empty? 				*/
  {
	timeout = 0;						/* just poll 						*/
  }

  DT_C(dt_yield,(ctl,					/* check for async messages 		*/
    "polling for connections"));

  DT_C(dtipc_poll,(ctl, listen,			/* wait or check for connections	*/
    DTIPC_POLL_ACCEPT, timeout,
      "connections", &isready));

  if (isready)							/* a new connection is ready? 		*/
  {
	DT_C(dtipc_wait,(ctl, listen));		/* accept connection 				*/

	DT_C(dtos_strdup,(ctl, "name",		/* fake name 						*/
      &name));
	DT_C(dtos_malloc2,(ctl,				/* space for connection structure 	*/
      (void **)&connection, 
      sizeof(*connection),
      F, "connection structure"));
	memset(connection, 0,				/* clear connection structure 		*/
      sizeof(*connection));

	DT_C(dtipc_split,(ctl, listen,		/* separate data from server 		*/
      &connection->ipc));
  
	DT_C(dtlist_after,(ctl, list,		/* add connection to queue			*/
      DTLIST_ELEM_TAIL, 
      name, (void *)connection,
      NULL));

	notified->wall = 0;					/* notify everyone 					*/
  }

  DT_C(tabrobot_server_count,(param,	/* get number of queue entries 		*/
    list, &n1));
  if (n1 == 0)
	goto cleanup;

  DT_C(dtlist_next,(ctl, list,			/* get list head 					*/
    DTLIST_ELEM_HEAD, &elem));
  connection = (connection_t *)
    elem->user;
  if (n1 == 1)							/* no one waiting? 					*/
	connection->extended = 0;
  else									/* someone is waiting? 				*/
  {
	double used;
	used = now.wall -					/* time used 						*/
	  connection->time.wall;
	if (!connection->extended)			/* not already extended? 			*/
	{
	  if (max-used < min)				/* do we need a grace period? 		*/
	  {
		connection->time.wall =			/* grace is param->min 				*/
          now.wall-max+min;
		dt_dbg(ctl, F, DT_DBG_MASK_USR2,
          "grace had used %0.2f, now used %0.2f", used, 
		  now.wall -
	      connection->time.wall);
		used = now.wall -				/* time used 						*/
	      connection->time.wall;
	  }
	  connection->extended = 1;			/* only extend once 				*/
	}
	if (used > max)						/* maximum time used? 				*/
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "time expired (%0.0f sec) for %s",
        now.wall-connection->time.wall,
        connection->ipc.client_spec);
	  DT_C(tabrobot_server_done,(param,	/* nuke the current connection 		*/
        list, client, ptz, notified));
	}
  }

  while (*client == NULL)				/* old client is invalid?			*/
  {
	dtlist_elem_t *elem;
	DT_C(dtlist_next,(ctl, list,		/* get list head 					*/
      DTLIST_ELEM_HEAD, &elem));
	if (!elem)							/* list has gone empty?			    */
	  break;
	connection = (connection_t *)
      elem->user;
	*client = &connection->ipc;			/* use this ipc for data 			*/
	if (!connection->bad)				/* connection looks good so far? 	*/
	{
	  int isready;
   	  DT_G(dtipc_poll,(ctl, *client,
        DTIPC_POLL_WRITE, param->epsilon,
        "write", &isready));
	  if (rc == DT_RC_GOOD && isready)
		DT_G(dtipc_write_all,(ctl,		/* tell client he's up 				*/
          *client, GO, strlen(GO)));
	  if (rc != DT_RC_GOOD || !isready)	/* client went away? 				*/
	  {
		dt_dbg(ctl, F, DT_DBG_MASK_USR2,
          "could not notify %s",
          (*client)->client_spec);
		connection->bad = 1;
	  }
	}
	if (connection->bad)				/* client connection went bad? 		*/
	{
	  DT_C(tabrobot_server_done,(param,	/* release the client connection 	*/
        list, client, ptz, notified));
	  continue;
	}
	DT_C(dtos_time,(ctl,				/* remember when we granted access 	*/
      &connection->time));
	dt_dbg(ctl, F, DT_DBG_MASK_USR2,
      "access granted to %s",
      (*client)->client_spec);
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabrobot_server_notify(
  dtparam_t *param,
  dtlist_t *list,
  dtos_time_t *notified)
{
  DT_F("tabrobot_server_notify");
  dt_ctl_t *ctl = &param->ctl;
  dtos_time_t now;
  dtlist_elem_t *elem;
  int i;
  connection_t *connection;
  double total;
  char t[128];
  dt_rc_e rc;

  DT_C(dtos_time,(ctl, &now));
  if (now.wall - notified->wall <		/* notify only every few seconds 	*/
	  param->omega/1000.0)
	goto cleanup;

  *notified = now;

  DT_C(dtlist_next,(ctl, list,			/* get list head 					*/
    DTLIST_ELEM_HEAD, &elem));
  if (elem == NULL)						/* list	empty? 						*/
	goto cleanup;

  connection = (connection_t *)
	elem->user;
  total = param->max/1000.0 -			/* remaining time on current client */
    (now.wall-connection->time.wall);

  i = 1;
  while(1)
  {
	int hours, minutes, seconds;
	  int isready;
    DT_C(dtlist_next,(ctl, list,		/* get next list entry				*/
      elem, &elem));
	if (elem == NULL)					/* end of list? 					*/
	  break;
	connection = (connection_t *)
      elem->user;
	if (connection->bad)				/* bad connection? 					*/
	  continue;
	hours = (int)(total/3600.0);
	minutes = (int)((total-hours*3600)/60.0);
	seconds = (int)total % 60;
	DT_C(dtstr_printf,(ctl, 
      t, sizeof(t),
      FORMAT, i, hours, minutes, seconds));
	total += param->max / 1000.0;
	DT_G(dtipc_poll,(ctl, 
      &connection->ipc,
      DTIPC_POLL_WRITE, param->epsilon,
      "write", &isready));
	if (rc == DT_RC_GOOD && isready)
	  DT_G(dtipc_write_all,(ctl,		/* tell client his wait time		*/
        &connection->ipc,
        t, strlen(t)));
	if (rc != DT_RC_GOOD || !isready)	/* could not talk to client? 		*/
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "could not notify %s",
        connection->ipc.client_spec);
	  connection->bad = 1;				/* mark this connection bad 		*/
	}
	i++;
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabrobot_server_command(
  dtparam_t *param,
  dtlist_t *list,
  dtipc_t *ipc,
  dtptz_t *ptz,
  char *ptzspec,
  dtos_time_t *stop,
  int *more,
  double *resume)
{
  DT_F("tabrobot_server_command");
  dt_ctl_t *ctl = &param->ctl;
  dtos_time_t now;
  long timeout;
  int isready;
  char cmd[128];
  char cmd2[256];
  double duration = 0.0;
  dt_rc_e rc;

  *more = 1;

  if (stop->wall > 0)					/* stop is pending? 				*/
  {
	DT_C(dtos_time,(ctl, &now));		/* current time 					*/
    timeout = (stop->wall - now.wall) *	/* milliseconds until stop			*/
      1000.0;
	timeout = DT_MAX(timeout, 0);
	timeout = DT_MIN(timeout, 
      param->omega);
  }
  else									/* no stop is pending? 				*/
	timeout = param->omega;

  DT_C(dt_yield,(ctl, "read poll"));

  DT_C(dtipc_poll,(ctl, ipc,			/* wait for incoming data 			*/
    DTIPC_POLL_READ, timeout,
    "ipc read", &isready));
  if (!isready)							/* no data ready? 					*/
  {
    if (stop->wall == 0)				/* no stop is pending? 				*/
	  goto cleanup;
	DT_C(dtos_time,(ctl, &now));		/* current time 					*/
	if (now.wall < stop->wall)			/* stop not due yet? 				*/
	  goto cleanup;
	DT_C(dtptz_stop,(ctl, ptz));		/* give stop to ptz 				*/
	stop->wall = 0;
	goto cleanup;
  }

  DT_C(dtipc_read_line2,(ctl,			/* get request line 				*/
    ipc, param->epsilon, 
    cmd, sizeof(cmd)));

  _rtrim(cmd);							/* nuke the crlf's 					*/
  if (strlen(cmd) == 0)
  {
	dt_dbg(ctl, NULL, 
      DT_DBG_MASK_COMMAND, 
      "client closed her end");
	*more = 0;
	goto cleanup;
  }

  if (!strcmp(cmd, "nop"))				/* a nop? 						*/
	goto nop;

  DT_C(dtstr_htmlencode,(ctl,
    cmd, cmd2, sizeof(cmd2)));
  dt_dbg(ctl, F,
    DT_DBG_MASK_COMMAND, 
    "cmd=\"%s\"", cmd2);
										/* ................................ */
  if (!strcmp(cmd, "exit"))	
  {
	rc = DT_RC_STOP;
	goto cleanup;
  }
										/* ................................ */
  if (DT_ABBREV(cmd, "reopen"))	
  {
	char tmp[256];
	char ack[256];
	char *p = strchr(cmd, ',');
	if (p == NULL)
	{
	  rc = dt_err(ctl, F,
        "no ptz spec given after repoen");
	  goto cleanup;
	}
	p += 1;

	if (DT_ABBREV(p, "ipc,rocket,"))	/* new spec is a rocket spec? 		*/
	{
	  long port;
	  int isnumber;
	  p = strchr(p, ',') + 1;
	  p = strchr(p, ',') + 1;
	  DT_C(dtstr_to_long2,(ctl,			/* see if spec has bare port 		*/
        p, &port, &isnumber));
	  if (isnumber)						/* spec has bare port? 				*/
	  {
		DT_C(dtstr_printf,(ctl,			/* use sender's IP address 			*/
          tmp, sizeof(tmp),
          "ipc,%s",
          ipc->client_spec));
        strcpy(strchr(tmp, ':')+1, p);	/* replace port in spec 			*/
		p = tmp;
	  }
	}
	if (strcmp(ptzspec, p))				/* spec has been changed? 			*/
	{
	  DT_C(dtptz_uninit,(ctl, ptz));
	  DT_C(dtptz_init,(ctl, ptz, p, 0L));
	  DT_C(dtstr_printf,(ctl, 
        ack, sizeof(ack),
        "ptz spec %s changed to %s%s", 
        ptzspec, p, CRLF));
	  strcpy(ptzspec, p);
	}
	else
	{
	  DT_C(dtstr_printf,(ctl, 
        ack, sizeof(ack),
        "ptz spec %s is unchanged%s", 
        p, CRLF));
	}

	DT_C(dtipc_write_all2,(ctl, ipc,	/* acknowledge						*/
      param->epsilon, ack, strlen(ack)));
	goto cleanup;
  }
										/* ................................ */
  else
  if (DT_ABBREV(cmd, "preset=,"))		/* command starts with preset=? 		*/
  {
	char *p = strchr(cmd, ',') + 1;		/* skip the "preset=," 				*/
	char *desc = strchr(p, ',');		/* find comma after preset number	*/
	if (desc != NULL)					/* preset description given? 		*/
	{
	  *desc++ = '\0';					/* separate number and description 	*/
	  DT_C(tabrobot_preset_set,(param,	/* set the preset description 		*/
        p, desc));
	}
  }
										/* ................................ */
										/* syntax is preset?,1[+2+...] 		*/
  else
  if (DT_ABBREV(cmd, "preset?,"))		/* command starts with preset? 		*/
  {
	char *p = strchr(cmd, ',') + 1;		/* skip the "preset?," 				*/
	while (p != NULL)
	{
	  char desc[256];
	  char *q = strchr(p, '+');			/* find end of list item 			*/
	  if (q != NULL)					/* there is another item? 			*/
		*q++ = '\0';					/* separate list items 				*/
	  DT_C(dtstr_printf,(ctl,
        desc, sizeof(desc),
        "preset%s=", p));
	  DT_C(tabrobot_preset_get,(param,	/* get the preset description 		*/
        p, desc+strlen(desc), 
        sizeof(desc)-strlen(desc)));
	  DT_C(dtstr_printf,(ctl,
        desc+strlen(desc), 
        sizeof(desc)-strlen(desc),
        "%s", CRLF));
	  DT_C(dtipc_write_all2,(ctl, ipc,	/* return the preset				*/
        param->epsilon, 
        desc, strlen(desc)));
	  p = q;							/* advance to next list item 		*/
	}
	DT_C(dtipc_write_all2,(ctl, ipc,	/* blank line after reply			*/
      param->epsilon, CRLF, strlen(CRLF)));
  }
										/* ................................ */
  else
  if (!strcmp(cmd, "rev"))				/* command is "rev"					*/
  {
	const char *rev_string;
	int rev_major;
	int rev_minor;
	long rev_date;
	char t[256];
	dt_rc_e rc;

	DT_C(tabrobot_rev,(ctl,
      &rev_string,
     &rev_major, &rev_minor,
      &rev_date));

	DT_C(dtstr_printf,(ctl, t, sizeof(t),
      "%s %02d.%02d %d" CRLF,
      rev_string,
      rev_major, rev_minor,
      rev_date));
	DT_C(dtipc_write_all2,(ctl, ipc, 
      param->epsilon, t, strlen(t)));
  }
										/* ................................ */
  else
  if (!strcmp(cmd, "resume"))			/* command is "resume"				*/
  {
	int isreadable;
	char t[256];
	DT_C(dtos_isreadable,(ctl,			/* see if suspend file exists 		*/
      param->datadir, &isreadable));
	if (isreadable)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
		"removing suspend file %s",
        param->datadir);
	  DT_C(dtstr_printf,(ctl, t, sizeof(t), "resuming"));
	  DT_C(dtos_unlink,(ctl, 
        (char *)param->datadir));
	}
	else
	{
	  DT_C(dtstr_printf,(ctl, t, sizeof(t), "was not suspended"));
	}
	DT_C(dtipc_write_all2,(ctl, ipc, param->epsilon, t, strlen(t)));
	DT_C(dtipc_write_all2,(ctl, ipc, param->epsilon, CRLF, strlen(CRLF)))
  }
										/* ................................ */
  else
  if (DT_ABBREV(cmd, "suspend"))		/* command starts with suspend? 	*/
  {
	dtfd_t fd;
	long now;
	char t[256];
	int l;
	long minutes;
	time_t tresume;

	char *q = strchr(cmd, ',');			/* find comma in command 			*/
	if (q)
	  DT_C(dtstr_to_long,(ctl,
        q+1, &minutes))
	else								/* no comma? 						*/
	  minutes = 10;

    time(&now);
	*resume = minutes * 60 + now;
   
	DT_C(dtstr_printf,(ctl, t, sizeof(t), "server time now is %s", ctime(&now)));
	_rtrim(t);
	DT_C(dtipc_write_all2,(ctl, ipc, param->epsilon, t, strlen(t)));
	DT_C(dtipc_write_all2,(ctl, ipc, param->epsilon, CRLF, strlen(CRLF)))
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE, "%s", t);

	tresume = (time_t)(*resume);
	DT_C(dtstr_printf,(ctl, t, sizeof(t), "to resume in %ld minutes at %s", 
      minutes, ctime(&tresume)));
	_rtrim(t);
	DT_C(dtipc_write_all2,(ctl, ipc, param->epsilon, t, strlen(t)));
	DT_C(dtipc_write_all2,(ctl, ipc, param->epsilon, CRLF, strlen(CRLF)))
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE, "%s", t);

	DT_C(dtstr_printf,(ctl, t, sizeof(t), "%0.0f\n", (*resume)));
	DT_C(dtfd_open,(ctl, &fd, param->datadir, "w"));
	DT_C(dtfd_write_all,(ctl, &fd, t, strlen(t)));
	DT_C(dtstr_printf,(ctl, t, sizeof(t), "%s", ctime(&tresume)));
	DT_C(dtfd_write_all,(ctl, &fd, t, strlen(t)));
	DT_C(dtfd_close,(ctl, &fd));
  }
										/* ................................ */
  else
  if (DT_ABBREV(cmd, "ptz,"))			/* command starts with ptz? 		*/
  {
	char *p = strchr(cmd, ',') + 1;		/* skip the "ptz," 					*/
	char *q = strchr(p, ',');			/* find comma after ptz command 	*/

	if (stop->wall > 0)					/* stop is pending? 				*/
	{
	  DT_C(dtptz_stop,(ctl, ptz));		/* give stop to ptz 				*/
	  stop->wall = 0;
	}

	if (q != NULL)						/* stuff after the command?			*/
	{
	  *q++ = '\0';						/* separate command from stuff 		*/
	  if (p[0] == 'G' ||				/* ptz command is GO or SET? 		*/
		  p[0] == 'S')
	  {
		DT_C(tabrobot_preset_set,(		/* presume stuff is preset desc		*/
          param, p+1, q));
	  }
	  else								/* presume stuff is a duration 		*/
	  {
		DT_C(dtstr_to_double,(ctl,
          q, &duration));
		DT_C(dtos_time,(ctl, &now));
		duration *= param->b;			/* convert unitless 0:100 to secs	*/
	    stop->wall = now.wall + duration;
	  }
	}
	DT_C(dtptz_command,(ctl, ptz,		/* give command to ptz 				*/
      strchr(cmd, ',')+1));

	if (duration > 0.0 &&				/* some movement? 					*/
		duration < 2.0)					/* movement under 2 seconds? 		*/
	{
	  dtos_time_t now;
	  long delay;
	  DT_C(dtos_time,(ctl, &now));		/* current time 					*/
	  delay = (stop->wall - now.wall) *	/* milliseconds until stop			*/
        1000.0;
      if (delay > 0)					/* delay not already satisfied?		*/
		DT_C(dtos_delay,(ctl, delay));	/* just wait here 					*/
	  DT_C(dtptz_stop,(ctl, ptz));		/* stop the command on the ptz		*/
	  if (DT_DBG(ctl, DT_DBG_MASK_COMMAND) &&
		  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
		dt_dbg(ctl, F, DT_DBG_MASK_ANY,
          "short wait %g for \"%s\" cancelled inline",
          duration, cmd2);
	  stop->wall = 0.0;					/* note we did it 					*/
	}
  }
  else
  {
	dt_err(ctl, F,
      "unrecognized command \"%s\"",
		 cmd2);
  }	
	
										/* ................................ */
nop:
  {
	dtlist_elem_t *elem;
	connection_t *connection;
	dtos_time_t now;
	int n;
	double total;
	int hours, minutes, seconds;
	char t[128];
	DT_C(tabrobot_server_count,(param,
      list, &n));
	DT_C(dtlist_next,(ctl, list,		/* get list head 					*/
      DTLIST_ELEM_HEAD, &elem));
	connection = (connection_t *)
	  elem->user;
	DT_C(dtos_time,(ctl, &now));
	if (n == 1)
	{
	  total =							/* time current client has used 	*/
        now.wall -
        connection->time.wall;
	  hours = (int)(total/3600.0);
	  minutes = (int)((total-hours*3600)/60.0);
	  seconds = (int)total % 60;
	  DT_C(dtstr_printf,(ctl, t, sizeof(t),
        ACK0, hours, minutes, seconds));
	}
	else
	{
	  total = param->max/1000.0 -		/* remaining time on current client */
        (now.wall-connection->time.wall);
	  if (total < 0)
		total = 0;
	  hours = (int)(total/3600.0);
	  minutes = (int)((total-hours*3600)/60.0);
	  seconds = (int)total % 60;
	  if (total == 0)
		DT_C(dtstr_printf,(ctl, t, sizeof(t),
          ACK1, n-1, hours, minutes, seconds))
	  else
		DT_C(dtstr_printf,(ctl, t, sizeof(t),
          ACK2, n-1, hours, minutes, seconds));
	}

	DT_C(dtipc_write_all2,(ctl, ipc,	/* return an ack 					*/
      param->epsilon, t, strlen(t)));
  }
  
cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_server(
  dtparam_t *param)
{
  DT_F("tabrobot_server");
  dt_ctl_t *ctl = &param->ctl;
  dtptz_t _ptz, *ptz = &_ptz;
  char ptzspec[256];
  dtipc_t _listen, *listen = &_listen;
  dtipc_t *client = NULL;
  dtlist_t _list, *list = &_list;
  int list_initialized = 0;
  dtos_time_t notified;
  dtos_time_t stop;
  double resume = 0;
  int more;
  dtfd_t fd;
  int isreadable;

  dt_rc_e rc;

  DTFD_CLEAR(ctl, &fd);

  ptz->initialized = 0;
  listen->type = DTIPC_TYPE_NONE;

  notified.wall = 0;					/* renotify everyone 				*/

  DT_C(dtptz_init,(ctl, ptz,			/* open ptz object 					*/
    param->output, 0));
  DTSTR_CPY(ctl,
    ptzspec, param->output);

  DT_C(dtipc_register,(ctl, listen,		/* register as a server 			*/
    param->connect, 0, 0));

  DT_C(dtlist_create,(ctl, list,		/* make a list 						*/
    "queue"));
  list_initialized = 1;

  DT_C(dtos_isreadable,(ctl, 			/* see if suspend file exists 		*/
    param->datadir, &isreadable));

  if (isreadable)						/* suspend file exists? 			*/
  {
	char t[32];
	unsigned long now;
	time(&now);
	DT_C(dtfd_open,(ctl, &fd, 
      param->datadir, "r"));
	DT_C(dtfd_read_line,(ctl, &fd, 
      t, sizeof(t)));
	DT_C(dtfd_close,(ctl, &fd));
	_rtrim(t);
	DT_C(dtstr_to_double,(ctl, t, &resume));
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
      "time to remove suspend file %s"
      " will be in %0.0f seconds",
      param->datadir,
      resume-now);
  }
  else
	dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
      "not suspended (%s is not readable)", 
      param->datadir);
  
  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "ready for ptz commands");

  stop.wall = 0;
  while(1)
  {
	long now;
	time(&now);
	if (resume < now)					/* time to remove suspend file? 	*/
	{
	  DT_C(dtos_isreadable,(ctl,		/* see if suspend file exists 		*/
        param->datadir, &isreadable));
	  if (isreadable)
	  {
		dt_dbg(ctl, F, DT_DBG_MASK_SCHEDULE,
          "removing suspend file %s",
          param->datadir);
	    DT_C(dtos_unlink,(ctl, 
          (char *)param->datadir));
	  }
	}

	DT_C(tabrobot_server_poll,(param,	/* get (queue) connections			*/
      listen, list, &client, ptz,
      &notified));

	DT_C(tabrobot_server_notify,(param,	/* notify queued clients			*/
      list, &notified));

	if (client != NULL)
	  DT_G(tabrobot_server_command,(	/* handle command					*/
        param, list, client, 
        ptz, ptzspec,
        &stop, &more, &resume));
	if (rc == DT_RC_STOP)
	  goto cleanup;

	if (client != NULL) 				/* we have a client? 				*/
	{
	  if (rc != DT_RC_GOOD ||			/* handling command failed? 		*/
		 !more)							/* client connection severed?		*/
	  {
		DT_C(tabrobot_server_done,(		/* close connection					*/
          param,
          list, &client, ptz,
          &notified));
		stop.wall = 0;
	  }
	}
  }
  
cleanup:
  DT_I(dtfd_close,(ctl, &fd));
  
  if (client != NULL)
	DT_I(tabrobot_server_done,(param,	/* close connection					*/
      list, &client, ptz, &notified));

  if (list_initialized)
  {
	DT_I(dtlist_free,(ctl, list));		/* free remaining list elements 	*/
	DT_I(dtlist_destroy,(ctl, list));	/* destroy the list 				*/
  }
  if (listen->type != DTIPC_TYPE_NONE)
    DT_I(dtipc_unregister,(ctl,			/* release resources				*/
      listen));

  if (ptz->initialized)
	DT_I(dtptz_uninit,(ctl, ptz));

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
