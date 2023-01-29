/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





typedef enum {
  DT_COLOR_ORG_LUMINANCE = 0,
  DT_COLOR_ORG_YCBYCR,
  DT_COLOR_ORG_YCBCR,
  DT_COLOR_ORG_Y_CB_CR,
  DT_COLOR_ORG_Y_CB_CR_411,
  DT_COLOR_ORG_CB,
  DT_COLOR_ORG_CR,
  DT_COLOR_ORG_YUYV,
  DT_COLOR_ORG_RGB888,
  DT_COLOR_ORG_R_G_B,
  DT_COLOR_ORG_RGB555,
  DT_COLOR_ORG_RGB565,
  DT_COLOR_ORG_PSEUDO,
  DT_COLOR_ORG_MAP,
  DT_COLOR_ORG_BMP24,
  DT_COLOR_ORG_UNKNOWN = 1000
} dt_color_org_e;

#define DT_COLOR_ORG_COUNT (15)

typedef struct {
  dt_color_org_e org;
} dt_color_org_t;


typedef struct {
  void *space;
  int initialized;
  int ncells;							/* number of color cells to use 	*/
  unsigned char *rlut;
  unsigned char *glut;
  unsigned char *blut;
} dt_color_convert_t;


#include <dtack/color.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
