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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: scr_cur.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/mem.h>

static const char cursor_data1[256] = {	/* arrow cursor image			    */
  1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,
  1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
  1,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,
  1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,
  1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,
  1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,
  1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,
  1,2,2,2,2,2,1,1,1,1,1,0,0,0,0,0,
  1,2,2,1,2,2,1,0,0,0,0,0,0,0,0,0,
  1,2,1,0,1,2,2,1,0,0,0,0,0,0,0,0,
  1,1,0,0,1,2,2,1,0,0,0,0,0,0,0,0,
  0,0,0,0,0,1,2,2,1,0,0,0,0,0,0,0,
  0,0,0,0,0,1,2,2,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0
};

static const char cursor_data2[256] = {	/* latched cursor image			    */
  0,0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,
  0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,
  0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,
  0,0,0,0,1,1,1,2,1,1,1,0,0,0,0,0,
  0,0,1,0,0,0,1,2,1,0,0,0,1,0,0,0,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,0,
  1,2,2,1,1,1,1,2,1,1,1,1,2,2,1,0,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,
  1,2,2,1,1,1,1,2,1,1,1,1,2,2,1,0,
  0,1,2,1,0,0,1,2,1,0,0,1,2,1,0,0,
  0,0,1,0,0,0,1,2,1,0,0,0,1,0,0,0,
  0,0,0,0,1,1,1,2,1,1,1,0,0,0,0,0,
  0,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,
  0,0,0,0,0,1,2,2,2,1,0,0,0,0,0,0,
  0,0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#ifdef NONO  
static const char cursor_data2[256] = {	/* latched cursor image			    */
  0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,
  0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,
  0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,
  1,1,1,1,2,2,2,2,2,1,1,1,1,0,0,0,
  1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
  1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
  1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
  1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
  1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
  1,1,1,1,2,2,2,2,2,1,1,1,1,0,0,0,
  0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,
  0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,0,
  0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
#endif

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_latch_restore(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_latch_restore");
  char *p =								/* temp space for flat buffer	    */
    (char *)scr->latchbuf +				/* after the dashed part		    */
    DT_MAX(scr->latch->xe, 
           scr->latch->ye);
  int xorg = 0;
  int yorg = 0;

  if (!scr->export_data)				/* no method?					    */
    return DT_RC_GOOD;

  DT_Q((scr->export_data),(ctl, scr,	/* top row						    */
    scr->latch, 
    xorg, yorg, scr->latch->xe, 1,
    8, p));
  p += scr->latch->xe;

  DT_Q((scr->export_data),(ctl, scr,	/* bottom row						*/
    scr->latch, 
    xorg, yorg+scr->latch->ye-1, 
    scr->latch->xe, 1, 
    8, p));
  p += scr->latch->xe;

  DT_Q((scr->export_data),(ctl, scr,	/* left column					    */
    scr->latch, 
    xorg, yorg, 1, scr->latch->ye, 
    8, p));
  p += scr->latch->ye;

  DT_Q((scr->export_data),(ctl, scr,	/* right column					    */
    scr->latch,
    xorg+scr->latch->xe-1, yorg, 
    1, scr->latch->ye, 
    8, p));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_latch_display(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_latch_display");
  char *p =								/* temp space for flat buffer	    */
    (char *)scr->latchbuf +				/* after the dashed part		    */
    DT_MAX(scr->latch->xe, 
           scr->latch->ye);
  int xorg = 0;
  int yorg = 0;

  if (!scr->export_data ||				/* no method?					    */
      !scr->import_data)
    return DT_RC_GOOD;

  DT_Q((scr->import_data),(ctl, scr,	/* top row						    */
    scr->latch, 
    xorg, yorg, scr->latch->xe, 1,
    p));
  p += scr->latch->xe;

  DT_Q((scr->import_data),(ctl, scr,	/* bottom row						*/
    scr->latch, 
    xorg, yorg+scr->latch->ye-1,
    scr->latch->xe, 1, 
    p));
  p += scr->latch->xe;

  DT_Q((scr->import_data),(ctl, scr,	/* left column					    */
    scr->latch, 
    xorg, yorg, 1, scr->latch->ye, 
    p));
  p += scr->latch->ye;

  DT_Q((scr->import_data),(ctl, scr,	/* right column					    */
    scr->latch,
    xorg+scr->latch->xe-1, yorg, 
    1, scr->latch->ye, 
    p));


  p = (char *)scr->latchbuf;			/* point at the dashed part		    */
  DT_Q((scr->export_data),(ctl, scr,	/* top row						    */
    scr->latch, 
    xorg, yorg, scr->latch->xe, 1,
    8, p));

  DT_Q((scr->export_data),(ctl, scr,	/* bottom row						*/
    scr->latch, 
    xorg, yorg+scr->latch->ye-1, 
    scr->latch->xe, 1, 
    8, p));

  DT_Q((scr->export_data),(ctl, scr,	/* left column					    */
    scr->latch, 
    xorg, yorg, 1, scr->latch->ye, 
    8, p));

  DT_Q((scr->export_data),(ctl, scr,	/* right column					    */
    scr->latch,
    xorg+scr->latch->xe-1, yorg, 
    1, scr->latch->ye, 
    8, p));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
dtscr_latch_draw(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int flag)
{
  DT_F("dtscr_latch_draw");

  if (scr->latchbuf)					/* something latched?			    */
  if (flag)
  {
    DT_Q(dtscr_latch_display,(ctl, scr));
  }
  else
  {
    DT_Q(dtscr_latch_restore,(ctl, scr));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
dtscr_latch(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_latch");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  if (scr->have_window_manager)			/* window manager to do this?		*/
    return DT_RC_GOOD;

  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  if (scr->latchbuf)					/* something latched?			    */
  {
    DT_Q(dtos_free2,(ctl,				/* free latch buffer			    */
      scr->latchbuf,
      F, "latch buffer"));
    scr->latchbuf = NULL;
    DT_Q(dtscr_refresh,(ctl, scr));		/* refresh all windows			    */
  }
  else									/* nothing latched?				    */
  {
    unsigned long *p;
    const unsigned long white =
      ((unsigned long)scr->white << 24L) | 
      ((unsigned long)scr->white << 16L) | 
      ((unsigned long)scr->white <<  8L) | 
      ((unsigned long)scr->white);
    const unsigned long black =
      ((unsigned long)scr->black << 24L) | 
      ((unsigned long)scr->black << 16L) | 
      ((unsigned long)scr->black <<  8L) | 
      ((unsigned long)scr->black);
    int i, n;

    DT_Q(dtscr_window_raise,(ctl,		/* raise the window				    */
      scr, windowname));
    scr->latch = win;					/* attach latch to this window	    */
    n = DT_MAX(win->xe, win->ye) + 		/* dashed part					  	*/
	  4 * sizeof(white) - 1;
    DT_Q(dtos_malloc2,(ctl,				/* space for outline buffer		    */
      &scr->latchbuf, n +
        2 * win->xe + 2 * win->ye,		/* under-latch save buffer		    */
      F, "latch buffer"));
    n = n / sizeof(white) + 1;
    p = (unsigned long *)scr->latchbuf;
    for (i=0; i<n; i+=2)				/* make dashed line segment		    */
	{
      p[i] = white;
      p[i+1] = black;
	}

    DT_Q(dtscr_cursor_move,(ctl,		/* move cursor into latch		    */
      scr, "", 0, 0));
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
dtscr_cursor_draw(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int flag)
{
  DT_F("dtscr_cursor_draw");
  dtscr_win_t *cursor_win;				/* window covering the cursor	    */
  char *p;
  const char *q;
  int xorg = 0;
  int yorg = 0;
  int i;
# define N (DTSCR_CURSOR_XSIZE * \
            DTSCR_CURSOR_YSIZE)
  const int n = N;
  int op;
  char tmp[N];

  if (flag)								/* not making cursor invisible?		*/
  {
    if (!scr->latchbuf)					/* nothing latched?				    */
      op = DT_CURSOR_OP_NORMAL;
	else
      op = DT_CURSOR_OP_DRAG;
  }
  else									/* making cursor invisible? 		*/
    op = DT_CURSOR_OP_OFF;

  if (scr->cursor)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "%s cursor at %d,%d",
      flag? "drawing": "erasing",
      scr->cursorx, scr->cursory);
    DT_Q(scr->cursor,(ctl, scr,			/* tell OS where cursor is 			*/
      &scr->cursorx, &scr->cursory, 
      op));
   }

  if (scr->have_mouse)					/* OS does mouse stuff?				*/
    return DT_RC_GOOD;

  cursor_win = scr->tmp_win;			/* use utility window			    */

  cursor_win->x0 = scr->cursorx;		/* init just enough of the window	*/
  cursor_win->y0 = scr->cursory;
  cursor_win->xe = DTSCR_CURSOR_XSIZE;
  cursor_win->ye = DTSCR_CURSOR_XSIZE;

  DT_MEMSET(tmp, 0, sizeof(tmp));
  if (flag)
  {
    const int white = scr->white;
    const int black = scr->black;

    if (scr->import_data)				/* has method?					    */
    DT_Q((scr->import_data),(ctl,		/* get small block under cursor	    */
      scr, cursor_win, 
      xorg, yorg, 
      DTSCR_CURSOR_XSIZE,
      DTSCR_CURSOR_YSIZE,
      scr->cursorbuf));
    DT_MEMCPY(tmp, scr->cursorbuf, n);		/* copy data to tmp as underlay	    */

    if (!scr->latchbuf)					/* nothing latched?				    */
      q = cursor_data1;					/* use arrow cursor				    */
    else								/* something latched?			    */
      q = cursor_data2;					/* use alternate cursor			    */

    for (i=0; i<n; i++)					/* overlay cursor image into tmp    */
	  if (q[i] == 1)					/* cursor edge?					    */
        tmp[i] = black;
      else
	  if (q[i] == 2)					/* cursor guts?					    */
        tmp[i] = white;
    p = tmp;							/* output small block with cursor	*/
  }
  else
    p = scr->cursorbuf;					/* output small block of old data	*/


  if (scr->export_data)					/* has method?					    */
  DT_Q((scr->export_data),(ctl,			/* output cursor					*/
    scr, cursor_win, 
    xorg, yorg, 
    DTSCR_CURSOR_XSIZE,
    DTSCR_CURSOR_YSIZE,
    8, p));

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_cursor_bump(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int dx,
  int dy)
{
  DT_F("dtscr_cursor_bump");

  if (strlen(windowname) != 0)
  {
    dtlist_elem_t *elem;
    dtscr_win_t *win;

    DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
      &scr->windowlist, 
      windowname, &elem));
    win = (dtscr_win_t *)elem->user;
	
	if (scr->move)						/* has move method? 				*/
	{
  	  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "bumping \"%s\" by %d,%d to %d,%d",
        windowname,
        dx, dy, 
        win->x0+dx, win->y0+dy);

      DT_Q((scr->move),(ctl, 
        scr, win, 
        win->x0+dx, win->y0+dy));
	}
	win->x0 += dx;
	win->y0 += dy;
  }
  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_cursor_move(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int dx,
  int dy)
{
  DT_F("dtscr_cursor_move");
  int bound;
  int old_cursorx = scr->cursorx;
  int old_cursory = scr->cursory;

  if (scr->latchbuf)					/* something is latched?		    */
  {
    scr->latch->x0 += dx;
    if (scr->latch->x0 < 0)				/* limit latch h motion		    	*/
      scr->latch->x0 = 0;
	if (scr->xe &&						/* screen size is known? 			*/
		scr->latch->x0+					/* attempting to move off right? 	*/
		scr->latch->xe >=
		scr->xe)
	  scr->latch->x0 = scr->xe -		/* keep latch in the screen 		*/
        scr->latch->xe - 1;

    scr->latch->y0 += dy;
    if (scr->latch->y0 < 0)				/* limit latch v motion		    	*/
      scr->latch->y0 = 0;
	if (scr->ye &&						/* screen size is known? 			*/
		scr->latch->y0+					/* attempting to move off bomb? 	*/
		scr->latch->ye >=
		scr->ye)
	  scr->latch->y0 = scr->ye -		/* keep latch in the screen 		*/
        scr->latch->ye - 1;
	
	bound = 1;							/* keep off the very edge 			*/

#ifdef NONO
    scr->cursorx = scr->latch->x0 +		/* center cursor in latch		    */
      scr->latch->xe/2 -
      DTSCR_CURSOR_XSIZE/2;				/* offset for latch cursor hotspot  */
    scr->cursory = scr->latch->y0 +
      scr->latch->ye/2 -
      DTSCR_CURSOR_YSIZE/2;
#endif
  }
  else									/* nothing latched?				    */
    bound = 0;							/* we can go right to the edge 		*/


  {
    scr->cursorx += dx;
    if (scr->cursorx <= bound)			/* limit cursor h motion		    */
       scr->cursorx = bound;
	if (scr->xe &&						/* screen size is known? 			*/
		scr->cursorx+					/* attempting to move off right? 	*/
        DTSCR_CURSOR_XSIZE >=
		scr->xe - bound)
      scr->cursorx = scr->xe -			/* keep cursor on screen 			*/
        DTSCR_CURSOR_XSIZE - 1 - bound;

    scr->cursory += dy;
    if (scr->cursory < bound)			/* limit cursor v motion		    */
      scr->cursory = bound;
	if (scr->ye &&						/* screen size is known? 			*/
		scr->cursory+					/* attempting to move off bottom? 	*/
        DTSCR_CURSOR_YSIZE >=
		scr->ye - bound)
      scr->cursory = scr->ye -			/* keep cursor on screen 			*/
        DTSCR_CURSOR_YSIZE - 1 - bound;
  }
  
  if (scr->have_mouse)					/* cursor move always means bump?	*/
    DT_Q(dtscr_cursor_bump,(ctl, scr,	/* bump window						*/
      windowname,
      scr->cursorx - old_cursorx,
      scr->cursory - old_cursory));


  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_cycle(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int direction)
{
  DT_F("dtscr_cycle");
  dtlist_elem_t *elem;
  dtscr_win_t *scr_win;

  if (direction == 1)
    DT_Q(dtlist_prev,(ctl,				/* get tail element				    */
      &scr->windowlist, 
      DTLIST_ELEM_TAIL, &elem))
  else
    DT_Q(dtlist_next,(ctl,				/* get head element				    */
      &scr->windowlist, 
      DTLIST_ELEM_HEAD, &elem));

  if (elem)								/* any elements in list?			*/
  {
    scr_win = (dtscr_win_t *)elem->user;
    scr->cursorx = scr_win->x0 +		/* move cursor into new window	    */
      scr_win->xe - 20;
    scr->cursory = scr_win->y0 + 
      scr_win->ye - 20;

    if (direction == 1)
      DT_Q(dtscr_window_raise,(ctl,		/* raise window on screen			*/
        scr, elem->name))
    else
      DT_Q(dtscr_window_lower,(ctl,		/* lower window on screen			*/
        scr, elem->name));
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
