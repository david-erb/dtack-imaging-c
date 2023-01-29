/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


/*..........................................................................
  CHANGES
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/


#include <dtack/base.h>

DT_RCSID("source/low $RCSfile: ntype_da.c,v $ $Revision: 1.1 $");

static dt_ntype_e list[] = {
  DT_NTYPE_B01,
  DT_NTYPE_B08,
  DT_NTYPE_B16,
  DT_NTYPE_B32,
  DT_NTYPE_S1,
  DT_NTYPE_S2,
  DT_NTYPE_S4,
  DT_NTYPE_F1,
  DT_NTYPE_F2,
  DT_NTYPE_NULL
};

static double size[] = {				/* sizes in bytes for ntypes 		*/
  0,									/* UNKNOWN */
  1.0/8.0,
  (double)sizeof(dt_ntype_b08_t),		/* B08 */
  (double)sizeof(dt_ntype_b16_t),		/* B16 */
  (double)sizeof(dt_ntype_b32_t),		/* B32 */
  (double)sizeof(dt_ntype_s1_t),		/* S1 */
  (double)sizeof(dt_ntype_s2_t),		/* S2 */
  (double)sizeof(dt_ntype_s4_t),		/* S4 */
  (double)sizeof(dt_ntype_f1_t),		/* F1 */
  (double)sizeof(dt_ntype_f2_t)			/* F2 */
};

static int bits[] = {					/* sizes in bits for ntypes			*/
  0,									/* UNKNOWN */
  1,									/* B01 */
  8,									/* B08 */
  16,									/* B16 */
  32,									/* B32 */
  8,									/* S1 */
  16,									/* S2 */
  32,									/* S4 */
  32,									/* F1 */
  64									/* F2 */
};

static double minval[] = {				/* min values for ntypes		  	*/
  0,									/* UNKNOWN */
  0,									/* B01 */
  0,									/* B08 */
  0,									/* B16 */
  0,									/* B32 */
  -128.0,								/* S1 */
  (double)((short)0x8000),				/* S2 */
  (double)((long)0x80000000L),			/* S4 */
  DT_NTYPE_F1_MINVAL,					/* F1 */
  DT_NTYPE_F2_MINVAL					/* F2 */
};

static double maxval[] = {				/* max values for ntypes		  	*/
  (double)0,							/* UNKNOWN */
  (double)0x1,							/* B01 */
  (double)0xff,							/* B08 */
  (double)0xffff,						/* B16 */
  (double)0xffffffffUL,					/* B32 */
  (double)0x7f,							/* S1 */
  (double)0x7fff,						/* S2 */
  (double)0x7fffffffL,					/* S4 */
  (double)DT_NTYPE_F1_MAXVAL,			/* F1 */
  (double)DT_NTYPE_F2_MAXVAL			/* F2 */
};

static dt_ntype_b01_t b01_max = (dt_ntype_b01_t)1;
static dt_ntype_b08_t b08_max = (dt_ntype_b08_t)0xff;	
static dt_ntype_b16_t b16_max = (dt_ntype_b16_t)0xffff;	
static dt_ntype_b32_t b32_max = (dt_ntype_b32_t)0xffffffffL;
static dt_ntype_s1_t  s1_max  = (dt_ntype_s1_t)0x7f;		
static dt_ntype_s2_t  s2_max  = (dt_ntype_s2_t)0x7fff;		
static dt_ntype_s4_t  s4_max  = (dt_ntype_s4_t)0x7fffffffL;
static dt_ntype_f1_t  f1_max  = (dt_ntype_f1_t)DT_NTYPE_F1_MAXVAL;		
static dt_ntype_f2_t  f2_max  = (dt_ntype_f2_t)DT_NTYPE_F2_MAXVAL;		

static const void *maxval_ptr[] = {		/* max values in native format		*/
  NULL,									/* UNKNOWN */
  (void *)&b01_max,
  (void *)&b08_max,
  (void *)&b16_max,
  (void *)&b32_max,
  (void *)&s1_max,
  (void *)&s2_max,
  (void *)&s4_max,
  (void *)&f1_max,
  (void *)&f2_max
};

static dt_ntype_e alias[] = {
  DT_NTYPE_B01,
  DT_NTYPE_B08,
  DT_NTYPE_B16,
  DT_NTYPE_B32,
  DT_NTYPE_S1,
  DT_NTYPE_S2,
  DT_NTYPE_S4,
  DT_NTYPE_F1,
  DT_NTYPE_F2,
  DT_NTYPE_B01,
  DT_NTYPE_B08,
  DT_NTYPE_B08,
  DT_NTYPE_B08,
  DT_NTYPE_B16,
  DT_NTYPE_B32,
  DT_NTYPE_S1,
  DT_NTYPE_S2,
  DT_NTYPE_S4,
  DT_NTYPE_F1,
  DT_NTYPE_F2,
  DT_NTYPE_NULL
};

static const char *string[] = {			/* descriptions for ntypes 			*/
  "unknown",							/* UNKNOWN */
  "b01",								/* B01 */
  "b08",								/* B08 */
  "b16",								/* B16 */
  "b32",								/* B32 */
  "s1",									/* I1 */
  "s2",									/* I2 */
  "s4",									/* I4 */
  "f1",									/* F1 */
  "f2",									/* F2 */
  "bit",								/* B01 */
  "uchar",								/* B08 */
  "byte",								/* B08 */
  "8bit",								/* B08 */
  "ushort",								/* B16 */
  "ulong",								/* B32 */
  "char",								/* I1 */
  "short",								/* I2 */
  "long",								/* I4 */
  "float",								/* F1 */
  "double",								/* F2 */
  NULL
};


const dt_ntype_e *dt_ntype_list = list;
const dt_ntype_e *dt_ntype_alias = alias;
const double *dt_ntype_sizeof = size;
const int *dt_ntype_bits = bits;
const double *dt_ntype_minval = minval;
const double *dt_ntype_maxval = maxval;
const void **dt_ntype_maxval_ptr = maxval_ptr;
const char **dt_ntype_string = string;

const int dt_ntype_count = sizeof(list) / sizeof(*list) - 1;





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
