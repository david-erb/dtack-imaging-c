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
| dtostream_ipc() - input stream on file
|
| SUMMARY
| Not documented yet.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: oipc.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtostream.h>
#include <dtack/ipc.h>
#include <dtack/os.h>

typedef struct {
  dtipc_t ipc;
} priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtostream_ipc_send(						/* put outgoing data packet			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream,					/* this dtostream object 			*/
  void *packet,
  long packet_length)
{
  DT_F("dtostream_ipc_send");
  priv_t *priv = (priv_t *)ostream->priv;

  DT_Q(dtipc_send,(ctl,					/* send data on ipc 				*/
    &priv->ipc,
    packet, packet_length));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtostream_ipc_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream)					/* this dtostream object 			*/
{
  DT_F("dtostream_ipc_uninit");
  priv_t *priv = (priv_t *)ostream->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
#ifdef NONO
	dtstream_ack_t *ack;
	DT_I(dtipc_recv,(ctl,				/* wait for ack						*/
      &priv->ipc,
      (void **)&ack, sizeof(*ack)));
	DT_I(dtos_free2,(ctl, ack,
      F, "ack"));
#endif
	DT_I(dtipc_unfind,(ctl,				/* release ipc connection 			*/
      &priv->ipc));
	if (priv != NULL)
      DT_I(dtos_free2,(ctl, priv,		/* free private data structure 		*/
        F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtostream_ipc_init(						/* make ostream on ipc				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream,					/* this dtostream object 			*/
  const char *spec,						/* our spec 						*/
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted client ID				*/
  long timeout)							/* timeout til server answers 		*/
{
  DT_F("dtostream_ipc_init");
  priv_t *priv = NULL;
  int ipc_initialized = 0;
  dtstream_id_t *server = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &ostream->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)ostream->priv;
  memset(priv, 0, sizeof(*priv));

  DT_C(dtipc_find,(ctl,					/* contact the server 				*/
    &priv->ipc, spec, 0, 0, timeout));
  ipc_initialized = 1;
  
  DTSTREAM_ID_SET_ACK(id);				/* give server our ID				*/
  DT_C(dtipc_send,(ctl, &priv->ipc,
    id, sizeof(*id)));

  DT_C(dtipc_recv,(ctl,					/* get server's ID					*/
    &priv->ipc,
    (void **)&server, 
    sizeof(*server)));

  if (!DTSTREAM_ID_IS_ACK(server))		/* server is nacking us? 			*/
  {
	rc = dt_err(ctl, F,
      "server <%s> nacked",
      DTSTREAM_ID_GET_STRING(server));
	goto cleanup;
  }

  if (DTSTREAM_ID_CMP(permit, server))	/* server not permitted? 			*/
  {
	rc = dt_err(ctl, F,
      "server <%s> not permitted"
      " under <%s>",
      DTSTREAM_ID_GET_STRING(server),
      DTSTREAM_ID_GET_STRING(permit));
	goto cleanup;
  }


  ostream->send =						/* assign methods 					*/
    dtostream_ipc_send;
  ostream->uninit = 
    dtostream_ipc_uninit;

cleanup:
  if (server != NULL)
	DT_I(dtos_free2,(ctl, server,
      F, "received server id"));
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
	  if (ipc_initialized)
	  {
	    DT_I(dtipc_unfind,(ctl,
          &priv->ipc));
	  }
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
	}
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
