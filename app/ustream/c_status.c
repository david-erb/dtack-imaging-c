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

#include <ustream.h>
DT_RCSID("app/ustream $RCSfile: c_status.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_status(							/* update current status string 	*/
  ustream_t *ustream,
  const char *format,
  ...)
{
  DT_F("ustream_status");
  dt_ctl_t *ctl = ustream->ctl;
  dtos_time_t time;
  char *s = ustream->status;
  va_list ap;

  DT_Q(dtos_time,(ctl, &time));
  DT_Q(dtstr_printf,(ctl, 
    s, sizeof(ustream->status),
    "[dt=%6.1f] ", 
    time.wall - ustream->time.wall));
  
  va_start(ap, format);
  DT_Q(dtstr_printf_va,(ctl,
    s+strlen(s),
    sizeof(ustream->status)-strlen(s),
    format, ap));
  va_end(ap);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
ustream_cmd_status(						/* handle status datagram request 	*/
  ustream_t *ustream,
  dtdgserver_t *dgserver,
  const char *command,
  char *arg)
{
  DT_F("ustream_cmd_status");
  dt_ctl_t *ctl = ustream->ctl;
  dt_rc_e rc = DT_RC_GOOD;
  DT_Q(ustream_response,(ustream, dgserver, 
    "server status: %s", ustream->status));
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
