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
| netchat_* - netchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <netchat.h>
DT_RCSID("app/netchat $RCSfile: init.c,v $ $Revision: 1.8 $");
#include <dtack/str.h>
#include <dtack/liner.h>

/*..........................................................................
 * initialize netchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_init(
  netchat_t **pnetchat,
  dtparam_t *param)
{
  DT_F("netchat_init");
  dt_ctl_t *ctl = &param->ctl;
  netchat_t *netchat = NULL;
  dtfd_t fd;
  dt_rc_e rc = DT_RC_GOOD;

  fd.fd = -1;

  DT_C(dtos_malloc2,(ctl,
    (void **) &netchat,
    sizeof(*netchat),
    F, "netchat structure"));
  
  memset(netchat, 0, 
    sizeof(*netchat));

  netchat->ctl = &param->ctl;
  netchat->ident = param->ident;		/* absolutely required 				*/
  netchat->poll_period = param->dt;
  netchat->state = 
    NETCHAT_STATE_STARTING;

  DT_C(netchat_log_setup,(netchat));	/* set up file logging 				*/

  DT_C(ustate_init_config,(ctl,			/* get ustate database 				*/
    &netchat->ustate, param->config));

  DT_C(ustate_find,(ctl,				/* get our dgserver listen address	*/
    &netchat->ustate,
    param->ident, "netchat",
    "dgserver_spec", 
    param->connect,
    &netchat->dgserver_spec));

										/* ................................ */
  if (strstr(param->dialog, ","))		/* looks like a dialog spec? 		*/
  {
	DT_C(netchat_dialog_init,(
      netchat, param));
  }
  else									/* else presume graphics spec 		*/
  if (strcmp(param->dialog, "none"))	/* unless it is none 				*/
  {
	DT_C(netchat_graphics_init,(
      netchat, param));
  }

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  if (rc != DT_RC_GOOD)
  {
    DT_I(netchat_uninit,(netchat));
	DT_I(netchat_log_quit,(netchat));	/* quit with the file logging 		*/
  }
  else
    *pnetchat = netchat;

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
netchat_uninit(
  netchat_t *netchat)
{
  DT_F("netchat_uninit");
  dt_rc_e rc = DT_RC_GOOD;

  if (netchat != NULL)
  {
	dt_ctl_t *ctl = netchat->ctl;

	if (netchat->dialog_initialized)
	  DT_I(netchat_dialog_uninit,(
        netchat));

	if (netchat->graphics_initialized)
	  DT_I(netchat_graphics_uninit,(
        netchat));

	DT_I(ustate_uninit,(ctl,
      &netchat->ustate));
  
	DT_I(netchat_log_quit,(netchat));	/* quit with the file logging 		*/

	DT_I(dtos_free2,(ctl, netchat,
      F, "netchat structure"));
  }

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
