/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: fake.c,v $ $Revision: 1.8 $");
#include <dtack/os.h>

#include <dtaudio.h>

typedef struct {
  int dummy;
} priv_t;

#define BYTES_PER_SECOND (8000)

/*..........................................................................
| NAME
| dtaudio_fake_read() - read audio data from device
|
| SUMMARY
| Read exactly ~length~ bytes to caller's ~buffer~.
| If not enough data is currently available, then block.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_fake_read(						/* read audio data					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  void *buffer,							/* where to put data				*/
  long length)							/* desired data length				*/
{
  DT_F("dtaudio_fake_read");
  DT_Q(dtos_delay,(ctl, 
    (1000.0 * length) / 
    BYTES_PER_SECOND));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_fake_write() - write audio data to device
|
| SUMMARY
| Write exactly ~length~ bytes from caller's ~buffer~ to device.
| If device is busy, then block.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_fake_write(					/* write audio data					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  void *buffer,							/* source of data					*/
  long length)							/* data length						*/
{
  DT_F("dtaudio_fake_write");
  DT_Q(dtos_delay,(ctl, 
    (1000.0 * length) / 
    BYTES_PER_SECOND));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_fake_uninit() - terminate conversation
|
| SUMMARY
| Terminate audio connection.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_fake_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio)						/* this dtaudio object 				*/
{
  DT_F("dtaudio_fake_uninit");
  priv_t *priv = (priv_t *)audio->priv;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
	audio->priv = NULL;
  }
  return rc;
}

/*..........................................................................
| NAME
| dtaudio_fake_init() - instantiate fake audio device
|
| SUMMARY
| Use the QNX beta Audio device which adopts /dev/dsp.
| Ignore ~spec~.
| Have an error if no audio device is present.
|
| This function does not block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_fake_init(						/* instantiate fake object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  const char *spec,						/* the server spec 					*/
  dtaudio_mode_e mode,					/* the record/play mode 			*/
  long timeout)							/* time to wait for open 			*/
{
  DT_F("dtaudio_fake_init");
  priv_t *priv = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &audio->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)audio->priv;
  memset(priv, 0, sizeof(*priv));

  audio->read =							/* assign methods 					*/
    dtaudio_fake_read;
  audio->write = 
    dtaudio_fake_write;
  audio->uninit = 
    dtaudio_fake_uninit;

cleanup:
  if (rc != DT_RC_GOOD &&
	  priv != NULL)
  {
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
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
