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
  CHANGES
  19980905 DE add gen_distance routine
 *..........................................................................*/



/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/lut8.h>

DT_RCSID("source/high $RCSfile: gen1.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_rect_islands(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int separation,
  int xsize,
  int ysize)
{
  DT_F("dtimage_gen_rect_islands");
  dtxy_t x, y;
  dtxy_t xx, yy;

  DT_Q(dtimage_constant,(ctl, image, background));

  for (y=separation; y<image->ye; y+=ysize+separation)
    for (x=separation; x<image->xe; x+=xsize+separation)
      for (yy=0; yy<ysize; yy++)
	  {
        if (y+yy >= image->ye) break;
        for (xx=0; xx<xsize; xx++)
        {
          if (x+xx >= image->xe) break;
          DT_Q(dtimage_set_pixel,(ctl, image, x+xx, y+yy, foreground));
		}
	  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_checkerboard(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int xsize,
  int ysize)
{
  DT_F("dtimage_gen_checkerboard");
  dtxy_t x, y;
  dtxy_t xx, yy;
  dtxy_t start;

  DT_Q(dtimage_constant,(ctl, image, background));

  start = 0;
  for (y=0; y<image->ye; y+=ysize)
  {
    for (x=start; x<image->xe; x+=xsize*2)
      for (yy=0; yy<ysize; yy++)
	  {
        if (yy >= image->ye) break;
        for (xx=0; xx<xsize; xx++)
        {
          if (xx >= image->xe) break;
          DT_Q(dtimage_set_pixel,(ctl,
            image, x+xx, y+yy, foreground));
		}
	  }
    start = start? 0: xsize;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_color256(
  dt_ctl_t *ctl,
  dtimage_t *image)
{
  DT_F("dtimage_gen_color256");
  dtxy_t x, y;
  double xcolor, ycolor;

  for (y=0; y<image->ye; y++)
  {
	ycolor = (double)y / (double)image->ye * 16.0;
    for (x=0; x<image->xe; x++)
    {
      xcolor = (double)x / 
        (double)image->xe * 16.0;
	  DTIMAGE_SETPIX1(DT_Q, ctl,
        image, x, y, 
        (int)ycolor*16+xcolor);
	}
  }

  return DT_RC_GOOD;
}



/*..........................................................................
 * image with arithmetic progression grid
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_grid1(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dtimage_gen_grid1");
  const dtxy_t xe = image->xe;
  const dtxy_t ye = image->ye;
  const dtxy_t dx = (dtxy_t)param->dx;
  const dtxy_t dy = (dtxy_t)param->dy;
  dtxy_t x, y;
  dtxy_t yi;
  const dt_ntype_b08_t foreground = (dt_ntype_b08_t)param->foreground;
  const dt_ntype_b08_t background = (dt_ntype_b08_t)param->background;
  dt_ntype_e ntypes[] = {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check data and memory			*/
    image, "image",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  yi = 0;
  for (y=0; y<ye; y++)
  {
    dt_ntype_b08_t *p1;
    DTIMAGE_MAPROW(ctl, image, y, (void **)&p1);
    {
      dt_ntype_b08_t * const p = p1;
      if (y == yi)						/* solid horizontal bar?			*/
      {
        DT_MEMSET(p, foreground, xe);
        yi += dy;
      }
      else								/* vertical bars only?				*/
      {
        DT_MEMSET(p, background, xe);
        for (x=0; x<xe; x+=dx)
          p[x] = foreground;
      }
    }
    DTIMAGE_PUTROW(ctl, image, y);
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 * image with arithmetic progression grid
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_grid2(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
{
  DT_F("dtimage_gen_grid2");
  dtxy_t xe = image->xe;
  dtxy_t ye = image->ye;
  dtxy_t x, y;
  dtxy_t i, yi;
  const dt_ntype_b08_t foreground = (dt_ntype_b08_t)param->foreground;
  const dt_ntype_b08_t background = (dt_ntype_b08_t)param->background;
  dt_ntype_e ntypes[] = {DT_NTYPE_B08, DT_NTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check data and memory			*/
    image, "image",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  i = 0;
  yi = 0;
  for (y=0; y<ye; y++)
  {
    dt_ntype_b08_t *p1;
    DTIMAGE_MAPROW(ctl, image, y, (void **)&p1);
    {
      dt_ntype_b08_t * const p = p1;
      if (y == yi)						/* solid horizontal bar?			*/
      {
        DT_MEMSET(p, foreground, xe);
        yi += (++i);
      }
      else								/* vertical bars only?				*/
      {
        int i = 0;
        DT_MEMSET(p, background, xe);
        for (x=0; x<xe; x+=(++i))
          p[x] = foreground;
      }
    }
    DTIMAGE_PUTROW(ctl, image, y);
  }

  return DT_RC_GOOD;
}



/*..........................................................................
 * generate distance image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_distance(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,
  dtxy_t xc,
  dtxy_t yc)
{
  DT_F("dtimage_gen_distance");
  dtxy_t x, y;
  dtxy_t xe = image->xe;
  dtxy_t ye = image->ye;
  dt_rc_e rc = DT_RC_GOOD;
  double offset;
  if (DTNTYPE_ISFLOAT(image->ntype))
	offset = 0.0;
  else
	offset = 0.5;
  for (y=0; y<ye; y++)
    for (x=0; x<xe; x++)
	  DT_C(dtimage_set_pixel,(ctl, 
        image, x, y, 
        sqrt((x-xc)*(x-xc) +
			 (y-yc)*(y-yc)) + offset));
cleanup:
  return rc;
}


/*..........................................................................
 * generate distance-squared image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_distance_squared(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,
  dtxy_t xc,
  dtxy_t yc)
{
  DT_F("dtimage_gen_distance_squared");
  dtxy_t x, y;
  dtxy_t xe = image->xe;
  dtxy_t ye = image->ye;
  dt_rc_e rc = DT_RC_GOOD;
  for (y=0; y<ye; y++)
    for (x=0; x<xe; x++)
	  DT_C(dtimage_set_pixel,(ctl, 
        image, x, y, 
        (x-xc)*(x-xc) +
		(y-yc)*(y-yc)));
cleanup:
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
