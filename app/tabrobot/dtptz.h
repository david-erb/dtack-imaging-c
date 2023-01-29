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
 *..........................................................................*/

#define DTPTZ_INIT_ARGS         dt_ctl_t *, struct dtptz_t *, const char *
#define DTPTZ_STOP_ARGS         dt_ctl_t *, struct dtptz_t *
#define DTPTZ_UNINIT_ARGS       dt_ctl_t *, struct dtptz_t *
#define DTPTZ_COMMAND_ARGS      dt_ctl_t *, struct dtptz_t *, const char *

#define DTPTZ_COMMAND_RESET "!!"
#define DTPTZ_COMMAND_STOP "<>"

typedef struct dtptz_t {
  void *priv;
  dt_rc_e (*uninit)(DTPTZ_UNINIT_ARGS);
  dt_rc_e (*stop)(DTPTZ_STOP_ARGS);
  dt_rc_e (*command)(DTPTZ_COMMAND_ARGS);
  int initialized;
} dtptz_t;

typedef dt_rc_e DTCONFIG_API1 dtptz_init_f(DTPTZ_INIT_ARGS);
typedef dt_rc_e dtptz_uninit_f(DTPTZ_UNINIT_ARGS);
typedef dt_rc_e dtptz_stop_f(DTPTZ_STOP_ARGS);
typedef dt_rc_e dtptz_command_f(DTPTZ_COMMAND_ARGS);

#include <dtptz.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
