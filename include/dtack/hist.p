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


/*----in hist.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthist_alloc2(							/* allocate space for histogram 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t width,					/* desired number of bins 			*/
  double minval,						/* data value at lowest bin 		*/
  double maxval,						/* data value above highest bin 	*/
  dtntype_e ntype)						/* histogram contents data type 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthist_alloc(							/* allocate space for histogram 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t width,					/* desired number of bins 			*/
  double minval,						/* data value at lowest bin 		*/
  double maxval)						/* data value above highest bin 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthist_free(							/* free space for histogram 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist)						/* histogram description structure 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthist_clear(							/* clear histogram counts 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist)						/* histogram description structure 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dthist_scale(							/* scale histogram counts			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  double height)						/* desired max height of bars  		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_bin_to_value(					/* get bin floor and ceiling 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t bin,						/* bin to query 					*/
  double *floor,						/* returned floor 					*/
  double *ceiling)						/* returned ceiling 				*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
DTHIST_BIN_TO_MINVALUE(					/* get bin floor   					*/
  dt_rc_check_f *check,					/* return code check function 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t bin,						/* bin to query 					*/
  double *floor)						/* returned floor 					*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
DTHIST_BIN_TO_MAXVALUE(					/* get bin ceiling 					*/
  dt_rc_check_f *check,					/* return code check function 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dthist_bin_t bin,						/* bin to query 					*/
  double *ceiling)		  				/* returned ceiling 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_value_to_bin(					/* get floor and ceiling of bin 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  double value,							/* value whose bin is desired 		*/
  dthist_bin_t *bin)					/* returned bin for value 			*/
DX_SUFFIX

DX_PREFIX
void
DTCONFIG_API1
DTHIST_VALUE_TO_BIN(					/* macro form 						*/
  dt_rc_check_f *check,					/* return code check function 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  double value,							/* value whose bin is desired 		*/
  dthist_bin_t *bin)					/* returned bin for value 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_bin_gaussian(
  dt_ctl_t *ctl,						/* environment control 				*/
  void *arg,
  double in,
  double *out)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_bin_uniform(
  dt_ctl_t *ctl,						/* environment control 				*/
  void *arg,
  double in,
  double *out)
DX_SUFFIX

/*----in hist2.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_add_values_fast(					/* accumulate frequencies fast		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  void *values,							/* array of input data values 		*/
  unsigned int beg,						/* start index						*/
  unsigned int end,						/* end index						*/
  dt_ntype_e ntype,						/* type of input data values 		*/
  int *cando)							/* return true if we did it 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_add_values(						/* accumulate frequencies 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  void *values,							/* array of input data values 		*/
  unsigned int beg,						/* start index						*/
  unsigned int end,						/* end index						*/
  dt_ntype_e ntype)						/* type of input data values 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_stats(							/* calculate statistics				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dtstats_t *stats)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dthist_gen(								/* generate histogram				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dthist_t *hist,						/* histogram description structure 	*/
  dt_double_f *double_func,
  void *arg)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
