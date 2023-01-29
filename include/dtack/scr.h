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
| include $RCSfile: dtscr.h,v $ $Revision: 1.12 $
 *..........................................................................*/

typedef enum {
  DTSCR_ACTION_UNSPECIFIED=0, 
  DTSCR_ACTION_NOOP, 
  DTSCR_ACTION_DEBUG, 
  DTSCR_ACTION_CLOSE_ONE, 
  DTSCR_ACTION_CLOSE_FAMILY, 
  DTSCR_ACTION_CLOSE_ALL, 
  DTSCR_ACTION_ICONIFY_ONE, 
  DTSCR_ACTION_ICONIFY_FAMILY, 
  DTSCR_ACTION_ICONIFY_ALL, 
  DTSCR_ACTION_RAISE_ONE, 
  DTSCR_ACTION_RAISE_FAMILY, 
  DTSCR_ACTION_RAISE_ALL, 
  DTSCR_ACTION_LOWER_ONE, 
  DTSCR_ACTION_LOWER_FAMILY, 
  DTSCR_ACTION_LOWER_ALL, 
  DTSCR_ACTION_COLOR_ON,
  DTSCR_ACTION_COLOR_OFF,
  DTSCR_ACTION_COLORMAP_ONE, 
  DTSCR_ACTION_COLORMAP_FAMILY, 
  DTSCR_ACTION_COLORMAP_ALL, 
  DTSCR_ACTION_MOVE, 
  DTSCR_ACTION_MOVE_UP, 
  DTSCR_ACTION_MOVE_DOWN, 
  DTSCR_ACTION_MOVE_LEFT, 
  DTSCR_ACTION_MOVE_RIGHT, 
  DTSCR_ACTION_MOVE_UP_FAST, 
  DTSCR_ACTION_MOVE_DOWN_FAST, 
  DTSCR_ACTION_MOVE_LEFT_FAST, 
  DTSCR_ACTION_MOVE_RIGHT_FAST, 
  DTSCR_ACTION_LATCH,
  DTSCR_ACTION_CYCLE_NEXT,
  DTSCR_ACTION_CYCLE_PREV,
  DTSCR_ACTION_REFRESH,
  DTSCR_ACTION_QUIT,
  DTSCR_ACTION_EXIT
} dtscr_action_e;

#define DTSCR_KEY_UPARROW         ('k')
#define DTSCR_KEY_DOWNARROW       ('j')
#define DTSCR_KEY_LEFTARROW       ('h')
#define DTSCR_KEY_RIGHTARROW      ('l')
#define DTSCR_KEY_UPARROW_FAST    ('K')
#define DTSCR_KEY_DOWNARROW_FAST  ('J')
#define DTSCR_KEY_LEFTARROW_FAST  ('H')
#define DTSCR_KEY_RIGHTARROW_FAST ('L')
#define DTSCR_KEY_ESCAPE          (0x1b)

#define DTSCR_KEY_QUIT (0x11)			/* ^q - quit						*/
#define DTSCR_KEY_DEBUG (0x01)			/* ^a - debug						*/
#define DTSCR_KEY_REFRESH (0x0c)		/* ^l - redraw all					*/
#define DTSCR_KEY_CLOSE_ONE (0x04)		/* ^d - delete (close) one			*/
#define DTSCR_KEY_ICONIFY_ONE (0x0d)	/* ^m - iconify one					*/
#define DTSCR_KEY_COLORMAP_ONE (0x16)	/* ^v - colormap one				*/
#define DTSCR_KEY_ENTER ('\n')			/* enter - raise one				*/
#define DTSCR_KEY_LATCH (' ')
#define DTSCR_KEY_CYCLE_NEXT (0x09)		/* tab - cycle next				    */
#define DTSCR_KEY_CYCLE_PREV (0x08)		/* backspace - cycle prev		    */

#define DTSCR_CURSOR_XSIZE (16)
#define DTSCR_CURSOR_YSIZE (16)

typedef struct {
  dtlut8_t current[256];
  unsigned char user_to_display[256];
  unsigned char display_to_user[256];
} dtscr_luts_t;

#define DTSCR_QUIT1 (0x01)

#define DTSCR_ACTION_STRING_MAX (32)
typedef struct dtscr_state_t {
  char windowname[						/* window's name					*/
    DT_WINDOWNAME_MAXLEN];
  int active;
  int x0, y0;							/* window position on screen	    */
  int xe, ye;							/* window size					    */
  int bits;								/* bits per pixel				    */
  int x, y;								/* mouse position				    */
  int left, middle, right;				/* mouse buttons				    */
  char keystring[DT_KEYSTRING_MAXLEN];	/* keyboard response			    */
  dtscr_action_e action;				/* action associated with state		*/
  struct dtscr_state_t *next;			/* for linked lists					*/
} dtscr_state_t;

typedef struct {
  dtscr_state_t *head;
} dtscr_states_t;

typedef struct {
  int depth;							/* depth of the drawable itself	    */
  int pixmap_bits;						/* depth of pixmap last written	    */
  dtscr_state_t state;
  char name[DT_WINDOWNAME_MAXLEN];
  long rowsize;							/* size of one row in pixels		*/
  int bpr;								/* size of one row in bytes			*/
  dtscr_luts_t *luts;
  void *priv;
  int x0, y0, xe, ye;
  void *mem;
  long id;
} dtscr_win_t;

#define DTSCR_CREATE_ARGS dt_ctl_t *, struct dtscr_t *, char *

#define DTSCR_CREATE_WINDOW_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, \
  const char *, int, int, int, int, int

#define DTSCR_EXPORT_DATA_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, \
  int, int, int, int, int, void *

#define DTSCR_IMPORT_DATA_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, \
  int, int, int, int, void *

#define DTSCR_EXPORT_COLOR_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, \
  dtlut8_t *, int, int

#define DTSCR_IMPORT_COLOR_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, \
  dtlut8_t *, int, int

#define DTSCR_CURSOR_ARGS \
  dt_ctl_t *, struct dtscr_t *, int *x, int *y, int op

#define DTSCR_WAIT_EVENT_ARGS \
  dt_ctl_t *, \
  struct dtscr_t *, \
  dtsvc_t *svc, \
  double timeout, \
  int flag, \
  dt_event_t *event

#define DTSCR_HANDLE_EVENT_ARGS \
  dt_ctl_t *, \
  struct dtscr_t *, \
  void *buffer, \
  int *handled, \
  dt_event_t *event

#define DTSCR_DESTROY_WINDOW_ARGS dt_ctl_t *, struct dtscr_t *, dtscr_win_t *
#define DTSCR_DESTROY_ARGS dt_ctl_t *, struct dtscr_t *
#define DTSCR_REFRESH_ARGS dt_ctl_t *, struct dtscr_t *
#define DTSCR_REDRAW_ARGS dt_ctl_t *, struct dtscr_t *, dtscr_win_t *

#define DTSCR_RAISE_ARGS   dt_ctl_t *, struct dtscr_t *, dtscr_win_t *
#define DTSCR_LOWER_ARGS   dt_ctl_t *, struct dtscr_t *, dtscr_win_t *
#define DTSCR_PRINT_ARGS   dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, \
                             const char *, double, int *
#define DTSCR_ICONIFY_ARGS dt_ctl_t *, struct dtscr_t *, dtscr_win_t *
#define DTSCR_MOVE_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *, int, int
#define DTSCR_COLORMAP_ARGS \
  dt_ctl_t *, struct dtscr_t *, dtscr_win_t *

typedef struct dtscr_t {
  dt_rc_e (*create_window)(DTSCR_CREATE_WINDOW_ARGS);
  dt_rc_e (*wait_event)(DTSCR_WAIT_EVENT_ARGS);
  dt_rc_e (*handle_event)(DTSCR_HANDLE_EVENT_ARGS);
  dt_rc_e (*destroy_window)(DTSCR_DESTROY_WINDOW_ARGS);
  dt_rc_e (*destroy)(DTSCR_DESTROY_ARGS);
  dt_rc_e (*refresh)(DTSCR_REFRESH_ARGS);
  dt_rc_e (*redraw)(DTSCR_REDRAW_ARGS);
  dt_rc_e (*export_data)(DTSCR_EXPORT_DATA_ARGS);
  dt_rc_e (*import_data)(DTSCR_IMPORT_DATA_ARGS);
  dt_rc_e (*export_color)(DTSCR_EXPORT_COLOR_ARGS);
  dt_rc_e (*import_color)(DTSCR_IMPORT_COLOR_ARGS);
  dt_rc_e (*raise)(DTSCR_RAISE_ARGS);
  dt_rc_e (*lower)(DTSCR_LOWER_ARGS);
  dt_rc_e (*print)(DTSCR_PRINT_ARGS);
  dt_rc_e (*iconify)(DTSCR_ICONIFY_ARGS);
  dt_rc_e (*move)(DTSCR_MOVE_ARGS);
  dt_rc_e (*colormap)(DTSCR_COLORMAP_ARGS);
  dt_rc_e (*cursor)(DTSCR_CURSOR_ARGS);
  dtscr_states_t states_default;

  dt_event_t last_event;				/* last event that happened			*/
  dtlist_t windowlist;					/* list of windows				    */

  void *priv;
  dtscr_win_t *latch;
  void *latchbuf;

  dtlut8_t *display_lut;
  dtscr_win_t *tmp_win;
  char *cursorbuf;

  int selectfd;
  int nwin;								/* number of windows after event    */
  int sigevent;							/* significant event counter	    */
  int cycle;							/* window cycle position		    */
  int cursorx;
  int cursory;
  int have_mouse;						/* true if OS provides mouse	    */
  int have_window_manager;				/* true if OS lets you move windows	*/
  int must_remap;						/* scr must do color matching	    */
  int overheadx;						/* per-window border overhead	    */
  int overheady;
  int margin_left, margin_right;
  int margin_top, margin_bot;
  int white;							/* for cursor and latch outline		*/
  int black;
  int xe;
  int ye;
  int refresh_pending;
} dtscr_t;

typedef dt_rc_e DTCONFIG_API1 dtscr_create_f(DTSCR_CREATE_ARGS);
typedef dt_rc_e dtscr_create_window_f(DTSCR_CREATE_WINDOW_ARGS);
typedef dt_rc_e dtscr_wait_event_f(DTSCR_WAIT_EVENT_ARGS);
typedef dt_rc_e dtscr_handle_event_f(DTSCR_HANDLE_EVENT_ARGS);
typedef dt_rc_e dtscr_destroy_window_f(DTSCR_DESTROY_WINDOW_ARGS);
typedef dt_rc_e dtscr_destroy_f(DTSCR_DESTROY_ARGS);
typedef dt_rc_e dtscr_refresh_f(DTSCR_REFRESH_ARGS);
typedef dt_rc_e dtscr_redraw_f(DTSCR_REDRAW_ARGS);
typedef dt_rc_e dtscr_export_data_f(DTSCR_EXPORT_DATA_ARGS);
typedef dt_rc_e dtscr_import_data_f(DTSCR_IMPORT_DATA_ARGS);
typedef dt_rc_e dtscr_export_color_f(DTSCR_EXPORT_COLOR_ARGS);
typedef dt_rc_e dtscr_import_color_f(DTSCR_IMPORT_COLOR_ARGS);

typedef dt_rc_e dtscr_raise_f(DTSCR_RAISE_ARGS);
typedef dt_rc_e dtscr_lower_f(DTSCR_LOWER_ARGS);
typedef dt_rc_e dtscr_print_f(DTSCR_PRINT_ARGS);
typedef dt_rc_e dtscr_iconify_f(DTSCR_ICONIFY_ARGS);
typedef dt_rc_e dtscr_move_f(DTSCR_MOVE_ARGS);
typedef dt_rc_e dtscr_colormap_f(DTSCR_COLORMAP_ARGS);
typedef dt_rc_e dtscr_cursor_f(DTSCR_CURSOR_ARGS);

#define DTSCR_HELP "\
<screen-spec-list> is a plus-sign separated list the form:\n\
      <screen-spec>[+<screen-spec>...]\n\
    <screen-spec> one of:\n\
      dummy\n\
      hws,<hws-spec>\n\
      canvas,<canvas-spec>\n\
    <hws-spec> is one of:\n\
      x,<host[:screen]>\n\
      qw4\n\
    <canvas-spec> is one of:\n\
      fastgraph,[chipset,[resolution]]\n\
      svgacc,[chipset,[resolution]]\n\
      hws,<hws-spec>"

#include <dtack/scr.p>







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
