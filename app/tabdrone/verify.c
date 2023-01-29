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
DT_RCSID("app/tabdrone $RCSfile: verify.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_verify(
  dtparam_t *param,
  dtipc_t *ipc)
{
  DT_F("tabdrone_verify");
  dt_ctl_t *ctl = &param->ctl;
  int c1, c2, c3, c4, cport;
  int v1, v2, v3, v4, vport;
  char *p;
  int n;

										/* ................................ */
  p = strchr(ipc->client_spec, ',');
  n = sscanf(p? p+1: ipc->client_spec,
    "%d.%d.%d.%d:%d",
    &c1, &c2, &c3, &c4, 
    &cport);
  if (n != 5 && n != 4)
	return dt_err(ctl, F,
      "unexpected client spec \"%s\""
      " (n=%d)",
      ipc->client_spec, n);
  
										/* ................................ */
  p = strchr(param->name, ',');
  n = sscanf(p? p+1: param->name,
    "%d.%d.%d.%d:%d",
    &v1, &v2, &v3, &v4, 
    &vport);
  if (n != 5 && n != 4)
	return dt_err(ctl, F,
      "unexpected verify spec \"%s\""
      " (n=%d)",
      param->name, n);

										/* ................................ */
  if (v1 != 0 && v1 != c1 ||
	  v2 != 0 && v2 != c2 ||
	  v3 != 0 && v3 != c3 ||
	  v4 != 0 && v4 != c4)
	return dt_err(ctl, F,
      "wrong client connected \"%s\"",
      ipc->client_spec);

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
