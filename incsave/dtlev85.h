/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





typedef struct dtlev85_t {
  double Ua;							/* uniformity numerator accumulator	*/
  double fmax, fmin;					/* max data range found				*/
  double u;								/* computed uniformity 				*/
  double label;							/* associate label with uniformity 	*/
  dtparam_t *param;						/* params (for callback)		    */
  struct dtlev85_t *next;				/* so can link these together 		*/
  long Aa;								/* total area accumulator			*/
  long n;								/* number of regions			    */
} dtlev85_t;

typedef 
DTCONFIG_API1
dt_rc_e 
dtlev85_label_callback_f(				/* per-label callback function	    */
  dt_ctl_t *ctl,
  void *callback_arg,
  dtlev85_t *lev85);

extern dt_ntype_e *dtlev85_gray_ntype_list;

#define DTLEV85_GRAY_NTYPE_LIST			/* gray ntypes are constrained		*/\
  dtlev85_gray_ntype_list

#include <dtlev85.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
