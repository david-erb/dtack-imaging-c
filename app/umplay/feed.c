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
U_R("app/umplay $RCSfile: feed.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND
#include <signal.h>
#endif

static int sigalrm_triggered;
static void sigalrm(int signum) { sigalrm_triggered =1; }

/*..........................................................................
| NAME
| dtfd_read() - read buffer from file
| 
| SUMMARY
| Reads data from file.
| Only makes one try.
| Zero bytes returned in ~*got~ probably means end of file.
| Not an error when ~*got~ is returned less than ~want~.
| 
| END
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
umplay_read_fifo(						/* read buffer from fifo			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to put data into			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *got)							/* returned number of bytes gotten 	*/
{
  DT_F("umplay_read_fifo");
  void (*sigalrm_previous)(int) = NULL;
  long mask = DT_DBG_MASK_IO |
    (want > 1? DT_DBG_MASK_BIGIO: 0);
  int e;
  int rc;

  if (want > DTCONFIG_READ_MAX)
    want = DTCONFIG_READ_MAX;

  sigalrm_previous =					/* server exit may nuke our alarms 	*/
    signal(SIGALRM, sigalrm);
  sigalrm_triggered = 0;
  alarm(1);
  if (fd->file == NULL)					/* did not do fopen?			    */
    rc = read(fd->fd, buf,				/* do the read raw					*/
      (size_t)want);	
  else
    rc = fread(buf, 1,					/* do the read buffered				*/
      (size_t)want, fd->file);
  e = errno;
  alarm(0);
  if (sigalrm_previous != NULL)
    signal(SIGALRM, sigalrm_previous);

  if (rc <= 0)							/* read failed?						*/
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "rc=%d, e=%d, sigalrm_triggered=%d",
      rc, e, sigalrm_triggered);

  if (rc == -1)							/* read failed?						*/
  if (e == EINTR && sigalrm_triggered)	/* timed out?						*/
  {
	rc = 0;								/* pretend no data read 			*/
  }
  else
  if (fd->file == NULL)					/* did not do fopen?			    */
    return dt_err(ctl, F,
      "read(\"%s\" fd=%d, buf, %ld) errno %d",
      fd->filename, fd->fd, want, e);
  else
    return dt_err(ctl, F,
      "fread(buf, 1, %ld, \"%s\" fd=%d) errno %d",
      want, fd->filename, fd->fd, e);
  
  if (rc < 0)							/* return bytes we got				*/
    *got = 0x10000L + rc;
  else
    *got = rc;

  dt_dbg(ctl, F, mask,
    "from \"%s\" read %ld of %ld", 
    fd->filename, *got, want);

  return DT_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_feed(
  u_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  void *buffer,							/* buffer to put data in 			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *pgot)							/* actual number of bytes returned 	*/
{
  U_F("umplay_feed");
  umplay_t *umplay = (umplay_t *)arg;
  long got = 0;
  
again:
  while(1)
  {
										/* ................................ */
    if (UMPLAY_HAS_COMMAND_IPC(umplay))	/* we have a command ipc to poll?	*/
	  U_Q(umplay_ipc_poll,(umplay));
										/* ................................ */
    if (UMPLAY_HAS_COMMAND_FIFO(umplay))/* we have a command fifo to poll?	*/
	  U_Q(umplay_fifo_poll,(umplay));
										/* ................................ */
    if (UMPLAY_HAS_KEYBD(umplay))		/* we have a keyboard to sample?	*/
	  U_Q(umplay_keybd_poll,(umplay));

	U_Q(umdec_set_control,(ctl,			/* tell hardware					*/
      umdec, &umplay->control));
	
	if (umplay->control.mode ==			/* mode is to stop? 				*/
		UMDEC_MODE_STOP ||
		umplay->exiting)				/* mode is to exit?					*/
	  return U_RC_GOOD;

	if (umplay->control.mode !=			/* mode is to not pause?			*/
		UMDEC_MODE_PAUSE)
	  break;
	
	U_Q(dtos_delay,(ctl, 250));			/* wait a bit 						*/
  }
										/* ................................ */
  if (want > 0)							/* not just command poll? 			*/
  {
	if (UMPLAY_HAS_DATA_IPC(umplay))	/* we have a data ipc to read?		*/
	{
	  U_Q(dtipc_read_some,(ctl,			/* read data from incoming ipc 		*/
        &umplay->data_ipc, buffer, 
        want, &got));
	}
	else								/* ................................ */
	if (UMPLAY_HAS_DATA_FIFO(umplay))	/* we have a data fifo to read?		*/
	{
	  U_Q(umplay_read_fifo,(ctl,		/* read data from incoming fifo		*/
        &umplay->data_fifo, buffer, 
        want, &got));
	  if (got == 0)						/* no data in fifo? 				*/
	  {
		dt_dbg(ctl, F, 
          DT_DBG_MASK_COMMAND,
          "read from fifo timed out");
		U_Q(dtos_delay,(ctl, 250));		/* wait a bit 						*/
		goto again;
	  }
	}
	else								/* ................................ */
	if (UMPLAY_HAS_FD(umplay))			/* we have a file to read? 			*/
	{
	  U_Q(dtfd_read,(ctl, &umplay->fd, 
        buffer, want, &got));
	  if (got == 0)
	    U_Q(dtfd_close,(ctl, 
          &umplay->fd));
	}
										/* ................................ */
	else								/* no data source ready?			*/
	{
	  U_Q(dtos_delay,(ctl, 250));		/* wait a bit 						*/
	  goto again;
	}
  }

  if (pgot != NULL)
    *pgot = got;

  return U_RC_GOOD;
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
