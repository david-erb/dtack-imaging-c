/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* example program dt_sez90_samp_pm                                         */
/* shows how to use dt_sez90_pds() with dt_sez90_mark()                     */
/* only argument is input image name                                        */
#include <dtack/base.h>
#include <dt_sez90.h>
void main(int argc, char *argv[])
{
  dtparam_t param;
  dtimage_t image;
  double hist[256];
  double pds[256];
  char mark[256];
  dtparam_init(&param);               /* init default parameters            */
  dtimage_import(&param.ctl, &image,  /* read image                         */
    argv[1]);
  dt_sez90_pds(&param, hist, 256,     /* compute pds                        */
    pds);
  dt_sez90_mark(&param, hist, 256,    /* mark peaks using pds               */
    pds, mark);
  dt_sez90_mark_dbg(&param, hist,     /* print peak locations               */
    256, mark,
    NULL, DT_DBG_MASK_ANY);
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
