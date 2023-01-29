/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* example program dt_sez90_samp_pl                                         */
/* demonstrates how to plot boxes around peaks and lines at their maxima    */
/* first argument is input image name, second argument is output plot name  */
#include <dtack/base.h>
#include <dt_sez90.h>
void main(int argc, char *argv[])
{
  dtparam_t param;
  dtimage_t image;
  double hist[256];
  char mark[256];
  dtimage_t plot;
  dtparam_init(&param);               /* init default parameters            */
  dtimage_import(&param.ctl, &image,  /* read and allocate image            */
    argv[1]);
  dt_sez90(&param, hist, 256, mark);  /* find peaks, set up mark array      */

  dtimage_create(&param.ctl, &plot,   /* create plot image                  */
    256, 128, DT_NTYPE_B08);          /* width 256, height 128              */
  dtimage_constant(&param.ctl, &plot, /* clear plot image to color 0        */
    0);
  dtimage_plot_xbars_double(          /* plot histogram in color 1          */
    &param.ctl, &plot, hist, 256,
    0, 128, 1);
  dt_sez90_plot_boxes(&param,         /* plot boxes in color 2              */
    mark, 256, &plot, 2);
  dt_sez90_plot_maxes(&param,         /* plot peak maxima in color 3        */
    mark, 256, &plot, 3);
  dtimage_export(&param.ctl, &plot,   /* output plot to display or file     */
    argv[2], -1, -1);
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
