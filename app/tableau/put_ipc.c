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
DT_RCSID("app/tableau $RCSfile: put_ipc.c,v $ $Revision: 1.8 $");

#define IPC ((put)->u.ipc.ipc)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_ipc(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
{
  DT_F("tableau_put_ipc");
  dt_ctl_t *ctl = &param->ctl;
  long sent;
  int ack_pending;
  dt_rc_e rc = DT_RC_GOOD;

  if (data == NULL)						/* supposed to clean up? 			*/
  {
	if (!put->partner_stopped &&	/* not partner-initiated stop? 		*/
		IPC != NULL)
	{
	  if (IPC->type != DTIPC_TYPE_NONE)	/* ipc was initialized? 			*/
	  {
		tableau_header_t h;
		memset(&h, 0, sizeof(h));
        DT_G(dtipc_send2,(ctl, IPC,		/* tell partner game over 			*/
          &h, sizeof(h), &sent));
	  }
	}
	if (!put->isserver &&				/* this is a client? 				*/
		IPC != NULL)
	{
	  if (IPC->type != DTIPC_TYPE_NONE)	/* ipc was initialized? 			*/
	  {
		DT_I(dtos_delay,(ctl, 2000));	/* let server digest end 			*/
  	    DT_I(dtipc_unfind,(ctl, IPC));
	  }
	  DT_I(dtos_free2,(ctl, IPC,
        F, "ipc"));
	}
	return rc;
  }

  DT_Q(dtipc_send2,(ctl, IPC,			/* put header to ipc 				*/
    (void *)header, 
    sizeof(tableau_header_t),
    &sent));
  if (sent < (int)sizeof(tableau_header_t))
  {
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "quitting because partner closed socket");
	put->partner_stopped = 1;		/* partner-initiated stop 			*/
	return DT_RC_GOOD;
  }
	
  DT_Q(dtipc_send2,(ctl, IPC,			/* put data to ipc 					*/
    data, header->l, &sent));
  if (sent < header->l)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "quitting because partner closed socket");
	put->partner_stopped = 1;		/* partner-initiated stop 			*/
	return DT_RC_GOOD;
  }
  
  put->u.ipc.sent += header->l;			/* count data bytes sent 			*/

# ifdef NONO
	dt_dbg(ctl, F, DT_DBG_MASK_USR2,
      "total acked=%ld,"
      " total sent=%ld, sponge=%ld",
      put->u.ipc.acked,
      put->u.ipc.sent, TABLEAU_SPONGE);
# endif

  DT_Q(dtipc_poll,(ctl,					/* check for incoming ack 			*/
    IPC, DTIPC_POLL_READ, 0,
    "ack", &ack_pending));

  while (ack_pending ||					/* while some ack is ready 			*/
    put->u.ipc.sent -					/* or while sponge is too full 		*/
    put->u.ipc.acked >	
	TABLEAU_SPONGE)
  {
	tableau_ack_t *ack;

    DT_Q(dtipc_recvt,(ctl, IPC,			/* get back an ack					*/
      (void **)&ack, 
      sizeof(tableau_ack_t),
      param->epsilon));
	
	ack->l = DTIPC_FIX(DTC, CTL, IPC, 
      ack->l);
#   ifdef NONO
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "new acked=%ld, total acked=%ld,"
        " total sent=%ld, sponge=%ld",
        ack->l, put->u.ipc.acked,
        put->u.ipc.sent, TABLEAU_SPONGE);
#   endif

	put->u.ipc.acked += ack->l;			/* sum bytes acked 					*/
	rc = DT_CAST(dt_rc_e, ack->rc);		/* examine ack's return code 		*/
	DT_Q(dtos_free2,(ctl, ack,
      F, "ack"));

    if (rc != DT_RC_GOOD)				/* oops this was a nack? 			*/
	{
	  put->partner_stopped = 1;	/* partner-initiated stop 			*/
	  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
        "quitting on nack"
        " from partner");
	  return DT_RC_GOOD;
	}

    DT_Q(dtipc_poll,(ctl,				/* check for incoming ack 			*/
      IPC, DTIPC_POLL_READ, 0,
      "ack", &ack_pending));
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
