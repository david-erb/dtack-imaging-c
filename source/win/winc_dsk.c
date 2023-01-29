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
| Called by dtwin_export_data and the others.
| Gets a client handle then calls the DSK-generated client-side functions.
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
DT_RCSID("base $RCSfile: winc_dsk.c,v $ $Revision: 1.1 $");

#include <dtack/win_r.h>					/* arg and ret structures 			*/
#include <dtack/winc.h>						/* client encodes arg structures 	*/
#include <dtack/list.h>						/* needed by dtdsk auxiliaries 		*/
#include <dtack/dsk.h>						/* where we encode arg to 			*/
#include <dtack/os.h>

/*..........................................................................*/
/*..........................................................................*/

#define PROLOG \
  dt_rc_e rc

#define EPILOG \
  *res = NULL; \
  return rc

/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_info(							/* get info on existing window	    */
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtdsk_win_t *win)
{
  DT_F("dtwinc_dsk_info");
  dtlist_t list;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  DT_Q(dtdsk_getlist,(ctl,				/* read window list				    */
    server, &list));

  DT_G(dtlist_find,(ctl, &list,			/* see if we have this window	    */
    id, &elem));
  if (elem != NULL)						/* we have it?					    */
    *win = *(dtdsk_win_t *)				/* return contents to caller	    */
      elem->user;

  DT_Q(dtlist_free,(ctl, &list));		/* free list elements			    */
  DT_Q(dtlist_destroy,(ctl, &list));	/* destroy list structure		    */

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_create_window(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_create_window_arg_t *argp,
  dtwin_r_create_window_ret_t **res)
{
  DT_F("dtwinc_dsk_create_window");
  PROLOG;

  const char *server = winc->server;
  const char *id = argp->name;
  int x0 = argp->x0;
  int y0 = argp->y0;
  int xe = argp->xe;
  int ye = argp->ye;
  int bits = argp->bits;

  char filename[128];
  dtfd_t fd;
  long offset;
  char c;
  const char *name;
  dtlist_t list;
  int cleanup_fd = 0;
  int cleanup_list = 0;
  int cleanup_new = 0;
  dtlist_elem_t *elem;
  dtdsk_win_t *newwin;

										/* ................................ */
  DT_C(dtdsk_getlist,(ctl,				/* load in the window list		    */
    server, &list));
  cleanup_list = 1;

  DT_C(dtlist_search,(ctl, &list,		/* see if we have this window	    */
    id, &elem));

  if (elem != NULL)						/* we have it?					    */
  {
    name = elem->name;					/* reuse its name space				*/
    newwin = (dtdsk_win_t *)elem->user;	/* reuse its data space				*/
    DT_C(dtlist_delete,(ctl, &list,		/* delete it from the list			*/
      elem));
  }
  else									/* not yet in list					*/
  {
    char *tmp;
    DT_C(dtos_malloc2,(ctl,				/* space for new name			    */
      (void **)&tmp,
      strlen(id)+1,
      F, "new elem->name"));
    strcpy(tmp, id);
    name = tmp;
    DT_C(dtos_malloc2,(ctl,				/* space for new element		    */
      (void **)&newwin,
      sizeof(dtdsk_win_t),
      F, "new elem->user"));

    DT_C(dtdsk_makename,(ctl,			/* filebase for the window data	    */
      server, DTDSK_FILEBASE, 
      id,
      newwin->filebase, 
      sizeof(newwin->filebase)));

    cleanup_new = 1;
  }
										/* ................................ */

  DT_C(dtdsk_makename,(ctl,				/* filename for the window data	    */
    server, DTDSK_FILENAME,
    newwin->filebase,
    filename, 
    sizeof(filename)));

  DT_C(dtfd_open,(ctl, &fd,				/* prepare to write zero data	    */
    filename, "w"));
  cleanup_fd = 1;

  offset = (long)xe * (long)ye;			/* how long the file will be	    */
  DT_C(dtfd_seek,(ctl, &fd,				/* seek before last byte		    */
    offset-1, DTFD_WHENCE_SET, NULL));
  c = 0;
  DT_C(dtfd_write_all,(ctl, &fd,		/* stretch out the file				*/
    &c, 1));

										/* ................................ */
  newwin->x0 = (dtdsk_int_t)x0;			/* make up the new elem			    */
  newwin->y0 = (dtdsk_int_t)y0;
  newwin->xe = (dtdsk_int_t)xe;
  newwin->ye = (dtdsk_int_t)ye;
  newwin->bits = (dtdsk_int_t)bits;

  DT_C(dtlist_after,(ctl, &list,		/* add new elem to head of list	    */
    DTLIST_ELEM_HEAD, 
    name, newwin, &elem));
  cleanup_new = 0;						/* will now be cleaned up with list	*/

  DT_C(dtdsk_putlist,(ctl,				/* put list back to disk		    */
    server, &list));

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "created window \"%s\" at (%d,%d) size (%d,%d) bits %d",
	id,
    x0, y0, xe, ye, bits);
  
cleanup:
  if (cleanup_new)
    DT_I(dtos_free2,(ctl,				/* free new element space			*/
      newwin, F,
      DTDSK_LIST_NAME 
      " new element"));

  if (cleanup_list)
    DT_I(dtlist_free,(ctl, &list));		/* free list elements			    */

  if (cleanup_list)
    DT_I(dtlist_destroy,(ctl, &list));	/* destroy list structure		    */

  if (cleanup_fd)
    DT_I(dtfd_close,(ctl, &fd));		/* close file					    */

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_export_data(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_export_data_arg_t *argp,
  dtwin_r_export_data_ret_t **res)
{
  DT_F("dtwinc_dsk_export_data");
  PROLOG;

  const char *server = winc->server;
  const char *id = argp->name;
  int x0 = argp->x0;
  int y0 = argp->y0;
  int xe = argp->xe;
  int ye = argp->ye;
  int bits = argp->bits;
  void *data = argp->d.d_val;

  dtdsk_win_t win;
  char filename[128];
  dtfd_t fd;
  int cleanup_fd = 0;
  long offset;
  dtxy_t y;

  if (bits != 8)
    return dt_err(ctl, F,
      "can only handle 8-bit data,"
      " not %d-bit", bits);

  DT_C(dtwinc_dsk_info,(ctl,			/* get info on existing window	    */
    server, id, &win));

  DT_C(dtdsk_makename,(ctl,				/* filename for the window data	    */
    server, DTDSK_FILENAME, 
    win.filebase,
    filename, 
    sizeof(filename)));

  DT_C(dtfd_open,(ctl, &fd,				/* open data file for update	    */
    filename, "rw"));
  cleanup_fd = 1;

  offset =(long)win.xe * (long)y0 +		/* calculate file offset		    */
    (long)x0;

  if (x0 == 0 &&						/* not writing patch in the middle?	*/
      xe == win.xe)
  {
    DT_C(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    DT_C(dtfd_write_all,(ctl, &fd,		/* write the whole thing		    */
      data, (long)xe * (long)ye));
  }
  else									/* writing patch in the middle?	    */
  for (y=0; y<ye; y++)					/* write row by row				    */
  {
    DT_C(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    DT_C(dtfd_write_all,(ctl, &fd,		/* write the row				    */
      data, xe));
    offset += (long)win.xe;
    data = (char *)data + 
      (unsigned int)xe;
  }

cleanup:
  if (cleanup_fd)
    DT_I(dtfd_close,(ctl, &fd));		/* close the data file			    */

  EPILOG;
}
/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_import_data(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_import_data_arg_t *argp,
  dtwin_r_import_data_ret_t **res)
{
  DT_F("dtwinc_dsk_import_data");
  PROLOG;

  const char *server = winc->server;
  const char *id = argp->name;
  int x0 = argp->x0;
  int y0 = argp->y0;
  int xe = argp->xe;
  int ye = argp->ye;
  void *data = NULL;
  long length =(long)xe * (long)ye;

  dtdsk_win_t win;
  char filename[128];
  dtfd_t fd;
  int cleanup_fd = 0;
  long offset;
  dtxy_t y;

  DT_C(dtwinc_dsk_info,(ctl,			/* get info on existing window	    */
    server, id, &win));

  DT_C(dtdsk_makename,(ctl,				/* filename for the window data	    */
    server, DTDSK_FILENAME,
    win.filebase,
    filename, 
    sizeof(filename)));

  DT_C(dtfd_open,(ctl, &fd,				/* open the data file to read	    */
    filename, "r"));
  cleanup_fd = 1;

  DT_C(dtos_malloc2,(ctl,				/* space for result data 			*/
    (void **)&data, length,
    F, "result data"));

  offset = (long)win.xe * (long)y0 +	/* calculate file offset		    */
    (long)x0;

  if (x0 == 0 &&						/* not reading patch in the middle?	*/
      xe == win.xe)
  {
    DT_C(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    DT_C(dtfd_read_all,(ctl, &fd,		/* write the whole thing		    */
      data, (long)xe * (long)ye));
  }
  else									/* reading patch in the middle?	    */
  for (y=0; y<ye; y++)					/* read row by row				    */
  {
    DT_C(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    DT_C(dtfd_read_all,(ctl, &fd,		/* read the row					    */
      data, xe));
    offset += (long)win.xe;
    data = (char *)data + 
      (unsigned int)xe;
  }

cleanup:
  if (cleanup_fd)
    DT_I(dtfd_close,(ctl, &fd));		/* close the data file			    */

  if (rc == DT_RC_GOOD)					/* no problem so far? 				*/
  {
    DT_G(dtos_malloc2,(ctl,				/* space to return structure 		*/
      (void **)res, sizeof(**res),
      F, "result structure"));
	if (rc == DT_RC_GOOD)				/* could not get structure space? 	*/
	{
      DT_MEMSET(*res, 0, sizeof(**res));
      (*res)->d.d_len = length;
      (*res)->d.d_val = (char *)data;
	}
  }

  if (rc != DT_RC_GOOD &&				/* something bad happened? 			*/
      data != NULL)						/* after data allocated? 			*/
    DT_I(dtos_free2,(ctl, data,
      F, "result data"));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_export_color(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_export_color_arg_t *argp,
  dtwin_r_export_color_ret_t **res)
{
  PROLOG;

  rc = DT_RC_GOOD;

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_import_color(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_import_color_arg_t *argp,
  dtwin_r_import_color_ret_t **res)
{
  PROLOG;

  rc = DT_RC_GOOD;

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_dsk_query(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_query_arg_t *argp,
  dtwin_r_query_ret_t **res)
{
  DT_F("dtwinc_dsk_query");
  PROLOG;

  const char *server = winc->server;
  const char *id = argp->name;

  dtdsk_win_t win;

  rc = DT_RC_GOOD;
  
  DT_Q(dtwinc_dsk_info,(ctl,			/* get info on existing window	    */
    server, id, &win));

  DT_Q(dtos_malloc2,(ctl,				/* space to return structure 		*/
    (void **)res, sizeof(**res),
    F, "result structure"));

  DT_MEMSET(*res, 0, sizeof(**res));		/* clear return structure 			*/
  (*res)->state.xe = win.xe;
  (*res)->state.ye = win.ye;
  (*res)->state.bits = win.bits;
  
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtwinc_dsk_init(
  dt_ctl_t *ctl,
  dtwinc_t *winc)
{
  DT_F("dtwinc_dsk_init");
  if (!strlen(winc->server))			/* server name not specified?	  	*/
    return dt_err(ctl, F,
      "empty server name");
  dt_dbg(ctl, F, DT_DBG_MASK_WIN, 
    "initializing client for \"%s\"", 
    winc->server);
  winc->create_window = dtwinc_dsk_create_window;
  winc->export_data   = dtwinc_dsk_export_data;
  winc->export_color  = dtwinc_dsk_export_color;
  winc->import_data   = dtwinc_dsk_import_data;
  winc->import_color  = dtwinc_dsk_import_color;
  winc->query         = dtwinc_dsk_query;
  winc->recv_max      = DTCONFIG_MALLOC_MAX;
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
