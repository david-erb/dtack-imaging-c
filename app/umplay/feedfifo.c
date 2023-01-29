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

#include <umplay.h>
U_R("app/umplay $RCSfile: feedfifo.c,v $ $Revision: 1.8 $");

#  ifndef DTDEPEND						/* no makefile dependency 			*/
#    ifdef DTCONFIG_IMPLIED_QNX4
#      include <sys/select.h>
#    endif
#  endif

typedef enum {
  DTFD_POLL_READ = 1001,
  DTFD_POLL_WRITE = 1002
} dtfd_poll_e;

#define DUMPSIZE (8192)

/*..........................................................................
 * timeout of -1 means wait forever
 *..........................................................................*/
static
dt_rc_e
dtfd_poll(								/* wait for fd to be active			*/
  dt_ctl_t *ctl,
  dtfd_t *fd,
  dtfd_poll_e what,
  long milliseconds,
  const char *waiting_for,
  int *isready)
{
  DT_F("dtfd_poll");
  fd_set fds;
  int in = (what == DTFD_POLL_READ);
  int first = -1;
  int ready = -1;

  do {
	struct timeval timeout;
	int sysrc;

	if (first == 1 && 
		DT_DBG(ctl, DT_DBG_MASK_BIGIO))
	{	
      dt_dbg(ctl, F, DT_DBG_MASK_BIGIO,
        "waiting for %s on fd %d",
        waiting_for, fd->fd);
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
	FD_SET(fd->fd, &fds);			/* wait for activity				*/
	sysrc = select(fd->fd+1,
      in? &fds: NULL, 
      in? NULL: &fds, 
      NULL, &timeout);
	if (sysrc == -1 &&
		errno == EINTR)
	{	
	  return dt_err(ctl, F,
	    "%s got unexpected signal",
        fd->filename);
	}
	if (sysrc == -1)					/* other failure in select()?	    */
      return dt_err(ctl, F,
        "%s select(%d, fds,"
        " NULL, NULL, (%ld,%ld))"
        " waiting for %s"
        " gave errno %d",
        fd->filename,
        fd->fd+1,
        timeout.tv_sec, timeout.tv_usec,
        waiting_for,
        errno);

	ready = FD_ISSET(fd->fd, &fds);

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

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
u_rc_e 
umplay_fifo_command_buf(
  umplay_t *umplay,
  umplay_deccmd_t *deccmd)
{
  U_F("umplay_fifo_command_buf");
  u_ctl_t *ctl = umplay->ctl;
  void *dump = NULL;
  u_rc_e rc = U_RC_GOOD;

  umplay->response[0] = '\0';

  switch(deccmd->cmd)
  {
	case DECCMD_INITPARAM:
	break;
	case DECCMD_PLAY:
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "received fifo structure DECCMD_PLAY speed %d",
        deccmd->u.playSpeed);
	  if (deccmd->u.playSpeed < -1)
  	    U_C(umplay_command,(umplay,
          UMPLAY_COMMAND_SFORWARD))
	  else
	  if (deccmd->u.playSpeed > 1)
  	    U_C(umplay_command,(umplay,
          UMPLAY_COMMAND_FFORWARD))
	  else
  	    U_C(umplay_command,(umplay,
          UMPLAY_COMMAND_FORWARD))
	}
	break;
	case DECCMD_PAUSE:
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "got command DECCMD_PAUSE");
	  if (!UMPLAY_IS_PAUSED(umplay))
  	    U_C(umplay_command,(umplay,
          UMPLAY_COMMAND_PAUSE));
	}
	break;
	case DECCMD_STEPFRAME:
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "got command DECCMD_STEPFRAME");
	  U_C(umplay_command,(umplay,
        UMPLAY_COMMAND_1FORWARD));
	break;
	case DECCMD_STEPGOP:
	break;
	case DECCMD_STOP:
	{
	  long got;
	  long dumped = 0;
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "got command DECCMD_STOP");
	  DT_C(dtos_malloc2,(ctl, &dump,
        DUMPSIZE, F, "dump area"));
	  U_C(umplay_command,(umplay,		/* handle stop command 				*/
        UMPLAY_COMMAND_QUIT));
	  if (!umplay->exiting)				/* not on our way to exit? 			*/
	  {
		do {
	      U_C(dtfd_read,(ctl,			/* empty data fifo					*/
            &umplay->data_fifo, dump,
            DUMPSIZE, &got));
		  dumped += got;
		} while (got > 0);
	    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
          "dumped %ld bytes from data fifo",
          dumped);
	  }
	}
	break;
	case DECCMD_NOP:
	break;
	case DECCMD_GETFRAME:
	{
	  umplay_decrep_t decrep;			/* 1024 bytes! 						*/
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "got command DECCMD_GETFRAME");
	  memset(&decrep, 0,				/* give back frame 0 every time 	*/
        sizeof(decrep));
	  decrep.reptype = DECREP_FRAME;
	  U_C(dtfd_write_all,(ctl,			/* write command into fifo 			*/
        &umplay->reply_fifo, 
        (void **)&decrep, 
        sizeof(decrep)));
	  fflush(umplay->reply_fifo.file);
	}
	break;
	case DECCMD_EXIT:					/* exit if server 					*/
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "got command DECCMD_EXIT");
	  U_C(umplay_command,(umplay,
        UMPLAY_COMMAND_EXIT));
	break;
	default:
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "got unknown command %d",
        deccmd->cmd);
  }

  if (strcmp(umplay->response, "") &&
	  strncmp(umplay->response, "ok", 2))
	dt_err(ctl, F, 
      "%s", umplay->response);

cleanup:
  if (dump != NULL)
    DT_I(dtos_free2,(ctl, dump,
      F, "dump area"));
  return rc;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_fifo_command(
  umplay_t *umplay)
{
  U_F("umplay_fifo_command");
  u_ctl_t *ctl = umplay->ctl;
  umplay_deccmd_t deccmd;
  char *c = (char *)&deccmd;
  u_rc_e rc = U_RC_GOOD;

  U_Q(dtfd_read_all,(ctl,				/* receive command from client 		*/
    &umplay->command_fifo, 
    (void **)&deccmd, 
    sizeof(deccmd)));

  U_Q(umplay_fifo_command_buf,(umplay,	/* handle command 					*/
    &deccmd));
  
  return DT_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_fifo_poll(
  umplay_t *umplay)
{
  U_F("umplay_fifo_poll");
  u_ctl_t *ctl = umplay->ctl;
  int fd = umplay->command_fifo.fd;		/* for short 						*/
  int rc = U_RC_GOOD;

  while(1)
  {
	int sysrc;
	umplay_deccmd_t deccmd;

    sysrc = read(fd, &deccmd,			/* attempt to read data 			*/
      sizeof(deccmd));
	if (sysrc == -1)
	{
	  int e = errno;
	  if (e == EWOULDBLOCK)				/* no data? 						*/
	    goto cleanup;
	  rc = dt_err(ctl, F,
        "non-blocking read error %d",
        errno);
	  goto cleanup;
	}
	else
	if (sysrc == 0)						/* command fifo closed? 			*/
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "command fifo closed,"
        " faking quit command");
	  memset(&deccmd, 0,
        sizeof(deccmd));
      deccmd.cmd = DECCMD_STOP;			/* pretend we got a quit command 	*/
	  U_C(umplay_fifo_command_buf,(		/* handle fake quit command			*/
        umplay, &deccmd));
	  goto cleanup;
	}
	else
	if (sysrc < sizeof(deccmd))			/* not even six bytes? 				*/
	{
	  rc = dt_err(ctl, F,
        "only %d bytes on command fifo"
        " but need %d",
        sysrc, sizeof(deccmd));
	  goto cleanup;
	}

	U_C(umplay_fifo_command_buf,(		/* handle command 					*/
      umplay, &deccmd));
	
	if (umplay->exiting)				/* we have been told to exit? 		*/
	  goto cleanup;
  }

cleanup:

  return U_RC_GOOD;						/* ignore return code 				*/
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
