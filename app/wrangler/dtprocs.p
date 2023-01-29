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

/*----in procsw.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtprocs_init(							/* instantiate dtprocs object 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  const char *listname)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtprocs_uninit(							/* release dtprocs object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs)						/* dtprocs object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtprocs_proc_describe(					/* make descriptive string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  dtprocs_proc_t *proc,
  char *string,
  int lmax)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtprocs_start(							/* start process					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  const char *name,
  const char *executable,
  const char *args,
  int inhandle,
  int outhandle,
  dtprocs_goodbye_f *goodbye,
  void *goodbye_arg,
  dtprocs_proc_t **pproc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtprocs_poll(							/* poll state of children 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs)						/* dtprocs object 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtprocs_reap(							/* reap first dead process			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtprocs_t *procs,						/* dtprocs object 					*/
  dtprocs_proc_t **proc)
DX_SUFFIX
#undef DX_SUFFIX
#undef DX_PREFIX
#ifdef __cplusplus
}
#endif
