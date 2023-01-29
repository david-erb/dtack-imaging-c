/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* Example Program umdec1                                                   */
/* Illustrates decoding of one frame                                        */
/* Command line requires two arguments:                                     */
/*   1. umdec spec                                                          */
/*   2. name of an mpeg file                                                */
/* Most error checking left out for clarity.                                */

#include <dtack/base.h>
#include <umdec.h>
                                      /* ---------------------------------- */
dt_rc_e
DTCONFIG_API1
myfeed(                               /* example feed callback              */
  dt_ctl_t *ctl,                      /* environment control                */
  umdec_t *umdec,                     /* umdec object                       */
  void *arg,                          /* caller's unchanged argument        */
  void *buffer,                       /* buffer to put data in              */
  long want,                          /* maximum number of bytes wanted     */
  long *got)                          /* actual number of bytes returned    */
{
  FILE *file = (FILE *)arg;           /* address our arg                    */

  *got = fread(buffer,                /* read data from file                */
    1, want, file);

  return DT_RC_GOOD;
}
                                      /* ---------------------------------- */
dt_rc_e
DTCONFIG_API1
myframe(                              /* example frame callback             */
  dt_ctl_t *ctl,                      /* environment control                */
  umdec_t *umdec,                     /* umdec object                       */
  void *arg,                          /* caller's unchanged argument        */
  umdec_frametype_e frametype)        /* tells what kind of frame it is     */
{
  umdec_status_t status;

  umdec_get_status(ctl, umdec,        /* get current status                 */
    &status);

  printf("umdec1: "                   /* print frame count                  */
    "frame %16.0f\n", status.frame);

  return DT_RC_GOOD;
}
                                      /* ---------------------------------- */
void main(
  int argc,
  char *argv[])
{
  dt_ctl_t *ctl = NULL;
  FILE *file;
  umdec_t umdec;

  file = fopen(argv[2],               /* open input file                    */
    DTCONFIG_FOPEN_RBINARY);

  umdec_init(ctl, &umdec,             /* create umdec object                */
    argv[1], 0);

  if (argc > 3)
    umdec_overlay_text(ctl, &umdec,   /* set up text overlay                */
      argv[3], 60, 400, 32,       /* text, position and height              */
      0x00ffffff, 0x00010101);    /* foreground and background              */

  if (argc > 4)
    umdec_overlay_image(ctl, &umdec,  /* set up image overlay               */
      argv[4], 60, 40);       /* filename and position                      */

  umdec_go(ctl, &umdec,               /* start mpeg decoding                */
    myfeed, (void *)file,
    myframe, NULL);

  umdec_uninit(ctl, &umdec);          /* release decoding resources         */
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
