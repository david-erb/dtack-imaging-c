/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* Example Program dtlev85c                                                 */
/* Illustrates dtlev85_init(), dtlev85_hist(), and                          */
/* dtlev85_compute().                                                       */
/* Single argument is the gray level image filename, such as:               */
/*     dtlev85c  data/dice64.pgm                                            */
/* Uniformity of image taken as monolithic region is computed and printed.  */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtlev85.h>

void main(int argc, char *argv[])
{
  dtimage_t seg;
  dtimage_t gray;
  dthist_t hist;
  dtlev85_t lev85;

  dtimage_import(NULL,                /* read and allocate gray image       */
    &gray, argc>1? argv[1]: "");
  dtimage_create(NULL, &seg,          /* create space for segmented image   */
    gray.xe, gray.ye, DT_NTYPE_B08);  /* same size as gray image            */
  dtimage_constant(NULL, &seg, 1);    /* make one giant blob of 1's         */
  dtimage_hist_alloc(NULL, &gray,     /* space for histogram                */
    &hist);
  dtimage_hist(NULL, &gray, &hist);   /* get histogram of gray image        */

  dtlev85_init(NULL, &lev85);         /* initialize lev85 structure         */
  dtlev85_hist(NULL,                  /* contribute giant blob              */
    &lev85, &hist);
  dtlev85_compute(NULL, &lev85);      /* compute final uniformity           */

  printf("dtlev85c:"                  /* print the uniformity               */
    " uniformity %7.5f\n",
    lev85.u);
  printf("dtlev85c: normal exit\n");
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
