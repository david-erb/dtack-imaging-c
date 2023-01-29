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

/*----in keymap.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_init(							/* initialize keymap 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  const char *spec,						/* initial keymap spec 				*/
  unsigned long flags)					/* control flags 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_uninit(						/* release keymap 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap)					/* keymap object descriptor 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_map(							/* add single key mapping			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  dt_key_t key,							/* key value 						*/
  const char *string,					/* raw bytes 						*/
  long flags)							/* control flags 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_builtin(						/* add predefined key mappings		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  const char *name,						/* name of predefined mapping set 	*/
  long flags)							/* control flags 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_match(							/* look up key value 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  char *b,								/* raw bytes buffer					*/
  int n,								/* number of raw bytes 				*/
  int *r,								/* returned characters remaining	*/
  dt_key_t *key,						/* returned key value 				*/
  int *ispartial)						/* returned true if partial match 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_name_str(						/* make string for key value 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  dt_key_t key,							/* the key value 					*/
  char *string,							/* return string 					*/
  int lmax)								/* maximum length of string 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_maps_str(						/* make string of mappings 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  dt_key_t key,							/* key value 						*/
  char *string,							/* returned string 					*/
  int lmax)								/* maximum length of string 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_parse(							/* parse keymap spec 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  const char *spec,						/* spec to parse 					*/
  char *message,						/* returned diagnostic message 		*/
  int message_max)						/* max length of message 			*/
DX_SUFFIX

/*----in keymapt.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_termcap_init(					/* init keymap structure			*/
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  const char *spec,					  
  unsigned long flags)
DX_SUFFIX

/*----in keymapq.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_qnxterm_init(					
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  const char *spec,					  
  unsigned long flags)
DX_SUFFIX

/*----in keymapc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtkeymap_ctlkeys_init(					
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  const char *spec,					  
  unsigned long flags)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
