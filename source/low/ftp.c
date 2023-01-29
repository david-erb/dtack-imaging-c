/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>

DT_RCSID("source/low $RCSfile: ftp.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/ftp.h>
#include <dtack/str.h>
#include <dtack/os.h>

#define CR (0x0d)
#define LF (0x0a)

#define DT_DBG_MASK_COMMAND  (0x00400000L)	/* commands to servers				*/

#define CONNECT_TIMEOUT \
  (ftp->connect_timeout > 0? ftp->connect_timeout: (30000))
#define ACCEPT_TIMEOUT \
  (ftp->accept_timeout > 0? ftp->accept_timeout: (30000))
#define READ_TIMEOUT \
  (ftp->read_timeout > 0? ftp->read_timeout: (30000))
#define WRITE_TIMEOUT \
  (ftp->write_timeout > 0? ftp->write_timeout: (30000))

#define MAX_PORT_TRY (100)				/* number of port retries 			*/
#define MAX_PORT_VAL (1000)				/* range of ports to use 			*/

/*..........................................................................*/
static
dt_rc_e
dtftp_code_string(						/* turn ftp reply code into string 	*/
  dt_ctl_t *ctl,
  int code,
  char *string,
  int lmax)
{
  DT_F("dtftp_code_string");
  char *s = NULL;
  switch(code)
  {
#   define CODE(C, S) case C: s = S; break;
    CODE(110, "restart marker reply")
    CODE(120, "service ready in nnn minutes")
    CODE(125, "data already open; transfer starting")
    CODE(150, "about to open data connection")
    CODE(200, "command okay")
    CODE(202, "command not implemented")
    CODE(211, "system status, or system help reply")
    CODE(212, "directory status")
    CODE(213, "file status")
    CODE(214, "help message")
    CODE(215, "name system type")
    CODE(220, "service ready for new user")
    CODE(221, "service closing")
    CODE(225, "data open; no transfer in progress")
    CODE(226, "closing data connection")
    CODE(227, "entering Passive Mode")
    CODE(230, "user logged in, proceed")
    CODE(250, "file action completed")
    CODE(257, "pathname created")
    CODE(331, "user name okay, need password")
    CODE(332, "need account for login")
    CODE(350, "file action pending further information")
    CODE(421, "service not available")
    CODE(425, "can't open data connection")
    CODE(426, "connection closed; transfer aborted")
    CODE(450, "file action not taken")
    CODE(451, "action aborted: local error in processing")
    CODE(452, "action not taken")
    CODE(500, "syntax error, command unrecognized")
    CODE(501, "syntax error in parameters or arguments")
    CODE(502, "command not implemented")
    CODE(503, "bad sequence of commands")
    CODE(504, "command not implemented for that parameter")
    CODE(530, "not logged in")
    CODE(532, "need account for storing files")
    CODE(550, "action not taken")
    CODE(551, "action aborted: page type unknown")
    CODE(552, "file action aborted")
    CODE(553, "action not taken")
#   undef CODE
  }

  if (s == NULL)
  {
    DT_Q(dtstr_printf,(ctl, string, lmax, "unknown ftp code"));
  }
  else
  {
    DT_Q(dtstr_printf,(ctl, string, lmax, "%s", s));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtftp_read_line() - read file up to new line
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

static
dt_rc_e
dtftp_read_line(							/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long max,								/* maximum size of buffer 			*/
  long read_timeout)
{
  DT_F("dtftp_read_line");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO | DT_DBG_MASK_BIGIO;
  char *b = (char *)buf;
  char c;
  int n = 0;
  dt_rc_e rc = DT_RC_GOOD;

  if (max <= 0)
    return dt_err(ctl, F,
      "invalid buffer size %d", max);

  for (have=0; have<max-1; have++)
  {
	int isready;
	DT_C(dtipc_poll,(ctl,				/* wait for incoming data			*/
      ipc, DTIPC_POLL_READ,
      read_timeout,
      "ready to read",
      &isready));

	if (!isready)
	  return dt_err(ctl, F, 
        "timeout after %d ms",
        read_timeout);

	DT_C(dtipc_read_some,(ctl, ipc, 	/* do the read						*/
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

cleanup:
  return rc;
}

/*..........................................................................*/

static
dt_rc_e
dtftp_write_all(						/* write all data to file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want,							/* number of bytes to write 		*/
  long write_timeout)
{
  DT_F("dtftp_write_all");
  long have;
  long got;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  dt_rc_e rc = DT_RC_GOOD;

  for (have=0; have<want; have+=got)
  {
	int isready;
	DT_C(dtipc_poll,(ctl,				/* wait for pipe to clear			*/
      ipc, DTIPC_POLL_WRITE,
      write_timeout,
      "ready to write",
      &isready));

	if (!isready)
	  return dt_err(ctl, F, 
        "timeout after %d ms",
        write_timeout);

	DT_C(dtipc_write_some,(ctl,			/* do the write						*/
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
      "only wrote %ld out of %ld to \"%s\"",
      have, want, ipc->client_spec);

cleanup:
  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_init(								/* allocate structure resources 	*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *control_spec,
  const char *data_spec,
  const char *user,
  const char *password)
{
  DT_F("dtftp_init");
  int isnumber;
  dt_rc_e rc;
  DT_MEMSET(ftp, 0, sizeof(*ftp));
  DT_C(dtos_strdup,(ctl, control_spec,	/* remember the control spec 		*/
    &ftp->control_spec));
  DT_C(dtos_strdup,(ctl, data_spec,		/* remember the data spec 			*/
    &ftp->data_spec));
  DT_C(dtos_strdup,(ctl, user,
    &ftp->user));
  DT_C(dtos_strdup,(ctl, password,
    &ftp->password));

  DT_C(dtstr_to_long2,(ctl,				/* see if data spec is port only 	*/
    ftp->data_spec, 
    &ftp->data_port_start,
    &isnumber));
  if (!isnumber)
    ftp->data_port = -1;
  else
    ftp->data_port = ftp->data_port_start;

cleanup:
  if (rc != DT_RC_GOOD)
  {
    if (ftp->password != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->password,
        F, "password"));
    if (ftp->user != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->user,
        F, "user"));
    if (ftp->data_spec != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->data_spec,
        F, "data_spec"));
    if (ftp->control_spec != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->control_spec,
        F, "control_spec"));
  }
  return rc;
}
  
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_uninit(							/* release structure resources 		*/
  dt_ctl_t *ctl,
  dtftp_t *ftp)
{
  DT_F("dtftp_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (ftp != NULL)
  {
	DT_I(dtftp_disconnect,(ctl, ftp)); 

	if (ftp->password != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->password,
        F, "password"));
	if (ftp->user != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->user,
        F, "user"));
	if (ftp->data_spec != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->data_spec,
        F, "data_spec"));
	if (ftp->control_spec != NULL)
      DT_I(dtos_free2,(ctl,
        ftp->control_spec,
        F, "control_spec"));

	DT_MEMSET(ftp, 0, sizeof(*ftp));
  }
  return rc;
}
  
/*..........................................................................*/
static
dt_rc_e
dtftp_wait(								/* wait for any command reply 		*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  int *reply,
  char *string,
  int lmax)
{
  DT_F("dtftp_wait");
  char t[128];
  int isline;
  long longval;
  int isnum;
  unsigned long mask = DT_DBG_MASK_COMMAND;
  dt_rc_e rc = DT_RC_GOOD;
  
  do {
	DT_C(dtftp_read_line,(ctl,			/* get first response line 			*/
      &ftp->control, t, sizeof(t),
      READ_TIMEOUT));
	isline = t[strlen(t)-1] == '\n';	/* this is an entire line? 			*/

	if (strchr(t, CR))					/* nuke the CR if any 				*/
	  *strchr(t, CR) = '\0';
	if (strchr(t, LF))					/* nuke the LF if any 				*/
	  *strchr(t, LF) = '\0';

	dt_dbg(ctl, F, mask, "got reply \"%s\"", t);

	if (strchr(t, ' '))					/* separate the first token if any	*/
      *strchr(t, ' ') = '\0';

	DT_C(dtstr_to_long2,(ctl,			/* interpret the reply code 		*/
      t, &longval, &isnum));

	if (isnum &&						/* this is the line we want? 		*/
		string != NULL &&				/* and caller wants response text? 	*/
		lmax > 0)
	{
	  DT_C(dtstr_printf,(ctl,
        string, lmax,
        "%s", t+strlen(t)+1));
	}
	 
    while (!isline)						/* not a complete line yet? 		*/
	{
      DT_C(dtftp_read_line,(ctl,		/* flush out rest of line 			*/	
        &ftp->control, t, sizeof(t),
        READ_TIMEOUT));
	  isline = t[strlen(t)-1] == '\n';	/* this is an entire line? 			*/
	}
  } while (!isnum);						/* loop until we get a code 		*/
  
  *reply = (int)longval;

cleanup:
  if (rc != DT_RC_GOOD)
	ftp->ioerror = 1;
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtftp_wait_code(						/* wait for specific code 			*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  int code)
{
  DT_F("dtftp_wait_code");
  int reply;
  char t[128];

  DT_Q(dtftp_wait,(ctl, ftp, &reply,	/* wait for the reply 				*/
    t, sizeof(t)));
  
  if (code != 0 &&						/* we care about the code?			*/
	  reply != code)					/* check the code 					*/
  {
    char s1[128];
	DT_Q(dtftp_code_string,(ctl,
      code, s1, sizeof(s1)));
    return dt_err(ctl, F,
      "wanted %d (%s) but got %d (%s)",
      code, s1, reply, t);
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtftp_command(							/* send command, no reply wait 		*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *command,
  const char *arg)
{
  DT_F("dtftp_command");
  char t[128];
  unsigned long mask = DT_DBG_MASK_COMMAND;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, mask,
    "%s%s%s", command,
    strlen(arg)? " ": "", arg);

  DT_Q(dtstr_printf,(ctl, t, sizeof(t),	/* build the command string 		*/
    "%s%s%s%c%c", 
    command, 
    strlen(arg)? " ": "",
    arg, CR, LF));

  DT_C(dtftp_write_all,(ctl,			/* send the command					*/
    &ftp->control, t, strlen(t),
    WRITE_TIMEOUT));

cleanup:
  if (rc != DT_RC_GOOD)
	ftp->ioerror = 1;
  
  return rc;
}
    
/*..........................................................................*/
static
dt_rc_e
dtftp_command_reply(					/* send command and expect reply 	*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *command,
  const char *arg,
  int code)
{
  DT_F("dtftp_command_reply");

  DT_Q(dtftp_command,(ctl, ftp,			/* send the command 				*/
    command, arg));
  
  DT_Q(dtftp_wait_code,(ctl, ftp,		/* wait for the reply 				*/
    code));
	  
  return DT_RC_GOOD;
}
    
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_register(
  dt_ctl_t *ctl,
  dtftp_t *ftp)
{
  DT_F("dtftp_register");
  char spec[64];
  char *p;
  int n, a1, a2, a3, a4, port;
  dt_rc_e rc;

  if (ftp->data_port == -1)				/* full spec? 						*/
  {
	DT_C(dtstr_printf,(ctl,				/* use it as is					    */
      spec, sizeof(spec), 
      ftp->data_spec));
  }
  else									/* data spec is incomplete?		    */
  {
    DT_C(dtstr_printf,(ctl,				/* use host from control connection	*/
      spec, sizeof(spec),
      "%s", ftp->control.client_spec));
	p = strchr(spec, ':');				/* find the colon					*/
	if (p == NULL)						/* oops no colon? 					*/
	{
	  rc = dt_err(ctl, F,
        "invalid ftp control spec \"%s\""
        " (no colon)", spec);
	  goto cleanup;
	}
	p++;								/* skip past the colon 				*/
    DT_C(dtstr_printf,(ctl,				/* add our data port 				*/
      p, sizeof(spec)-(p-spec),
      "%d", ftp->data_port));
	ftp->data_port++;					/* up to next port 					*/
	if (ftp->data_port >				/* past max usable port? 			*/
		ftp->data_port_start+
		MAX_PORT_VAL)
	  ftp->data_port =					/* revert to first port 			*/
        ftp->data_port_start;
  }

  DT_C(dtipc_register,(ctl, &ftp->data,	/* register and bind port 			*/
    spec, 0, 0));
  ftp->registered = 1;

  p = strchr(ftp->data.server_spec, 	/* find the comma					*/
    ',');			
  if (p == NULL)						/* oops no comma? 					*/
  {
	rc = dt_err(ctl, F,
      "invalid ftp data spec \"%s\""
      " (no comma)",
      ftp->data.server_spec);
	goto cleanup;
  }
  p++;									/* skip past the comma 				*/

  n = sscanf(p, "%d.%d.%d.%d:%d",
    &a1, &a2, &a3, &a4, &port);
  if (n != 5)
  {
	rc = dt_err(ctl, F,
      "invalid ftp data spec \"%s\""
      " (only %d fields)",
      ftp->control.server_spec, n);
	goto cleanup;
  }
  DT_C(dtstr_printf,(ctl,				/* format port that was bound		*/
    spec, 
    sizeof(spec),
    "%d,%d,%d,%d,%d,%d",
    a1, a2, a3, a4,
    (port >> 8) & 0xff,
    port & 0xff));

  DT_C(dtftp_command_reply,(ctl, ftp,	/* tell server what port to use		*/
    "PORT", spec, 200));

cleanup:
  
  return rc;
}
      
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_connect(
  dt_ctl_t *ctl,
  dtftp_t *ftp)
{
  DT_F("dtftp_connect");
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, &ftp->control,	/* contact the server 				*/
    ftp->control_spec, 0, 0, 
    CONNECT_TIMEOUT));
      
  ftp->found = 1;

  DT_C(dtftp_wait_code,(ctl, ftp,		/* wait for the first reply 		*/
    220));
    
  DT_C(dtftp_command_reply,(ctl, ftp, "USER", ftp->user,     331));
  DT_C(dtftp_command_reply,(ctl, ftp, "PASS", ftp->password, 230));
  DT_C(dtftp_command_reply,(ctl, ftp, "TYPE", "I",           200));

cleanup:
  if (rc != DT_RC_GOOD)
    DT_I(dtftp_disconnect,(ctl, ftp));
  
  return rc;
}
  
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_chdir(							/* change remote directory 			*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *dirname)
{
  DT_F("dtftp_chdir");
  DT_Q(dtftp_command_reply,(ctl, ftp, 
    "CWD", dirname, 250));
  return DT_RC_GOOD;
}
  
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_delete(							/* delete remote file 				*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *filename)
{
  DT_F("dtftp_delete");
  DT_Q(dtftp_command_reply,(ctl, ftp,	/* send delete command 				*/
    "DELE", filename, 0));				/* don't care about the code 		*/
  return DT_RC_GOOD;
}
  
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_mkdir(							/* make remote directory			*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *dirname)
{
  DT_F("dtftp_mkdir");
  DT_Q(dtftp_command_reply,(ctl, ftp,	/* send mkdir command 				*/
    "MKD", dirname, 0));				/* don't care about the code 		*/
  return DT_RC_GOOD;
}
  
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_put_start(						/* start data transfer 				*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const char *filename)
{
  DT_F("dtftp_put_start");
  int isready;
  int itry = 0;
  dt_rc_e rc;

again:
  DT_Q(dtftp_put_finish,(ctl, ftp));	/* finish previous transfer if any	*/
	
  DT_G(dtftp_register,(ctl, ftp));		/* go active on data port		    */
	
  if (rc == DT_RC_GOOD)
	DT_G(dtftp_command_reply,(ctl, ftp,	/* tell server to connect to us		*/
      "STOR", filename, 150));

  if (rc != DT_RC_GOOD)
	if (itry < MAX_PORT_TRY)			/* we have another retry coming? 	*/
	{
	  itry++;
	  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
        "retrying (%d out of %d)",
        itry, MAX_PORT_TRY);
	  goto again;
	}
	else
	  return dt_err(ctl, F,
        "retries exhausted");

  DT_Q(dtipc_poll,(ctl,					/* wait for incoming connection		*/
    &ftp->data, DTIPC_POLL_ACCEPT,
    ACCEPT_TIMEOUT,
    "ready to accept",
    &isready));

  if (!isready)
	return dt_err(ctl, F, 
      "timeout after %d ms",
      ACCEPT_TIMEOUT);

  DT_Q(dtipc_wait,(ctl, &ftp->data));	/* wait for server to connect 		*/

  ftp->started = 1;
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_put_data(							/* send some data 					*/
  dt_ctl_t *ctl,
  dtftp_t *ftp,
  const void *data,
  long length)
{
  DT_F("dtftp_put_data");
  dt_rc_e rc;

  DT_C(dtftp_write_all,(ctl,			/* send data to server 				*/
    &ftp->data, data, length,
    WRITE_TIMEOUT));

cleanup:
  if (rc != DT_RC_GOOD)
	ftp->ioerror = 1;
  
  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_put_finish(						/* wait for transfer to complete	*/
  dt_ctl_t *ctl,
  dtftp_t *ftp)
{
  DT_F("dtftp_put_finish");

  if (ftp->started)						/* transfer already started? 		*/
  {
	DT_Q(dtipc_done,(ctl,				/* close data socket				*/
      &ftp->data));

	ftp->started = 0;

	DT_Q(dtftp_wait_code,(ctl, ftp,		/* wait for the reply 				*/
      226));
  }

  if (ftp->registered)					/* we were active?				    */
  {
	DT_Q(dtipc_unregister,(ctl,			/* release data port				*/
      &ftp->data));

	ftp->registered = 0;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtftp_disconnect(
  dt_ctl_t *ctl,
  dtftp_t *ftp)
{
  DT_F("dtftp_disconnect");
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(dtftp_put_finish,(ctl, ftp));	/* finish previous transfer if any	*/

  if (ftp->registered)
  {
	DT_I(dtipc_unregister,(ctl,			/* release data port				*/
      &ftp->data));

	ftp->registered = 0;
  }

  if (ftp->found)
  {
	if (!ftp->ioerror)					/* still talking with server? 		*/
      DT_I(dtftp_command,(ctl, ftp,		/* tell server we are quitting 		*/
        "QUIT", ""));

    DT_I(dtipc_unfind,(ctl,				/* close the connection 			*/
      &ftp->control));

	ftp->found = 0;
  }
  
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
