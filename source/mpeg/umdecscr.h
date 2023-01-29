/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define UMDEC_SCR_WINDOW_NAME "umdec_scr"

#define UMDEC_SCR_PSEUDO_CELL_START (32)
#define UMDEC_SCR_PSEUDO_CELL_COUNT (214)	/* 32 (qnx)...246 (win32) 		*/

typedef struct {						/* passed to the callbacks 			*/
  dtimage_t image;						/* image space 						*/
  dtscr_t *scr;							/* screen output 					*/
  dt_color_convert_t convert;
  dt_color_org_t org_y_cb_cr;
  dt_color_org_t org_pseudo;
  dtlut8_t lut[256];
  dt_ntype_b08_t pseudo_remap[256];
  int skiplut;
  void *memalloc;
  void *tmp;
  int color_initialized;
} umdec_scr_t;

#include <umdecscr.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
