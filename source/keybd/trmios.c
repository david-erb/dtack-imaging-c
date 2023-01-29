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
DT_RCSID("source/keybd $RCSfile: keytrmio.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>

#if DTPRJ_TERMIOS == DT_1

#ifndef DTDEPEND
#include <termios.h>
#endif

#include <dtack/trmios.h>

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_init(					/* set up keyboard 					*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv,
  const char *spec,
  long flags)
{
  DT_F("dttrmios_keyboard_init");
  int sysrc;

    memset(priv, 0, 
      sizeof(*priv));

    priv->fd = fileno(stdin);			/* keyboard is assumed on stdin		*/

    sysrc = tcgetattr(priv->fd,		/* fetch current driver settings	*/
      &priv->old_tios);
    if (sysrc == -1)
      return dt_err(ctl, F,
        "tcgetattr(%d, &tios) errno %d",
        priv->fd, errno);

    priv->read_tios =					/* copy settings				    */
      priv->old_tios;

    priv->read_tios.c_lflag &= ~ICANON;	/* don't wait for return			*/
    priv->read_tios.c_lflag &= ~ISIG;	/* don't generate signals			*/
    priv->read_tios.c_lflag &= ~ECHO;	/* don't echo chars					*/
    priv->read_tios.c_lflag &= ~TOSTOP;	/* don't process job control    	*/
    priv->read_tios.c_lflag &= ~IEXTEN;	/* don't do extensions				*/

    priv->read_tios.c_cc[VMIN] = 10;	/* > longest escape sequence		*/
    priv->read_tios.c_cc[VTIME] = 1;	/* 100 ms inter-byte timeout   		*/
    priv->read_tios.c_cc[VINTR] = 0;	/* no interrupt character			*/
    priv->read_tios.c_cc[VQUIT] = 0;	/* no quit character				*/
    priv->read_tios.c_cc[VSTART] = 0;
    priv->read_tios.c_cc[VSTOP] = 0;
    priv->read_tios.c_cc[VSUSP] = 0;

/*	priv->read_tios.c_iflag &= ~INLCR;	/* no change NL to CR on input		*/
    priv->read_tios.c_iflag &= ~IGNCR;	/* don't ignore CR on input			*/
/*	priv->read_tios.c_iflag &= ~ICRNL;	/* no change CR to NL on input		*/
    priv->read_tios.c_iflag &= ~ISTRIP;	/* don't strip high bit				*/

/*  priv->read_tios.c_oflag &= ~OPOST;	/* don't expand tabs				*/

    priv->scan_tios =					/* copy settings			    	*/
      priv->read_tios;

    priv->scan_tios.c_cc[VMIN] = 0;		/* no characters is OK				*/
    priv->scan_tios.c_cc[VTIME] = 0;	/* immediate timeout				*/

    priv->state = DTTRMIOS_STATE_OLD;			/* state we are in				    */

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_uninit(				/* tear down keyboard				*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv)
{
  DT_F("dttrmios_keyboard_uninit");
  int sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv->state !=					/* not in the right state?		    */
	  DTTRMIOS_STATE_OLD)
  {
	sysrc = tcsetattr(priv->fd,			/* set old driver settings			*/
      TCSANOW, &priv->old_tios);
	if (sysrc == -1)
	  rc = dt_err(ctl, F,
        "tcsetattr(%d, TCSANOW,"
        " old_state) errno %d",
        priv->fd, errno);
  }

  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_scan(					/* get key from keyboard, no wait	*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv,
  char *buffer,
  int max,
  int *got)
{
  DT_F("dttrmios_keyboard_scan");
  int sysrc;

  if (priv->state !=					/* not in the right state?		    */
	  DTTRMIOS_STATE_SCAN)
  {
	sysrc = tcsetattr(priv->fd,			/* set for no waiting				*/
      TCSANOW,
      &priv->scan_tios);
	if (sysrc == -1)
      return dt_err(ctl, F,
        "tcsetattr(%d, TCSANOW,"
        " &tios) errno %d",
        priv->fd, errno);
	priv->state = DTTRMIOS_STATE_SCAN;
  }

  sysrc = read(priv->fd,				/* read from device				    */
    buffer, max);
  if (sysrc == -1)
    return dt_err(ctl, F,
      "read(%d, 0x%08lx, %d) errno %d",
      priv->fd, buffer, max);

  *got = sysrc;

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttrmios_keyboard_read(					/* get key from keyboard			*/
  dt_ctl_t *ctl,
  dttrmios_keyboard_t *priv,
  char *buffer,
  int max,
  int *got)
{
  DT_F("dttrmios_keyboard_read");
  int sysrc;

  DT_Q(dttrmios_keyboard_scan,(ctl,		/* pick up any pending characters 	*/
    priv, buffer, max, got));

  if (*got)								/* got stuff already? 				*/
    return DT_RC_GOOD;

  if (priv->state !=					/* not in the right state?		    */
	  DTTRMIOS_STATE_READ)
  {	
	sysrc = tcsetattr(priv->fd,			/* set new driver settings			*/
      TCSANOW,
      &priv->read_tios);
	if (sysrc == -1)
	  return dt_err(ctl, F,
        "tcsetattr(%d, TCSANOW,"
        " &tios) errno %d",
        priv->fd, errno);
	priv->state = DTTRMIOS_STATE_READ;
  }

  sysrc = read(priv->fd,				/* read from device				    */
    buffer, max);
  if (sysrc == -1)
    return dt_err(ctl, F,
      "read(%d, 0x%08lx, %d) errno %d",
      priv->fd, buffer, max);

  *got = sysrc;

  return DT_RC_GOOD;
}

#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
