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

#define DT_DBG_MASK_CONFIG (0x02000000L) /* config							*/

#include <dtack/os.h>

#define USTREAM_SPEC_MAXLEN (256)

#define USTREAM_LOGLINE_MAX (256)		/* max length of line in log 		*/
#define USTREAM_LOGBASE_MAX (256)		/* max length of log filename 		*/
#define USTREAM_LOGBASE "/CONFIG/ustream.%s."

typedef struct {
  dt_ctl_t *ctl;
  dtos_time_t time;
  ustate_t ustate;
  const char *ident;
  const char *dgserver_spec;
  const char *istream_preference;
  long poll_period;						/* poll period in milliseconds 		*/
  long connect_timeout;					/* time to wait output connect 		*/
  long accept_timeout;					/* time to wait for input accept 	*/
  int flag_shovel;
  int flag_locked;
  char message[256];
  char last_message[256];
  char state[256];
  char istream_spec[USTREAM_SPEC_MAXLEN];
  char ostream_spec[USTREAM_SPEC_MAXLEN];
  char permit[USTREAM_SPEC_MAXLEN];
  char logbase[USTREAM_LOGBASE_MAX];
  char logline[USTREAM_LOGLINE_MAX];
} ustream_t;

typedef
dt_rc_e 
DTCONFIG_API1
ustream_response_f(						/* response callback 				*/
  void *response_arg,
  const char *format,
  ...);

typedef
dt_rc_e 
DTCONFIG_API1
ustream_command_f(
  ustream_t *ustream,
  dtdg_server_t *dgserver,
  const char *command,
  char *arg);

typedef 
dt_rc_e 
DTCONFIG_API1
ustream_op_f(							/* an -op parameter function 		*/
  dtparam_t *param);


#include <ustream.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
