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
| dt_ - dt
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/inter $RCSfile: texttcap.c,v $ $Revision: 1.1 $");
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/text.h>
#include <dtack/os.h>

#if DTPRJ_TERMCAP == DT_1

# ifndef DTDEPEND
#   include <termcap.h>
# endif

static dttext_end_f dttext_tcap_end;
static dttext_out_f dttext_tcap_out;
static dttext_key_f dttext_tcap_key;

typedef struct {
  dtkeybd_t keyboard;
} dttext_tcap_priv_t;

#endif

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttext_tcap_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *init)
{
  DT_F("dttext_tcap_beg");

# if DTPRJ_TERMCAP != DT_1
  return dt_err_notlinked(ctl, F,
    "tcap support");
# endif

# if DTPRJ_TERMCAP == DT_1
  dttext_tcap_priv_t *priv;

  DT_Q(dtos_malloc2,(ctl,				/* space for priv structure		*/
    (void **)&priv,
    sizeof(*priv),
    F, "priv"));
  memset(priv, 0, sizeof(*priv));

  DT_Q(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &priv->keyboard, 
    text->keyboard_name, 0));

  text->priv = priv;

  text->end = dttext_tcap_end;			/* set methods					    */
  text->out = dttext_tcap_out;
  text->key = dttext_tcap_key;

#ifdef NONO
  text->xe = COLS;						/* stuff we have to tell caller	    */
  text->ye = LINES;
#endif

  return DT_RC_GOOD;
# endif
}

#if DTPRJ_TERMCAP == DT_1
/*..........................................................................*/
static
dt_rc_e
dttext_tcap_end(						/* end operations					*/
  dt_ctl_t *ctl,
  dttext_t *text)
{
  DT_F("dttext_tcap_end");
  dttext_tcap_priv_t *priv =
    (dttext_tcap_priv_t *)text->priv;
  dt_rc_e rc = DT_RC_GOOD;

#ifdef NONO
  wmove(stdscr, text->ye-1, 0);
  wclrtoeol(stdscr);
  wrefresh(stdscr);
  endwin();
  fflush(stdout);
#endif

  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &priv->keyboard));

  DT_I(dtos_free2,(ctl, priv,		/* free our priv structure	    */
    F, "priv"));  
  
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttext_tcap_out(						/* output text at position			*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *string,
  int x,
  int y,
  unsigned long flags)
{
/*DT_F("dttext_tcap_out");*/

#ifdef NONO
  wmove(stdscr, y, x);					/* redisplay contents highlighted   */
  if (flags & DTTEXT_FLAG_NORMAL ||
      flags & DTTEXT_FLAG_HIGHLIGHT)
  {
    if (flags & DTTEXT_FLAG_HIGHLIGHT)
      wstandout(stdscr);
    waddstr(stdscr, string);			/* output string				    */
    if (flags & DTTEXT_FLAG_HIGHLIGHT)
	{
      wstandout(stdscr);				/* PC tcap needs this to turn off	*/
      wstandend(stdscr);
	}
  }
  else
  if (flags & DTTEXT_FLAG_CURSOR)		/* cursor placement causes refresh	*/
    wrefresh(stdscr);
#endif
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_tcap_key(						/* take keyboard input				*/
  dt_ctl_t *ctl,
  dttext_t *text,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dttext_tcap_key");
  dttext_tcap_priv_t *priv =
    (dttext_tcap_priv_t *)text->priv;

  DT_Q(dtkeybd_read,(ctl,			/* get key from keyboard interface	*/
    &priv->keyboard, key));

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
