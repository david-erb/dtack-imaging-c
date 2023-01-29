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
DT_RCSID("source/screen $RCSfile: canvas.c,v $ $Revision: 1.7 $");
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/lut8.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>

static const char *choices[] = {
  "hws",
  "fastgraph",
  "svgacc",
  "dummy",
  NULL
};

static dtcanvas_init_f *methods[] = {
  dtcanvas_hws_init,
  dtcanvas_fastgraph_init,
  dtcanvas_svgacc_init,
  dtcanvas_dummy_init
};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *server)
{
  DT_F("dtcanvas_init");
  char *driver;
  int choice;
  dt_rc_e rc;

  if (server == NULL)
    server = "";

  DT_Q(dtos_strdup,(ctl, server,
    &server));

  driver = strchr(server, ',');			/* driver part after server		    */
  if (driver)
    *driver++ = '\0';
  else
    driver = "";

  DT_C(dtstr_choice,(ctl, choices,		/* look up canvas type			    */
    server, &choice));

  if (choice == -1)						/* unknown type					    */
  {
    rc = dt_err(ctl, F,
      "unknown canvas spec \"%s\"",
      server);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous canvas spec \"%s\"",
      server);
	goto cleanup;
  }

  DT_MEMSET(canvas, 0, sizeof(*canvas));	/* clear structure				    */

  DT_C((methods[choice]),(ctl,			/* chop off the canvas spec			*/
    canvas, driver))

  canvas->server = server;
  canvas->driver = driver;

  return DT_RC_GOOD;
  
cleanup:
  DT_I(dtos_free2,(ctl, server,
    F, "server"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_uninit(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->uninit)
    DT_Q((canvas->uninit),(ctl, 
      canvas));
  if (canvas->server)
    DT_Q(dtos_free,(ctl,				/* free parse string			    */ 
      canvas->server));
  DT_MEMSET(canvas, 0, sizeof(*canvas));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_open(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_open");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->open)
  DT_Q((canvas->open),(ctl, canvas));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_input(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dtcanvas_input");
  dt_rc_e rc = DT_RC_GOOD;
  if (event != NULL)
  {
    DT_MEMSET(event, 0, sizeof(*event));	/* clear event structure 			*/
    if (canvas->input)					/* we have an input method? 		*/
	  DT_Q((canvas->input),(ctl,		/* go get the input 				*/
        canvas, svc, timeout, 
        flag, event));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_close(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_close");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->close)
  DT_Q((canvas->close),(ctl, canvas));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_putblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_putblock");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->putblock)
  DT_Q((canvas->putblock),(ctl, canvas, data, x0, y0, xe, ye));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_getblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_getblock");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->getblock)
  DT_Q((canvas->getblock),(ctl, canvas, data, x0, y0, xe, ye));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_putcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_putcolor");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->putcolor)
  DT_Q((canvas->putcolor),(ctl, canvas, lut, start, count));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_getcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_getcolor");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->getcolor)
  DT_Q((canvas->getcolor),(ctl, canvas, lut, start, count));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_cursor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int op)
{
  DT_F("dtcanvas_cursor");
  if (canvas->cursor)
  DT_Q((canvas->cursor),(ctl, canvas, x, y, op));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_clear(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int background)
{
  DT_F("dtcanvas_clear");
  dt_rc_e rc = DT_RC_GOOD;
  if (canvas->clear)
  DT_Q((canvas->clear),(ctl, canvas, background));
  return rc;
}

/*..........................................................................
 * returns overlap rectangle 
 * destination (0,0,canvas->xe,canvas->ye)
 * source (x0,y0,xe,ye)
 * overlap (cx0,cy0,cxe,cye)
 * if either cxe or cye is less then there is no overlap
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_clip(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
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
  dt_rc_e rc = DT_RC_GOOD;

  if (y0 < 0)							/* clip top of image?			    */
  {
    data = (char *)data - y0 * xe;
    ye += y0;
    y0 = 0;
  }

  if (y0+ye > canvas->ye)				/* clip bottom of image?		    */
    ye = canvas->ye - y0;

  if (x0 < 0)							/* clip left of image?			    */
  {
	data = (char *)data - x0;
	xe += x0;
	x0 = 0;
  }

  if (x0+xe > canvas->xe)				/* clip right of image?			    */
    xe = canvas->xe - x0;

  *cdata = data;
  *cx0 = x0;
  *cy0 = y0;
  *cxe = xe;
  *cye = ye;

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
