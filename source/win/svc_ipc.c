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
| DESCRIPTION
| This is the ipc cover level.
|
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: svc_ipc.c,v $ $Revision: 1.1 $");

#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/ipc.h>
#include <dtack/os.h>

typedef struct {
  dtipc_t ipc;
} priv_t;

/*..........................................................................*/
static
dt_rc_e
dtsvc_ipc_unregister(					/* unregister service name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc)
{
  DT_F("dtsvc_ipc_unregister");
  priv_t *priv = (priv_t *)
    svc->priv;
  dtipc_t *ipc = &priv->ipc;
  dt_rc_e rc = DT_RC_GOOD;
  DT_I(dtipc_unregister,(ctl, ipc));
  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtsvc_ipc_wait(							/* wait for request					*/
  dt_ctl_t *ctl,
  dtsvc_t *svc)
{
  DT_F("dtsvc_ipc_wait");
  priv_t *priv = (priv_t *)
    svc->priv;
  dtipc_t *ipc = &priv->ipc;
  DT_Q(dtipc_wait,(ctl, ipc));
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtsvc_ipc_issvc(						/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  unsigned long id,
  void *h,
  int *issvc)
{
  DT_F("dtsvc_ipc_isipc");
  priv_t *priv = (priv_t *)
    svc->priv;
  dtipc_t *ipc = &priv->ipc;
  DT_Q(dtipc_isipc,(ctl, ipc, id, (dtipc_header_t *)h, issvc));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsvc_ipc_getipc(
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  void **vipc)
{
  priv_t *priv = (priv_t *)
    svc->priv;
  *vipc = (void *)&priv->ipc;
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtsvc_ipc_register(						/* register service name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  const char *name,
  unsigned long flags)  
{
  DT_F("dtsvc_ipc_register");
  priv_t *priv = (priv_t *)NULL;
  dtipc_t *ipc;
  dt_rc_e rc;

  DT_Q(dtos_malloc2,(ctl,
    (void **)&priv, 
    sizeof(*priv),
    F, "priv"));
  DT_MEMSET(priv, sizeof(*priv), 0);

  ipc = &priv->ipc;
  
  DT_G(dtipc_register,(ctl, ipc,
    name, 0, flags));
  
  if (rc != DT_RC_GOOD)
  {
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
	return rc;
  }

  svc->type       = DTSVC_TYPE_IPC;
  svc->priv       = (void *)priv;
  svc->unregister = dtsvc_ipc_unregister;
  svc->wait       = dtsvc_ipc_wait;
  svc->issvc      = dtsvc_ipc_issvc;
  svc->selectfd   = ipc->selectfd;

  if (ipc->flags & DTIPC_FLAG_SELF_BACKGROUND)
    svc->flags = DTSVC_FLAG_SELF_BACKGROUND;

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
