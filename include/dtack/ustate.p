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

/*----in ustatelib.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
ustate_init_config(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate,						/* ustate object 					*/
  const char *config_filename)			/* config file to read 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
ustate_search(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate,						/* ustate object 					*/
  const char *ident,					/* object's instance ident			*/
  const char *program,					/* program's name 					*/
  const char *attribute,				/* attribute name 					*/
  const char *dflt,						/* default value 					*/
  const char **value)					/* returned value 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
ustate_find(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate,						/* ustate object 					*/
  const char *ident,					/* object's instance ident			*/
  const char *program,					/* program's name 					*/
  const char *attribute,				/* attribute name 					*/
  const char *dflt,						/* default value 					*/
  const char **value)					/* returned value 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
ustate_uninit(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate)						/* ustate object 					*/
DX_SUFFIX

/*----in rev.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
ustate_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
