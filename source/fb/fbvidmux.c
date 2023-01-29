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
| DESCRIPTION
| As yet undocumented.
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
DT_RCSID("source/fb $RCSfile: fbvidmux.c,v $ $Revision: 1.7 $");
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/portio.h>
#include <dtack/vidmux.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_vidmux_parse(						/* parse vidmux						*/
  dt_ctl_t *ctl,
  const char *source,
  long *port,
  long *value,
  long *delay)
{
  DT_F("dtfb_vidmux_parse");
  char *s = NULL;
  char *p;
  int n;
  dt_rc_e rc;

  if (source == NULL)
    goto cleanup;

  DT_C(dtos_strdup,(ctl, source, &s));	/* local copy to edit 				*/
  DT_C(dtstr_tokenize,(ctl, s, ',',		/* separate into tokens 			*/
    &n));

  if (n != 2 && n != 3)					/* need exactly 2 or 3 tokens */
  {
	rc = dt_err(ctl, F,
      "\"%s\" is not valid for vidmux",
      source);
	goto cleanup;
  }

  p = s;
  DT_C(dtstr_to_long,(ctl, p, port));	/* vidmux I/O address 				*/

  p = p + strlen(p) + 1;
  DT_C(dtstr_to_long,(ctl, p, value));	/* vidmux switch value 				*/

  if (n == 3)							/* third token given? 				*/
  {
	p = p + strlen(p) + 1;
	DT_C(dtstr_to_long,(ctl, p,			/* delay after vidmux switch, */
      delay));
  }
  else									/* no third token given? 			*/
    *delay = 0;

  dt_dbg(ctl, F, 
    DT_DBG_MASK_SCHEDULE,
    "vidmux 0x%03x switch to %d,"
    " delay %ld",
    *port, *value, *delay);

cleanup:
  if ( s != NULL)
    DT_I(dtos_free2,(ctl, s,
      F, "source string"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtfb_vidmux_switch(						/* switch vidmux					*/
  dt_ctl_t *ctl,
  dtportio_t *portio,
  long port,
  long value)
{
  DT_F("dtfb_vidmux_switch");
  while(1)
  {
	if (value < 4)						/* on the right vidmux? 			*/
	{
	  DT_Q(dtportio_outb,(ctl,			/* poke the port 					*/
        portio, 
        (int)port, (int)value));
	  break;
	}
	else								/* on an upstream vidmux? 			*/
	{
	  DT_Q(dtportio_outb,(ctl,			/* poke the port 					*/
        portio, 
        (int)port, 3));
	  port++;
	  value -= 4;
	}
  }
  return DT_RC_GOOD;
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
