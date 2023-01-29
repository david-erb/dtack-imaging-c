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
DT_RCSID("server $RCSfile: view.c,v $ $Revision: 1.4 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/win_r.h>					/* needed inside dtwins.h only 		*/
#include <dtack/wins.h>						/* we are an svc-on-scr server 		*/

#include <dtack/dsk.h>

#include <dtack/str.h>
#include <dtack/mem.h>

#include <dtack/view.h>

/*..........................................................................*/

dt_rc_e
dtview_panel(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *title,
  const char *string,
  int x0,
  int y0,
  int gravity)
{
  DT_F("dtview_panel");
  dtimage_t image_struct;
  dtimage_t *image = &image_struct;
  const char *p, *q;
  int ncols, nrows;

  q = string;
  ncols = 0;
  nrows = 0;
  do {									/* scan input string			    */
    p = strchr(q, '\n');				/* end of current line			    */
    if (p == NULL)
      p = q + strlen(q);
    ncols = DT_MAX(ncols, (int)(p-q));	/* remember longest line		    */
    nrows++;							/* count the rows				    */
    q = p + 1;							/* next line in string			    */
  } while(*p);

  DT_Q(dtimage_create3,(ctl, image,		/* space for the panel image	    */
    ncols*8,							/* pagesize							*/
    1,									/* npages						    */
    ncols*8, 
    nrows*16,
    DT_NTYPE_B08, NULL));

  DT_Q(dtimage_constant,(ctl,			/* fill image background		    */
    image, DTVIEW_BACKGROUND));

  DT_Q(dtimage_plot_text_8x16,(ctl,		/* render string into image		    */
    image, (char *)string, 0, 0,
    DTVIEW_FOREGROUND));

  switch(gravity)
  {
    case DTVIEW_GRAVITY_SW: 
      y0 = y0 - nrows*16 - scr->overheady;
    break;
    case DTVIEW_GRAVITY_NE: 
      x0 = x0 - ncols*8 - scr->overheadx;
    break;
    case DTVIEW_GRAVITY_SE: 
      x0 = x0 - ncols*8 - scr->overheadx;
      y0 = y0 - nrows*16 - scr->overheady;
    break;
    case DTVIEW_GRAVITY_NW:
	break;
    case DTVIEW_GRAVITY_CENTER: 
      x0 = x0 - (ncols*8 + scr->overheadx) / 2;
      y0 = y0 - (nrows*16 + scr->overheady) / 2;
    break;
    default:;
  }

  DT_Q(dtxchg_scr_export_image,(ctl,	/* output image to screen		    */
    scr, title,
    image, x0, y0));

  DT_Q(dtimage_free,(ctl, image));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtview_dsk(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *tmpdir)
{
  DT_F("dtview_dsk");
  dtsvc_t svc;
  char *svcspec = NULL;
  dtwins_t wins;
  int have_wins = 0;
  dt_rc_e rc;

  svc.priv = NULL;

  DT_C(dtos_strdup3,(ctl,				/* put together a dsk svcspec		*/
    "dsk", ",", tmpdir, &svcspec));

  DT_C(dtsvc_register,(ctl,				/* register public name			    */
    &svc, svcspec, 0));					/* don't go background 				*/

  DT_C(dtwins_init,(ctl, &wins, scr));	/* prepare to serve 				*/
  have_wins = 1;

  while (rc == DT_RC_GOOD)				/* loop for work 					*/
  {
	DT_C(svc.wait,(ctl, &svc));			/* wait for svc work 				*/
	DT_C(dtwins_dispatch,(ctl,			/* handle the work using scr		*/
	  &wins, &svc));
  }

cleanup:
  if (rc == DT_RC_STOP)					/* just got a stop from client? 	*/
	rc = DT_RC_GOOD;					/* don't get excited 				*/

  if (svcspec != NULL)
    DT_I(dtos_free2,(ctl, svcspec,
      F, "svcspec"));
    
  if (have_wins)
    DT_I(dtwins_uninit,(ctl, &wins));	/* quit serving						*/

  if (svc.priv != NULL)
    DT_I(dtsvc_unregister,(ctl, &svc));	/* quit dsk server activities		*/

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtview_dsk_delete(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *dskspec)
{
  DT_F("dtview_dsk_delete");
  dtxchg_e which;
  char *id = NULL;
  dtlist_t dsklist;
  dtlist_elem_t *dskelem;
  dtlist_elem_t *screlem;
  dt_err_t err;
  char message[DT_ERR_LENGTH];
  dt_rc_e rc;

  DT_C(dtxchg_parse,(ctl,				/* parse to get the id				*/
    dskspec, &which,
    NULL, &id, 
    &err, message, sizeof(message)));
  if (err != DT_ERR_NONE)
  {
	rc = dt_err(ctl, F,
      "%s", message);
	goto cleanup;
  }
  
  DT_C(dtdsk_getlist,(ctl, id,			/* read list of windows from file	*/
    &dsklist));

  dskelem = DTLIST_ELEM_HEAD;			/* go through disk list from top 	*/
  do {
    DT_C(dtlist_next,(ctl, &dsklist,	/* fetch next window structure		*/
      dskelem, &dskelem));
    if (dskelem != NULL)	
    {
      DT_C(dtlist_search,(ctl,			/* search screen list 				*/
        &scr->windowlist, 
        dskelem->name,
        &screlem));
	  if (screlem == NULL)				/* gone from screen? 				*/
	  {
		dt_dbg(ctl, F, 
          DT_DBG_MASK_IPC |
          DT_DBG_MASK_WIN,
          "\"%s\" is gone from screen,"
          " deleting from %s",
          dskelem->name, dskspec);
        DT_C(dtdsk_delete,(ctl, id, 	/* delete disk window				*/
          dskelem->name));
	  }
    }
  } while (dskelem != NULL);

cleanup:
  DT_I(dtlist_free,(ctl, &dsklist));	/* free list					    */
  DT_I(dtlist_destroy,(ctl, &dsklist));	/* destroy list					    */

  if (id)
    DT_I(dtos_free2,(ctl, id,
      F, "exchanger id"));

  return rc;
}

/*..........................................................................
| get data from one scr window and output it to ipc server
 *..........................................................................*/

static
dt_rc_e
dtview_save_one(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *windowname,
  const char *dirname,
  char *message,
  int message_max)
{
  DT_F("dtview_save_one");
  dtscr_state_t state;
  dtimage_t image;
  char *dsk_server = NULL;
  dt_rc_e rc;

  image.xe = 0;
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\" \"%s\"", 
    dirname, windowname);

  DT_C(dtxchg_scr_import_image,(ctl,	/* make image from window		    */
    scr, windowname, &image, 
    0, 1));								/* only need one page at a time */

  DT_C(dtscr_query_state,(ctl, scr,		/* find out x0,y0 of window			*/
    windowname, &state));

  if (state.xe == 0)					/* window does not exist? 			*/
  {
    rc = dt_err(ctl, F,
      "window \"%s\""
      " does not exist",
      windowname);
	goto cleanup;
  }

  DT_C(dtos_strdup3,(ctl,				/* make up server name 				*/
    "dsk", ",", dirname,
    &dsk_server));

  DT_C(dtxchg_win_export_image,(ctl,	/* output image to new place 		*/
    dsk_server, windowname, &image,
    state.x0, state.y0));

cleanup:
  if (dsk_server != NULL)
    DT_I(dtos_free2,(ctl, dsk_server,
      F, "dsk_server"));

  if (rc != DT_RC_GOOD)
  {
	DT_I(dtstr_printf,(ctl,
      message, message_max,
      "\"%s\" not saved",
      windowname));
  }
  else
  {
	dtdsk_win_t win;
	char filename[128];

    DT_I(dtdsk_makename,(ctl,			/* filebase for the window data	    */
      dirname, DTDSK_FILEBASE, 
      windowname,
      win.filebase, 
      sizeof(win.filebase)));

    DT_I(dtdsk_makename,(ctl,			/* filename for the window data	    */
      dirname, DTDSK_FILENAME,
      win.filebase,
      filename, 
      sizeof(filename)));

	DT_I(dtstr_printf,(ctl,				/* make a pretty message 			*/
      message, message_max,
      "\"%s\" saved to %s",
      windowname, filename));
  }
  
  if (image.xe != 0)
  DT_I(dtimage_free,(ctl, &image));		/* get rid of temporary image 		*/
  
  return rc;
}

/*..........................................................................
| get data from all scr windows and output to dsk server
 *..........................................................................*/

dt_rc_e
dtview_save(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *dirname)
{
  DT_F("dtview_save");
  char *s = NULL;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "saving to dirname \"%s\"", 
    dirname? dirname: "NULL");

# define SMAX 2048
  DT_C(dtos_malloc2,(ctl,
    (void **)&s, SMAX,
    F, "panel string"));
  s[0] = '\0';							/* clear string 					*/
  
  if (dirname != NULL)
  {
	int isdir;
    DT_C(dtos_isdir,(ctl,
      dirname, &isdir));
    if (!isdir)
	{
      DT_C(dtstr_printf,(ctl,
        s+strlen(s), SMAX-strlen(s),
        "%s is not a directory:"
        " no windows saved\n", dirname));
	}
	else
	{
	  dtlist_elem_t *elem;
	  elem = DTLIST_ELEM_TAIL;			/* start at tail of list 			*/
	  do {								/* save all windows 				*/
		DT_C(dtlist_prev,(ctl,
	      &scr->windowlist,
          elem, &elem));
		if (elem != NULL)					/* not end of list?				    */
		if (strcmp(elem->name, DTVIEW_SAVE_TITLE) &&
			strcmp(elem->name, DTVIEW_HELP_TITLE) &&
			strcmp(elem->name, DTVIEW_MEMORY_TITLE) &&
			strcmp(elem->name, DTVIEW_INTRODUCE_TITLE))
		{
		  DT_C(dtview_save_one,(ctl, scr,	/* save this window 				*/
            elem->name, dirname,
            s+strlen(s), 
            SMAX-strlen(s)));
		  strcat(s, "\n");
		}
	  } while (elem != NULL);
	}

	{									/* delete all gone windows 			*/
    DT_C(dtview_dsk_delete,(ctl,		/* eradicate all old stuff 			*/
        scr, dirname));
	}
  }
  else									/* no output dirname name given? 	*/
    DT_C(dtstr_printf,(ctl,
      s+strlen(s), SMAX-strlen(s),
      "no -output param:"
      " no windows saved\n"));

  if (strlen(s) == 0)					/* no message output resulted? 		*/
    DT_C(dtstr_printf,(ctl,
      s+strlen(s), SMAX-strlen(s),
      "no windows to save"))
  else
    s[strlen(s)-1] = '\0';				/* nuke trailing newline 			*/

  DT_C(dtview_panel,(ctl, scr,			/* output informational panel 		*/
    DTVIEW_SAVE_TITLE,
    s,
    DTVIEW_SAVE_X0,
    DTVIEW_SAVE_Y0,
    DTVIEW_SAVE_GRAVITY));
  
cleanup:
  if (s != NULL)
    DT_I(dtos_free2,(ctl, s,
    F, "panel string"));
  return rc;
#undef SMAX
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
