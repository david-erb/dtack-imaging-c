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

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: limit.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_limit(							/* limit frames						*/
  tabdrone_t *tabdrone)					/* application state 				*/
{
  DT_F("tabdrone_limit");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;

  if (param->max > 0)					/* time limit imposed? 				*/
  {
	dtos_time_t now;
	DT_Q(dtos_time,(ctl, &now));
	if (tabdrone->frames_sent > 0 &&
		now.wall -
		tabdrone->start_time.wall > 
		param->max)
	{
	  dt_dbg(ctl, NULL, DT_DBG_MASK_USR1,
        "limit %g seconds exceeded",
        param->max);
	  return DT_RC_STOP;
	}
  }

  if (param->max < 0)					/* frame limit imposed? 			*/
  {
	if (tabdrone->frames_sent >
		-param->max)
	{
	  dt_dbg(ctl, NULL, DT_DBG_MASK_USR1,
        "limit %g frames exceeded",
        -param->max);
	  return DT_RC_STOP;
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
