/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DT_DBG_MASK_CONFIG (0x02000000L) /* config							*/

typedef enum {
  DTAUDIO_MODE_READ = 1111,
  DTAUDIO_MODE_WRITE = 2222
} dtaudio_mode_e;

#define DTAUDIO_INIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtaudio_t *audio, \
  const char *spec, \
  dtaudio_mode_e mode, \
  long timeout
#define DTAUDIO_UNINIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtaudio_t *audio
#define DTAUDIO_READ_ARGS \
  dt_ctl_t *ctl, \
  struct dtaudio_t *audio, \
  void *buffer, \
  long length
#define DTAUDIO_WRITE_ARGS \
  dt_ctl_t *ctl, \
  struct dtaudio_t *audio, \
  void *buffer, \
  long length
#define DTAUDIO_SET_VOLUME_ARGS \
  dt_ctl_t *ctl, \
  struct dtaudio_t *audio, \
  int volume
#define DTAUDIO_GET_VOLUME_ARGS \
  dt_ctl_t *ctl, \
  struct dtaudio_t *audio, \
  int *volume

typedef struct dtaudio_t {
  void *priv;
  char *itype;
  dt_rc_e (*uninit)(DTAUDIO_UNINIT_ARGS);
  dt_rc_e (*read)(DTAUDIO_READ_ARGS);
  dt_rc_e (*write)(DTAUDIO_WRITE_ARGS);
  dt_rc_e (*set_volume)(DTAUDIO_SET_VOLUME_ARGS);
  dt_rc_e (*get_volume)(DTAUDIO_GET_VOLUME_ARGS);
  int initialized;
} dtaudio_t;

typedef dt_rc_e DTCONFIG_API1 dtaudio_init_f(DTAUDIO_INIT_ARGS);
typedef dt_rc_e dtaudio_uninit_f(DTAUDIO_UNINIT_ARGS);
typedef dt_rc_e dtaudio_read_f(DTAUDIO_READ_ARGS);
typedef dt_rc_e dtaudio_write_f(DTAUDIO_WRITE_ARGS);
typedef dt_rc_e dtaudio_set_volume_f(DTAUDIO_SET_VOLUME_ARGS);
typedef dt_rc_e dtaudio_get_volume_f(DTAUDIO_GET_VOLUME_ARGS);

#include <dtaudio.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
