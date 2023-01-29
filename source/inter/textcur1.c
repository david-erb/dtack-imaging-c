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
DT_RCSID("source/inter $RCSfile: textcur1.c,v $ $Revision: 1.1 $");
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/text.h>
#include <dtack/os.h>

#if DTPRJ_CURSES == DT_1
  static dttext_end_f dttext_curses_end;
  static dttext_out_f dttext_curses_out;
  static dttext_clear_f dttext_curses_clear;
  static dttext_key_f dttext_curses_key;

  typedef struct {
    dtkeybd_t keyboard;
  } dttext_curses_priv_t;

# ifndef DTDEPEND
#   include <curses.h>
# endif

# ifdef DTCONFIG_IMPLIED_SUNOS
      int waddstr(WINDOW *, char *);
      int waddch(WINDOW *, int);
      int waddstr(WINDOW *, char *);
      int werase(WINDOW *);
      int wclrtoeol(WINDOW *);
      int wclear(WINDOW *);
      int wmove(WINDOW *, int, int);
      int wgetch(WINDOW *);
      int winsch(WINDOW *, int);
      int wdelch(WINDOW *);
      int wrefresh(WINDOW *);
      void endwin(void);
      WINDOW *newwin(int, int, int, int);
      WINDOW *subwin(WINDOW *, int, int, int, int);
      int delwin(WINDOW *);
#	  if DTPRJ_CURSES_SYSV == DT_1
        int cbreak(void);
        int noecho(void);
        int erasechar(void);
#     else
        int wstandout(WINDOW *);
        int wstandend(WINDOW *);
        int stty(int, struct sgttyb *);
#     endif
#  endif
#endif

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttext_curses_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *init)
{
  DT_F("dttext_curses_beg");

# if DTPRJ_CURSES != DT_1
  return dt_err_notlinked(ctl, F,
    "curses support");
# endif

# if DTPRJ_CURSES == DT_1
  dttext_curses_priv_t *priv;

  DT_Q(dtos_malloc2,(ctl,				/* space for priv structure			*/
    (void **)&priv,
    sizeof(*priv),
    F, "priv"));
  memset(priv, 0, sizeof(*priv));

  DT_Q(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &priv->keyboard, 
    text->keyboard_name, 0));

  text->priv = priv;
  text->keyboard = &priv->keyboard;		/* so upper levels in print maps 	*/

  text->end = dttext_curses_end;		/* set methods					    */
  text->out = dttext_curses_out;
  text->clear = dttext_curses_clear;
  text->key = dttext_curses_key;

  initscr();
  cbreak();
  noecho();

# ifdef F_GRAY							/* this curses has colors?		    */
	wattrset(stdscr, F_GRAY|B_BLACK);
# endif

  text->xe = COLS - 1;					/* stuff we have to tell caller	    */
  text->ye = LINES;

  return DT_RC_GOOD;
# endif
}

#if DTPRJ_CURSES == DT_1
/*..........................................................................*/
static
dt_rc_e
dttext_curses_end(						/* end operations					*/
  dt_ctl_t *ctl,
  dttext_t *text)
{
  DT_F("dttext_curses_end");
  dttext_curses_priv_t *priv =
    (dttext_curses_priv_t *)text->priv;
  dt_rc_e rc = DT_RC_GOOD;

  wmove(stdscr, text->ye-1, 0);
  wclrtoeol(stdscr);
  wrefresh(stdscr);
  endwin();
  fflush(stdout);

  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &priv->keyboard));

  DT_I(dtos_free2,(ctl, priv,			/* free our priv structure	    	*/
    F, "priv"));  
  
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttext_curses_out(						/* output text at position			*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *string,
  int x,
  int y,
  unsigned long flags)
{
/*DT_F("dttext_curses_out");*/

  wmove(stdscr, y, x);					/* redisplay contents highlighted   */
  if (flags & DTTEXT_FLAG_NORMAL ||
      flags & DTTEXT_FLAG_HIGHLIGHT)
  {
    if (flags & DTTEXT_FLAG_HIGHLIGHT)
      wstandout(stdscr);
    waddstr(stdscr, (char *)string);	/* output string				    */
    if (flags & DTTEXT_FLAG_HIGHLIGHT)
	{
      wstandout(stdscr);				/* PC curses needs this to turn off	*/
      wstandend(stdscr);
	}
  }
  else
  if (flags & DTTEXT_FLAG_CURSOR)		/* cursor placement causes refresh	*/
  {
    wrefresh(stdscr);
    fflush(stdout);
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_curses_clear(					/* clear screen						*/
  dt_ctl_t *ctl,
  dttext_t *text)
{

  wrefresh(stdscr);
  wclear(stdscr);
  fflush(stdout);
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_curses_key(						/* take keyboard input				*/
  dt_ctl_t *ctl,
  dttext_t *text,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dttext_curses_key");
  dttext_curses_priv_t *priv =
    (dttext_curses_priv_t *)text->priv;

  wrefresh(stdscr);

  if (iskey)
    DT_Q(dtkeybd_scan,(ctl,				/* get key with no waiting			*/
      &priv->keyboard, iskey, key))
  else
    DT_Q(dtkeybd_read,(ctl,				/* wait for key						*/
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
