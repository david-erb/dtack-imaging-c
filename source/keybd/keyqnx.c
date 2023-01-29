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

DT_RCSID("source/keybd $RCSfile: keyqnxterm.c,v $ $Revision: 1.1 $");

extern dtkeybd_init_f dtkeybd_qnxterm_init;

#if DTPRJ_QNXTERM == DT_1
#include <dtack/ipc.h>
#include <dtack/qnx4.h>

#ifndef DTDEPEND
#  include <sys/qnxterm.h>
#endif

static dtkeybd_scan_f dtkeybd_qnxterm_scan;
static dtkeybd_read_f dtkeybd_qnxterm_read;
static dtkeybd_uninit_f dtkeybd_qnxterm_uninit;

typedef struct {
  int dummy;
} private_t;

/*..........................................................................*/
static
dt_rc_e
dtkeybd_qnxterm_scan(					/* get key from keyboard, no wait	*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_qnxterm_scan");
  private_t *private = 
    (private_t *)keyboard->private;

  if (kbhit())
    DT_Q(dtkeybd_qnxterm_read,(			/* actually get the keys			*/
      ctl, keyboard));
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkeybd_qnxterm_read(					/* get key from keyboard			*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_qnxterm_read");
  private_t *private = 
    (private_t *)keyboard->private;
  int k;

  k = term_key();						/* get key from qnxterm interface 	*/
  if (k & 0xff00)						/* extended key?					*/
  {
    keyboard->buffer[keyboard->n]   = 0xff;
    keyboard->buffer[keyboard->n+1] = k & 0xff;
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
dtkeybd_qnxterm_uninit(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dtkeybd_qnxterm_uninit");
  private_t *private = 
    (private_t *)keyboard->private;
  dt_rc_e rc = DT_RC_GOOD;

  if (DTQNX4_TERM(ctl))					/* qnx is in TERM mode? 			*/
  {
	term_restore();						/* restore state before term_load	*/
	ctl->misc_flags &=
      ~DT_MISC_FLAG_QNX4_TERM;
  }
  DT_I(dtos_free2,(ctl, private,
    F, "private"));
  return rc;
}
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeybd_qnxterm_init(						/* set up keyboard					*/
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  const char *spec,
  long flags)
{
  DT_F("dtkeybd_qnxterm_init");
#if DTPRJ_QNXTERM == DT_1
  private_t *private = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&private,
    sizeof(private_t),
    F, "private"));  

  if (!DTQNX4_TERM(ctl))				/* qnx is not in TERM mode?			*/
  {
	if (term_load() == -1)				/* start qnxterm running		    */
	{
	  rc = dt_err(ctl, F,
        "errno %d in term_load()",
         errno);
	  goto cleanup;
	}
	ctl->misc_flags |=
      DT_MISC_FLAG_QNX4_TERM;
  }
	
  keyboard->private = (void *)private;
  keyboard->scan = dtkeybd_qnxterm_scan;
  keyboard->read = dtkeybd_qnxterm_read;
  keyboard->uninit = dtkeybd_qnxterm_uninit;

  return DT_RC_GOOD;
cleanup:
  if (private)
    DT_I(dtos_free2,(ctl, private,
      F, "private"));
  return rc;
# else
  return dt_err_notlinked(ctl, F, "qnxterm");
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
