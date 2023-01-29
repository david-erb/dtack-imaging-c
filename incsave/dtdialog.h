/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTDIALOG_INIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  struct dtdialog_t *existing, \
  char *init, \
  const char *keyboard
#define DTDIALOG_UNINIT_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog
#define DTDIALOG_BUTTON_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  char *label, \
  int pos, \
  dt_key_t key
#define DTDIALOG_INPUT_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  dtdialog_field_t **start, \
  dt_key_t *key
#define DTDIALOG_HIDE_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  int flags
#define DTDIALOG_TWIRL_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  int x, \
  int y, \
  int flags
#define DTDIALOG_KEY_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  int *iskey, \
  dt_key_t *key
#define DTDIALOG_HELP_ARGS \
  dt_ctl_t *ctl, \
  struct dtdialog_t *dialog, \
  const char *filename, \
  const char *topic, \
  int flags

typedef struct {
  const char *name;
  char *contents;
  int l;								/* current length (priv use)		*/
  int x;
  int y;
  int lmax;
  int changed;
} dtdialog_field_t;

#define DTDIALOG_MAXFIELDS (128)
#define DTDIALOG_NBUTTONS (6)
#define DTDIALOG_BUTTON_MAXLEN (14)

#define DTDIALOG_HIDE_FLAG_CLEAR (0x01)
#define DTDIALOG_HIDE_FLAG_OFF (0x02)

#define DTDIALOG_TWIRL_FLAG_STOP    (0x01)
#define DTDIALOG_TWIRL_FLAG_START   (0x02)
#define DTDIALOG_TWIRL_FLAG_ADVANCE (0x04)

#define DTDIALOG_FLAG_NONE    ((unsigned long)0x00000000L)
#define DTDIALOG_FLAG_VSMALL  ((unsigned long)0x00000001L)
#define DTDIALOG_FLAG_VMEDIUM ((unsigned long)0x00000002L)
#define DTDIALOG_FLAG_VLARGE  ((unsigned long)0x00000004L)

#define DTDIALOG_FIELD_NORMAL (0)		/* used as "length" in field def	*/
#define DTDIALOG_FIELD_MASK (-256)
#define DTDIALOG_FIELD_HIGHLIGHT (DTDIALOG_FIELD_MASK|0x01)
#define DTDIALOG_FIELD_BEEP      (DTDIALOG_FIELD_MASK|0x02)
#define DTDIALOG_FIELD_FIXED     (DTDIALOG_FIELD_MASK|0x04)

typedef struct dtdialog_t {
  dt_rc_e DTCONFIG_API1DEC (*init)(DTDIALOG_INIT_ARGS);
  dt_rc_e (*uninit)(DTDIALOG_UNINIT_ARGS);
  dt_rc_e (*button)(DTDIALOG_BUTTON_ARGS);
  dt_rc_e (*input)(DTDIALOG_INPUT_ARGS);
  dt_rc_e (*hide)(DTDIALOG_HIDE_ARGS);
  dt_rc_e (*twirl)(DTDIALOG_TWIRL_ARGS);
  dt_rc_e (*key)(DTDIALOG_KEY_ARGS);
  dt_rc_e (*help)(DTDIALOG_HELP_ARGS);
  void *priv;
  void *key_arg;
  void *keyboard;						/* so upper levels can print maps 	*/
  char *title;
  char *subinit;
  char *helpstring;
  dtdialog_field_t *field;
  dtdialog_field_t *butfield;
  int maxfields;
  int nfields;
  int xe;
  int ye;
  unsigned long flags;
  int existing;
  int hidden;
  char butlabel[DTDIALOG_NBUTTONS*DTDIALOG_BUTTON_MAXLEN];
} dtdialog_t;

typedef dt_rc_e DTCONFIG_API1 dtdialog_init_f(DTDIALOG_INIT_ARGS);
typedef dt_rc_e dtdialog_uninit_f(DTDIALOG_UNINIT_ARGS);
typedef dt_rc_e dtdialog_button_f(DTDIALOG_BUTTON_ARGS);
typedef dt_rc_e dtdialog_input_f(DTDIALOG_INPUT_ARGS);
typedef dt_rc_e dtdialog_hide_f(DTDIALOG_HIDE_ARGS);
typedef dt_rc_e dtdialog_twirl_f(DTDIALOG_TWIRL_ARGS);
typedef dt_rc_e dtdialog_help_f(DTDIALOG_HELP_ARGS);
typedef dt_rc_e dtdialog_key_f(DTDIALOG_KEY_ARGS);

#define DTDIALOG_HELP "\
<dialog-spec> is one of:\n\
    text\n\
    msw"

#include <dtack/dialog.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
