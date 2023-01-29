/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DTSVC_FLAG_SELF_BACKGROUND 0x02

#define DTSVC_SERVER_MAXLEN (128)

typedef enum {
  DTSVC_TYPE_NONE = 0,
  DTSVC_TYPE_IPC = 1,
  DTSVC_TYPE_DSK = 2
} dtsvc_type_e;

#define DTSVC_REGISTER_ARGS \
  dt_ctl_t *ctl, \
  struct dtsvc_t *svc, \
  const char *name, \
  unsigned long flags
#define DTSVC_UNREGISTER_ARGS \
  dt_ctl_t *ctl, \
  struct dtsvc_t *svc
#define DTSVC_WAIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtsvc_t *svc
#define DTSVC_ISSVC_ARGS \
  dt_ctl_t *ctl, \
  struct dtsvc_t *svc, \
  unsigned long id, \
  void *h, \
  int *issvc

typedef struct dtsvc_t {
  dt_rc_e (*unregister)(DTSVC_UNREGISTER_ARGS);
  dt_rc_e (*wait)(DTSVC_WAIT_ARGS);
  dt_rc_e (*issvc)(DTSVC_ISSVC_ARGS);
  void *priv;
  unsigned long flags;
  int selectfd;
  dtsvc_type_e type;
  char server[DTSVC_SERVER_MAXLEN];
} dtsvc_t;

typedef dt_rc_e DTCONFIG_API1 dtsvc_register_f(DTSVC_REGISTER_ARGS);
typedef dt_rc_e dtsvc_unregister_f(DTSVC_UNREGISTER_ARGS);
typedef dt_rc_e dtsvc_wait_f(DTSVC_WAIT_ARGS);
typedef dt_rc_e dtsvc_issvc_f(DTSVC_ISSVC_ARGS);
  
#include <dtack/svc.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
