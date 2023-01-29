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
DT_RCSID("source/screen $RCSfile: scrcan.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>
#include <dtack/scr.h>

#include <dtack/mem.h>

typedef struct {
  dtcanvas_t canvas;
  long id_pool;
} priv_t;

  static dtscr_create_window_f dtscr_canvas_create_window;
  static dtscr_export_data_f dtscr_canvas_export_data;
  static dtscr_import_data_f dtscr_canvas_import_data;
  static dtscr_export_color_f dtscr_canvas_export_color;
  static dtscr_import_color_f dtscr_canvas_import_color;
  static dtscr_wait_event_f dtscr_canvas_wait_event;
  static dtscr_destroy_window_f dtscr_canvas_win_destroy;
  static dtscr_destroy_f dtscr_canvas_destroy;
  static dtscr_refresh_f dtscr_canvas_refresh;
  static dtscr_redraw_f dtscr_canvas_redraw;
  static dtscr_raise_f dtscr_canvas_raise;
  static dtscr_lower_f dtscr_canvas_lower;
/*static dtscr_iconify_f dtscr_canvas_iconify;*/
/*static dtscr_move_f dtscr_canvas_move;*/
  static dtscr_colormap_f dtscr_canvas_colormap;
  static dtscr_cursor_f dtscr_canvas_cursor;

/*..........................................................................
 * redrawing a scr window which is implemented with canvas means we take
 *   advantage of the complete bitmap of the window as provided by scr
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_redraw(					/* draw window from scr's bitmap	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_canvas_redraw");
  priv_t *priv = (priv_t *)scr->priv;
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)win->mem;			/* take advantage of scr's bitmap	*/
  dtxy_t y;
  long offset = 0;
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "redrawing window \"%s\"",
	win->name);

  y = 0;
  while (y<win->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DT_Q((memalloc->get),(ctl, 			/* get the next block				*/
      memalloc, offset,
      &p, &datasize));
    nrows = datasize / win->xe;			/* how many rows in block			*/

  if (scr->must_remap)
  DT_Q_1D(dt1d_lookup_inplace_uchar,(
    (unsigned char *)p, 
    (unsigned int)win->xe * 
    (unsigned int)win->ye,
    win->luts->user_to_display));

    DT_Q(dtcanvas_putblock,(ctl,		/* put the block on the canvas	    */
      &priv->canvas, p,
      win->x0, win->y0+y, 
      win->xe, nrows));

    DT_Q((memalloc->unmap),(ctl, 	  	/* unmap the block					*/
      memalloc, offset));
    offset += datasize;

    y += nrows;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_canvas_create(					/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *server)
{
  DT_F("dtscr_canvas_create");
  dtcanvas_t canvas;
  priv_t *priv;

  DT_Q(dtcanvas_init,(ctl, &canvas,		/* init canvas structure		    */
    server));

  DT_Q(dtcanvas_open,(ctl, &canvas));	/* open canvas						*/

  DT_Q(dtos_malloc2,(ctl,				/* space for priv stuff		    	*/
    (void **)&priv, 
    sizeof(*priv),
    F, "priv"));

  priv->canvas = canvas;				/* init priv stuff			    	*/
  priv->id_pool = 1000;					/* begin id sequence numbers	    */
  scr->must_remap = canvas.must_remap;	/* tell caller to remap colors	    */

  scr->xe = canvas.xe;					/* inform caller of canvas size	    */
  scr->ye = canvas.ye;
  scr->priv = priv;
  scr->create_window =  dtscr_canvas_create_window;
  scr->export_data =    dtscr_canvas_export_data;
  scr->import_data =    dtscr_canvas_import_data;
  scr->export_color =   dtscr_canvas_export_color;
  scr->import_color =   dtscr_canvas_import_color;
  scr->wait_event =     dtscr_canvas_wait_event;
  scr->destroy_window = dtscr_canvas_win_destroy;
  scr->destroy =        dtscr_canvas_destroy;
  scr->refresh =        dtscr_canvas_refresh;
  scr->redraw =         dtscr_canvas_redraw;
  scr->raise =          dtscr_canvas_raise;
  scr->lower =          dtscr_canvas_lower;
/*scr->iconify =        dtscr_canvas_iconify;*/
/*scr->move =           dtscr_canvas_move;*/
  scr->colormap =       dtscr_canvas_colormap;
  scr->cursor =         dtscr_canvas_cursor;


  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_create_window(				/* create new window				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)
{
  DT_F("dtscr_win_create_window");
  priv_t *priv = (priv_t *)scr->priv;

  if (x0 == -1 && y0 == -1)				/* interactively position?		    */
    win->x0 = win->y0 = 10;				/* forget it for now			    */

  if (!scr->refresh_pending)
    DT_Q(dtscr_canvas_redraw,(ctl,		/* just redraw it				    */
      scr, win));

  win->id = priv->id_pool++;			/* give it a new id				    */

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_export_data(				/* export patch into window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits,
  void *data)
{
  DT_F("dtscr_canvas_export_data");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dtcanvas_putblock,(ctl,
    &priv->canvas, data,
    (int)(win->x0+x0),
    (int)(win->y0+y0), 
    (int)xe, (int)ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_import_data(				/* import patch from window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  int x0,
  int y0,
  int xe,
  int ye,
  void *data)
{
  DT_F("dtscr_canvas_import_data");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dtcanvas_getblock,(ctl,
    &priv->canvas, data,
    (int)(win->x0+x0),
    (int)(win->y0+y0), 
    (int)xe, (int)ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 * exporting color for window only changes canvas's display colors if
 * the window is on top of the list
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_export_color(				/* export color into window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtscr_canvas_export_color");
  priv_t *priv = (priv_t *)scr->priv;
  dtlist_elem_t *elem;
  elem = DTLIST_ELEM_HEAD;
  DT_Q(dtlist_next,(ctl,				/* fetch top window in list	    	*/
    &scr->windowlist, elem, &elem));
  if (win == NULL ||
	  (dtscr_win_t *)elem->user == win)	/* target window is the top? 		*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_LUT,
      "setting canvas's display lut"
      " because window \"%s\" is top",
      win? win->name: "NULL");
    dt_dbg(ctl, F, DT_DBG_MASK_LUT,
      "count %d, rgb[%d] %d,%d,%d",
      count, start,
      lut[0].r, lut[0].g, lut[0].b);
    DT_Q(dtcanvas_putcolor,(ctl,		/* change canvas's display lut 		*/
      &priv->canvas, lut, 
      start, count));
  }
  else
    dt_dbg(ctl, F, DT_DBG_MASK_LUT,
      "not setting canvas's display lut"
      " because window \"%s\" is not top",
      win->name);
  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_import_color(				/* import color into window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtscr_canvas_import_color");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dtcanvas_getcolor,(ctl,			/* base canvas color 				*/
    &priv->canvas, 
    lut, start, count));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_wait_event(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dtscr_canvas_wait_event");
  priv_t *priv = (priv_t *)scr->priv;
  int mousex, mousey;
  dtscr_win_t *win;
  dtlist_elem_t *elem;

  DT_Q(dtcanvas_input,(ctl,				/* get input from canvas		    */
    &priv->canvas, svc, timeout,
    flag, event));

  if (event->type == DT_EVENT_IPC)		/* ipc is special				    */
  {										/* no window finding				*/
  }
  else
  if (event->type == 0)					/* nothing happened?				*/
  {
	event->type = DT_EVENT_NOTHING;		/* ignore it 						*/
  }
  else
  if (event->type != DT_EVENT_NOTHING)
  {
    mousex = scr->cursorx;				/* use our own cursor only          */
    mousey = scr->cursory;

    elem = DTLIST_ELEM_HEAD;
    while (elem)
    {
      DT_Q(dtlist_next,(ctl,			/* fetch next window in list	    */
        &scr->windowlist, elem,
        &elem));
      if (elem)							/* not the last in the list?	    */
      {
        win = (dtscr_win_t *)elem->user;
        if (mousex >= win->x0 &&
            mousex <  win->x0+win->xe &&
            mousey >= win->y0 &&
            mousey <  win->y0+win->ye)
  	    break;
      }
    } 

    if (elem)							/* on a window?						*/
      event->id = win->id;
    else								/* not on a window				    */
      event->id = 0;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_destroy(					/* break down connection			*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_canvas_destroy");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dtcanvas_close,(ctl,
    &priv->canvas));

  DT_Q(dtcanvas_uninit,(ctl,
    &priv->canvas));

  DT_Q(dtos_free2,(ctl, 
    priv, F, "priv"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * raising a scr window which is implemented with canvas means redrawing
 *   the window
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_raise(						/* draw window in front of others	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_canvas_raise");
  DT_Q(dtscr_canvas_redraw,(ctl,		/* redraw window					*/
    scr, win)); 
  DT_Q(dtscr_canvas_colormap,(ctl,
    scr, win));
  return DT_RC_GOOD;
}

/*..........................................................................
 * raising a scr window which is implemented with canvas means redrawing
 *   all the windows -- presumes list order is correct
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_lower(						/* draw window behind others		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  scr->refresh_pending = 1;				/* refresh at next input		    */
  return DT_RC_GOOD;
}

/*..........................................................................
 * setting colormap of a window which is implemented with canvas means
 *   setting the canvas display color map to be that of the window
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_colormap(					/* set colormap for window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_canvas_colormap");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dtcanvas_putcolor,(ctl,			/* change canvas display colors 	*/
    &priv->canvas, 
    win &&								/* not wanting base colormap?		*/
    win->luts?							/* and window has lut allocated? 	*/
      win->luts->current:				/* use window's lut 				*/
	  scr->display_lut,					/* otherwise use base lut 			*/
    0, 256));
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "setting canvas's display lut"
    " to that of window \"%s\"",
    win? win->name: "NULL");
  return DT_RC_GOOD;
}

/*..........................................................................
 * refreshing an scr which is implemented on a canvas means clearing the
 *   canvas then looping through the scr's window list, redrawing each
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_refresh(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_canvas_refresh");
  priv_t *priv = (priv_t *)scr->priv;
  dtlist_elem_t *elem;
  dtscr_win_t *win = NULL;

  elem = DTLIST_ELEM_TAIL;				/* start at the "backest" window	*/

  DT_Q(dtcanvas_clear,(ctl, 			/* clear canvas to background color	*/
    &priv->canvas,
    scr->black));

  do {
    DT_Q(dtlist_prev,(ctl,				/* get next window to front		    */
      &scr->windowlist, elem, &elem));
    if (elem)							/* we're not at the top?		    */
	{
      win = (dtscr_win_t *)elem->user;
      DT_Q(dtscr_canvas_redraw,(ctl, 	/* redraw window					*/
        scr, win));
	}
  } while(elem);

  DT_Q(dtscr_canvas_colormap,(ctl,		/* set colormap of top window	    */
    scr, win));

  return DT_RC_GOOD;
}



/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_cursor(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int *x,
  int *y,
  int op)
{
  DT_F("dtscr_canvas_cursor");
  priv_t *priv = (priv_t *)scr->priv;
  DT_Q(dtcanvas_cursor,(ctl,
    &priv->canvas, x, y, op));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_canvas_win_destroy(				/* destroy window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  scr->refresh_pending = 1;				/* refresh at next input		    */
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
