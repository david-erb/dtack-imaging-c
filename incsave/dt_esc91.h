/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





typedef dt_rc_e dt_esc94_tfunc_f(		/* function called at each pixel	*/
  dtparam_t *param,
  void *targ,
  double imn,
  dtxy_t x,
  dtxy_t y,
  double *thresh);

typedef struct {						/* used by the predefined tfuncs	*/
  dtimage_t image;
  unsigned long count;
} dt_esc94_targ_t;

dt_esc94_tfunc_f dt_esc94_tfunc_std;
dt_esc94_tfunc_f dt_esc94_tfunc_k;
dt_esc94_tfunc_f dt_esc94_tfunc_threshimg;
dt_esc94_tfunc_f dt_esc94_tfunc_cell;

#define DT_ESC94_TFUNC_STD       ((dt_esc94_tfunc_f *)1)
#define DT_ESC94_TFUNC_K         ((dt_esc94_tfunc_f *)2)
#define DT_ESC94_TFUNC_THRESHIMG ((dt_esc94_tfunc_f *)3)
#define DT_ESC94_TFUNC_CELL      ((dt_esc94_tfunc_f *)4)

#include <dt_esc94.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
