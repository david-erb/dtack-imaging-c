/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTWINS_CREATE_WINDOW_ARGS \
  dt_ctl_t *ctl, \
  struct dtwins_t *wins, \
  dtwin_r_create_window_arg_t *argp, \
  dtwin_r_create_window_ret_t *res

#define DTWINS_EXPORT_DATA_ARGS \
  dt_ctl_t *ctl, \
  struct dtwins_t *wins, \
  dtwin_r_export_data_arg_t *argp, \
  dtwin_r_export_data_ret_t *res

#define DTWINS_IMPORT_DATA_ARGS \
  dt_ctl_t *ctl, \
  struct dtwins_t *wins, \
  dtwin_r_import_data_arg_t *argp, \
  dtwin_r_import_data_ret_t *res

#define DTWINS_EXPORT_COLOR_ARGS \
  dt_ctl_t *ctl, \
  struct dtwins_t *wins, \
  dtwin_r_export_color_arg_t *argp, \
  dtwin_r_export_color_ret_t *res

#define DTWINS_IMPORT_COLOR_ARGS \
  dt_ctl_t *ctl, \
  struct dtwins_t *wins, \
  dtwin_r_import_color_arg_t *argp, \
  dtwin_r_import_color_ret_t *res

#define DTWINS_QUERY_ARGS \
  dt_ctl_t *ctl, \
  struct dtwins_t *wins, \
  dtwin_r_query_arg_t *argp, \
  dtwin_r_query_ret_t *res

typedef struct dtwins_t {
  dt_rc_e (*create_window)(DTWINS_CREATE_WINDOW_ARGS);
  dt_rc_e (*export_data)(DTWINS_EXPORT_DATA_ARGS);
  dt_rc_e (*import_data)(DTWINS_IMPORT_DATA_ARGS);
  dt_rc_e (*export_color)(DTWINS_EXPORT_COLOR_ARGS);
  dt_rc_e (*import_color)(DTWINS_IMPORT_COLOR_ARGS);
  dt_rc_e (*query)(DTWINS_QUERY_ARGS);
  dtscr_t *scr;							/* scr to use when serving 			*/
} dtwins_t;

#include <dtack/wins.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
