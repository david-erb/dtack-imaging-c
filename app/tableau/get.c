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

#include <tableau.h>
DT_RCSID("app/tableau $RCSfile: get.c,v $ $Revision: 1.8 $");

#define IPC ((get)->u.ipc.ipc)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_common_free(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_common_free");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (*l != 0 && *data != NULL)			/* we have a given buffer? 			*/
  {
	DT_I(dtos_free2,(ctl, *data,	
      F, "buffer data"));
	*data = NULL;
	*l = 0;
  }

  if (*header != NULL)					/* we have a given header? 			*/
  {
	DT_I(dtos_free2,(ctl, *header,
      F, "header"));
	*header = NULL;
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_ipc_free(
  dtparam_t *param,
  tableau_get_t *get)
{
  DT_F("tableau_get_ipc_free");
  dt_ctl_t *ctl = &param->ctl;
  tableau_ack_t ack;
  long sent;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (!get->partner_stopped)		/* not partner-initiated stop? 		*/
  {
    memset(&ack, 0, sizeof(ack));		/* formulate nack 					*/
    ack.rc = DT_CAST(tableau_len_t,
      DT_RC_STOP);
    DT_I(dtipc_send2,(ctl, IPC,			/* send nack						*/
      (void *)&ack, sizeof(ack), 
      &sent));
  }

  if (!get->isserver)					/* this is a client? 				*/
  {
    DT_I(dtipc_unfind,(ctl, IPC));
	DT_I(dtos_free2,(ctl, IPC,
      F, "ipc"));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_ipc(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_ipc");
  dt_ctl_t *ctl = &param->ctl;
  tableau_ack_t ack;
  long sent;
  dt_rc_e rc;

  DT_C(tableau_get_common_free,(param,	/* free previous buffer			    */
    get, header, data, l));

  DT_C(dtipc_recvt,(ctl, IPC,			/* get header from ipc 				*/
    (void **)header, 
    sizeof(tableau_header_t),
    param->epsilon));
	
  TABLEAU_HEADER_FIX(DT_C, ctl, IPC,	/* fix endianness 					*/
    *header);  

  if ((*header)->l == 0)				/* client is done sending? 			*/
  {
	get->partner_stopped = 1;		/* don't nack this at close 		*/
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "stopping because partner is done");
	goto cleanup;
  }

  DT_C(dtipc_recvt,(ctl, IPC,			/* get data from ipc 				*/
    data, (*header)->l,
    param->epsilon));
  *l = (*header)->l;					/* remember how big buffer is 		*/

  memset(&ack, 0, sizeof(ack));			/* formulate ack 					*/
  ack.l = (*header)->l;
  ack.n = (*header)->n;
  ack.rc = DT_CAST(tableau_len_t,
    DT_RC_GOOD);
  DT_C(dtipc_send2,(ctl, IPC,			/* send ack							*/
    (void *)&ack, sizeof(ack),
    &sent));
  if (sent < (int)sizeof(ack))			/* client closed socket? 			*/
  {
	get->partner_stopped = 1;		/* don't nack this at close 		*/
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "stopping because partner closed socket");
	goto cleanup;
  }

cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
{
  DT_F("tableau_get_setup");
  dt_ctl_t *ctl = &param->ctl;
  
  if (spec == NULL ||
	  strlen(spec) == 0)
    return dt_err(ctl, F,
      "NULL or empty input name");

  DT_Q(dtos_time,(ctl, &get->start));	/* remember starting time 			*/

#define IS(TYPE) (DT_ABBREV(spec, TYPE))
  
										/* ................................ */
  if (IS("socket") ||
	  IS("rocket") ||
	  IS("qnx4"))
  {
	DT_Q(dtos_malloc2,(ctl,				/* space for ipc structure 			*/
      (void **)&IPC, sizeof(*IPC),
      F, "ipc"));
    DT_Q(tableau_ipc_setup,(param,
      spec, "put", IPC));				/* tell server to put so we can get	*/
	get->function = tableau_get_ipc;
	get->free = tableau_get_ipc_free;
    get->type = TABLEAU_GET_IPC;
  }
  else									/* ................................ */
  if (IS("cortex") ||
	  IS("vmc") ||
	  IS("psi") ||
	  IS("vbmci") ||
	  IS("vfw") ||
	  IS("meteor") ||
	  IS("bt") ||
	  IS("dir") ||
	  IS("gen"))
  {
    DT_Q(tableau_get_fb_setup,(param,	/* fb has its own setup function 	*/
      get, spec));
    get->type = TABLEAU_GET_FB;
  }
  else									/* ................................ */
  if (IS("roll"))
  {
    DT_Q(tableau_get_roll_setup,(param,	/* roll has its own setup function 	*/
      get, spec));
    get->type = TABLEAU_GET_ROLL;
  }
  else									/* ................................ */
  if (IS("ser"))
  {
    DT_Q(tableau_get_ser_setup,(param,	/* ser has its setup function 		*/
      get, spec));
  }
  else									/* ................................ */
  if (IS("file"))
  {
	char *name = strchr(spec, ',');
	if (name == NULL)
	  return dt_err(ctl, F,
        "no name given");
	name++;								/* skip the comma 					*/
    DT_Q(tableau_get_file_setup,(param,	/* file has its own setup function 	*/
      get, name));
    get->type = TABLEAU_GET_FILE;
  }
  else									/* ................................ */
  {
	return dt_err(ctl, F,
	  "invalid input spec \"%s\"", 
      spec);
  }

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
