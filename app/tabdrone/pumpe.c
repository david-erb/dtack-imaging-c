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
DT_RCSID("app/tabdrone $RCSfile: pump.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_pump(							/* pump input to output 			*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
{
  DT_F("tabdrone_pump");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  void *data = NULL;
  long ndata;
  long xe, ye;
  void *buffer = NULL;
  long nbuffer;
  long obuffer;
  dt_rc_e rc;
  
  while(1)								/* frame loop 						*/
  {
	DT_C(tabdrone_feedback,(tabdrone,	/* check for input 					*/
      ipc));
	DT_C(tabdrone_grab,(param,			/* grab frame						*/
	  &data, &ndata, &xe, &ye));
	DT_C(tabdrone_compress,(param,		/* compress frame 					*/
      data, ndata, 
      xe, ye,
      param->t,
      &buffer, &nbuffer, &obuffer));

	DT_C(tabdrone_output,(tabdrone,		/* output compressed frame			*/
      buffer, obuffer, 
      NULL,								/* can only be video 				*/
      ipc));

	DT_C(tabdrone_limit,(tabdrone));	/* enforce time limit 				*/

	DT_C(dtos_delay,(ctl,				/* delay if required 				*/
	  tabdrone->delay));
  }
  
cleanup:
  if (buffer != NULL)
	DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));
  if (data != NULL)
	DT_I(dtos_free2,(ctl, data,
      F, "data"));

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
