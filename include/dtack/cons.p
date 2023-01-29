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

/*----in cons.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_init(
  dt_ctl_t *ctl)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_uninit(
  dt_ctl_t *ctl)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_add(
  dt_ctl_t *ctl,
  const char *tag,
  dtcons_opfunc_f *opfunc,
  void *arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_search(							/* search, no error if not found    */
  dt_ctl_t *ctl,
  const char *tag,
  dtcons_opfunc_f **opfunc,
  void **arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_tag(								/* get nth tag						*/
  dt_ctl_t *ctl,
  int nth,
  const char **tag,
  int *iscurr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_find(							/* find tag, error if not found	    */
  dt_ctl_t *ctl,
  const char *tag,
  dtcons_opfunc_f **opfunc,
  void **arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_swap(
  dt_ctl_t *ctl,
  int i)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_goto(
  dt_ctl_t *ctl,
  dtcons_opfunc_f *opfunc,
  void *arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_jump(
  dt_ctl_t *ctl,
  int nth)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_del(
  dt_ctl_t *ctl,
  dtcons_opfunc_f *opfunc,
  void *arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_input(
  dt_ctl_t *ctl,
  dt_key_t *key)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcons_update(							/* update all consoles			    */
  dt_ctl_t *ctl,
  const char *status,
  dt_key_t *key)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
