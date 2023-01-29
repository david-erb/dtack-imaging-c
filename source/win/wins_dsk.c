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
| Called by dsk server.
| Inputs args from dtipc_recv and deserializes them.
| Calls dtwin_svc function with argp and res structures.
| Serializes results out to dtipc_send.
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
#include <dtack/os.h>

DT_RCSID("base $RCSfile: wins_dsk.c,v $ $Revision: 1.1 $");

#include <dtack/win_r.h>
#include <dtack/lut8.h>						/* to satisfy dtscr.h only		    */
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/dsk.h>
#include <dtack/wins.h>

/*..........................................................................*/
static
dt_rc_e
dtwins_dsk_one(
  dt_ctl_t *ctl,
  const char *server,					/* our name						    */
  const char *name,						/* window name from list		    */
  dtdsk_win_t *window,					/* window data from list		    */
  dtscr_t *scr,							/* opened connection to screen	    */
  void *buf,							/* buffer to use				    */
  long bufsize)
{
  DT_F("dtwins_dsk_one");
  char filename[128];
  int isreadable;
  dtfd_t fd;
  dtxy_t y;
  dtxy_t bufrows;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dtdsk_makename,(ctl,				/* filename for the window data	    */
    server, DTDSK_FILENAME, 
    window->filebase, 
    filename, 
    sizeof(filename)));

  DT_Q(dtos_isreadable,(ctl,			/* see if window file exists   		*/
    filename, &isreadable));
  if (!isreadable)						/* file not there? 					*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "window \"%s\" from base %s, file %s"
      " does not exist",
      name, window->filebase, filename);
	return DT_RC_GOOD;					/* don't worry about it 			*/
  }

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "window \"%s\" from base %s, file %s",
    name, window->filebase, filename);

  DT_Q(dtscr_create_window,(ctl,		/* make the window on screen	    */
    scr, name,
    (int)window->x0, (int)window->y0,
    (int)window->xe, (int)window->ye,
    (int)window->bits));

  DT_Q(dtfd_open,(ctl, &fd,				/* prepare to read the data			*/
    filename, "r"));

  bufrows = (dtxy_t)
    (bufsize / (int)window->xe);		/* rows in one buffer			    */
  if (bufrows == 0)
    rc = dt_err(ctl, F,
      "row size %ld too big"
      " for buffer size %ld",
      window->xe, bufsize);

  y = 0;
  if (rc == DT_RC_GOOD)
  do {
    if (y+bufrows > window->ye)			/* last buffer may be less		    */
      bufrows = (dtxy_t)window->ye-y;
    DT_G(dtfd_read_all,(ctl, &fd,		/* read buffer from file		    */
      buf, bufrows * (int)window->xe));
    DT_GI(dtscr_export_data,(ctl,		/* make the window on screen	    */
      scr, name,
      0, y, (int)window->xe, bufrows,
      (int)window->bits, buf));
    y += bufrows;
  } while (rc == DT_RC_GOOD &&
           y < window->ye);

  DT_Q(dtfd_close,(ctl, &fd));			/* close file					    */
    
  return rc;
}

/*..........................................................................
| Get next window name from disk list and display it with dtscr.
| Disk list pointer is set to start of list by dtsvc_dsk_register().
| At end of list, an empty window name is returned.
| It is an error to call this function with an ~*svc~ structure which
| was not initialized by dtsvc_dsk_register().
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwins_dsk_dispatch(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtsvc_t *svc)
{
  DT_F("dtwins_dsk_dispatch");
  dtlist_elem_t *elem;
# define BUFSIZE 8192
  void *buf;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "dispatching");

  DT_Q(dtsvc_dsk_next,(ctl, svc,		/* fetch next window from list 		*/
    &elem));

  if (elem == NULL)						/* list has been traversed? 		*/
    return dt_err(ctl, F,
      "no windows to dispatch");

  DT_Q(dtos_malloc2,(ctl,				/* space for transfer buffer 		*/
    (void **)&buf, BUFSIZE,
    F, "dsk-to-scr transfer buffer"));

  DT_G(dtwins_dsk_one,(ctl,				/* read window and put on screen 	*/
    svc->server,
    elem->name,
    (dtdsk_win_t *)elem->user,
    wins->scr,
    buf, BUFSIZE));

  DT_I(dtos_free2,(ctl, buf,
    F, "dsk-to-scr transfer buffer"));

  return rc;
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
