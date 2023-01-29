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

/*----in interact.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_parse(					/* parse params from console args	*/
  int argc,
  char *argv[],
  const char *progname,
  const char *usage,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_leave(					/* clean up before leaving 			*/
  dtparam_t *param,
  dt_rc_e xrc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_interactive_ready(
  dt_ctl_t *ctl,
  const char *progname,
  int argc,
  char *argv[])
DX_SUFFIX

/*----in interdia.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_after(					/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char *usage,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct,
  dt_rc_e xrc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_dialog(					/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  int argc,
  char *argv[],
  const char *usage,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct)
DX_SUFFIX

/*----in interhlp.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_interactive_help(
  dt_ctl_t *ctl,
  const char *basename,
  const char *topic)
DX_SUFFIX

/*----in assert.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_assert_defined(
  dt_ctl_t *ctl,
  int isdefined,
  char *F,
  char *symbol,
  char *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API2
dt_assert(
  dt_ctl_t *ctl,						/* environment control 				*/
  int condition,						/* assertion truth condition 		*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like printf() 	*/
  ...)									/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_assert_va(
  dt_ctl_t *ctl,						/* environment control 				*/
  int condition,						/* assertion truth condition 		*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like printf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

/*----in dbg.c:----*/

	    void
DTCONFIG_API1
dt_dbg_cryptset(						/* set encryption password         */
  dt_ctl_t *ctl,
  const char *password,
  const char *reminder);
										/* use cryptout for all output */
void
DTCONFIG_API1
dt_dbg_cryptout(		
  int fd,
  const char *string,
  int length);


DX_PREFIX
void
DTCONFIG_API1
dt_dbg_string(							/* no prefix or substitutes			*/
  struct dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API2
dt_dbg(									/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dt_dbg_va(								/* output debug message, va_list	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const long class_bits,				/* debug message class 				*/
  const char *format,					/* message format, like vsprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

/*----in dbg2.c:----*/


DX_PREFIX
void
DTCONFIG_API2
dt_dbg_trace(							/* debug program location  			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *sourcefile,				/* caller's __FILE__ 				*/
  long sourceline,						/* caller's __LINE__ 				*/
  const char *format,					/* message format, like printf() 	*/
  ...)									/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dt_dbg_trace_va(						/* debug program location  			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *sourcefile,				/* caller's __FILE__ 				*/
  long sourceline,						/* caller's __LINE__ 				*/
  const char *format,					/* message format, like printf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_dbg_parse(							/* set dbg flags from dbg string	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to parse 					*/
  long *dbg_mask,						/* returned debug mask 				*/
  char *message,						/* returned error message 			*/
  int lmax)								/* maximum length of message 		*/
DX_SUFFIX

/*----in err.c:----*/


DX_PREFIX
void
DTCONFIG_API1
dt_err_string(							/* no prefix or substitutes			*/
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API2
dt_err(									/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_err_va(								/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like vsprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

/*----in err2.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_err_notlinked(
  dt_ctl_t *ctl,
  const char *caller,
  const char *desc)
DX_SUFFIX

/*----in enter.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_enter_newfield_callback(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  int x,
  int y,
  int labelwidth,
  int inputwidth)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_enter_param_callback(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  dtparam_flag_t *flag)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_enter_fields(
  dtparam_t *param,						/* parameter description structure 	*/
  dt_enter_arg_t *arg,					/* argument description structure 	*/
  dt_enter_newfield_f *newfield,
  void *newfield_arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_enter_feedback(
  dtparam_t *param,						/* parameter description structure 	*/
  const char *message,					/* parsing message 					*/
  dt_enter_arg_t *arg)					/* argument description structure 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_enter_standard_to(					/* handle errto, dbgto, sayto		*/
  dtparam_t *param)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_enter_param(							/* parse params from console args	*/
  int argc,
  char *argv[],
  const char *progname,
  const char *usage,
  dtparam_t *param,
  int *pn)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_enter_parse(							/* parse command line				*/
  int argc,								/* number of argv[] elements 		*/
  char *argv[],							/* array of string pointers			*/
  const char *progname,					/* program name 					*/
  const char *usage,					/* program usage description 		*/
  dtparam_t *param,						/* parameter description structure 	*/
  dt_enter_arg_t *arg,					/* argument description structure 	*/
  dt_enter_check_f *check,				/* caller's argument check function */
  void *appstruct)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API2
dt_enter_arg(							/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  ...)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_enter_arg_va(						/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  va_list ap)							/* variable argument list		    */
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API2
dt_enter_arg2(							/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  ...)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_enter_arg2_va(						/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  va_list ap)							/* variable argument list		    */
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_parse(					/* parse params from console args	*/
  int argc,
  char *argv[],
  const char *progname,
  const char *usage,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check, 
  void *appstruct)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_after(					/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char *usage,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct,
  dt_rc_e xrc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_interactive_leave(					/* clean up before leaving 			*/
  dtparam_t *param,
  dt_rc_e xrc)
DX_SUFFIX

/*----in leave.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_leave(								/* clean up before leaving 			*/
  dtparam_t *param)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dt_usage_say(							/* say usage string					*/
  dtparam_t *param,
  const char *string)
DX_SUFFIX

/*----in heapmon.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_heapmon_init(						/* begin heap monitoring		    */
  dt_ctl_t *ctl)						/* environment control 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_heapmon_uninit(						/* terminate heap monitoring	    */
  dt_ctl_t *ctl,
  int *assert_fail)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_heapmon_report(						/* report on heap usage			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller)					/* caller string for message 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_heapmon_check(						/* check heap integrity				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *caller)							/* caller string for messages 		*/
DX_SUFFIX

/*----in ntype.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtntype_lookup(
  dt_ctl_t *ctl,
  const char *string,
  dtntype_e *v,
  char *message,
  int max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtntype_to_string(
  dt_ctl_t *ctl,
  dtntype_e ntype,
  char *string,
  int max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtntype_sizeof(
  dt_ctl_t *ctl,
  dtntype_e ntype,
  double *size)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtntype_promote(
  dt_ctl_t *ctl,
  dtntype_e ntype1,
  dtntype_e ntype2,
  dtntype_e *ntype3,
  char *string,
  int max)
DX_SUFFIX

/*----in say.c:----*/


DX_PREFIX
void
DTCONFIG_API1
dt_say_string(							/* no prefix or substitutes			*/
  dt_ctl_t *ctl,
  const char *string,
  int length)
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API2
dt_say(									/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  ...)									/* format substitutions 			*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
dt_say_va(								/* output console message, va_list	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like vsprintf() 	*/
  va_list ap)							/* format substitutions 			*/
DX_SUFFIX

/*----in yield.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_yield_status(						/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *status)					/* format of yield message 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API2
dt_yield(								/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *format,					/* format of yield message 			*/
  ...)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_yield_va(							/* yield timeslice 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *format,					/* format of yield message 			*/
  va_list ap)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_yield_parse(							/* set yield flags from string		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to parse 					*/
  unsigned long *yield_flags,			/* returned yield flags				*/
  char *message,						/* returned error message 			*/
  int lmax)								/* maximum length of message 		*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
