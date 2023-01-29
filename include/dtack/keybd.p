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

/*----in keybd.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_init(							/* initialize keyboard 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeybd_t *keybd,						/* keyboard object descriptor 		*/
  const char *spec,						/* keyboard specification 			*/
  long flags)							/* control flags 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_uninit(							/* close keyboard 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeybd_t *keybd)						/* keyboard object descriptor 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_read(							/* read keyboard				    */
  dt_ctl_t *ctl,						/* environment control			    */
  dtkeybd_t *keybd,						/* keyboard object descriptor 		*/
  dt_key_t *key)						/* returned key value			    */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_scan(							/* poll keyboard				    */
  dt_ctl_t *ctl,						/* environment control			    */
  dtkeybd_t *keybd,						/* keyboard descriptor object		*/
  int *iskey,							/* returned true if key available 	*/
  dt_key_t *key)						/* returned key value if available  */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_flush(							/* flush keyboard input			    */
  dt_ctl_t *ctl,						/* environment control			    */
  dtkeybd_t *keybd)						/* keyboard descriptor object	    */
DX_SUFFIX

/*----in keyqnx.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_qnxterm_init(						/* set up keyboard					*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  const char *spec,
  long flags)
DX_SUFFIX

/*----in keyunix.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_unix_init(						/* set up keyboard					*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  const char *spec,
  long flags)
DX_SUFFIX

/*----in keydos.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeybd_dos_init(						/* set up keyboard					*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  const char *spec,
  long flags)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
