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

/*----in fd.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_open(								/* open a file						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *filename,					/* name of file to open 			*/
  const char *mode)						/* file open mode string 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_fd(								/* wrap object around existing fd	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *filename,					/* filename associated with fd 		*/
  int sysfd)							/* the system file descriptor 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_close(								/* close end keep file				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd)							/* file object descriptor 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_delete(							/* close and delete file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd)							/* file object descriptor 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_read_buffer(						/* copy buffer from input buffer	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to put data into			*/
  long want,							/* maximum number of bytes wanted 	*/
  long *got)							/* returned number of bytes gotten 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_read(								/* read buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf1,							/* buffer to put data into			*/
  long want1,							/* maximum number of bytes wanted 	*/
  long *got1)							/* returned number of bytes gotten 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_read_all(							/* fill buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long want)							/* maximum number of bytes wanted 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_read_line(							/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long max)								/* maximum size of buffer 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_skip(								/* skip bytes by reading			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  long want)							/* number of bytes wanted 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_write(								/* write buffer to file				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want,							/* number of bytes to write 		*/
  long *got)							/* returned number of bytes written */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_write_all(							/* write all data to file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want)							/* number of bytes to write 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_seek(								/* seek to position					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  long offset,							/* offset byte number 				*/
  dtfd_whence_e whence,					/* type of seek 					*/
  long *position)						/* returned absolute position 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_size(								/* return uncompressed file size	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  long *size)							/* returned uncompressed size		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_poll(								/* wait for fd to be active			*/
  dt_ctl_t *ctl,
  dtfd_t *fd,
  dtfd_poll_e what,
  long milliseconds,
  const char *waiting_for,
  int *isready)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtfd_rev(								/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
