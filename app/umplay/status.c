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
U_R("app/umplay $RCSfile: status.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

u_rc_e 
U_API
umplay_status_string(
  umplay_t *umplay,
  char *string,
  int max)
{
  U_F("umplay_status_string");
  u_ctl_t *ctl = umplay->ctl;

  dtstr_printf(ctl, string, max,
    "%s", "status:");

  if (UMPLAY_HAS_SOURCE(umplay))		/* we already have a data source?	*/
  {
	U_Q(dtstr_printf,(ctl, 
      string+strlen(string), 
      max-strlen(string),
      " input from %s",
      umplay->fd.filename));

	if (UMPLAY_IS_PAUSED(umplay))
      U_Q(dtstr_printf,(ctl, 
        string+strlen(string), 
        max-strlen(string),
        " %s", "paused"))
	else
	  U_Q(dtstr_printf,(ctl, 
        string+strlen(string), 
        max-strlen(string),
        " %s", "playing"));

	U_Q(dtstr_printf,(ctl, 
      string+strlen(string), 
      max-strlen(string),
      " at frame %g",
      umplay->frame));
  }
  else
	U_Q(dtstr_printf,(ctl, 
      string+strlen(string), 
      max-strlen(string),
      " no input specified"));

  return U_RC_GOOD;
}

/*..........................................................................*/

u_rc_e 
U_API
umplay_status_say(
  umplay_t *umplay)
{
  U_F("umplay_status_say");
  u_ctl_t *ctl = umplay->ctl;
  char string[256];

  U_Q(umplay_status_string,(umplay,
    string, sizeof(string)));

  u_say(ctl, NULL, "%s", string);

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
