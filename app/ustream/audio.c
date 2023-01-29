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
| :dtaudio: Function Group - audio device control
|
| SUMMARY
| The ~dtaudio~ object gives a process the ability to record and play audio.
| 
| The device is modeled as a half-duplex model.
| When you instantiate the demise, you must get the direction of data.
| 
| From the process's point of view, the calling sequence 
| 1.	call dtaudio_init() to instantiate the object
| 2.	call dtaudio_write() if playing sounds
| 3.	call dtaudio read() if recording sounds
| 4.	call dtaudio_uninit() to release the object
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: audio.c,v $ $Revision: 1.8 $");

#include <dtaudio.h>
#include <dtack/str.h>
#include <dtack/os.h>
  
static const char *choices[] = {
  "",
  "fake",
  "sbqnx",
  NULL
};

static dtaudio_init_f *methods[] = {
  NULL,
  dtaudio_fake_init,
  dtaudio_sbqnx_init,
  NULL
};

/*..........................................................................
| NAME
| dtaudio_init() - instantiate audio device
|
| SUMMARY
| Initialize the audio device object ~audio~.
| Expect ~spec~ to the of the form #type,info#.
| Have an error if #info# is not one of a predefined set.
| Have an error if no audio device is present.
| Pass ~info~ to the specific implementation routine.
|
| This function does not block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_init(							/* instantiate dtaudio object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  const char *spec,						/* the server spec 					*/
  dtaudio_mode_e mode,					/* the record/play mode 			*/
  long timeout)
{
  DT_F("dtaudio_init");
  char *info;
  int choice;
  dt_rc_e rc;

  memset(audio, 0,						/* clear the structure 				*/
    sizeof(*audio));

  if (spec == NULL)
    spec = "";

  DT_Q(dtos_strdup,(ctl, spec,
    &audio->itype));

  info = strchr(audio->itype, ',');		/* info part after itype	    	*/
  if (info)
    *info++ = '\0';
  else
    info = "";

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "audio->itype=\"%s\" info=\"%s\"",
    audio->itype, info);

  DT_C(dtstr_choice,(ctl, choices,		/* look up audio->itype	    		*/
    audio->itype, &choice));

  if (choice == -1 || choice == 0)		/* unknown type?				    */
  {
    rc = dt_err(ctl, F,
      "unknown audio itype \"%s\"",
      audio->itype);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type?				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous audio itype \"%s\"",
      audio->itype);
	goto cleanup;
  }

  DT_C((methods[choice]),(ctl,			/* give info to implementation		*/
    audio, info, mode, timeout))

  audio->initialized = 1;
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (audio->itype != NULL)
      DT_I(dtos_free2,(ctl,
        audio->itype,
        F, "itype"));
	memset(audio, 0, sizeof(*audio));
  }
  return rc;
}

/*..........................................................................
| NAME
| dtaudio_read() - read audio data from device
|
| SUMMARY
| Read exactly ~length~ bytes to caller's ~buffer~.
| If not enough data is currently available, then block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_read(							/* read audio data					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  void *buffer,							/* where to put data				*/
  long length)							/* desired data length				*/
{
  DT_F("dtaudio_read");
  if (audio->read != NULL)
    DT_Q((*audio->read),(
      ctl, audio, buffer, length)) 
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for audio itype %s",
      audio->itype);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_write() - write audio data to device
|
| SUMMARY
| Write exactly ~length~ bytes from caller's ~buffer~ to device.
| If device is busy, then block.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_write(							/* write audio data					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  void *buffer,							/* source of data					*/
  long length)							/* data length						*/
{
  DT_F("dtaudio_write");
  if (audio->write != NULL)
    DT_Q((*audio->write),(
      ctl, audio, buffer, length))
  else
    return dt_err(ctl, F,
      "function not implemented"
      " for audio itype %s",
      audio->itype);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_set_volume() - set volume level
|
| SUMMARY
| Set volume level ~volume~, clipped to the range 0:100.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_set_volume(						/* set volume level					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  int volume)							/* set volume level 				*/
{
  DT_F("dtaudio_set_volume");

  if (audio->set_volume != NULL)
    DT_Q((*audio->set_volume),(
      ctl, audio, volume));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_get_volume() - get volume level
|
| SUMMARY
| Return the volume level ~volume~ in the range 0:100.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_get_volume(						/* get volume level					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  int *volume)							/* returned volume level 			*/
{
  DT_F("dtaudio_get_volume");
  if (audio->get_volume != NULL)
    DT_Q((*audio->get_volume),(
      ctl, audio, volume))
  else
    *volume = 100;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_uninit() - terminate conversation
|
| SUMMARY
| Terminate audio connection.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtaudio_uninit(							/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio)						/* this dtaudio object 				*/
{
  DT_F("dtaudio_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (audio->initialized)
  {
  	if (audio->uninit != NULL)			/* object has destructor function? 	*/
      DT_Q((*audio->uninit),(
        ctl, audio));
	if (audio->itype != NULL)
	  DT_I(dtos_free2,(ctl,
        audio->itype,
        F, "itype"));
  }
  memset(audio, 0, sizeof(*audio));		/* clear the data structure 		*/
    
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
