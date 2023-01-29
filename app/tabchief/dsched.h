/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DSCHED_GET(V) \
  if (!strncmp(p, #V, strlen(#V))) \
    V = p + strlen(#V) + 1;

#define DSCHED_ACTION_ADD (-1)
#define DSCHED_ACTION_NOP (-2)

#define DSCHED_SENTINEL "end of rules"

#define DSCHED_ACTION(BUTTON) \
  (!strcmp(BUTTON, "add=ADD")? DSCHED_ACTION_ADD: \
  (!strcmp(BUTTON, "nop=NOP")? DSCHED_ACTION_NOP: \
  (int)strtol(button, NULL, 0)))

#define DSCHED_ISERROR(DSCHED) ((DSCHED)->error[0])

#define DSCHED_FUNCTION static

typedef struct {
  char *day;
  char *start;
  char *end;
  char *period;
  char *duration;
} dsched_rule_t;

typedef struct {
  char *spigot;
  char description[1];
} dsched_cam_t;

#define DSCHED_RULES (128)

typedef struct {
  dtparam_t *param;
  dtipc_t *ipc;
  const char *form;
  const char *id;
  dsched_rule_t rule[DSCHED_RULES];
  dsched_rule_t added;
  int nrules;
  char error[256];
  char change[128];
} dsched_t;

extern const char **dsched_days;

#include <dsched.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
