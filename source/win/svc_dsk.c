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
DT_RCSID("source/win $RCSfile: svc_dsk.c,v $ $Revision: 1.1 $");

#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/dsk.h>
#include <dtack/os.h>
#include <dtack/str.h>

typedef struct {
  dtlist_t list;
  dtlist_elem_t *elem;
} priv_t;

/*..........................................................................*/
static
dt_rc_e
dtsvc_dsk_unregister(					/* unregister service name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc)
{
  DT_F("dtsvc_dsk_unregister");
  priv_t *priv = (priv_t *)
    svc->priv;
  dtlist_t *list = &priv->list;
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(dtlist_free,(ctl, list));		/* free all list elements 			*/
  DT_I(dtlist_destroy,(ctl, list));		/* free list structure itself 		*/

  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));

  svc->priv = NULL;
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtsvc_dsk_wait(							/* wait for request					*/
  dt_ctl_t *ctl,
  dtsvc_t *svc)
{
  DT_F("dtsvc_dsk_wait");
  priv_t *priv = (priv_t *)
    svc->priv;
  
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "waiting, priv 0x%08lx->elem 0x%08lx->name \"%s\"",
    priv,
    priv? priv->elem: NULL,
    priv && priv->elem? 
      priv->elem->name: "NULL");

  if (priv->elem == NULL)			/* end of input? 					*/
    return DT_RC_STOP;

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtsvc_dsk_issvc(						/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  unsigned long id,
  void *h,
  int *issvc)
{
  priv_t *priv = (priv_t *)
    svc->priv;
  *issvc = (priv->elem != NULL);
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsvc_dsk_next(							/* return next window name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  dtlist_elem_t **elem)
{
  DT_F("dtsvc_dsk_next");
  priv_t *priv = (priv_t *)
    svc->priv;

  *elem = priv->elem;				/* give back current entry name 	*/
  
  if (priv->elem != NULL)			/* we have a current name? 			*/
  {
    DT_Q(dtlist_prev,(ctl,				/* fetch next entry in list 		*/
      &priv->list,					
      priv->elem, &priv->elem));
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "\"%s\" extracted,"
      " there are %smore windows",
      (*elem)->name, 
      priv->elem? "": "no ");
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtsvc_dsk_register(						/* register service name			*/
  dt_ctl_t *ctl,
  dtsvc_t *svc,
  const char *name,
  unsigned long flags)  
{
  DT_F("dtsvc_dsk_register");
  priv_t priv;
  unsigned int count;

  DT_MEMSET(&priv, 0, sizeof(priv));
  
  DT_Q(dtdsk_getlist,(ctl, name,		/* read disk list from file 		*/
    &priv.list));

  DT_Q(dtlist_prev,(ctl,				/* fetch first entry in list 		*/
    &priv.list,					
    DTLIST_ELEM_TAIL, &priv.elem));

  DT_Q(dtlist_count,(ctl,
    &priv.list, &count));

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "disk list has %u elements",
    count);
  
  DT_Q(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &svc->priv, 
    sizeof(priv),
    F, "priv"));
  
  DT_MEMCPY(svc->priv, &priv,		/* copy initialized structure 		*/
    sizeof(priv));

  svc->type       = DTSVC_TYPE_DSK;
  svc->unregister = dtsvc_dsk_unregister;
  svc->wait       = dtsvc_dsk_wait;
  svc->issvc      = dtsvc_dsk_issvc;
  svc->selectfd   = -1;

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
