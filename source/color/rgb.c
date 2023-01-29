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

DT_RCSID("source/color $RCSfile: rgb.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_luminance_init(
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
dt_color_rgb565_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * rgb565 for n=2 is RGB565
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb565,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
{
  const unsigned int n = xe * ye;
  unsigned int i;

  for (i=0; i<n; i++)
  {
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_rgb888_init(
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
dt_color_rgb565_to_rgb888_uninit(
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
dt_color_rgb565_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb565,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
{
  const unsigned short *p = 
    (unsigned short *)rgb565;
  const unsigned int n = xe * ye;
  unsigned int i;

  for (i=0; i<n; i++)
  {
	const int v = p[i];
	*rgb888++ = (v << 3) & 0xf8;		/* THIS IS NOT RIGHT! 				*/
	*rgb888++ = (v >> 8) & 0xf8;
	*rgb888++ = (v >> 11) & 0xf8;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_luminance_init(
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
dt_color_rgb555_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * rgb555 for n=2 is RGB555
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb555,				/* input in form Ycbycr				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
{
  const unsigned int n = xe * ye;
  unsigned int i;

  for (i=0; i<n; i++)
  {
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_rgb888_init(
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
dt_color_rgb555_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * rgb555 for n=2 is Ycbycr
 * rgb888 for n=2 is rgb888rgb888
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb555,				/* input in form RGB				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
{
  const unsigned short *p = 
    (unsigned short *)rgb555;
  const unsigned int n = xe * ye;
  unsigned int i;
  
										/* xRRR RRGG GGGB BBBB 				*/

  for (i=0; i<n; i++)
  {
	const int v = p[i];
	*rgb888++ = (v >> 7) & 0xf8;		/* R								*/
	*rgb888++ = (v >> 2) & 0xf8;		/* G 								*/
	*rgb888++ = (v << 3);				/* B 								*/
  }

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
