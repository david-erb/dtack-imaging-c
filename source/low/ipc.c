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
  20000223 DE added dtipc_write_all2 with timeout
  19990725 DE use strlen(data) if length is -1
 *..........................................................................*/


/*..........................................................................
| NAME
| Introduction - intro
|
| DESCRIPTION
| This is the ipc cover level.
| All Dtack applications use this API.
|
| The constructor function for servers is dtipc_register().
| The constructor function for clients is dtipc_find().
|
| Each function must have the :dt_ctl_t: runtime environment structure
| pointer as its first argument.
| The runtime environment structure controls such things as debug,
| error, and memory management handling.
|
| Each function must have the :dtipc_t: structure pointer as its
| second argument.
| This structure is essentially the object descriptor.
| It is initialized by the constructor functions and
| must be passed to all subsequent functions.
|
| The constructor functions also choose the implementation.
| The constructor functions examine its ~spec~ string argument to determine
| which implementation to use.
| The local "choices" and "methods" tables map the ~spec~ string to one of the 
| known implementation init functions.
| Each implementation has only one external function: init.
| The init function for an implementation is responsible for loading
| the :dtipc_t: structure's function pointers.
|
| For some functions, this cover level API will drop straight through
| to the corresponding implementation function.
| For other functions, the cover level performs tasks common to all
| implementations
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: ipc.c,v $ $Revision: 1.1 $");

#include <dtack/ipc.h>
#include <dtack/label.h>
#include <dtack/os.h>
#include <dtack/str.h>


static const char *choices[] = {
  "qnx4",
  "qbaby",
  "socket",
  "rocket",
  NULL
};

static dtipc_init_f *methods[] = {
  dtipc_qnx4_init,
  dtipc_qbaby_init,
  dtipc_rocket_init,
  dtipc_rocket_init
};

/*..........................................................................*/
static
dt_rc_e
dtipc_init(								/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  char *init,
  char **subinit,
  long recv_max,
  unsigned long flags)  
{
  DT_F("dtipc_init");
  int choice;

  if (init == NULL)
    return dt_err(ctl, F,
      "NULL ipc name");
  else
  if (strlen(init) == 0)
    return dt_err(ctl, F,
      "zero-length ipc name");

  *subinit = strchr(init,				/* get subinit part					*/
    DT_SEPARATOR_SPEC);

  if (*subinit != NULL)					/* separate init from subinit	    */
  {
    if (*subinit == init)
      return dt_err(ctl, F,
        "zero-length server name in \"%s\"",
        init);
    **subinit = '\0';
	*subinit = *subinit + 1;
  }


  DT_Q(dtstr_choice,(ctl, choices,		/* look up ipc name			    	*/
    init, &choice));

  if (choice == -1)						/* unknown name					    */
    return dt_err(ctl, F,
      "unknown ipc spec \"%s\"",
      init);
  else
  if (choice < -1)						/* ambiguous name				    */
    return dt_err(ctl, F,
      "ambiguous ipc spec \"%s\"",
      init);

  DT_Q((methods[choice]),(ctl,			/* chop off the ipc spec			*/
    ipc, recv_max, flags))

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_register(							/* register service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long recv_max,
  unsigned long flags)  
{
  DT_F("dtipc_register");
  char *type = NULL;
  char type_dynamic[64];
  char *server;
  int ipc_initialized = 0;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "called with name \"%s\"",
    name != NULL? name: "NULL");

  if (name != NULL)
    if (strlen(name) <					/* we could do this on the stack? 	*/
        sizeof(type_dynamic))	
	{
	  type = type_dynamic;
	  strcpy(type_dynamic, name);
	}
	else								/* too big for the stack? 			*/
      DT_C(dtos_strdup,(ctl, name,	
        &type))

  DT_C(dtipc_init,(ctl, ipc,			/* separate type from server	    */
    type, &server, recv_max, flags));
  ipc_initialized = 1;

  DT_C((ipc->rregister),(ctl, ipc,		/* register the name			    */
    server));

cleanup:
  if (rc != DT_RC_GOOD &&
	  ipc_initialized && 
	  ipc->uninit != NULL)
  {
    DT_I((ipc->uninit),(ctl, ipc));
	DT_MEMSET(ipc, 0, sizeof(*ipc));		/* clear the structure 				*/
  }

  if (type != NULL &&
      type != type_dynamic)
    DT_I(dtos_free,(ctl, type));		/* free parse string			    */

  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_unregister(						/* unregister service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_unregister");
  dt_rc_e rc = DT_RC_GOOD;
  DT_I((ipc->unregister),(ctl, ipc));
  if (ipc->uninit != NULL)
    DT_I((ipc->uninit),(ctl, ipc));
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_wait(								/* wait for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_wait");
  DT_Q((ipc->wait),(ctl, ipc));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_poll(								/* poll for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  dtipc_poll_e what,
  long timeout,
  const char *waitingfor,
  int *isready)
{
  DT_F("dtipc_poll");
  if (ipc->poll == NULL)
    return dt_err(ctl, F,
      "no poll function defined");
  DT_Q((ipc->poll),(ctl, ipc, what, timeout, waitingfor, isready));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_isipc(							/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  unsigned long id,
  dtipc_header_t *h,
  int *isipc)
{
  DT_F("dtipc_isipc");
  DT_Q((ipc->isipc),(ctl, ipc, id, h, isipc));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_find(								/* find registered service name		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long recv_max,
  unsigned long flags,
  long timeout)  
{
  DT_F("dtipc_find");
  char *type = NULL;
  char *server;
  int ipc_initialized = 0;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "called with name \"%s\"",
    name? name: "NULL");

  if (name != NULL)
  DT_C(dtos_strdup,(ctl, name,			/* we will tokenize it			    */
    &type));

  DT_C(dtipc_init,(ctl, ipc,			/* separate type from server	    */
    type, &server, recv_max, flags));
  ipc_initialized = 1;

  DT_C((ipc->find),(ctl, ipc,			/* find the name				    */
    server, timeout));

cleanup:
  if (ipc_initialized && 
	  rc != DT_RC_GOOD &&
	  ipc->uninit != NULL)
  {
    DT_I((ipc->uninit),(ctl, ipc));
	DT_MEMSET(ipc, 0, sizeof(*ipc));		/* clear the structure 				*/
  }
  
  if (type != NULL)
    DT_I(dtos_free2,(ctl, type,			/* free parse string			    */
      F, "spec copy"));

  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_split(							/* split server/client ipc			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc1,
  dtipc_t *ipc2)
{
  DT_F("dtipc_split");
  dt_rc_e rc = DT_RC_GOOD;

  if (ipc1->split != NULL)
    DT_I((ipc1->split),(ctl, ipc1, ipc2));
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_unfind(							/* unfind registered service name	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_unfind");
  dt_rc_e rc = DT_RC_GOOD;
  DT_I((ipc->unfind),(ctl, ipc));

  if (ipc->uninit != NULL)
    DT_I((ipc->uninit),(ctl, ipc));
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_done(								/* server is done with connection   */
  dt_ctl_t *ctl,
  dtipc_t *ipc)
{
  DT_F("dtipc_done");
  DT_Q((ipc->done),(ctl, ipc));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_send(								/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length)
{
  DT_F("dtipc_send");
  DT_Q((ipc->send),(ctl, ipc, data, length));
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_send2(							/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length,
  long *sent)
{
  DT_F("dtipc_send2");
  if (ipc->send2 != NULL)
	DT_Q((ipc->send2),(ctl, ipc, data, length, sent))
  else
    return dt_err(ctl, F,
      "function not implemented for this ipc type");
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtipc_write_some() - write data to file
| 
| SUMMARY
| Writes data to file.
| Will continue to write until all ~want~ bytes have been written.
| Returns an error if a system error is found before all bytes are written.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_write_some(						/* write data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length,
  long *wrote)
{
  DT_F("dtipc_write_some");
  if (ipc->write != NULL)
  {
	if (length == -1)
	  length = strlen((char *)data);
    DT_Q((ipc->write),(ctl, ipc, 
      data, length, wrote));
  }
  else
    return dt_err(ctl, F,
      "function not implemented for this ipc type");
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_write_all2(						/* write all data with timeout		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  long timeout,							/* max total time to wait 			*/
  const void *buf,						/* buffer holding data 				*/
  long want)							/* number of bytes to write 		*/
{
  DT_F("dtipc_write_all2");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  dtos_time_t beg, now;

  if (want == -1)
	want = strlen((char *)buf);

  DT_Q(dtos_time,(ctl, &beg));
  for (have=0; have<want; have+=got)
  {
	int isready;
	long t;
	DT_Q(dtos_time,(ctl, &now));
	if (timeout == -1)
	  t = -1;
	else
	{
	  t = timeout - (int)(1000.0 *
        (now.wall-beg.wall));
	  if (t < 0)
	    t = 0;
	}
	
	DT_Q(dtipc_poll,(ctl, ipc,
      DTIPC_POLL_WRITE, t,
      "write", &isready));
	if (!isready)
	  return dt_err(ctl, F,
        "timeout %ld ms reached",
        timeout);
	DT_Q(dtipc_write_some,(ctl,			/* do the write						*/
      ipc,
      (char *)buf+(size_t)have, 
      DT_MIN(want-have, 
        DTCONFIG_WRITE_MAX),
      &got));
    if (DT_DBG(ctl, mask))
	  dt_dbg(ctl, F, mask,
        "now have written %ld of %ld (%ld%%)",
        got+have, want,
        (100L * (long)(got+have)) /
        (want? want: 1));
    if (got == 0)						/* end-of-file?					    */
      break;
  }

  if (have < want)						/* didn't get all we wanted?		*/
    return dt_err(ctl, F,
      DT_LABEL_EOF
      " only wrote %ld out of %ld to \"%s\"",
      have, want, ipc->client_spec);

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_write_all(						/* write all data to file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want)							/* number of bytes to write 		*/
{
  DT_F("dtipc_write_all");
  DT_Q(dtipc_write_all2,(ctl, ipc,
    -1,									/* never timeout 					*/
    buf, want));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_recv(								/* receive data, known length		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length)
{
  DT_F("dtipc_recv");
  DT_Q((ipc->recv),(ctl, ipc, data, length));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_recvt(							/* receive, known length, timeout	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length,
  long timeout)
{
  DT_F("dtipc_recvt");
  if (ipc->recvt != NULL)
	DT_Q((ipc->recvt),(ctl, ipc, 
      data, length, timeout))
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for this ipc type");
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_recv2(							/* receive data, unknown length		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long *length)
{
  DT_F("dtipc_recv2");
  if (ipc->recv2 != NULL)
	DT_Q((ipc->recv2),(ctl, ipc, 
      data, length))
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for this ipc type");
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtipc_read_all() - fill buffer from ipc
| 
| SUMMARY
| Reads data from file.
| Will continue to read until all ~want~ bytes have been read.
| Returns an error if end of file is reached before buffer is full.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_read_some(						/* receive data on connection		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void *data,
  long maxlength,
  long *length)
{
  DT_F("dtipc_read_some");
  if (ipc->read != NULL)
    DT_Q((ipc->read),(ctl, ipc, data, maxlength, length))
  else
    return dt_err(ctl, F,
      "function not implemented for this ipc type");
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtipc_read_all(							/* fill buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long want)							/* maximum number of bytes wanted 	*/
{
  DT_F("dtipc_read_all");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);

  for (have=0; have<want; have+=got)
  {
	DT_Q(dtipc_read_some,(ctl,			/* do the read						*/
      ipc,
      (char *)buf+(size_t)have, 
      DT_MIN(want-have, 
        DTCONFIG_READ_MAX),
      &got));
	dt_dbg(ctl, F, mask,
      "now have read %ld of %ld (%ld%%)",
      got+have, want,
      (100L * (long)(got+have)) /
      (want? want: 1));
    if (got == 0)						/* end-of-file?					    */
      break;
  }

  if (have < want)						/* didn't get all we wanted?		*/
    return dt_err(ctl, F,
      "only read %ld out of %ld",
      have, want);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtipc_read_line2() - read file up to new line with timeout
| 
| SUMMARY
| Read data from file.
| Continue to read until ~max-1~ bytes have been read,
| or until a newline character is reached.
| If ~max-1~ bytes are reached before the newline is found,
| leave remaining bytes on line to be read by next call.
| If reached, the newline character is returned in the buffer.
| Always place an end-of-string character in the returned buffer.
| 
| Have an error if end of file is reached before buffer is full.
| Have an error if timeout occurs before a complete line is read..
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_read_line2(						/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  long timeout,							/* timeout 							*/
  void *buf,							/* buffer to return bytes in 		*/
  long max)								/* maximum size of buffer 			*/
{
  DT_F("dtipc_read_line2");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO | DT_DBG_MASK_BIGIO;
  char *b = (char *)buf;
  char c;
  int n = 0;

  if (max <= 0)
    return dt_err(ctl, F,
      "invalid buffer size %d", max);

  for (have=0; have<max-1; have++)
  {
	int isready;
	DT_Q(dtipc_poll,(ctl, ipc,
      DTIPC_POLL_READ, 
      timeout,
      "ASCII line",
      &isready));
	if (!isready)
	  return dt_err(ctl, F,
        "timeout %ld ms reached",
        timeout);
	DT_Q(dtipc_read_some,(ctl, ipc, 	/* do the read						*/
      &c, 1, &got));
    if (got == 0)						/* end-of-file?					    */
      break;
	b[n++] = c;							/* stuff character into buffer 		*/
	if (c == '\n')						/* quit looping upon newline 		*/
	  break;
  }

  b[n] = '\0';							/* end string always 				*/
  
  if (n == max-1 && c != '\n')
    dt_dbg(ctl, F, mask,
	  "line longer than %d", max-1);
  else
    dt_dbg(ctl, F, mask,
	  "line length %d", n);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtipc_read_line() - read file up to new line
| 
| SUMMARY
| Read data from file.
| Continue to read until ~max-1~ bytes have been read,
| or until a newline character is reached.
| If ~max-1~ bytes are reached before the newline is found,
| leave remaining bytes on line to be read by next call.
| If reached, the newline character is returned in the buffer.
| Always place an end-of-string character in the returned buffer.
| 
| Have an error if end of file is reached before buffer is full.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtipc_read_line(						/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long max)								/* maximum size of buffer 			*/
{
  DT_F("dtipc_read_line");
  DT_Q(dtipc_read_line2,(ctl, ipc, 
    -1,									/* no timeout 						*/
    buf, max));

  return DT_RC_GOOD;
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
