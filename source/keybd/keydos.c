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

#if defined(DTCONFIG_DB0) || \
    defined(DTCONFIG_DT0) || \
    defined(DTCONFIG_DV0) || \
    defined(DTCONFIG_DW0) || \
    defined(DTCONFIG_DW1) || \
    defined(DTCONFIG_WV0) || \
    defined(DTCONFIG_WV4) || \
    defined(DTCONFIG_WV5)
#  ifndef DTDEPEND
#    include <conio.h>
#  endif
#endif

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>

DT_RCSID("source/keybd $RCSfile: keydos.c,v $ $Revision: 1.1 $");

extern dtkeybd_init_f dtkeybd_dos_init;

#if defined(DTCONFIG_IMPLIED_DOS) || \
    defined(DTCONFIG_IMPLIED_MSW32)		/* console mode windows			    */

static dtkeybd_scan_f dtkeybd_dos_scan;
static dtkeybd_read_f dtkeybd_dos_read;
static dtkeybd_uninit_f dtkeybd_dos_uninit;

typedef struct {
  int dummy;
} priv_t;

/*..........................................................................*/
static
dt_rc_e
dtkeybd_dos_scan(						/* get key from keyboard, no wait	*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_dos_scan");

  if (kbhit())
    DT_Q(dtkeybd_dos_read,(				/* actually get the keys			*/
      ctl, keyboard));
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkeybd_dos_read(						/* get key from keyboard			*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  int k;

  k = getch();							/* get key from dos interface	 	*/
  if (k == 0x00)						/* extended key?					*/
  {
    keyboard->buffer[keyboard->n]   = 0xff;
    keyboard->buffer[keyboard->n+1] = getch();
    keyboard->n += 2;
  }
  else
  {
    keyboard->buffer[keyboard->n] = k;
    keyboard->n += 1;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkeybd_dos_uninit(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_dos_uninit");
  priv_t *priv = 
    (priv_t *)keyboard->priv;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv)
  DT_I(dtos_free2,(ctl, priv,
    F, "priv"));
  
  return rc;
}
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_dos_init(						/* set up keyboard					*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  const char *spec,
  long flags)
{
#if defined(DTCONFIG_IMPLIED_DOS) || \
    defined(DTCONFIG_IMPLIED_MSW32)		/* console mode windows			    */

    keyboard->priv = NULL;
    keyboard->scan = dtkeybd_dos_scan;
    keyboard->read = dtkeybd_dos_read;
    keyboard->uninit = dtkeybd_dos_uninit;

  return DT_RC_GOOD;

# else
  DT_F("dtkeybd_dos_init");
  return dt_err_notlinked(ctl, F, "dos keyboard");
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
