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
 *..........................................................................*/

typedef enum {
  UMDEC_MODE_STARTING = 0,				/* initial mode						*/
  UMDEC_MODE_FORWARD = 1,				/* normal forward 					*/
  UMDEC_MODE_FFORWARD,					/* fast forward 					*/
  UMDEC_MODE_SFORWARD,					/* slow forward						*/
  UMDEC_MODE_1FORWARD,					/* single step forward				*/
  UMDEC_MODE_1FORWARDED,				/* did single step forward			*/
  UMDEC_MODE_PAUSE,
  UMDEC_MODE_STOP,
  UMDEC_MODE_UNKNOWN = 1000,			/* force enum size int 				*/
} umdec_mode_e;

typedef struct {
  umdec_mode_e mode;
} umdec_control_t;

typedef struct {
  const char *id;
  double frame;
  double pts;
} umdec_status_t;

typedef enum {
  UMDEC_FRAMETYPE_FRAMEINC = 1,			/* frame increment, no data			*/
  UMDEC_FRAMETYPE_UNKNOWN = 1000,
} umdec_frametype_e;

#define UMDEC_INIT_ARGS         dt_ctl_t *, struct umdec_t *, const char *
#define UMDEC_UNINIT_ARGS       dt_ctl_t *, struct umdec_t *
#define UMDEC_GO_ARGS           dt_ctl_t *, struct umdec_t *, umdec_control_t *
#define UMDEC_SET_CONTROL_ARGS  dt_ctl_t *, struct umdec_t *, umdec_control_t *
#define UMDEC_GET_CONTROL_ARGS  dt_ctl_t *, struct umdec_t *, umdec_control_t *
#define UMDEC_GET_STATUS_ARGS   dt_ctl_t *, struct umdec_t *, umdec_status_t *
#define UMDEC_OVERLAY_IMAGE_ARGS   dt_ctl_t *, struct umdec_t *, \
  const char *filename,					/* filename containing image 		*/\
  int x,								/* column 							*/\
  int y									/* row 								*/
#define UMDEC_OVERLAY_TEXT_ARGS    dt_ctl_t *, struct umdec_t *, \
  const char *string,					/* text to display 					*/\
  int x,								/* column 							*/\
  int y,								/* row 								*/\
  int height,							/* text height in pixels 			*/\
  long foreground,						/* foreground color RGB 			*/\
  long background						/* background color RGB 			*/
#define UMDEC_OVERLAY_CLEAR_ARGS   dt_ctl_t *, struct umdec_t *

#define UMDEC_FEED_CALLBACK_ARGS \
  dt_ctl_t *ctl, struct umdec_t *umdec, void *arg, \
  void *buffer, long want, long *got
#define UMDEC_FRAME_CALLBACK_ARGS \
  dt_ctl_t *ctl, struct umdec_t *umdec, void *arg, \
  umdec_frametype_e frametype

typedef struct umdec_t {
  umdec_control_t control;
  umdec_status_t status;
  void *feed_arg;
  void *frame_arg;
  void *priv;
  dt_rc_e (*uninit)(UMDEC_UNINIT_ARGS);
  dt_rc_e (*go)(UMDEC_GO_ARGS);
  dt_rc_e (*set_control)(UMDEC_SET_CONTROL_ARGS);
  dt_rc_e (*get_control)(UMDEC_GET_CONTROL_ARGS);
  dt_rc_e (*get_status)(UMDEC_GET_STATUS_ARGS);
  dt_rc_e (*overlay_image)(UMDEC_OVERLAY_IMAGE_ARGS);
  dt_rc_e (*overlay_text)(UMDEC_OVERLAY_TEXT_ARGS);
  dt_rc_e (*overlay_clear)(UMDEC_OVERLAY_CLEAR_ARGS);
  dt_rc_e DTCONFIG_API1DEC (*feed_callback)(UMDEC_FEED_CALLBACK_ARGS);
  dt_rc_e DTCONFIG_API1DEC (*frame_callback)(UMDEC_FRAME_CALLBACK_ARGS);
  dt_rc_e rc;
} umdec_t;

typedef dt_rc_e DTCONFIG_API1 umdec_init_f(UMDEC_INIT_ARGS);
typedef dt_rc_e umdec_uninit_f(UMDEC_UNINIT_ARGS);
typedef dt_rc_e umdec_go_f(UMDEC_GO_ARGS);
typedef dt_rc_e umdec_set_control_f(UMDEC_SET_CONTROL_ARGS);
typedef dt_rc_e umdec_get_control_f(UMDEC_GET_CONTROL_ARGS);
typedef dt_rc_e umdec_get_status_f(UMDEC_GET_STATUS_ARGS);
typedef dt_rc_e umdec_overlay_image_f(UMDEC_OVERLAY_IMAGE_ARGS);
typedef dt_rc_e umdec_overlay_text_f(UMDEC_OVERLAY_TEXT_ARGS);
typedef dt_rc_e umdec_overlay_clear_f(UMDEC_OVERLAY_CLEAR_ARGS);

typedef dt_rc_e DTCONFIG_API1 umdec_feed_callback_f(UMDEC_FEED_CALLBACK_ARGS);

typedef dt_rc_e DTCONFIG_API1 umdec_frame_callback_f(UMDEC_FRAME_CALLBACK_ARGS);

#include <umdec.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
