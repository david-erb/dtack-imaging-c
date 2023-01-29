/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* -----------------------------------------------------------------------  */
#include <dtack/base.h>							/* fundamental declarations 		*/

#define U_R DT_RCSID
#define U_F DT_F

#define U_Q DT_Q
#define U_C DT_C
#define U_I DT_I
#define U_G DT_G

#define U_API    DTCONFIG_API1
#define U_APIDEC DTCONFIG_API1DEC

#define U_RC_GOOD DT_RC_GOOD
#define U_RC_BAD  DT_RC_BAD
#define U_RC_STOP DT_RC_STOP

#define U_FOPEN_RBINARY DTCONFIG_FOPEN_RBINARY 
#define U_FOPEN_WBINARY DTCONFIG_FOPEN_WBINARY
#define U_FOPEN_ABINARY DTCONFIG_FOPEN_ABINARY
#define U_FOPEN_RWBINARY DTCONFIG_FOPEN_RWBINARY
#define U_FOPEN_WRBINARY DTCONFIG_FOPEN_WRBINARY

#define u_err dt_err
#define u_dbg dt_dbg
#define u_say dt_say
#define u_err_string dt_err_string
#define u_dbg_string dt_dbg_string
#define u_say_string dt_say_string
#define u_err_va dt_err_va
#define u_dbg_va dt_dbg_va
#define u_say_va dt_say_va

typedef dt_rc_e u_rc_e;
typedef dt_ctl_t u_ctl_t;
/* -----------------------------------------------------------------------  */

#include <dtack/str.h>

#include <dtack/ipc.h>

#include <dtack/mem.h>
#include <dtack/os.h>

#include <dtack/list.h>
#include <dtack/liner.h>

#include <umdec.h>

#include <dtack/keymap.h>
#include <dtack/keybd.h>

#define UMPLAY_HAS_FD(UMPLAY) \
  ((UMPLAY)->fd.fd != -1)

#define UMPLAY_HAS_COMMAND_IPC(UMPLAY) \
  ((UMPLAY)->command_ipc.type != DTIPC_TYPE_NONE)
#define UMPLAY_HAS_DATA_IPC(UMPLAY) \
  ((UMPLAY)->data_ipc.type != DTIPC_TYPE_NONE)

#define UMPLAY_HAS_DATA_FIFO(UMPLAY) \
  ((UMPLAY)->data_fifo.fd != -1)
#define UMPLAY_HAS_COMMAND_FIFO(UMPLAY) \
  ((UMPLAY)->command_fifo.fd != -1)
#define UMPLAY_HAS_REPLY_FIFO(UMPLAY) \
  ((UMPLAY)->reply_fifo.fd != -1)

#define UMPLAY_HAS_SCREEN(UMPLAY) \
  ((UMPLAY)->screen_initialized)
#define UMPLAY_HAS_KEYBD(UMPLAY) \
  ((UMPLAY)->keybd_initialized)
#define UMPLAY_HAS_SOURCE(UMPLAY) \
  (UMPLAY_HAS_FD(UMPLAY) || \
   UMPLAY_HAS_DATA_FIFO(UMPLAY))

#define UMPLAY_IS_PAUSED(UMPLAY) \
  ((UMPLAY)->control.mode == UMDEC_MODE_PAUSE)

										/* ................................ */
enum DECCNTL {
  DECCMD_INITPARAM=0, 
  DECCMD_PLAY, 
  DECCMD_PAUSE, 
  DECCMD_STEPFRAME, 
  DECCMD_STEPGOP, 
  DECCMD_STOP, 
  DECCMD_NOP, 
  DECCMD_GETFRAME,
  DECCMD_EXIT	
};

typedef struct {
	short	cmd;
	union {
		struct {
			short playSpeed;
			short videoMode;
		} init;
		short	playSpeed;				/* (-inf, -8] slowest; 
										   [-8, -2] slow mo; 
										   [-1, 1] normal; 
										   [2, inf) fast */

		short	numStep;				/* number of frames of GOP to step 	*/
	} u;
} umplay_deccmd_t;
										/* ................................ */
enum DECREPTYPE {
  DECREP_FRAME
};

typedef struct {
	short reptype;
	union{
		long	framenum;
		char	pad[1024-2]; 
	} u;
} umplay_decrep_t;

										/* ................................ */
#define UMPLAY_KEY_SOURCE 'i'
#define UMPLAY_KEY_FORWARD 'f'
#define UMPLAY_KEY_FFORWARD '+'
#define UMPLAY_KEY_SFORWARD '-'
#define UMPLAY_KEY_1FORWARD '-'
#define UMPLAY_KEY_PAUSE 'p'
#define UMPLAY_KEY_QUIT 'q'
#define UMPLAY_KEY_EXIT 'x'
#define UMPLAY_KEY_STATUS 's'

										/* ................................ */
#define UMPLAY_COMMAND_SOURCE "input"
#define UMPLAY_COMMAND_FORWARD "forward"
#define UMPLAY_COMMAND_FFORWARD "fforward"
#define UMPLAY_COMMAND_SFORWARD "sforward"
#define UMPLAY_COMMAND_1FORWARD "1forward"
#define UMPLAY_COMMAND_PAUSE "pause"
#define UMPLAY_COMMAND_QUIT "quit"
#define UMPLAY_COMMAND_EXIT "exit"
#define UMPLAY_COMMAND_STATUS "status"
										/* ................................ */
#define UMPLAY_PATH_MAX (256)
#define UMPLAY_RESPONSE_MAX (256)
#define UMPLAY_LOGLINE_MAX (256)
#define UMPLAY_LOGFILE "/CONFIG/umplay"
										/* ................................ */
#define UMPLAY_CONFIG_COMMENT "#*"
#define UMPLAY_CONFIG_SEPARATOR ":"
#define UMPLAY_OVERLAY_TEXT_STRING       "umplay.overlay.text.string"
#define UMPLAY_OVERLAY_TEXT_X0           "umplay.overlay.text.x0"
#define UMPLAY_OVERLAY_TEXT_Y0           "umplay.overlay.text.y0"
#define UMPLAY_OVERLAY_TEXT_HEIGHT       "umplay.overlay.text.height"
#define UMPLAY_OVERLAY_TEXT_FOREGROUND	 "umplay.overlay.text.foreground"
#define UMPLAY_OVERLAY_TEXT_BACKGROUND   "umplay.overlay.text.background"
#define UMPLAY_OVERLAY_IMAGE_VP_FILENAME "umplay.overlay.image.vp.filename"
#define UMPLAY_OVERLAY_IMAGE_X0          "umplay.overlay.image.x0"
#define UMPLAY_OVERLAY_IMAGE_Y0          "umplay.overlay.image.y0"
										/* ................................ */

typedef struct {						/* passed to the callbacks 			*/
  dtparam_t *param;
  dt_ctl_t *ctl;
  dtlist_t config_list;					/* list read from config file 		*/
  dtipc_t command_ipc;					/* ipc control						*/
  dtipc_t data_ipc;						/* ipc data							*/
  dtfd_t fd;							/* file source 						*/
  dtfd_t data_fifo;						/* data fifo						*/
  dtfd_t command_fifo;					/* command fifo						*/
  dtfd_t reply_fifo;					/* reply fifo						*/
  dtkeybd_t keybd;						/* keyboard control 				*/
  umdec_control_t control;				/* last control sent to umdec 		*/
  unsigned long frame;
  long delay;
  int keybd_initialized;
  int command_ipc_registered;
  int exiting;
  char response[UMPLAY_RESPONSE_MAX];
  char logline[UMPLAY_LOGLINE_MAX];
  char config_filename[UMPLAY_PATH_MAX];
  char data_fifo_name[UMPLAY_PATH_MAX];
  char command_fifo_name[UMPLAY_PATH_MAX];
  char reply_fifo_name[UMPLAY_PATH_MAX];
} umplay_t;


typedef dt_rc_e umplay_command_f(
  umplay_t *umplay, 
  const char *command);

#include <umplay.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
