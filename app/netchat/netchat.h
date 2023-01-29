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
#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>

#include <ustate.h>

#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/dialog.h>

#define NETCHAT_SPEC_MAXLEN (256)
#define NETCHAT_IDENT_MAXLEN (32)
										/* ................................ */
#define NETCHAT_M_NOSUCHPARTNER \
  "can't find %s"
#define NETCHAT_M_PARTNERFIRST \
  "choose partner first"
#define NETCHAT_M_TALKTOSELF \
  "can't talk to self"
#define NETCHAT_M_OFFLINE \
  "%s is offline"
#define NETCHAT_M_CALLWAITING \
  "%s is calling"
#define NETCHAT_M_BUSY \
  "%s is busy"
#define NETCHAT_M_PRIVACY \
  "%s has privacy on"
#define NETCHAT_M_HANGUP \
  ""

										/* ................................ */

typedef enum {
  NETCHAT_STATE_TALK       = 't',
  NETCHAT_STATE_TALKING    = 'T',
  NETCHAT_STATE_LISTEN     = 'l',
  NETCHAT_STATE_LISTENING  = 'L',
  NETCHAT_STATE_PRIVACY    = 'P',
  NETCHAT_STATE_IDLE       = 'i',
  NETCHAT_STATE_IDLING     = 'I',
  NETCHAT_STATE_STARTING   = 'S'
} netchat_state_e;

										/* ................................ */
#define NETCHAT_HAVE_GRAPHICS(NETCHAT) ((NETCHAT)->graphics_initialized)
#define NETCHAT_HAVE_DIALOG(NETCHAT) ((NETCHAT)->dialog_initialized)

#define NETCHAT_LOGLINE_MAX (256)		/* max length of line in log 		*/
#define NETCHAT_LOGBASE_MAX (256)		/* max length of log filename 		*/
#define NETCHAT_LOGBASE "/CONFIG/netchat.%s."

typedef struct {
  dt_ctl_t *ctl;
  dtdialog_t dialog;
  ustate_t ustate;
  const char *ident;
  const char *dgserver_spec;
  dtdialog_field_t 
    *message_field,
    *state_field,
    *partner_field;
  long poll_period;						/* poll period in milliseconds 		*/
  int dialog_initialized;
  int graphics_initialized;
  int initialized;
  int update;							/* whether screen needs updating 	*/
  netchat_state_e state;
  char 
    message_contents[80],
    state_contents[16],
    partner_contents[32];
  char message[256];
  char last_error[256];
  char logbase[NETCHAT_LOGBASE_MAX];
  char logline[NETCHAT_LOGLINE_MAX];
} netchat_t;

										/* ................................ */
#define NETCHAT_CONFIG_COMMENT "#*"
#define NETCHAT_CONFIG_SEPARATOR ':'
										/* ................................ */

typedef 
dt_rc_e 
DTCONFIG_API1
netchat_op_f(							/* an -op parameter function 		*/
  dtparam_t *param);

#define NETCHAT_STATE_TIMEOUT (500)	/* time to wait for status connect	*/
#define NETCHAT_CONNECT_TIMEOUT (2000)	/* time to wait to contact server	*/
#define NETCHAT_HEADER_TIMEOUT (2000)	/* time to wait for header			*/
#define NETCHAT_RESPONSE_TIMEOUT (500)	/* time to wait for body lines		*/

#include <netchat.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
