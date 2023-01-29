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
 * THIS FILE IS PROPRIETARY AND CONFIDENTIAL MATERIAL OF DTACK LLC
 * THIS FILE IS TO BE CONSIDERED A TRADE SECRET OF DTACK LLC
 * THE CONTENTS OF THIS FILE MUST REMAIN PROPERLY PROTECTED FROM DISCLOSURE
 * COPYRIGHT (C) 1998 DTACK LLC, ALL RIGHTS RESERVED
 *..........................................................................*/

#define DTIPC_FLAG_SIGWASBLOCKED     0x0001
#define DTIPC_FLAG_SELF_BACKGROUND   0x0002
#define DTIPC_FLAG_SIGNAL_ON_FIND    0x0004
#define DTIPC_FLAG_SIGNAL_ON_REPLY   0x0008
#define DTIPC_FLAG_FORK_AFTER_WAIT   0x0010
#define DTIPC_FLAG_TRY_FOREVER       0x0020
#define DTIPC_FLAG_TRY_10            0x0040
#define DTIPC_FLAG_TRY_DELAY_10      0x0080
#define DTIPC_FLAG_TRY_DELAY_30      0x0100
#define DTIPC_FLAG_TRY_DELAY_60      0x0200
#define DTIPC_FLAG_NOQUEUE           0x0400
#define DTIPC_FLAG_IGNORE_SIGCHLD    0x0800

#define DTIPC_RECV_MAX_DEFAULT			/* max message willing to receive	*/\
  DTCONFIG_MALLOC_MAX

typedef struct {
  unsigned long id;
} dtipc_handle_t;

typedef enum {
  DTIPC_TYPE_NONE = 0,
  DTIPC_TYPE_QNX4 = 1,
  DTIPC_TYPE_SOCKET = 3,
  DTIPC_TYPE_DDE = 4,
  DTIPC_TYPE_QBABY = 5
} dtipc_type_e;

typedef enum {
  DTIPC_POLL_ACCEPT =  0x0001,
  DTIPC_POLL_CONNECT = 0x0002,
  DTIPC_POLL_READ =    0x0004,
  DTIPC_POLL_WRITE =   0x0008,
  DTIPC_POLL_SUBEND =  0x0010
} dtipc_poll_e;

#define DTIPC_ACK 0xfeedbabe
#define DTIPC_NACK 0xdeadbabe
typedef long dtipc_ack_t;

#define DTIPC_MAGIC       0xdeafbeefU
#define DTIPC_MAGIC_OTHER 0xefbeafdeU

#define DTIPC_FIX(CHECK, CTL, IPC, L)                                         \
   (((IPC)->need_fix)?                                                        \
   ((((dtntype_b32_t)(L) & 0x000000ffUL) << 24) |                           \
    (((dtntype_b32_t)(L) & 0x0000ff00UL) <<  8) |                           \
    (((dtntype_b32_t)(L) & 0x00ff0000UL) >>  8) |                           \
    (((dtntype_b32_t)(L) & 0xff000000UL) >> 24)): (L))

typedef struct {
  dtntype_b32_t magic;
  dtntype_b32_t total;
  dtntype_b32_t length;
  dtntype_b32_t pid;
} dtipc_header_t;

#define DTIPC_INIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  long recv_max, \
  unsigned long flags
#define DTIPC_REGISTER_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  const char *name
#define DTIPC_UNREGISTER_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc
#define DTIPC_WAIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc
#define DTIPC_POLL_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  dtipc_poll_e what, \
  long timeout, \
  const char *waitingfor, \
  int *isready
#define DTIPC_ISIPC_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  unsigned long id, \
  dtipc_header_t *h, \
  int *isipc
#define DTIPC_FIND_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  const char *name, \
  long timeout
#define DTIPC_UNFIND_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc
#define DTIPC_DONE_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc
#define DTIPC_SEND_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  const void *data, \
  long length
#define DTIPC_SEND2_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  const void *data, \
  long length, \
  long *sent
#define DTIPC_WRITE_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  const void *data, \
  long length, \
  long *wrote
#define DTIPC_RECV_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  void **data, \
  long length
#define DTIPC_RECVT_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  void **data, \
  long length, \
  long timeout
#define DTIPC_RECV2_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  void **data, \
  long *length
#define DTIPC_READ_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc, \
  void *data, \
  long maxlength, \
  long *length
#define DTIPC_UNINIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc
#define DTIPC_SPLIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtipc_t *ipc1, \
  struct dtipc_t *ipc2

typedef struct dtipc_t {
  dt_rc_e (*rregister)(DTIPC_REGISTER_ARGS);
  dt_rc_e (*unregister)(DTIPC_UNREGISTER_ARGS);
  dt_rc_e (*wait)(DTIPC_WAIT_ARGS);
  dt_rc_e (*poll)(DTIPC_POLL_ARGS);
  dt_rc_e (*isipc)(DTIPC_ISIPC_ARGS);
  dt_rc_e (*find)(DTIPC_FIND_ARGS);
  dt_rc_e (*unfind)(DTIPC_UNFIND_ARGS);
  dt_rc_e (*done)(DTIPC_DONE_ARGS);
  dt_rc_e (*send)(DTIPC_SEND_ARGS);
  dt_rc_e (*send2)(DTIPC_SEND2_ARGS);
  dt_rc_e (*write)(DTIPC_WRITE_ARGS);
  dt_rc_e (*recv)(DTIPC_RECV_ARGS);
  dt_rc_e (*recvt)(DTIPC_RECVT_ARGS);
  dt_rc_e (*recv2)(DTIPC_RECV2_ARGS);
  dt_rc_e (*read)(DTIPC_READ_ARGS);
  dt_rc_e (*uninit)(DTIPC_UNINIT_ARGS);
  dt_rc_e (*split)(DTIPC_SPLIT_ARGS);
  void *priv;
  char server_spec[64];
  char client_spec[64];
  long recv_max;
  unsigned long flags;
  dtipc_type_e type;
  dtipc_handle_t handle;
  int selectfd;							/* set to -1 if select impossible   */
  int datahandle;
  int need_fix;							/* set in recv 						*/
  char buffer[32];
} dtipc_t;

typedef dt_rc_e DTCONFIG_API1 dtipc_init_f(DTIPC_INIT_ARGS);
typedef dt_rc_e dtipc_rregister_f(DTIPC_REGISTER_ARGS);
typedef dt_rc_e dtipc_unregister_f(DTIPC_UNREGISTER_ARGS);
typedef dt_rc_e dtipc_wait_f(DTIPC_WAIT_ARGS);
typedef dt_rc_e dtipc_poll_f(DTIPC_POLL_ARGS);
typedef dt_rc_e dtipc_isipc_f(DTIPC_ISIPC_ARGS);
typedef dt_rc_e dtipc_find_f(DTIPC_FIND_ARGS);
typedef dt_rc_e dtipc_unfind_f(DTIPC_UNFIND_ARGS);
typedef dt_rc_e dtipc_done_f(DTIPC_DONE_ARGS);
typedef dt_rc_e dtipc_send_f(DTIPC_SEND_ARGS);
typedef dt_rc_e dtipc_send2_f(DTIPC_SEND2_ARGS);
typedef dt_rc_e dtipc_recv_f(DTIPC_RECV_ARGS);
typedef dt_rc_e dtipc_recvt_f(DTIPC_RECVT_ARGS);
typedef dt_rc_e dtipc_recv2_f(DTIPC_RECV2_ARGS);
typedef dt_rc_e dtipc_read_f(DTIPC_READ_ARGS);
typedef dt_rc_e dtipc_uninit_f(DTIPC_UNINIT_ARGS);
typedef dt_rc_e dtipc_split_f(DTIPC_SPLIT_ARGS);
  
#include <dtack/ipc.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
