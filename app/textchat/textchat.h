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

#define TEXTCHAT_SPEC_MAXLEN (256)
#define TEXTCHAT_IDENT_MAXLEN (32)
										/* ................................ */
#define TEXTCHAT_M_NOSUCHPARTNER \
  "can't find %s"
#define TEXTCHAT_M_PARTNERFIRST \
  "choose partner first"
#define TEXTCHAT_M_TALKTOSELF \
  "can't talk to self"
#define TEXTCHAT_M_OFFLINE \
  "%s is offline"
#define TEXTCHAT_M_CALLWAITING \
  "%s is calling"
#define TEXTCHAT_M_BUSY \
  "%s is busy"

										/* ................................ */

typedef enum {
  TEXTCHAT_STATE_TALK       = 't',
  TEXTCHAT_STATE_TALKING    = 'T',
  TEXTCHAT_STATE_LISTEN     = 'l',
  TEXTCHAT_STATE_LISTENING  = 'L',
  TEXTCHAT_STATE_PRIVACY    = 'P',
  TEXTCHAT_STATE_IDLE       = 'i',
  TEXTCHAT_STATE_IDLING     = 'I',
  TEXTCHAT_STATE_STARTING   = 'S'
} textchat_state_e;

										/* ................................ */
#define TEXTCHAT_HAVE_GRAPHICS(TEXTCHAT) ((TEXTCHAT)->graphics_initialized)
#define TEXTCHAT_HAVE_DIALOG(TEXTCHAT) ((TEXTCHAT)->dialog_initialized)

#define TEXTCHAT_LOGLINE_MAX (256)		/* max length of line in log 		*/
#define TEXTCHAT_LOGBASE_MAX (256)		/* max length of log filename 		*/
#define TEXTCHAT_LOGBASE "/CONFIG/textchat.%s."

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
  textchat_state_e state;
  char 
    message_contents[80],
    state_contents[16],
    partner_contents[32];
  char message[256];
  char logbase[TEXTCHAT_LOGBASE_MAX];
  char logline[TEXTCHAT_LOGLINE_MAX];
} textchat_t;

										/* ................................ */
#define TEXTCHAT_CONFIG_COMMENT "#*"
#define TEXTCHAT_CONFIG_SEPARATOR ':'
										/* ................................ */

typedef 
dt_rc_e 
DTCONFIG_API1
textchat_op_f(							/* an -op parameter function 		*/
  dtparam_t *param);

#define TEXTCHAT_STATE_TIMEOUT (500)	/* time to wait for status connect	*/
#define TEXTCHAT_CONNECT_TIMEOUT (2000)	/* time to wait to contact server	*/
#define TEXTCHAT_HEADER_TIMEOUT (2000)	/* time to wait for header			*/
#define TEXTCHAT_RESPONSE_TIMEOUT (500)	/* time to wait for body lines		*/

#include <textchat.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
