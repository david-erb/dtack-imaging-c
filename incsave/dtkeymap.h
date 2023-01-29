/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
| include $RCSfile: dtkeymap.h,v $ $Revision: 1.1 $
 *..........................................................................*/

#define DTKEYMAP_FLAG_ADD     (0x04)
#define DTKEYMAP_FLAG_NEW     (0x08)
#define DTKEYMAP_FLAG_REPLACE (0x10)

#define DTKEYMAP_MAXSTRING (256)
#define DTKEYMAP_MAXENTRIES (64)

typedef struct {
  char *s;
  dt_key_t k;
  short l;
} dtkeymap_entry_t;

#define DTKEYMAP_INIT_ARGS dt_ctl_t *,    struct dtkeymap_t *, \
  const char *, unsigned long
#define DTKEYMAP_UNINIT_ARGS dt_ctl_t *,  struct dtkeymap_t *

typedef struct dtkeymap_t {
  dtkeymap_entry_t *entry;				/* entry table 						*/
  dt_rc_e (*uninit)(DTKEYMAP_UNINIT_ARGS);
  const char *spec;
  char *ks_string;
  char *ke_string;
  const char *uninit_desc;
  int maxstring;
  int n;								/* current number of entries 		*/
  int m;								/* maximum number of entries		*/
  int ks_done;
  char s[DTKEYMAP_MAXSTRING];
} dtkeymap_t;

typedef dt_rc_e DTCONFIG_API1 dtkeymap_init_f(DTKEYMAP_INIT_ARGS);
typedef dt_rc_e dtkeymap_uninit_f(DTKEYMAP_UNINIT_ARGS);

#include <dtack/keymap.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
