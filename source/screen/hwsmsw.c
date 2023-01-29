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
  20000506 DE added mouse click event, invalidate rect on export_data
 *..........................................................................*/



#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: hwsmsw.c,v $ $Revision: 1.1 $");
#include <dtack/label.h>
#include <dtack/errno.h>

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/hws.h>
#include <dtack/str.h>

#include <dtack/msw.h>

#if 0
#define STRETCH_WIN
#else
#endif

extern
dt_rc_e 
DTCONFIG_API1
dtmsw_message_string(					/* convert message to string		*/
  dt_ctl_t *ctl,
  int message,
  const char **string);

#ifdef DTCONFIG_IMPLIED_MSW
#ifndef DTDEPEND
#include <commdlg.h>
#endif

extern dthws_connect_f dthws_connect;
static dthws_disconnect_f dthws_msw_disconnect;
static dthws_win_create_f dthws_msw_win_create;
static dthws_wait_event_f dthws_msw_wait_event;
static dthws_handle_event_f dthws_msw_handle_event;
static dthws_win_destroy_f dthws_msw_win_destroy;
static dthws_export_data_f dthws_msw_export_data;
static dthws_import_data_f dthws_msw_import_data;
static dthws_export_color_f dthws_msw_export_color;
static dthws_import_color_f dthws_msw_import_color;
static dthws_win_clear_f dthws_msw_win_clear;
static dthws_win_raise_f dthws_msw_win_raise;
static dthws_win_lower_f dthws_msw_win_lower;
static dthws_win_lower_f dthws_msw_win_lower;
static dthws_win_cursor_f dthws_msw_win_cursor;

#include <dtack/mem.h>

typedef struct {
  HWND hwnd;
  HDC hdc;
  HPALETTE hpal;						/* each window has its own pallette */
  PALETTEENTRY pallette[256];
  int inverted;
  int destroyed;						/* destroyed by window manager 		*/
  int control_key;
  int shift_key;
  int shown;
  char name[DT_WINDOWNAME_MAXLEN+1];
} priv_win_t;

typedef struct {
  const char *classname;
  HPEN hpen;
  HBRUSH hbrush;
} priv_t;

#define LONG_CTL (0)					/* SetWindowLong indexes 			*/
#define LONG_HWS (4)
#define LONG_HWS_WIN (8)
#define LONG_EXTRA (12)					/* amount of extra data 			*/
										/* ................................ */
typedef struct {		
	unsigned long     biSize;
	unsigned long     biWidth;
	unsigned long     biHeight;
	unsigned short    biPlanes;
	unsigned short    biBitCount;
	unsigned long     biCompression;
	unsigned long     biSizeImage;
	unsigned long     biXPelsPerMeter;
	unsigned long     biYPelsPerMeter;
	unsigned long     biClrUsed;
	unsigned long     biClrImportant;
} dtdib_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_disconnect(
  dt_ctl_t *ctl,
  dthws_t *hws)
{
  DT_F("dthws_msw_disconnect");
  priv_t *priv =
    (priv_t *)hws->priv;

  if (priv != (priv_t *)NULL)
  {
	if (priv->hbrush != (HBRUSH)NULL)
	  DeleteObject(priv->hbrush);
	if (priv->hpen != (HPEN)NULL)
	  DeleteObject(priv->hpen);
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "disconnecting");

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
dthws_msw_win_create(					/* make/display a new window		*/
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
  DT_F("dthws_msw_win_create");
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

  dt_dbg(ctl, F, 
	DT_DBG_MASK_WIN,
	"CreateWindow(class=\"%s\","
	" name=\"%s\") gave hwnd=0x%08lx at [%d,%d] size [%d,%d]",
	priv->classname,
	priv_win->name,
	hws_win->id, x0, y0, xe, ye);

  priv_win->hdc = GetDC(				/* get and keep the window's dc	    */
    priv_win->hwnd);
  if (priv_win->hdc == (HDC)NULL)
    return dt_err(ctl, F,
      "GetDC failed");

# if !defined(STRETCH_WIN)
  SelectObject(priv_win->hdc,			/* for painting dead area 			*/
    GetStockObject(WHITE_BRUSH));

  SelectObject(priv_win->hdc, priv->hpen);
  SelectObject(priv_win->hdc, priv->hbrush);
#endif

  {
    LOGPALETTE pallette;
	BOOL boolrc;

	pallette.palVersion = 0x300;
	pallette.palNumEntries = 1;
    priv_win->hpal = CreatePalette(
      &pallette);
    if (priv_win->hpal == (HPALETTE)NULL)
	  return dt_err(ctl, F,
        "CreatePalette failed");
	dt_dbg(ctl, F, 
      DT_DBG_MASK_WIN|DT_DBG_MASK_LUT,
      "created pallette 0x%08lx",
      (long)priv_win->hpal);
	boolrc = ResizePalette(priv_win->hpal,
      256);
	if (boolrc == FALSE)
	  return dt_err(ctl, F,
        "ResizePalette failed");
  }

  {
	int i;
	static nth = 0;
	for (i=0; i<256; i++)				/* initialize the pallette 			*/
	{
      priv_win->pallette[i].peRed = nth? i: 0;
	  priv_win->pallette[i].peGreen = nth? 0: i;
	  priv_win->pallette[i].peBlue = 0;
	  priv_win->pallette[i].peFlags = 0;
	}
	nth++;
  }

  SetPaletteEntries(priv_win->hpal,		/* set pallette entries				*/
    0, 256, priv_win->pallette);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_stretch(						/* stretch window's data			*/
  dt_ctl_t *ctl,
  HDC hdc,
  HPALETTE hpal,
  PALETTEENTRY *pallette,
  void *memalloc,
  int x0,
  int y0,
  int xe,
  int ye,
  int outx0,
  int outy0,
  int outxe,
  int outye)
{
  DT_F("dthws_msw_update");
  struct {
    dtdib_t dib;
    RGBQUAD rgb[256];
  } header;
  int i;
  long offset = (long)y0 * (long)xe;
  long max = (long)xe * (long)ye;
  double scale;
  int row;
  WORD winrc;
  void *p = NULL;
  int pxe = xe + ((4-((xe)&3))&3);		/* padded size of dib 				*/

  dt_rc_e rc = DT_RC_GOOD;

  header.dib.biSize =          sizeof(header.dib);
  header.dib.biWidth =         pxe;
  header.dib.biHeight =        ye;
  header.dib.biPlanes =        1;
  header.dib.biBitCount =      8;
  header.dib.biCompression =   0;
  header.dib.biSizeImage =     0;
  header.dib.biXPelsPerMeter = 0;
  header.dib.biYPelsPerMeter = 0;
  header.dib.biClrUsed =       0;
  header.dib.biClrImportant =  0;

  SelectPalette(hdc, hpal, FALSE);		/* select pallette into dc			*/
  RealizePalette(hdc);					/* make pallette current			*/

  for (i=0; i<256; i++)					/* transfer pallette to dib 		*/
  {
	header.rgb[i].rgbRed = pallette[i].peRed;
	header.rgb[i].rgbGreen = pallette[i].peGreen;
	header.rgb[i].rgbBlue = pallette[i].peBlue;
  }

  scale = (double)outye / (double)ye;
  row = 0;
  while (row<ye)						/* until end of last chunk 			*/
  {
	void *data;
	unsigned int size, nrows, outn;
	DT_C(dtmem_map,(ctl,				/* map next chunk					*/
      memalloc, offset,
      &data, &size));
	nrows = size / xe;

	if (pxe == xe)						/* don't need to pad? 				*/
	  p = data;
	else
	{
	  unsigned int psize =				/* size of the pad buffer 			*/
        (unsigned int)pxe * nrows;
	  char *d1, *p1;
      if (p == NULL)					/* pad buffer not allocated yet? 	*/
	  {
	    DT_C(dtos_malloc2,(ctl,			/* allocate pad buffer 				*/
          &p, psize, F, "pad"));
	    DT_MEMSET(p, psize*nrows, 0);		/* clear pad buffer 				*/
	  }
	  d1 = (char *)data;
	  p1 = (char *)p;
	  for (i=0; i<nrows; i++)			/* copy rows into padded area 		*/
	  {
	    DT_MEMCPY(p1, d1, xe);
		p1 += pxe;
		d1 += xe;
	  }
	}
	
    header.dib.biHeight = nrows;		/* set dib height to chunk height 	*/

	outn = (unsigned int)(0.5 +			/* number of output rows 			*/
      (double)nrows * scale);

	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "[%d,%d] [%d,%d] [%d,%d] [%d,%d]",
      outx0, outy0, outxe, outn,
      0, 0, xe, nrows);

	if (scale == 1.0)					/* no stretching? 					*/
	{
	  winrc = SetDIBitsToDevice(hdc,	/* paint from dib to dc 			*/
        x0, y0+row,						/* dest x, dest y 					*/
        xe, nrows,						/* width, height of the dib			*/
        0, 0,							/* source x, source y in dib		*/
        0, nrows,						/* start scan, num scans 			*/
        p,
        (BITMAPINFO *)&header,
        DIB_RGB_COLORS);
      if (winrc == 0)
      {
	    rc = dt_err(ctl, F,
          "SetDIBitsToDevice() returned 0");
		goto cleanup;
      }
	}
	else								/* need to stretch? 				*/
	{
      winrc = StretchDIBits(hdc,		/* paint/stretch dib to dc 			*/
        outx0, outy0, outxe, outn,
        0, 0, xe, nrows,
        p,
        (BITMAPINFO *)&header,
        DIB_RGB_COLORS,
        SRCCOPY);
      if (winrc == 0)
      {
	    rc = dt_err(ctl, F,
          "StrecthDIBits() returned 0");
		goto cleanup;
      }
	}

	DT_C(dtmem_unmap,(ctl,				/* unmap the chunk					*/
      memalloc, offset));
	offset += size;
	row += nrows;
    outy0 += outn;
  }

cleanup:
  if (pxe != xe && p != NULL)			/* we allocated a pad buffer? 		*/
	DT_I(dtos_free2,(ctl, p,			/* free the pad buffer 				*/
      F, "pad"));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_update(						/* write window's data				*/
  dt_ctl_t *ctl,
  HWND hwnd,
  HDC hdc,
  HPALETTE hpal,
  PALETTEENTRY *pallette,
  void *memalloc,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_msw_update");
  RECT rect;
  GetClientRect(hwnd, &rect);			/* size of inside of window 		*/

#if defined(STRETCH_WIN)
  
  DT_Q(dthws_msw_stretch,(ctl,			/* stretch image to fit window 		*/
    hdc, hpal, pallette,
    memalloc,
    x0, y0, xe, ye,
    0, 0,
    rect.right, rect.bottom));
  
#else  									/* not supposed to stretch? 		*/
  
  DT_Q(dthws_msw_stretch,(ctl,			/* draw without stretching			*/
    hdc, hpal, pallette,
    memalloc,
    x0, y0, xe, ye,
    0, 0, xe, ye));
  if (xe < rect.right)					/* dead area to right? 				*/
	Rectangle(hdc,						/* paint dead area 					*/
      xe, 0, 
      rect.right+1, rect.bottom+1);
  if (ye < rect.bottom)					/* dead area below image? 			*/
	Rectangle(hdc,						/* paint dead area 					*/
      0, ye, xe+1, rect.bottom+1);
	  
#endif
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_win_destroy(					/* get rid of window			    */
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_msw_win_destroy");
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "destroying \"%s\" priv_win->destroyed=%d",
    priv_win->name, priv_win->destroyed);

  if (priv_win->hwnd != (HWND)NULL)
  {
	if (priv_win->hpal != (HPALETTE)NULL)
  	  DeleteObject(priv_win->hpal);		/* get rid of pallette 				*/
	if (priv_win->hdc != (HDC)NULL)
      ReleaseDC(priv_win->hwnd, 
        priv_win->hdc);

    SetWindowLong(priv_win->hwnd, 
      LONG_HWS, 
      (long)NULL);
	SetWindowLong(priv_win->hwnd, 
      LONG_HWS_WIN,
      (long)NULL);

	if (!priv_win->destroyed)			/* windows doesn't know about it? 	*/
	  DestroyWindow(priv_win->hwnd);
  }

  DT_I(dtos_free2,(ctl, priv_win,
    F, "priv win"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_export_color(					/* write window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut8,
  int start,
  int count)
{
  DT_F("dthws_msw_export_color");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win =
    (priv_win_t *)(hws_win? hws_win->priv: NULL);
  unsigned char *tmplut;
  int i;

  if (hws_win == NULL)					/* color setting background lut? 	*/
    return DT_RC_GOOD;					/* don't do it 						*/

  for (i=start; i<start+count; i++)		/* save lut values 					*/
  {
	priv_win->pallette[i].peRed = lut8[i].r;
	priv_win->pallette[i].peGreen = lut8[i].g;
	priv_win->pallette[i].peBlue = lut8[i].b;
  }

  SetPaletteEntries(priv_win->hpal,		/* set pallette entries				*/
    0, 256, priv_win->pallette);

  InvalidateRect(priv_win->hwnd, NULL,	/* force pixels to redraw		    */
    FALSE); 

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_export_data(					/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_msw_export_data");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  RECT rect;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, "ctl=0x%08lx ctl->msw=0x%08lx", ctl, ctl->msw);

  if (!priv_win->shown)					/* window not up yet? 				*/
  {
    ShowWindow(priv_win->hwnd,			/* pop up the window				*/
      SW_NORMAL);
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "ShowWindow(\"%s\" hwnd=0x%04x) has been called", 
      priv_win->name,
      priv_win->hwnd);
	
	priv_win->shown = 1;
  }

  rect.left = x0;
  rect.top = y0;
  rect.right = x0 + xe - 1;
  rect.bottom = y0 + ye - 1;
  InvalidateRect(priv_win->hwnd,
    &rect, FALSE);

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "InvalidateRect(\"%s\" hwnd=0x%04x) has been called"
    " [%d,%d] [%d,%d]", 
    priv_win->name,
    priv_win->hwnd,
    rect.left, rect.top,
    rect.right, rect.bottom);

  UpdateWindow(priv_win->hwnd);		/* send (not post) paint message    */

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "UpdateWindow(\"%s\" hwnd=0x%04x) has been called",
    priv_win->name,
    priv_win->hwnd);

#if 0
  DT_Q(dthws_msw_update,(ctl,
    priv_win->hwnd, 
    priv_win->hdc, 
    priv_win->hpal,
    priv_win->pallette,					/* our private pallette 			*/
    hws_win->memalloc,					/* data from backing store 			*/
    x0, y0, xe, ye));
#endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_import_data(					/* get window's data				*/
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
  DT_F("dthws_msw_import_data");
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
dthws_msw_import_color(					/* get window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  dtlut8_t *lut,
  int start,
  int count)
{
#ifdef NONO
  DT_F("dthws_msw_import_color");
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
dthws_msw_win_clear(						/* clear window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int background)
{
#ifdef NONO
  DT_F("dthws_msw_win_clear");
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
dthws_msw_win_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
#ifdef NONO
  DT_F("dthws_msw_win_raise");
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
dthws_msw_win_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
#ifdef NONO
  DT_F("dthws_msw_win_lower");
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
dthws_msw_win_print(					/* print window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)
{
  DT_F("dthws_msw_win_print");
  priv_t *priv =
    hws->priv;
  priv_win_t *priv_win =
    hws_win->priv;
  
  PRINTDLG pd;
  DEVNAMES *devnames = NULL;
  const char *driver;
  const char *device;
  const char *output;
  BOOL boolrc;
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;
  HDC prndc = NULL;
  int caps;
  DOCINFO di;
  BOOL intrc;
  int doc_started = 0;
  int page_started = 0;
  int logpixelsx, logpixelsy;
  int horzres, vertres;
  double dpm;
  int pxe, pye;

  dt_rc_e rc = DT_RC_GOOD;

  *submitted = 0;

  DT_MEMSET(&pd, 0, sizeof(pd));
  pd.lStructSize = sizeof(pd);
  if (printer == (char *)NULL ||		/* caller wants default printer? 	*/
	  !strcmp(printer, "default"))
	pd.Flags = PD_RETURNDEFAULT;
  pd.hwndOwner = (HWND)msw->pidwin;

  boolrc = PrintDlg(&pd);
  if (boolrc == FALSE)
  {
	DWORD dlgrc;
	dlgrc = CommDlgExtendedError();
	if (dlgrc == 0)						/* user hit cancel? 				*/
	  dt_dbg(ctl, F, 
        DT_DBG_MASK_COMMAND,
        "user canceled print");
	else
	  rc = dt_err(ctl, F,
        "CommDlgExtendedError() = %ld (0x%08lx)",
        dlgrc, dlgrc);
	goto cleanup;
  }

  devnames = (DEVNAMES *)GlobalLock(pd.hDevNames);
  driver = (const char *)devnames + devnames->wDriverOffset;
  device = (const char *)devnames + devnames->wDeviceOffset;
  output = (const char *)devnames + devnames->wOutputOffset;
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "driver=\"%s\"", driver);
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "device=\"%s\"", device);
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "output=\"%s\"", output);

  prndc = CreateDC(driver, device, output, 0L);
  if (prndc == NULL)
  {
	rc = dt_err(ctl, F,
      "CreateDC(\"%s\", \"%s\", \"%s\", 0L) failed",
      driver, device, output);
	goto cleanup;
  }

  caps = GetDeviceCaps(prndc, RASTERCAPS);
  if (!(caps & RC_BITBLT))
  {
	rc = dt_err(ctl, F,
      "GetDeviceCaps(prndc, RASTERCAPS) = 0x%04x, RC_BITBLT bit 0x%04x not set",
      caps, RC_BITBLT);
	goto cleanup;
  }

#if defined(DTCONFIG_IMPLIED_MSW32)		/* win32, has extended error info?	*/
#define PRINTRC(D)						\
  {										\
	char s[256];						\
    DWORD e = GetLastError();			\
    DTERRNO_LASTS(e, s, sizeof(s));		\
	rc = dt_err(ctl, F,					\
      "%s failed"						\
      DTERRNO_LASTFS,					\
      D,								\
      DTERRNO_LASTES(e, s));			\
    goto cleanup;						\
  }										\
  else									\
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	\
      "%s successful", D);
#else  									/* no win32 extended error info?	*/
#define PRINTRC(D)						\
  {										\
	char s[256];						\
	rc = dt_err(ctl, F,					\
      "%s failed", D);					\
    goto cleanup;						\
  }										\
  else									\
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	\
      "%s successful", D);
#endif

#define INTRC(D, R)						\
  if (R <= 0)							\
    PRINTRC(D)
#define BOOLRC(D, B)					\
  if (B == FALSE)						\
    PRINTRC(D)

  DT_MEMSET(&di, 0, sizeof(di));
  di.cbSize = sizeof(di);
  di.lpszDocName = "image";
  intrc = StartDoc(prndc, &di);
  INTRC("StartDoc", intrc);
  doc_started = 1;
  intrc = StartPage(prndc);
  INTRC("StartPage", intrc);
  page_started = 1;

  logpixelsx = GetDeviceCaps(prndc,		/* pixels per logical inch 			*/
    LOGPIXELSX);
  logpixelsy = GetDeviceCaps(prndc,
	LOGPIXELSY);
  horzres = GetDeviceCaps(prndc,
	HORZRES);
  vertres = GetDeviceCaps(prndc,
	VERTRES);

  dpm = logpixelsx / 25.4;				/* printer dots per millimeter 		*/
  pxe = 0.5 + dpm / ppm * hws_win->xe;	/* image width in printer dots 		*/
  pye = 0.5 + dpm / ppm * hws_win->ye;	/* image height in printer dots 	*/

  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "LOGPIXELSX=%d, LOGPIXELSY=%d",
    logpixelsx, logpixelsy);
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "HORZRES=%d, VERTRES=%d",
    horzres, vertres);
  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
    "dpm=%g, pxe=%d, pye=%d",
    dpm, pxe, pye);

  DT_C(dthws_msw_stretch,(ctl,			/* stretch window's data			*/
    prndc,
    priv_win->hpal,
    priv_win->pallette,					/* our private pallette				*/
    hws_win->memalloc,					/* data from backing store 			*/
    0, 0,								/* just print the whole thing	    */
    hws_win->xe,
    hws_win->ye,
    (horzres-pxe)/2,					/* center the image on the page 	*/
    (vertres-pye)/2,
    pxe, pye));
										/* ................................ */
  *submitted = 1;

cleanup:
  if (page_started)
	EndPage(prndc);
  if (doc_started)
	EndDoc(prndc);

  if (prndc != NULL)
    DeleteDC(prndc);
  if (devnames != NULL)
	GlobalUnlock(pd.hDevNames);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dthws_msw_win_cursor(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int *x,
  int *y,
  int op)
{
#ifdef NONO
  DT_F("dthws_msw_win_cursor");
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
dthws_msw_win_move(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  int x,
  int y)
{
#ifdef NONO
  DT_F("dthws_msw_win_move");
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
dthws_msw_event_paint(					/* handle events for window			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win)
{
  DT_F("dthws_msw_win_event_paint");
  priv_t *priv =
    (priv_t *)hws->priv;
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  if (priv_win->hpal != (HPALETTE)NULL)	/* private pallette allocated? 		*/
  {
	HWND hwnd = priv_win->hwnd;
	HDC hdc;
	PAINTSTRUCT ps;
	dt_rc_e rc;
	hdc = BeginPaint(hwnd, &ps);

	if (DT_DBG(ctl, DT_DBG_MASK_ASYNC) &&
		DT_DBG(ctl, DT_DBG_MASK_PIXEL))
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "\"%s\" got WM_PAINT"
        " priv_win->hdc=0x%08lx"
        " BeginPaint=0x%08lx",
        priv_win->name,
        (long)priv_win->hdc,
        (long)hdc);

	DT_G(dthws_msw_update,(ctl,			/* write window's data				*/
      priv_win->hwnd,
      priv_win->hdc,
      priv_win->hpal,
      priv_win->pallette,				/* our private pallette				*/
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
dthws_msw_event_key(				/* handle events for window			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  MSG *msg,
  dt_event_t *event)
{
  DT_F("dthws_msw_event_key");
  priv_win_t *priv_win =
    (priv_win_t *)hws_win->priv;
  const char *msg_text;
  int clicked = 0;
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
  else
  if (msg->message == WM_LBUTTONDOWN ||	/* mouse button down?				*/
	  msg->message == WM_MBUTTONDOWN ||
	  msg->message == WM_RBUTTONDOWN)
  {
	msg_text = "WM_XBUTTONDOWN";
	event->type = DT_EVENT_MOUSEPRESS;
	event->mousex = LOWORD(msg->lParam);
	event->mousey = HIWORD(msg->lParam);
	event->left = msg->wParam & MK_LBUTTON;
	event->middle = msg->wParam & MK_MBUTTON;
	event->right = msg->wParam & MK_RBUTTON;
	DT_MEMSET(event->keys, 0,			/* clear any key value			    */
      sizeof(event->keys));
	clicked = 1;
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
  if (!clicked)
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
dthws_msw_wndproc(
  HWND hwnd,
  UINT message,
  UINT wparam,
  LONG lparam)
{
  DT_F("dthws_msw_wndproc");
  long procrc = 0;

  dt_ctl_t *ctl;
  dthws_win_t *hws_win;
  dthws_t *hws;
  priv_t *priv;
  priv_win_t *priv_win;

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

  if (message != WM_PAINT &&
	  message != WM_MOUSEMOVE)
  {
	const char *s;
	dtmsw_message_string(ctl,			/* look up message string 			*/
      message, &s);

	if (ctl != NULL)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "0x%04lx, 0x%04x (%s), 0x%04x, 0x%08lx priv_win=0x%08lx",
	    (long)hwnd,
        message, s, wparam,
        lparam, (long)priv_win);
	}
	else
	{
#   if 0
	  FILE *file;
	  file = fopen("wndproc.out", "a");	/* open scratch file 				*/
	  if (file != NULL)
	  {
	    fprintf(file,					/* write pid and window name 		*/
          "0x%04lx, 0x%04x (%s), 0x%04x, 0x%08lx priv_win=0x%08lx\n",
	      (long)hwnd,
          message, s, wparam,
          lparam, (long)priv_win);
	    fclose(file);
	  }
#   endif
	}
  }

  if (priv_win !=  (priv_win_t *)NULL)	/* window is not under destruction?	*/
  switch(message)
  {
    case WM_PAINT:
	  DT_Q(dthws_msw_event_paint,(ctl,
        hws, hws_win));
	break;
    case WM_SETFOCUS:
    case WM_PALETTECHANGED:
      InvalidateRect(priv_win->hwnd,	/* force pixel redrawing 			*/
        NULL, FALSE); 
	break;
    case WM_DESTROY:
	  {
#       ifdef DTCONFIG_IMPLIED_MSW16
	      PostAppMessage(				/* tell win16 upper layer about it  */
            GetCurrentTask(), 
#       else
	      PostThreadMessage(			/* tell win32 upper layer about it	*/
            GetCurrentThreadId(), 
#       endif
          DTMSW_MESSAGE_DESTROY, 
         (WORD)hwnd, lparam);
	    priv_win->destroyed = 1;		/* mark windows knows about it		*/
	  }
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
dthws_msw_handle_event(					/* handle event buffer				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  void *buffer,
  int *handled,
  dt_event_t *event)
{
  DT_F("dthws_msw_handle_event");
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

  if (elem == NULL)						/* didn't find the window?			*/
  {
	if (msg.message != WM_PAINT &&
		msg.message != WM_MOUSEMOVE)
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "0x%04lx, 0x%04x (%s), not our window (0x%04lx)",
	    (long)msg.hwnd, 
        msg.message, msg_text, 
        first_id);
	*handled = 0;
	return DT_RC_GOOD;
  }

  priv_win = (priv_win_t *)				/* our private info on the window 	*/
    hws_win->priv;

  if (msg.message != WM_PAINT &&
	  msg.message != WM_MOUSEMOVE)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "0x%04lx, 0x%04x (%s), 0x%04x, 0x%08lx",
	  (long)priv_win->hwnd,
      msg.message, msg_text, msg.wParam,
      msg.lParam);
  }
  
  *handled = 1;

  event->id = (long)msg.hwnd;			/* remember where event happened    */

  switch (msg.message)
  {
										/* ................................ */
	case WM_PAINT:
	  DT_Q(dthws_msw_event_paint,(ctl,
        hws, hws_win));
	break;
										/* ................................ */
	case WM_SIZE:
	if (priv_win != NULL)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "\"%s\" got WM_SIZE %d %d",
        priv_win->name, 
        LOWORD(msg.lParam), 
        HIWORD(msg.lParam));
	}
	break;
										/* ................................ */
	case WM_SETFOCUS:
    case WM_PALETTECHANGED:
      InvalidateRect(priv_win->hwnd,	/* force pixel redrawing 			*/
        NULL, FALSE); 
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
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
	  DT_Q(dthws_msw_event_key,(ctl,
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
dthws_msw_wait_event(					/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dthws_msw_wait_event");
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

	DT_Q(dthws_msw_handle_event,(ctl,
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

dt_rc_e
DTCONFIG_API1
dthws_msw_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *server)
{
  DT_F("dthws_msw_connect");
#ifdef DTCONFIG_IMPLIED_MSW
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

# if !defined(STRETCH_WIN)
    priv->hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    priv->hbrush = CreateSolidBrush(RGB(100, 0, 0));
# endif

  priv->classname = "dthws_msw_class";

  DT_MEMSET(&c, 0, sizeof(c));
  c.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  c.lpfnWndProc = dthws_msw_wndproc;
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

  hws->disconnect = dthws_msw_disconnect;
  hws->win_create = dthws_msw_win_create;
  hws->wait_event = dthws_msw_wait_event;
  hws->handle_event = dthws_msw_handle_event;
  hws->win_destroy = dthws_msw_win_destroy;
  hws->export_data = dthws_msw_export_data;
  hws->import_data = dthws_msw_import_data;
  hws->export_color = dthws_msw_export_color;
  hws->import_color = dthws_msw_import_color;
  hws->win_move = dthws_msw_win_move;
  hws->win_clear = dthws_msw_win_clear;
  hws->win_raise = dthws_msw_win_raise;
  hws->win_lower = dthws_msw_win_lower;
  hws->win_print = dthws_msw_win_print;
  hws->win_cursor = dthws_msw_win_cursor;

cleanup:
  if (rc != DT_RC_GOOD &&
	  priv != NULL)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  return rc;

# else
  return dt_err_notlinked(ctl, F, "Microsoft Windows API");
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
