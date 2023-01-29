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
  19981014 DE remove sunpro1 compiler warnings
  19980716 DE check for negative time in double-click
 *..........................................................................*/


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
DT_RCSID("source/screen $RCSfile: hwsx.c,v $ $Revision: 1.1 $");

#if DTPRJ_X == DT_1

#  ifndef DTDEPEND

#    include <X11/Xlib.h>
#    include <X11/Xutil.h>
#    include <X11/Xos.h>
#    include <X11/Xatom.h>
#    include <X11/keysym.h>
#    include <X11/cursorfont.h>
#ifndef bzero
#    define bzero(s, n) DT_MEMSET ((s), 0, (n)) /* called by FD_ZERO macro	  */
#endif

#    include <sys/ipc.h>
#    include <sys/shm.h>
#    include <X11/extensions/XShm.h>
#    include <X11/extensions/multibuf.h>

#  endif

#if defined(DTCONFIG_IMPLIED_SUNOS) &&	/* not in this guy's header files!	*/\
    defined(DTCONFIG_IMPLIED_GNU)
void *shmat (int, void *, int);
int shmget  (key_t, int, int);
int shmctl  (int, int, struct shmid_ds *);
int shmdt   (void *);
#endif

#endif

#if !defined(_XFUNCPROTO_H_)			/* no new-style (R6) prototyping?	*/
#define NOSHM							/* can't do this then 				*/
#define NOMBUF							/* can't do this either				*/
#define FREE_CAST char *
#else
#define FREE_CAST void *
#endif

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/hws.h>

#if DTPRJ_X == DT_1

extern dthws_connect_f dthws_connect;
static dthws_disconnect_f dthws_x_disconnect;
static dthws_win_create_f dthws_x_win_create;
static dthws_wait_event_f dthws_x_wait_event;
static dthws_win_destroy_f dthws_x_win_destroy;
static dthws_export_data_f dthws_x_export_data;
static dthws_import_data_f dthws_x_import_data;
static dthws_export_color_f dthws_x_export_color;
static dthws_import_color_f dthws_x_import_color;
static dthws_win_clear_f dthws_x_win_clear;
static dthws_win_raise_f dthws_x_win_raise;
static dthws_win_lower_f dthws_x_win_lower;
static dthws_win_cursor_f dthws_x_win_cursor;

#include <dtack/mem.h>

typedef struct {
  void *mem;
  XImage *image;
  XShmSegmentInfo shminfo;
  Colormap colormap;
  Window window;
  Multibuffer multibuf[2];
  int multibufi;						/* multi buffer we are drawing in 	*/
  Window parent;
  GC gc;
  int depth;							/* depth of the drawable itself	    */
  unsigned int foreground;
  unsigned int background;
  char name[DT_WINDOWNAME_MAXLEN+1];
  int xborder, yborder;
  int border_width;
  Cursor cursor;
  int cursorx;
  int cursory;
  Time click_time;
  Time unclick_time;
  unsigned int click_state;
  int isshared;							/* we are using XShm interface 		*/
} dthws_x_priv_win_t;

typedef struct {
  Display *display;
  Screen *screen;						/* screen we will be drawing on		*/
  Colormap default_colormap;
  Colormap colormap;					/* common colormap, if any 			*/
  Window rootwin;
  Visual *visual;
  XVisualInfo *visual_info;
  int screen_num;	
  int selectfd;
  int use_shared;						/* OK to use shared images 			*/
  int use_multibuf;						/* OK to use multi buffering		*/
  int option_backing_store;
  int option_default_colormap;
  int option_shared_colormap;
  int option_multibuffer;
  int option_remap_colors;
  int option_shared_memory;
} dthws_x_priv_t;


/*..........................................................................
 * X non-fatal error handler
 *..........................................................................*/

static unsigned xerror;					/* don't really like this			*/

#define XERROR_CHECK(FUNC)				/* macro to check the rc of X func	*/\
{                                                                             \
  if (xerror == 0xdeafbabe)                                                   \
  {                                                                           \
    return dt_err(ctl, F,                                                     \
      "in %s: %s", #FUNC, "XIO error");                                       \
  }                                                                           \
  else                                                                        \
  if (xerror != 0)                                                            \
  {                                                                           \
    char text[128];                                                           \
    if (priv != NULL)														  \
      XGetErrorText(priv->display,                                            \
        xerror, text, sizeof(text)-1);                                        \
    else																	  \
      strcpy(text, "unknown error");										  \
    return dt_err(ctl, F,                                                     \
      "in %s: %s", #FUNC, text);                                              \
  }                                                                           \
}

#define X(FUNC, ARGS)					/* macro to check the rc of X func	*/\
{                                                                             \
  XERROR_CHECK(FUNC);														  \
  xerror = 0;                                                                 \
  FUNC ARGS;                                                                  \
  XERROR_CHECK(FUNC);														  \
}

static int xerror_handler(
  Display *display,
  XErrorEvent *error)
{
  xerror = error->error_code;			/* just save error in global	    */
  return 0;								/* don't really know what to give   */
}

static int xioerror_handler(
  Display *display)
{
  xerror = 0xdeafbabe;					/* just save error in global	    */
  return 0;								/* spec says not to return			*/
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_scr_from_name(					/* choose screen based on name		*/
  dt_ctl_t *ctl,
  dthws_x_priv_t *priv,
  const char *name,
  int *screen)
{
  *screen = DefaultScreen(				/* use default screen			    */
    priv->display);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_disconnect(
  dt_ctl_t *ctl,
  dthws_t *hws)
{
  DT_F("dthws_x_disconnect");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;

  if (priv->colormap != 0 &&
	  priv->colormap !=
	  priv->default_colormap)
    XFreeColormap(priv->display,
      priv->colormap);

  if (priv->visual_info != NULL)
    XFree((FREE_CAST)priv->visual_info);

  XCloseDisplay(priv->display);		/* close server connection			*/

  DT_Q(dtos_free2,(ctl, priv,
    F, "priv"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * window structure has already been found or allocated and name assigned
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_create_nonshared(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_x_win_create_image_nonshared");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;
  dtxy_t xe = hws_win->xe;
  dtxy_t ye = hws_win->ye;
  void *data;
  unsigned int size;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "using non-shared XImage interface");

  DT_Q(dtmem_map,(ctl,					/* map all data					    */
    hws_win->memalloc, 0,
    &data, &size));

  if ((long)size < (long)xe*(long)ye)	/* data not all mapped?			    */
  {
	DT_Q(dthws_x_win_destroy,(ctl, 
      hws, hws_win));
    return dt_err(ctl, F,
      "%s backing bitmap not allocated"
      " in contiguous memory",
      priv_win->name);
  }

  priv_win->image = XCreateImage(		/* make X image from our data	  	*/
    priv->display,
    priv->visual,
    8, ZPixmap, 0,						/* depth, format, and offset		*/
    (char *)data, xe, ye,
    8, 0);								/* pad and bytes-per-line			*/
  XERROR_CHECK(XCreateImage);
  if (priv_win->image == NULL)
  {
    dt_rc_e rc;
    rc = dt_err(ctl, F,
      "at XCreateImage");  
    DT_Q(dthws_x_win_destroy,(ctl, 
      hws, hws_win));
    return rc;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_create_shared(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_x_win_create_image_shared");
#ifdef NOSHM
  return dt_err_notlinked(ctl, F, "X Shm extension");
#else
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;
  dtxy_t xe = hws_win->xe;
  dtxy_t ye = hws_win->ye;
  void *newmem;
  void *data;
  unsigned int size;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "using shared XImage interface");

  DT_MEMSET(
    &priv_win->shminfo, 0,
    sizeof(priv_win->shminfo));

  priv_win->image = XShmCreateImage(	/* make X image from our data	  	*/
    priv->display,
    priv->visual,
    8,									/* depth 							*/
    ZPixmap,							/* format 							*/
    NULL,								/* data 							*/
    &priv_win->shminfo,
    xe, ye);							/* width and height 				*/
  XERROR_CHECK(XShmCreateImage);

  if (priv_win->image == NULL)
  {
    rc = dt_err(ctl, F,
      "error in XShmCreateImage");  
    goto cleanup;
  }

  priv_win->shminfo.shmid =
    shmget(IPC_PRIVATE,
      priv_win->image->bytes_per_line*
      priv_win->image->height,
      IPC_CREAT | 0777);

  if (priv_win->shminfo.shmid == -1)
  {
    rc = dt_err(ctl, F,
      "shmget(IPC_PRIVATE, %d, IPC_CREAT|0777)"
      " errno %d",
      priv_win->image->bytes_per_line*
      priv_win->image->height,
      errno);
    goto cleanup;
  }

  priv_win->image->data = (char *)
    shmat(priv_win->shminfo.shmid,
      0, 0);
  priv_win->shminfo.shmaddr = 
    priv_win->image->data;

  if (priv_win->shminfo.shmaddr == (void *)(-1))
  {
    rc = dt_err(ctl, F,
      "shmat(%d, 0, 0)"
      " errno %d",
      priv_win->shminfo.shmid,
      errno);
    priv_win->shminfo.shmaddr = NULL;
    goto cleanup;
  }

  priv_win->shminfo.readOnly = False;

  priv_win->isshared = 
    XShmAttach(priv->display,
      &priv_win->shminfo);
  XERROR_CHECK(XShmAttach);
  if (priv_win->isshared == 0)
  {
    rc = dt_err(ctl, F,
      "error in XShmAttach");  
    goto cleanup;
  }

  DT_C(dtmem_prealloc,(ctl,				/* wrap memalloc around shared area */
    &newmem, 
    xe, ye,  
    (void **)&priv_win->image->data,
    xe * ye, 1,
    F, "shared image"));

  DT_C(dtmem_free,(ctl,					/* free caller's backing bitmap		*/
    hws_win->memalloc,
    F, "old backing store"));

  hws_win->memalloc = newmem;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "using shared XImage interface"
    " shmid %d, shmaddr 0x%08lx",
    priv_win->shminfo.shmid,
    priv_win->shminfo.shmaddr);

cleanup:

  if (rc != DT_RC_GOOD)
    DT_I(dthws_x_win_destroy,(ctl, 
      hws, hws_win));

  return rc;
#endif									/* not NOSHM						*/
}

/*..........................................................................
 * window structure has already been found or allocated and name assigned
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_create_image(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_x_win_create_image");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (priv->use_shared)
    DT_Q(dthws_x_win_create_shared,(ctl, hws, hws_win))
  else
    DT_Q(dthws_x_win_create_nonshared,(ctl, hws, hws_win));
  
  return rc;
}

/*..........................................................................
 * window structure has already been found or allocated and name assigned
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_create(						/* make/display a new window		*/
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
  DT_F("dthws_x_win_create");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win;
  XSetWindowAttributes attrib;
  unsigned long attrib_mask = 0;		/* stuff for window	attributes		*/
  XGCValues gcvalues;
  unsigned long gc_mask = 0;			/* stuff for gc						*/
  int does;
  XSizeHints *normal_hints;				/* stuff for window manager size	*/
  XTextProperty text;					/* stuff for window manager name	*/
  XClassHint class_hints;
  char *tp, ts[256];
  XEvent event;							/* to wait for exposure event	    */

  if (strlen(windowname) >
      sizeof(priv_win->name)-1)
    return dt_err(ctl, F,
      "\"%s\" too long", windowname);

  DT_Q(dtos_malloc2,(ctl,
    &hws_win->priv,
    sizeof(*priv_win),
    F, "priv win"));
  priv_win = (dthws_x_priv_win_t *)hws_win->priv;

  DT_MEMSET(priv_win, 0, 
    sizeof(*priv_win));

  strcpy(priv_win->name,
    windowname);
										/* ................................ */
  priv_win->depth =						/* depth to create the window	    */
    priv->visual_info[0].depth;

  if (bits > priv_win->depth)			/* too many bits for window?		*/
    return dt_err(ctl, F,
      "cannot create %d-bit image"
      " on %d-bit screen",
      bits, priv_win->depth);

										/* ................................ */
  priv_win->foreground =				/* remember foreground in window    */
    WhitePixel(priv->display, 
      priv->screen_num);
  priv_win->background =				/* remember background in window    */
    BlackPixel(priv->display, 
     priv->screen_num);

										/* ................................ */
  if (bits == 1)						/* no need for colormap?		    */
  {
    priv_win->colormap = 0;
    attrib.background_pixel =
      priv_win->background;
  }
  else
  if (priv->colormap != 0)				/* all windows are sharing colormap */
  {
	priv_win->colormap = priv->colormap;
    attrib.background_pixel = 0;

    attrib_mask |= CWColormap;			/* use non-default colormap			*/
    attrib.colormap = 
      priv_win->colormap;
  }
  else
  {
    int i;
    XColor xcolor[256];

    priv_win->colormap =				/* create a colormap				*/
      XCreateColormap(
        priv->display,
        priv->rootwin,
        priv->visual, AllocAll);
    XERROR_CHECK(XCreateColormap);
    if (priv_win->colormap == 0)
      return dt_err(ctl, F,
        "at XCreateColormap");

    for (i=0; i<256; i++)				/* want indices of all colors	    */
	{
      xcolor[i].pixel = i;
      xcolor[i].flags =
        DoRed |
        DoGreen |
        DoBlue;
	}

    X(XQueryColors,(priv->display,		/* get root's colors			    */
      DefaultColormap(priv->display,
        priv->screen_num),
      xcolor, 256));

    X(XStoreColors,(priv->display,		/* set this window's colors		    */
      priv_win->colormap,
      xcolor, 256));

	dt_dbg(ctl, F,
      DT_DBG_MASK_WIN | 
      DT_DBG_MASK_LUT,
	  "window \"%s\" has private"
      " colormap 0x%08lx",
      windowname, priv->colormap);

    attrib_mask |= CWColormap;			/* use non-default colormap			*/
    attrib.colormap = 
      priv_win->colormap;

    attrib.background_pixel = 0;
  }
  attrib_mask |= CWBackPixel;			/* we are setting background		*/
										/* ................................ */

  if (priv->option_backing_store)
  {
	attrib_mask |= CWBackingStore;		/* set backing store attribute 		*/
	attrib.backing_store = WhenMapped;
  }
										/* ................................ */

  attrib_mask |= CWBitGravity;			/* we are setting bit gravity		*/
  attrib.bit_gravity =
    NorthWestGravity;

  priv_win->window = XCreateWindow(		/* make the window					*/
    priv->display,
    priv->rootwin,
	x0, y0, xe, ye,
    4,									/* border width						*/
    priv_win->depth,					/* window depth					    */
    InputOutput,
    priv->visual,						/* visual						    */
    attrib_mask, &attrib);
  XERROR_CHECK(XCreateWindow);

  if (priv_win->window == 0)
    return dt_err(ctl, F,
      "at XCreateWindow");

  if (priv_win->colormap)
  {
    X(XSetWindowColormap,(				/* why do this after CreateWindow?	*/
      priv->display,
      priv_win->window,
      priv_win->colormap));
  }

  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
    "window \"%s\" colormap"
    " initialized to 0x%08x", 
    windowname, priv_win->colormap);
										/* ................................ */
										/* notify window manager of size	*/
  normal_hints = XAllocSizeHints();

  if (x0 >= 0 || y0 >= 0)				/* user is specifying start?		*/
  {
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	normal_hints->flags |= USPosition;	/* flag that this is user's choice	*/
	normal_hints->x = x0;
	normal_hints->y = y0;
  }
  else
	normal_hints->flags |= PPosition;	/* this is program's choice			*/

  normal_hints->flags |= PSize;			/* size always is program's choice	*/
  normal_hints->width = xe;
  normal_hints->height = ye;

  X(XSetWMNormalHints,(priv->display,
    priv_win->window,
    normal_hints));

  X(XFree,((FREE_CAST)normal_hints));
										/* ................................ */
										/* notify window manager of name	*/

  sprintf(tp=ts, "dt %s",				/* make a uniquer name				*/
    priv_win->name);

  X(XStringListToTextProperty,(			/* create a text property			*/
    &tp, 1, &text));
  
  X(XSetWMName,(priv->display,			/* name goes in title bar			*/
    priv_win->window,
    &text));
  X(XSetWMIconName,(priv->display,		/* name goes in icon				*/
    priv_win->window, &text));

  X(XFree,((FREE_CAST)text.value));

  class_hints.res_name =				/* window instance name				*/
    priv_win->name;
  class_hints.res_class =				/* window class name				*/
    "dt";
  X(XSetClassHint,(priv->display,		/* set window name information		*/
    priv_win->window, &class_hints));

										/* ................................ */
  gcvalues.foreground =
    priv_win->foreground;
  gcvalues.background =
    priv_win->background;

  gc_mask = GCForeground|GCBackground;
  priv_win->gc = XCreateGC(				/* create default context			*/
    priv->display, 
    priv_win->window,
    gc_mask, &gcvalues);
  XERROR_CHECK(XCreateGC);
  if (priv_win->gc == 0)
    return dt_err(ctl, F,
      "at XCreateGC");
  X(XSetFillStyle,(priv->display,		/* only fill action is to clear	    */
    priv_win->gc,
    FillSolid));

										/* ................................ */
  X(XSelectInput,(						/* select events we want to see		*/
    priv->display,
    priv_win->window,
    ExposureMask |
    KeyPressMask |
    KeyReleaseMask |
	ButtonPressMask |
	ButtonReleaseMask |
	PointerMotionMask |
    StructureNotifyMask));				/* window moved or resized		    */

										/* ................................ */
  X(XMapWindow,(priv->display,			/* pop up the window				*/
    priv_win->window));

										/* ................................ */
  X(XFlush,(priv->display));			/* clear output buffer to server	*/

  dt_dbg(ctl, F,
    DT_DBG_MASK_ASYNC,        
    "waiting for exposure on window 0x%08x",
    priv_win->window);

  do {
    XMaskEvent(priv->display,			/* wait for any window's exposure	*/
      ExposureMask|StructureNotifyMask,
      &event);
    if (event.type == Expose)
      dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC,        
        "got exposure on window 0x%08x: count %d",
        event.xexpose.window,
        event.xexpose.count);
    else
    if (event.type == ConfigureNotify)
	{
      dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC,        
        "got configure on window 0x%08x: at (%d, %d) size (%d, %d) border %d",
        event.xconfigure.window,
        event.xconfigure.x,
        event.xconfigure.y,
        event.xconfigure.width,
        event.xconfigure.height,
        event.xconfigure.border_width);
      hws_win->x0 = event.xconfigure.x + /* correct for window manager	    */
        event.xconfigure.border_width/2;
      hws_win->y0 = event.xconfigure.y +
        event.xconfigure.border_width/2;
      priv_win->border_width = 
        event.xconfigure.border_width;
	}
    else
    if (event.type == ReparentNotify)
	{
      dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC,        
        "got reparent on window 0x%08x"
        " parent 0x%08lx: at (%d, %d)",
        event.xreparent.window,
        event.xreparent.parent,
        event.xreparent.x,
        event.xreparent.y);
	  if (event.xreparent.parent ==		/* reparenting to root? 			*/
		  priv->rootwin)
  	    priv_win->parent = 0;
	  else								/* reparenting to new window?		*/
  	    priv_win->parent =
	      event.xreparent.parent;
	}
    else
      dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC,        
        "got event type %d: ignored",
        event.type);
    XERROR_CHECK("event loop bottom");
  } while(event.type != Expose ||		/* we get configure before expose	*/
          event.xexpose.window !=		/* until we get one for this window	*/
          priv_win->window ||
          event.xexpose.count != 0);

  XFlush(priv->display);				/* clear output buffer to server	*/

  hws_win->xe = xe;						/* remember the size			    */
  hws_win->ye = ye;
  hws_win->bits = bits;

  DT_Q(dthws_x_win_create_image,(ctl,
    hws, hws_win));

  hws_win->id = priv_win->window;		/* unique window identifier			*/
										/* ................................ */
# ifndef NOMBUF
  if (priv->use_multibuf)
  {
	int n;
    n = XmbufCreateBuffers(
      priv->display, 
      priv_win->window, 
      2,
      MultibufferUpdateActionBackground, 
      MultibufferUpdateHintFrequent,
      priv_win->multibuf);
	if (n < 2)
	  return dt_err(ctl, F,
        "XmbufCreateBuffers only"
        " returned %d, expected 2",
        n);
  }
# endif
										/* ................................ */
  {
    XWindowAttributes attributes;
    X(XGetWindowAttributes,(
      priv->display,
      priv_win->window,
      &attributes));

    dt_dbg(ctl, F, 
      DT_DBG_MASK_ASYNC |
      DT_DBG_MASK_ALIMITS,
      "window 0x%08x attributes:"
      " at (%d, %d) size (%d, %d)"
      " border %d",
      priv_win->window,
      attributes.x,
      attributes.y,
      attributes.width,
      attributes.height,
      attributes.border_width);

    dt_dbg(ctl, F, 
      DT_DBG_MASK_ASYNC |
      DT_DBG_MASK_ALIMITS,
      "   backing_store=%d", 
      " backing_planes=%lu",
      attributes.backing_store,
      attributes.backing_planes);

    priv_win->xborder =					/* correct to top level edge	  	*/
      attributes.x;
    priv_win->yborder =
      attributes.y;
  }

  hws_win->x0 -= priv_win->xborder;		/* correct for outer frame			*/
  hws_win->y0 -= priv_win->yborder;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
Bool predicate(
  Display *display, 
  XEvent *event,
  char *arg) 
{ return True; }


static
dt_rc_e
dthws_x_wait_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dthws_x_wait_event");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_win_t *hws_win;
  int xpending;
  int svcpending;
  int svc_selectfd;
  dtlist_elem_t *elem;
  dthws_x_priv_win_t *priv_win;
  XEvent report;
  Window window;
  int x, y;
  int i;
  int b1, b2, b3;
  int quit = 0;
  KeySym keysym;
  char s[32];
  fd_set fds;
  dt_rc_e rc;

  if (svc && 
	  svc->type != DTSVC_TYPE_NONE)
    svc_selectfd = svc->selectfd;
  else
  {
	svc = NULL;
    svc_selectfd = -1;
  }

  if (event != NULL)
  do {
	DT_MEMSET(event, 0, sizeof(*event));	/* clear output event structure	    */
  
	event->type = DT_EVENT_NOTHING;		/* in case nothing happens 			*/
	
    DT_MEMSET(&report, 0, sizeof(report));	/* start with clean report		    */

	X(XSync,(priv->display, False));
    xpending = XEventsQueued(			/* check if events are queued 		*/
      priv->display, 
      QueuedAfterReading);				/* actually read socket 			*/
    XERROR_CHECK(XEventsQueued);

										/* ................................ */
    if (xpending)						/* display events are pending? 		*/
	{
      X(XNextEvent,(priv->display,		/* grab the next pending event 		*/
        &report));
	}
										/* ................................ */
	else
	if (timeout == -1)					/* this is a poll?			    	*/
	{
      if (svc != NULL)					/* we are using dt svc?		    	*/
	  {
        DT_Q(svc->issvc,(ctl,			/* see if we have any svc input	    */
          svc, 0, NULL, &svcpending));
        if (svcpending)	
	    {			
  	      event->type = DT_EVENT_IPC;
          dt_dbg(ctl, F, DT_DBG_MASK_IPC,
            "svc input found at poll");
	    }
	  }
      break;							/* out of while(!quit) loop			*/
	}
										/* ................................ */
	else								/* not a poll and no events? 		*/
	{									/* this means we really wait 		*/
      int sysrc;
      FD_ZERO(&fds);
      FD_SET(priv->selectfd, &fds);		/* wait on the scr fd				*/
  	  if (svc_selectfd != -1)
        FD_SET(svc_selectfd, &fds);		/* wait on the svc fd too		    */

      sysrc = select(					/* wait here 						*/
        DT_MAX(priv->selectfd,
               svc_selectfd)+1,
        &fds, NULL, NULL, NULL);
      if (sysrc == -1)
        return dt_err(ctl, F,
          "errno %d in select()", 
          errno);

      if (svc != NULL)					/* and we are using dt svc?		    */
	  {
        DT_Q(svc->issvc,(ctl,			/* see if this was an svc input	    */
          svc, 0, NULL, &svcpending));

        if (svcpending)					/* return to caller on svc input    */
	    {								/* the rest of the event structure	*/
										/* does not have to be valid when	*/
										/* it is an svc event			    */
  	      event->type = DT_EVENT_IPC;
          dt_dbg(ctl, F, DT_DBG_MASK_IPC,
            "svc input causes return");
          break;						/* out of while(!quit) loop			*/
	    }
	  }

      XERROR_CHECK(XCheckIfEvent);
      if (!XCheckIfEvent(				/* event queue has nothing either?	*/
             priv->display,
             &report, predicate, NULL))
  	  {
        XERROR_CHECK(XCheckIfEvent);
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "out select(%d,%d) -> "
          " ISSET %d,%d"
          " but no X events in queue",
          priv->selectfd,
          svc? svc->selectfd: 0,
          FD_ISSET(priv->selectfd, &fds),
          svc? FD_ISSET(svc->selectfd, &fds): 0);
        continue;
  	  }
	}

										/* ................................ */
    if (report.type == ButtonPress)
    {
      window = report.xbutton.window;
      x  = report.xbutton.x;
      y  = report.xbutton.y;
      b1 = (report.xbutton.state & Button1Mask) != 0;
      b2 = (report.xbutton.state & Button2Mask) != 0;
      b3 = (report.xbutton.state & Button3Mask) != 0;
      if (report.xbutton.button == Button1) b1 = 1;
      if (report.xbutton.button == Button2) b2 = 1;
      if (report.xbutton.button == Button3) b3 = 1;
    }
										/* ................................ */
    else
    if (report.type == MotionNotify)	/* motion event? 					*/
    {
	  unsigned int state;
	  window = report.xmotion.window;
      if (report.xmotion.is_hint)		/* just a motion hint? 				*/
	  {
        Window root, child;
        int rootx, rooty;
        X(XQueryPointer,(				/* go get pointer position 			*/
          priv->display,
          window,
          &root, &child, 
          &rootx, &rooty,
          &x, &y,
          &state));
	  }
	  else								/* full motion report? 				*/
	  {
		x = report.xmotion.x;
		y = report.xmotion.y;
        state = report.xmotion.state;
	  }
      b1 = (state & Button1Mask) != 0;
      b2 = (state & Button2Mask) != 0;
      b3 = (state & Button3Mask) != 0;
    }
										/* ................................ */
    else
    if (report.type == KeyPress ||
        report.type == KeyRelease)
    {
	  DT_MEMSET(s, 0, sizeof(s));
      X(XLookupString,(&report.xkey,	/* get keysym and string		    */
        s, sizeof(s), &keysym, NULL));
      window = report.xkey.window;
      x  = report.xkey.x;
      y  = report.xkey.y;
      b1 = (report.xkey.state & Button1Mask) != 0;
      b2 = (report.xkey.state & Button2Mask) != 0;
      b3 = (report.xkey.state & Button3Mask) != 0;
    }
										/* ................................ */
    else
    if (report.type == Expose)
    { 
      window = report.xexpose.window;
    }
										/* ................................ */
    else
    if (report.type == ConfigureNotify)
    { 
      window = report.xconfigure.window;
    }
										/* ................................ */
    else
    if (report.type == ButtonRelease)
    { 
      window = report.xbutton.window;
    }
										/* ................................ */
    else
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "unknown event type %d",
        report.type);
      continue;							/* can't make us quit			    */
	}

    elem = DTLIST_ELEM_HEAD;
    while (elem)
    {
      DT_Q(dtlist_next,(ctl,			/* fetch next window in list	    */
        &hws->windowlist, elem,
        &elem));
      if (elem)							/* not the last in the list?	    */
	  {
		hws_win = (dthws_win_t *)elem->user;
		if ((unsigned long)hws_win->id == window)
		  break;
	  }
    }

    if (elem == NULL)					/* didn't find the window?			*/
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "event type 0%08x on unknown window 0x%08x",
        report.type, window);
      continue;							/* can't make us quit			    */
	}

    priv_win = (dthws_x_priv_win_t *)	/* our private info on the window 	*/
      hws_win->priv;		
	
	if (report.type == MotionNotify &&	/* cursor motion? 					*/
		priv_win->cursorx == x &&		/* result of a warp? 				*/
		priv_win->cursory == y)
	  continue;							/* ignore the event 				*/

    event->id = window;					/* remember where event happened    */

    if (report.type == ButtonPress ||	/* we need to save the state?		*/
        report.type == MotionNotify ||
        report.type == KeyPress ||
        report.type == KeyRelease)
    {
      event->mousex = x;
      event->mousey = y;
      event->left = b1;
      event->middle = b2;
      event->right = b3;
      DT_MEMSET(event->keys, 0,			/* clear any key value			    */
        sizeof(event->keys));
	  priv_win->cursorx = x;			/* remember cursor to save warps	*/
	  priv_win->cursory = y;
    }

    event->type = DT_EVENT_NOTHING;
	switch (report.type)
	{
										/* ................................ */
	  case ConfigureNotify:
        priv_win->border_width = 
          report.xconfigure.border_width;

	    if (hws_win->xe !=				/* this looks like a resize? 		*/
			report.xconfigure.width ||
			hws_win->ye !=
			report.xconfigure.height)
		{
    XWindowAttributes attributes;
    XSizeHints hints;

    X(XGetWindowAttributes,(
      priv->display,
      priv_win->window,
      &attributes));

	X(XGetNormalHints,(
      priv->display,
      priv_win->window,
      &hints));
	
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,  
      "current geometry at (%d, %d) size (%d, %d)",
      hws_win->x0, hws_win->y0,      
      hws_win->xe, hws_win->ye);

    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,        
      "window 0x%08x attributes:"
      " at (%d, %d) size (%d, %d)"
      " border %d",
      priv_win->window,
      attributes.x,
      attributes.y,
      attributes.width,
      attributes.height,
      attributes.border_width);

    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,        
      "window 0x%08x hints:"
      " at (%d, %d) size (%d, %d)",
      priv_win->window,
      hints.x,
      hints.y,
      hints.width,
      hints.height);

		  event->type = DT_EVENT_RESIZED;
		  event->xe = report.xconfigure.width;
		  event->ye = report.xconfigure.height;
		}
		else							/* not a resize, must be a move */
		{
		  event->type = DT_EVENT_MOVED;
		  hws_win->x0 =
		  event->x0 = report.xconfigure.x -
            priv_win->xborder +
            report.xconfigure.border_width / 2;
		  hws_win->y0 =
		  event->y0 = report.xconfigure.y -
            priv_win->yborder +
            report.xconfigure.border_width / 2;
		}
		
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "window \"%s\" saw (se=%d) ConfigureNotify"
          " now at (%4d,%4d) size (%4d,%4d) %s",
          priv_win->name,
          report.xconfigure.send_event,
          event->x0, event->y0,
          event->xe, event->ye,
	      event->type == DT_EVENT_MOVED?
            "MOVED": "RESIZED");

      break;
										/* ................................ */
	  case MotionNotify:
        event->type = DT_EVENT_MOUSEMOVE;
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "window \"%s\" saw MotionNotify"
          " (%4d,%4d) %d%d%d is_hint=%d",
          priv_win->name,
          x, y, b1, b2, b3, 
          report.xmotion.is_hint);
      break;
										/* ................................ */
	  case ButtonPress:
      {
		long dt = report.xbutton.time -
          priv_win->unclick_time;
		if (priv_win->click_state ==	/* same button state? 				*/
			report.xbutton.state &&
            dt > 0 && dt < 300L)		/* and so quick? 					*/
		{
          event->type = DT_EVENT_MOUSEDOUBLEPRESS;
		  if (event->left) event->left++;
		  if (event->middle) event->middle++;
		  if (event->right) event->right++;
		}
		else
          event->type = DT_EVENT_MOUSEPRESS;
		priv_win->click_state =
          report.xbutton.state;
		priv_win->click_time =
		  report.xbutton.time;
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "window \"%s\" saw ButtonPress"
          " dt=%ldms (%4d,%4d) %d%d%d",
          priv_win->name, dt,
          x, y, b1, b2, b3);
	  }
      break;
										/* ................................ */
	  case ButtonRelease:
      {
		long dt = report.xbutton.time -
          priv_win->click_time;
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "window \"%s\" saw ButtonRelease"
          " dt=%ld ms",
          priv_win->name, dt);
		priv_win->unclick_time =
		  report.xbutton.time;
	  }
      break;
										/* ................................ */
	  case Expose:
        dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "window \"%s\" saw Expose %d"
          " (%4d,%4d) (%4d,%4d)",
          priv_win->name,
          report.xexpose.count,
          report.xexpose.x,
          report.xexpose.y,
          report.xexpose.width,
          report.xexpose.height);
#       ifndef NOSHM
		if (priv_win->isshared)
		  X(XShmPutImage,(priv->display,
            priv_win->window,
            priv_win->gc,
            priv_win->image,
            report.xexpose.x,
            report.xexpose.y,
            report.xexpose.x,
            report.xexpose.y,
            report.xexpose.width,
            report.xexpose.height,
            False))
		else
#       endif
          X(XPutImage,(priv->display,	/* replace the obstructed part	    */
            priv_win->window,
            priv_win->gc,
            priv_win->image,
            report.xexpose.x,
            report.xexpose.y,
            report.xexpose.x,
            report.xexpose.y,
            report.xexpose.width,
            report.xexpose.height));
        X(XFlush,(priv->display));
      break;
										/* ................................ */
      case KeyPress:
	  if (keysym >= XK_space &&			/* includes a-z, 0-9, punctuation	*/
          keysym <= XK_asciitilde)
      {
        int i;
        event->type = DT_EVENT_KEYBOARD;
        dt_dbg(ctl, F,
          DT_DBG_MASK_ASYNC,        
          "window \"%s\" saw KeyPress"   
          " KeySym \"%s\", char 0x%02x",
          priv_win->name,
          XKeysymToString(keysym),
          s[0]&0xff);
        for (i=0;						/* use remapped string			    */
             s[i] && 
             i < DT_KEYSTRING_MAXLEN; 
             i++)
          event->keys[i] = s[i];
        event->nkeys = i;
      }
      else
										/* ................................ */

	  if (keysym == XK_Escape ||		/* only care about control keys		*/
          keysym == XK_Left ||
          keysym == XK_Right ||
          keysym == XK_Up ||
          keysym == XK_Down ||
          keysym == XK_BackSpace ||
          keysym == XK_Tab ||
          keysym == XK_Return)

      {
        int i;
        event->type = DT_EVENT_KEYBOARD;
        dt_dbg(ctl, F,
          DT_DBG_MASK_ASYNC,        
          "window \"%s\" saw KeyPress, KeySym \"%s\"",
          priv_win->name,
          XKeysymToString(keysym));
		switch(keysym)
		{
		  case XK_Left:      event->keys[0] = DT_KEY_LEFTARROW;  break;
		  case XK_Right:     event->keys[0] = DT_KEY_RIGHTARROW; break;
		  case XK_Up:        event->keys[0] = DT_KEY_UPARROW;    break;
		  case XK_Down:      event->keys[0] = DT_KEY_DOWNARROW;  break;
		  case XK_Escape:    event->keys[0] = DT_KEY_ESCAPE;     break;
		  case XK_BackSpace: event->keys[0] = 0x08;              break;
		  case XK_Tab:       event->keys[0] = 0x09;              break;
		  case XK_Return:    event->keys[0] = '\n';              break;
		  default:           event->keys[0] = keysym;
		}
        event->nkeys = 1;
      }
      break;
										/* ................................ */
      case KeyRelease:
      break;
										/* ................................ */
      default:
        dt_err(ctl, F, 
          "unexpected event type %d (0x%08x)",
          report.type, report.type);
      break;
										/* ................................ */
    }

    XERROR_CHECK("event loop");
    if (event->type !=					/* user-generated event?		    */
        DT_EVENT_NOTHING)
	{
	  if (flag == 0)					/* just polling? 					*/
	    quit = 1;
	  else
      if (flag == 1)					/* to quit after any event?			*/
        quit = 1;
      else
      if (flag == 3)					/* to quit on any user event?	    */
        quit = 1;
	}
	else								/* non-user event?				    */
	{
	  if (flag == 1 &&					/* supposed to quit immediately?    */
          !XPending(priv->display))		/* event queue has nothing else?	*/
        quit= 1;
	}
	if (flag == 0 &&					/* just polling? 					*/
        !XPending(priv->display))		/* event queue has nothing else?	*/
      quit= 1;
    XERROR_CHECK(XPending);

  } while (!quit);

  return DT_RC_GOOD;
}
#undef F

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_destroy(					/* get rid of window			    */
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_x_win_destroy");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;
  dt_rc_e rc = DT_RC_GOOD;

# ifndef NOSHM
  if (priv_win->isshared)
  {
    X(XShmDetach,(priv->display,
      &priv_win->shminfo));
	priv_win->isshared = 0;
  }
# endif

  if (priv_win->image != NULL)
  {
    priv_win->image->data = NULL;	/* tell X not to free data			*/
    X(XDestroyImage,(priv_win->image));	/* let X free its image structure	*/
    priv_win->image = NULL;
  }

  if (priv_win->shminfo.shmaddr != NULL &&
	  priv_win->shminfo.shmaddr != (void *)(-1))
  {
	int sysrc;
    sysrc = shmdt(
      priv_win->shminfo.shmaddr);
	if (sysrc != 0)
	  rc = dt_err(ctl, F,
        "shmdt(0x%08lx) failed,"
        " errno %d",
        priv_win->shminfo.shmaddr,
        errno);
	priv_win->shminfo.shmaddr = NULL;
  }

  if (priv_win->shminfo.shmid != -1 && 
	  priv_win->shminfo.shmid != 0) 
  {
	int sysrc;
    sysrc = shmctl(
      priv_win->shminfo.shmid,
      IPC_RMID, 0);
	if (sysrc != 0)
	  rc = dt_err(ctl, F,
        "shmctl(%d, IPC_RMID, 0) failed,"
        " errno %d",
        priv_win->shminfo.shmid,
        errno);
	priv_win->shminfo.shmid = 0;
  }

  if (priv_win->colormap != 0 &&
	  priv_win->colormap != priv->colormap)
  {
    X(XFreeColormap,(priv->display,
      priv_win->colormap));
    priv_win->colormap = 0;
  }

  if (priv_win->cursor != 0)
  {
    X(XFreeCursor,(priv->display,
      priv_win->cursor));
    priv_win->cursor = 0;
  }

  X(XFreeGC,(priv->display,
    priv_win->gc));

  X(XDestroyWindow,(priv->display,		/* delete window					*/
    priv_win->window));

  X(XFlush,(priv->display));			/* force deletion now				*/

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "window \"%s\" destroyed",
    priv_win->name);

  DT_I(dtos_free2,(ctl, priv_win,
    F, "priv win"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_export_color(					/* write window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dthws_x_export_color");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)(hws_win? hws_win->priv: NULL);
  XColor xcolor[256];
  int i;

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
    "priv_win->colormap is 0x%08lx->0x%08lx,"
    " start=%d, count=%d",
    priv_win, 
    priv_win? 
      (void *)priv_win->colormap: NULL,
    start, count);
  
  if (priv_win == NULL ||
	  priv_win->colormap == 0)			/* no colormap capability in win?	*/
    return DT_RC_GOOD;

										/* ................................ */
  if (priv_win->colormap ==				/* window uses default colormap?	*/
	  priv->default_colormap)
  { 
	unsigned long plane_masks[1];
	int nset;
	unsigned long set[256];

	for (nset=count; nset>0; nset--)	/* search for free colors 			*/
      if (XAllocColorCells(
            priv->display,
            priv->default_colormap,
            False,
            plane_masks, 0,
            set, nset))
        break;
	XERROR_CHECK(XAllocColorCells);

	dt_dbg(ctl, F, 
      DT_DBG_MASK_WIN | 
      DT_DBG_MASK_LUT,
      "%d entries in root's colormap"
      " are settable",
      nset);

	for (i=0; i<nset; i++)				/* set up the ones we can		    */
	{
      int j = set[i];
	  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
        "setting color j=%3d 0x%02x 0x%02x 0x%02x",
        j, lut[j].r, lut[j].g, lut[j].b);
	  xcolor[i].red   = lut[j].r << 8;	/* X likes them shifted left	    */
	  xcolor[i].green = lut[j].g << 8;
	  xcolor[i].blue  = lut[j].b << 8;
	  xcolor[i].pixel = j;
	  xcolor[i].flags =
        DoRed | DoGreen | DoBlue;
	}

	X(XStoreColors,(					/* force root's colormap like ours	*/
      priv->display,
      DefaultColormap(
        priv->display,
        priv->screen_num), 
      xcolor, nset));

	X(XFreeColors,(						/* once we set them we're done	    */
      priv->display,
      DefaultColormap(
        priv->display,
        priv->screen_num), 
      set, nset, 0));
  }
										/* ................................ */
  else									/* window has writable colormap? 	*/
  {
    for (i=0; i<count; i++)				/* set flags and index				*/
    {
      xcolor[i].red   = lut[i].r << 8;	/* X likes them shifted left	    */
	  xcolor[i].green = lut[i].g << 8;
	  xcolor[i].blue  = lut[i].b << 8;
	  xcolor[i].pixel = i + start;
	  xcolor[i].flags =
        DoRed |
        DoGreen |
        DoBlue;
	}

    X(XStoreColors,(					/* load cells in window's colormap	*/
      priv->display,
      priv_win->colormap,
      xcolor, count));

	X(XFlush,(priv->display));			/* clear output buffer to server	*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_export_data(					/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_x_export_data");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;
  XID window;

  if (priv->use_multibuf)
	window = priv_win->multibuf[		/* use current multibuffer 			*/
      priv_win->multibufi];
  else
    window = priv_win->window;			/* use regular window 				*/
  
# ifndef NOSHM
  if (priv_win->isshared)
    XShmPutImage(priv->display,			/* update patch on screen		    */
      window,
      priv_win->gc,
      priv_win->image,
      x0, y0,
      x0, y0,
      xe, ye,
      False);      
  else
# endif
  {
    X(XPutImage,(priv->display,			/* update patch on screen		    */
      window,
      priv_win->gc,
      priv_win->image,
      x0, y0,
      x0, y0,
      xe, ye));
	DT_Q(hws->win_raise,(ctl, hws, 
      hws_win));
  }

# ifndef NOMBUF
  if (priv->use_multibuf)
  { 
	X(XmbufDisplayBuffers,(				/* display updated buffer 			*/
     priv->display, 1, &window, 0, 0));
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "switched to multibuf[%d] = 0x%08lx",
      priv_win->multibufi, window);
	priv_win->multibufi ^= 1;			/* next time use other one 			*/
  }
# endif

#ifdef NONO
    XFlush(priv->display);				/* force output immediately		    */
#endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_import_data(					/* get window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  return DT_RC_GOOD;
}

/*..........................................................................
| lut[N].i members not changed
| 
| color start is put into lut[0], start+count into lut[count-1]
|
| if hws_win is NULL, then DefaultColormap of screen is returned
*..........................................................................*/

static
dt_rc_e
dthws_x_import_color(					/* get window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dthws_x_import_color");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  Colormap colormap;
  XColor xcolor[256];
  int i;

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  if (hws_win == NULL)					/* caller wants base colormap?	    */
  {
	if (priv->colormap ==				/* all windows using root colormap? */
		priv->default_colormap)
	  colormap = priv->default_colormap;
	else								/* private colormap being used? 	*/
	{
	  for (i=0; i<count; i++)			/* don't care about actual values	*/
	  {
	    lut[i].r = 
        lut[i].g = 
        lut[i].b = i;
	    lut[i].i = 1;					/* presume all cells free 			*/
	  }
	  return DT_RC_GOOD;
	}
  }
  else									/* colormap of a specific window?	*/
  {
    dthws_x_priv_win_t *priv_win =
      (dthws_x_priv_win_t *)hws_win->priv;
    if (priv_win->colormap == 0)		/* no colormap capability in win?	*/
      return DT_RC_GOOD;
    colormap = priv_win->colormap;
  }

  for (i=0; i<count; i++)				/* want indices of all colors	    */
    xcolor[i].pixel = i + start;

  X(XQueryColors,(priv->display,		/* get colors					    */
    colormap, xcolor, count));

  for (i=0; i<count; i++)				/* store in caller's array		   	*/
  {
    lut[i].r = xcolor[i].red   >> 8;
    lut[i].g = xcolor[i].green >> 8;
    lut[i].b = xcolor[i].blue  >> 8;
	lut[i].i = 1;						/* presume all cells free 			*/
  }
										/* ................................ */

  if (hws_win == NULL)					/* caller wants base colormap?	    */
  {
	unsigned long plane_masks[1];
	int nset;
	unsigned long set[256];
	for (nset=256; nset>0; nset--)		/* search for free colors 			*/
      if (XAllocColorCells(
            priv->display,
            colormap,
            False,
            plane_masks, 0,
            set, nset))
        break;
    XERROR_CHECK(XAllocColorCells);

	X(XFreeColors,(						/* once we set them we're done	    */
      priv->display,
      colormap,
      set, nset, 0));

    for (i=0; i<count; i++)				/* presume no cell free				*/
	  lut[i].i = 0;

	for (i=0; i<nset; i++)				/* mark which cells really are free	*/
	  if (set[i] < (unsigned long)count)
		lut[set[i]].i = 1;
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_clear(						/* clear window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int background)
{
  DT_F("dthws_x_win_clear");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;

  X(XSetForeground,(priv->display,		/* filling clears to background	    */
    priv_win->gc,
    background));

  X(XFillRectangle,(priv->display,
    priv_win->window,
    priv_win->gc,
    0, 0, hws_win->xe, hws_win->ye));

  XFlush(priv->display);				/* force output immediately		    */

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_x_win_raise");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;

  X(XRaiseWindow,(						/* tell X to raise window			*/
    priv->display,
    priv_win->window));

  X(XFlush,(priv->display));			/* force output immediately		    */

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_x_win_lower");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;

  X(XLowerWindow,(						/* tell X to lower window			*/
    priv->display,
    priv_win->window));

  X(XFlush,(priv->display));			/* force output immediately		    */

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_cursor(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int *x,
  int *y,
  int op)
{
  DT_F("dthws_x_win_cursor");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;

  if (priv_win->cursor == 0)			/* haven't made our cursor yet? 	*/
  {
	char bitmap[32];
	Pixmap pixmap;
	XColor black;

	DT_MEMSET(bitmap, 0, sizeof(bitmap));
	pixmap =
      XCreatePixmapFromBitmapData(		/* make a bitmap 					*/
	    priv->display,
        priv_win->window,				/* drawable 						*/
        bitmap,							/* data 							*/
        16, 16,							/* size 							*/
        1,								/* foreground 						*/
        0,								/* background 						*/
        1);								/* depth 							*/
    XERROR_CHECK(XCreatePixmapFromBitmapData);
	DT_MEMSET(&black, 0, sizeof(black));
	priv_win->cursor =
	  XCreatePixmapCursor(				/* make a cursor 					*/
	    priv->display,
        pixmap,							/* source 							*/
		pixmap,							/* mask								*/
        &black,							/* foreground color 				*/
        &black,							/* background color 				*/
        0, 0);							/* hot point 						*/
    XERROR_CHECK(XCreatePixmapCursor);

    X(XFreePixmap,(priv->display, pixmap));	/* free pixmap resource			    */

	X(XDefineCursor,(priv->display,		/* associate cursor with window 	*/
      priv_win->window,
      priv_win->cursor));
  }
  
  if (op != DT_CURSOR_OP_QUERY &&
      op != DT_CURSOR_OP_OFF)
  {
    if (*x != priv_win->cursorx ||
        *y != priv_win->cursory)
    {
	  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
	    "warping to %d,%d", *x, *y);
   	    priv_win->cursorx = *x;
	    priv_win->cursory = *y;
	  X(XWarpPointer,(priv->display,
        priv_win->window, 
        priv_win->window, 
        0, 0, 0, 0,
        *x, *y));
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_x_win_move(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int x,
  int y)
{
  DT_F("dthws_x_win_move");
  dthws_x_priv_t *priv =
    (dthws_x_priv_t *)hws->priv;
  dthws_x_priv_win_t *priv_win =
    (dthws_x_priv_win_t *)hws_win->priv;
  Window root, child;
  int rootx, rooty;
  int winx, winy;
  unsigned int state;

  X(XMoveWindow,(priv->display,			/* move window 						*/
    priv_win->window, 
    x + priv_win->border_width/2, 
    y + priv_win->border_width/2));

  X(XFlush,(priv->display));			/* clear output buffer to server	*/

  X(XQueryPointer,(						/* go get pointer position 			*/
    priv->display,
    priv->rootwin,
    &root, &child, 
    &rootx, &rooty,
    &winx, &winy,
    &state));

  if ((root == priv_win->window ||		/* pointer is inside this window? 	*/
	   child == priv_win->window) ||
	  priv_win->parent &&
	  (root == priv_win->parent ||		/* pointer is inside this window? 	*/
	   child == priv_win->parent))
  {
    X(XWarpPointer,(priv->display,		/* pointer follows 					*/
      priv->rootwin,
      None,								/* relative motion				    */
      0, 0, 0, 0,						/* anywhere in root window		    */
      x - hws_win->x0,
      y - hws_win->y0));
  }
    
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_x_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *server)
{
  DT_F("dthws_x_connect");
#if DTPRJ_X == DT_1
  dthws_x_priv_t *priv = NULL;
  char s[128];
  char *b;
  XVisualInfo need;						/* stuff for visual					*/
  int nmatch;
  int i;
  dt_rc_e rc;

  DT_Q(dtos_malloc2,(ctl, 
    (void **)&priv,
    sizeof(*priv),
    F, "priv"));
  DT_MEMSET(priv, 0, sizeof(*priv));

  if (server == NULL ||					/* no server specified? 			*/
	  strlen(server) == 0)
    server = getenv("DISPLAY");
  
  if (server == NULL ||					/* still no server specified? 		*/
	  strlen(server) == 0)
    server = ":0.0";

  if (strlen(server) > sizeof(s)-1)
  {
	rc = dt_err(ctl, F,
      "server name length %d"
      " too long (max %d)",
      strlen(server), sizeof(s)-1);
	goto cleanup;
  }

  strcpy(s, server);					/* copy to tmp space 				*/
  b = strchr(s, ',');					/* check for a comma in the server 	*/
  if (b != NULL)						/* there is a comma? 				*/
    *b++ = '\0';						/* separate args from server 		*/
  else
    b = "";

  if (strchr(b, 'B') &&
	  strchr(b, 'b') ||
	  strchr(b, 'C') &&
	  strchr(b, 'c') ||
	  strchr(b, 'D') &&
	  strchr(b, 'd') ||
	  strchr(b, 'M') &&
	  strchr(b, 'm') ||
	  strchr(b, 'R') &&
	  strchr(b, 'r') ||
	  strchr(b, 'S') &&
	  strchr(b, 's'))
  {
	rc = dt_err(ctl, F,
      "conflicting options in \"%s\"",
      b);
	goto cleanup;
  }

  priv->option_backing_store    = strchr(b, 'B') != NULL;
  priv->option_default_colormap = strchr(b, 'D') != NULL;
  priv->option_shared_colormap  = strchr(b, 'C') != NULL;
  priv->option_multibuffer      = strchr(b, 'M') != NULL;
  priv->option_remap_colors     = strchr(b, 'R') != NULL;
  priv->option_shared_memory    = strchr(b, 'S') != NULL;

  priv->display =						/* start talking with X server		*/ 
    XOpenDisplay(s);
  if (priv->display == 0)
  {
    rc = dt_err(ctl, F,
      "at XOpenDisplay(\"%s\")", s);
	goto cleanup;
  }

  priv->selectfd =						/* hopefully socket to server		*/
    ConnectionNumber(			
      priv->display);

  DT_C(dthws_x_scr_from_name,(ctl,		/* determine screen number		    */
    priv, s,
    &priv->screen_num));

  priv->screen = ScreenOfDisplay(
    priv->display, priv->screen_num);

  priv->default_colormap =				/* remember colormap for screen	    */
    DefaultColormap(priv->display,
      priv->screen_num);

  priv->rootwin = RootWindow(			/* on same screen as root			*/
    priv->display, priv->screen_num);

  XSetErrorHandler(xerror_handler);		/* handle non-fatal error events    */
  XSetIOErrorHandler(xioerror_handler);	/* handle fatal error events    	*/
  
										/* ................................ */
  if (  /* s[0] == ':' || */			/* server on same computer? 		*/
	  priv->option_shared_memory)		/* or told explicitly to use it? 	*/
  {
# ifndef NOSHM
	Status status;
	int major, minor;
	Bool issharedpixmaps;
	status = XShmQueryVersion(			/* check for shared image extension */
      priv->display,
      &major, &minor, &issharedpixmaps);

	if (status == True)					/* server supports shared images? 	*/
	{
      priv->use_shared = 1;
	  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "shared memory extension used");
	}
	else
# endif
	if (priv->option_shared_memory)
	  return dt_err(ctl, F,
        "shared memory extension"
        " requested but not available");
  }
  
										/* ................................ */
  if (priv->option_multibuffer)			/* told to do multi buffering?		*/
  {
# ifndef NOMBUF
	Bool status;
	int event_base, error_base;
	status = XmbufQueryExtension(		/* check for multibuf extension 	*/
      priv->display,
      &event_base, &error_base);
    XERROR_CHECK(XmbufQueryExtension);
	if (status == True)					/* server supports multi buffering?	*/
	{
      priv->use_multibuf = 1;
	  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "multi buffer extension used");
	}
	else
# endif
	  return dt_err(ctl, F,
        "multi buffer extension"
        " requested but not available");
  }
										/* ................................ */
  priv->visual = DefaultVisual(			/* easy way of choosing visual	    */
    priv->display, priv->screen_num);

  need.visualid =						/* get id for the visual we chose   */
    XVisualIDFromVisual(
      priv->visual);

  priv->visual_info = XGetVisualInfo(	/* get info for the visual we chose	*/
    priv->display, VisualIDMask,
    &need, &nmatch);
  XERROR_CHECK(XGetVisualInfo);
  if (nmatch == 0)						/* this should never happen			*/
    return dt_err(ctl, F,
      "no info for chosen visual");

										/* ................................ */
  priv->visual = DefaultVisual(			/* easy way of choosing visual	    */
    priv->display, priv->screen_num);
  XERROR_CHECK(DefaultVisual);

  need.visualid =						/* get id for the visual we chose   */
    XVisualIDFromVisual(priv->visual);

  priv->visual_info = XGetVisualInfo(	/* get info for the visual we chose	*/
    priv->display, VisualIDMask,
    &need, &nmatch);
  XERROR_CHECK(XGetVisualInfo);
  if (nmatch == 0)						/* this should never happen			*/
    return dt_err(ctl, F,
      "no info for chosen visual");

										/* ................................ */
  if (priv->option_default_colormap)	/* all windows use default colormap?*/
  {
	priv->colormap = 
      priv->default_colormap;

	dt_dbg(ctl, F,
      DT_DBG_MASK_WIN | 
      DT_DBG_MASK_LUT,
	  "all windows use default"
      " colormap 0x%08lx",
      priv->colormap);

  }
  else									/* ................................ */
  if (priv->option_shared_colormap)		/* all windows share colormap? 		*/
  {
    XColor xcolor[256];
	priv->colormap =					/* create a colormap				*/
      XCreateColormap(
        priv->display,
        priv->rootwin,
        priv->visual, AllocAll);
    if (priv->colormap == 0)
      return dt_err(ctl, F,
        "at XCreateColormap");

    for (i=0; i<256; i++)				/* want indices of all colors	    */
	{
      xcolor[i].pixel = i;
      xcolor[i].flags =
        DoRed |
        DoGreen |
        DoBlue;
	}

    X(XQueryColors,(priv->display,		/* get root's colors			    */
      DefaultColormap(priv->display,
        priv->screen_num),
      xcolor, 256));

    X(XStoreColors,(priv->display,		/* set this window's colors		    */
      priv->colormap,
      xcolor, 256));

	dt_dbg(ctl, F,
      DT_DBG_MASK_WIN | 
      DT_DBG_MASK_LUT,
	  "all windows share colormap 0x%08lx",
      priv->colormap);
  }
  else
	dt_dbg(ctl, F,
      DT_DBG_MASK_WIN | 
      DT_DBG_MASK_LUT,
	  "each window gets a private"
      " colormap");
										/* ................................ */
  if (priv->option_backing_store)
  {
	int does = DoesBackingStore(
      priv->screen);
	if (does != WhenMapped &&			/* screen has a backing store? 		*/
	    does != Always)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "backing store requested"
        " but not available");
	  priv->option_backing_store = 0;
	}
  }

  if (priv->option_backing_store)
    dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "backing store used");
										/* ................................ */
  if (priv->option_remap_colors)
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "will attempt to match"
      " image lut to display lut");
  else
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "will not attempt to match"
      " image lut to display lut");
										/* ................................ */

  hws->priv = (void *)priv;
  hws->xe = DisplayWidth(				/* return display size to caller    */
    priv->display, priv->screen_num);
  hws->ye = DisplayHeight(
    priv->display, priv->screen_num);
  hws->do_remap = priv->option_remap_colors;
  
  hws->disconnect = dthws_x_disconnect;
  hws->win_create = dthws_x_win_create;
  hws->wait_event = dthws_x_wait_event;
  hws->win_destroy = dthws_x_win_destroy;
  hws->export_data = dthws_x_export_data;
  hws->import_data = dthws_x_import_data;
  hws->export_color = dthws_x_export_color;
  hws->import_color = dthws_x_import_color;
  hws->win_move = dthws_x_win_move;
  hws->win_clear = dthws_x_win_clear;
  hws->win_raise = dthws_x_win_raise;
  hws->win_lower = dthws_x_win_lower;
  hws->win_cursor = dthws_x_win_cursor;
  return DT_RC_GOOD;

cleanup:

  if (priv)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  return rc;

# else
  return dt_err_notlinked(ctl, F, "X Windows");
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
