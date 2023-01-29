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
DT_RCSID("app/ustream $RCSfile: init.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_init(
  ustream_t *ustream,
  dtparam_t *param)
{
  DT_F("ustream_init");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;
  
  memset(ustream, 0, sizeof(*ustream));
  
  ustream->ctl = ctl;
  ustream->ident = param->ident;		/* absolutely required 				*/

  DT_C(ustream_log_setup,(ustream));	/* set up file logging 				*/

  DT_C(ustate_init_config,(ctl,			/* get ustate database 				*/
    &ustream->ustate, param->config));

  DT_C(ustate_find,(ctl,				/* get our dgserver listen address	*/
    &ustream->ustate,
    param->ident, "ustream",
    "dgserver_spec", 
    param->connect,
    &ustream->dgserver_spec));

  DT_C(ustate_find,(ctl,				/* get istream preference 			*/
    &ustream->ustate,
    param->ident, "ustream",
    "istream_preference", 
    param->path,
    &ustream->istream_preference));

  ustream->poll_period = param->dt;
  ustream->connect_timeout = 2000;		/* milliseconds 					*/
  ustream->accept_timeout = 2000;

cleanup:
  if (rc != DT_RC_GOOD)
  {
    DT_I(ustate_uninit,(ctl, 
      &ustream->ustate));
	DT_I(ustream_log_quit,(ustream));	/* quit with the file logging 		*/
  }
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustream_uninit(
  ustream_t *ustream)
{
  DT_F("ustream_uninit");
  dt_ctl_t *ctl = ustream->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(ustate_uninit,(ctl,
    &ustream->ustate));
  
  DT_I(ustream_log_quit,(ustream));		/* quit with the file logging 		*/

  memset(ustream, 0, sizeof(*ustream));

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
