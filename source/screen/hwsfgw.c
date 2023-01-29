/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: hwsfgw.c,v $ $Revision: 1.1 $");
#include <dtack/label.h>
#include <dtack/errno.h>

#if DTPRJ_FGW == DT_1

#  ifndef DTDEPEND
#    include <fgwin.h>
#  endif

#endif

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/hws.h>
#include <dtack/str.h>

#include <dtack/msw.h>

extern
dt_rc_e 
DTCONFIG_API1
dtmsw_message_string(					/* convert message to string		*/
  dt_ctl_t *ctl,
  int message,
  const char **string);

#if DTPRJ_FGW == DT_1

extern dthws_connect_f dthws_connect;
static dthws_disconnect_f dthws_fgw_disconnect;
static dthws_win_create_f dthws_fgw_win_create;
static dthws_wait_event_f dthws_fgw_wait_event;
static dthws_handle_event_f dthws_fgw_handle_event;
static dthws_win_destroy_f dthws_fgw_win_destroy;
static dthws_export_data_f dthws_fgw_export_data;
static dthws_import_data_f dthws_fgw_import_data;
static dthws_export_color_f dthws_fgw_export_color;
static dthws_import_color_f dthws_fgw_import_color;
static dthws_win_clear_f dthws_fgw_win_clear;
static dthws_win_raise_f dthws_fgw_win_raise;
static dthws_win_lower_f dthws_fgw_win_lower;
static dthws_win_lower_f dthws_fgw_win_lower;
static dthws_win_cursor_f dthws_fgw_win_cursor;

#include <dtack/mem.h>

typedef struct {
  HWND hwnd;
  HDC hdc;
  HPALETTE hpal;
  int inverted;
  int destroyed;						/* destroyed by window manager 		*/
  int control_key;
  int shift_key;
  char name[DT_WINDOWNAME_MAXLEN+1];
} priv_win_t;

typedef struct {
  const char *classname;
  unsigned char tmplut[768];
} priv_t;

#define LONG_CTL (0)					/* SetWindowLong indexes 			*/
#define LONG_HWS (4)
#define LONG_HWS_WIN (8)
#define LONG_EXTRA (12)					/* amount of extra data 			*/

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_disconnect(
  dt_ctl_t *ctl,
  dthws_t *hws)
{
  DT_F("dthws_fgw_disconnect");
  priv_t *priv =
    (priv_t *)hws->priv;
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "disconnecting");

  fg_vbfin();							/* terminate virtual buffers		*/

  DT_Q(dtos_free2,(ctl, priv,
    F, "priv"));
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "disconnected");

  return DT_RC_GOOD;
}

/*..........................................................................
 * window structure has already been found or allocated and name assigned
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_create(					/* make/display a new window		*/
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
  DT_F("dthws_fgw_win_create");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win;
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/

  if (msw == NULL)
    return dt_err(ctl, F,
      "msw structure is NULL"
      " (program not initialized correctly)");

  if (strlen(windowname) >
      sizeof(priv_win->name)-1)
    return dt_err(ctl, F,
      "\"%s\" too long", windowname);

  DT_Q(dtos_malloc2,(ctl,				/* space for the private structure 	*/
    &hws_win->priv,
    sizeof(*priv_win),
    F, "priv win"));
  priv_win = (priv_win_t *)
    hws_win->priv;

  DT_MEMSET(priv_win, 0, 
    sizeof(*priv_win));

  strcpy(priv_win->name,
    windowname);
										/* ................................ */
  priv_win->hwnd = CreateWindow(		/* create the image window 		*/
    priv->classname, 
    priv_win->name,
	WS_OVERLAPPEDWINDOW,
    x0, y0, xe+8, ye+27,
    (HWND)NULL, 
    (HMENU)NULL,
    (HINSTANCE)msw->hinstance, 
    NULL);
  if (priv_win->hwnd == (HWND)NULL)
    return dt_err(ctl, F,
      "CreateWindow(class=\"%s\","
      " name=\"%s\") failed"
#     ifdef DTCONFIG_IMPLIED_MSW32
        " (GetLastError=%ld)"
#     endif
      ,
      priv->classname,
	  priv_win->name
#     ifdef DTCONFIG_IMPLIED_MSW32
        , GetLastError()
#     endif
    );
	
  hws_win->id = (long)priv_win->hwnd;	/* unique window identifier			*/

  hws_win->xe = xe;						/* remember the size			    */
  hws_win->ye = ye;

  hws_win->x0 = x0;						/* remember the position 			*/
  hws_win->y0 = y0;

  SetWindowLong(priv_win->hwnd,			/* pass stuff into window proc 		*/
    LONG_CTL,
    (long)ctl);
  SetWindowLong(priv_win->hwnd, 
    LONG_HWS, 
    (long)hws);
  SetWindowLong(priv_win->hwnd, 
    LONG_HWS_WIN,
    (long)hws_win);

  priv_win->hdc = GetDC(				/* get and keep the window's dc	    */
    priv_win->hwnd);
  if (priv_win->hdc == (HDC)NULL)
    return dt_err(ctl, F,
      "GetDC failed");

  fg_setdc(priv_win->hdc);				/* activate before realizing	    */

  {
	int i;
	unsigned char *tmplut = 
      priv->tmplut;
	for (i=0; i<256; i++)				/* customize the pallette 			*/
	{
      *tmplut++ = i;
	  *tmplut++ = i;
	  *tmplut++ = i;
	}
    priv_win->hpal = fg_logpal(10, 236,	/* get a logical pallette 			*/
      (char *)priv->tmplut);
    if (priv_win->hpal ==
		(HPALETTE)NULL)
	  return dt_err(ctl, F,
        "fg_logpal failed");
	fg_realize(priv_win->hpal);			/* activate the pallette 			*/
	dt_dbg(ctl, F, DT_DBG_MASK_LUT,
      "created pallette 0x%08lx"
      " on dc 0x%08lx",
      (long)priv_win->hpal, 
      (long)priv_win->hdc);
  }

  ShowWindow(priv_win->hwnd,			/* pop up the window				*/
    SW_NORMAL);
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "ShowWindow(0x%04x) called", priv_win->hwnd);

  UpdateWindow(priv_win->hwnd);			/* send (not post) paint message    */

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "UpdateWindow(0x%04x) called", priv_win->hwnd);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_vballoc(						/* write window's data				*/
  dt_ctl_t *ctl,
  int xe,
  int ye,
  int *handle)
{
  DT_F("dthws_fgw_vballoc");
  dt_rc_e rc = DT_RC_GOOD;

  *handle = fg_vballoc(xe, ye);			/* virtual buffer for our data 	*/
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
	"fg_vballoc(%d, %d) = %d",
    xe, ye, *handle);

  if (*handle == -1)
	rc = dt_err(ctl, F,
      "Windows virtual buffer table full,"
      " fg_vballoc(%d, %d) returned %d",
      xe, ye, *handle);
  else
  if (*handle == -2)
	rc = dt_err(ctl, F,
      "cannot allocate memory for the Windows virtual buffer,"
      " fg_vballoc(%d, %d) returned %d",
      xe, ye, *handle);
  else
  if (*handle < 0 || *handle > 31)
	rc = dt_err(ctl, F,
      "cannot export image to Windows,"
      " fg_vballoc(%d, %d) returned %d",
      xe, ye, *handle);

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_update(						/* write window's data				*/
  dt_ctl_t *ctl,
  HDC hdc,
  HPALETTE hpal,
  void *memalloc,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_fgw_update");
  int handle;
  int fgrc = -1;
  long offset = (long)y0 * (long)xe;
  long max = (long)xe * (long)ye;
  int row;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dthws_fgw_vballoc,(ctl,			/* virtual buffer for our data 		*/
    xe, ye, &handle));

  fgrc = fg_vbopen(handle);				/* make us the active virtual buffer */
  if (fgrc != 0)
  {
	rc = dt_err(ctl, F,
      "fg_vbopen(%d) failed, rc %d",
      handle, fgrc);
	goto cleanup;
  }

  fg_setdc(hdc);
  fg_realize(hpal);						/* activate the pallette 			*/

  fg_vbcolors();						/* set vb's colors to pallette's    */

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "x0=%d, y0=%d, xe=%d, ye=%d"
    " offset=%ld, max=%ld, hpal=0x%08lx",
    x0, y0, xe, ye, offset, max,
    (long)hpal);

  row = 0;
  while (row<ye)
  {
	void *data;
	unsigned int size, nrows;
	DT_Q(dtmem_map,(ctl,				/* map next chunk					*/
      memalloc, offset,
      &data, &size));
	nrows = size / xe;
	fg_move(0, row+nrows-1);
	fg_putimage((char *)data, xe,		/* copy data into virtual buffer	*/
      nrows);
	row += nrows;
	DT_Q(dtmem_unmap,(ctl,				/* unmap the chunk					*/
      memalloc, offset));
	offset += size;
  }

  fg_vbpaste(							/* virtual buffer to active page 	*/
    0, xe-1, 0, ye-1,
    x0, y0+ye-1);

cleanup:
  if (fgrc == 0)
    fg_vbclose();

  if (handle >= 0 && 
	  handle <= 31)
  {
    fg_vbfree(handle);
    dt_dbg(ctl, F, DT_DBG_MASK_WIN,
	  "fg_vbfree(%d)",
      handle);
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_destroy(					/* get rid of window			    */
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_fgw_win_destroy");
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  dt_rc_e rc = DT_RC_GOOD;


  if (priv_win->hwnd != (HWND)NULL)
  {
	if (priv_win->hpal != (HPALETTE)NULL)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "DeleteObject(0x%08lx) for \"%s\"",
        (long)priv_win->hpal,
        priv_win->name);
	  DeleteObject(priv_win->hpal);		/* get rid of pallette 				*/
	}
	if (priv_win->hdc != (HDC)NULL)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "ReleaseDC(0x%08lx) for \"%s\"",
        (long)priv_win->hdc,
        priv_win->name);
      ReleaseDC(priv_win->hwnd, 
        priv_win->hdc);
	}
    SetWindowLong(priv_win->hwnd, 
      LONG_HWS, 
      (long)NULL);
    SetWindowLong(priv_win->hwnd, 
      LONG_HWS_WIN,
      (long)NULL);
	if (!priv_win->destroyed)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "DestroyWindow(0x%08lx) for \"%s\"",
        (long)priv_win->hwnd,
        priv_win->name);
	  DestroyWindow(priv_win->hwnd);
	}
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "dtos_free2(priv_win=0x%08lx) for \"%s\"",
    priv_win,
    priv_win->name);

  DT_I(dtos_free2,(ctl, priv_win,
    F, "priv win"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_export_color(					/* write window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut8,
  int start,
  int count)
{
  DT_F("dthws_fgw_export_color");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win =
    (priv_win_t *)(hws_win? hws_win->priv: NULL);
  unsigned char *tmplut;
  int i;

  if (hws_win == NULL)					/* color setting background lut? 	*/
    return DT_RC_GOOD;					/* don't do it 						*/

  if (priv_win->hpal == (HPALETTE)NULL)
	return dt_err(ctl, F,
      "no pallette assigned");

  tmplut = priv->tmplut;
  for (i=0; i<count; i++)
  {
	*tmplut++ = lut8[i].r;
	*tmplut++ = lut8[i].g;
	*tmplut++ = lut8[i].b;
  }
  
  DeleteObject(priv_win->hpal);			/* get rid of pallette 				*/
		 
  fg_setdc(priv_win->hdc);				/* activate before realizing	    */

#ifdef NONO
  fg_realize(priv->hpal);				/* activate the pallette 			*/

  fg_setdacs(start, count,				/* update colors 					*/ 
    priv->tmplut);		
#endif

  priv_win->hpal = fg_logpal(			/* get a new logical pallette 		*/
    start, count,
    (char *)priv->tmplut);
  if (priv_win->hpal == (HPALETTE)NULL)
	return dt_err(ctl, F,
      "fg_logpal failed");

  fg_realize(priv_win->hpal);			/* activate the pallette 			*/

  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
    "new pallette %d colors starting at %d"
    " in pallette 0x%08lx"
    " on dc 0x%08lx",
    count, start,
    (long)priv_win->hpal, 
    (long)priv_win->hdc);

	  DT_Q(dthws_fgw_update,(ctl, 		/* write window's data				*/
        priv_win->hdc,
        priv_win->hpal,					/* pallette everyone's using	    */
        hws_win->memalloc,				/* data from backing store 			*/
        0, 0,							/* just update the whole thing	    */
        hws_win->xe,
        hws_win->ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_export_data(					/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_fgw_export_data");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;

  DT_Q(dthws_fgw_update,(ctl,
    priv_win->hdc, 
    priv_win->hpal,
    hws_win->memalloc,					/* data from backing store 			*/
    x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_import_data(					/* get window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
#ifdef NONO
  DT_F("dthws_fgw_import_data");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    win->priv;
#endif
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
dthws_fgw_import_color(					/* get window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut,
  int start,
  int count)
{
#ifdef NONO
  DT_F("dthws_fgw_import_color");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
#endif
  int i;
  for (i=0; i<count; i++)				/* store in caller's array		   	*/
  {
    lut[i].r = i+start;
    lut[i].g = i+start;
    lut[i].b = i+start;
	lut[i].i = (i > 20 && i < 236);		/* presume all cells free 			*/
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_clear(						/* clear window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int background)
{
#ifdef NONO
  DT_F("dthws_fgw_win_clear");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
#endif
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
#ifdef NONO
  DT_F("dthws_fgw_win_raise");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
#endif
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
#ifdef NONO
  DT_F("dthws_fgw_win_lower");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
#endif
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_print(					/* print window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)
{
  DT_F("dthws_fgw_win_print");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
  int handle;
  int openrc = -1;
  HDC hdc = (HDC)NULL;
  int printrc = -1;
  int x0 = 0;
  int y0 = 0;
  int xe = hws_win->xe;
  int ye = hws_win->ye;
  long offset = (long)y0 * (long)xe;
  long max = (long)xe * (long)ye;
  int row;
  int pxe, pye;
  dt_rc_e rc = DT_RC_GOOD;

  *submitted = 0;

  DT_Q(dthws_fgw_vballoc,(ctl,			/* virtual buffer for our data 		*/
    xe, ye, &handle));

  openrc = fg_vbopen(handle);			/* make us the active virtual buffer */
  if (openrc != 0)
  {
	rc = dt_err(ctl, F,
      "fg_vbopen(%d) failed, rc %d",
      handle, openrc);
	goto cleanup;
  }

  hdc = GetDC(priv_win->hwnd);			/* get temporary dc					*/
  if (hdc == (HDC)NULL)
    return dt_err(ctl, F,
      "GetDC failed");

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "got temporary DC 0x%08lx",
    (long)hdc);

  fg_setdc(hdc);

  fg_vbcolors();						/* set vb's colors to pallette's    */

  row = 0;
  while (row<ye)
  {
	void *data;
	unsigned int size, nrows;
	DT_Q(dtmem_map,(ctl,				/* map next chunk					*/
      hws_win->memalloc, offset,
      &data, &size));
	nrows = size / xe;
	fg_move(0, row+nrows-1);
	fg_putimage((char *)data, xe,		/* copy data into virtual buffer	*/
      nrows);
	row += nrows;
	DT_Q(dtmem_unmap,(ctl,				/* unmap the chunk					*/
      hws_win->memalloc, offset));
	offset += size;
  }

  printrc = fg_printer(STARTDOC);
  if (printrc != 0)
  {
	const char *m;
	if (printrc == -1)
	  m = "printer not defined in WIN.INI";
	else
	if (printrc == -2)
	  m = "printer offline or otherwise available";
	else
	if (printrc == -3)
	  m = "printer does not support the STRETCHDIB function";
	else
	if (printrc == -4)
	  m = "could not create the printer device context";
	else
	  m = "unknown error";
	rc = dt_err(ctl, F,
      DT_LABEL_NOPRINTER
      " fg_printer(STARTDOC) returned %d (%s)",
      printrc, m);
	goto cleanup;
  }

  pxe = 100.0 * (double)xe / ppm;		/* physical dimension 				*/
  pye = 100.0 * (double)ye / ppm;
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "ppm=%g, xe=%d, pxe=%d",
    ppm, xe, pxe); 
  
  fg_vbprint(							/* virtual buffer to active page 	*/
    0, xe-1, 0, ye-1,
    0, pxe, 0, pye,
    6);									/* 0.01 millimeters					*/

  *submitted = 1;

cleanup:
  if (printrc == 0)						/* print was started? 				*/
  {
	if (rc == DT_RC_GOOD)				/* print was successful? 			*/
	{
	  fg_printer(NEWFRAME);				/* finish the page 					*/
	  fg_printer(ENDDOC);
	}
	else
	  fg_printer(ABORTDOC);
  }

  if (hdc != (HDC)NULL)					/* release temporary DC 			*/
	ReleaseDC(priv_win->hwnd, hdc);
	
  if (openrc == 0)
    fg_vbclose();

  if (handle >= 0 && 
	  handle <= 31)
  {
    fg_vbfree(handle);
    dt_dbg(ctl, F, DT_DBG_MASK_WIN,
	  "fg_vbfree(%d)",
      handle);
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_cursor(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int *x,
  int *y,
  int op)
{
#ifdef NONO
  DT_F("dthws_fgw_win_cursor");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
#endif
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_win_move(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int x,
  int y)
{
#ifdef NONO
  DT_F("dthws_fgw_win_move");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
#endif
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_event_paint(				/* handle events for window			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_fgw_win_event_paint");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  if (priv_win->hpal != (HDC)NULL)
  {
	HWND hwnd = priv_win->hwnd;
	HDC hdc;
	PAINTSTRUCT ps;
	dt_rc_e rc;
	hdc = BeginPaint(hwnd, &ps);

	if (DT_DBG(ctl, DT_DBG_MASK_ASYNC) &&
		DT_DBG(ctl, DT_DBG_MASK_PIXEL))
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "\%s\" got WM_PAINT"
        " priv_win->hdc=0x%08lx"
        " BeginPaint=0x%08lx",
        priv_win->name,
        (long)priv_win->hdc,
        (long)hdc);

	DT_G(dthws_fgw_update,(ctl,			/* write window's data				*/
      priv_win->hdc,
      priv_win->hpal,					/* pallette everyone's using	    */
      hws_win->memalloc,				/* data from backing store 			*/
      0, 0,								/* just update the whole thing	    */
      hws_win->xe,
      hws_win->ye));

	EndPaint(hwnd, &ps);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_event_key(				/* handle events for window			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  MSG *msg,
  dt_event_t *event)
{
  DT_F("dthws_fgw_event_key");
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  const char *msg_text;
  event->keys[0] = '\0';

  if (msg->message == WM_KEYDOWN)		/* raw keydown? 					*/
  {
    int vk = msg->wParam;
	msg_text = "WM_KEYDOWN";
    switch(vk)
    {
      case VK_LEFT:      event->keys[0] = DT_KEY_LEFTARROW;  break;
      case VK_RIGHT:     event->keys[0] = DT_KEY_RIGHTARROW; break;
      case VK_UP:        event->keys[0] = DT_KEY_UPARROW;    break;
      case VK_DOWN:      event->keys[0] = DT_KEY_DOWNARROW;  break;
      case VK_ESCAPE:    event->keys[0] = DT_KEY_ESCAPE;     break;
      case VK_BACK:      event->keys[0] = 0x08;              break;
      case VK_TAB:       event->keys[0] = 0x09;              break;
      case VK_RETURN:    event->keys[0] = '\n';              break;
  	  default:
		TranslateMessage(msg);			/* translate key to ASCII 			*/
    }
  }
  else
  if (msg->message == WM_CHAR)			/* translated char? 				*/
  {
	msg_text = "WM_CHAR";
	event->keys[0] = msg->wParam;;
  }
  else									/* should never happen 				*/
	return DT_RC_GOOD;

  if (event->keys[0] != '\0')			/* usable key? 						*/
  {
	event->type = DT_EVENT_KEYBOARD;
	event->nkeys = 1;
	{
	  char s[64];
	  dtstr_printchar(ctl, event->keys[0], s, sizeof(s));
	  dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC,        
        "window \"%s\" saw %s 0x%02x maps to \"%s\"",
        priv_win->name, msg_text,
        msg->wParam, s);
	}
  }
  else
  {
	dt_dbg(ctl, F,
      DT_DBG_MASK_ASYNC,        
      "window \"%s\" saw %s 0x%02x ignored",
      priv_win->name, msg_text,
      msg->wParam);
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

long WINAPI _export
dthws_fgw_wndproc(
  HWND hwnd,
  UINT message,
  UINT wparam,
  LONG lparam)
{
  DT_F("dthws_fgw_wndproc");
  long procrc = 0;

  dt_ctl_t *ctl;
  dthws_win_t *hws_win;
  dthws_t *hws;
  priv_t *priv;
  priv_win_t *priv_win;
  long mask = DT_DBG_MASK_ASYNC;

  ctl = (dt_ctl_t *)
    GetWindowLong(hwnd, LONG_CTL);
  hws = (dthws_t *)
    GetWindowLong(hwnd, LONG_HWS);
  priv = (priv_t *)
    (hws == NULL? NULL: hws->priv);
  hws_win = (dthws_win_t *)
    GetWindowLong(hwnd, LONG_HWS_WIN);
  priv_win = (priv_win_t *)
    (hws_win == NULL? NULL: hws_win->priv);

  if (message == WM_PAINT ||
	  message == WM_MOUSEMOVE)
	mask = DT_DBG_MASK_PIXEL;

  {
	const char *s;
	dtmsw_message_string(ctl,			/* look up message string 			*/
      message, &s);
	if (ctl != NULL)
	  dt_dbg(ctl, F, mask,
        "0x%04x, 0x%04x (%s), 0x%04x, 0x%08lx",
	    hwnd,
        message, s, wparam,
        lparam);
	else
	{
#   if 0
	  FILE *file;
	  file = fopen("wndproc.out", "a");	/* open scratch file 				*/
	  if (file != NULL)
	  {
	    fprintf(file,					/* write pid and window name 		*/
          "0x%04x, 0x%04x (%s), 0x%04x, 0x%08lx\n",
	      hwnd,
          message, s, wparam,
          lparam);
	    fclose(file);
	  }
#   endif
	}
  }

  if (hws && hws_win)					/* window has valid user settings?	*/
  switch(message)
  {
    case WM_PAINT:
	  DT_Q(dthws_fgw_event_paint,(ctl,
        hws, hws_win));
	break;
    case WM_SETFOCUS:
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_ASYNC |
        DT_DBG_MASK_LUT,
        "SETFOCUS realizing pallette"
        " 0x%08lx on dc 0x%08lx",
        (long)priv_win->hpal, 
        (long)priv_win->hdc);
      fg_setdc(priv_win->hdc);
      fg_realize(priv_win->hpal);
	break;
    case WM_PALETTECHANGED:
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_ASYNC |
        DT_DBG_MASK_LUT,
        "PALLETECHANGED realizing pallette"
        " 0x%08lx on dc 0x%08lx",
        (long)priv_win->hpal, 
        (long)priv_win->hdc);
      fg_setdc(priv_win->hdc);
      fg_realize(priv_win->hpal);
	break;
    case WM_DESTROY:
#     ifdef DTCONFIG_IMPLIED_MSW16
	    PostAppMessage(GetCurrentTask(), 
#     else
	    PostThreadMessage(GetCurrentThreadId(), 
#     endif
        DTMSW_MESSAGE_DESTROY, 
        (WORD)hwnd, lparam);
	  priv_win->destroyed = 1;
	break;
  }

  procrc = DefWindowProc(hwnd,
    message, wparam, lparam);
  
  return procrc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_handle_event(					/* handle event buffer				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  void *buffer,
  int *handled,
  dt_event_t *event)
{
  DT_F("dthws_fgw_handle_event");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  MSG msg = *(MSG *)buffer;
  priv_t *priv = (priv_t *)hws->priv;
  dthws_win_t *hws_win;
  dtlist_elem_t *elem;
  priv_win_t *priv_win;
  long first_id = 0;
  const char *msg_text;
  int pending;
  int quit = 0;
  long mask = DT_DBG_MASK_ASYNC;

  if (event == NULL)
	return DT_RC_GOOD;

  DT_MEMSET(event, 0, sizeof(*event));		/* clear output event structure	    */
  
  event->type = DT_EVENT_NOTHING;		/* in case nothing happens 			*/

  if (msg.message ==					/* a window was destroyed? 			*/
	  DTMSW_MESSAGE_DESTROY)
  {
	msg.hwnd = msg.wParam;
	msg.wParam = 0;
  }

  if (msw != NULL &&					/* this is a control event?		    */
	  (long)msg.hwnd == msw->pidwin)
  {
	DT_Q(dtmsw_yield,(ctl));			/* pick off the control event		*/
	*handled = 1;
	return DT_RC_GOOD;
  }

  elem = DTLIST_ELEM_HEAD;
  while (elem)
  {
	DT_Q(dtlist_next,(ctl,				/* fetch next window in list	    */
      &hws->windowlist, elem,
      &elem));
	if (elem != NULL)					/* not the last in the list?	    */
	{
	  hws_win = (dthws_win_t *)elem->user;
	  if (first_id == 0)
		first_id = hws_win->id;
	  if (hws_win->id == (long)msg.hwnd)
		break;
    }
  }

  dtmsw_message_string(ctl,				/* look up message string 			*/
    msg.message, &msg_text);

  if (msg.message == WM_PAINT ||
	  msg.message == WM_MOUSEMOVE)
	mask = DT_DBG_MASK_PIXEL;

  if (elem == NULL)						/* didn't find the window?			*/
  {
	dt_dbg(ctl, F, mask,
      "0x%04lx, 0x%04x (%s), not our window (0x%04lx)",
	  (long)msg.hwnd, 
      msg.message, msg_text, 
      first_id);
	*handled = 0;
	return DT_RC_GOOD;
  }

  priv_win = (priv_win_t *)				/* our private info on the window 	*/
    hws_win->priv;

  dt_dbg(ctl, F, mask,
    "0x%04x, 0x%04x (%s), 0x%04x, 0x%08lx",
	priv_win->hwnd,
    msg.message, msg_text, msg.wParam,
    msg.lParam);
  

  *handled = 1;

  event->id = (long)msg.hwnd;			/* remember where event happened    */

  switch (msg.message)
  {
										/* ................................ */
	case WM_PAINT:
	  DT_Q(dthws_fgw_event_paint,(ctl,
        hws, hws_win));
	break;
										/* ................................ */
	case WM_SIZE:
	if (priv_win != NULL)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "\%s\" got WM_SIZE %d %d",
        priv_win->name, 
        LOWORD(msg.lParam), 
        HIWORD(msg.lParam));
	}
	break;
										/* ................................ */
	case WM_SETFOCUS:
	if (priv != NULL &&
		priv_win->hpal != (HDC)NULL)
	{
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_ASYNC |
        DT_DBG_MASK_LUT,
        "SETFOCUS realizing pallette"
        " 0x%08lx on dc 0x%08lx",
        (long)priv_win->hpal, 
        (long)priv_win->hdc);
      fg_setdc(priv_win->hdc);
      fg_realize(priv_win->hpal);
	  /*      InvalidateRect(priv_win->hwnd, NULL, TRUE); */
	}
	break;
										/* ................................ */
    case DTMSW_MESSAGE_DESTROY:
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "\"%s\" got DTMSW_MESSAGE_DESTROY",
        priv_win->name);
	  event->type = DT_EVENT_DESTROYED;	/* tell upper layers about this		*/
	  event->id = priv_win->hwnd;		/* tell who got destroyed.ne			*/
	break;
										/* ................................ */
    case WM_QUIT:
  	    return dt_err(ctl, F,
          "got WM_QUIT on window 0x%08lx",
          (long)priv_win->hwnd);
										/* ................................ */
    case WM_KEYDOWN:
    case WM_CHAR:
	  DT_Q(dthws_fgw_event_key,(ctl,
        hws, hws_win, &msg, event));
    break;
										/* ................................ */
    case WM_KEYUP:
      dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC,        
        "window \"%s\" saw KEYUP VK 0x%02x",
        priv_win->name,
        msg.wParam);
	  switch(msg.wParam)
	  {
		case VK_CONTROL:   priv_win->control_key = 0; break;
		case VK_SHIFT:     priv_win->shift_key = 0; break;
	  }
	break;
										/* ................................ */
  }
	
  DefWindowProc(msg.hwnd,				/* tell Windows about the event		*/
    msg.message, msg.wParam,
    msg.lParam);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_fgw_wait_event(					/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dthws_fgw_wait_event");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  dthws_win_t *hws_win;
  priv_win_t *priv_win;
  int pending;
  int quit = 0;
  MSG msg;
  int handled;

  if (event == NULL)
	return DT_RC_GOOD;

  do {
	DT_MEMSET(event, 0, sizeof(*event));	/* clear output event structure	    */
  
	event->type = DT_EVENT_NOTHING;		/* in case nothing happens 			*/
	
	if (svc != NULL)					/* we are using dt svc?		    	*/
	{
	  int svcpending;
	  DT_Q(svc->issvc,(ctl,				/* see if we have any svc input	    */
        svc, 0, NULL, &svcpending));
	  if (svcpending)					/* an svc connection is waiting? 	*/
	  {			
		event->type = DT_EVENT_IPC;
		dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "svc input found at poll");
		break;							/* let caller handle it immediately */
	  }
	}

    pending = PeekMessage(&msg, NULL,	/* check event queue				*/
      0, 0, PM_NOREMOVE);

	if (pending && 
		msg.message ==					/* a window was destroyed? 			*/
		DTMSW_MESSAGE_DESTROY)
	  msg.hwnd = msg.wParam;

    if (pending && msw != NULL &&		/* this is a control event?		    */
		(long)msg.hwnd == msw->pidwin)
	{
	  DT_Q(dtmsw_yield,(ctl));			/* pick off the control event		*/
	  continue;
	}

	if (pending)						/* we have a pending display event?	*/
	{
	  GetMessage(&msg, (HWND)NULL,		/* grab the pending event 			*/
        0, 0);
	  if (msg.message ==				/* a window was destroyed? 			*/
		  DTMSW_MESSAGE_DESTROY)
		msg.hwnd = msg.wParam;
	}
										/* ................................ */
	else								/* no events pending?				*/
	if (timeout == -1)					/* this is only a poll?		    	*/
	{
      break;							/* out of while(!quit) loop			*/
	}
										/* ................................ */
	else								/* not a poll and no events? 		*/
	{						
	  /* WaitMessage();					/* this means we really wait 		*/
#     ifdef DTCONFIG_IMPLIED_WIN32
	    DT_Q(dtos_delay,(ctl, 200));	/* sleep a short time on Win32		*/
#     endif
#     ifdef DTCONFIG_IMPLIED_WIN16
	    DT_Q(dtmsw_yield,(ctl));		/* just yield on Win16				*/
#     endif
	  continue;
	}

	DT_Q(dthws_fgw_handle_event,(ctl,
      hws,
      (void *)&msg, &handled, event));

    if (!handled)						/* didn't find the window?			*/
	{
	  const char *s;
	  dtmsw_message_string(ctl,			/* look up message string 			*/
        msg.message, &s);
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "%d=0x%04x=%s"
        " on non-hws win 0x%08lx",
        msg.message, 
        msg.message, 
        s,
        (long)msg.hwnd);
      break;							/* quit for now 					*/
	}

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
          !PeekMessage(&msg,			/* event queue has nothing else?	*/
            (HWND)NULL, 0, 0,    
            PM_NOREMOVE))
        quit= 1;
	}
	if (flag == 0 &&					/* just polling? 					*/
		!PeekMessage(&msg,				/* event queue has nothing else?	*/
          (HWND)NULL, 0, 0,    
          PM_NOREMOVE))
      quit= 1;
  } while (!quit);

  return DT_RC_GOOD;
}

#endif


/*..........................................................................
| NAME
| Introduction - intro
|
| DESCRIPTION
| DOESN'T WORK RIGHT FOR 256-COLOR DISPLAY.
| INACTIVE WINDOWS NOT UPDATED PROPERLY.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_fgw_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *server)
{
  DT_F("dthws_fgw_connect");
#if DTPRJ_FGW == DT_1
  priv_t *priv = NULL;
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  WNDCLASS c;
  ATOM atom;
  dt_rc_e rc;

  if (msw == NULL)
    return dt_err(ctl, F,
      "msw structure is NULL"
      " (program not initialized correctly)");

  DT_C(dtos_malloc2,(ctl, 
    (void **)&priv,
    sizeof(*priv),
    F, "priv"));

  DT_MEMSET(priv, 0, sizeof(*priv));

  priv->classname = "dtfgw_class";

  DT_MEMSET(&c, 0, sizeof(c));
  c.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  c.lpfnWndProc = dthws_fgw_wndproc;
  c.cbWndExtra = LONG_EXTRA;
  c.hInstance =							/* make sure class exists 			*/
    (HINSTANCE)msw->hinstance;
  c.hCursor = LoadCursor(NULL,			/* explicit cursor for this class	*/
    IDC_ARROW);
  c.hIcon = LoadIcon(
    (HINSTANCE)msw->hinstance,
    "dthws_icon");
  if (c.hIcon == (HICON)NULL)
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,  
      "no icon named \"%s\" found",
      "dthws_icon");
  c.hbrBackground = (HBRUSH)
    GetStockObject(WHITE_BRUSH);
  c.lpszClassName = priv->classname;

  atom = RegisterClass(&c);
  if (atom == 0)
    return dt_err(ctl, F,
      "RegisterClass(\"%s\") failed"
#     ifdef DTCONFIG_IMPLIED_MSW32
        " (GetLastError=%ld)"
#     endif
      ,
      c.lpszClassName
#     ifdef DTCONFIG_IMPLIED_MSW32
        , GetLastError()
#     endif
    );

  fg_vbinit();							/* start virtual buffers 			*/

  hws->priv = (void *)priv;
  hws->xe =								/* return display size to caller    */
	GetSystemMetrics(SM_CXSCREEN);
  hws->ye =	
	GetSystemMetrics(SM_CYSCREEN);
  hws->do_remap = 0;					/* we don't remap ourselves 		*/
  hws->do_ymirror = 1;					/* data in backing store mirrored 	*/

  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "screen is %dx%d pixels",
     hws->xe, hws->ye);

  hws->disconnect = dthws_fgw_disconnect;
  hws->win_create = dthws_fgw_win_create;
  hws->wait_event = dthws_fgw_wait_event;
  hws->handle_event = dthws_fgw_handle_event;
  hws->win_destroy = dthws_fgw_win_destroy;
  hws->export_data = dthws_fgw_export_data;
  hws->import_data = dthws_fgw_import_data;
  hws->export_color = dthws_fgw_export_color;
  hws->import_color = dthws_fgw_import_color;
  hws->win_move = dthws_fgw_win_move;
  hws->win_clear = dthws_fgw_win_clear;
  hws->win_raise = dthws_fgw_win_raise;
  hws->win_lower = dthws_fgw_win_lower;
  hws->win_print = dthws_fgw_win_print;
  hws->win_cursor = dthws_fgw_win_cursor;

cleanup:
  if (rc != DT_RC_GOOD &&
	  priv != NULL)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  return rc;

# else
  return dt_err_notlinked(ctl, F, "Fastgraph for windows");
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
