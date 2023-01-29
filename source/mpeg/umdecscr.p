#ifdef __cplusplus
extern "C" {
#endif

/*----in umdecscr.c:----*/

dt_rc_e
DTCONFIG_API1
umdec_scr_create(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_scr_t *umdec_scr,	
  const char *scr_spec);
dt_rc_e
DTCONFIG_API1
umdec_scr_destroy(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_scr_t *umdec_scr);
dt_rc_e
DTCONFIG_API1
umdec_scr_callback(
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* umdec object 					*/
  void *arg,							/* caller's unchanged argument 		*/
  int frametype,						/* type of frame being given 		*/
  void **buffer,						/* buffers containing frame data	*/
  int xe,								/* frame width 						*/
  int ye)								/* frame height						*/;

/*----in fast.c:----*/

dt_rc_e 
DTCONFIG_API1
umdec_scr_fast_lookup(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dt_ntype_b08_t *table,
  dtimage_t *output);
dt_rc_e
DTCONFIG_API1
umdec_scr_fast_decimate(
  dt_ctl_t *ctl,
  unsigned char *s,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *d);

/*----in raw.c:----*/

dt_rc_e 
DTCONFIG_API1
umdec_scr_raw(
  dt_ctl_t *ctl,
  umdec_scr_t *umdec_scr,
  void **buffer,
  long xe,
  long ye);
#ifdef __cplusplus
}
#endif
