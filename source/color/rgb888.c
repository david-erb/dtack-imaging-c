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
#include <dtack/os.h>
#include <dtack/lut8.h>

#ifndef DTDEPEND
#if DTPRJ_XV == DT_1
#include <dtack/xv310a.h>
#endif
#endif

DT_RCSID("source/color $RCSfile: rgb888.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_luminance_init(
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
dt_color_rgb888_to_luminance_uninit(
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
dt_color_rgb888_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb888,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_pseudo_init(
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
dt_color_rgb888_to_pseudo_uninit(
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
dt_color_rgb888_to_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb888,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo)
{
  DT_F("dt_color_rgb888_to_pseudo");
# if DTPRJ_XV != DT_1
  return dt_assert_defined(ctl,
    0, F, "DTPRJ_XV310A",
    "XV color conversion library");
# else
  unsigned char *rlut;
  unsigned char *glut;
  unsigned char *blut;
  int xvrc;

  DT_Q(dtos_malloc2,(ctl,				/* space for separate luts 			*/
    (void **)&rlut, 256 * 3,
    F, "component lut"));
  
  glut = rlut + 256;
  blut = rlut + 512;
  
  xvrc = xv310a_slow_quant(
    rgb888, 
    xe,
    ye,
    pseudo,
    blut,
    glut,
    rlut,
    convert->ncells?  
      convert->ncells: 236);

  if (xvrc == 0)
  {
	dtlut8_t *lut8 = (dtlut8_t *)		/* combined lut after data 			*/
      (pseudo + xe * ye);
	int i;
	for (i=0; i<256; i++)
	{
	  lut8[i].r = rlut[i];
	  lut8[i].g = glut[i];
	  lut8[i].b = blut[i];
	  lut8[i].i = 0;
	}
  }

  DT_Q(dtos_free2,(ctl,					/* free separate luts 				*/
    rlut,
    F, "component lut"));

  if (xvrc != 0)
    return dt_err(ctl, F,
      "xv310a_slow_quant returned error (%d)", xvrc);
  return DT_RC_GOOD;
 #endif
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
