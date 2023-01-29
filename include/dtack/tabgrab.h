/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>
#include <dtack/ipc.h>
#include <dtack/os.h>
#include <dtack/str.h>

typedef struct {
  dtfb_t fb;
  dtfd_t fd;
  dtos_time_t beg;						/* wall time at first time 			*/
  dtparam_t *param;
  dt_ctl_t *ctl;
  void *b;
  long l;
  long handle;							/* win32 mmf handle					*/
  long lock;							/* win32 sempaphore					*/
  long frame;
  long nrequests;
  long nalready;
  int fb_initialized;
  char filename[128];
} tabgrab_t;

#define TABGRAB_IS_SHM(TABGRAB) \
  ((TABGRAB)->filename[0] == '*')
#define TABGRAB_DO_LOCK(TABGRAB) \
  ((TABGRAB)->filename[1] != '*')

										/* ................................ */
#include <dtack/tableau.h>
#include <tabgrab.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
