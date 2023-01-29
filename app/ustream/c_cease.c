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
DT_RCSID("app/ustream $RCSfile: c_cease.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
ustream_cmd_cease(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("ustream_cmd_cease");
  ustream_t *ustream = (ustream_t *)arg;

  if (ustream->flag_shovel)				/* we are currently shoveling? 		*/
  {
	DT_Q(dtdg_server_response,(ctl, 
      dgserver, 
      DTDG_RESPONSE_RESULT
      ": ceasing shoveling"));
	ustream->flag_shovel = 0;			/* quit shoveling 					*/

    ustream->istream_spec[0] = '\0';	/* clear partnership fields 		*/
    ustream->ostream_spec[0] = '\0';
    ustream->permit[0] = '\0';

	ustream->last_message[0] = '\0';	/* clear previous error message 	*/
  }
  else
  {
	return dt_err(ctl, F,
      "not currently shoveling");
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
