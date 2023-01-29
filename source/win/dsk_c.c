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
| Implement the client-side calls on top of ipc calls.
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

DT_RCSID("source/low $RCSfile: dsk_c.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/dsk.h>						/* objects shared with server	    */
#include <dtack/dsk_c.h>

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdsk_getwin(							/* get info on existing window	    */
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtdsk_win_t *win)
{
  DT_F("dtdsk_getwin");
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
dt_rc_e
DTCONFIG_API1
dtdsk_clnt_create_window(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits)
{
  DT_F("dtdsk_clnt_create_window");
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
  dt_rc_e rc, retrc;

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
  retrc = rc;

  if (cleanup_new)
    DT_G(dtos_free2,(ctl,				/* free new element space			*/
      newwin, F,
      DTDSK_LIST_NAME 
      " new element"));

  if (cleanup_list)
    DT_G(dtlist_free,(ctl, &list));		/* free list elements			    */
  if (retrc == DT_RC_GOOD)
    retrc = rc;

  if (cleanup_list)
    DT_G(dtlist_destroy,(ctl, &list));	/* destroy list structure		    */
  if (retrc == DT_RC_GOOD)
    retrc = rc;

  if (cleanup_fd)
    DT_G(dtfd_close,(ctl, &fd));		/* close file					    */
  if (retrc == DT_RC_GOOD)
    retrc = rc;

  return retrc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdsk_clnt_export_data(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  int x0,
  int y0,
  int xe,
  int ye,
  int bits,
  const void *data)
{
  DT_F("dtdsk_clnt_export_data");
  dtdsk_win_t win;
  char filename[128];
  dtfd_t fd;
  long offset;
  dtxy_t y;
  dt_rc_e rc, retrc;

  DT_Q(dtdsk_getwin,(ctl,				/* get info on existing window	    */
    server, id, &win));

  DT_Q(dtdsk_makename,(ctl,				/* filename for the window data	    */
    server, DTDSK_FILENAME, 
    win.filebase,
    filename, 
    sizeof(filename)));

  DT_Q(dtfd_open,(ctl, &fd,				/* open data file for update	    */
    filename, "rw"));

  offset =(long)win.xe * (long)y0 +		/* calculate file offset		    */
    (long)x0;

  if (x0 == 0 &&						/* not writing patch in the middle?	*/
      xe == win.xe)
  {
    DT_G(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    if (rc == DT_RC_GOOD)
    DT_G(dtfd_write_all,(ctl, &fd,		/* write the whole thing		    */
      data, (long)xe * (long)ye));
  }
  else									/* writing patch in the middle?	    */
  for (y=0; y<ye; y++)					/* write row by row				    */
  {
    DT_G(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    if (rc != DT_RC_GOOD)
      break;
    DT_G(dtfd_write_all,(ctl, &fd,		/* write the row				    */
      data, xe));
    if (rc != DT_RC_GOOD)
      break;
    offset += (long)win.xe;
    data = (char *)data + 
      (unsigned int)xe;
  }

  retrc = rc;
  DT_G(dtfd_close,(ctl, &fd));			/* close the data file			    */

  if (retrc == DT_RC_GOOD)
    retrc = rc;

  return retrc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdsk_clnt_import_data(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  int x0,
  int y0,
  int xe,
  int ye,
  void *data)
{
  DT_F("dtdsk_clnt_import_data");
  dtdsk_win_t win;
  char filename[128];
  dtfd_t fd;
  long offset;
  dtxy_t y;
  dt_rc_e rc, retrc;

  DT_Q(dtdsk_getwin,(ctl,				/* get info on existing window	    */
    server, id, &win));

  DT_Q(dtdsk_makename,(ctl,				/* filename for the window data	    */
    server, DTDSK_FILENAME,
    win.filebase,
    filename, 
    sizeof(filename)));

  DT_Q(dtfd_open,(ctl, &fd,				/* open the data file to read	    */
    filename, "r"));

  offset = (long)win.xe * (long)y0 +	/* calculate file offset		    */
    (long)x0;

  if (x0 == 0 &&						/* not reading patch in the middle?	*/
      xe == win.xe)
  {
    DT_G(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    if (rc == DT_RC_GOOD)
    DT_G(dtfd_read_all,(ctl, &fd,		/* write the whole thing		    */
      data, (long)xe * (long)ye));
  }
  else									/* reading patch in the middle?	    */
  for (y=0; y<ye; y++)					/* read row by row				    */
  {
    DT_G(dtfd_seek,(ctl, &fd,			/* seek to start of row			    */
      offset, DTFD_WHENCE_SET, NULL));
    if (rc != DT_RC_GOOD)
      break;
    DT_G(dtfd_read_all,(ctl, &fd,		/* read the row					    */
      data, xe));
    if (rc != DT_RC_GOOD)
      break;
    offset += (long)win.xe;
    data = (char *)data + 
      (unsigned int)xe;
  }

  retrc = rc;
  DT_G(dtfd_close,(ctl, &fd));			/* close the data file			    */

  if (retrc == DT_RC_GOOD)
    retrc = rc;

  return retrc;
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
