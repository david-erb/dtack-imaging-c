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
DT_RCSID("source/screen $RCSfile: hwsqw4.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/hws.h>
#include <dtack/str.h>

#include <dtack/mem.h>

extern dthws_connect_f dthws_qw4_init;

#if DTPRJ_QW4 == DT_1
#define DTCONFIG_QNX4_QWTUNNEL

static int global_color = 0;
static int global_background = 0;

static dthws_disconnect_f dthws_qw4_disconnect;
static dthws_win_create_f dthws_qw4_win_create;
static dthws_wait_event_f dthws_qw4_wait_event;
static dthws_win_destroy_f dthws_qw4_win_destroy;
static dthws_export_data_f dthws_qw4_export_data;
static dthws_import_data_f dthws_qw4_import_data;
static dthws_export_color_f dthws_qw4_export_color;
static dthws_import_color_f dthws_qw4_import_color;
static dthws_win_clear_f dthws_qw4_win_clear;

#ifndef DTDEPEND
#	include <windows/Qwindows.h>
#	ifdef DTCONFIG_QNX4_QWTUNNEL
#		include <windows/Qtunnel.h>
#	endif
#	include <sys/kernel.h>
#endif

typedef struct {
  int picture;
  char name[DT_WINDOWNAME_MAXLEN];
} dthws_qw4_win_priv_t;

typedef struct {
  char *spec;
  char *server;
  unsigned int maxbuf;
  int sid;
  int x0_default;
  int y0_default;
  int htpp;
  int vtpp;
  int protect_lut;						/* protect first lut entries 		*/
  int use_tunnel;
  void *qwt;							/* pointer from qwt open			*/
  unsigned long qwtmaxbuf;				/* max buffer size to give to qwt	*/
} dthws_qw4_priv_t;

#define MAP_MAX (100000L)

/*..........................................................................
 *..........................................................................*/

static
void
clip(
  dt_ctl_t *ctl,
  int xsize,
  int ysize,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye,
  void **cdata,
  int *cx0,
  int *cy0,
  int *cxe,
  int *cye)
{
  if (y0 < 0)							/* clip top of image?			    */
  {
    data = (char *)data - y0 * xe;
    ye += y0;
    y0 = 0;
  }

  if (y0+ye > ysize)					/* clip bottom of image?		    */
    ye = ysize - y0;

  if (x0 < 0)							/* clip left of image?			    */
  {
	data = (char *)data - x0;
	xe += x0;
	x0 = 0;
  }

  if (x0+xe > xsize)					/* clip right of image?			    */
    xe = xsize - x0;

  *cdata = data;
  *cx0 = x0;
  *cy0 = y0;
  *cxe = xe;
  *cye = ye;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_disconnect(
  dt_ctl_t *ctl,
  dthws_t *hws)
{
  DT_F("dthws_qw4_disconnect");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
	GraphicsClose(priv->sid);			/* close connection to server		*/
	if (priv->spec != NULL)
	  DT_I(dtos_free2,(ctl, priv->spec,
        F, "priv->spec"));
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }

  return rc;
}

/*..........................................................................
 * window structure has already been found or allocated and name assigned
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_win_create(					/* make/display a new window		*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)
{
  DT_F("dthws_qw4_win_create");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dthws_qw4_win_priv_t *win_priv;
  char *options;
  char *actions;

  DTCSTACK_HIGHWATER;

  if (x0 == -1 && y0 == -1)
  {
    x0 = priv->x0_default+= 20;;
    y0 = priv->y0_default+= 20;;
  }

  DT_Q(dtos_malloc2,(ctl, 
    (void **)&win_priv,
    sizeof(*win_priv),
    F, "priv")); 

  DT_MEMSET(win_priv, 0,					/* clear out the structure 			*/
    sizeof(*win_priv));

  hws_win->priv = win_priv;

  win_priv->picture = Picture(			/* make a picture					*/
    windowname, NULL);
  if (win_priv->picture == 0)
    return dt_err(ctl, F,
      "error in Picture(\"%s\", NULL)",
      windowname);

  dtstr_printf(ctl,						/* remember the window name 		*/
    win_priv->name,
    sizeof(win_priv->name),
    "%s", windowname);

  WindowAt( 							/* position window to come			*/
    y0*priv->vtpp,
    x0*priv->htpp,
    NULL, NULL);

/* -w takes triangular "close" button out of title bar
   -r means window cannot be resized
   -O removes outline from the frame
   -C means don't center the title
   -o means don't try to keep entire window on screen
*/

  hws_win->id = WindowOpen(
    windowname,
    ye * priv->vtpp,
    xe * priv->htpp,
    options = "D-w-C-o;auBSMYx",
    actions = "taXMRI",
    windowname, 0);

  if (hws_win->id == 0)
    return dt_err(ctl, F,
      "error in WindowOpen(\"%s\", %d, %d, %s, %s, %s, 0)",
      windowname,
      ye, xe,
      options, actions, windowname);

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "WindowOpen(\"%s\", %d, %d, %s, %s, %s, 0) = 0x%08lx",
    windowname,
    ye, xe,
    options, actions, windowname,
    hws_win->id);

  WindowBarCurrent('T', "r");			/* put buttons in title bar 		*/

#define W(X) ((X)*priv->htpp)
#define H(Y) ((Y)*priv->vtpp)
#define BOXW 13
#define BOXH 13

#define TAG_BUTTON_CLOSE "tag_button_close"

  DrawAt(H(7), W(xe - BOXW - 32));
  DrawGroup(H(BOXH+2), W(BOXW+2),
    0,									/* user-defined type 				*/
    "H",								/* format 							*/
    "N",								/* options 							*/
    TAG_BUTTON_CLOSE);					/* tag 								*/

  {
    SetColor("r", QW_TRANSPARENT);		/* no border around box 			*/
    SetColor("l", QW_BLACK);			/* lines of X are black				*/
    SetLineThickness("l", 1);
    SetFill("r",						/* box is solid gray 				*/
      QW_DARK_GRAY,
      QW_DOT_PAT+4);
    DrawAt(H(0), W(0));
    DrawRect(H(BOXH), W(BOXW), NULL, NULL);
    DrawAt(H(2), W(2));
    DrawLine(H(BOXH-4), W(BOXW-4),
      NULL, NULL);
    DrawAt(H(2), W(3));
    DrawLine(H(BOXH-4), W(BOXW-4),
      NULL, NULL);

    DrawAt(H(2), W(BOXW-2));
    DrawLine(H(BOXH-4), -W(BOXW-4),
      NULL, NULL);
    DrawAt(H(2), W(BOXW-3));
    DrawLine(H(BOXH-4), -W(BOXW-4),
      NULL, NULL);
	
    DrawEnd(TAG_BUTTON_CLOSE);
    Draw();
  }

#undef BOXH
#undef BOXW
#undef H
#undef W

  WindowBarCurrent(0, NULL);

  hws_win->x0 = x0;						/* remember the position		    */
  hws_win->y0 = y0;
  hws_win->xe = xe;						/* remember the size			    */
  hws_win->ye = ye;

  return DT_RC_GOOD;
}

/*..........................................................................
 * don't understand nature of qw4's exposure coordinates, so always
 *   just redraw entire window
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_redraw(						/* redraw exposed part of window	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_qw4_redraw");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dthws_qw4_win_priv_t *win_priv =
    (dthws_qw4_win_priv_t *)hws_win->priv;
  dtxy_t y;
  long offset = 0;

  DTCSTACK_HIGHWATER;

  WindowCurrent(hws_win->id);
  PictureCurrent(win_priv->picture);

  DrawAt(0, 0);
  DrawRect(								/* clear space under image to black	*/
    hws_win->ye * priv->vtpp,
    hws_win->xe * priv->htpp,
    "!", NULL);

  y = 0;
  while (y<hws_win->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DT_Q(dtmem_get,(ctl,				/* get the next block               */
      hws_win->memalloc, offset,
      &p, &datasize));
    nrows = datasize / hws_win->xe;     /* how many rows in block           */

    DT_Q(dthws_qw4_export_data,(ctl,	/* output the block					*/
      hws, hws_win, p,
      0, y, hws_win->xe, nrows));

    DT_Q(dtmem_unmap,(ctl,				/* unmap the block                  */
      hws_win->memalloc, offset));
    offset += datasize;

    y += nrows;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_expose(						/* handle exposure					*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  QW_EVENT_MSG *event_msg)
{
  DT_F("dthws_qw4_expose");
  dtlist_elem_t *elem;

  DTCSTACK_HIGHWATER;

  DT_Q(dthws_qw4_redraw,(ctl,			/* redraw the exposed window		*/
    hws, hws_win));

#ifdef NONO
  Draw();

  SetColor("r", global_color? QW_RED: QW_YELLOW);
  SetFill("r",  global_color? QW_RED: QW_YELLOW, 0);
  global_color ^= 1;
  DrawAt(
    event_msg->window.rect.row, 
    event_msg->window.rect.col);
  DrawRect(
    event_msg->window.rect.height, 
    event_msg->window.rect.width,
    "!", NULL);
  SetColor("r", global_background);
  SetFill("r", global_background, 0);
  Draw();
#endif

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_wait_event(					/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dthws_qw4_wait_event");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  int quit = 0;
  int x, y;
  int b1, b2, b3;
  int key;
  int i;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  DT_MEMSET(event, 0, sizeof(*event));		/* clear output report			    */

  do {
    QW_EVENT_MSG event_msg;
    pid_t pid;
    int action;
    int matches;
	
	event->type = DT_EVENT_NOTHING;		/* in case nothing happens 			*/

	if (flag == 0 && !EventWaiting())	/* just polling and nothing there? 	*/
      break;

    pid = GetEvent(0, (void *)&event_msg,
      sizeof(event_msg));
    if (pid == 0)
      continue;
    else
    if (pid == -1)
      return dt_err(ctl, F,
        "GetEvent failed");

    action = Event(&event_msg);

	if (action == 0)					/* non-qw event						*/
    {
      if (svc)							/* we have svc possibilities?		*/
      {
        int issvc;
        DT_Q(svc->issvc,(ctl, 			/* see if it's an svc event			*/
          svc, pid,
          (void *)&event_msg,
          &issvc));
        if (issvc)						/* it is an svc event?				*/
  	    {								/* the rest of the event structure	*/
										/* does not have to be valid when	*/
										/* it is an svc event			    */
  	      event->type = DT_EVENT_IPC;
  	      quit = 1;						/* quit right away				    */
		}
      }
	}
	else
	{
	  dthws_win_t *hws_win;
	  dtlist_elem_t *elem;
	  int user;
#ifdef NONO
      if (ctl->dbg_mask & 
          DT_DBG_MASK_ASYNC)
        EventPrint(F, &event_msg);		/* print info about the report	    */
#endif

      elem = DTLIST_ELEM_HEAD;
      while (elem)
      {
        DT_Q(dtlist_next,(ctl,			/* fetch next window in list	    */
          &hws->windowlist, elem,
          &elem));
        if (elem)						/* not the last in the list?	    */
		{
		  hws_win = (dthws_win_t *)elem->user;
		  if (hws_win->id == event_msg.hdr.window)
		    break;
		}
	  }

	  if (elem == NULL)					/* didn't find the window?			*/
	  {
		dt_dbg(ctl, F, 
          DT_DBG_MASK_ASYNC,
          "event type %c%c"
          " on unknown window 0x%08x",
          action>>8,
          action&0xff,
          event_msg.hdr.window);
		continue;						/* can't make us quit			    */
	  }

      event->id = event_msg.hdr.window;

      key = event_msg.hdr.code;

      if (action == QW_CLICK)
      {
		dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "click event_msg.hdr.key = \"%s\"",
          event_msg.hdr.key);
        event->mousex = event_msg.hdr.col / priv->htpp;
        event->mousey = event_msg.hdr.row / priv->vtpp;
        event->left = 0;
        event->middle = 0;
        event->right = 0;
		if (!strcmp(event_msg.hdr.key, TAG_BUTTON_CLOSE))
		{
		  event->nkeys = 1;
          event->keys[0] = 0x04;		/* ^D 								*/
          event->keys[1] = '\0';
		  event->type = DT_EVENT_KEYBOARD;
		}
		else							/* treat it like a mouse click 		*/
		{
		  event->left = event_msg.hdr.select;
		  event->middle = event_msg.hdr.adjust;
		  event->right = event_msg.hdr.menu;
          event->keys[0] = '\0';
          event->nkeys = 0;
          event->type = DT_EVENT_MOUSEPRESS;
		}
        user = 1;
      }
	  else
      if (action == QW_TYPED)
      {
        event->mousex = event_msg.hdr.col / priv->htpp;
        event->mousey = event_msg.hdr.row / priv->vtpp;
        event->left = 0;
        event->middle = 0;
        event->right = 0;
        switch(key & 0xffff)
        {
          case 0xffa6: event->keys[0] = DT_KEY_RIGHTARROW; break;
          case 0xffa4: event->keys[0] = DT_KEY_LEFTARROW; break;
          case 0xffa1: event->keys[0] = DT_KEY_UPARROW; break;
          case 0xffa9: event->keys[0] = DT_KEY_DOWNARROW; break;
          default: event->keys[0] = key;
        }
        event->nkeys = 1;
        user = 1;
        event->type = DT_EVENT_KEYBOARD;
      }
  	  else
      if ((action == QW_EXPOSED &&      /* exposure?                        */
           event_msg.hdr.code == 'U' && /* uncover?                         */
           event_msg.hdr.area == 0))	/* in pane 0?                       */
	  {
        DT_Q(dthws_qw4_expose,(ctl,		/* handle the exposure				*/
          hws, hws_win, &event_msg));
		dt_dbg(ctl, F, 
          DT_DBG_MASK_ASYNC,
          "exposed (%d, %d) size (%d, %d)",
          event_msg.window.rect.col / priv->htpp, 
          event_msg.window.rect.row / priv->vtpp,
          event_msg.window.rect.width / priv->htpp,
          event_msg.window.rect.height / priv->vtpp); 
	    user = 0;
	  }
  	  else
      if (action == QW_MOVED)			/* move?                        	*/
	  {
		hws_win->x0 =
		event->x0 = event_msg.window.col / priv->htpp;
		hws_win->y0 =
		event->y0 = event_msg.window.row / priv->vtpp;
		dt_dbg(ctl, F, 
          DT_DBG_MASK_ASYNC,
          "moved to (%d, %d) code %c",
          event->x0, event->y0, 
          event_msg.hdr.code != 0? 
            event_msg.hdr.code: '0'); 
		if (event_msg.hdr.code != 'I')	/* but not simply made inactive?    */
		{
          DT_Q(dthws_qw4_expose,(ctl,	/* handle the exposure				*/
            hws, hws_win, &event_msg));
		}
        event->type = DT_EVENT_MOVED;
	    user = 1;
	  }
  	  else
      if (action == QW_RESIZED)			/* resize?                          */
	  {
		hws_win->x0 =
		event->x0 = event_msg.window.rect.col / priv->htpp;
		hws_win->y0 =
		event->y0 = event_msg.window.rect.row / priv->vtpp;
		event->xe = event_msg.window.rect.width / priv->htpp;
		event->ye = event_msg.window.rect.height / priv->vtpp;
		dt_dbg(ctl, F, 
          DT_DBG_MASK_ASYNC,
          "resized (%d, %d) size (%d, %d)",
          event_msg.window.rect.col / priv->htpp, 
          event_msg.window.rect.row / priv->vtpp,
          event_msg.window.rect.width / priv->htpp,
          event_msg.window.rect.height / priv->vtpp); 
        event->type = DT_EVENT_RESIZED;
	    user = 1;
	  }
	  else
	    user = 0;

	  if (user)							/* user-generated event?		    */
	  {
		if (flag == 0)					/* just polling? 					*/
	      quit = 1;
		else
        if (flag == 1)					/* supposed to quit after each one?	*/
          quit = 1;
        else
        if (flag == 3)					/* quit after user events			*/
          quit = 1;
	  }
  	  else								/* non-user event?				    */
	  {
	    if (flag == 1 &&			  	/* supposed to quit immediately?    */
            !EventWaiting())			/* event queue has nothing else?	*/
          quit= 1;
  	  }
	  if (flag == 0 &&					/* just polling? 					*/
          !EventWaiting())				/* event queue has nothing else?	*/
        quit= 1;
    }
  } while(!quit);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_win_destroy(					/* get rid of window			    */
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_qw4_win_destroy");

  DTCSTACK_HIGHWATER;

  WindowCurrent(hws_win->id);
  WindowClose();

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "WindowClose() on 0x%08lx",
    hws_win->id);

  DT_Q(dtos_free2,(ctl,					/* free priv structure			*/
    hws_win->priv,
    F, "hws_win->priv"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * only do tunnels if entire row of data is used
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_export_tunnel(				/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
# ifdef DTCONFIG_QNX4_QWTUNNEL
  DT_F("dthws_qw4_export_tunnel");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  int w = xe;
  QW_RECT_AREA pane_region;
  int ndraws = 0;
  unsigned long maxr;
  unsigned int left;
  QWT_POINT pos, area;

  DTCSTACK_HIGHWATER;

  PaneInfo(&pane_region, 				/* where window is on whole screen	*/
    NULL, NULL);
  pos.x = pane_region.col /				/* where we want image to go		*/
    priv->htpp + x0;	
  pos.y = pane_region.row /
    priv->vtpp + y0;

  left = ye;
  maxr = priv->qwtmaxbuf / xe;			/* max number of rows at one time   */

  area.x = xe;
  while (left)
  {
	area.y = DT_MIN(left, maxr);
	qwt_DrawBitmap(data, area.y * xe,
      QWT_BYTE_IMAGE,
      &pos, &area, xe);
	qwt_Flush(QWT_SYNCH);
	ndraws++;
	pos.y += area.y;
	left -= area.y;
	data = (char *)data + area.y * xe;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "tunnel drawn in %d draws", ndraws);
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_export_normal(				/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  void *data,
  int x0,
  int y0,
  int xe,
  int w,
  int ye)
{
  DT_F("dthws_qw4_export_normal");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  int nrows;
  int y;
  QW_IMAGE_DEF image = {0};

  DTCSTACK_HIGHWATER;
  
  if (xe == w)							/* can output big blocks?			*/
    nrows = (int)(priv->maxbuf/w);		/* rows that fit in one draw		*/
  else									/* data not contiguous?				*/
    nrows = 1;							/* output row at a time				*/

  image.size = sizeof(image);
  image.width = w;
  image.format = 'M';
  image.bits_per_color = 8;

  for (y=0; y<ye; y+=nrows)				/* for each buf full				*/
  {
    if (y+nrows > ye)
      nrows = ye - y;
    image.length = w * nrows;
    image.height = nrows;
    DrawAt((y+y0) * priv->vtpp, 
      x0 * priv->htpp);
    DrawImage(&image, data, 
      NULL, "!", NULL);
    data = (char *)data + xe * nrows;
  }
  Draw();

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_export_data(					/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_qw4_export_data");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dthws_qw4_win_priv_t *win_priv =
    (dthws_qw4_win_priv_t *)hws_win->priv;
  char winstate[21];
  int w = xe;
  int reason = 0;

  WindowCurrent(hws_win->id);
  PictureCurrent(win_priv->picture);

  WindowInfo(NULL, 						/* current window state				*/
    NULL, NULL,
    NULL, NULL, winstate);

  clip(ctl,								/* do clipping					    */
    hws_win->xe, hws_win->ye,
    data, x0, y0, xe, ye,
    &data, &x0, &y0, &w, &ye);

  if (xe != w)							/* rows are clipped?				*/
    reason = 2;
  else
  if (!priv->use_tunnel)				/* told not to use tunnels? 		*/
    reason = 3;
  else
  if (strchr(winstate, 'C'))			/* not completely uncovered?		*/
    reason = 4;
# ifndef DTCONFIG_QNX4_QWTUNNEL
    reason = 1;
# endif

  if (!reason)							/* supposed to use tunnels? 		*/
    DT_Q(dthws_qw4_export_tunnel,(
      ctl, hws, data,
      x0, y0, xe, ye))
  else
  {
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "not doing tunnels, reason=%d",
      reason);
    DT_Q(dthws_qw4_export_normal,(
      ctl, hws, data,
      x0, y0, xe, w, ye))
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_import_data(					/* get window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_export_color(					/* write window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dthws_qw4_export_color");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dthws_qw4_win_priv_t *win_priv =
    (dthws_qw4_win_priv_t *)
      (hws_win? hws_win->priv: NULL);
  int i;

  DTCSTACK_HIGHWATER;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\%s\" start=%d, count=%d",
    win_priv? win_priv->name: "NULL",
    start, count);

  for (i=0; i<count; i++)
  {
	if (i+start >= priv->protect_lut)	/* protected lut entries?			*/
  	  ScreenColor(i+start, 
        (lut[i].r*10000)/256, 
        (lut[i].g*10000)/256, 
        (lut[i].b*10000)/256, 
        NULL, NULL);
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "done setting colors");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_import_color(					/* get window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dthws_qw4_import_color");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dthws_qw4_win_priv_t *win_priv =
    (dthws_qw4_win_priv_t *)
      (hws_win? hws_win->priv: NULL);

  DTCSTACK_HIGHWATER;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\%s\" start=%d, count=%d",
    win_priv? win_priv->name: "NULL",
    start, count);

  if (hws_win == NULL)
  {
    int i;
    for (i=0; i<count; i++)
    {
      unsigned r, g, b;
      if (i == 0)
      {
        lut[i].r = 0;
        lut[i].g = 0;
        lut[i].b = 0;
      }
      else
      {
        int ret;
        ret = ScreenColorInfo(start+i, NULL,
          &r, &g, &b, NULL);
        if (ret != start+i)
          return dt_err(ctl, F,
            "ScreenColorInfo(%d...) returned %d",
            start+i, ret);
        lut[i].r = ((long)(r-1) << 8) / 10000L;
        lut[i].g = ((long)(g-1) << 8) / 10000L;
        lut[i].b = ((long)(b-1) << 8) / 10000L;
      }
    }
  }
  else
    return dt_err(ctl, F,
      "cannot get color table for individual window");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_qw4_win_clear(					/* clear window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int background)
{
  DT_F("dthws_qw4_win_clear");
  dthws_qw4_priv_t *priv =
    (dthws_qw4_priv_t *)hws->priv;
  dthws_qw4_win_priv_t *win_priv =
    (dthws_qw4_win_priv_t *)hws_win->priv;

  DTCSTACK_HIGHWATER;

  WindowCurrent(hws_win->id); 
  PictureCurrent(win_priv->picture);

  global_background = background;
  SetColor("r", background);
  SetFill("r", background, 0);
  DrawAt(0, 0);
  DrawRect(
    hws_win->ye * priv->vtpp,
    hws_win->xe * priv->htpp,
    "!", NULL);

  return DT_RC_GOOD;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_qw4_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *spec)
{
  DT_F("dthws_qw4_connect");
#if DTPRJ_QW4 == DT_1
  dthws_qw4_priv_t *priv;
  int ntokens;
  char *p;
  int sid;
  unsigned int maxbuf;
  QW_BOX size;
  QW_BOX tpp;
  unsigned n_colors;
  int xe, ye;
  dt_rc_e rc;

  DTCSTACK_HIGHWATER;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL window server spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(dthws_qw4_priv_t),
    F, "priv"));
  DT_MEMSET(priv, 0,						/* clear private structure 			*/
    sizeof(dthws_qw4_priv_t));

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));

  priv->server = "";					/* default server on own node 		*/
  priv->protect_lut = 20;				/* default protect first 20 entries */
  priv->use_tunnel = 0;					/* default don't use tunnel 		*/

  p = priv->spec;

  if (ntokens >= 1)
  {
	priv->server = p;
	p += strlen(p) + 1;
  }
  if (ntokens >= 2)
  {
	if (strstr(p, "tunnel"))			/* spec says use tunnel? 			*/
	  priv->use_tunnel = 1;
	if (strstr(p, "noprotect"))			/* spec says use all lut entries? 	*/
	  priv->protect_lut = 0;
	p += strlen(p) + 1;
  }
  
  sid = GraphicsOpen(priv->server);		/* open connection to server        */
  if (sid == 0)
  {
    rc = dt_err(ctl, F,
      "GraphicsOpen(\"%s\") failed",
      priv->server);
	goto cleanup;
  }

  {
    unsigned version;
    char name[QW_MAX_NAME_LEN];
    int n_pat;
    QW_BOX ppi;
    int owner;
    char model[QW_MAX_NAME_LEN];
    version = ScreenInfo(name,
      &n_colors, &n_pat, model,
      &size, &ppi, &tpp, &owner);
    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "QNX4 Windows version %-u.%-u.%-u",
      (version / 1000),
      (version - version / 1000 * 1000)/100,
      (version %100));
    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "screen server name \"%s\", n_colors %u",
      name, n_colors);
    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "screen size %dx%d",
      size.width/tpp.width,
      size.height/tpp.height);  
  }

  if (n_colors < 256)
  {
    rc = dt_err(ctl, F,
      "screen only supports %u colors, need 256",
      n_colors);
	goto cleanup;
  }

  maxbuf = DrawStart(NULL,
    MAP_MAX);
  if (maxbuf == 0)
  {
    rc = dt_err(ctl, F,
      "DrawStart(NULL, %ld) failed",
      MAP_MAX);
	goto cleanup;
  }
  maxbuf =                        		/* don't be greedy and use it all   */
    (int)(95L*maxbuf) / 100L;

#ifdef DTCONFIG_QNX4_QWTUNNEL
  if (priv->use_tunnel)
  {
	priv->qwtmaxbuf = 32767;			/* tunnel size seems fixed          */
	priv->qwt = qwt_TunnelOpen(
      priv->qwtmaxbuf, 1024L);
	if (priv->qwt == NULL)
	{
      rc = dt_err(ctl, F,
        "qwt_TunnelOpen failed");
	  goto cleanup;
	}
	qwt_Flush(QWT_SYNCH);
  }
#endif

  priv->sid = sid;
  priv->maxbuf = maxbuf;
  priv->htpp = tpp.width;
  priv->vtpp = tpp.height;

  DT_MEMSET(hws, 0, sizeof(*hws));			/* clear out structure				*/

  hws->xe = size.width;
  hws->ye = size.height;
  hws->priv = priv;
  hws->disconnect = dthws_qw4_disconnect;
  hws->win_create = dthws_qw4_win_create;
  hws->wait_event = dthws_qw4_wait_event;
  hws->win_destroy = dthws_qw4_win_destroy;
  hws->export_data = dthws_qw4_export_data;
  hws->import_data = dthws_qw4_import_data;
  hws->export_color = dthws_qw4_export_color;
  hws->import_color = dthws_qw4_import_color;
  hws->win_clear = dthws_qw4_win_clear;

cleanup:
  if (rc != DT_RC_GOOD &&
	  priv != NULL)
  {
	if (priv->spec != NULL)
	  DT_I(dtos_free2,(ctl, priv->spec,
        F, "priv->spec"));
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }

  return rc;
#else
  return dt_err_notlinked(ctl, F, "QNX4 Windows");
#endif
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
