/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTVIEW_GRAVITY_NW 0
#define DTVIEW_GRAVITY_NE 1
#define DTVIEW_GRAVITY_SW 2
#define DTVIEW_GRAVITY_SE 3
#define DTVIEW_GRAVITY_CENTER 4

#define DTVIEW_FOREGROUND (255)
#define DTVIEW_BACKGROUND (100)

#define DTVIEW_MEMORY_KEY "M"
#define DTVIEW_SAVE_KEY      "S"
#define DTVIEW_SAVE_KEY_CHAR 'S'
#define DTVIEW_HELP_KEY "?"

#define DTVIEW_INTRODUCE_TITLE "Introducing..."
#define DTVIEW_INFO_TITLE "Info..."
#define DTVIEW_INFO_X0 (scr->xe/2)
#define DTVIEW_INFO_Y0 (scr->ye/2)

#define DTVIEW_MEMORY_TITLE DTVIEW_INFO_TITLE

#define DTVIEW_MEMORY_X0      DTVIEW_INFO_X0
#define DTVIEW_MEMORY_Y0      DTVIEW_INFO_Y0
#define DTVIEW_MEMORY_GRAVITY DTVIEW_GRAVITY_CENTER

#define DTVIEW_SAVE_TITLE   DTVIEW_INFO_TITLE
#define DTVIEW_SAVE_X0      DTVIEW_INFO_X0
#define DTVIEW_SAVE_Y0      DTVIEW_INFO_Y0
#define DTVIEW_SAVE_GRAVITY DTVIEW_GRAVITY_CENTER
		
#define DTVIEW_HELP_TITLE   DTVIEW_INFO_TITLE
#define DTVIEW_HELP_X0      DTVIEW_INFO_X0
#define DTVIEW_HELP_Y0      DTVIEW_INFO_Y0
#define DTVIEW_HELP_GRAVITY DTVIEW_GRAVITY_CENTER

typedef
dt_rc_e
DTCONFIG_API1
dtview_loop_f(							/* loop for graphics or ipc 		*/
  dtparam_t *param,
  dtwins_t *wins,
  dtsvc_t *svc,
  const char *output,
  const char *help_string);

#include <dtack/view.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
