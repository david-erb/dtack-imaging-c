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

/*----in iact.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtiact_buttons(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *status,
  dtiact_but_t *waitbut,
  dtiact_but_t *runbut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtiact_twirl(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *status)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtiact_cons_opfunc(
  dt_ctl_t *ctl,
  dtcons_op_e op,
  void *arg1,
  void *arg2)
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
dtiact_out_string_err( 
dt_ctl_t *ctl, 
const char *string, 
int length)
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
dtiact_out_string_dbg( 
dt_ctl_t *ctl, 
const char *string, 
int length)
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
dtiact_out_string_say( 
dt_ctl_t *ctl, 
const char *string, 
int length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtiact_newalpha(
  dt_ctl_t *ctl,
  dtalpha_t **palpha,
  char *tag)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
