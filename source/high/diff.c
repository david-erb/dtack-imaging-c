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
DT_RCSID("source/high $RCSfile: ndiffs.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_ndiffs() - compare two images
| 
| SUMMARY
| Compare all pixels of one image to the corresponding pixels of
| a second image.
| Two pixels are considered to be the same if the absolute value
| of their difference is less than ~epsilon~.
| Return ~*ndiffs~ equal to 0 if there are no differences.
| Otherwise return ~*ndiffs~ equal to the total number of pixels
| which differed.
| Ignore pixels of image beyond the extent of the other.
| Permit images of different data types to be compared.
| Have an error if either row number is outside its respective image.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_ndiffs(							/* compare two images			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image1,					/* first image to compare 			*/
  dtimage_t *image2,					/* second image to compare 			*/
  double epsilon,						/* maximum allowable difference 	*/
  long *ndiffs)							/* returned total number of diffs	*/
{
  DT_F("dtimage_ndiffs");
  dtstats_t stats;

  stats.want = 
    DTSTATS_NLESS | DTSTATS_NMORE;
  stats.have = 0;
  DT_Q(dtimage_stats2,(ctl,				/* stats does the work				*/
    image1, image2, epsilon,
    &stats));

  *ndiffs = stats.nless + stats.nmore;
  
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
