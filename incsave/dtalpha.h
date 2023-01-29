/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DTALPHA_STATUS_FIELD_LEN (24)
#define DTALPHA_AT_FIELD_LEN (24)

#define DTALPHA_FIELDNAME_COMMAND "command"
#define DTALPHA_FIELDNAME_INPUT "input"
#define DTALPHA_FIELDNAME_STATUS "status"
#define DTALPHA_FIELDNAME_AT "at"
#define DTALPHA_FIELDNAME_ERROR "error"
#define DTALPHA_FIELDNAME_LINE "line"

typedef struct {
  dtdialog_t dialog;
  dtnote_t note;
  dtnote_cursor_t cursor;
  int xe, ye;
  int w, h;
  long pos;
  int top;
  int bot;
  int painted;
  int scroll_lock;
  char *s;
  char input[32];
  char status[DTALPHA_STATUS_FIELD_LEN];
  char at[DTALPHA_AT_FIELD_LEN];
  int at_bot, at_n;
  char msg[52];
  dtdialog_field_t *input_field;
  dtdialog_field_t *status_field;
  dtdialog_field_t *at_field;
  dtdialog_field_t *msg_field;
  dtdialog_field_t **field;				/* dialog field pointers		    */
} dtalpha_t;

#include <dtack/alpha.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
