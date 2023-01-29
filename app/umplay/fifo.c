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
U_R("app/umplay $RCSfile: fifo.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>

#ifndef DTDEPEND						/* no makefile dependency 			*/
#  include <sys/ioctl.h>
#endif

/* ------------------------------------------------------------------------
 * requires connect and variant
 * ignores input
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_op_fifo(
  dtparam_t *param)
{
  U_F("umplay_op_fifo");
  u_ctl_t *ctl = &param->ctl;
  int umdec_initialized = 0;
  umplay_t *umplay = NULL;
  umdec_t _umdec, *umdec = &_umdec;
  umplay_deccmd_t deccmd;
  u_rc_e rc;

  U_C(umplay_init,(param, &umplay));	/* init callback umplay structure	*/

  U_C(umplay_log_setup,(ctl));			/* set up file logging 				*/

  if (param->connect != NULL &&
	  strcmp(param->connect, "none"))
    U_C(dtipc_register,(ctl,			/* establish listen address			*/
      &umplay->command_ipc,
      param->connect, 0, 0));

  U_C(umplay_umdec_init,(umplay, param,	/* initialize the umdec object 		*/
    umdec));
  umdec_initialized = 1;

  while(1)
  {
	int sysrc;
	unsigned long mode = 1;

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "waiting for client to open"
      " data fifo %s",
      umplay->data_fifo_name);
	U_C(dtfd_open,(ctl,					/* open data fifo 					*/
      &umplay->data_fifo,
      umplay->data_fifo_name, "r"));

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "waiting for client to open"
      " command fifo %s",
      umplay->command_fifo_name);
	U_C(dtfd_open,(ctl,					/* open command fifo 				*/
      &umplay->command_fifo,
      umplay->command_fifo_name, "r"));

	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "waiting for client to open"
      " reply fifo %s",
      umplay->reply_fifo_name);
	U_C(dtfd_open,(ctl,					/* open reply fifo 					*/
      &umplay->reply_fifo,
      umplay->reply_fifo_name, "w"));
  
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "waiting for client to send"
      " init command on command fifo %s"
      " (%d bytes)", 
      umplay->command_fifo.filename,
      sizeof(deccmd));

	U_C(dtfd_read_all,(ctl,				/* read initial command 			*/
      &umplay->command_fifo,
      (void **)&deccmd,
      sizeof(deccmd)));

	if (deccmd.cmd !=					/* invalid initial command? 		*/
	    DECCMD_INITPARAM)
	{
	  rc = dt_err(ctl, F,
        "init command 0 expected"
        " but got command %d",
        (int)deccmd.cmd);
	  goto cleanup;
	}
	
	if (deccmd.u.playSpeed < -1)		/* prepare initial mode 			*/
  	  umplay->control.mode =
        UMDEC_MODE_SFORWARD;
	else
	if (deccmd.u.playSpeed > 1)
  	  umplay->control.mode =
        UMDEC_MODE_FFORWARD;
	else
  	  umplay->control.mode =
        UMDEC_MODE_FORWARD;
	
	sysrc = ioctl(						/* go into non-blocking mode 		*/
      umplay->command_fifo.fd, 
      FIONBIO, &mode);
	if (sysrc < 0)
	{
	  rc = dt_err(ctl, F,
		"ioctl(%d, FIONBIO, 1)"
		DTERRNO_FMT,
        umplay->command_fifo.fd, 
		DTERRNO_ARG(errno));
	  goto cleanup;
	}

	sysrc = ioctl(						/* go into non-blocking mode 		*/
      umplay->data_fifo.fd, 
      FIONBIO, &mode);
	if (sysrc < 0)
	{
	  rc = dt_err(ctl, F,
		"ioctl(%d, FIONBIO, 1)"
		DTERRNO_FMT,
        umplay->data_fifo.fd, 
		DTERRNO_ARG(errno));
	  goto cleanup;
	}

	U_C(umdec_go,(ctl, umdec,			/* this starts playing immediately 	*/
      umplay_feed, (void *)umplay,
      umplay_frame, (void *)umplay,
      &umplay->control));

	U_C(dtfd_close,(ctl,				/* close the fifos 					*/
      &umplay->reply_fifo));

	U_C(dtfd_close,(ctl, 
      &umplay->command_fifo));

	U_C(dtfd_close,(ctl, 
      &umplay->data_fifo));
	
	if (umplay->exiting)				/* we have been told to exit? 		*/
	  goto cleanup;
  }
  
cleanup:
  if (umdec_initialized)
    U_I(umdec_uninit,(ctl, umdec));		/* clean up the umdec object 		*/

  if (umplay != NULL)
    U_I(umplay_uninit,(param, umplay));

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
