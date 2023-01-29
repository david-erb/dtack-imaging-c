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

/*----in 1d.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_err_ntype(							/* error because can't do ntype		*/
  dt1d_ntype_e ntype,
  const dt1d_ntype_e *ntypes)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_err_length(						/* error because length is bad		*/
  dt1d_i_t length)
DX_SUFFIX

/*----in 1d_abs.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_abs(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv)
DX_SUFFIX

/*----in 1d_arith.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_add(								/* vector add 						*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant offset 					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sub(								/* vector subtract					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant offset 					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_mult(								/* vector multiplication 			*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant factor 					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_div(								/* vector division					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const double a,						/* first factor 					*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double b,						/* second factor 					*/
  const double c,						/* constant divisor					*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

/*----in 1d_ran.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_ran_uniform(
  dt1d_ran_t *ran,
  double *x)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_ran_uniform_init(
  dt1d_ran_t *ran,
  long seed)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_ran_gauss(
  dt1d_ran_t *ran,
  double *x)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_ran_gauss_init(
  dt1d_ran_t *ran,
  long seed)
DX_SUFFIX

/*----in 1d_range.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_uchar(
  const unsigned char * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_ushort(
  const unsigned short * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_uint32(
  const dtntype_b32_t * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_char(
  const signed char * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_short(
  const short * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_sint32(
  const dtntype_s4_t * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_float(
  const float * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_double(
  const double * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range_int(
  const int * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_range(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv,
  dt1d_i_t *vimin,
  dt1d_i_t *vimax)
DX_SUFFIX

/*----in 1d_cast.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_cast(
  void *vin,
  dt1d_i_t nin,
  dt1d_ntype_e in_ntype,
  void *vout,
  dt1d_i_t nout,
  dt1d_ntype_e out_ntype)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_cast2(
  void *vin,
  dt1d_i_t offin,
  dt1d_i_t incin,
  dt1d_i_t nin,
  dt1d_ntype_e in_ntype,
  void *vout,
  dt1d_i_t offout,
  dt1d_i_t incout,
  dt1d_i_t nout,
  dt1d_ntype_e out_ntype)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_cmp(								/* vector compare					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double epsilon,					/* maximum allowable difference		*/
  long *cmp)							/* returned difference location 	*/
DX_SUFFIX

/*----in 1d_diff.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_diff(								/* vector difference count			*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in1,						/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  const void * in2,						/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  const double epsilon,					/* maximum allowable difference		*/
  long *nless,							/* returned count in1 less than in2	*/
  long *nmore)							/* returned count in1 more than in2 */
DX_SUFFIX

/*----in 1d_fft.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_fft_complex(						/* compute 1D FFT					*/
  dt1d_ntype_f2_t *vector,
  dt1d_i_t n,
  int forward)
DX_SUFFIX

/*----in 1d_fftw.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_fftw(								/* compute 1D FFT					*/
  dt1d_ntype_f2_t *vector,
  dt1d_i_t n,
  int forward)
DX_SUFFIX

/*----in 1d_set.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_uchar(
  unsigned char * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_ushort(
  unsigned short * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_uint32(
  dtntype_b32_t * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_char(
  signed char * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_short(
  signed short * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_sint32(
  dtntype_s4_t * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_float(
  float * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set_double(
  double * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_set(
  dt1d_ntype_e ntype,
  void * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value)
DX_SUFFIX

/*----in 1d_stats.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_stats(								/* compute blob statistics 			*/
  dt1d_ntype_e ntype,					/* type of elements 				*/
  void *in,								/* input array	 					*/
  dt1d_i_t nin,							/* number of elements 				*/
  dtstats_t *stats)					/* return statistics 				*/
DX_SUFFIX

/*----in 1d_sum.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sum(								/* vector sum					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in,						/* input vector 					*/
  dt1d_i_t nin,							/* length of input vector 			*/
  double *sum)							/* returned sum				*/
DX_SUFFIX

/*----in 1d_sumsq.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_uchar(
  const unsigned char * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_ushort(
  const unsigned short * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_uint32(
  const dtntype_b32_t * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_char(
  const signed char * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_short(
  const signed short * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_sint32(
  const dtntype_s4_t * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_float(
  const float * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_double(
  const double * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq(
  dt1d_ntype_e ntype,
  const void * const in,
  const dt1d_i_t nin,
  double *sum)
DX_SUFFIX

/*----in 1d_sumsd.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_uchar(
  const unsigned char * const v1,
  const dt1d_i_t nv1,
  const unsigned char * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_ushort(
  const unsigned short * const v1,
  const dt1d_i_t nv1,
  const unsigned short * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_uint32(
  const dtntype_b32_t * const v1,
  const dt1d_i_t nv1,
  const dtntype_b32_t * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_char(
  const signed char * const v1,
  const dt1d_i_t nv1,
  const signed char * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_short(
  const signed short * const v1,
  const dt1d_i_t nv1,
  const signed short * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_sint32(
  const dtntype_s4_t * const v1,
  const dt1d_i_t nv1,
  const dtntype_s4_t * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_float(
  const float * const v1,
  const dt1d_i_t nv1,
 const  float * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs_double(
  const double * const v1,
  const dt1d_i_t nv1,
  const double * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_diffs(
  dt1d_ntype_e ntype,
  const void * const v1,
  const dt1d_i_t nv1,
  const void * const v2,
  const dt1d_i_t nv2,
  double *sum)
DX_SUFFIX

/*----in 1d_sumss.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_uchar(
  const unsigned char * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_ushort(
  const unsigned short * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_uint32(
  const dtntype_b32_t * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_char(
  const signed char * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_short(
  const signed short * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_sint32(
  const dtntype_s4_t * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_float(
  const float * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar_double(
  const double * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sumsq_scalar(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv,
  double scalar,
  double *sum)
DX_SUFFIX

/*----in 1d_cumf.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_cumf(
  dt1d_ntype_e ntype,
  const void * vin,	
  dt1d_i_t nin,		
  void * vout,		
  dt1d_i_t nout)
DX_SUFFIX

/*----in 1d_cumr.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_cumr(
  dt1d_ntype_e ntype,
  const void * vin,	
  dt1d_i_t nin,		
  void * vout,		
  dt1d_i_t nout)
DX_SUFFIX

/*----in 1d_look.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_lookup_uint32(						/* lookup every member in table		*/
  dtntype_b32_t * const in,
  const dt1d_i_t nin,
  dtntype_b32_t * const out,
  const dt1d_i_t nout,
  const dtntype_b32_t * const table)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_lookup_sint32(						/* lookup every member in table		*/
  dtntype_s4_t * const in,
  const dt1d_i_t nin,
  dtntype_s4_t * const out,
  const dt1d_i_t nout,
  const dtntype_s4_t * const table)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_lookup_uchar(						/* lookup every member in table		*/
  unsigned char * const in,
  const dt1d_i_t nin,
  unsigned char * const out,
  const dt1d_i_t nout,
  const unsigned char * const table)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_lookup_inplace_uint32(				/* lookup every member in table		*/
  dtntype_b32_t * const v,
  const dt1d_i_t nv,
  const dtntype_b32_t * const table)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_lookup_inplace_sint32(				/* lookup every member in table		*/
  dtntype_s4_t * const v,
  const dt1d_i_t nv,
  const dtntype_s4_t * const table)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_lookup_inplace_uchar(				/* lookup every member in table		*/
  unsigned char * const v,
  const dt1d_i_t nv,
  const unsigned char * const table)
DX_SUFFIX

/*----in 1d_logic.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_and(
  dt1d_ntype_e ntype,
  const void * in1,	
  dt1d_i_t nin1,	
  const dtntype_b32_t a,	
  const void * in2,	
  dt1d_i_t nin2,	
  const dtntype_b32_t b,	
  const dtntype_b32_t c,	
  void * out,		
  dt1d_i_t nout)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_or(
  dt1d_ntype_e ntype,
  const void * in1,	
  dt1d_i_t nin1,	
  const dtntype_b32_t a,	
  const void * in2,	
  dt1d_i_t nin2,	
  const dtntype_b32_t b,	
  const dtntype_b32_t c,	
  void * out,		
  dt1d_i_t nout)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_xor(
  dt1d_ntype_e ntype,
  const void * in1,	
  dt1d_i_t nin1,	
  const dtntype_b32_t a,	
  const void * in2,	
  dt1d_i_t nin2,	
  const dtntype_b32_t b,	
  const dtntype_b32_t c,	
  void * out,		
  dt1d_i_t nout)
DX_SUFFIX

/*----in 1d_noise.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_random_uniform_double(
  dt1d_ran_t *ran,
  double *v,
  dt1d_i_t nv)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_random_gauss_double(
  dt1d_ran_t *ran,
  double *v,
  dt1d_i_t nv)
DX_SUFFIX

/*----in 1d_sort1.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sort(								/* vector in-place numerical sort	*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *v,
  dt1d_i_t nv)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sortp(								/* vector index sort				*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void **v,								/* index list 						*/
  dt1d_i_t nv)
DX_SUFFIX

/*----in 1d_sort5.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sort_struct(
  void *v,
  unsigned int size,
  dt1d_i_t nv,
  dt1d_compare_f *compare)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_sort_struct2(
  void *v,
  unsigned int size,
  dt1d_i_t nv,
  dt1d_compare2_f *compare,
  void *arg,							/* passthrough to compare func 		*/
  int ascending)
DX_SUFFIX

/*----in 1d_conv.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_convolve(							/* vector division					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void * in1,							/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  void * in2,							/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  void * out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_boxcarr(							/* vector division					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *in,
  dt1d_i_t nin,
  dt1d_i_t instride, 
  dt1d_i_t kernel_size,
  void *out,
  dt1d_i_t nout,	
  dt1d_i_t outstride)
DX_SUFFIX

/*----in 1d_clip.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_uchar(
  const double * const v1,
  const dt1d_i_t nv1,
  unsigned char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_ushort(
  const double * const v1,
  const dt1d_i_t nv1,
  unsigned short * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_uint32(
  const double * const v1,
  const dt1d_i_t nv1,
  dtntype_b32_t * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_char(
  const double * const v1,
  const dt1d_i_t nv1,
  signed char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_short(
  const double * const v1,
  const dt1d_i_t nv1,
  signed short * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_sint32(
  const double * const v1,
  const dt1d_i_t nv1,
  dtntype_s4_t * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_float(
  const double * const v1,
  const dt1d_i_t nv1,
  float * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_uint32_uchar(
  const dtntype_b32_t * const v1,
  const dt1d_i_t nv1,
  unsigned char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_double_double(
  const double * const v1,
  const dt1d_i_t nv1,
  double * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip_sint32_uchar(
  const dtntype_s4_t * const v1,
  const dt1d_i_t nv1,
  unsigned char * const v2,
  const dt1d_i_t nv2,
  double min,
  double max)
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_clip(
  dt1d_ntype_e ntype,
  const void * const v,
  const dt1d_i_t nv,
  double vmin,
  double vmax)
DX_SUFFIX

/*----in 1d_thres.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_thresh_scalar(						/* scalar threshold					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *in,								/* first input vector 				*/
  dt1d_i_t nin,							/* length of first input vector 	*/
  double scalar,						/* scalar threshold					*/
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  void *out,							/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_thresh_vector(						/* vector threshold					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  void *in1,							/* first input vector 				*/
  dt1d_i_t nin1,						/* length of first input vector 	*/
  void *in2,							/* second input vector 				*/
  dt1d_i_t nin2,						/* length of second input vector 	*/
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  const void *out,						/* output vector 					*/
  dt1d_i_t nout)						/* length of output vector 			*/
DX_SUFFIX

/*----in 1d_vari.c:----*/


DX_PREFIX
dt1d_rc_e
DT1D_API
dt1d_variance(							/* vector variance					*/
  dt1d_ntype_e ntype,					/* vector element type 				*/
  const void * in,						/* input vector 					*/
  dt1d_i_t nin,							/* length of input vector 			*/
  double *variance)						/* returned variance				*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
