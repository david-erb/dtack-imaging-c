/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/t.h>

DT_RCSID("test $RCSfile: gen256.c,v $ $Revision: 1.8 $");

#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtt_check - check condition
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| As yet undocumented.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtt_gen_color256(					/* generate 16x16 squares 			*/
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  DT_F("dttscr_gen_color256");
  dt_ntype_b08_t *buf = NULL;
  dtxy_t row, col;
  dtxy_t nrows = 16;
  dtxy_t ncols = 16;
  dtxy_t x, y;
  dtxy_t xinc, yinc;
  dt_ntype_b08_t *p;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for row buffer 			*/
    (void **)&buf, image->xe,
    F, "row buffer"));
  DT_MEMSET(buf, 0, image->xe);			/* clear row buffer 				*/

  yinc = image->ye / nrows;
  xinc = image->xe / ncols;

  if (xinc == 0)						/* image too narrow? 				*/
  {
	xinc = 1;
    ncols = image->xe;
  }

  if (yinc == 0)						/* image too short? 				*/
  {
	yinc = 1;
    nrows = image->ye;
  }
  else									/* image tall enough? 				*/
  for (y=nrows*yinc; y<image->ye; y++)	/* clear bottom part of image 		*/
  {
	DTIMAGE_GETROW2(DT_C, ctl,
      image, y, &p);
    DT_MEMCPY(p, buf, image->xe);
	DTIMAGE_PUTROW2(DT_C, ctl,
      image, y);
  }

  for (row=0; row<nrows; row++)			/* loop over block rows 			*/
  {
	const dtxy_t col0 = row * nrows;
	p = buf;
    for (col=0; col<ncols; col++)		/* build row buffer 				*/
	  for (x=0; x<xinc; x++)
        *p++ = col0 + col;

	for (y=0; y<yinc; y++)				/* loop over rows in block 			*/
	{
	  const dtxy_t y0 = row * yinc;
	  DTIMAGE_GETROW2(DT_C, ctl,		/* access row 						*/
        image, y0+y, &p);
      DT_MEMCPY(p, buf, image->xe);		/* set row values 					*/
	  DTIMAGE_PUTROW2(DT_C, ctl,		/* release row 						*/
        image, y0+y);
	}
  }
cleanup:
  if (buf)
    DT_I(dtos_free2,(ctl, buf,
      F, "row buffer"));

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
