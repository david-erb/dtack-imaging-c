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
DT_RCSID("server $RCSfile: xchg_scr.c,v $ $Revision: 1.5 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/mem.h>

#define DESC "image display"

/*..........................................................................*
 * output image to scr window
 * new window sized to fit image exactly
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_scr_export_image(
  dt_ctl_t *ctl,
  void *vscr,
  const char *windowname,
  dtimage_t *image,
  int x0,
  int y0)
{
  DT_F("dtxchg_scr_export_image");
#if DTPRJ_NOSCREEN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  int reason = 0;
  dtscr_t *scr = (dtscr_t *)vscr;
  dtscr_state_t state;
  dtxy_t y;

  DT_Q(dtscr_query_state,(ctl, scr,		/* see if window exists				*/
    windowname, &state));

  if (state.x0 != x0 &&					/* window is new or being changed? 	*/
	  x0 != DTIMAGE_NOPOS)
    reason = 1;
  if (state.y0 != y0 &&					/* window is new or being changed? 	*/
	  y0 != DTIMAGE_NOPOS)
    reason = 2;
  if (state.xe != image->xe ||
	  state.ye != image->ye)
    reason = 3;
  if (state.bits != 
	  dt_ntype_bits[image->ntype])
    reason = 5;
  
  if (reason)							/* reason to create new window? 	*/
  {
	if (x0 == DTIMAGE_NOPOS &&
		state.x0 >= 0)
	  x0 = state.x0;
	if (y0 == DTIMAGE_NOPOS &&
		state.y0 >= 0)
	  y0 = state.y0;
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "old (%d, %d) (%d, %d) bits=%d",
      state.x0, state.y0,
      state.xe, state.ye,
      state.bits);
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "new (%d, %d) (%d, %d) bits=%d",
      x0, y0,
      image->xe, image->ye,
	  dt_ntype_bits[image->ntype]);
    DT_Q(dtscr_create_window,(ctl,		/* make window same as image size	*/
      scr, windowname,
      x0, y0, image->xe, image->ye,
      dt_ntype_bits[image->ntype]));
  }
#ifdef NONO
  else
    DT_Q(dtscr_window_raise,(ctl,		/* bring window to front			*/
      scr, windowname));
#endif

  y = 0;
  while (y<image->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_GETROWS(ctl, image, y,		/* map the next block				*/
      &p, &datasize);
    nrows = datasize / image->rowsize;  /* how many rows in block			*/

    DT_Q(dtscr_export_data,(ctl,		/* put the block on the scr			*/
      scr, windowname, 0, y, 
      image->xe, nrows,
      dt_ntype_bits[image->ntype],
      p));

    DTIMAGE_UNMAPROW(ctl, image, y);    /* don't need this block any more   */
    y += nrows;
  }
  
  return DT_RC_GOOD;
#endif
}

#if DTPRJ_NOSCREEN == DT_1
#else
/*..........................................................................*/

static
dt_rc_e
dtxchg_scr_import_blocks(
  dt_ctl_t *ctl,
  void *vscr,
  const char *windowname,
  dtimage_t *image)
{
  DT_F("dtxchg_scr_import_image");
  dtscr_t *scr = (dtscr_t *)vscr;
  dtxy_t y;

  y = 0;
  while (y<image->ye)
  {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_MAPROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;  /* how many rows in block           */

    DT_Q(dtscr_import_data,(ctl,		/* get the block from the scr		*/
      scr, windowname, 0, y, 
      image->xe, nrows, p));

    DTIMAGE_PUTROW(ctl, image, y);		/* mark the block as changed    	*/
    y += nrows;
  }
  
  return DT_RC_GOOD;
}
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_scr_import_image(
  dt_ctl_t *ctl,
  void *vscr,
  const char *windowname,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_scr_import_image");
#if DTPRJ_NOSCREEN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtscr_t *scr = (dtscr_t *)vscr;
  dtscr_state_t state;
  dtimage_t tmp;
  static dt_ntype_e ntypes[] = {
    DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  tmp.xe = 0;

  DT_C(dtscr_query_state,(ctl, scr,		/* find out how big the image is	*/
    windowname, &state));

  if (state.xe == 0)				/* window does not exist? 			*/
  {
    rc = dt_err(ctl, F,
      "Sorry, window \"%s\""
      " does not exist",
      windowname);
	goto cleanup;
  }

  DT_C(dtimage_create3,(ctl, &tmp,		/* get space for the image          */
    pagesize, npages,
    state.xe, state.ye,
    DT_NTYPE_B08, NULL));

  rc = dtimage_check_1f(ctl,            /* check image validity             */
    &tmp, "new image",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    goto cleanup;

  DT_C(dtxchg_scr_import_blocks,(		/* import it block by block			*/
    ctl, scr, windowname, &tmp));

cleanup:
  if (rc == DT_RC_GOOD)					/* everything worked? 				*/
    *image = tmp;						/* return what we got 				*/
  else									/* import blocks failed?			*/
  {
    if (tmp.xe != 0)					/* image got allocated? 			*/
      DT_Q(dtimage_free,(ctl, &tmp));	/* free space we allocated			*/
  }
  
  return rc;
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
