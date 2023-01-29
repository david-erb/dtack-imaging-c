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
U_R("app/umdemo $RCSfile: feedkey.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

u_rc_e 
U_API
umdemo_keybd_poll(
  umdemo_t *umdemo)
{
  U_F("umdemo_keybd_poll");
  u_ctl_t *ctl = umdemo->ctl;
  int iskey;
  dt_key_t key;
  char command[256];
  
  umdemo->response[0] = '\0';			/* clear response field 			*/

  U_Q(dtkeybd_scan,(ctl,				/* poll keyboard for command		*/
    &umdemo->keybd, &iskey, &key));

  if (iskey)							/* we have a key? 					*/
  {
	umdemo_command_f *command_func;
	const char *string;

	U_Q(umdemo_command_func_from_key,(
      ctl, key, &command_func,
      umdemo->response,
      sizeof(umdemo->response)));

	if (!umdemo->response[0])
	U_Q(umdemo_command_string_from_key,(
      ctl, key, &string,
      umdemo->response,
      sizeof(umdemo->response)));

	if (key == UMDEMO_KEY_SOURCE)
	{
	  dtstr_printf(ctl,
        command, sizeof(command),
        "enter filename: ");
	  u_say_string(ctl,
        command, strlen(command));
	  dtstr_printf(ctl,
        command, sizeof(command),
        "%s ", string);
	  U_Q(umdemo_keybd_line,(ctl,
        &umdemo->keybd,
        command+strlen(command),
        sizeof(command)-strlen(command)));
	  string = command;
	}

	if (!umdemo->response[0])
	U_Q((*command_func),(umdemo, 
      string));
  }

  if (umdemo->response[0])				/* some response needed? 			*/
  {
    u_say(ctl, NULL, "%s",
      umdemo->response);
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
