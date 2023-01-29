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
| dtostream_audio() - input stream on audio
|
| SUMMARY
| Not documented yet.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSaudio: oaudio.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtostream.h>
#include <dtaudio.h>
#include <dtack/os.h>

typedef struct {
  dtaudio_t audio;
} priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtostream_audio_send(					/* put outgoing data packet			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream,					/* this dtostream object 			*/
  void *packet,
  long packet_length)
{
  DT_F("dtostream_audio_send");
  priv_t *priv = (priv_t *)ostream->priv;

  DT_Q(dtaudio_write,(ctl,
    &priv->audio, 
    packet, packet_length));
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtostream_audio_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream)					/* this dtostream object 			*/
{
  DT_F("dtostream_audio_uninit");
  priv_t *priv = (priv_t *)ostream->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
	DT_I(dtaudio_uninit,(ctl,			/* release the audio device 		*/
      &priv->audio));
	DT_I(dtos_free2,(ctl, priv,			/* free private data structure 		*/
      F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtostream_audio_init(					/* make ostream on audio				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtostream_t *ostream,					/* this dtostream object 			*/
  const char *spec,
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted server ID				*/
  long timeout)
{
  DT_F("dtostream_audio_init");
  priv_t *priv = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &ostream->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)ostream->priv;
  memset(priv, 0, sizeof(*priv));

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "spec=\"%s\"", spec);

  DT_C(dtaudio_init,(ctl,				/* access the audio device 			*/
    &priv->audio, spec,
    DTAUDIO_MODE_WRITE,
    timeout));

  ostream->send =						/* assign methods 					*/
    dtostream_audio_send;
  ostream->uninit = 
    dtostream_audio_uninit;

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
