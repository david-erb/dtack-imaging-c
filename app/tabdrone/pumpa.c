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
| Pump from already-compressed directory.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: pumpa.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_pump(							/* pump input to output 			*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
{
  DT_F("tabdrone_pumpa");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  const char *stream = param->input;
  int isreadable;
  dt_rc_e rc;

  DT_C(dtos_isreadable,(ctl, stream,	/* stream is a readable filename? 	*/
    &isreadable));
  
  if (isreadable)
	DT_C(tabdrone_clip,(tabdrone, ipc))
  else
	DT_C(tabdrone_live,(tabdrone, ipc))
  
cleanup:
  
  return rc;
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
