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
| dtistream_audio() - input stream on audio
|
| SUMMARY
| Not documented yet.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSaudio: iaudio.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtistream.h>
#include <dtaudio.h>
#include <dtack/os.h>

typedef struct {
  dtaudio_t audio;
  long packet_size;
  long samples_per_second;
} priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_audio_is_available(			/* check for client data pending 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  int *is_available)
{
  *is_available = 1;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_audio_recv(					/* get incoming data packet			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  void **packet,
  long *packet_length)
{
  DT_F("dtistream_audio_recv");
  priv_t *priv = (priv_t *)istream->priv;
  dt_rc_e rc = DT_RC_GOOD;

  *packet = NULL;
  DT_C(dtos_malloc2,(ctl,				/* get space for the packet 		*/
    packet, priv->packet_size,
    F, "audio packet"));

  DT_C(dtaudio_read,(ctl,				/* read from the audio device */
    &priv->audio,
    *packet, priv->packet_size));
  *packet_length = priv->packet_size;
  
cleanup:
  if (rc != DT_RC_GOOD &&
	  *packet != NULL)
    DT_I(dtos_free2,(ctl, *packet,
      F, "audio packet"));

  return rc;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_audio_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream)					/* this dtistream object 			*/
{
  DT_F("dtistream_audio_uninit");
  priv_t *priv = (priv_t *)istream->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
	DT_I(dtaudio_uninit,(ctl,			/* release the audio device 		*/
      &priv->audio));
    DT_I(dtos_free2,(ctl, priv,			/* free private data structure		*/
      F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_audio_init(					/* make istream on audio				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  const char *spec,
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted server ID				*/
  long timeout)
{
  DT_F("dtistream_audio_init");
  priv_t *priv = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &istream->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)istream->priv;
  memset(priv, 0, sizeof(*priv));

  DT_C(dtaudio_init,(ctl,				/* access the audio device 			*/
    &priv->audio, spec,
    DTAUDIO_MODE_READ, 
    timeout));

  priv->packet_size = 500;

  istream->is_available =				/* assign methods 					*/
    dtistream_audio_is_available;
  istream->recv = 
    dtistream_audio_recv;
  istream->uninit = 
    dtistream_audio_uninit;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
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
