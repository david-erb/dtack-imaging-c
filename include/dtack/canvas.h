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
| include $RCSfile: dtcanvas.h,v $ $Revision: 1.6 $
 *..........................................................................*/

#define DTCANVAS_INIT_ARGS      dt_ctl_t *, struct dtcanvas_t *, char *
#define DTCANVAS_UNINIT_ARGS    dt_ctl_t *, struct dtcanvas_t *
#define DTCANVAS_OPEN_ARGS      dt_ctl_t *, struct dtcanvas_t *
#define DTCANVAS_INPUT_ARGS     dt_ctl_t *, struct dtcanvas_t *, \
                                  dtsvc_t *svc, double timeout, int, dt_event_t *
#define DTCANVAS_CLOSE_ARGS     dt_ctl_t *, struct dtcanvas_t *
#define DTCANVAS_PUTBLOCK_ARGS  dt_ctl_t *, struct dtcanvas_t *, \
                                  void *, int, int, int, int
#define DTCANVAS_GETBLOCK_ARGS  dt_ctl_t *, struct dtcanvas_t *, \
                                  void *, int, int, int, int
#define DTCANVAS_PUTCOLOR_ARGS  dt_ctl_t *, struct dtcanvas_t *, \
                                  dtlut8_t *, int, int
#define DTCANVAS_GETCOLOR_ARGS  dt_ctl_t *, struct dtcanvas_t *, \
                                  dtlut8_t *, int, int
#define DTCANVAS_CURSOR_ARGS    dt_ctl_t *, struct dtcanvas_t *, \
                                  int *, int *, int
#define DTCANVAS_CLEAR_ARGS     dt_ctl_t *, struct dtcanvas_t *, int

typedef struct dtcanvas_t {
  void *priv;
  char *server;
  char *driver;
  dt_rc_e (DTCONFIG_API1DEC *init)(DTCANVAS_INIT_ARGS);
  dt_rc_e (*uninit)(DTCANVAS_UNINIT_ARGS);
  dt_rc_e (*open)(DTCANVAS_OPEN_ARGS);
  dt_rc_e (*input)(DTCANVAS_INPUT_ARGS);
  dt_rc_e (*close)(DTCANVAS_CLOSE_ARGS);
  dt_rc_e (*putblock)(DTCANVAS_PUTBLOCK_ARGS);
  dt_rc_e (*getblock)(DTCANVAS_GETBLOCK_ARGS);
  dt_rc_e (*putcolor)(DTCANVAS_PUTCOLOR_ARGS);
  dt_rc_e (*getcolor)(DTCANVAS_GETCOLOR_ARGS);
  dt_rc_e (*cursor)(DTCANVAS_CURSOR_ARGS);
  dt_rc_e (*clear)(DTCANVAS_CLEAR_ARGS);
  int xe;
  int ye;
  int must_remap;						/* caller must do color matching	*/
} dtcanvas_t;

typedef dt_rc_e DTCONFIG_API1 dtcanvas_init_f(DTCANVAS_INIT_ARGS);
typedef dt_rc_e dtcanvas_uninit_f(DTCANVAS_UNINIT_ARGS);
typedef dt_rc_e dtcanvas_open_f(DTCANVAS_OPEN_ARGS);
typedef dt_rc_e dtcanvas_input_f(DTCANVAS_INPUT_ARGS);
typedef dt_rc_e dtcanvas_close_f(DTCANVAS_CLOSE_ARGS);
typedef dt_rc_e dtcanvas_putblock_f(DTCANVAS_PUTBLOCK_ARGS);
typedef dt_rc_e dtcanvas_getblock_f(DTCANVAS_GETBLOCK_ARGS);
typedef dt_rc_e dtcanvas_putcolor_f(DTCANVAS_PUTCOLOR_ARGS);
typedef dt_rc_e dtcanvas_getcolor_f(DTCANVAS_GETCOLOR_ARGS);
typedef dt_rc_e dtcanvas_cursor_f(DTCANVAS_CURSOR_ARGS);
typedef dt_rc_e dtcanvas_clear_f(DTCANVAS_CLEAR_ARGS);

typedef 
dt_rc_e 
DTCONFIG_API1
dtcanvas_mouse_f(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x, 
  int *y,
  int *buttons);

#include <dtack/canvas.p>







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
