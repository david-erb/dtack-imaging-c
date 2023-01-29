/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef __cplusplus
extern "C" {
#endif
#ifndef DTDEPEND
#include <stdio.h>						/* files common to ALL systems	    */
#include <math.h>
#include <string.h>
#if defined(DTCONFIG_SG1)
#	include <memory.h>
# endif
#endif

#if defined(DTCONFIG_WW0) || \
    defined(DTCONFIG_WW1) || \
    defined(DTCONFIG_WV0) || \
    defined(DTCONFIG_WV1) || \
    defined(DTCONFIG_WB0) || \
    defined(DTCONFIG_WB3)
#   define DT1D_API _pascal _export
#endif

#if defined(DTCONFIG_WV4)				/* Microsoft Visual C++ 4.0		  	*/
#   define DT1D_API __declspec( dllexport )
#endif

#ifndef DT1D_API
#   define DT1D_API
#endif

#define DT1D_CAST(TYPE, VAL) ((TYPE)(VAL))

#define DT1D_MIN(A, B) \
  (((A)<(B))?(A):(B))
#define DT1D_MAX(A, B) \
  (((A)>(B))?(A):(B))

extern unsigned int *dt1d_square_table;	/* table[-256..255] of squares		*/

typedef enum {							/* return codes						*/
  DT1D_RC_BAD = -1,
  DT1D_RC_GOOD = 0
} dt1d_rc_e;

#define DT1D_NTYPE_NULL     DTNTYPE_NULL 
#define DT1D_NTYPE_UNKNOWN  DTNTYPE_UNKNOWN 
#define DT1D_NTYPE_B01  DTNTYPE_B01 
#define DT1D_NTYPE_B08  DTNTYPE_B08 
#define DT1D_NTYPE_B16  DTNTYPE_B16 
#define DT1D_NTYPE_B32  DTNTYPE_B32 
#define DT1D_NTYPE_S1   DTNTYPE_S1  
#define DT1D_NTYPE_S2   DTNTYPE_S2  
#define DT1D_NTYPE_S4   DTNTYPE_S4  
#define DT1D_NTYPE_F1   DTNTYPE_F1  
#define DT1D_NTYPE_F2   DTNTYPE_F2  

typedef dtntype_e dt1d_ntype_e;

typedef dtntype_b01_t dt1d_ntype_b01_t;
typedef dtntype_b08_t dt1d_ntype_b08_t;
typedef dtntype_b16_t dt1d_ntype_b16_t;
typedef dtntype_b32_t dt1d_ntype_b32_t;
typedef dtntype_s1_t  dt1d_ntype_s1_t;
typedef dtntype_s2_t  dt1d_ntype_s2_t;
typedef dtntype_s4_t  dt1d_ntype_s4_t;
typedef dtntype_f1_t  dt1d_ntype_f1_t;
typedef dtntype_f2_t  dt1d_ntype_f2_t;

#define DT1D_NTYPE_PAIR(A, B)			/* combine two ntypes into one case	*/\
  ((A)*100+(B))

#define DT1D_NTYPE_F1_MINVAL (-1e38)
#define DT1D_NTYPE_F1_MAXVAL (1e38)
#define DT1D_NTYPE_F2_MINVAL (-1e300)
#define DT1D_NTYPE_F2_MAXVAL (1e300)

extern 
const dt1d_ntype_e *dt1d_ntype_list;	/* list of all valid ntypes			*/
extern 
const double *dt1d_ntype_sizeof;		/* array turning ntype_e to sizeof	*/
extern 
const double *dt1d_ntype_minval;		/* array turning ntype_e to minval  */
extern 
const double *dt1d_ntype_maxval;		/* array turning ntype_e to maxval  */


typedef int dt1d_i_t;

typedef int dt1d_compare_f(
  void *s1, 
  void *s2, 
  unsigned int result);

typedef struct {
  char priv[1024];
} dt1d_ran_uniform_t;

typedef struct {
  double next;
  int second;
} dt1d_ran_gauss_t;

typedef struct {
  dt1d_ran_uniform_t uniform;
  dt1d_ran_gauss_t gauss;
} dt1d_ran_t;


#include <dtack/1d.p>

										/* ................................ */
#define DT1D_B_BODIES \
DT1D_BODY(DT1D_NTYPE_B08, dt1d_ntype_b08_t, uchar) \
DT1D_BODY(DT1D_NTYPE_B16, dt1d_ntype_b16_t, ushort) \
DT1D_BODY(DT1D_NTYPE_B32, dt1d_ntype_b32_t, uint32)
#define DT1D_S_BODIES \
DT1D_BODY(DT1D_NTYPE_S1,  dt1d_ntype_s1_t,  char) \
DT1D_BODY(DT1D_NTYPE_S2,  dt1d_ntype_s2_t,  short) \
DT1D_BODY(DT1D_NTYPE_S4,  dt1d_ntype_s4_t,  sint32)
#define DT1D_F_BODIES \
DT1D_BODY(DT1D_NTYPE_F1,  dt1d_ntype_f1_t,  float) \
DT1D_BODY(DT1D_NTYPE_F2,  dt1d_ntype_f2_t,  double)

#define DT1D_BIG_BODIES \
DT1D_BODY(DT1D_NTYPE_B32, dt1d_ntype_b32_t, uint32) \
DT1D_BODY(DT1D_NTYPE_S4,  dt1d_ntype_s4_t,  sint32) \
DT1D_BODY(DT1D_NTYPE_F1,  dt1d_ntype_f1_t,  float) \
DT1D_BODY(DT1D_NTYPE_F2,  dt1d_ntype_f2_t,  double)

#define DT1D_INTEGER_BODIES \
  DT1D_B_BODIES \
  DT1D_S_BODIES
#define DT1D_FLOATING_BODIES \
  DT1D_F_BODIES

#define DT1D_UNSIGNED_BODIES \
  DT1D_B_BODIES
#define DT1D_SIGNED_BODIES \
  DT1D_S_BODIES \
  DT1D_F_BODIES

#define DT1D_ALL_BODIES \
  DT1D_B_BODIES \
  DT1D_S_BODIES \
  DT1D_F_BODIES
										/* ................................ */
#define DT1D_BODY(NTYPE, DTYPE, NAME) \
extern  \
dt1d_rc_e DT1D_API dt1d_clip_ ## NAME( \
  DTYPE * v, \
  dt1d_i_t nv, \
  const DTYPE vmin, \
  const DTYPE vmax);
DT1D_ALL_BODIES							/* clip declarations 				*/
#undef DT1D_BODY
										/* ................................ */
#define DT1D_BODY(NTYPE, DTYPE, NAME) \
extern  \
dt1d_rc_e DT1D_API dt1d_abs_ ## NAME( \
  DTYPE * v, \
  dt1d_i_t nv);
DT1D_SIGNED_BODIES						/* abs declarations 				*/
#undef DT1D_BODY
										/* ................................ */
#define DT1D__OP(DTYPE) \
  const DTYPE * in1,														  \
  dt1d_i_t nin1,															  \
  const DTYPE c1,															  \
  const DTYPE * in2,														  \
  dt1d_i_t nin2,															  \
  const DTYPE c2,															  \
  const DTYPE c3,															  \
  DTYPE * out,																  \
  dt1d_i_t nout
										/* ................................ */
#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
extern																		  \
dt1d_rc_e DT1D_API dt1d_cumf_ ## NAME( 										  \
  const DTYPE * vin, 														  \
  dt1d_i_t nin,                                                               \
  DTYPE * vout,																  \
  dt1d_i_t nout);
DT1D_BIG_BODIES							/* cumf declarations 				*/
#undef DT1D_BODY
										/* ................................ */
#define DT1D_BODY(NTYPE, DTYPE, NAME) 										  \
extern																		  \
dt1d_rc_e DT1D_API dt1d_cumr_ ## NAME( 										  \
  const DTYPE * vin, 														  \
  dt1d_i_t nin,                                                               \
  DTYPE * vout,																  \
  dt1d_i_t nout);
DT1D_BIG_BODIES							/* cumr declarations 				*/
#undef DT1D_BODY
										/* ................................ */
#ifdef __cplusplus
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
