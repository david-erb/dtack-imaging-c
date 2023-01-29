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
DT_RCSID("app/ustream $RCSfile: sbqnx.c,v $ $Revision: 1.8 $");
#include <dtack/os.h>

#include <dtaudio.h>

#define DESC "QNX audio"

#ifdef DTCONFIG_IMPLIED_QNX432

#ifndef DTDEPEND
#  include <signal.h>
#  include <sys/audio.h>
#  include <fcntl.h>
#endif

typedef struct {
  dtfd_t fd;
  char *spec;
  char *filename;
  dtaudio_mode_e mode;
  int volume;
} priv_t;

#define BASE 0x220
#define DEFAULT_FILENAME "/dev/dsp"
#define OUTP(PORT, VALUE) DT_Q(dtaudio_sbqnx_out,(ctl, PORT, VALUE));

/*..........................................................................*
 *..........................................................................*/

static
void
new_alarm_func(
  int signum)
{
}

/*..........................................................................*
 *..........................................................................*/

static
dt_rc_e 
dtaudio_sbqnx_out(
  dt_ctl_t *ctl,
  int port,
  int value)				  
{
  DT_F("dtaudio_sbqnx_out");
  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
    "out(%03x, %02x)",
    port, (unsigned int)(value & 0xff));
  outp(port, value);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_sbqnx_read() - read audio data from device
|
| SUMMARY
| Read exactly ~length~ bytes to caller's ~buffer~.
| If not enough data is currently available, then block.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_sbqnx_read(						/* read audio data					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  void *buffer,							/* where to put data				*/
  long length)							/* desired data length				*/
{
  DT_F("dtaudio_sbqnx_read");
  priv_t *priv = (priv_t *)audio->priv;
  DT_Q(dtfd_read_all,(ctl, &priv->fd,
    buffer, length));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_sbqnx_write() - write audio data to device
|
| SUMMARY
| Write exactly ~length~ bytes from caller's ~buffer~ to device.
| If device is busy, then block.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_sbqnx_write(					/* write audio data					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  void *buffer,							/* source of data					*/
  long length)							/* data length						*/
{
  DT_F("dtaudio_sbqnx_write");
  priv_t *priv = (priv_t *)audio->priv;
  DT_Q(dtfd_write_all,(ctl, &priv->fd,
    buffer, length));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_sbqnx_set_volume() - set volume level
|
| SUMMARY
| Set volume level ~volume~, clipped to the range 0:100.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_sbqnx_set_volume(				/* set volume level					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  int volume)							/* set volume level 				*/
{
  DT_F("dtaudio_sbqnx_set_volume");
  priv_t *priv = (priv_t *)audio->priv;
  int level;

  level = volume * 15 / 100;
  level = DT_MAX(level, 0);
  level = DT_MIN(level, 15);
  
  if (priv->mode == DTAUDIO_MODE_WRITE)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "0x22 level 0x%02x",
      (level << 4) | level);

    OUTP(BASE+4, 0x22);					/* select master OUTPut 			*/
    OUTP(BASE+5, 
      (level << 4) | level);
  }
  else
  if (priv->mode == DTAUDIO_MODE_READ)
  {
    OUTP(BASE+4, 0x2e);					/* select line input level 			*/
    OUTP(BASE+5, 
      (level << 4) | level);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_sbqnx_get_volume() - get volume level
|
| SUMMARY
| Return the volume level ~volume~ in the range 0:100.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_sbqnx_get_volume(				/* set volume level					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  int *volume)							/* set volume level 				*/
{
  DT_F("dtaudio_sbqnx_get_volume");
  priv_t *priv = (priv_t *)audio->priv;
  int level;
  
  if (priv->mode == DTAUDIO_MODE_WRITE)
  {
    OUTP(BASE+4, 0x22);					/* select master OUTPut 			*/
    level = inp(BASE+5);
  }
  else
  if (priv->mode == DTAUDIO_MODE_READ)
  {
    OUTP(BASE+4, 0x2e);					/* select line input level 			*/
    level = inp(BASE+5);
  }

  level = (level * 100) / 15;
  level = DT_MAX(level, 0);
  level = DT_MIN(level, 100);
  
  *volume = level;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtaudio_sbqnx_uninit() - terminate conversation
|
| SUMMARY
| Terminate audio connection.
| 
| END
 *..........................................................................*/

static
dt_rc_e
dtaudio_sbqnx_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio)						/* this dtaudio object 				*/
{
  DT_F("dtaudio_sbqnx_uninit");
  priv_t *priv = (priv_t *)audio->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
	if (priv->fd.fd != -1)
	  DT_I(dtfd_close,(ctl, 
        &priv->fd));
	if (priv->spec != NULL)
      DT_I(dtos_free2,(ctl, priv->spec,
        F, "spec"));
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
	audio->priv = NULL;
  }
  return rc;
}

#endif

/*..........................................................................
| NAME
| dtaudio_sbqnx_init() - instantiate sbqnx audio device
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
dtaudio_sbqnx_init(						/* instantiate sbqnx object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtaudio_t *audio,						/* this dtaudio object 				*/
  const char *spec,						/* the server spec 					*/
  dtaudio_mode_e mode,					/* the record/play mode 			*/
  long timeout)							/* time to wait for open 			*/
{
  DT_F("dtaudio_sbqnx_init");
#ifdef DTCONFIG_IMPLIED_QNX432
  priv_t *priv = NULL;
  int ntokens;
  char *p;
  long longval;
  int flags;
  int isreadable;
  int old_alarm_time;
  void (*old_alarm_func)(int);
  int alarm_set = 0;
  int fd;
  int sysrc;
  unsigned long i;
  dt_rc_e rc;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL audio spec");
    goto cleanup;
  }
  else
  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "zero-length audio spec");
    goto cleanup;
  }

  if (mode == DTAUDIO_MODE_WRITE)		/* check the mode 					*/
    flags = O_WRONLY;
  else
  if (mode == DTAUDIO_MODE_READ)
    flags = O_RDONLY;
  else
  {
    rc = dt_err(ctl, F,
      "invalid mode %d", mode);
	goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &audio->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)audio->priv;
  memset(priv, 0, sizeof(*priv));

  priv->fd.fd = -1;
  priv->mode = mode;


										/* ................................ */
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "spec=\"%s\"", spec);

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec, ',', &ntokens));

  p = priv->spec;

  if (ntokens >= 1)
  {
    priv->filename = p;
	p += strlen(p) + 1;
  }

  if (priv->filename == NULL)
    priv->filename = DEFAULT_FILENAME;

  if (ntokens >= 2)						/* volume level						*/
  {
    DT_C(dtstr_to_long,(ctl,
      p, &longval));
	priv->volume = longval;
	p += strlen(p) + 1;
  }
  else
    priv->volume = -1;
										/* ................................ */
  
  DT_C(dtos_isreadable,(ctl,			/* make sure audio device exists 	*/
    priv->filename,
    &isreadable));
  if (!isreadable)
  {
	rc = dt_err(ctl, F,
      "%s does not exist", 
      priv->filename);
	goto cleanup;
  }
  
										/* ................................ */

#ifdef NONO
  setvbuf(priv->file, 0,				/* increase buffer size 			*/
    _IOFBF, 8192);
#endif
  
  old_alarm_time = alarm(0);			/* save old alarm time				*/
  old_alarm_func =						/* set new handler, save old one	*/
    signal(SIGALRM, new_alarm_func);
  alarm_set = 1;
  alarm(2);								/* set new alarm time				*/

  fd = open(priv->filename, flags);		/* try to open the file 			*/
  if (fd == -1)
  {
    rc = dt_err(ctl, F,
      "errno %d (%s) in open(%s, %d)",
      errno, strerror(errno),
      priv->filename, flags);
	goto cleanup;
  }

  i = 8000;
  sysrc = ioctl(fd, SOUND_PCM_WRITE_RATE, &i);
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "errno %d (%s) in ioctl(%d,"
      " SOUND_PCM_WRITE_RATE, %d)",
      errno, strerror(errno),
      fd, i);
	goto cleanup;
  }
  i = 1;
  sysrc = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &i);
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "errno %d (%s) in ioctl(%d,"
      " SOUND_PCM_WRITE_CHANNELS, %d)",
      errno, strerror(errno),
      fd, i);
	goto cleanup;
  }
  i = 8;
  sysrc = ioctl(fd, SOUND_PCM_WRITE_BITS, &i);
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "errno %d (%s) in"
      " ioctl(%d, SOUND_PCM_WRITE_BITS, %d)",
      errno, strerror(errno),
      fd, i);
	goto cleanup;
  }

  alarm(0);								/* cancel our timer					*/
  signal(SIGALRM, old_alarm_func);		/* replace old handler				*/
  alarm(old_alarm_time);				/* start old clock ticking			*/
  alarm_set = 0;

  if (mode == DTAUDIO_MODE_WRITE)
  {
    OUTP(BASE+4, 0x22);					/* select master OUTPut 			*/
    OUTP(BASE+5, 0xff);
    OUTP(BASE+4, 0x04);					/* select dsp OUTPut level 			*/
    OUTP(BASE+5, 0xff);

										/* the next lines are for Reveal	*/
    OUTP(BASE+4, 0x2e);					/* select line input level 			*/
    OUTP(BASE+5, 0x00);
    OUTP(BASE+4, 0x0a);					/* select mic input level 			*/
    OUTP(BASE+5, 0x00);

    OUTP(BASE+4, 0x0c);					/* select line as recording input	*/
    OUTP(BASE+5, 0x06);					/* needed for Crystal chipset 		*/
  }
  else
  if (mode == DTAUDIO_MODE_READ)
  {
    OUTP(BASE+4, 0x2e);					/* select line input level 			*/
    OUTP(BASE+5, 0xcc);
    OUTP(BASE+4, 0x0a);					/* select mic input level 			*/
    OUTP(BASE+5, 0xff);
    OUTP(BASE+4, 0x3c);					/* select OUTPut control			*/
    OUTP(BASE+5, 0x00);					/* all sources off for no feedback	*/
										/* the next lines are for Reveal	*/
    OUTP(BASE+4, 0x22);					/* select master OUTPut				*/
    OUTP(BASE+5, 0x00);					/* volume 0 for no feedback			*/
  }

  DT_C(dtfd_fd,(ctl, &priv->fd,			/* make a dtfd out of it 			*/
    priv->filename, fd));

  audio->read =							/* assign methods 					*/
    dtaudio_sbqnx_read;
  audio->write = 
    dtaudio_sbqnx_write;
  audio->uninit = 
    dtaudio_sbqnx_uninit;
  audio->set_volume = 
    dtaudio_sbqnx_set_volume;
  audio->get_volume = 
    dtaudio_sbqnx_get_volume;

  if (priv->volume != -1)
    DT_C(dtaudio_sbqnx_set_volume,(ctl,	/* set initial volume 				*/
      audio, priv->volume));
  
cleanup:
  if (alarm_set)
  {
	alarm(0);							/* cancel our timer					*/
	signal(SIGALRM, old_alarm_func);	/* replace old handler				*/
	alarm(old_alarm_time);				/* start old clock ticking			*/
  }
  if (rc != DT_RC_GOOD &&
	  priv != NULL)
  {
	if (priv->fd.fd != -1)
	  DT_I(dtfd_close,(ctl, 
        &priv->fd))
	else
	if (fd != -1)
	  close(fd);

	if (priv->spec != NULL)
      DT_I(dtos_free2,(ctl, priv->spec,
        F, "spec"));

	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }
  return rc;
#else
  return dt_err_notlinked(ctl, F, DESC);
#endif
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
