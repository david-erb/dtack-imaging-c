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
  CHANGES
  19981014 DE don't clobber mouse location
 *..........................................................................*/



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

/*..........................................................................
 * dtscr interface implemented on error calls
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: scrhws.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/hws.h>
#include <dtack/scr.h>

#include <dtack/str.h>
#include <dtack/mem.h>

typedef struct {
  dthws_t hws;
} priv_t;

typedef struct {
  dthws_win_t hws_win;
} win_priv_t;

#define DESC_WIN_DATA "data for window \"%s\""
#define DESC_WIN_STRUCT "struct for window \"%s\""

static dtscr_create_window_f dtscr_hws_win_create;
static dtscr_wait_event_f dtscr_hws_wait_event;
static dtscr_handle_event_f dtscr_hws_handle_event;
static dtscr_destroy_window_f dtscr_hws_win_destroy;
static dtscr_destroy_f dtscr_hws_destroy;
static dtscr_export_data_f dtscr_hws_export_data;
static dtscr_import_data_f dtscr_hws_import_data;
static dtscr_export_color_f dtscr_hws_export_color;
static dtscr_import_color_f dtscr_hws_import_color;
static dtscr_refresh_f dtscr_hws_refresh;
static dtscr_redraw_f dtscr_hws_redraw;

static dtscr_raise_f dtscr_hws_win_raise;
static dtscr_lower_f dtscr_hws_win_lower;
static dtscr_print_f dtscr_hws_win_print;
static dtscr_iconify_f dtscr_hws_win_iconify;
static dtscr_move_f dtscr_hws_win_move;


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_hws_create(						/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *server)
{
  DT_F("dtscr_hws_create");
  priv_t *priv;
  dt_rc_e rc;

  DT_Q(dtos_malloc2,(ctl,				/* space for priv stuff		    */
    (void **)&priv, 
    sizeof(*priv),
    F, "priv"));

  DT_MEMSET(priv, 0, sizeof(*priv));

  DT_G(dthws_connect,(ctl, 				/* connect to hws server			*/
    &priv->hws, server));
  if (rc != DT_RC_GOOD)
  {
    DT_Q(dtos_free2,(ctl, priv,
      F, "priv"));
    return rc;
  }

  scr->have_mouse = 1;
  scr->have_window_manager = 1;
  scr->xe = priv->hws.xe;				/* inform caller of display size    */
  scr->ye = priv->hws.ye;
  scr->must_remap = priv->hws.do_remap;

  scr->priv =        priv;
  scr->destroy =        dtscr_hws_destroy;
  scr->create_window =  dtscr_hws_win_create;
  scr->wait_event =     dtscr_hws_wait_event;
  scr->handle_event =     dtscr_hws_handle_event;
  scr->destroy_window = dtscr_hws_win_destroy;
  scr->export_data =    dtscr_hws_export_data;
  scr->import_data =    dtscr_hws_import_data;
  scr->export_color =   dtscr_hws_export_color;
  scr->import_color =   dtscr_hws_import_color;
  scr->refresh =        dtscr_hws_refresh;
  scr->redraw =         dtscr_hws_redraw;
  scr->raise =          dtscr_hws_win_raise;
  scr->lower =          dtscr_hws_win_lower;
  scr->print =          dtscr_hws_win_print;
  scr->iconify =        dtscr_hws_win_iconify;
  scr->move =           dtscr_hws_win_move;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_create(					/* create new window				*/
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
  DT_F("dtscr_hws_win_create");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv;

  DT_Q(dtos_malloc2,(ctl, 
    (void **)&win_priv,
    sizeof(*win_priv),
    F, "priv"));

  win->priv = win_priv;					/* let caller hold instance pointer	*/
  
  DT_Q(dthws_win_create,(ctl,			/* do the method				    */
    &priv->hws, 
    &win_priv->hws_win,
    windowname,
    x0, y0, xe, ye, bits));

  win->x0 = win_priv->hws_win.x0;		/* return actual screen location	*/
  win->y0 = win_priv->hws_win.y0;

  win->id = win_priv->hws_win.id;		/* identify the window for scr	    */

  DT_Q(dthws_win_clear,(ctl,			/* clear guts of window				*/
    &priv->hws,
    &win_priv->hws_win,
    scr->black));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_destroy(					/* destroy window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_hws_win_destroy");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_win_destroy,(ctl,			/* do the method				    */
    &priv->hws, 
    &win_priv->hws_win));

  DT_Q(dtos_free2,(ctl, 
    win->priv,
    F, "priv"));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_export_data(					/* export patch into window			*/
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
  DT_F("dtscr_hws_export_data");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  if (win_priv != NULL)				/* not a utility window?		    */
    DT_Q(dthws_export_data,(ctl,
      &priv->hws, 
      &win_priv->hws_win,
      data, x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_import_data(				/* import patch from window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  int x0,
  int y0,
  int xe,
  int ye,
  void *data)
{
  DT_F("dtscr_hws_import_data");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_import_data,(ctl,
    &priv->hws, 
    &win_priv->hws_win,
    data, x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_export_color(					/* put window's colormap			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,						/* this may be NULL 				*/
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtscr_hws_export_color");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)
    (win? win->priv: NULL);

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  DT_Q(dthws_export_color,(ctl,			/* do method					    */
    &priv->hws, 
    (dthws_win_t *)(win_priv? &win_priv->hws_win: NULL),
    lut,
    start, count));
  return DT_RC_GOOD;
}

/*..........................................................................
 * win may be NULL indicating that caller wants base colormap
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_import_color(					/* get window's colormap			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtscr_hws_import_color");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = 
    (win_priv_t *)(win? win->priv: NULL);

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  DT_Q(dthws_import_color,(ctl,			/* do method					    */
    &priv->hws, 
    (dthws_win_t *)(win? &win_priv->hws_win: NULL),
    lut,
    start, count));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_wait_event(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dtscr_hws_wait_event");
  priv_t *priv = (priv_t *)scr->priv;

  do {
    DT_Q(dthws_wait_event,(ctl,			/* get input from hws		    	*/
      &priv->hws, svc, timeout, flag,
      event));
  } while(event->type ==				/* ignore mouse move events 		*/
      DT_EVENT_MOUSEMOVE);

#if 0
  event->mousex = scr->cursorx;			/* ignore mouse position info 		*/
  event->mousey = scr->cursory;
#endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_handle_event(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  void *buffer,
  int *handled,
  dt_event_t *event)
{
  DT_F("dtscr_hws_handle_event");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dthws_handle_event,(ctl,			/* get input from hws		    	*/
    &priv->hws, buffer, handled,
    event));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_destroy(						/* break down connection			*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_hws_destroy");
  priv_t *priv = (priv_t *)scr->priv;

  DT_Q(dthws_disconnect,(ctl,
    &priv->hws));

  DT_Q(dtos_free2,(ctl, 
    priv, F, "priv"));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_raise(					/* raise window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_hws_win_raise");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_win_raise,(ctl,			/* do the method				    */
    &priv->hws, 
    &win_priv->hws_win));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_lower(					/* lower window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_hws_win_lower");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_win_lower,(ctl,			/* do the method				    */
    &priv->hws,
    &win_priv->hws_win));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_print(					/* print window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)
{
  DT_F("dtscr_hws_win_print");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_win_print,(ctl,			/* do the method				    */
    &priv->hws,
    &win_priv->hws_win, printer, ppm,
    submitted));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_iconify(					/* iconify window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_hws_win_iconify");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_win_iconify,(ctl,			/* do the method				    */
    &priv->hws, 
    &win_priv->hws_win));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_win_move(						/* move window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win,
  int x,
  int y)
{
  DT_F("dtscr_hws_win_move");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;

  DT_Q(dthws_win_move,(ctl,				/* do the method				    */
    &priv->hws, 
    &win_priv->hws_win,
    x, y));

  return DT_RC_GOOD;
}

/*..........................................................................
 * refreshing an scr which is implemented on an hws means just looping
 *   backwards through the scr's window list, raising each
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_refresh(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_hws_refresh");
  priv_t *priv = (priv_t *)scr->priv;
  dtlist_elem_t *elem;
  elem = DTLIST_ELEM_TAIL;				/* start at the "backest" window	*/

  do {
    DT_Q(dtlist_prev,(ctl,				/* get next window to front		    */
      &scr->windowlist, elem, &elem));
    if (elem)							/* we're not at the top?		    */
	{
      dtscr_win_t *scr_win =			/* reference list element		    */
        (dtscr_win_t *)elem->user;
      win_priv_t *win_priv =			/* priv part for scr_hws		    */
        (win_priv_t *)scr_win->priv;

      DT_Q(dthws_win_raise,(ctl,		/* do the method				    */
        &priv->hws, 
        &win_priv->hws_win));
	}
  } while(elem);

  return DT_RC_GOOD;
}

/*..........................................................................
 * redrawing a scr window which is implemented with hws means we take
 *   advantage of the complete bitmap of the window as provided by scr
 *..........................................................................*/

static
dt_rc_e
dtscr_hws_redraw(						/* draw window from scr's bitmap	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_win_t *win)
{
  DT_F("dtscr_hws_redraw");
  priv_t *priv = (priv_t *)scr->priv;
  win_priv_t *win_priv = (win_priv_t *)win->priv;
  dtmemalloc_t *memalloc = 
    (dtmemalloc_t *)win->mem;			/* take advantage of scr's bitmap	*/
  dtxy_t y;
  long offset = 0;

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

    DT_Q(dthws_export_data,(ctl,		/* put the block on the hws	    	*/
      &priv->hws, 
      &win_priv->hws_win, p,
      0, y, 
      win->xe, nrows));

    DT_Q((memalloc->unmap),(ctl, 	  	/* unmap the block					*/
      memalloc, offset));
    offset += datasize;

    y += nrows;
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
