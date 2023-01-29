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

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtheap_check(							/* check heap integrity 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  const char *caller)					/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtheap_set_limit(						/* set artificial malloc limit	    */
  dt_ctl_t *ctl,
  void *vheap,
  long allocs_more,
  long bytes_more)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtheap_track_alloc(						/* track a malloc just done			*/
  dt_ctl_t *ctl,
  void *vheap,
  void *ptr,
  const long size,
  const char *who,
  const char *why)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtheap_track_free(						/* track a heap free operation 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  void *ptr)							/* block being freed 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtheap_init(							/* commence heap tracking 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void **vheap,							/* heap structure 					*/
  int count)							/* initial table entry count 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtheap_uninit(							/* quit heap tracking 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap)							/* heap structure 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtheap_outstanding(						/* return outstanding heap info 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  long *blocks,							/* outstanding blocks 				*/
  long *bytes)							/* outstanding bytes 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtheap_report(							/* produce heap report 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  void *vheap,							/* heap structure 					*/
  const char *caller)					/* caller ID						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtheap_check_limit(
  dt_ctl_t *ctl,
  void *vheap,
  long size,
  const char *caller)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtheap_avail(
  dt_ctl_t *ctl,
  void *vheap,
  unsigned long chunk_size,
  unsigned long maxalloc,
  unsigned long *nchunks)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtheap_avail_say(
  dt_ctl_t *ctl,
  void *vheap,
  unsigned long chunk_size,
  const char *caller)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
