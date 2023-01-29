/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: cmp.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_cmp() - compare two images
| 
| SUMMARY
| Compare all pixels of one image to the corresponding pixels of
| a second image.
| Two pixels are considered to be the same if the absolute value
| of their difference is less than ~epsilon~.
| Return ~*cmp~ equal to 0 if there are no differences.
| Otherwise return ~*cmp~ of -1 if the pixel from ~image1~
| is less than the pixel from ~image2~ at the first difference.
| Finally, return ~*cmp~ of 1 if the first different pixel from ~image1~
| is greater than the pixel from ~image2~.
| Return the column and row location of the first difference if ~*cmp~ is not 0.
| Return nothing in ~cmp~, ~x~, or ~y~ if they are NULL.
| Ignore pixels of image beyond the extent of the other.
| Permit images of different data types to be compared.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_cmp(							/* compare two images			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image1,					/* first image to compare 			*/
  dtimage_t *image2,					/* second image to compare 			*/
  double epsilon,						/* maximum allowable difference 	*/
  long *cmp,							/* returned sign of difference		*/
  dtxy_t *x,							/* column location of difference 	*/
  dtxy_t *y)							/* row location 					*/
{
  DT_F("dtimage_cmp");
  dtstats_t stats;

  stats.want = DTSTATS_CMP;
  stats.have = 0;
  DT_Q(dtimage_stats2,(ctl,				/* stats does the work				*/
    image1, image2, epsilon,
    &stats));

  if (cmp != NULL)
    *cmp = stats.sign;
  if (x != NULL)
    *x = stats.x0;
  if (y != NULL)
    *y = stats.y0;
  
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
