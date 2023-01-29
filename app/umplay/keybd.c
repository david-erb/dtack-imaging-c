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
U_R("app/umplay $RCSfile: keybd.c,v $ $Revision: 1.8 $");

/*..........................................................................*/

u_rc_e 
U_API
umplay_keybd_line(
  u_ctl_t *ctl,
  dtkeybd_t *keybd,
  char *line,
  int max)
{
  U_F("umplay_keybd_line");
  dt_key_t key;
  int n = 0;
  
  do {
	char c;

	U_Q(dtkeybd_read,(ctl,				/* wait for key 					*/
      keybd, &key));

	c = (char)key;
	if (key == DT_KEY_BACKSPACE &&		/* backspace is special 			*/
		n > 0)
	{
	  c = 0x08;
	  n--;
	}
	else
	if (key == DT_KEY_ENTER)			/* enter is special 			*/
	{
	  c = '\n';
	  line[n] = '\0';
	}
	else
	if (key < 0xff &&					/* regular ASCII character? 		*/
		isprint(c) &&
		n < max)						/* and line not full? 				*/
	{
	  line[n++] = c;
	}
	else								/* non-printable character? 		*/
	  c = 0x07;							/* beep 							*/
	
	if (c != '\0')
      u_say_string(ctl, &c, 1);

  }	while (key != DT_KEY_ENTER);		/* enter terminates input 			*/

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
