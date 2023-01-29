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

#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: delay.c,v $ $Revision: 1.8 $");
#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/str.h>
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_server_delay(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("dgdg_server_delay");

  if (dgserver != NULL)
  {
	long milliseconds;
	int isnumber;
	DT_Q(dtstr_to_long2,(ctl, rest,
      &milliseconds, &isnumber));
	if (isnumber)
	{
	  DT_Q(dtos_delay,(ctl, milliseconds));
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RC
        ": %d", DT_RC_GOOD));
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RESULT
        ": delayed %ld", milliseconds));
	}
	else
	{
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RC
        ": %d", DT_RC_BAD));
	  DT_Q(dtdg_server_response,(ctl, dgserver,
        DTDG_RESPONSE_RESULT
        ": \"%s\" is not a number", rest));
	}
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
