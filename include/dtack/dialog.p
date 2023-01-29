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

/*----in dialog.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_init2(							/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_t *existing,
  dtdialog_init_f *init,
  char *subspec,
  const char *title,
  unsigned long flags,
  const char *keyboard)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_init(							/* initialize dialog structure		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdialog_t *dialog,					/* dialog object description 		*/
  dtdialog_t *existing,					/* existing dialog object 			*/
  const char *spec,						/* dialog specification string 		*/
  const char *title,					/* dialog screen title 				*/
  unsigned long flags,					/* dialog appearance flags 			*/
  const char *keyboard)					/* keyboard description string 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_uninit(						/* uninit structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_field(							/* define field						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x,
  int y,
  const char *name,
  char *contents,
  int lmax)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_button(						/* define button					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  char *label,							/* button's label				    */
  int pos,								/* horizontal relative position	    */
  dt_key_t key)							/* key-equivalent to return		    */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_input(							/* take input						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_field_t **start,
  dt_key_t *key)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_hide(							/* hide screen						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_twirl(							/* twirl screen						*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int x, 
  int y,
  int flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_key(					   
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  int *iskey,
  dt_key_t *key)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_help(					   
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  const char *filename,
  const char *topic,
  int flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_transit(
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dt_key_t key,
  dtdialog_field_t *oldfield,
  dtdialog_field_t **newfield)
DX_SUFFIX

/*----in diatext.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_text_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_t *existing,
  char *init,
  const char *keyboard)
DX_SUFFIX

/*----in diavba.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdialog_vba_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtdialog_t *dialog,
  dtdialog_t *existing,
  char *init,
  const char *keyboard)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
