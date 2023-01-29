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
U_R("app/umdemo $RCSfile: status.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

u_rc_e 
U_API
umdemo_status_string(
  umdemo_t *umdemo,
  char *string,
  int max)
{
  U_F("umdemo_status_string");
  u_ctl_t *ctl = umdemo->ctl;

  dtstr_printf(ctl, string, max,
    "%s", "status:");

  if (UMDEMO_HAS_SOURCE(umdemo))		/* we already have a data source?	*/
  {
	U_Q(dtstr_printf,(ctl, 
      string+strlen(string), 
      max-strlen(string),
      " input from %s",
      umdemo->fd.filename));

	if (UMDEMO_IS_PAUSED(umdemo))
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
      umdemo->frame_last));
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
umdemo_status_say(
  umdemo_t *umdemo)
{
  U_F("umdemo_status_say");
  u_ctl_t *ctl = umdemo->ctl;
  char string[256];

  U_Q(umdemo_status_string,(umdemo,
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
