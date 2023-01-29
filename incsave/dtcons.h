/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




										/* standard console things tied to	*/
										/* standard function key inputs 	*/
#define DTCONS_KEY_HELP    DT_KEY_F1
#define DTCONS_KEY_RUNSTOP DT_KEY_ENTER
#define DTCONS_KEY_RUNSTOP DT_KEY_ENTER
#define DTCONS_KEY_SWAP    DT_KEY_F2
#define DTCONS_KEY_EXIT    DT_KEY_F3
#define DTCONS_KEY_TOP     DT_KEY_F4
#define DTCONS_KEY_BOT     DT_KEY_F5

										/* console actions 					*/
typedef enum {
  DTCONS_OP_NOOP,
  DTCONS_OP_INPUT,
  DTCONS_OP_UPDATE,
  DTCONS_OP_STATUS,
  DTCONS_OP_ENTER,
  DTCONS_OP_LEAVE,
  DTCONS_OP_CLOSE
} dtcons_op_e;

typedef 
dt_rc_e 
DTCONFIG_API1
dtcons_opfunc_f(
  dt_ctl_t *, 
  dtcons_op_e,
  void *, 
  void *);

typedef 
dt_rc_e 
DTCONFIG_API1
dtcons_update_f(		/* update function 					*/
  dt_ctl_t *ctl, 
  const char *status,
  dt_key_t *key);

#define DTCONS_MAXCONS (6)				/* maximum consoles at once 		*/
#define DTCONS_TAG_MAXLEN (32)

#define DTCONS_PASSMAX (8)				/* bytes passed between consoles 	*/

typedef struct {
  dtcons_opfunc_f *opfunc[DTCONS_MAXCONS];
  dtcons_update_f *update;
  void *arg[DTCONS_MAXCONS];
  char tag[DTCONS_MAXCONS][DTCONS_TAG_MAXLEN+1];
  int ncons;
  int maxcons;
  int curr;
  dt_err_string_f *err_string;
  dt_dbg_string_f *dbg_string;
  dt_say_string_f *say_string;
  void *errto;
  void *dbgto;
  void *sayto;
} dtcons_t;

#include <dtack/cons.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
