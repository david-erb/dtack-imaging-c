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
| dtistream_ipc() - input stream on ipc
|
| SUMMARY
| Not documented yet.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: iipc.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtistream.h>
#include <dtack/ipc.h>
#include <dtack/str.h>
#include <dtack/os.h>

typedef struct {
  dtipc_t ipc;
} priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_ipc_is_available(				/* check for client data pending 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  int *is_available)
{
  DT_F("dtistream_ipc_is_available");
  priv_t *priv = (priv_t *)istream->priv;
  
  DT_Q(dtipc_poll,(ctl,					/* check for input 					*/
    &priv->ipc,
    DTIPC_POLL_READ,
    0, "data packet from client",
    is_available));
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_ipc_recv(						/* get incoming request 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  void **packet,
  long *packet_length)
{
  DT_F("dtistream_ipc_recv");
  priv_t *priv = (priv_t *)istream->priv;
  *packet = NULL;
  *packet_length = 0;
  DT_Q(dtipc_recv2,(ctl,				/* wait for and get data packet		*/
    &priv->ipc,
    packet, packet_length));
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_ipc_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream)					/* this dtistream object 			*/
{
  DT_F("dtistream_ipc_uninit");
  priv_t *priv = (priv_t *)istream->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
#ifdef NONO
	dtstream_ack_t ack;
	memset(&ack, 0, sizeof(ack));
	DT_I(dtipc_send,(ctl,				/* send for ack						*/
      &priv->ipc,
      &ack, sizeof(ack)));
#endif
	DT_I(dtipc_done,(ctl, 
      &priv->ipc));
	DT_I(dtipc_unregister,(ctl, 
      &priv->ipc));
	if (priv != NULL)
      DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_ipc_init(						/* make istream on ipc				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  const char *spec,						/* our spec 						*/
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted client ID				*/
  long timeout)							/* timeout waiting for client 		*/
{
  DT_F("dtistream_ipc_init");
  priv_t *priv = NULL;
  int ipc_initialized = 0;
  int is_ready;
  dtstream_id_t *client = NULL;
  dtos_time_t start;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,
    &istream->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)istream->priv;
  memset(priv, 0, sizeof(*priv));

  DT_C(dtipc_register,(ctl,				/* advertise name					*/
    &priv->ipc, spec, 0, 0));
  ipc_initialized = 1;

  DT_C(dtos_time,(ctl, &start));
  while (1)
  {
	dtos_time_t now;
	long timeused;
	DT_C(dtos_time,(ctl, &now));		/* current moment 					*/
	timeused = (now.wall-start.wall) *	/* time we waited so far 			*/
      1000.0;
	DT_C(dtipc_poll,(ctl,				/* wait for a client to connect		*/
      &priv->ipc,
      DTIPC_POLL_ACCEPT,
      DT_MAX(0, timeout - timeused),
      "connection from client",
      &is_ready));

	if (!is_ready)
    {
	  rc = dt_err(ctl, F,
        "<%s> on spec %s"
        " did not connect within %ld ms",
        DTSTREAM_ID_GET_STRING(permit),
        spec, timeout);
	  goto cleanup;
    }

	DT_C(dtipc_wait,(ctl,				/* accept the connection 			*/
      &priv->ipc));

    DT_C(dtipc_recv,(ctl,				/* get client's ID					*/
      &priv->ipc,
      (void **)&client, 
      sizeof(*client)));

    if (DTSTREAM_ID_CMP(permit,			/* client not permitted? 			*/
           client))
	{
      dt_dbg(ctl, F, DT_DBG_MASK_USR1,
        "nacking client <%s>"
        " not permitted under <%s>",
        DTSTREAM_ID_GET_STRING(client),
        DTSTREAM_ID_GET_STRING(permit));
	  DTSTREAM_ID_SET_NACK(id);			/* give that client a nack 			*/
	  DT_G(dtipc_send,(ctl, &priv->ipc,	/* ignore rc from nack 				*/
        id, sizeof(*id)));
	  DT_C(dtos_free2,(ctl, client,
        F, "received client id"));
	  client = NULL;
	  DT_C(dtipc_done,(ctl,				/* finish this conversation 		*/
        &priv->ipc));
	}
	else
	{
	  DTSTREAM_ID_SET_ACK(id);			/* give client an ack 				*/
	  DT_C(dtipc_send,(ctl, &priv->ipc,
        id, sizeof(*id)));
	  break;
	}
  }
  
  istream->is_available =				/* assign methods 					*/
    dtistream_ipc_is_available;
  istream->recv = 
    dtistream_ipc_recv;
  istream->uninit = 
    dtistream_ipc_uninit;

cleanup:
  if (client != NULL)
	DT_I(dtos_free2,(ctl, client,
      F, "received client id"));
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
	  if (ipc_initialized)
	    DT_I(dtipc_unregister,(ctl,
          &priv->ipc));
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
