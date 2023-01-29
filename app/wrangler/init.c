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
| main program
| 
| END
 *..........................................................................*/

#include <wrangler.h>
DT_RCSID("app/wrangler $RCSfile: init.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>
#include <dtack/liner.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_uninit(
  wrangler_t *wrangler)
{
  DT_F("wrangler_uninit");
  dt_ctl_t *ctl = wrangler->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (wrangler->drone_args != NULL)
	DT_I(dtos_free2,(ctl,
      wrangler->drone_args,
      F, "drone args"));

  DT_I(dtprocs_uninit,(ctl, 
    &wrangler->procs));
  
  memset(wrangler, 0, 
    sizeof(*wrangler));

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
wrangler_init(
  wrangler_t *wrangler,
  dtparam_t *param)
{
  DT_F("wrangler_init");
  dt_ctl_t *ctl = &param->ctl;
  dtos_time_t now;
  dt_rc_e rc;

  memset(wrangler, 0, sizeof(*wrangler));
  wrangler->ctl = ctl;
  wrangler->param = param;
  wrangler->ident = param->ident;
  wrangler->docpath = param->datadir;
  wrangler->executable = param->path;
  wrangler->drone_template = param->config;
  wrangler->parent_spec = param->parent;;
  wrangler->max_children = param->n;
  wrangler->min_port = param->min;
  wrangler->max_port = param->max;

  DT_C(dtprocs_init,(ctl, 
    &wrangler->procs, param->config));

  DT_C(dtos_time,(ctl, &now));			/* current historical second		*/
  DT_C_1D(dt1d_ran_uniform_init,(		/* init random number generation 	*/
    &wrangler->random,
    (long)now.wall));
  
cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(wrangler_uninit,(wrangler));
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
