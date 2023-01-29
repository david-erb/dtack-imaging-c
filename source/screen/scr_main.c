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
  20000506 DE move list destroy call
  19981014 DE different cursor positions
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

#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: scr_main.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/mem.h>
#include <dtack/str.h>
#include <dtack/font.h>

#define DESC_WIN_DATA "backing bitmap for window \"%s\""
#define DESC_WIN_LUTS "lookup tables for window \"%s\""
#define DESC_WIN_STRUCT "struct for window \"%s\""

static const char *choices[] = {
  "hws",
  "canvas",
  "dummy",
  NULL
};

static dtscr_create_f *methods[] = {
  dtscr_hws_create,
  dtscr_canvas_create,
  dtscr_dummy_create
};

#define BORDER (2)
#define FONTHEIGHT (16)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_create(							/* create the environment			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *server)
	{
  DT_F("dtscr_create");
  char *server2 = NULL;
  char *driver;
  int choice;
  dt_rc_e rc;

  if (server == NULL)
    return dt_err(ctl, F,
      "NULL screen spec");
  else
  if (strlen(server) == 0)
    return dt_err(ctl, F,
      "zero-length screen spec");

  DT_MEMSET(scr, 0, sizeof(*scr));			/* clear screen structure 			*/

  DT_C(dtos_strdup,(ctl, server,
    &server2));

  driver = strchr(server2, ',');		/* driver part after server		    */
  if (driver)
    *driver++ = '\0';
  else
    driver = "";

  DT_C(dtstr_choice,(ctl, choices,		/* look up scr type				    */
    server2, &choice));

  if (choice == -1)						/* unknown type					    */
  {
    rc = dt_err(ctl, F,
      "unknown screen spec \"%s\"",
      server2);
	goto cleanup;
  }
  else
  if (choice < -1)						/* ambiguous type				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous screen spec \"%s\"",
      server2);
	goto cleanup;
  }

  DT_C((methods[choice]),(ctl,			/* do the method					*/
    scr, driver))

  DT_C(dtos_free,(ctl, server2));		/* don't need this string anymore	*/
  server2 = NULL;
										/* ................................ */
  DT_C(dtos_malloc2,(ctl,				/* get space for display lut 		*/
    (void **)&scr->display_lut,
    256 * sizeof(*scr->display_lut),
    F, "display lut"));

  if (scr->must_remap &&
      scr->import_color)
  {
	DT_C((scr->import_color),(ctl,		/* get root screen's colors	    	*/
      scr, 
      NULL,								/* NULL means get it from screen	*/
      scr->display_lut, 
      0, 256));

	DT_C(dtlut8_tag,(ctl,				/* set tags in display lut		    */
      scr->display_lut));
	
	DT_C(dtlut8_sort,(ctl,				/* sort display colors			    */
      scr->display_lut));
    scr->white = 34;					/* for cursor and latch outline		*/
    scr->black = 240;
  }
  else									/* don't remap colors			    */
  {
    scr->white = 255;					/* for cursor and latch outline		*/
    scr->black = 1;
    DT_C(dtlut8_standard,(ctl,			/* make standard lut			    */
      scr->display_lut));
    if (scr->export_color)				/* has method?					    */
  	  DT_C((scr->export_color),(ctl,	/* tell server about initial color 	*/
        scr, NULL, scr->display_lut,
        0, 256));
  }
										/* ................................ */
  DT_C(dtos_malloc2,(ctl,				/* space for utility window 		*/
    (void **)&scr->tmp_win,
    sizeof(*scr->tmp_win),
    F, "scr->tmp_win"));
  
  DT_C(dtos_malloc2,(ctl,				/* space to shape cursor in 		*/
    (void **)&scr->cursorbuf,
    DTSCR_CURSOR_XSIZE *
    DTSCR_CURSOR_YSIZE,
    F, "scr->cursorbuf"));


  if (scr->have_window_manager)			/* window manager does borders		*/
  {
    scr->cursorx = 0;					/* initial cursor position		    */
    scr->cursory = 0;
    scr->margin_left = 0;
    scr->margin_right = 0;
    scr->margin_top = 0;
    scr->margin_bot = 0;
  }
  else
  {
    scr->cursorx = 100;					/* initial cursor position		    */
    scr->cursory = 100;
    scr->margin_left = (BORDER);
    scr->margin_right = (BORDER);
    scr->margin_top = (BORDER+FONTHEIGHT+BORDER);
    scr->margin_bot = (BORDER);
  }

  scr->overheadx =						/* per-window overhead			    */
    scr->margin_left+scr->margin_right;
  scr->overheady = 
    scr->margin_top+scr->margin_bot;

  DT_C(dtscr_state_default_actions,(	/* make list of default actions		*/
    ctl, scr, &scr->states_default));

  DT_C(dtlist_create,(ctl,				/* create window list			    */
    &scr->windowlist, "window list"));

  DT_C(dtscr_refresh,(ctl, scr));		/* refresh screen to background	    */

  return DT_RC_GOOD;
cleanup:
  if (scr->cursorbuf != NULL)
    DT_I(dtos_free2,(ctl, scr->cursorbuf,
      F, "cursorbuf"));
  if (scr->tmp_win != NULL)
    DT_I(dtos_free2,(ctl, scr->tmp_win,
      F, "tmp_win"));
  if (scr->display_lut != NULL)
    DT_I(dtos_free2,(ctl, scr->display_lut,
      F, "display_lut"));
  if (server2 != NULL)
    DT_I(dtos_free,(ctl, server2));
  
  DT_MEMSET(scr, 0, sizeof(*scr));
  return rc;
}

/*..........................................................................
 *       wwwwwwwwwwwwwwwwwwwwwwwwwwwwww
 *       wbbbbbbbbbbbbbbbbbbbbbbbbbbbbw
 *       wb windowname               bw
 *       wwwwwwwwwwwwwwwwwwwwwwwwwwwwww
 *       wbbbbbbbbbbbbbbbbbbbbbbbbbbbbw
 *       wb                          bw
 *       wb                          bw
 *       wb                          bw
 *       wb                          bw
 *       wb                          bw
 *       wb                          bw
 *       wb                          bw
 *       wb                    xe,ye\bw
 *       wbbbbbbbbbbbbbbbbbbbbbbbbbbbbw
 *       wwwwwwwwwwwwwwwwwwwwwwwwwwwwww
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_margin(					/* draw margin in backing bitmap	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_window_margin");
  dtlist_elem_t *elem;
  dtscr_win_t *win;
  dtmemalloc_t *memalloc;
  unsigned int y;
  unsigned int uxe;
  unsigned int uye;
  long offset;
  unsigned int size;
  char *p;
  unsigned int units;
  unsigned int i;

  if (scr->have_window_manager)			/* window manager does borders		*/
    return DT_RC_GOOD;


  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  memalloc = (dtmemalloc_t *)win->mem;	/* underlying big memory 			*/
  uye = memalloc->units;
  uxe = memalloc->size;
  for (y=0; y<uye; y+=i)				/* for whole patch				    */
  {
    offset = (long)(y) *				/* offset where row starts			*/
      (long)memalloc->size;
    DT_Q((memalloc->map),(ctl,			/* map the memory					*/
      memalloc, offset, 
      (void **)&p, &size));
    units = size / uxe;					/* units we can access				*/
    units = DT_MIN(units, uye-y);		/* limit at end of patch		    */
    DT_MEMSET(p, scr->black, units*uxe);	/* clear the patch to black		    */
    for (i=0; i<units; i++)
    {
      if (y+i == 0 ||					/* one of the horizontal lines?	    */
          y+i == BORDER+FONTHEIGHT ||
          y+i == uye - 1)
        DT_MEMSET(p, scr->white, uxe);
      else								/* vertical lines?				    */
        p[0] = p[uxe-1] = scr->white;
      p += uxe;
    }
    DT_Q((memalloc->put),(ctl,			/* update big memory 				*/
      memalloc, offset));
  }

  DT_Q(dtfont_8x16_string,(ctl,			/* output title with font 			*/
    windowname, 
    memalloc, DT_NTYPE_B08,
    BORDER+2, BORDER,
    scr->white));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_create_window(					/* make a window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)								/* bits per pixel					*/
{
  DT_F("dtscr_create_window");
  dtlist_elem_t *elem;
  dtscr_win_t *win;
  char tmp[DT_WINDOWNAME_MAXLEN+64];

  if (strlen(windowname)+1 >
      DT_WINDOWNAME_MAXLEN)
    return dt_err(ctl, F,
      "window name \"%s\" too long",
      windowname);

  DT_Q(dtlist_search,(ctl,				/* see if window exists				*/
    &scr->windowlist, 
    windowname, &elem));

  if (elem)								/* it exists?					    */
  {
    if (x0 == -1 && y0 == -1)			/* interactively position?		    */
    {
      win = (dtscr_win_t *)elem->user;
      x0 = win->x0;						/* use last position			    */
      y0 = win->y0;
    }
    DT_Q(dtscr_window_destroy,(ctl,		/* get rid of old window		    */
      scr, windowname));
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "replacing window \"%s\" at (%d,%d) size [%dx%d]",
	  windowname, x0, y0, xe, ye);
  }
  else
  {
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "creating window \"%s\" at (%d,%d) size [%dx%d]",
	  windowname, x0, y0, xe, ye);
  }

  sprintf(tmp, DESC_WIN_STRUCT,
    windowname);
  DT_Q(dtos_malloc2,(ctl,				/* space for window instance	    */
    (void **)&win, 
    sizeof(*win),
    F, tmp));
  DT_MEMSET(win, 0, sizeof(*win));
  strcpy(win->name, windowname);
  win->x0 = x0;
  win->y0 = y0;
  win->xe = xe + scr->overheadx;
  win->ye = ye + scr->overheady;
  win->pixmap_bits = bits;

  sprintf(tmp, DESC_WIN_DATA,
    windowname);

# ifdef DTCONFIG_IMPLIED_DOS16			/* this is going to run on DOS-16?  */
    DT_Q(dtmem_alloc,(ctl,				/* space for backing bitmap		    */
      &win->mem,
	  (unsigned int)(2048/win->xe) *	/* use tiny page size				*/
        win->xe, 1,
      win->xe, win->ye,
      F, tmp));
# else  	                            /* not DOS 16-bit? 					*/
    DT_Q(dtmem_alloc,(ctl,				/* space for backing bitmap		    */
      &win->mem,
      0, 0,								/* default page size and count 		*/
      win->xe, win->ye,
      F, tmp));
# endif

  DT_Q(dtlist_after,(ctl,				/* add to head of list			    */
    &scr->windowlist, 
    DTLIST_ELEM_HEAD,
    win->name, win, NULL));

  DT_Q(dtmem_set,(ctl, win->mem,		/* clear backing bitmap			    */
    (char)scr->black));

  DT_Q(dtscr_window_margin,(ctl, scr,	/* make margin in backing bitmap	*/
    windowname));

  if (scr->create_window)
  DT_Q((scr->create_window),(ctl, scr,	/* do the method				    */
    win, windowname,
    x0, y0, xe, ye, bits));

#ifdef NONO
  if (scr->must_remap &&				/* color remapping going on? 		*/
	  scr->export_color)
#endif

  if (scr->export_color)
  {
    sprintf(tmp, DESC_WIN_LUTS,
      windowname);
    DT_Q(dtos_malloc2,(ctl,				/* space for window's luts		    */
      (void **)&win->luts, 
      sizeof(*win->luts),
      F, tmp));
    DT_Q(dtlut8_standard,(ctl,			/* make standard lut			    */
      win->luts->current));
    DT_Q(dtscr_export_color,(ctl,		/* output own lut to window		    */
      scr, windowname,
      win->luts->current, 0, 256));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_export_data(						/* export patch into window			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits,
  void *data)
{
  DT_F("dtscr_export_data");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* see if window exists				*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

#if 1
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "exporting to \"%s\""
    " (%d,%d)-(%d,%d) remap=%d",
	windowname,
    x0, y0, xe, ye,
    scr->must_remap);
#endif

  if (scr->must_remap)					/* screen pixels are remapped? 		*/
  DT_Q_1D(dt1d_lookup_inplace_uchar,(	/* map input pixels to screen 		*/
    (unsigned char *)data, 
    (unsigned int)xe*(unsigned int)ye,
    win->luts->user_to_display));

  DT_Q(dtmem_putpatch,(ctl,				/* copy patch into backing store    */
    win->mem,
    data,
    x0 + scr->margin_left, 
    y0 + scr->margin_top,
    xe, ye));

  if (scr->export_data)					/* has a method?				    */
  DT_Q((scr->export_data),(ctl, scr,	/* do the method				    */
    win,
    x0 + scr->margin_left, 
    y0 + scr->margin_top,
    xe, ye, bits, data));

  if (scr->must_remap)					/* screen pixels are remapped? 		*/
  DT_Q_1D(dt1d_lookup_inplace_uchar,(	/* fix them before returning 		*/
    (unsigned char *)data, 
    (unsigned int)xe*(unsigned int)ye,
    win->luts->display_to_user));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_import_data(						/* extract image from window	    */
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0,
  int xe,
  int ye,
  void *data)
{
  DT_F("dtscr_import_data");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* see if window exists				*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "importing from \"%s\""
    " (%d,%d)-(%d,%d) remap=%d",
	windowname,
    x0, y0, xe, ye,
    scr->must_remap);

  DT_Q(dtmem_getpatch,(ctl,				/* just get it from backing store  	*/
    win->mem,
    data, 
    x0 + scr->margin_left, 
    y0 + scr->margin_top,
    xe, ye));

  if (scr->must_remap)					/* screen pixels are remapped? 		*/
  DT_Q_1D(dt1d_lookup_inplace_uchar,(	/* fix them before returning 		*/
    (unsigned char *)data, 
    (unsigned int)xe*(unsigned int)ye,
    win->luts->display_to_user));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e dtscr_lookup(
  dt_ctl_t *ctl,
  void *vmemalloc,
  unsigned char *lut)
{
  DT_F("dtscr_lookup");
  dtmemalloc_t *memalloc = (dtmemalloc_t *)vmemalloc;
  long offset;
  unsigned int size;

  for (offset=0;
       offset < memalloc->total;
       offset += size)
  {
    void *p;
    DT_Q((memalloc->map),(ctl,
      memalloc, offset, 
      &p, &size));
	DT_Q_1D(dt1d_lookup_inplace_uchar,(
      (unsigned char *)p, size, lut));
    DT_Q_1D((memalloc->put),(ctl,
      memalloc, offset));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * if caller names non-existing window, one will be created and zero-filled
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_export_color(						/* export color lut to window		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtscr_export_color");
  dtlist_elem_t *elem;
  dtscr_win_t *win;
  dtlut8_t *display_lut = NULL;
  int i;
  dt_rc_e rc;

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  DT_C(dtlist_find,(ctl,				/* see if window exists				*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  dt_dbg(ctl, F, DT_DBG_MASK_LUT,
    "new colors start %d count %d"
    " for \"%s\", must_remap=%d",
    start, count, windowname,
    scr->must_remap);

    if (win->luts == NULL)				/* haven't already done this?	    */
	{
  	  char tmp[DT_WINDOWNAME_MAXLEN+64];
      sprintf(tmp, DESC_WIN_LUTS,
        windowname);
      DT_C(dtos_malloc2,(ctl,			/* space for window's luts		    */
        (void **)&win->luts, 
        sizeof(*win->luts),
        F, tmp));
      DT_C(dtlut8_standard,(ctl,		/* make standard lut			    */
        win->luts->current));
	}

    for (i=0; i<count; i++)				/* copy new lut elements 			*/
      win->luts->current[i+start] =
        lut[i];


  if (scr->must_remap &&
      scr->import_color)
  {

	DT_C(dtscr_lookup,(ctl, win->mem,	/* change backing store to user 	*/
      win->luts->display_to_user));
	
    for (i=0; i<DTLUT8_NSTANDARD; i++)
    {
      int idx;
	  int j = dtlut8_standard_index[i];
	  
      DT_C(dtlut8_closest,(ctl,			/* find closest color			    */
        &win->luts->current[j],
        scr->display_lut,
        &idx));
      if (idx != -1)					/* there is a closest one?		    */
	  {
		dt_dbg(ctl, F, DT_DBG_MASK_LUT,
		  "i %d, j %d, 0x%02x%02x%02x%02x = idx %d 0x%02x%02x%02x%02x",
		  i, j,
          win->luts->current[j].r,
          win->luts->current[j].g,
          win->luts->current[j].b,
          win->luts->current[j].i,
		  idx,
          scr->display_lut[idx].r,
          scr->display_lut[idx].g,
          scr->display_lut[idx].b,
          scr->display_lut[idx].i);

        win->luts->current[j] =			/* set it so we find perfect match 	*/
          scr->display_lut[idx];
	  }
    }
 
    DT_C(dtlut8_match,(ctl,				/* match display to user		    */
      win->luts->current,
      scr->display_lut,
      win->luts->user_to_display,
      win->luts->display_to_user));

	DT_C(dtos_malloc2,(ctl, 
      (void **)&display_lut,
      256*sizeof(*display_lut),
      F, "display_lut"));

	for (i=0; i<256; i++)				/* align our display to screen		*/
	{
	  int j;
	  j = win->luts->user_to_display[i];
	  display_lut[j] =
        win->luts->current[i];
	}

	if (DT_DBG(ctl, DT_DBG_MASK_LUT))
    for (i=0; i<DT_MIN(16, count); i++)
      dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "%3d. %3d %3d",
        i+start,
        win->luts->user_to_display[i+start],
        win->luts->display_to_user[i+start]);

	DT_C(dtscr_lookup,(ctl, win->mem,	/* change backing store to display	*/
      win->luts->user_to_display));

    if (scr->redraw)
      DT_C((scr->redraw),(ctl,			/* redraw the image data 			*/
        scr, win));
    if (scr->export_color)
    {
      DT_C((scr->export_color),(ctl,	/* tell server about color change 	*/
        scr, win, 
        display_lut,
        0, 256));
  	  for (i=0; i<10; i++)
        dt_dbg(ctl, F, DT_DBG_MASK_LUT,
          "lut[%d] rgb %3d %3d %3d",
          i, 
		  display_lut[i].r,
		  display_lut[i].g,
		  display_lut[i].b);
	}
  }
  else
  {
    if (scr->export_color)
      DT_C((scr->export_color),(ctl,	/* tell server about color change 	*/
        scr, win, lut,
        start, count));
  }

cleanup:
  if (display_lut != NULL)
    DT_I(dtos_free2,(ctl, display_lut,
      F, "display_lut"));

  return rc;
}

/*..........................................................................
 * windowname may be NULL indicating caller wants base colors
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_import_color(						/* import color lut to window		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtscr_import_color");
  dtscr_win_t *win;

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  if (windowname == NULL ||
	  strlen(windowname) == 0)
  {
    win = NULL;
    if (scr->import_color)
      DT_Q((scr->import_color),(ctl,
        scr, win, lut,
        start, count))
    else
      return dt_err(ctl, F, "no method");
  }
  else
  {
    dtlist_elem_t *elem;
	int i;
    DT_Q(dtlist_find,(ctl,				/* see if window exists				*/
      &scr->windowlist, 
      windowname, &elem));
    win = (dtscr_win_t *)elem->user;

	for (i=0; i<count; i++)
      lut[i] = win->luts->current[i+start]; 

  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_translate_event(					/* translate event 					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  int *user,
  int *quit)
{
  DT_F("dtscr_translate_event");
  dt_event_t *event = &scr->last_event;
  dtlist_elem_t *elem;
  dtscr_state_t state_struct;
  dtscr_state_t *state = &state_struct;

  if (scr->refresh_pending)
    DT_Q(dtscr_refresh,(ctl, scr));

  if (event->type == DT_EVENT_IPC)		/* svc is special				    */
	return DT_RC_GOOD;

  DT_MEMSET(state, 0, sizeof(*state));

  elem = DTLIST_ELEM_HEAD;
  while (elem)
  {
	DT_Q(dtlist_next,(ctl,				/* fetch next window in list        */
      &scr->windowlist, 
      elem, &elem));
	if (elem && 						/* not the last in the list?        */
		event->id == ((dtscr_win_t *)	/* and this is the window?			*/
		  elem->user)->id)
	  break;
  }
  
  if (elem)								/* on a window?                     */
	strcpy(state->windowname,
      elem->name);
  else                  				/* not on a window                  */
    strcpy(state->windowname, "");

  switch(event->type)
  {
    case DT_EVENT_NOTHING:				/* ................................ */
	break;
    case DT_EVENT_DESTROYED:			/* ................................ */
		dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
          "handling DESTROYED event"
          " for \"%s\" (0x%04x)",
          state->windowname, event->id);
		if (strlen(state->windowname))	/* we recognize the window? 		*/
		  DT_Q(dtscr_window_destroy,(	/* destroy the window 				*/
            ctl, scr, 
            state->windowname));
	break;
    case DT_EVENT_MOVED:				/* ................................ */
	{
        dtscr_win_t *win;
        *user = 0;
        if (elem == NULL)
          return dt_err(ctl, F,
            "MOVED event for unidentifed window id %d",
            event->id);
		else
		dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
		  "MOVED \"%s\" to %d,%d",
		  elem->name, event->x0, event->y0);
        win = (dtscr_win_t *)elem->user;
        win->x0 = event->x0;			/* remember new position		    */
        win->y0 = event->y0;
	}
	break;
    case DT_EVENT_RESIZED:				/* ................................ */
    {
        dtscr_win_t *win;
        *user = 1;
        if (elem == NULL)
          return dt_err(ctl, F,
            "RESIZED event for unidentifed window id %d",
            event->id);
		else
		dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
		  "RESIZED \"%s\" to %d,%d",
		  elem->name, event->xe, event->ye);

        win = (dtscr_win_t *)elem->user;
        win->x0 = event->x0;			/* remember new position		    */
        win->y0 = event->y0;
	}
	break;
    case DT_EVENT_MOUSEPRESS:			/* ................................ */
    case DT_EVENT_MOUSEDOUBLEPRESS:
    case DT_EVENT_MOUSEMOVE:
    case DT_EVENT_KEYBOARD:
	{
	  int i;
	  int matches;
        *user = 1;
        state->x = event->mousex;		/* make state for doing matching	*/
        state->y = event->mousey;
        state->left = event->left;
        state->middle = event->middle;
        state->right = event->right;

        for (i=0; i<event->nkeys; i++)
		{
          switch(event->keys[i])
          {
            case DT_KEY_ENTER:
              state->keystring[i] = DTSCR_KEY_ENTER;      break;
            case DT_KEY_DELETE:
              state->keystring[i] = DTSCR_KEY_CLOSE_ONE;  break;
			case DT_KEY_TAB:
              state->keystring[i] = DTSCR_KEY_CYCLE_NEXT; break;
            case DT_KEY_BACKSPACE:
              state->keystring[i] = DTSCR_KEY_CYCLE_PREV; break;
            case DT_KEY_LEFTARROW:
              state->keystring[i] = DTSCR_KEY_LEFTARROW;  break;
            case DT_KEY_RIGHTARROW:
              state->keystring[i] = DTSCR_KEY_RIGHTARROW; break;
            case DT_KEY_UPARROW:
              state->keystring[i] = DTSCR_KEY_UPARROW;    break;
            case DT_KEY_DOWNARROW:
              state->keystring[i] = DTSCR_KEY_DOWNARROW;  break;
            default:
              state->keystring[i] = event->keys[i];
          }
		}
        state->keystring[i] = '\0';
  
        dtscr_state_dbg(ctl, scr,		/* spit out state we just got		*/
          state, F, DT_DBG_MASK_ASYNC,
          "new state:  ");
  
        DT_Q(dtscr_state_matches,(ctl,	/* see if we have matching states	*/
          scr, &scr->states_default,
          state, &matches));
  
        if (matches != 0)				/* there are some matches?			*/
          DT_Q(dtscr_state_actions,(ctl, /* do all actions for this state	*/
            scr, &scr->states_default,
            state, quit));
	}
    break;
    default:
      return dt_err(ctl, F,
        "dt_event type %d that we can't handle",
        event->type);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_wait_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtsvc_t *svc,
  int flag)
{
  DT_F("dtscr_wait_event");
  dt_event_t *event = &scr->last_event;
  int user;
  double timeout;
  int quit = 0;

  if (scr->refresh_pending)
    DT_Q(dtscr_refresh,(ctl, scr));

  if (scr->wait_event == NULL)			/* don't have method?			 	*/
    return DT_RC_GOOD;					/* skip it					     	*/

  if (flag == 0)						/* this is a poll? 					*/
    timeout = -1;
  else									/* this is a wait? 					*/
    timeout = 0;

  do {
    dtlist_elem_t *elem;
	int matches = 0;
    int i;

    if (flag != 0)						/* this is not a poll?				*/
	{
	  DT_Q(dtscr_cursor_draw,(ctl, scr,	/* draw cursor						*/
        1));
	  DT_Q(dtscr_latch_draw,(ctl, scr, 	/* replace latch				    */
        1));
	}

    DT_Q((scr->wait_event),(ctl, scr,	/* get the event					*/
      svc, timeout, flag, event));

    event->mousex -= scr->cursorx; 
    event->mousey -= scr->cursory;

    if (flag != 0)						/* this is not a poll?				*/
	{
	  DT_Q(dtscr_cursor_draw,(ctl, scr,	/* remove cursor					*/
        0));
      DT_Q(dtscr_latch_draw,(ctl, scr,	/* remove old latch				    */
        0));
	}

    if (event->type == DT_EVENT_IPC)	/* svc is special				    */
	{									/* no state matching			    */
      dt_dbg(ctl, F,
        DT_DBG_MASK_ASYNC|
        DT_DBG_MASK_IPC,
        "svc request causes return");
      break;	
	}

    DT_Q(dtscr_translate_event,(ctl,	/* translate event 					*/
      scr, &user, &quit));

	if (flag == 0)						/* just polling? 					*/
	  quit = 1;
    else
    if (flag == 1)						/* to quit after any event?			*/
      quit = 1;
    else
    if (flag == 2 && matches == 0)		/* to quit on unmatched event?		*/
      quit = 1;
    else
    if (flag == 3)						/* to quit on any user event?	    */
      quit = user;
  } while (!quit);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_handle_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  void *buffer,
  int *handled)
{
  DT_F("dtscr_handle_event");
  dt_event_t *event = &scr->last_event;
  int user;
  double timeout;
  int quit = 0;

  if (scr->refresh_pending)
    DT_Q(dtscr_refresh,(ctl, scr));

  if (scr->handle_event == NULL)		/* don't have method?			 	*/
  {
	*handled = 0;
    return DT_RC_GOOD;					/* skip it					     	*/
  }

  DT_Q((scr->handle_event),(ctl, scr,	/* handle the event					*/
    buffer, handled, event))

  if (*handled)
    DT_Q(dtscr_translate_event,(ctl,	/* translate event 					*/
      scr, &user, &quit));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_query_state(						/* query window state				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  dtscr_state_t *state)
{
  DT_F("dtscr_query_state");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_MEMSET(state, 0, sizeof(*state));		/* clear structure being returned	*/

  DT_Q(dtlist_search,(ctl,				/* see if window exists				*/
    &scr->windowlist, 
    windowname, &elem));

  if (elem == NULL)						/* window does not exist? 			*/
    return DT_RC_GOOD;

  win = (dtscr_win_t *)elem->user;		/* address the window structure 	*/

										/* give back just enough to keep	*/
										/* the win interface happy		    */
  state->x0 = win->x0;
  state->y0 = win->y0;
  state->xe = win->xe - scr->overheadx;
  state->ye = win->ye - scr->overheady;
  state->bits = win->pixmap_bits;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_window_destroy(					/* close window					    */
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_window_destroy");
  dtlist_elem_t *elem;
  dtscr_win_t *win;
  char tmp[DT_WINDOWNAME_MAXLEN+64];
  dt_rc_e rc = DT_RC_GOOD;
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "destroying \"%s\"", windowname);

  DT_I(dtlist_find,(ctl,				/* see if window exists				*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  DT_I(dtlist_delete,(ctl,				/* remove window from list		    */
    &scr->windowlist, elem));

  if (scr->destroy_window)				/* has method?					    */
  DT_I((scr->destroy_window),(ctl, scr,	/* do method						*/
    win));

  sprintf(tmp, DESC_WIN_DATA,
    windowname);
  DT_I(dtmem_free,(ctl,					/* space for backing store		    */
    win->mem, F, tmp));

  sprintf(tmp, DESC_WIN_LUTS,
    windowname);
  if (win->luts)
  DT_I(dtos_free2,(ctl,					/* space for lookup tables			*/
    win->luts, F, tmp));

  sprintf(tmp, DESC_WIN_STRUCT,			/* get rid of window's structure    */
    windowname);
  DT_I(dtos_free2,(ctl,
    win, F, tmp));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_destroy(							/* destroy the screen				*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_destroy");
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;
  
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "destroying");

  do {
    DT_Q(dtlist_next,(ctl,				/* fetch next window in list	    */
      &scr->windowlist, 
      DTLIST_ELEM_HEAD, &elem));
    if (elem)							/* not the last in the list?	    */
      DT_I(dtscr_window_destroy,(		/* free it						    */
        ctl, scr, elem->name));
  } while(elem);

  if (scr->latchbuf)					/* something latched?			    */
    DT_I(dtos_free2,(ctl,				/* free latch buffer			    */
      scr->latchbuf,
      F, "latch buffer"));

  if (scr->destroy)
    DT_I((scr->destroy),(ctl, scr));

  DT_I(dtlist_destroy,(ctl, 			/* get rid of list structure		*/
    &scr->windowlist));

  DT_I(dtscr_state_free,(				/* free list of default actions		*/
    ctl, scr, &scr->states_default));

  if (scr->cursorbuf != NULL)
    DT_I(dtos_free2,(ctl, scr->cursorbuf,
      F, "cursorbuf"));
  if (scr->tmp_win != NULL)
    DT_I(dtos_free2,(ctl, scr->tmp_win,
      F, "tmp_win"));
  if (scr->display_lut != NULL)
    DT_I(dtos_free2,(ctl, scr->display_lut,
      F, "display_lut"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_window_raise");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "raising \"%s\"", elem->name);

  DT_Q(dtlist_front,(ctl,				/* bring to front of list		 	*/
    &scr->windowlist, elem,
    NULL));

  if (scr->raise)						/* there is a method?			    */
    DT_Q((scr->raise),(ctl, 
      scr, win));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_window_lower");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  DT_Q(dtlist_back,(ctl,				/* bring to back of list		 	*/
    &scr->windowlist, elem,
    NULL));

  if (scr->lower)						/* there is a method?			    */
    DT_Q((scr->lower),(ctl, 
      scr, win));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtscr_popup_text_timed(					/* pop up text window for time		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *title,
  const char *text,
  long milliseconds)
{
  DT_F("dtscr_popup_text_timed");
  dtimage_t image;
  dtxy_t x0, y0, xe, ye;
  int window_created = 0;
  long i, n;
  dt_rc_e rc;

  image.xe = 0;

  x0 = 8;
  y0 = 4;
  xe = x0 + strlen(text) * 8 + x0;
  ye = y0 + 16 + y0 - 2;

  DT_C(dtimage_create,(ctl, &image,
    xe, ye, DTNTYPE_B08));
  
  DT_C(dtmem_clear,(ctl,				/* clear memory to 0 				*/
    image.memalloc));

  DT_C(dtfont_8x16_string,(ctl,			/* draw text in image			  	*/
    text, 
    image.memalloc, image.ntype,
    x0, y0, 234.0));

  x0 = (scr->xe - xe) / 2;				/* screen location of window	    */
  y0 = (scr->ye - ye) / 2;
  DT_C(dtscr_create_window,(ctl,		/* create window on screen		    */
    scr, title, 
    x0, y0, xe, ye, 8));
  window_created = 1;

  DT_C(dtscr_export_data,(ctl, scr,		/* send message to window		    */
    title,
    0, 0, xe, ye, 8,
    image.row[0].dvoid));

#define POLLTIME (100L)					/* milliseconds 					*/
  
  n = milliseconds / POLLTIME + 1L;
  for (i=0; i<n; i++)
  {
    DT_C(dtscr_wait_event,(ctl,			/* poll for event					*/
      scr, (dtsvc_t *)NULL, 0));
	DT_C(dtos_delay,(ctl, POLLTIME));
  }

cleanup:
  if (window_created)					/* window got created?			    */
  {
	DT_I(dtscr_window_destroy,(ctl,		/* destroy it					    */
      scr, title));
          
    DT_I(dtscr_wait_event,(ctl,			/* poll for event (screen repair)	*/
      scr, (dtsvc_t *)NULL, 0));
  }

  if (image.xe > 0)
	DT_I(dtimage_free,(ctl, &image));
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_print(						/* print window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)
{
  DT_F("dtscr_window_print");
  const char *title;
  const char *text;
  dtlist_elem_t *elem;
  dtscr_win_t *win;
  dt_rc_e rc;

  title = "Print job failed...";
  text = "Could not find window.";
  DT_C(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  text = "No print method implemented.";
  if (!scr->print)						/* there is a method?			    */
	goto cleanup;

  text = "Printer error.";

  DT_C((scr->print),(ctl,				/* ask lower layer to print 		*/
    scr, win, printer, ppm,
    submitted));

  text = "Print job canceled.";
  if (*submitted == 0)
	goto cleanup;

  title = "Print job submitted...";
  text = "Print job successfully submitted.";

cleanup:
  DT_I(dtscr_popup_text_timed,(ctl,
    scr, title, text, 3000));
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_iconify(					/* iconify window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_window_iconify");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  if (scr->iconify)
    DT_Q((scr->iconify),(ctl, 
      scr, win));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_move(						/* move window						*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  int x0,
  int y0)
{
  DT_F("dtscr_window_move");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  if (scr->move)
    DT_Q((scr->move),(ctl, 
      scr, win, x0, y0));

  win->x0 = x0;
  win->y0 = y0;

  return DT_RC_GOOD;
}

/*..........................................................................
| canvas colormaps by setting entire canvas colormap to that of window
| hws colormaps by doing best color match of window to root then remapping
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_window_colormap(					/* colormap window					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname)
{
  DT_F("dtscr_window_colormap");
  dtlist_elem_t *elem;
  dtscr_win_t *win;

  DT_Q(dtlist_find,(ctl,				/* find window name				 	*/
    &scr->windowlist, 
    windowname, &elem));
  win = (dtscr_win_t *)elem->user;

  if (scr->colormap)
    DT_Q((scr->colormap),(ctl, 
      scr, win));

  return DT_RC_GOOD;
}

/*..........................................................................
 * clear screen and redisplay all windows
 * to clear junk out from in front of windows
 * canvases on DOS can get covered by junky text
 * windows in hws can get covered up by windows from other applications
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_refresh(							/* refresh the screen				*/
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_refresh");

  if (scr->refresh)
  DT_Q((scr->refresh),(ctl, scr));

  scr->refresh_pending = 0;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_raise_all(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtscr_raise_all");
  dtlist_elem_t *elem;
  elem = DTLIST_ELEM_TAIL;				/* start at the "backest" window	*/

  if (scr->raise)						/* there is a method?			    */
  do {
    DT_Q(dtlist_prev,(ctl,				/* get next window to front		    */
      &scr->windowlist, elem, &elem));
    if (elem)							/* we're not at the top?		    */
      DT_Q((scr->raise),(ctl, scr,		/* raise the window					*/
        (dtscr_win_t *)elem->user));
  } while(elem);

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
