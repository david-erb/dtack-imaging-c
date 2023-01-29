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
| include $RCSfile: dthws.h,v $ $Revision: 1.4 $
 *..........................................................................*/

#define DTHWS_CONNECT_ARGS      dt_ctl_t *, struct dthws_t *, char *
#define DTHWS_DISCONNECT_ARGS   dt_ctl_t *, struct dthws_t *
#define DTHWS_WIN_CREATE_ARGS   dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  const char *, int, int, int, int, int
#define DTHWS_WAIT_EVENT_ARGS   dt_ctl_t *, struct dthws_t *, \
                                  dtsvc_t *, double timeout, \
                                  int flag, dt_event_t *
#define DTHWS_HANDLE_EVENT_ARGS   dt_ctl_t *, struct dthws_t *, \
                                  void *buffer, int *handled, dt_event_t *
#define DTHWS_WIN_DESTROY_ARGS  dt_ctl_t *, struct dthws_t *, dthws_win_t *
#define DTHWS_EXPORT_DATA_ARGS  dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  void *, int, int, int, int
#define DTHWS_IMPORT_DATA_ARGS  dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  void *, int, int, int, int
#define DTHWS_EXPORT_COLOR_ARGS dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  dtlut8_t *, int, int
#define DTHWS_IMPORT_COLOR_ARGS dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  dtlut8_t *, int, int
#define DTHWS_WIN_CURSOR_ARGS   dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  int *, int *, int
#define DTHWS_WIN_CLEAR_ARGS    dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  int
#define DTHWS_WIN_RAISE_ARGS    dt_ctl_t *, struct dthws_t *, dthws_win_t *
#define DTHWS_WIN_LOWER_ARGS    dt_ctl_t *, struct dthws_t *, dthws_win_t *
#define DTHWS_WIN_PRINT_ARGS    dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  const char *, double, int *
#define DTHWS_WIN_ICONIFY_ARGS    dt_ctl_t *, struct dthws_t *, dthws_win_t *
#define DTHWS_WIN_MOVE_ARGS     dt_ctl_t *, struct dthws_t *, dthws_win_t *, \
                                  int, int


typedef struct {
  void *priv;
  void *memalloc;
  long id;
  int x0;
  int y0;
  int xe;
  int ye;
  int bits;
} dthws_win_t;

typedef struct dthws_t {
  void *priv;
  dt_rc_e (*disconnect)(DTHWS_DISCONNECT_ARGS);
  dt_rc_e (*win_create)(DTHWS_WIN_CREATE_ARGS);
  dt_rc_e (*wait_event)(DTHWS_WAIT_EVENT_ARGS);
  dt_rc_e (*handle_event)(DTHWS_HANDLE_EVENT_ARGS);
  dt_rc_e (*win_destroy)(DTHWS_WIN_DESTROY_ARGS);
  dt_rc_e (*export_data)(DTHWS_EXPORT_DATA_ARGS);
  dt_rc_e (*import_data)(DTHWS_IMPORT_DATA_ARGS);
  dt_rc_e (*export_color)(DTHWS_EXPORT_COLOR_ARGS);
  dt_rc_e (*import_color)(DTHWS_IMPORT_COLOR_ARGS);
  dt_rc_e (*win_cursor)(DTHWS_WIN_CURSOR_ARGS);
  dt_rc_e (*win_clear)(DTHWS_WIN_CLEAR_ARGS);
  dt_rc_e (*win_raise)(DTHWS_WIN_RAISE_ARGS);
  dt_rc_e (*win_lower)(DTHWS_WIN_LOWER_ARGS);
  dt_rc_e (*win_print)(DTHWS_WIN_PRINT_ARGS);
  dt_rc_e (*win_iconify)(DTHWS_WIN_ICONIFY_ARGS);
  dt_rc_e (*win_move)(DTHWS_WIN_MOVE_ARGS);
  int xe;
  int ye;
  int do_remap;
  int do_ymirror;
  int nprint;
  dtlist_t windowlist;
} dthws_t;

typedef dt_rc_e DTCONFIG_API1 dthws_connect_f(DTHWS_CONNECT_ARGS);
typedef dt_rc_e dthws_disconnect_f(DTHWS_DISCONNECT_ARGS);
typedef dt_rc_e dthws_win_create_f(DTHWS_WIN_CREATE_ARGS);
typedef dt_rc_e dthws_wait_event_f(DTHWS_WAIT_EVENT_ARGS);
typedef dt_rc_e dthws_handle_event_f(DTHWS_HANDLE_EVENT_ARGS);
typedef dt_rc_e dthws_win_destroy_f(DTHWS_WIN_DESTROY_ARGS);
typedef dt_rc_e dthws_export_data_f(DTHWS_EXPORT_DATA_ARGS);
typedef dt_rc_e dthws_import_data_f(DTHWS_IMPORT_DATA_ARGS);
typedef dt_rc_e dthws_export_color_f(DTHWS_EXPORT_COLOR_ARGS);
typedef dt_rc_e dthws_import_color_f(DTHWS_IMPORT_COLOR_ARGS);
typedef dt_rc_e dthws_win_cursor_f(DTHWS_WIN_CURSOR_ARGS);
typedef dt_rc_e dthws_win_clear_f(DTHWS_WIN_CLEAR_ARGS);
typedef dt_rc_e dthws_win_raise_f(DTHWS_WIN_RAISE_ARGS);
typedef dt_rc_e dthws_win_lower_f(DTHWS_WIN_LOWER_ARGS);
typedef dt_rc_e dthws_win_print_f(DTHWS_WIN_PRINT_ARGS);
typedef dt_rc_e dthws_win_iconify_f(DTHWS_WIN_ICONIFY_ARGS);
typedef dt_rc_e dthws_win_move_f(DTHWS_WIN_MOVE_ARGS);

#include <dtack/hws.p>







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
