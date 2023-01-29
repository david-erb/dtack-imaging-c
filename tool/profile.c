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
| dttool_profile utility - make profile of row or column
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

DT_RCSID("tool $RCSfile: profile.c,v $ $Revision: 1.6 $");

#define USAGE "[params] source h|v pos dest"

/*..........................................................................*/
static dt_rc_e dttool_profile_parse(			/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  DT_F("dttool_profile_parse");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(h, 128);				/* height of profile display	    */
  DTPARAM_DEFAULT(foreground, 255);		/* color for drawing bars		    */
  DTPARAM_DEFAULT(background, 0);
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dttool_parse,(param, F, USAGE,		/* go parse args					*/
    argc, argv, n));

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e dttool__profile(					/* make profile plot				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  int dir,
  dtxy_t pos,
  dtimage_t *plot,
  double foreground,
  double background)
{
  DT_F("dttool__profile");
  dt_ntype_b08_t *profile;				/* profile array					*/
  int ybot = plot->ye - 1;				/* profile plot limits				*/
  double dmin = 0.0;					/* min data on plot				    */
  double dmax = 256.0;					/* max data on plot				    */
  dtxy_t x;
  int ylast;
  dt_rc_e rc;

  DT_Q(dtos_malloc,(ctl,				/* space for profile				*/
    (void **)&profile,
    plot->xe * sizeof(*profile)));

  for (x=0; x<plot->xe; x++)			/* make profile  					*/
  {
    double d;
    if (dir == 'h')
  	  DT_G(dtimage_get_pixel,(ctl,		/* get pixel along profile		    */
        image, x, pos, &d))
    else
  	  DT_G(dtimage_get_pixel,(ctl,		/* get pixel along profile		    */
        image, pos, x, &d));
    if (rc != DT_RC_GOOD) break;
    d = DT_MAX(d, dmin);
    d = DT_MIN(d, dmax);
    profile[x] = (dt_ntype_b08_t)
      ((plot->ye-1) *					/* scale data for window		    */
       (d-dmin) / (dmax-dmin));
  }

  DT_GI(dtimage_constant,(ctl,			/* clear profile plot				*/
    plot, background));

  ylast = 0;
  if (rc == DT_RC_GOOD)
  for (x=0; x<plot->xe; x++)			/* for each bar					    */
  {
    int ybar = ybot - (int)profile[x];	/* bar height						*/
    int ydiff = ybar - ylast;
    int yhalf = ydiff / 2 + ylast;
    int yinc = ydiff? ydiff / abs(ydiff): 0;
    int y = ylast;
    if (x > 0)							/* not first column?			    */
    {
      do {								/* poor man's line drawing		    */
        DT_G(dtimage_set_pixel,(ctl,	/* draw left half of line		    */
          plot, x-1, y, foreground));
        if (rc != DT_RC_GOOD) break;
        if (ylast != yhalf)
          y += yinc;
      } while (y != yhalf);
      if (rc == DT_RC_GOOD)				/* left half of line OK?		    */
      do {								/* draw right half of line		    */
        DT_G(dtimage_set_pixel,(ctl,
          plot, x, y, foreground));
        if (rc != DT_RC_GOOD) break;
        y += yinc;
      } while (y != ybar);
	}
    if (rc != DT_RC_GOOD) break;
    ylast = ybar;						/* height of last bar			    */
  }

  DT_Q(dtos_free,(ctl, profile));		/* free profile						*/

  return rc;
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_profile)
{
  dtimage_t image;						/* image structure					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int n;
  char *arg_input;
  char *arg_dir;
  char *arg_pos;
  char *arg_output;
  long pos;
  dtimage_t plot;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dttool_profile_parse,(param,		/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 4)						/* check args						*/
    DT_GI(dttool_usage,(param, USAGE));

  arg_input = argv[0];					/* get positionals				    */
  arg_dir = argv[1];
  arg_pos = argv[2];
  arg_output = argv[3];

  if (strcmp(arg_dir, "h") &&			/* check direction validity		    */
      strcmp(arg_dir, "v"))
    DT_GI(dttool_usage,(param, USAGE));

  DT_GI(dtstr_to_long,(ctl, arg_pos,	/* convert numeric positionals	    */
    &pos));

  image.xe = 0;
  DT_GI(dtimage_import,(ctl,			/* get image from file or window	*/
    &image, arg_input));

  plot.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* get space for profile plot		*/
    &plot,
    arg_dir[0]=='h'?image.xe:image.ye,
    param->h,
    DT_NTYPE_B08));

  DT_GI(dttool__profile,(ctl,			/* make profile image of image   	*/
    &image, arg_dir[0], 
    (dtxy_t)pos, &plot,
    param->foreground,
    param->background));

  DT_GI(dtimage_export,(ctl,			/* output profile plot				*/
    &plot, arg_output, DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  if (plot.xe != 0)						/* we actually allocated it?	    */
  DT_G(dtimage_free,(ctl,				/* free profile plot				*/
    &plot));

  if (plot.xe != 0)						/* we actually allocated it?	    */
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
