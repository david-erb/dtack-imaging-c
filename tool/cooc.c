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
| dttool_cooc utility - make co-occurrence matrix image
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
#include <dtack/os.h>

DT_RCSID("tool $RCSfile: cooc.c,v $ $Revision: 1.6 $");

#define USAGE "[params] source [dest]"

/*..........................................................................*/
static 
dt_rc_e 
dttool_cooc_parse(						/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  DT_F("dttool_cooc_parse");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(h, 256);				/* width of cooc display		    */
  DTPARAM_DEFAULT(w, 256);				/* height of cooc display		    */
  DTPARAM_DEFAULT(dx, 0);				/* cooc x direction and offset	    */
  DTPARAM_DEFAULT(dy, 0);				/* cooc y direction and offset	    */
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dttool_parse,(param, F, USAGE,	/* go parse args					*/
    argc, argv, n));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_cooc)
{
  dtimage_t image;						/* image structure					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int i, n;
  char *arg_input;
  char *arg_output;
  long w, h;
  dtimage_t cooc;
  dt_rc_e rc = DT_RC_GOOD;
  int dx[4], dy[4], nd;

  DT_Q(dttool_cooc_parse,(param,		/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc < 1 || argc > 2)				/* check args						*/
    DT_GI(dttool_usage,(param, USAGE));

  arg_input = argv[0];					/* get positionals				    */
  if (argc >= 2)
    arg_output = argv[1];
  else
    arg_output = NULL;

  image.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
    &image, arg_input));

  if (rc == DT_RC_GOOD)
  {
    if (param->w_flag &					/* w param is on command line?	    */
        DTPARAM_FLAG_VALUE)
      w = param->w;
    else
      w = (long)dt_ntype_maxval[image.ntype]+1;

    if (param->h_flag &					/* h param is on command line?	    */
        DTPARAM_FLAG_VALUE)
      h = param->h;
    else
      h = (long)dt_ntype_maxval[image.ntype]+1;

    if (param->dx_flag &				/* dx param is on command line?	    */
        DTPARAM_FLAG_VALUE ||           /* or								*/
        param->dy_flag &				/* dy param is on command line?	    */
        DTPARAM_FLAG_VALUE)
	{
      dx[0] = (int)param->dx;
      dy[0] = (int)param->dy;
      nd = 1;
	}
    else								/* neither dx nor dy?			    */
	{
      dx[0] = 1;  dy[0] = 0;			/* do it multiple times			    */
      dx[1] = 0;  dy[1] = 1;
      dx[2] = 1;  dy[2] = 1;
      dx[3] = 1;  dy[3] = -1;
      nd = 4;
	}
  }

  cooc.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for cooc image			    */
    &cooc, w, h,
    DT_NTYPE_B32));

  DT_GI(dtimage_cooc_set,(ctl,			/* make cooc image					*/
    &image, dx[0], dy[0],
    &cooc));
  for (i=1; i<nd; i++)
    DT_GI(dtimage_cooc_sum,(ctl,		/* add in other cooc directions		*/
      &image, dx[i], dy[i],
      &cooc));

  if (image.ntype == DT_NTYPE_B01 ||	/* input was binary?			    */
      arg_output == NULL)				/* or no output name given?		    */
    DT_GI(dtimage_dbg_values,(ctl,		/* just print output			    */
      &cooc, NULL, DT_DBG_MASK_ANY))
  else
  {
    dtimage_t byte;
    double max, min, scale;
    byte.xe = 0;
    DT_GI(dtimage_create,(ctl, &byte,	/* make byte image of same size	    */
      cooc.xe, cooc.ye, DT_NTYPE_B08));

#ifdef NONO
    DT_GI(dtimage_add_scalar,(ctl, 		/* log doesn't handle 0's very well	*/
      &cooc, 1.0, &cooc));
#endif
    DT_GI(dtimage_range,(ctl, &cooc,	/* range of input values		    */
      &min, &max));
    if (rc == DT_RC_GOOD)
	{
      max = log10(max);					/* range of output values		    */
      min = log10(min);
      scale = 255.0 / (max - min);		/* scale to byte image			    */
#ifdef NONO
      DT_G(dtimage_log10,(ctl, &cooc,	/* log10-transform cooc image	    */
        scale, scale*min, &byte));
#endif
	}

    DT_GI(dtimage_export,(ctl,			/* output byte image				*/
      &byte, arg_output, DTIMAGE_NOPOS, DTIMAGE_NOPOS));

	if (byte.xe != 0)					/* byte image got allocated?	    */
      DT_G(dtimage_free,(ctl, &byte));
  }

  if (cooc.xe != 0)						/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free cooc plot					*/
    &cooc));

  if (image.xe != 0)					/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free image						*/
    &image));

  DT_Q(dtparam_free,(param));			/* free param space					*/

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
