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
| include $RCSfile: dtfb.h,v $ $Revision: 1.6 $
 *..........................................................................*/

typedef enum {
  DTFB_CONTROL_FREEZE,
  DTFB_CONTROL_ACQUIRE,
  DTFB_CONTROL_SNAPSHOT,
  DTFB_CONTROL_RESET
} dtfb_control_e;

#define DTFB_INIT_ARGS      dt_ctl_t *, struct dtfb_t *, const char *
#define DTFB_UNINIT_ARGS    dt_ctl_t *, struct dtfb_t *
#define DTFB_CONTROL_ARGS   dt_ctl_t *, struct dtfb_t *, dtfb_control_e
#define DTFB_PUTBLOCK_ARGS  dt_ctl_t *, struct dtfb_t *, \
                              void *, int, int, int, int
#define DTFB_GETBLOCK_ARGS  dt_ctl_t *, struct dtfb_t *, \
                              void *, int, int, int, int
#define DTFB_SOURCE_ARGS  dt_ctl_t *, struct dtfb_t *, const char *source
#define DTFB_PUTCOLOR_ARGS  dt_ctl_t *, struct dtfb_t *, \
                                  dtlut8_t *, int, int
#define DTFB_GETCOLOR_ARGS  dt_ctl_t *, struct dtfb_t *, \
                                  dtlut8_t *, int, int
#define DTFB_CURSOR_ARGS    dt_ctl_t *, struct dtfb_t *, \
                                  int *, int *, int
#define DTFB_CLEAR_ARGS     dt_ctl_t *, struct dtfb_t *, int

typedef struct dtfb_t {
  void *priv;
  char *type;
  char *info;
  dt_rc_e (*uninit)(DTFB_UNINIT_ARGS);
  dt_rc_e (*control)(DTFB_CONTROL_ARGS);
  dt_rc_e (*putblock)(DTFB_PUTBLOCK_ARGS);
  dt_rc_e (*getblock)(DTFB_GETBLOCK_ARGS);
  dt_rc_e (*source)(DTFB_SOURCE_ARGS);
  dt_rc_e (*putcolor)(DTFB_PUTCOLOR_ARGS);
  dt_rc_e (*getcolor)(DTFB_GETCOLOR_ARGS);
  dt_rc_e (*cursor)(DTFB_CURSOR_ARGS);
  dt_rc_e (*clear)(DTFB_CLEAR_ARGS);
  dt_color_org_t org;
  unsigned int rowsize;
  int xe;
  int ye;
} dtfb_t;

typedef dt_rc_e DTCONFIG_API1 dtfb_init_f(DTFB_INIT_ARGS);
typedef dt_rc_e dtfb_uninit_f(DTFB_UNINIT_ARGS);
typedef dt_rc_e dtfb_control_f(DTFB_CONTROL_ARGS);
typedef dt_rc_e dtfb_putblock_f(DTFB_PUTBLOCK_ARGS);
typedef dt_rc_e dtfb_getblock_f(DTFB_GETBLOCK_ARGS);
typedef dt_rc_e dtfb_source_f(DTFB_SOURCE_ARGS);
typedef dt_rc_e dtfb_putcolor_f(DTFB_PUTCOLOR_ARGS);
typedef dt_rc_e dtfb_getcolor_f(DTFB_GETCOLOR_ARGS);
typedef dt_rc_e dtfb_cursor_f(DTFB_CURSOR_ARGS);
typedef dt_rc_e dtfb_clear_f(DTFB_CLEAR_ARGS);

#include <dtack/fb.p>







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
