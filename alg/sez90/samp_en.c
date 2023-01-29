/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* example program dt_sez90_samp_en                                         */
/* illustrates use of dt_sez90_ends()                                       */
/* only argument is input image name                                        */
#include <dtack/base.h>
#include <dt_sez90.h>
void main(int argc, char *argv[])
{
  dtparam_t param;
  dtimage_t image;
  double hist[256];
  char mark[256];
  int i, before, after;
  dtparam_init(&param);               /* init default parameters            */
  dtimage_import(&param.ctl, &image,  /* read and allocate image            */
    argv[1]);
  dt_sez90(&param, hist, 256, mark);  /* find peaks, set up mark array      */
  before = 0;
  for (i=0; i<256; i++)               /* count peaks before                 */
    if (mark[i] & DT_SEZ90_START)
      before++;
  dt_sez90_ends(&param, hist, 256,    /* mark peaks at ends                 */
     mark);
  after = 0;
  for (i=0; i<256; i++)               /* count peaks after                  */
    if (mark[i] & DT_SEZ90_START)
      after++;
  printf("%d peaks before, %d peaks after\n",
    before, after);
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
