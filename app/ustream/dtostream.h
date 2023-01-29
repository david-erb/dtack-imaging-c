/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTOSTREAM_INIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtostream_t *ostream, \
  const char *spec, \
  dtstream_id_t *id, \
  dtstream_id_t *permit, \
  long timeout
#define DTOSTREAM_UNINIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtostream_t *ostream
#define DTOSTREAM_SEND_ARGS \
  dt_ctl_t *ctl, \
  struct dtostream_t *ostream, \
  void *packet, \
  long packet_length

typedef struct dtostream_t {
  void *priv;
  char *scheme;
  dt_rc_e (*uninit)(DTOSTREAM_UNINIT_ARGS);
  dt_rc_e (*send)(DTOSTREAM_SEND_ARGS);
  int initialized;
} dtostream_t;

typedef dt_rc_e DTCONFIG_API1 dtostream_init_f(DTOSTREAM_INIT_ARGS);
typedef dt_rc_e dtostream_uninit_f(DTOSTREAM_UNINIT_ARGS);
typedef dt_rc_e dtostream_send_f(DTOSTREAM_SEND_ARGS);

#include <dtostream.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
