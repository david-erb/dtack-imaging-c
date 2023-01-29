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
 * this file contains functions which cannot live in a Microsoft Windows dll
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtimage_assign(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of args				*/
  ...)									/* variable arguments 				*/
{
  DT_F("dtimage_assign");

  va_list ap;
  va_start(ap, ntype);

  DT_Q(dtimage_assign_va,(ctl, image,	/* use other function 				*/
    xe, ye, ntype, ap));
  
  va_end(ap);

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
