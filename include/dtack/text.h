/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define DTTEXT_FLAG_NORMAL    (0x1)
#define DTTEXT_FLAG_HIGHLIGHT (0x2)
#define DTTEXT_FLAG_CURSOR    (0x4)

#define DTTEXT_BEG_ARGS \
  dt_ctl_t *ctl, \
  struct dttext_t *text, \
  const char *spec
#define DTTEXT_END_ARGS \
  dt_ctl_t *ctl, \
  struct dttext_t *text
#define DTTEXT_CLEAR_ARGS \
  dt_ctl_t *ctl, \
  struct dttext_t *text
#define DTTEXT_OUT_ARGS \
  dt_ctl_t *ctl, \
  struct dttext_t *text, \
  const char *string, \
  int x, \
  int y, \
  unsigned long flags
#define DTTEXT_KEY_ARGS \
  dt_ctl_t *ctl, \
  struct dttext_t *text, \
  int *iskey, \
  dt_key_t *

#define DTTEXT_MAXFIELDS (128)

typedef struct dttext_t {
  dt_rc_e (*end)(DTTEXT_END_ARGS);
  dt_rc_e (*out)(DTTEXT_OUT_ARGS);
  dt_rc_e (*clear)(DTTEXT_CLEAR_ARGS);
  dt_rc_e (*key)(DTTEXT_KEY_ARGS);
  void *priv;
  const char *keyboard_name;
  dtkeybd_t *keyboard;					/* so upper levels can print maps		*/
  int xe;
  int ye;
} dttext_t;

typedef dt_rc_e DTCONFIG_API1 dttext_beg_f(DTTEXT_BEG_ARGS);
typedef dt_rc_e dttext_end_f(DTTEXT_END_ARGS);
typedef dt_rc_e dttext_out_f(DTTEXT_OUT_ARGS);
typedef dt_rc_e dttext_clear_f(DTTEXT_CLEAR_ARGS);
typedef dt_rc_e dttext_key_f(DTTEXT_KEY_ARGS);

#define DTTEXT_HELP "\
<text-spec> is one of:\n\
    curses\n\
    tcap"

#include <dtack/text.p>






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
