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

/*----in param.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtparam_setflags(						/* set flags of all params			*/
  dtparam_t *param,						/* param structure to operate on 	*/
  dtparam_flag_t flag)					/* flag value to set on each param 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtparam_callback(						/* callback for each param			*/
  dtparam_t *param,						/* param structure to be traversed 	*/
  dtparam_callback_f *callback,			/* function to call for each param	*/
  void *arg)							/* arg passed through to callback 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtparam_parse(							/* parse params from argc/argv		*/
  dtparam_t *param,						/* parameter structure to use 		*/
  int argc,								/* number of argv elements 			*/
  char *argv[],							/* tokens from command line			*/
  int *nused,							/* returned number of tokens used 	*/
  char *message,						/* returned parse error message 	*/
  int lmax)								/* max length of message 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtparam_init(							/* init parameter structure			*/
  dtparam_t *param)						/* param structure to init 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtparam_free(							/* free space in param structure	*/
  dtparam_t *param)						/* param structure to operate on 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API2
dtparam_set(							/* mark keywords for parsing 		*/
  dtparam_t *param,						/* ~param~ structure to mark 		*/
  dtparam_flag_t flags,					/* flags for all keywords 			*/
  ...)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtparam_set_va(							/* mark keywords for parsing 		*/
  dtparam_t *param,						/* ~param~ structure to mark 		*/
  dtparam_flag_t flags,					/* flags for all keywords 			*/
  va_list ap)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
