/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................*/

#define DTWINC_INIT_ARGS dt_ctl_t *, dtwinc_t *
#define DTWINC_UNINIT_ARGS dt_ctl_t *, struct dtwinc_t *
#define DTWINC_CREATE_WINDOW_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_create_window_arg_t *, \
                      dtwin_r_create_window_ret_t **
#define DTWINC_EXPORT_DATA_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_export_data_arg_t *, \
                      dtwin_r_export_data_ret_t **
#define DTWINC_EXPORT_COLOR_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_export_color_arg_t *, \
                      dtwin_r_export_color_ret_t **
#define DTWINC_IMPORT_DATA_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_import_data_arg_t *, \
                      dtwin_r_import_data_ret_t **
#define DTWINC_IMPORT_COLOR_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_import_color_arg_t *, \
                      dtwin_r_import_color_ret_t **
#define DTWINC_WAIT_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_wait_arg_t *, \
                      dtwin_r_wait_ret_t **
#define DTWINC_QUERY_ARGS dt_ctl_t *, struct dtwinc_t *, \
                      dtwin_r_query_arg_t *, \
                      dtwin_r_query_ret_t **

#define DTWINC_MAXLEN (8)
#define DTWINC_SERVER_MAXLEN (128)
#define DTWINC_EXTRA_MAXLEN (32)

typedef struct dtwinc_t {
  dt_rc_e (*uninit)(DTWINC_UNINIT_ARGS);
  dt_rc_e (*create_window)(DTWINC_CREATE_WINDOW_ARGS);
  dt_rc_e (*export_data)(DTWINC_EXPORT_DATA_ARGS);
  dt_rc_e (*export_color)(DTWINC_EXPORT_COLOR_ARGS);
  dt_rc_e (*import_data)(DTWINC_IMPORT_DATA_ARGS);
  dt_rc_e (*import_color)(DTWINC_IMPORT_COLOR_ARGS);
  dt_rc_e (*query)(DTWINC_QUERY_ARGS);
  char server[DTWINC_SERVER_MAXLEN];
  char extra[DTWINC_EXTRA_MAXLEN];
  long send_max;
  long recv_max;
} dtwinc_t;

typedef dt_rc_e DTCONFIG_API1 dtwinc_init_f(DTWINC_INIT_ARGS);

#include <dtack/winc.p>






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
