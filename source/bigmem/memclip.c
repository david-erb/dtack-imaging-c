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
DT_RCSID("src/low $RCSfile: memclip.c,v $ $Revision: 1.2 $");
#include <dtack/mem.h>

/*..........................................................................
 * returns overlap rectangle 
 * destination (0,0,mxe,mye)
 * source (x0,y0,xe,ye)
 * overlap (cx0,cy0,cxe,cye)
 * if either cxe or cye is less then there is no overlap
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmem_clip(
  dt_ctl_t *ctl,
  void *vmemalloc,
  void *data,
  long x0,
  long y0,
  long xe,
  long ye,
  void **cdata,
  long *cx0,
  long *cy0,
  long *cxe,
  long *cye)
{
  dt_rc_e rc = DT_RC_GOOD;
  dtmemalloc_t *memalloc = (dtmemalloc_t *)vmemalloc;

  if (y0 < 0)							/* clip top of image?			    */
  {
    data = (char *)data + 
      (unsigned int)(-y0 * xe);
    ye += y0;
    y0 = 0;
  }

  if (y0+ye > (long)memalloc->units)	/* clip bottom of image?		    */
    ye = memalloc->units - y0;

  if (x0 < 0)							/* clip left of image?			    */
  {
	data = (char *)data +
      (unsigned int)(-x0);
	xe += x0;
	x0 = 0;
  }

  if (x0+xe > (long)memalloc->size)		/* clip right of image?			    */
    xe = memalloc->size - x0;

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
