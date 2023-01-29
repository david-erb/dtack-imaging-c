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
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/list.h>
#include <dsched.h>

typedef dt_rc_e 
DTCONFIG_API1
tabchief_f(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg);

typedef struct {
  char response[256];
  dt_rc_e reply_rc;
} tabchief_t;

#define TABCHIEF_NAME_MAX (64)

#define TABCHIEF_CAMS_MAX (8)

#define TABCHIEF_COMMAND_SCHEDULE "schedule"

extern dtlist_t tabchief_proclist;

#define TABCHIEF_FLAG_WHOLE 0x0001
#define TABCHIEF_FLAG_TAIL  0x0002
#define TABCHIEF_FLAG_SIZE  0x0004
#define TABCHIEF_FLAG_PRE   0x0008
#define TABCHIEF_FLAGS_SHOW \
  (TABCHIEF_FLAG_WHOLE|TABCHIEF_FLAG_SIZE|TABCHIEF_FLAG_PRE)
#define TABCHIEF_FLAGS_TAIL \
  (TABCHIEF_FLAG_TAIL|TABCHIEF_FLAG_SIZE|TABCHIEF_FLAG_PRE)
#define TABCHIEF_FLAGS_RAW  \
  (TABCHIEF_FLAG_WHOLE)

#define TABCHIEF_TRIM(S)                                                      \
  {                                                                           \
    char *p;                                                                  \
	p = strrchr((S), '.');				/* chop off filetype				*/\
	if (p != NULL)                                                            \
	  *p = '\0';                                                              \
	p = strrchr((S), '/');				/* trim off path before '/'			*/\
	if (p != NULL)                                                            \
	  memmove((char *)(S), p+1,                                               \
        strlen(p));                                                           \
	p = strrchr((S), '\\');				/* trim off path before '\'			*/\
	if (p != NULL)                                                            \
	  memmove((char *)(S), p+1,                                               \
        strlen(p));                                                           \
  } 

#include <tabchief.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
