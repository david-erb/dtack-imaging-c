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
| USAGE
| version
| 
| 02-16 19991109 don't care abount unacknowledged dead child
| 02-13 19980802 require audio
| 02-13 19980802 recompile
| 02-12 19980422 ?
| 02-05 19970917 start url to drone
| 02-04 19970901 set port range (-min, -max)
|                parameterize  poll time timeout (-dt)
|                parent authorization (-parent)
|
| END
 *..........................................................................*/

#include <wrangler.h>
DT_RCSID("app/wrangler $RCSfile: rev.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "wrangler app";
  if (rev_major != NULL)
    *rev_major = 02;
  if (rev_minor != NULL)
    *rev_minor = 16;
  if (rev_date != NULL)
    *rev_date = 19991109;
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
