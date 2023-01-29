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

/*----in pnm.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtpnm_read_byte(						/* read a byte from a pnm file		*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd,
  const char *what,
  char *byte)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtpnm_read_double(						/* read ascii double from pnm file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd,
  const char *what,
  double *val)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtpnm_read_ulong(						/* read ascii long from a pnm file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd,
  const char *what,
  unsigned long *val)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtpnm_read_header(						/* read pnm header from opened file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtpnm_write_header(						/* write pnm header to opened file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
