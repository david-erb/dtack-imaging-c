/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* Example Program dtlev85b                                                 */
/* Illustrates dtlev85_label().                                             */
/* Two arguments are: 1) the gray level image filename, and                 */
/* 2) the segmented image filename, for example:                            */
/*     dtlev85b  data/dice64.pgm  data/dice64s.pgm                          */
/* Uniformity of each label is computed and printed.                        */
/* All error checking left out for clarity.                                 */

#include <dtack/base.h>
#include <dtlev85.h>

dt_rc_e mycallback(                   /* callback routine just prints       */
  dt_ctl_t *ctl,
  void *callback_arg,
  dtlev85_t *lev85)
{
  printf(
    "dtlev85b: label %g: uniformity %0.5f"
    " over %ld regions"
    " of total area %ld\n",
    lev85->label,
    lev85->u, lev85->n, lev85->Aa);
  return DT_RC_GOOD;
}

void main(int argc, char *argv[])
{
  dtimage_t gray;
  dtimage_t segmented;
  dtimage_import(NULL,                /* read and allocate gray image       */
    &gray, argv[1]);
  dtimage_import(NULL,                /* read and allocate segmented image  */
    &segmented, argv[2]);

  dtlev85_label(NULL, &gray,          /* compute uniformity by labels       */
    &segmented, mycallback, NULL);    /* call mycallback for each label     */
  printf("dtlev85b: normal exit\n");
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
