/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTISTREAM_INIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtistream_t *istream, \
  const char *spec, \
  dtstream_id_t *id, \
  dtstream_id_t *permit, \
  long timeout
#define DTISTREAM_UNINIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtistream_t *istream
#define DTISTREAM_IS_AVAILABLE_ARGS \
  dt_ctl_t *ctl, \
  struct dtistream_t *istream, \
  int *is_available
#define DTISTREAM_RECV_ARGS \
  dt_ctl_t *ctl, \
  struct dtistream_t *istream, \
  void **packet, \
  long *packet_length

typedef struct dtistream_t {
  void *priv;
  char *scheme;
  dt_rc_e (*uninit)(DTISTREAM_UNINIT_ARGS);
  dt_rc_e (*is_available)(DTISTREAM_IS_AVAILABLE_ARGS);
  dt_rc_e (*recv)(DTISTREAM_RECV_ARGS);
  int initialized;
} dtistream_t;

typedef dt_rc_e DTCONFIG_API1 dtistream_init_f(DTISTREAM_INIT_ARGS);
typedef dt_rc_e dtistream_uninit_f(DTISTREAM_UNINIT_ARGS);
typedef dt_rc_e dtistream_is_available_f(DTISTREAM_IS_AVAILABLE_ARGS);
typedef dt_rc_e dtistream_recv_f(DTISTREAM_RECV_ARGS);

#include <dtistream.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
