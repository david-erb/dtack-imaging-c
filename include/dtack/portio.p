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

/*----in portio.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_init(							/* init portio operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  const char *spec)						/* bus spec 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_uninit(						/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio)					/* portio object descriptor 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_inb(							/* input byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to input from 				*/
  int *value)							/* returned byte value 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_outb(							/* output byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to output to 				*/
  int value)							/* value to output					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_init(							/* init portio operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  const char *spec)						/* bus spec 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_uninit(						/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio)					/* portio object descriptor 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_inb(							/* input byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to input from 				*/
  int *value)							/* returned byte value 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_outb(							/* output byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to output to 				*/
  int value)							/* value to output					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_init(							/* init portio operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  const char *spec)						/* bus spec 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_uninit(						/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio)					/* portio object descriptor 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_inb(							/* input byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to input from 				*/
  int *value)							/* returned byte value 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtportio_outb(							/* output byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to output to 				*/
  int value)							/* value to output					*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
