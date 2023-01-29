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
DT_RCSID("app/ustream $RCSfile: shovel.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtistream.h>
#include <dtostream.h>

#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
ustream_shovel(
  ustream_t *ustream,
  dtdg_server_t *dgserver)
{
  DT_F("ustream_shovel");
  dt_ctl_t *ctl = ustream->ctl;
  int is_available;
  dtistream_t _istream, *istream = &_istream;
  dtostream_t _ostream, *ostream = &_ostream;
  dtstream_id_t _id, *id = &_id;
  dtstream_id_t _permit, *permit = &_permit;
  void *packet = NULL;
  long packet_length;
  long total = 0;
  dt_rc_e rc;

  istream->initialized = 0;
  ostream->initialized = 0;
  
  DT_C(dtos_time,(ctl, 
    &ustream->time));

  ustream_state(ustream,
    DTDG_STATE_CONNECTING 
    " output stream %s",
    ustream->ostream_spec);

  DTSTREAM_ID_SET_STRING(id, 
    ustream->ident);
  DTSTREAM_ID_SET_STRING(permit, 
    ustream->permit);

  DT_C(dtostream_init,(ctl,				/* instantiate output stream 		*/
    ostream, 
    ustream->ostream_spec,
    id, permit,
    ustream->connect_timeout));

  ustream_state(ustream,
    DTDG_STATE_CONNECTING 
    " input stream %s",
    ustream->istream_spec);

  DT_C(dtistream_init,(ctl,				/* instantiate input stream 		*/
    istream,  
    ustream->istream_spec,
    id, permit,
    ustream->accept_timeout));

  while(1)
  {
	DT_C(dtistream_is_available,(ctl,	/* check if stream data available 	*/
      istream, 
      &is_available));

	if (is_available != 0)				/* stream data is available? 		*/
	{
	  DT_C(ustream_server_check,(		/* handle datagram commands			*/
        ustream, dgserver));
	  if (ustream->flag_shovel != 1)	/* supposed to cease shoveling? 	*/
	    break;

	  ustream_state(ustream,
        DTDG_STATE_SHOVELING
        ", reading input %s,"
        " (%ld bytes)",
        ustream->istream_spec, total);

	  DT_C(dtistream_recv,(ctl,			/* get the stream data 				*/
        istream,
        &packet, &packet_length));
	  if (packet_length == 0)			/* input stream is depleted? 		*/
	    break;

	  ustream_state(ustream,
        DTDG_STATE_SHOVELING
        ", writing output %s,"
        " (%ld bytes)",
        ustream->ostream_spec, total);

	  DT_C(ustream_server_check,(		/* handle datagram commands			*/
        ustream, dgserver));
	  if (ustream->flag_shovel != 1)	/* supposed to cease shoveling? 	*/
	    break;

	  DT_C(dtostream_send,(ctl,			/* send stream data to sink 		*/
        ostream,
        packet, packet_length));
	  total += packet_length;

	  DT_C(dtos_free2,(ctl, packet,		/* free the stream packet			*/
        F, "received data"));
	  packet = NULL;
	}
	else								/* no stream data available? 		*/
	{
	  ustream_state(ustream,
        DTDG_STATE_SHOVELING
        ", waiting for input %s,"
        " (%ld bytes)",
        ustream->istream_spec, total);

	  DT_C(ustream_server_check,(		/* handle datagram commands			*/
        ustream, dgserver));
	  if (ustream->flag_shovel != 1)	/* supposed to cease shoveling? 	*/
	    break;

      DT_C(dtos_delay,(ctl, 
        ustream->poll_period));
	}
  }

cleanup:
  if (total != 0)
    dt_dbg(ctl, F, DT_DBG_MASK_USR2,
      "shoveled %ld bytes, %s",
      total,
      ustream->flag_shovel == 2?
        "freshening": "idling");

  if (packet != NULL)
	DT_I(dtos_free2,(ctl, packet,		/* free the datagram command 		*/
      F, "received data"));

  ustream_state(ustream,
    DTDG_STATE_DISCONNECTING
    " output stream %s",
    ustream->ostream_spec);

  DT_I(dtostream_uninit,(ctl,			/* disconnect output stream			*/
    ostream));
  
  ustream_state(ustream,
    DTDG_STATE_DISCONNECTING
     " input stream %s",
    ustream->istream_spec);

  DT_I(dtistream_uninit,(ctl,			/* disconnect input stream 			*/
    istream));

  DT_I(dtos_time,(ctl,					/* grab this state time 			*/
    &ustream->time));

  if (rc == DT_RC_STOP)					/* server has been told to stop? 	*/
    ustream_state(ustream,
      DTDG_STATE_DISCONNECTING);
  else
  if (rc == DT_RC_SHUTDOWN)				/* computer supposed to shut down? 	*/
    ustream_state(ustream,
      DTDG_STATE_SHUTTING_DOWN);
  else
  {
    ustream_state(ustream,
      DTDG_STATE_WAITING);
    rc = DT_RC_GOOD;					/* ignore errors here 				*/
  }

  if (ustream->flag_shovel == 2)		/* supposed to freshen connection? 	*/
    ustream->flag_shovel = 1;			/* go right back into shoveling 	*/
  else
    ustream->flag_shovel = 0;			/* cease shoveling 					*/
  
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
