/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DTIACT_BUT_HELP  {"Help", DTCONS_KEY_HELP}
#define DTIACT_BUT_HALT  {"Halt", DTCONS_KEY_RUNSTOP}
#define DTIACT_BUT_RUN   {"Run",  DTCONS_KEY_RUNSTOP}
#define DTIACT_BUT_EXIT  {"Exit", DTCONS_KEY_EXIT}
#define DTIACT_BUT_SWAP  {"Swap", DTCONS_KEY_SWAP}
#define DTIACT_BUT_TOP   {"Top",  DTCONS_KEY_TOP}
#define DTIACT_BUT_BOT   {"Bot",  DTCONS_KEY_BOT}

										/* standard console names 			*/
#define DTIACT_CONS_BASE   "INPUT screen"
#define DTIACT_CONS_OUTPUT "OUTPUT screen"
#define DTIACT_CONS_ERR    ""
#define DTIACT_CONS_DBG    ""
#define DTIACT_CONS_SAY    ""
#define DTIACT_CONS_HELP   "HELP screen"

#define DTIACT_STATUS_RUNNING "running"
#define DTIACT_STATUS_WAITING "waiting for input"

#define DTIACT_FIELDNAME_ERROR "error"
#define DTIACT_FIELDNAME_STATUS "status"
#define DTIACT_FIELDNAME_ARG "arg"

typedef struct {
  char *label;
  dt_key_t key;
} dtiact_but_t;

#include <dtack/iact.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
