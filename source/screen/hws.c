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
DT_RCSID("source/screen $RCSfile: hws.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/hws.h>

#include <dtack/pnm.h>
#include <dtack/str.h>


#include <dtack/mem.h>

static const char *choices[] = {
  "x",
  "qw4",
  "fgw",
  "msw",
  NULL
};

static dthws_connect_f *methods[] = {
  dthws_x_connect,
  dthws_qw4_connect,
  dthws_fgw_connect,
  dthws_msw_connect,
  NULL
};

#define DATA_DESC "bitmap"

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_connect(
  dt_ctl_t *ctl,
  dthws_t *hws,
  char *init)
{
  DT_F("dthws_connect");
  char hwstype[64];
  char *server;
  int choice;

  if (init == NULL)
    init = "";

  if (strlen(init) > sizeof(hwstype)-1)
    return dt_err(ctl, F,
      "\"%s\" too long", init);

  strcpy(hwstype, init);				/* string we will parse				*/

  server = strchr(hwstype,				/* get server part					*/
    DT_SEPARATOR_SPEC);
  if (server == NULL)
    server = "";
  else
    *server++ = '\0';

  DT_Q(dtstr_choice,(ctl, choices,		/* check out hws choices         	*/
    hwstype, &choice));
  if (choice == -1)						/* unknown choice?					*/
    return dt_err(ctl, F,
      "unknown hws type \"%s\"",
      hwstype);
  else
  if (choice < -1)						/* ambiguous choice?				*/
    return dt_err(ctl, F,
      "ambiguous hws type \"%s\"",
      hwstype);

  DT_Q((methods[choice]),(ctl,			/* connect to hws server           	*/
    hws, server));

  DT_Q(dtlist_create,(ctl,              /* create window list               */
    &hws->windowlist,
    "window list"));

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "@0x%08lx hws->windowlist.n=%d m=%d",
    hws, hws->windowlist.n,
    hws->windowlist.m);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_disconnect(
  dt_ctl_t *ctl,
  dthws_t *hws)
{
  DT_F("dthws_disconnect");
  dtlist_elem_t *elem;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "@0x%08lx hws->windowlist.n=%d m=%d",
    hws, hws->windowlist.n,
    hws->windowlist.m);

  do {
    DT_Q(dtlist_next,(ctl,              /* fetch next window in list        */
      &hws->windowlist,
      DTLIST_ELEM_HEAD, &elem));
    if (elem)                           /* not the last in the list?        */
      DT_Q(dthws_win_destroy,(        	/* free it                          */
        ctl, hws,
        (dthws_win_t *)elem->user));
  } while(elem);

  DT_Q(dtlist_destroy,(ctl, 			/* free window list space			*/
    &hws->windowlist));

  if (hws->disconnect)
  DT_Q(hws->disconnect,(ctl, hws));		/* close server connection			*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * does backing bitmap to keep input requests quick
 * presumes all hws will want a backing pixmap
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_create(						/* make/display a new window		*/
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
  DT_F("dthws_win_create");
  dt_rc_e rc;

  DT_MEMSET(hws_win, 0, sizeof(*hws_win));	/* clear out the structure			*/

  DT_Q(dtmem_alloc,(ctl,				/* space for backing bitmap         */
    &hws_win->memalloc, 0, 1, xe, ye,
    F, DATA_DESC));

  if (hws->win_create)
  DT_C(hws->win_create,(ctl, hws,		/* do method					    */
    hws_win, windowname,
    x0, y0, xe, ye, bits));

  DT_C(dtlist_after,(ctl,               /* put window name in list          */
    &hws->windowlist,
    DTLIST_ELEM_HEAD,
    windowname,
    (void *)hws_win, NULL));

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "added window \"%s\""
    " elem->user=0x%08lx",
    windowname, (void *)hws_win);

  return DT_RC_GOOD;

cleanup:
  DT_I(dtmem_free,(ctl,
    hws_win->memalloc, F, DATA_DESC));

  DT_MEMSET(hws_win, 0, sizeof(*hws_win));	/* clear out the structure			*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_wait_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dthws_wait_event");

  if (hws->wait_event)
  DT_Q(hws->wait_event,(ctl, hws,		/* do the method				    */
    svc, timeout, flag, event));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_handle_event(						/* hold window until user interacts	*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  void *buffer,
  int *handled,
  dt_event_t *event)
{
  DT_F("dthws_handle_event");

  if (hws->handle_event)
  DT_Q(hws->handle_event,(ctl, hws,		/* do the method				    */
    buffer, handled, event));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_destroy(						/* get rid of window			    */
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
{
  DT_F("dthws_win_destroy");
  dtlist_elem_t *elem;

  elem = DTLIST_ELEM_HEAD;
  while (elem)
  {
    DT_Q(dtlist_next,(ctl,            	/* fetch next window in list        */
      &hws->windowlist, elem,
      &elem));
    if (elem &&                        	/* not the last in the list?        */
        elem->user == win)
        break;
  }

  if (hws->win_destroy)
  DT_Q(hws->win_destroy,(ctl, hws,		/* do the method				  	*/
    win));

  if (win->memalloc != NULL)
  DT_Q(dtmem_free,(ctl,               	/* free data ourselves              */
    win->memalloc, F, DATA_DESC));

  if (elem) 			                /* found the window?          		*/
    DT_Q(dtlist_delete,(ctl,            /* get it out of the window list    */
      &hws->windowlist, elem))
  else
    return dt_err(ctl, F,
      "could not find window");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_export_data(						/* write window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_export_data");

  if (hws->do_ymirror == 0)				/* not supposed to mirror on y? 	*/
  {
    DT_Q(dtmem_putpatch,(ctl,			/* copy patch into backing bitmap	*/
      hws_win->memalloc,
      data, x0, y0, xe, ye));
  }
  else									/* supposed to mirror on y? 		*/
  {
	char *d = (char *)data;				/* the new data					    */
	int y;

	for (y=0; y<ye; y++)				/* for each row 					*/
	{
      DT_Q(dtmem_putpatch,(ctl,			/* copy row into backing bitmap		*/
        hws_win->memalloc,
        d, x0, y0+ye-y-1, xe, 1));
	  d += xe;
	}

#   ifdef NONO
	char *b = (char *)hws_win->data +
      (y0 + ye - 1) * xe + x0;

	for (y=0; y<ye; y++)				/* copy to backing store 			*/
	{
	  DT_MEMCPY(b, d, xe);
	  b -= hws_win->xe;
	  d += xe;
	}
#   endif
  }

  if (hws->export_data)
  DT_Q(hws->export_data,(ctl, hws,		/* do the method				    */
    hws_win, data,
    x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_import_data(						/* get window's data				*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dthws_import_data");

  if (hws->import_data)
  DT_Q(hws->import_data,(ctl, hws,		/* do the method				    */
    hws_win, data,
    x0, y0, xe, ye));

  DT_Q(dtmem_getpatch,(ctl,             /* copy patch from backing bitmap	*/
    hws_win->memalloc,
    data, x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_export_color(						/* write window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dthws_export_color");

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  if (hws->export_color)
  DT_Q(hws->export_color,(ctl, hws,
    win, lut,
    start, count));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_import_color(						/* get window's colormap			*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dthws_import_color");

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  if (hws->import_color)
  DT_Q(hws->import_color,(ctl, hws,
    win, lut,
    start, count));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_clear(						/* clear window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  int background)
{
  DT_F("dthws_win_clear");

  DT_Q(dtmem_set,(ctl,  				/* set bitmap memory to constant 	*/
    win->memalloc, 
    (char)background));

  if (hws->win_clear)
  DT_Q(hws->win_clear,(ctl, hws,
    win, background));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_raise(						/* raise window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
{
  DT_F("dthws_win_raise");

  if (hws->win_raise)
  DT_Q(hws->win_raise,(ctl, hws,
    win));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_lower(						/* lower window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
{
  DT_F("dthws_win_lower");

  if (hws->win_lower)
  DT_Q(hws->win_lower,(ctl, hws,
    win));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_print(						/* print window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *hws_win,
  const char *printer,					/* OS-specific printer string		*/
  double ppm,							/* pixels per millimeter 			*/
  int *submitted)						/* return true if submitted 		*/
{
  DT_F("dthws_win_print");
  dtfd_t fd;
  dt_rc_e rc = DT_RC_GOOD;
  
  fd.fd = -1;

  if (hws->win_print)					/* underlying layer wants to do it?	*/
  {
	DT_Q(hws->win_print,(ctl, hws,
      hws_win, printer, ppm, 
      submitted));
  }
# ifdef DTCONFIG_IMPLIED_UNIXLIKE
  else
  {
	char filename[512];
	char command[600];
	dtpnm_t pnm;
	long bytes;
	int sysrc;

	*submitted = 0;

	if (printer == NULL)
	  return dt_err(ctl, F,
        "printer string"
        " (i.e. script name)"
        " is NULL");

	DT_C(dtstr_printf,(ctl,
       filename, sizeof( filename),
      "/tmp/dtprint.%05d.%08lx.%03d.pgm",
      getpid(), (unsigned long)hws,
      hws->nprint++));
	DT_C(dtfd_open,(ctl, &fd,			/* open file for output 			*/
      filename, "w"));

	DT_MEMSET(&pnm, 0, sizeof(pnm));		/* clear pnm structure 				*/

	pnm.magic = DTPNM_MAGIC_PGM_RAW;	/* load fields of pnm structure 	*/
	pnm.xe = hws_win->xe;
	pnm.ye = hws_win->ye;
	pnm.ntype = hws_win->bits == 1? 
      DTNTYPE_B01: DTNTYPE_B08;

	DT_C(dtpnm_write_header,(ctl,		/* write pnm header to opened file	*/
      &pnm, &fd));

	bytes = (long)(
      (hws_win->xe * hws_win->ye) /
      dt_ntype_sizeof[pnm.ntype]);
	DT_C(dtmem_write,(ctl,				/* write backing store 				*/
      hws_win->memalloc,				/* memalloc to write 				*/
      0, bytes,							/* offset and size 					*/
      &fd));
	
	DT_C(dtfd_close,(ctl, &fd));		/* close completed file 			*/
	fd.fd = -1;

	DT_C(dtstr_printf,(ctl,				/* submit print job 				*/
      command, sizeof(command),
      "%s %s %d %d %g &",
      printer, filename,
      hws_win->xe, hws_win->ye,
      ppm));

	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "submitting \"%s\"", command);
	sysrc = system(command);
	if (sysrc != 0)
	{
	  rc = dt_err(ctl, F,
        "system(\"%s\") had sysrc=%d (0x%08lx)",
		command, sysrc, (long)sysrc);
	  goto cleanup;
	}

	*submitted = 1;
  }
# endif

cleanup:
  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));
  
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_iconify(						/* iconify window					*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win)
{
  DT_F("dthws_win_iconify");

  if (hws->win_iconify)
  DT_Q(hws->win_iconify,(ctl, hws,
    win));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_move(							/* move window						*/
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  int x,
  int y)
{
  DT_F("dthws_win_move");

  if (hws->win_move)
  DT_Q(hws->win_move,(ctl, hws,
    win, x, y));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dthws_win_cursor(
  dt_ctl_t *ctl,
  dthws_t *hws,
  dthws_win_t *win,
  int *x,
  int *y,
  int op)
{
  DT_F("dthws_cursor");
  if (hws->win_cursor)
  DT_Q((hws->win_cursor),(ctl, hws, win, x, y, op));
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
