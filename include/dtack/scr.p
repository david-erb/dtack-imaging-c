#ifdef __cplusplus
extern "C" {
#endif
#ifdef DX_PREFIX
#  undef DX_PREFIX
#endif
#ifndef DT_PREFIX
#  define DX_PREFIX
#else
#  define DX_PREFIX DT_PREFIX
#endif
#ifdef DX_SUFFIX
#  undef DX_SUFFIX
#endif
#ifndef DT_SUFFIX
#  define DX_SUFFIX ;
#else
#  define DX_SUFFIX DT_SUFFIX
#endif
#ifdef DT_DEFARGVAL
#  undef DT_DEFARGVAL
#  define DT_DEFARGVAL(A) = (A)
#endif

/*----in scr_main.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_create(							/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *server)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_margin(					/* draw margin in backing bitmap	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_create_window(					/* make a window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)								/* bits per pixel					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_export_data(						/* export patch into window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits,
  void *data)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_import_data(						/* extract image from window	    */
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  void *data)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_export_color(						/* export color lut to window		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_import_color(						/* import color lut to window		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtlut8_t *lut,
  int start,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_translate_event(					/* translate event 					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int *user,
  int *quit)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_wait_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtsvc_t *svc,
  int flag)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_handle_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  void *buffer,
  int *handled)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_query_state(						/* query window state				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtscr_state_t *state)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_destroy(					/* close window					    */
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_destroy(							/* destroy the screen				*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_print(						/* print window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_iconify(					/* iconify window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_move(						/* move window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_window_colormap(					/* colormap window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_refresh(							/* refresh the screen				*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_raise_all(
  dt_ctl_t *ctl,
  dtscr_t *scr)
DX_SUFFIX

/*----in scr_cur.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_latch_draw(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int flag)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_latch(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_cursor_draw(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int flag)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_cursor_bump(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int dx,
  int dy)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_cursor_move(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int dx,
  int dy)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_cycle(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int direction)
DX_SUFFIX

/*----in scr_act.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_match(						/* determine if states match		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state1,
  dtscr_state_t *state2,
  int *match)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_matches(					/* count matches for state in list	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  dtscr_state_t *state,
  int *count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_free(	
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_add(						/* add new state					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  dtscr_state_t *state)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_dontcare(					/* make new don't-care state		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_make_dontcare(				/* make a new don't-care state		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t **pstate)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_keystring_action(			/* add new state for keystring		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  char *keystring,
  dtscr_action_e action)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_default_actions(			/* make default states list			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_action(						/* take action in state				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state,
  dtscr_action_e action,
  int *quit)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_actions(					/* take action in matching states	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  dtscr_state_t *state,
  int *quit)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_action_string(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_action_e action,
  char *string)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_state_string(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state,
  char *string,
  int lmax)
DX_SUFFIX

/*----in scr_dbg.c:----*/


DX_PREFIX
void
DTCONFIG_API1
dtscr_state_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state,
  char *F,
  long mask,
  char *prefix)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtscr_states_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  char *F,
  long mask)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtscr_list_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *F,
  long mask)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtscr_all_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *F,
  long mask)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtscr_limits_dbg(						/* debug limits stuff				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *F,
  long mask)
DX_SUFFIX

/*----in scrdum.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_dummy_create(						/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *server)
DX_SUFFIX

/*----in scrcan.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_canvas_create(					/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *server)
DX_SUFFIX

/*----in scrhws.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtscr_hws_create(						/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *server)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
