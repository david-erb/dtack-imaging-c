/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* example program dt_sez90_samp_ma                                         */
/* illustrates use of dt_sez90() and dt_sez90_next()                        */
/* only argument is input image name                                        */
#include <dtack/base.h>
#include <dt_sez90.h>
void main(int argc, char *argv[])
{
  dtparam_t param;
  dtimage_t image;
  dthist_t hist;
  char mark[256];
  int s, m, e;
  dtparam_init(&param);               /* init default parameters            */
  dtimage_import(&param.ctl, &image,  /* read and allocate image            */
    argv[1]);
  e = 0;                              /* start at beginning of array        */
  do {
    dt_sez90_peak_next(&param,        /* find next peak in mark array       */
      mark, 256, e, &s, &m, &e);
    printf("peak start %d, max %d, end %d\n",
      s, m ,e);
  } while (s < 256);
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
