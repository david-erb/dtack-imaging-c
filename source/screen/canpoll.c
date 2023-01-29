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
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: canpoll.c,v $ $Revision: 1.7 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/lut8.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_poll(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dt_event_t *event,
  double timeout,
  int flag,
  dtkeybd_t *keybd,
  dtcanvas_mouse_f *mousefunc,
  int *mousex,
  int *mousey,
  dtos_time_t *unclick_time,
  int *click_state)
{
  DT_F("dtcanvas_poll");

  if (event)
  do {
	int iskey;
	int buttons;

    DT_MEMSET(event, 0, sizeof(*event));	/* clear event structure		    */
	
    DT_Q(dtkeybd_scan,(ctl,				/* check for key					*/
      keybd,
      &iskey, 
      &event->keys[0]));
	if (iskey)
	{
	  event->nkeys = 1;					/* number of keys we got		    */
	  event->type = DT_EVENT_KEYBOARD;
	  if (mousefunc)					/* mouse not turned off? 			*/
  	    DT_Q(mousefunc,(ctl, canvas,	/* poll mouse status 				*/
          &event->mousex,
          &event->mousey,
		  &buttons));
	  break;
	}
	
	if (mousefunc)						/* mouse not turned off? 			*/
	{
	  DT_Q(mousefunc,(ctl, canvas,		/* poll mouse status 				*/
        &event->mousex,
        &event->mousey,
		&buttons));
	  event->left = !!(buttons & 0x01);
	  event->middle = !!(buttons & 0x02);
	  event->right = !!(buttons & 0x04);
	  if (buttons != 0)					/* some buttons down? 				*/
	  {
		double dt;
		dtos_time_t now;
		DT_Q(dtos_time,(ctl, &now));	/* time at this click 				*/
        dt = now.wall -					/* time since last unclick 			*/
          unclick_time->wall;
		if (buttons == *click_state &&	/* same button state? 				*/
            dt < 0.300)					/* and so quick? 					*/
		{
		  event->type = 
            DT_EVENT_MOUSEDOUBLEPRESS;
		  if (event->left) event->left++;
		  if (event->middle) event->middle++;
		  if (event->right) event->right++;
		}
		else
		  event->type = 
            DT_EVENT_MOUSEPRESS;

		*click_state = buttons;
		
		while(buttons != 0)				/* wait for button release 			*/
		{
		  int x, y;
    	  DT_Q(mousefunc,(ctl, canvas,
            &x, &y, &buttons));
		}
		DT_Q(dtos_time,(ctl,			/* time at this unclick 			*/
          unclick_time));
	    break;
	  }
	  else								/* no buttons down? 				*/
	  if (event->mousex != *mousex ||	/* mouse has moved? 				*/
          event->mousey != *mousey)
	  {
		event->type = DT_EVENT_MOUSEMOVE;
	    break;
	  }
	}
	if (flag == 0)						/* just a poll? 					*/
	{
  	  DT_Q(mousefunc,(ctl, canvas,		/* poll mouse status 				*/
        &event->mousex,
        &event->mousey,
	    &buttons));
      break;
	}
  } while (timeout >= 0);

  *mousex = event->mousex;
  *mousey = event->mousey;

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
