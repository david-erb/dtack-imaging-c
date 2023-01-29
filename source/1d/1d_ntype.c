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


#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

static dt1d_ntype_e list[] = {
  DT1D_NTYPE_B01,
  DT1D_NTYPE_B08,
  DT1D_NTYPE_B16,
  DT1D_NTYPE_B32,
  DT1D_NTYPE_S1,
  DT1D_NTYPE_S2,
  DT1D_NTYPE_S4,
  DT1D_NTYPE_F1,
  DT1D_NTYPE_F2,
  DT1D_NTYPE_NULL
};

static double size[] = {				/* sizes in bytes for ntypes 		*/
  0,									/* UNKNOWN */
  1.0/8.0,
  (double)sizeof(dt1d_ntype_b08_t),		/* B08 */
  (double)sizeof(dt1d_ntype_b16_t),		/* B16 */
  (double)sizeof(dt1d_ntype_b32_t),		/* B32 */
  (double)sizeof(dt1d_ntype_s1_t),		/* S1 */
  (double)sizeof(dt1d_ntype_s2_t),		/* S2 */
  (double)sizeof(dt1d_ntype_s4_t),		/* S4 */
  (double)sizeof(dt1d_ntype_f1_t),		/* F1 */
  (double)sizeof(dt1d_ntype_f2_t)		/* F2 */
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
  DT1D_NTYPE_F1_MINVAL,					/* F1 */
  DT1D_NTYPE_F2_MINVAL					/* F2 */
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
  (double)DT1D_NTYPE_F1_MAXVAL,			/* F1 */
  (double)DT1D_NTYPE_F2_MAXVAL			/* F2 */
};

const dt1d_ntype_e *dt1d_ntype_list = list;
const double *dt1d_ntype_sizeof = size;
const double *dt1d_ntype_minval = minval;
const double *dt1d_ntype_maxval = maxval;






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
