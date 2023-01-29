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
U_R("app/umplay $RCSfile: feedkey.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

u_rc_e 
U_API
umplay_keybd_poll(
  umplay_t *umplay)
{
  U_F("umplay_keybd_poll");
  u_ctl_t *ctl = umplay->ctl;
  int iskey;
  dt_key_t key;
  char command[256];
  
  umplay->response[0] = '\0';			/* clear response field 			*/

  U_Q(dtkeybd_scan,(ctl,				/* poll keyboard for command		*/
    &umplay->keybd, &iskey, &key));
        
  if (iskey)							/* we have a key? 					*/
  {
	umplay_command_f *command_func;
	const char *string;
  
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
	"iskey=%d, key=%d", 
    iskey, iskey? key: 0);

	U_Q(umplay_command_func_from_key,(
      ctl, key, &command_func,
      umplay->response,
      sizeof(umplay->response)));

	if (!umplay->response[0])
	U_Q(umplay_command_string_from_key,(
      ctl, key, &string,
      umplay->response,
      sizeof(umplay->response)));

	if (key == UMPLAY_KEY_SOURCE)
	{
	  dtstr_printf(ctl,
        command, sizeof(command),
        "enter filename: ");
	  u_say_string(ctl,
        command, strlen(command));
	  dtstr_printf(ctl,
        command, sizeof(command),
        "%s ", string);
	  U_Q(umplay_keybd_line,(ctl,
        &umplay->keybd,
        command+strlen(command),
        sizeof(command)-strlen(command)));
	  string = command;
	}

	if (!umplay->response[0])
	U_Q((*command_func),(umplay, 
      string));
  }

  if (umplay->response[0])				/* some response needed? 			*/
  {
    u_say(ctl, NULL, "%s",
      umplay->response);
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
