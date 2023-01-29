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
U_R("app/umdemo $RCSfile: feed.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

u_rc_e 
U_API
umdemo_feed(
  u_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  void *buffer,							/* buffer to put data in 			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *got)							/* actual number of bytes returned 	*/
{
  U_F("umdemo_feed");
  umdemo_t *umdemo = (umdemo_t *)arg;
  umdec_control_t control;
  *got = 0;
  
again:
  U_Q(umdec_get_control,(ctl,			/* get current umdec control 		*/
    umdec, &control));

  do {
										/* ................................ */
    if (UMDEMO_HAS_COMMAND_IPC(umdemo))	/* we have a command ipc to poll?	*/
	  U_Q(umdemo_ipc_poll,(umdemo));
										/* ................................ */
    if (UMDEMO_HAS_COMMAND_FIFO(umdemo))/* we have a command fifo to poll?	*/
	  U_Q(umdemo_fifo_poll,(umdemo));
										/* ................................ */
    if (UMDEMO_HAS_KEYBD(umdemo))		/* we have a keyboard to sample?	*/
	  U_Q(umdemo_keybd_poll,(umdemo));

	if (umdemo->stopping ||				/* command is stopping us? 			*/
		umdemo->exiting)				/* or exiting us? 					*/
	{
	  control.mode = UMDEC_MODE_STOP;
	  U_Q(umdec_set_control,(ctl,		/* tell hardware to stop 			*/
        umdec, &control));
	  return U_RC_GOOD;
	}

    if (UMDEMO_IS_PAUSED(umdemo) &&		/* we are supposed to pause? 		*/
		control.mode !=					/* but hardware not already paused?	*/
		UMDEC_MODE_PAUSE)
	{
	  control.mode = UMDEC_MODE_PAUSE;
	  U_Q(umdec_set_control,(ctl,		/* tell hardware to pause 			*/
        umdec, &control));
	}

    if (UMDEMO_IS_PAUSED(umdemo))		/* we are paused?					*/
  	  U_Q(dtos_delay,(ctl, 250));		/* wait a bit 						*/

  } while(UMDEMO_IS_PAUSED(umdemo));
										/* ................................ */
  if (UMDEMO_HAS_DATA_IPC(umdemo))		/* we have a data ipc to read?		*/
  {
	U_Q(dtipc_read,(ctl,				/* read data from incoming ipc 		*/
      &umdemo->data_ipc, buffer, 
      want, got));
  }
  else									/* ................................ */
  if (UMDEMO_HAS_DATA_FIFO(umdemo))		/* we have a data fifo to read?		*/
  {
	U_Q(dtfd_read,(ctl,					/* read data from incoming fifo		*/
      &umdemo->data_fifo, buffer, 
      want, got));
  }
  else									/* ................................ */
  if (UMDEMO_HAS_FD(umdemo))			/* we have a file to read? 			*/
  {
    *got = 0;
    U_Q(dtfd_read,(ctl, &umdemo->fd, 
      buffer, want, got));
	if (*got == 0)
	  U_Q(dtfd_close,(ctl, 
        &umdemo->fd));
  }
										/* ................................ */
  else									/* no data source ready?			*/
  {
	U_Q(dtos_delay,(ctl, 250));			/* wait a bit 						*/
	goto again;
  }

  if (control.mode !=					/* not already going forward? 		*/
	  UMDEC_MODE_FORWARD)
  {
	control.mode = UMDEC_MODE_FORWARD;
	U_Q(umdec_set_control,(ctl,			/* tell hardware to go forward		*/
      umdec, &control));
  }

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
