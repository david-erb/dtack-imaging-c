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

/*----in text.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_beg(								/* begin text operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text,						/* returned text object description	*/
  const char *spec)						/* specification string 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_end(								/* end text operations				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text)						/* text object description 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_out(								/* output text at position			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text,						/* text object description 			*/
  const char *string,					/* string to be output 				*/
  int x,								/* output column position 			*/
  int y,								/* output row position 				*/
  unsigned long flags)					/* text appearance control flags 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_clear(							/* clear screen						*/
  dt_ctl_t *ctl,						/* environment control				*/
  dttext_t *text)						/* text object description 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_key(								/* take keyboard input				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dttext_t *text,						/* text object description 			*/
  int *iskey,							/* returned true if key hit 		*/
  dt_key_t *key)						/* returned key value 				*/
DX_SUFFIX

/*----in textcur1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_curses_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *init)
DX_SUFFIX

/*----in texttcap.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_tcap_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *init)
DX_SUFFIX

/*----in textdos1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_int10_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *init)
DX_SUFFIX

/*----in textqnx1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dttext_qnxterm_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *spec)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
