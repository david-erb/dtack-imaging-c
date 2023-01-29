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

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: feedback.c,v $ $Revision: 1.8 $");

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_feedback(						/* check for feedback from java		*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
{
  DT_F("tabdrone_feedback");
  dt_ctl_t *ctl = tabdrone->ctl;
  int isready;
  char cmd[256];
  long d;
  dt_rc_e rc;

  while(1)								/* eat all available input 			*/
  {
	int l;
	if (tabdrone->sponge_frames &&		/* pay attention to sponging? 		*/
		tabdrone->frames_sent -
		tabdrone->frames_acked >
		tabdrone->sponge_frames)
	{
      DT_C(dtipc_poll,(ctl, ipc,		/* wait for ack						*/
        DTIPC_POLL_READ, 
        30000,
        "ipc read", &isready));
	  if (!isready)						/* no data ready? 					*/
	  {
		dt_dbg(ctl, F, 
          DT_DBG_MASK_USR1,
		  "timeout waiting for ack");
		rc = DT_RC_STOP;
	    goto cleanup;
	  }
	}
	else								/* don't care about sponge? 		*/
	{
      DT_C(dtipc_poll,(ctl, ipc,		/* poll for incoming data 			*/
        DTIPC_POLL_READ, 
        0,
        "ipc read", &isready));
	  if (!isready)						/* no data ready? 					*/
	    goto cleanup;
	}

	DT_C(dtipc_read_line,(ctl,			/* get request line 				*/
      ipc, cmd, sizeof(cmd)));
		
	l = strlen(cmd);
	if (l == 0)							/* input socket is closed? 			*/
	{
	  dt_dbg(ctl, F,
        DT_DBG_MASK_COMMAND|
		DT_DBG_MASK_USR1, 
        "scan for feedback detects"
        " socket is closed");
	  rc = DT_RC_STOP;
	  goto cleanup;
	}
	
	while (l > 0 &&						/* nuke the crlf's 					*/
		   cmd[l-1] == LF ||
		   cmd[l-1] == CR)
	  cmd[--l] = '\0';	

										/* ................................ */
	if (l == 0)							/* blank line? 						*/
	{
	}
										/* ................................ */
	else
	if (!strncmp(cmd, "GET", 3))
	{
	  char *p;
	  p = strstr(cmd, "sponge_frames=");
	  if (p != NULL)
	  {
		p = strchr(p, '=') + 1;
		if (!sscanf(p, "%ld", 
          &tabdrone->sponge_frames))
		{
		  rc = dt_err(ctl, F,
            "invalid sponge_frames in \"%s\"",
            cmd);
		  goto cleanup;
		}
		dt_dbg(ctl, F, 
          DT_DBG_MASK_COMMAND,
		  "sponge frames set to %ld",
          tabdrone->sponge_frames);
	  }
	}
										/* ................................ */
	else
	if (!strncmp(cmd, "ack", 3))
	{
	  tabdrone->frames_acked++;
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_COMMAND,
        "got ack \"%s\", sent=%ld, acked=%ld",
        cmd,
		tabdrone->frames_sent,
		tabdrone->frames_acked);
	}
										/* ................................ */
	else
	if (1 == sscanf(cmd,
		  "delay,%ld", &d))
	{
	  tabdrone->delay =					/* convert to milliseconds 			*/
        (double)tabdrone->param->w *
        (double)d / 100.0;
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_COMMAND,
	    "got \"%s\""
        " new delay %ld ms",
        cmd, tabdrone->delay);
	}
										/* ................................ */
	else
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
	    "ignored \"%s\"", cmd);
	}
	  
  }
  
cleanup:
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
