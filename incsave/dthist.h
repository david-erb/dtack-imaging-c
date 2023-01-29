/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTHIST_FREQ2_NTYPE DTNTYPE_B32
typedef dtntype_b32_t dthist_freq2_t;

typedef unsigned int dthist_bin_t;		/* proper way to index freq array 	*/

#define DTHIST_BIN_FORMAT "u"
#define DTHIST_FREQ_FORMAT "lu"

extern dt_ntype_e *dthist_ntype_list;	/* allowed data types 				*/

typedef enum {
  DTHIST_TYPE_DIRECT,					/* value maps to bin directly 		*/
  DTHIST_TYPE_OFFSET,					/* value simply offset to bin 		*/
  DTHIST_TYPE_SCALED					/* value offset/scaled to bin 		*/
} dthist_type_e;

typedef struct {
  double minval;
  double maxval;
  double dscale;						/* scale data to bin 				*/
  void *freq;							/* frequency array 					*/
  dtntype_e ntype;
  dthist_bin_t width;
  int ioffset;
  dthist_type_e type;					/* type of mapping to do 			*/
} dthist_t;

#include <dtack/hist.p>

#define DTHIST_BIN_TO_MINVALUE(CHECK, CTL, HIST, BIN, VALUE) \
{ \
  *(VALUE) = (BIN) / (HIST)->dscale + (HIST)->minval; \
}
#define DTHIST_BIN_TO_MAXVALUE(CHECK, CTL, HIST, BIN, VALUE) \
{ \
  *(VALUE) = ((BIN)+1) / (HIST)->dscale + (HIST)->minval; \
}

#define DTHIST_VALUE_TO_BIN(CHECK, CTL, HIST, VALUE, BIN) \
{ \
  *(BIN) = ((VALUE) <  (HIST)->minval)? 0 : ( \
           ((VALUE) >= (HIST)->maxval)? (HIST)->width-1 : ( \
           (dthist_bin_t)(((VALUE)-(HIST)->minval) * (HIST)->dscale))); \
}






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
