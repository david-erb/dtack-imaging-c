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
DT_RCSID("server $RCSfile: xchg_can.c,v $ $Revision: 1.5 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>
#include <dtack/mem.h>

#define DESC "canvas drawing"

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_canvas_export_image(
  dt_ctl_t *ctl,
  void *vcanvas,
  dtimage_t *image,
  int x0,
  int y0)
{
  DT_F("dtxchg_canvas_export_image");
#if DTPRJ_NOSCREEN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtcanvas_t *canvas = (dtcanvas_t *)vcanvas;
  dtxy_t y;

  y = 0;
  while (y<image->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_GETROWS(ctl, image, y,		/* map the next block	*/
      &p, &datasize);
    nrows = datasize / image->rowsize;  /* how many rows in block	*/

    DT_Q(dtcanvas_putblock,(ctl,		/* put the block on the canvas	*/
      canvas, p, x0, y0+y, 
      image->xe, nrows));

    DTIMAGE_UNMAPROW(ctl, image, y);    /* don't need this block any more   */
    y += nrows;
  }
  
  return DT_RC_GOOD;
#endif
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_canvas_import_image(
  dt_ctl_t *ctl,
  void *vcanvas,
  dtimage_t *image,
  int x0,
  int y0)
{
  DT_F("dtxchg_canvas_import_image");
#if DTPRJ_NOSCREEN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtcanvas_t *canvas = (dtcanvas_t *)vcanvas;
  dtxy_t y;

  y = y0;
  while (y<image->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_MAPROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;  /* how many rows in block           */

    DT_Q(dtcanvas_getblock,(ctl,		/* put the block on the canvas		*/
      canvas, p, x0, y0+y, 
      image->xe, nrows));

    DTIMAGE_PUTROW(ctl, image, y);		/* mark the block as changed    	*/
    y += nrows;
  }
  
  return DT_RC_GOOD;
#endif
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
