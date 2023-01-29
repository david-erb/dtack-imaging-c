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
| NAME
| dttooltext utility - text image to/from window
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: text.c,v $ $Revision: 1.2 $");

#define USAGE "[params] source \"text\" dest"
  char *p, *q;
  int ncols, nrows;

dt_rc_e
dttool_text_add(
  dt_ctl_t *ctl,
  dtimage_t *input,
  char *string,
  int xpos,
  int ypos,
  double foreground,
  double background,
  dtimage_t *output)
{
  DT_F("dttool_text_add");
  char *p, *q;
  dtxy_t ncols, nrows;
  dtxy_t x0, y0;
  dtxy_t xe, ye;
  dtimage_t inlay;
  dt_rc_e rc = DT_RC_GOOD;

  q = string;
  ncols = 0;
  nrows = 0;
  do {									/* size of block in char cells	    */
    p = strchr(q, '\n');
    if (p == NULL)
      p = q + strlen(q);
    ncols = DT_MAX(ncols, (int)(p-q));
    nrows++;
    q = p + 1;
  } while(*p);

  ncols *= 8;							/* size of block in pixels		    */
  nrows *= 16;

  x0 = 0;								/* for when text is within image	*/
  xe = input->xe;
  if (xpos < 0)							/* text starts to left of image?    */
  {
    x0 = -xpos;
    xe += -xpos;
    xpos = 0;
  }
  if (xpos+ncols > xe)					/* text goes beyond right of image?	*/
    xe = xpos+ncols;

  y0 = 0;								/* for when text is within image	*/
  ye = input->ye;
  if (ypos < 0)							/* text starts above image?   		*/
  {
    y0 = -ypos;
    ye += -ypos;
    ypos = 0;
  }
  if (ypos > ye)
    ypos = ye;
  if (ypos+nrows > ye)					/* text goes below bottom of image?	*/
    ye = ypos+nrows;

  output->xe = 0;
  DT_GI(dtimage_create,(ctl, output,	/* space for output image		    */
    xe, ye, input->ntype));

  DT_GI(dtimage_constant,(ctl, output,	/* set background in output image	*/
    background));

  inlay.xe = 0;						
  DT_GI(dtimage_overlay,(ctl, output,	/* input inlayed into output		*/
    &inlay, 
    x0, y0,
    input->xe, input->ye));

  DT_GI(dtimage_cast,(ctl, input,		/* copy input to inlay			    */
    &inlay));

  DT_I(dtimage_free,(ctl, &inlay));		/* free the inlay				    */

  DT_GI(dtimage_plot_text_8x16,(ctl,	/* draw text					    */
    output, string, xpos, ypos,
    foreground));

  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_text)
{
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;	
  dtimage_t output;
  int n;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT_P(param, dbg, 0);	/* set default for params we want	*/
  DTPARAM_DEFAULT_P(param, x, 0);		/* dest place on page or screen	    */
  DTPARAM_DEFAULT_P(param, y, 0);
  DTPARAM_DEFAULT_P(param, foreground, 255);
  DTPARAM_DEFAULT_P(param, background, 0);

  DT_GI(dt_enter_param,(				/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check args						*/
    DT_GI(dt_usage_say,(
      param, USAGE));

  input.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
    &input, argv[0]));

  output.xe = 0;
  DT_GI(dttool_text_add,(ctl, &input,	/* add text and make new image		*/
    argv[1],
    (int)param->x,
    (int)param->y, 
    param->foreground,
    param->background,
    &output));

  DT_GI(dtimage_export,(ctl,			/* put image into file or window	*/
    &output, argv[2],
    DTIMAGE_NOPOS,
    DTIMAGE_NOPOS));

  DT_I(dtimage_free,(ctl,				/* free cropped image				*/
    &output));

  DT_I(dtimage_free,(ctl,				/* free input image					*/
    &input));

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  return rc;
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
