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
DT_RCSID("app/ustream $RCSfile: c_state.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_state(							/* update current state string 	*/
  ustream_t *ustream,
  const char *format,
  ...)
{
  DT_F("ustream_state");
  dt_ctl_t *ctl = ustream->ctl;
  dtos_time_t time;
  char *s = ustream->state;
  va_list ap;

  DT_Q(dtos_time,(ctl, &time));

  s[0] = '\0';

  va_start(ap, format);
  DT_Q(dtstr_printf_va,(ctl,
    s+strlen(s),
    sizeof(ustream->state)-strlen(s),
    format, ap));

  DT_Q(dtstr_printf,(ctl, 
    s+strlen(s),
    sizeof(ustream->state)-strlen(s),
    " [dt=%6.1f]", 
    time.wall - ustream->time.wall));

  va_end(ap);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
ustream_cmd_state(						/* handle state datagram request 	*/
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max)
{
  DT_F("ustream_cmd_state");
  ustream_t *ustream = (ustream_t *)arg;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtdg_server_response,(ctl, 
    dgserver,
    DTDG_RESPONSE_STATE
    ": %s%s%s", 
    ustream->flag_locked? DTDG_STATE_LOCKED: "",
    ustream->flag_locked? ", ": "",
    ustream->state));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    DTDG_RESPONSE_OSTREAM_SPEC
    ": %s", 
    ustream->ostream_spec));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    DTDG_RESPONSE_ISTREAM_SPEC
    ": %s", 
    ustream->istream_spec));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    DTDG_RESPONSE_PERMIT
    ": %s", 
    ustream->permit));

  DT_Q(dtdg_server_response,(ctl, 
    dgserver, 
    DTDG_RESPONSE_LASTERROR
    ": %s", 
    strlen(ustream->last_message)?
      ustream->last_message: "none"));
  
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
