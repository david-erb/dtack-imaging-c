/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  dtipc_t _ipc;
  dtipc_t *ipc;
  dtlist_t header_list;
  dtlist_t response_list;
  int initialized;
  int error;
} dtdg_t;

typedef dtdg_t dtdg_server_t;
typedef dtdg_t dtdg_client_t;

typedef
dt_rc_e 
DTCONFIG_API1
dtdg_server_request_f(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  void *arg,
  const char *command,
  char *rest,
  char *message,
  int message_max);

#define DTDG_SERVER_REQUEST_MAXLEN (1024)

#define DTDG_RESPONSE_MAXLEN (1024)

										/* ................................ */

#define DTDG_REQUEST_STATE         "GET /state"
#define DTDG_REQUEST_IDENT         "GET /ident"

#define DTDG_RESPONSE_IDENT        "server_ident"
#define DTDG_RESPONSE_STATE        "server_state"
#define DTDG_RESPONSE_RC           "server_rc"
#define DTDG_RESPONSE_RESULT       "server_result"
#define DTDG_RESPONSE_ERROR        "server_error"
#define DTDG_RESPONSE_LASTERROR    "last_error"
#define DTDG_RESPONSE_OSTREAM_SPEC "ostream_spec"
#define DTDG_RESPONSE_ISTREAM_SPEC "istream_spec"
#define DTDG_RESPONSE_PERMIT       "permit"

#define DTDG_STATE_WAITING         "waiting for command"
#define DTDG_STATE_CONNECTING      "connecting"
#define DTDG_STATE_DISCONNECTING   "disconnecting"
#define DTDG_STATE_SHOVELING       "shoveling"
#define DTDG_STATE_LOCKED          "locked"
#define DTDG_STATE_EXITING         "exiting"
#define DTDG_STATE_SHUTTING_DOWN   "shutting down"
#include <dtack/dg.p>

#ifdef __cplusplus
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
