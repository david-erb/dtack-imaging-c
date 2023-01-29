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
DT_RCSID("source/screen $RCSfile: candum.c,v $ $Revision: 1.7 $");
#include <dtack/lut8.h>
#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>

typedef struct {
  int dummy;
} dtcanvas_dummy_priv_t;

#define XX_X(NAME) DT_F(NAME); DTCSTACK_HIGHWATER

extern dtcanvas_init_f dtcanvas_dummy_init;
static dtcanvas_uninit_f dtcanvas_dummy_uninit;
static dtcanvas_open_f dtcanvas_dummy_open;
static dtcanvas_input_f dtcanvas_dummy_input;
static dtcanvas_close_f dtcanvas_dummy_close;
static dtcanvas_putblock_f dtcanvas_dummy_putblock;
static dtcanvas_getblock_f dtcanvas_dummy_getblock;
static dtcanvas_putcolor_f dtcanvas_dummy_putcolor;
static dtcanvas_getcolor_f dtcanvas_dummy_getcolor;
static dtcanvas_clear_f dtcanvas_dummy_clear;
static dtcanvas_cursor_f dtcanvas_dummy_cursor;

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcanvas_dummy_init(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *init)
{
  DT_F("dtcanvas_dummy_init");
  DTCSTACK_HIGHWATER;
  DT_Q(dtos_malloc2,(ctl, 
    &canvas->priv,
    sizeof(dtcanvas_dummy_priv_t),
    F, "priv"));  

  canvas->xe = 800;
  canvas->ye = 600;

  canvas->init     = dtcanvas_dummy_init;
  canvas->uninit   = dtcanvas_dummy_uninit;
  canvas->open     = dtcanvas_dummy_open;
  canvas->input    = dtcanvas_dummy_input;
  canvas->close    = dtcanvas_dummy_close;
  canvas->putblock = dtcanvas_dummy_putblock;
  canvas->getblock = dtcanvas_dummy_getblock;
  canvas->putcolor = dtcanvas_dummy_putcolor;
  canvas->getcolor = dtcanvas_dummy_getcolor;
  canvas->clear    = dtcanvas_dummy_clear;
  canvas->cursor   = dtcanvas_dummy_cursor;

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_uninit(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  DT_F("dtcanvas_dummy_uninit");
  DT_Q(dtos_free2,(ctl, 
    canvas->priv,
    F, "priv"));  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_open(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  XX_X("dtcanvas_dummy_open");
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_clear(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int background)
{
  XX_X("dtcanvas_dummy_clear");
  return DT_RC_GOOD;
}

/*..........................................................................
 * this dummy "wait event" always returns the control-q key to satisfy
 *   servers which may otherwise wait for real interactive input
 * this may not work if the server does not expect control-q
 * it might be better if this routine could somehow signal "end"
 *   without needing a specific key
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_input(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtsvc_t *svc,
  double timeout,
  int flag,
  dt_event_t *event)
{
  XX_X("dtcanvas_dummy_input");

  if (event != NULL)
  {
    DT_MEMSET(event, 0, sizeof(*event));
    if (timeout != -1)					/* this is not a poll? 				*/
    {
      event->type = DT_EVENT_KEYBOARD;
      event->keys[0] = 0x11;			/* control-Q 						*/
      event->nkeys = 1;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_close(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas)
{
  XX_X("dtcanvas_dummy_close");
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_putblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  XX_X("dtcanvas_dummy_putblock");
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_getblock(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  void *data,
  int x0,
  int y0,
  int xe,
  int ye)
{
  XX_X("dtcanvas_dummy_getblock");
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_putcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  XX_X("dtcanvas_dummy_putcolor");
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_getcolor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  dtlut8_t *lut,
  int start,
  int count)
{
  XX_X("dtcanvas_dummy_getcolor");
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtcanvas_dummy_cursor(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int *x,
  int *y,
  int op)
{
  XX_X("dtcanvas_dummy_cursor");
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
