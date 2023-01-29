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
DT_RCSID("source/low $RCSfile: points.c,v $ $Revision: 1.8 $");
#include <dtack/point.h>				/* xyz points 						*/
#include <dtack/points.h>				/* lists of xyz points 				*/

/*-----------------------------------------------------------------------	
 * print points to file with given format
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoints_2dprintf(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtpoint_2d_t *points,					/* points array 					*/
  dt1d_i_t n,							/* number of points in list 		*/
  FILE *file,							/* file to print to 				*/
  const char *format)
{
  dt1d_i_t i;
  for (i=0; i<n; i++)
    fprintf(file, format, 
      i, points[i].x, points[i].y);

  return DT_RC_GOOD;
}

/*-----------------------------------------------------------------------	
 * compare lists of points
 *-----------------------------------------------------------------------*/

dt_rc_e 
dtpoints_2dcompare(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtpoint_2d_t *points1,				/* points array 					*/
  dtpoint_2d_t *points2,				/* points array 					*/
  dt1d_i_t n,							/* number of points in list 		*/
  double epsilon,						/* maximum allowable difference 	*/
  int *cmp,								/* returned -1, 0 or 1 				*/
  dt1d_i_t *k)							/* index of first difference 		*/
{
  dt1d_i_t i;
  *cmp = 0;
  for (i=0; i<n && *cmp==0; i++)
    if (points1[i].x - points2[i].x < -epsilon ||
		points1[i].y - points2[i].y < -epsilon)
	  *cmp = -1;
    else
    if (points1[i].x - points2[i].x > epsilon ||
		points1[i].y - points2[i].y > epsilon)
	  *cmp = 1;

  if (*cmp && k)
	*k = i - 1;

  return DT_RC_GOOD;
}
