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
DT_RCSID("source/screen $RCSfile: canhws.c,v $ $Revision: 1.7 $");
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>
#include <dtack/hws.h>

#include <dtack/str.h>

typedef struct {
  dthws_t hws;							/* connection to hws server		    */
  dthws_win_t win;						/* canvas is just single hws win    */
} dtcanvas_hws_priv_t;

extern dtcanvas_init_f dtcanvas_hws_init;
static dtcanvas_uninit_f dtcanvas_hws_uninit;
static dtcanvas_open_f dtcanvas_hws_open;
static dtcanvas_input_f dtcanvas_hws_input;
static dtcanvas_close_f dtcanvas_hws_close;
static dtcanvas_putblock_f dtcanvas_hws_putblock;
static dtcanvas_getblock_f dtcanvas_hws_getblock;
static dtcanvas_putcolor_f dtcanvas_hws_putcolor;
static dtcanvas_getcolor_f dtcanvas_hws_getcolor;
static dtcanvas_cursor_f dtcanvas_hws_cursor;
static dtcanvas_clear_f dtcanvas_hws_clear;

#define DESC "X Windows"

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_hws_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *server)
{
  DT_F("dtcanvas_hws_init");
  dtcanvas_hws_priv_t *priv;
  int x0 = 0;							/* default canvas position		    */
  int y0 = 0;
  int xe = 800;							/* default canvas size			    */
  int ye = 600;
  int allocated = 0;
  int connected = 0;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl, 
    (void **)&priv,
    sizeof(dtcanvas_hws_priv_t),
    F, "priv"));  
  allocated = 1;

  DT_C(dthws_connect,(ctl,				/* connect to hws server           	*/
    &priv->hws, server));
  connected = 1;

  DT_C(dthws_win_create,(ctl,			/* make the big canvas window		*/
    &priv->hws, &priv->win, 
    strlen(server)? server: 
      "dtcanvas_hws",
    x0, y0, xe, ye, 8));

  canvas->must_remap = 0;
  canvas->xe = xe;						/* remember canvas dimensions	    */
  canvas->ye = ye;
  canvas->priv  = priv;
  canvas->init     = dtcanvas_hws_init;
  canvas->uninit   = dtcanvas_hws_uninit;
  canvas->open     = dtcanvas_hws_open;
  canvas->input    = dtcanvas_hws_input;
  canvas->close    = dtcanvas_hws_close;
  canvas->putblock = dtcanvas_hws_putblock;
  canvas->getblock = dtcanvas_hws_getblock;
  canvas->putcolor = dtcanvas_hws_putcolor;
  canvas->getcolor = dtcanvas_hws_getcolor;
  canvas->cursor   = dtcanvas_hws_cursor;
  canvas->clear    = dtcanvas_hws_clear;
  return DT_RC_GOOD;

cleanup:

  if (connected)
  DT_G(dthws_disconnect,(ctl,			/* disconnect from hws server     	*/
    &priv->hws));

  if (allocated)
  DT_G(dtos_free2,(ctl,
    priv, F, "priv"));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_uninit(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_hws_uninit");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_win_destroy,(ctl,			/* destroy window					*/
    &priv->hws, &priv->win));

  DT_Q(dthws_disconnect,(ctl,			/* tear down connection to server	*/
    &priv->hws));

  DT_Q(dtos_free2,(ctl, 
    canvas->priv,
    F, "priv"));  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_open(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_input(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  DT_F("dtcanvas_hws_input");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;
  dt_event_t local;

  if (event == NULL)
    event = &local;
  DT_Q(dthws_wait_event,(ctl,			/* get input from hws window		*/
    &priv->hws,
    svc, timeout, flag,
    event));							/* event report returned		    */

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_close(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_putblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_hws_putblock");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_export_data,(ctl,
    &priv->hws, &priv->win,
    data, x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_getblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  DT_F("dtcanvas_hws_getblock");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_import_data,(ctl,
    &priv->hws, &priv->win,
    data, x0, y0, xe, ye));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_putcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_hws_putcolor");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_export_color,(ctl,
    &priv->hws,
    &priv->win,
    lut, start, count));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_getcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtcanvas_hws_getcolor");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_import_color,(ctl,			/* get hws's root colormap			*/
    &priv->hws, NULL,
    lut, start, count));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_cursor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int op)
{
  DT_F("dtcanvas_hws_cursor");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_win_cursor,(ctl,
    &priv->hws, &priv->win,
    x, y, op));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_hws_clear(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int background)
{
  DT_F("dtcanvas_hws_clear");
  dtcanvas_hws_priv_t *priv =
    (dtcanvas_hws_priv_t *)canvas->priv;

  DT_Q(dthws_win_clear,(ctl,
    &priv->hws, &priv->win,
    background));

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
