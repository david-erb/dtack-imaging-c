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

/*----in os_delay.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_delay(
  dt_ctl_t *ctl,
  long milliseconds)
DX_SUFFIX

/*----in os_futil.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_tmpnam(
  dt_ctl_t *ctl,
  char *string,
  int l)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_unlink(							/* delete file by given name	    */
  dt_ctl_t *ctl,
  char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_unlink(
  dt_ctl_t *ctl,
  char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_isreadable(						/* see if filename is readable		*/
  dt_ctl_t *ctl,
  const char *filename,
  int *isreadable)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_isreadable(						/* see if filename is readable		*/
  dt_ctl_t *ctl,
  const char *filename,
  int *isreadable)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_isdir(								/* see if cifname is a directory	*/
  dt_ctl_t *ctl,
  const char *dirname,
  int *isdir)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_isatty_file(						/* see if file is a file or not		*/
  dt_ctl_t *ctl,
  FILE *file,
  int *is)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_isatty_fd(							/* see if fd is a file or not		*/
  dt_ctl_t *ctl,
  int fd,
  int *is)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_futil_mtime(						/* return file's last mod time		*/
  dt_ctl_t *ctl,
  const char *filename,
  long *mtime)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_futil_size(						/* return file's size in bytes		*/
  dt_ctl_t *ctl,
  const char *filename,
  long *size)
DX_SUFFIX

/*----in os_heap.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_heap_integrity(					/* check heap integrity 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller)					/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_heap_fill(							/* fill heap 						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long chunk_size,				/* size of chunk to fill with		*/
  unsigned long maxalloc,				/* maximum size to fill to 			*/
  unsigned long *nchunks)				/* returned chunks to fill heap		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_heap_fill_say(						/* fill heap, say results 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long chunk_size,				/* chunk size to fill with 			*/
  const char *F)						/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_heap_option(						/* set heap option					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtos_heap_option_e option)			/* heap option desired 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_heap_table_init(
  dt_ctl_t *ctl,
  dtos_heap_table_t **table,
  int count)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_heap_table_reinit(
  dt_ctl_t *ctl,
  dtos_heap_table_t **table,
  int *count,
  int increment)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_heap_whowhy_dbg(					/* print out who/why heap table		*/
  dt_ctl_t *ctl,
  dtos_heap_table_t *table,
  int tablesize,
  const char *f,
  const char *desc)
DX_SUFFIX

/*----in os_heap2.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_heap_table_dbg(					/* print out a heap table 			*/
  dt_ctl_t *ctl,
  dtos_heap_table_t *table,
  int tablesize,
  const char *f,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_heap_walk(							/* debug while walking heap 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  int type,								/* type of walk to take 			*/
  const char *caller)					/* caller ID 						*/
DX_SUFFIX

/*----in os_mem.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_strdup(							/* duplicate string					*/
  dt_ctl_t *ctl,
  const char *src,
  char **dest)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_strdup3(							/* duplicate/contatenate strings	*/
  dt_ctl_t *ctl,
  const char *src1,
  const char *src2,
  const char *src3,
  char **dest)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_malloc3(							/* malloc space, NULL if fail		*/
  dt_ctl_t *ctl,
  void **ptr,
  const long size,
  const char *caller,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_malloc2(							/* malloc space, return in ptr		*/
  dt_ctl_t *ctl,
  void **ptr,
  const long size,
  const char *caller,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_malloc(							/* malloc space, return in ptr		*/
  dt_ctl_t *ctl,
  void **ptr,
  const long size)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_realloc3(							/* realloc space, NULL if fail		*/
  dt_ctl_t *ctl,
  void *oldptr,
  void **newptr,
  const long size,
  const char *caller,
  const char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_free(								/* free allocated space				*/
  dt_ctl_t *ctl,
  void *ptr)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtos_free2(								/* free allocated space				*/
  dt_ctl_t *ctl,
  void *ptr,
  const char *caller,
  const char *desc)
DX_SUFFIX

/*----in os_time.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time_delay(						/* delay							*/
  dt_ctl_t *ctl,
  dtos_time_t *since,					/* event time 						*/
  long milli)							/* total delay needed since event 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time_str1(							/* print date/time as string		*/
  dt_ctl_t *ctl,
  long date,							/* date as yyyymmdd 				*/
  long time,							/* time as hhmmss 					*/
  char *string,							/* returned string 					*/
  int max)								/* max length of string 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time_report(
  dt_ctl_t *ctl,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  const char *f)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time_stamp(						/* make date/time stamp strings		*/
  dt_ctl_t *ctl,
  char *date_stamp,
  char *time_stamp)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_time(								/* store current time			    */
  dt_ctl_t *ctl,
  dtos_time_t *mark)
DX_SUFFIX

/*----in os_env.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_getpid(
  dt_ctl_t *ctl,
  int *pid)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtos_getenv(
  dt_ctl_t *ctl,
  const char *var,
  char *val,
  int max,
  int *len)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
