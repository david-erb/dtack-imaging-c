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
U_R("app/umdemo $RCSfile: frame.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

u_rc_e
U_API
umdemo_frame(
  u_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  umdec_frametype_e frametype,			/* type of frame being given 		*/
  void **buffer,						/* buffers containing frame data	*/
  int xe,								/* frame width 						*/
  int ye)								/* frame height						*/
{
  U_F("umdemo_raw");
  umdemo_t *umdemo = (umdemo_t *)arg;
  u_rc_e rc;

  if (frametype ==
	  UMDEC_FRAMETYPE_YCBCR_420)
  {
  }
  else
  if (frametype ==						/* just a frame increment? 			*/
	  UMDEC_FRAMETYPE_FRAMEINC)
  {
	umdec_status_t status;
    U_Q(umdec_get_status,(ctl, umdec,	/* get current status               */
      &status));
	umdemo->frame_last = status.frame;
  }
  else
  {
	return u_err(ctl, F,
      "cannot handle UMDEC_FRAMETYPE=%d",
      frametype);
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
