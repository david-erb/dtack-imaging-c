/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* Example Program dtlev85d                                                 */
/* Illustrates blob-by-blob direct-pixel uniformity computation.            */
/* The two arguments are: 1) gray level image filename, and                 */
/* 2) segmented image filename, for example:                                */
/*     dtlev85d  data/dice64.pgm  data/dice64s.pgm                          */
/* Uniformity of each blob is computed and printed.                         */
/* Bounding box size of each blob is also printed just for fun.             */
/* Segmented image should not be larger than gray image.                    */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtlev85.h>

static dtimage_t gray;                /* global also seen by callback       */

/*..........................................................................*/
dt_rc_e
mydeliver(
  dt_ctl_t *ctl,
  dtrun_t *run,                       /* connected components housekeeping  */
  dtrun_blob_t *blob,
  void *arg)
{
  dtlev85_t lev85;
  dt_stats_t stats = {                /* specify stats we want on blob      */
    DT_STATS_WIDTH |
    DT_STATS_HEIGHT};
  dtimage_blob_stats(NULL, &gray,     /* compute stats on blob              */
     run, blob, &stats);

  dtlev85_init(NULL, &lev85);         /* init lev85 structure               */
  dtlev85_blob(NULL,                  /* simply add blob to it              */
    &lev85, &gray, run, blob,
    &stats);
  dtlev85_compute(NULL, &lev85);      /* compute blob's uniformity          */
  printf("dtlev85d:  blob"            /* print out interesting stuff        */
    " label %6g, "
    " area %6ld, "
    " width %3u height %3u,"
    " uniformity %0.5f\n",
    blob->label, lev85.Aa,
    stats.width, stats.height,
    lev85.u);
  return DT_RC_GOOD;                  /* keep doing blobs                   */
}
/*..........................................................................*/
void main(int argc, char *argv[])
{
  dtimage_t segmented;

  dtimage_import(NULL,                /* read and allocate gray image       */
    &gray, argv[1]);
  dtimage_import(NULL,                /* read and allocate segmented image  */
    &segmented, argv[2]);

  dtimage_adj_blob(NULL,              /* do connected components            */
    &segmented,                       /* on segmented image                 */
    0, 0, gray.xe, gray.ye,           /* use whole input image              */
    0.0, 0,                           /* ignoring no pixels, 4-connected    */
    mydeliver, NULL);                 /* deliver blobs into our function    */
  printf("dtlev85d: normal exit\n");
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
