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

/*----in ipc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_register(							/* register service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long recv_max,
  unsigned long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_unregister(						/* unregister service name			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_wait(								/* wait for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_poll(								/* poll for request					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  dtipc_poll_e what,
  long timeout,
  const char *waitingfor,
  int *isready)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_isipc(							/* see if it's one of ours			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  unsigned long id,
  dtipc_header_t *h,
  int *isipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_find(								/* find registered service name		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const char *name,
  long recv_max,
  unsigned long flags,
  long timeout)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_split(							/* split server/client ipc			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc1,
  dtipc_t *ipc2)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_unfind(							/* unfind registered service name	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_done(								/* server is done with connection   */
  dt_ctl_t *ctl,
  dtipc_t *ipc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_send(								/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_send2(							/* send data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length,
  long *sent)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_write_some(						/* write data on connection			*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  const void *data,
  long length,
  long *wrote)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_write_all2(						/* write all data with timeout		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  long timeout,							/* max total time to wait 			*/
  const void *buf,						/* buffer holding data 				*/
  long want)							/* number of bytes to write 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_write_all(						/* write all data to file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  const void *buf,						/* buffer holding data 				*/
  long want)							/* number of bytes to write 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_recv(								/* receive data, known length		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_recvt(							/* receive, known length, timeout	*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long length,
  long timeout)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_recv2(							/* receive data, unknown length		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void **data,
  long *length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_read_some(						/* receive data on connection		*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  void *data,
  long maxlength,
  long *length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_read_all(							/* fill buffer from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long want)							/* maximum number of bytes wanted 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_read_line2(						/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  long timeout,							/* timeout 							*/
  void *buf,							/* buffer to return bytes in 		*/
  long max)								/* maximum size of buffer 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_read_line(						/* read file up to new line			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtipc_t *ipc,							/* file object descriptor 			*/
  void *buf,							/* buffer to return bytes in 		*/
  long max)								/* maximum size of buffer 			*/
DX_SUFFIX

/*----in ipcqnx4.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_qnx4_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_qnx4_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
DX_SUFFIX

/*----in ipcqbaby.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_qbaby_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
DX_SUFFIX

/*----in ipcrock.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_rocket_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_rocket_rev(						/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/*----in ipcdde.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_dde_init(							/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
  unsigned long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtipc_dde_init(						/* init structure					*/
  dt_ctl_t *ctl,
  dtipc_t *ipc,
  long recv_max,
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
