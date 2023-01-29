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

/*----in note.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_clear(							/* clear note						*/
  dt_ctl_t *ctl,
  dtnote_t *note)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_newpage(							/* go to next page for writing		*/
  dt_ctl_t *ctl,
  dtnote_t *note)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_add(
  dt_ctl_t *ctl,
  dtnote_t *note,
  const char *s,
  int l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_append(
  dt_ctl_t *ctl,
  dtnote_t *note,
  const char *s,
  int l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_setpos(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor,
  long pos)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_nextpage(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_next(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor,
  char **s)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_prevpage(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtnote_prev(
  dt_ctl_t *ctl,
  dtnote_t *note,
  dtnote_cursor_t *cursor,
  char **s)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
