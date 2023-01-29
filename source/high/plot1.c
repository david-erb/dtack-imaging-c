/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: plot1.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtimage_plot_pixel_func(				/* set pixel to value				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to draw line in		    */
  dtxy_t x,								/* current pixel location 			*/
  dtxy_t y,
  void *arg)							/* caller-defined value 			*/
{
  DT_F("dtimage_plot_pixel_func");
  double color = *(double *)arg;
  DTIMAGE_SETPIX1(DT_Q, ctl, image, x, y, color);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_plot - x
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

dt_rc_e 
DTCONFIG_API1
dtimage_plot_line2(						/* draw line in image			  	*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw line in		    */
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2,
  dtimage_pixel_f *func,
  void *arg)
{
/*
 * Digital Line Drawing
 * by Paul Heckbert
 * from "Graphics Gems", Academic Press, 1990
 * adapted by Dtack Inc.
 */

/*
 * digline: draw digital line from (x1,y1) to (x2,y2),
 * calling a user-supplied procedure at each pixel.
 * Does no clipping.  Uses Bresenham's algorithm.
 *
 * Paul Heckbert	3 Sep 85
 */

# define DOTPROC(X, Y) \
    DT_Q(func,(ctl, image, (dtxy_t)x, (dtxy_t)y, arg))
# define DT_ABS(I) (((I)<0)? -(I): (I))
# define DT_SGN(I) (((I)<0)? -1: (((I)>0)? 1: 0))

  DT_F("dtimage_plot_line2");
  long lx1 = x1;
  long ly1 = y1;
  long lx2 = x2;
  long ly2 = y2;
  long d, dx, dy;
  long ax, ay;
  long sx, sy;
  long x, y;
 
  dx = lx2 - lx1;
  ax = DT_ABS(dx) << 1;
  sx = DT_SGN(dx);
  dy = ly2 - ly1;
  ay = DT_ABS(dy) << 1;
  sy = DT_SGN(dy);

    x = lx1;
    y = ly1;
    if (ax>ay)
	{									/* x dominant */
	  d = ay-(ax>>1);
	  for (;;) 
	  {
	    DOTPROC(x, y);
	    if (x==lx2)
		  break;
	    if (d>=0)
		{
		  y += sy;
		  d -= ax;
	    }
	    x += sx;
	    d += ay;
	  }
    }
    else
	{									/* y dominant */
	  d = ax-(ay>>1);
	  for (;;) 
	  {
	    DOTPROC(x, y);
	    if (y==ly2)
		  break;
	    if (d>=0) 
		{
		  x += sx;
		  d -= ay;
	    }
	    y += sy;
	    d += ax;
	  }
    }
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtimage_plot_line(						/* draw line in image			  	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to draw line in		    */
  dtxy_t x1,							/* starting column 					*/
  dtxy_t y1,							/* starting row 					*/
  dtxy_t x2,							/* ending column 					*/
  dtxy_t y2,							/* ending row 						*/
  double color)							/* color to set pixels to 			*/
{
  DT_F("dtimage_plot_line");
  void *arg = (void *)&color;
  DT_Q(dtimage_plot_line2,(ctl,			/* call more general function 		*/
    image, x1, y1, x2, y2,
    dtimage_plot_pixel_func, arg));		/* pass what we want to do 			*/
  return DT_RC_GOOD;
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
