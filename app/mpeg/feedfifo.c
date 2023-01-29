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

#include <umdemo.h>
U_R("app/umdemo $RCSfile: feedfifo.c,v $ $Revision: 1.8 $");

#  ifndef DTDEPEND						/* no makefile dependency 			*/
#    ifdef DTCONFIG_IMPLIED_QNX4
#      include <ioctl.h>
#      include <sys/select.h>
#    endif
#  endif

typedef enum {
  DTFD_POLL_READ = 1001,
  DTFD_POLL_WRITE = 1002
} dtfd_poll_e;

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

u_rc_e 
U_API
umdemo_fifo_command(
  umdemo_t *umdemo)
{
  U_F("umdemo_fifo_command");
  u_ctl_t *ctl = umdemo->ctl;
  umdemo_deccmd_t deccmd;
  u_rc_e rc = U_RC_GOOD;

  U_Q(dtfd_read_all,(ctl,				/* receive command from client 		*/
    &umdemo->command_fifo, 
    (void **)&deccmd, 
    sizeof(deccmd)));

  if (deccmd.cmd != DECCMD_NOP)
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "got command %d",
      deccmd.cmd);

  switch(deccmd.cmd)
  {
	case DECCMD_INITPARAM:
	break;
	case DECCMD_PLAY:
	  U_Q(umdemo_command,(umdemo,
        UMDEMO_COMMAND_FORWARD));
	break;
	case DECCMD_PAUSE:
	  if (!UMDEMO_IS_PAUSED(umdemo))
  	    U_Q(umdemo_command,(umdemo,
          UMDEMO_COMMAND_PAUSE));
	break;
	case DECCMD_STEPFRAME:
	  U_Q(umdemo_command,(umdemo,
        UMDEMO_COMMAND_PAUSE));
	break;
	case DECCMD_STEPGOP:
	break;
	case DECCMD_STOP:
	  U_Q(umdemo_command,(umdemo,
        UMDEMO_COMMAND_QUIT));
	break;
	case DECCMD_NOP:
	break;
	case DECCMD_GETFRAME:
	{
	  umdemo_decrep_t decrep;			/* 1024 bytes! 						*/
	  memset(&decrep, 0,				/* give back frame 0 every time 	*/
        sizeof(decrep));
	  decrep.reptype = DECREP_FRAME;
	  U_Q(dtfd_write_all,(ctl,			/* write command into fifo 			*/
        &umdemo->reply_fifo, 
        (void **)&decrep, 
        sizeof(decrep)));
	  fflush(umdemo->reply_fifo.file);
	}
	break;
  }
  
  if (umdemo->response[0] != '\0')
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "command response %s",
      umdemo->response);

  return rc;
}

/*..........................................................................*/

u_rc_e 
U_API
umdemo_fifo_poll(
  umdemo_t *umdemo)
{
  U_F("umdemo_fifo_poll");
  u_ctl_t *ctl = umdemo->ctl;
  int ready;
  int rc = U_RC_GOOD;

  U_Q(dtfd_poll,(ctl,					/* check for incoming command 		*/
    &umdemo->command_fifo,
    DTFD_POLL_READ, 0,
    "command fifo",
    &ready));

  if (!ready)							/* nobody talking to us?		    */
    return U_RC_GOOD;

  U_C(umdemo_fifo_command,(umdemo));	/* read and handle the command	    */

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
