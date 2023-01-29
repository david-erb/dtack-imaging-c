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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

/*..........................................................................
 * dtscr interface implemented on error calls
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/os.h>

DT_RCSID("source/screen $RCSfile: scrdum.c,v $ $Revision: 1.1 $");

typedef struct {
  long id_pool;
} priv_t;

  static dtscr_create_window_f dtscr_dummy_create_window;
  static dtscr_wait_event_f dtscr_dummy_wait_event;
  static dtscr_destroy_f dtscr_dummy_destroy;

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_dummy_create(						/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *server)
{
  DT_F("dtscr_dummy_create");
  priv_t *priv;

  DT_Q(dtos_malloc2,(ctl,				/* space for priv stuff		    	*/
    (void **)&priv, 
    sizeof(*priv),
    F, "priv"));

  priv->id_pool = 1000;					/* begin id sequence numbers	    */

  scr->xe = 800;
  scr->ye = 600;

  scr->priv =           (void *)priv;
  scr->create_window =  dtscr_dummy_create_window;
  scr->wait_event =     dtscr_dummy_wait_event;
  scr->destroy =        dtscr_dummy_destroy;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_dummy_create_window(				/* create new window				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)
{
  priv_t *priv = (priv_t *)scr->priv;

  win->id = priv->id_pool++;			/* give it a new id				    */

  return DT_RC_GOOD;
}

/*..........................................................................
 * this dummy "wait event" always returns the QUIT key to satisfy
 *   servers which may otherwise wait for real interactive input
 * this may not work if the QUIT action has been disassociated from
 *   the QUIT key
 * it might be better if this routine could find out which key is
 *   associated with the QUIT action
 * it would be best if this routine could just specify a QUIT action 
 *   directly
 *..........................................................................*/

static
dt_rc_e
dtscr_dummy_wait_event(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  if (event != NULL)
  {
    DT_MEMSET(event, 0, sizeof(*event));
    if (timeout != -1)					/* this is not a poll? 				*/
    {
      event->type = DT_EVENT_KEYBOARD;
      event->keys[0] = DTSCR_KEY_QUIT;
      event->nkeys = 1;
    }
  }
  if (event->type == 0)					/* nothing happened?				*/
  {
	event->type = DT_EVENT_NOTHING;		/* ignore it 						*/
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_dummy_destroy(					/* break down connection			*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_dummy_destroy");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dtos_free2,(ctl, 
    priv, F, "priv"));

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
