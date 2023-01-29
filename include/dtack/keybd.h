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
| include $RCSfile: dtkeybd.h,v $ $Revision: 1.1 $
 *..........................................................................*/

#define DTKEYBD_MAXMAPENTRIES (128)
#define DTKEYBD_MAXBUFFER (32)

										/* share bit space with keymap 		*/
#define DTKEYBD_FLAG_INIT_NOMAP			/* don't create initial keymap		*/\
  (0x0001)

#define DTKEYBD_FLAG_INIT_FULLSCREEN	/* initiate full screen mode		*/\
  (0x0002)

										/* ................................	*/
#define DTKEYBD_INIT_ARGS dt_ctl_t *,    struct dtkeybd_t *, \
  const char *, long
#define DTKEYBD_UNINIT_ARGS dt_ctl_t *,  struct dtkeybd_t *
#define DTKEYBD_SCAN_ARGS dt_ctl_t *,    struct dtkeybd_t *
#define DTKEYBD_READ_ARGS dt_ctl_t *,    struct dtkeybd_t *
#define DTKEYBD_UNINIT_ARGS dt_ctl_t *,  struct dtkeybd_t *
										/* ................................ */
typedef struct dtkeybd_t {
  dtkeymap_t keymap;
  char *spec;
  const char *spec1;
  const char *spec2;
  const char *spec3;
  void *priv;
  dt_rc_e (*uninit)(DTKEYBD_UNINIT_ARGS);
  dt_rc_e (*scan)(DTKEYBD_SCAN_ARGS);
  dt_rc_e (*read)(DTKEYBD_READ_ARGS);
  int fd;
  int maxbuffer;
  int n;
  int ispartial;
  char buffer[DTKEYBD_MAXBUFFER];
} dtkeybd_t;

typedef dt_rc_e DTCONFIG_API1 dtkeybd_init_f(DTKEYBD_INIT_ARGS);
typedef dt_rc_e dtkeybd_uninit_f(DTKEYBD_UNINIT_ARGS);
typedef dt_rc_e dtkeybd_scan_f(DTKEYBD_SCAN_ARGS);
typedef dt_rc_e dtkeybd_read_f(DTKEYBD_READ_ARGS);

										/* ................................	*/
#include <dtack/keybd.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
