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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: compress.c,v $ $Revision: 1.8 $");
#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/mem.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_compress(
  dtparam_t *param,
  void *data,
  long ndata,
  long xe,
  long ye,
  long quality,
  void **buffer,
  long *nbuffer,
  long *obuffer)
{
  DT_F("tabdrone_compress");
  dt_ctl_t *ctl = &param->ctl;
  long mbuffer = ndata;					/* can't be bigger than source! 	*/
  void *memalloc = NULL;
  dtimage_t image;
  char q[64];
  dt_rc_e rc;

  image.xe = 0;
  
  if (*buffer == NULL ||				/* buffer not allocated yet? 		*/
	  *nbuffer < mbuffer)				/* given buffer is too small? 		*/
  {
	if (*buffer != NULL)				/* we even have a given buffer? 	*/
	  DT_C(dtos_free2,(ctl, *buffer,
        F, "buffer"));
    DT_C(dtos_malloc2,(ctl, buffer,		/* space for the buffer 			*/
      mbuffer,
      F, "buffer"));
	*nbuffer = mbuffer;					/* remember how big it is 			*/
  }

  DT_C(dtmem_prealloc,(ctl,
    &memalloc,
    xe*ye, 1,							/* size, units 						*/
    &data,
    xe*ye, 1,							/* pagesize, npages given 			*/
	F, "prealloc"));

  DT_C(dtimage_create2,(ctl,			/* wrap image around memalloc 		*/
    &image, 
    xe, ye, DT_NTYPE_B08,
    memalloc));
  
  DT_C(dtstr_printf,(ctl, q, sizeof(q),
    "rgb888+%ld", (long)param->t));
  DT_C(dtxchg_jpeg_export_memory,(ctl,	/* compress image to memory			*/
    q,
    *buffer,
    *nbuffer,
    obuffer,
    &image));

cleanup:
  if (image.xe != 0)
	DT_I(dtimage_free,(ctl, &image));
  if (memalloc != NULL)
	DT_I(dtmem_free,(ctl, memalloc,
      F, "prealloc"));
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
