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
| Introduction - intro
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/color.h>

DT_RCSID("source/color $RCSfile: yuyv.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * yuyv for n=2 is YUYV
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *yuyv,					/* input in form YUYV				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
{
  const unsigned int n = xe * ye;
  unsigned int i;

  for (i=0; i<n; i+=2)
  {
	*luminance++ = *yuyv++;				/* get the luminance 				*/
	yuyv++;								/* skip the chrominance 			*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#define LIMIT(x) (unsigned char)(((x) < 0) ? 0 : ((x) > 255) ? 255 : (x))

dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *yuyv,					/* input in form YUYV				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
{
  dtxy_t x, y;
  int y1, u, y2, v;
  double uv;

  for (y=0; y<ye; y++)
  for (x=0; x<xe; x+=2)									
  {														
	y1 = *yuyv++;						
	u  = *yuyv++ - 128;					
	y2 = *yuyv++;						
	v  = *yuyv++ - 128;					
	uv = 0.34375 * u - 0.703125 * v;	
														
	*rgb888++ = LIMIT(y1 + 1.375 * v);					
	*rgb888++ = LIMIT(y1 - uv);							
	*rgb888++ = LIMIT(y1 + 1.734375 * u);				
														
	*rgb888++ = LIMIT(y2 + 1.375 * v);					
	*rgb888++ = LIMIT(y2 - uv);							
	*rgb888++ = LIMIT(y2 + 1.734375 * u);				
  }

#ifdef NONO
  
    *rgb888++ = u;										
    *rgb888++ = u;										
    *rgb888++ = u;										
														
    *rgb888++ = u;										
    *rgb888++ = u;										
    *rgb888++ = u;										
	
#endif

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
