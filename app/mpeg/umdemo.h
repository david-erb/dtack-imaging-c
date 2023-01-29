/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#include <u.h>

#include <dtack/str.h>

#include <dtack/ipc.h>

#include <dtack/mem.h>
#include <dtack/os.h>

#include <umdec.h>

#include <dtack/keymap.h>
#include <dtack/keybd.h>

#define UMDEMO_HAS_FD(UMDEMO) \
  ((UMDEMO)->fd.fd != -1)

#define UMDEMO_HAS_COMMAND_IPC(UMDEMO) \
  ((UMDEMO)->command_ipc.type != DTIPC_TYPE_NONE)
#define UMDEMO_HAS_DATA_IPC(UMDEMO) \
  ((UMDEMO)->data_ipc.type != DTIPC_TYPE_NONE)

#define UMDEMO_HAS_DATA_FIFO(UMDEMO) \
  ((UMDEMO)->data_fifo.fd != -1)
#define UMDEMO_HAS_COMMAND_FIFO(UMDEMO) \
  ((UMDEMO)->command_fifo.fd != -1)
#define UMDEMO_HAS_REPLY_FIFO(UMDEMO) \
  ((UMDEMO)->reply_fifo.fd != -1)

#define UMDEMO_HAS_SCREEN(UMDEMO) \
  ((UMDEMO)->screen_initialized)
#define UMDEMO_HAS_KEYBD(UMDEMO) \
  ((UMDEMO)->keybd_initialized)
#define UMDEMO_HAS_SOURCE(UMDEMO) \
  (UMDEMO_HAS_FD(UMDEMO) || \
   UMDEMO_HAS_DATA_FIFO(UMDEMO))

#define UMDEMO_IS_PAUSED(UMDEMO) \
  ((UMDEMO)->frame_last != 0 && \
   (UMDEMO)->frame_pause != 0 && \
   (UMDEMO)->frame_last >= \
   (UMDEMO)->frame_pause)

										/* ................................ */
enum DECCNTL {
  DECCMD_INITPARAM=0, 
  DECCMD_PLAY, 
  DECCMD_PAUSE, 
  DECCMD_STEPFRAME, 
  DECCMD_STEPGOP, 
  DECCMD_STOP, 
  DECCMD_NOP, 
  DECCMD_GETFRAME	
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
} umdemo_deccmd_t;
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
} umdemo_decrep_t;

										/* ................................ */
#define UMDEMO_KEY_SOURCE 'i'
#define UMDEMO_KEY_FORWARD 'f'
#define UMDEMO_KEY_PAUSE 'p'
#define UMDEMO_KEY_QUIT 'q'
#define UMDEMO_KEY_EXIT 'x'
#define UMDEMO_KEY_STATUS 's'

										/* ................................ */
#define UMDEMO_COMMAND_SOURCE "input"
#define UMDEMO_COMMAND_FORWARD "forward"
#define UMDEMO_COMMAND_PAUSE "pause"
#define UMDEMO_COMMAND_QUIT "quit"
#define UMDEMO_COMMAND_EXIT "exit"
#define UMDEMO_COMMAND_STATUS "status"
										/* ................................ */
#define UMDEMO_STATE_FLAG_PAUSED (0x0001)

typedef struct {						/* passed to the callbacks 			*/
  dtparam_t *param;
  dt_ctl_t *ctl;
  dtipc_t command_ipc;					/* ipc control						*/
  dtipc_t data_ipc;						/* ipc data							*/
  dtfd_t fd;							/* file source 						*/
  dtfd_t data_fifo;						/* data fifo						*/
  dtfd_t command_fifo;					/* command fifo						*/
  dtfd_t reply_fifo;					/* reply fifo						*/
  dtkeybd_t keybd;						/* keyboard control 				*/
  double frame_max;
  double frame_last;
  double frame_pause;
  int screen_initialized;
  int keybd_initialized;
  int color_initialized;
  int command_ipc_registered;
  int pausing;
  int stepping;
  int speeding;
  int crawling;
  int stopping;
  int exiting;
  unsigned long state_flags;
  char response[256];
} umdemo_t;


typedef dt_rc_e umdemo_command_f(
  umdemo_t *umdemo, 
  const char *command);

#include <umdemo.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
