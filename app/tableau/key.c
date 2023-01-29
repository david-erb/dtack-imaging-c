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
| main program
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/xchg.h>

DT_RCSID("test $RCSfile: key.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
tableau_keywait(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dt_key_t *key)
{
  DT_F("tableau_keywait");

  DT_Q(dtscr_wait_event,(ctl,			/* wait for user event				*/
    scr, NULL, 3));

  if (scr->last_event.nkeys == 0)
    *key = DT_KEY_IGNORE;
  else
    *key = scr->last_event.keys[0];

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "nkeys %d, event key 0x%04x, *key 0x%04x", 
    scr->last_event.nkeys,
    scr->last_event.keys[0],
    *key);
  
  if (*key == DTSCR_KEY_QUIT)			/* user hit quit key?			    */
    return DT_RC_STOP;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
tableau_keypoll(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dt_key_t *key)
{
  DT_F("tableau_keypoll");

  DT_Q(dtscr_wait_event,(ctl,			/* poll for event					*/
    scr, NULL, 0));

  if (scr->last_event.nkeys == 0)
    *key = DT_KEY_IGNORE;
  else
    *key = scr->last_event.keys[0];

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "nkeys %d, event key 0x%04x, *key 0x%04x", 
    scr->last_event.nkeys,
    scr->last_event.keys[0],
    *key);
  
  if (*key == DTSCR_KEY_QUIT)			/* user hit quit key?			    */
    return DT_RC_STOP;

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
