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

#include <ustream.h>
DT_RCSID("app/ustream $RCSfile: c_lock.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

#include <dtstream.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_cmd_lock(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("ustream_cmd_stream");
  ustream_t *ustream = (ustream_t *)arg;

  ustream->message[0] = '\0';			/* clear error message 				*/
  ustream->last_message[0] = '\0';

  if (ustream->flag_locked)				/* currently locked? 				*/
  {
    ustream->flag_locked = 0;			/* release lock 					*/
	DT_Q(dtdg_server_response,(ctl, 
      dgserver, 
      DTDG_RESPONSE_RESULT
      ": unlocking"));
  }
  else
  {
	ustream->flag_locked = 1;
	ustream->flag_shovel = 0;			/* quit shoveling 					*/
	DT_Q(dtdg_server_response,(ctl, 
      dgserver, 
      DTDG_RESPONSE_RESULT
      ": locking"));
  }
  
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
