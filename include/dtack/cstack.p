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

/*----in cstack.c:----*/


DX_PREFIX
void
DTCONFIG_API1
dtcstack_init(
  dt_ctl_t *ctl)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtcstack_mark(
  dt_ctl_t *ctl)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtcstack_mark2(
  dt_ctl_t *ctl,
  const char *F)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtcstack_query(
  dt_ctl_t *ctl,
  unsigned long *pstack1,
  unsigned long *pstack_highwater)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtcstack_dbg(
  dt_ctl_t *ctl,
  const char *F,
  long mask)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtcstack_highwater(
  dt_ctl_t *ctl,
  const char *F)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dtcstack_say(
  dt_ctl_t *ctl,
  const char *F,
  long mask)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
