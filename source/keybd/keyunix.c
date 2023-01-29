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
| NAME
| Introduction - intro
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>

DT_RCSID("source/keybd $RCSfile: keyunix.c,v $ $Revision: 1.1 $");

extern dtkeybd_init_f dtkeybd_unix_init;

#if DTPRJ_TERMIOS == DT_1

#ifndef DTDEPEND
#include <termios.h>
#endif
#include <dtack/trmios.h>

static dtkeybd_scan_f dtkeybd_unix_scan;
static dtkeybd_read_f dtkeybd_unix_read;
static dtkeybd_uninit_f dtkeybd_unix_uninit;

typedef struct {
  dttrmios_keyboard_t trmios;
} priv_t;

/*..........................................................................*/
static
dt_rc_e
dtkeybd_unix_scan(						/* get key from keyboard, no wait	*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_unix_scan");
  priv_t *priv = 
    (priv_t *)keyboard->priv;
  int got;
  DT_Q(dttrmios_keyboard_scan,(ctl,
    &priv->trmios,
    keyboard->buffer + keyboard->n,
    keyboard->maxbuffer - keyboard->n,
    &got));
  keyboard->n += got;
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkeybd_unix_read(						/* get key from keyboard			*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_unix_read");
  priv_t *priv = 
    (priv_t *)keyboard->priv;
  int got;
  DT_Q(dttrmios_keyboard_read,(ctl,
    &priv->trmios,
    keyboard->buffer + keyboard->n,
    keyboard->maxbuffer - keyboard->n,
    &got));
  keyboard->n += got;
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkeybd_unix_uninit(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_unix_uninit");
  priv_t *priv = 
    (priv_t *)keyboard->priv;
  dt_rc_e rc = DT_RC_GOOD;
  DT_G(dttrmios_keyboard_uninit,(ctl,
    &priv->trmios));
  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));
  return rc;
}
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_unix_init(						/* set up keyboard					*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  const char *spec,
  long flags)
{
  DT_F("dtkeybd_unix_init");
#if DTPRJ_TERMIOS == DT_1
  priv_t *priv = NULL;
  dt_rc_e rc = DT_RC_GOOD;

    DT_C(dtos_malloc2,(ctl,				/* space for priv structure	    */
      (void **)&priv,
      sizeof(priv_t),
      F, "priv"));  

    DT_C(dttrmios_keyboard_init,(ctl,
      &priv->trmios, spec, 0));
	
    keyboard->priv = (void *)priv;
    keyboard->scan = dtkeybd_unix_scan;
    keyboard->read = dtkeybd_unix_read;
    keyboard->uninit = dtkeybd_unix_uninit;

  return DT_RC_GOOD;
cleanup:
  if (priv)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  return rc;
# else
  return dt_err_notlinked(ctl, F, "termios");
# endif
}






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
