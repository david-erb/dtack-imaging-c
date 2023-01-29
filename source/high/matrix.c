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
| TITLE
| new
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  19990130 DE added to the library
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: matrix.c,v $ $Revision: 1.8 $");
#include <dtack/os.h>
#include <dtack/point.h>				/* xyz points 						*/
#include <dtack/poly.h>					/* polynomial transformation		*/
#include <dtack/matrix.h>				/* matrix operations 				*/

#define TINY 1.0e-20

/*---------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

dt_rc_e 
dtimage_lu_decompose(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* the image matrix 				*/
  dt1d_i_t *indices,
  double *d,
  int *singular)
{
  DT_F("dtimage_lu_decompose");
  dtntype_e ntypes[] = {				/* only operate on F2 images 		*/
    DTNTYPE_F2,
    DTNTYPE_NULL
  };
  dt_rc_e rc = DT_RC_GOOD;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, NULL,
    ntypes, 0, F);
  if (rc != DT_RC_GOOD)
    return rc;
  
  if (image->xe != image->ye)
	DT_CLEANUP((ctl, F,
      "image size [%d %d] is not square",
      image->xe, image->ye));

  DT_C(dtmatrix_lu_decompose,(ctl,
    (void **)image->row, image->xe, image->ntype,
    indices, d, singular));
cleanup:
  return rc;
}

/*---------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

dt_rc_e 
dtimage_lu_substitute(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* the image matrix 				*/
  dt1d_i_t *indices,
  double *b)
{
  DT_F("dtimage_lu_substitute");
  DT_Q(dtmatrix_lu_substitute,(ctl,
    (void **)image->row, image->xe, image->ntype,
    indices, b));
  return DT_RC_GOOD;
}
#undef PSECT


