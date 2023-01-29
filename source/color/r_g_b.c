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


DT_RCSID("source/color $RCSfile: r_g_b.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_luminance_init(
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
dt_color_r_g_b_to_luminance_uninit(
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
dt_color_r_g_b_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r_g_b,				
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
dt_color_r_g_b_to_pseudo_init(
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
dt_color_r_g_b_to_pseudo_uninit(
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
dt_color_r_g_b_to3_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
{
  DT_F("dt_color_r_g_b_to3_pseudo");
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
    F, "component luts"));
  
  glut = rlut + 256;
  blut = rlut + 512;
  
  xvrc = xv310a_slow_quant3(			/* quantize in rgb space 			*/
    r, g, b,
    xe,
    ye,
    pseudo,
    rlut,
    glut,
    blut,
    convert->ncells?  
      convert->ncells: 236);

  if (xvrc == 0)
  {
	dtlut8_t *lut8 = (dtlut8_t *)lut;	/* multiplexed lut output space		*/
	int i;
	for (i=0; i<256; i++)				/* multiplex the lut 				*/
	{
	  lut8[i].r = rlut[i];
	  lut8[i].g = glut[i];
	  lut8[i].b = blut[i];
	  lut8[i].i = 0;
	}
  }

  DT_Q(dtos_free2,(ctl,					/* free separate luts 				*/
    rlut, F, "component luts"));

  if (xvrc != 0)
    return dt_err(ctl, F,
      "xv310a_slow_quant3 returned error (%d)", xvrc);
  return DT_RC_GOOD;
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_map_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_r_g_b_to_map_init");
  DT_MEMSET(convert, 0, sizeof(*convert));
  
  DT_Q(dtos_malloc2,(ctl,				/* space for separate luts 			*/
    (void **)&convert->rlut, 256 * 3,
    F, "component luts"));
  
  convert->glut = convert->rlut + 256;
  convert->blut = convert->rlut + 512;
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_map_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_r_g_b_to_map_uninit");
  if (convert->rlut != NULL)
    DT_Q(dtos_free2,(ctl,				/* free separate luts 				*/
      convert->rlut, 
      F, "component luts"));
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to3_map(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
{
  DT_F("dt_color_y_g_cr_to_map");
# if DTPRJ_XV != DT_1
  return dt_assert_defined(ctl,
    0, F, "DTPRJ_XV310A",
    "XV color conversion library");
# else
  unsigned char *rlut;
  unsigned char *glut;
  unsigned char *blut;
  int xvrc;

  if (lut == NULL)						/* color does not want new lut 		*/
    goto map;

  DT_Q(dtos_malloc2,(ctl,				/* space for separate luts 			*/
    (void **)&rlut, 256 * 3,
    F, "component luts"));
  
  glut = rlut + 256;
  blut = rlut + 512;
  
  xvrc = xv310a_slow_quant3(			/* quantize in ycrg space 			*/
    r, 
    g,									/* green gets most of the bits 		*/
    b, 
    xe,
    ye,
    pseudo,
    rlut,
    glut,
    blut,
    convert->ncells?  
      convert->ncells: 236);

  if (xvrc == 0)
  {
	dtlut8_t *lut8;
	int i;
	DT_MEMCPY(convert->rlut, rlut, 256);	/* keep a copy for mapping			*/
	DT_MEMCPY(convert->glut, glut, 256);
	DT_MEMCPY(convert->blut, blut, 256);
    lut8 = (dtlut8_t *)lut;				/* multiplexed lut output space		*/
	for (i=0; i<256; i++)				/* multiplex the lut 				*/
	{
	  lut8[i].r = rlut[i];
	  lut8[i].g = glut[i];
	  lut8[i].b = blut[i];
	  lut8[i].i = 0;
	}
  }

  DT_Q(dtos_free2,(ctl,					/* free separate luts 				*/
    rlut, F, "component luts"));

  if (xvrc != 0)
    return dt_err(ctl, F,
      "xv310a_slow_quant3 returned error (%d)", xvrc);
  
  return DT_RC_GOOD;
										/* ................................ */
map:
  
  xvrc = xv310a_slow_map3(				/* map data to previous luts		*/
    r, 
    g,									
    b, 
    xe,
    ye,
    pseudo,
    convert->rlut,						/* previously generated luts 		*/
    convert->glut,
    convert->blut,
    convert->ncells?  
      convert->ncells: 236);

  if (xvrc != 0)
    return dt_err(ctl, F,
      "xv310a_slow_map3 returned error (%d)", xvrc);
  
  return DT_RC_GOOD;
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_y_cb_cr_init(
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
dt_color_r_g_b_to_y_cb_cr_uninit(
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
dt_color_r_g_b_to3_y_cb_cr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *y,
  unsigned char *cb,
  unsigned char *cr)
{
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
