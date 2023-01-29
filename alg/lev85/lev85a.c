/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* Example Program dtlev85a                                                 */
/* Illustrates dtlev85_total().                                             */
/* Three arguments are: 1) the gray level image filename,                   */
/* 2) the segmented image filename, and 3) the background value             */
/*     dtlev85a  data/dice64.pgm  data/dice64s.pgm  0                       */
/* total uniformity of non-background regions is computed and printed       */
/* all error checking left out for clarity                                  */

#include <dtack/base.h>
#include <dtlev85.h>

void main(int argc, char *argv[])
{
  dtimage_t gray;
  dtimage_t segmented;
  double background;
  dtlev85_t lev85;

  dtimage_import(NULL,                /* read and allocate gray image       */
    &gray, argv[1]);
  dtimage_import(NULL,                /* read and allocate segmented image  */
    &segmented, argv[2]);
  background = atof(argv[3]);         /* get background from command line   */

  dtlev85_total(NULL, &gray,          /* compute total uniformity           */
    &segmented, background, &lev85);

  printf("dtlev85a: uniformity %0.5f" /* print the uniformity               */
    " over %ld regions covering"
    " %ld%% of the image area\n",
    lev85.u, lev85.n,
    (100L * lev85.Aa) /
    ((long)gray.xe * (long)gray.ye));
  printf("dtlev85a: normal exit\n");
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
