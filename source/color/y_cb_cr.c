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

DT_RCSID("source/color $RCSfile: y_cb_cr.c,v $ $Revision: 1.8 $");

#define INT32 long
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))
#define SCALEBITS	16	/* speediest right-shift on some machines */
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_luminance_init(
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
dt_color_y_cb_cr_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * y_cb_cr for n=2 is Y_Cb_Cr
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				/* input in form Y_Cb_Cr			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
{
  const unsigned int n = xe * ye;

  DT_MEMCPY(luminance, y_cb_cr, n);		/* entire luminance is first 		*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_rgb888_init");
  DT_Q(dt_color_ycbycr_to_rgb888_init,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_rgb888_uninit");
  DT_Q(dt_color_ycbycr_to_rgb888_uninit,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * y_cb_cr for n=2 is Y_Cb_Cr
 * rgb888 for n=2 is rgbrgb
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				/* input in form Y_Cb_Cr			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
{
  const unsigned int n = xe * ye;
  unsigned char *yp = y_cb_cr;
  unsigned char *cbp = y_cb_cr + n;
  unsigned char *crp = cbp + n / 2;
  unsigned int i;
  
  int *Cr_r_tab;
  int *Cb_b_tab;
  INT32 *Cr_g_tab;
  INT32 *Cb_g_tab;
  unsigned char *clip;

  Cr_r_tab = (int *)(convert->space);
  Cb_b_tab = (int *)(Cr_r_tab + 256);
  Cr_g_tab = (INT32 *)(Cb_b_tab + 256);
  Cb_g_tab = (INT32 *)(Cr_g_tab + 256);
  clip = (unsigned char *)(Cb_g_tab + 256) + 128;

  for (i=0; i<n; i++)
  {
	const int y  = *yp++;
	const int cb = *cbp++;
	const int cr = *crp++;
	const int rb = RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr], SCALEBITS);

	*rgb888++ = clip[y + Cr_r_tab[cr]];
	*rgb888++ = clip[y + rb];
	*rgb888++ = clip[y + Cb_b_tab[cb]];
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_411_to_rgb888_init");
  DT_Q(dt_color_ycbycr_to_rgb888_init,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_411_to_rgb888_uninit");
  DT_Q(dt_color_ycbycr_to_rgb888_uninit,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * y_cb_cr_411 for n=2 is Y_Cb_Cr
 * rgb888 for n=2 is rgbrgb
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr_411,			/* input in form Y_Cb_Cr_411			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
{
  DT_F("dt_color_y_cb_cr_411_to_rgb888");
  const unsigned int n = xe * ye;
  unsigned char *ype = y_cb_cr_411;
  unsigned char *ypo = ype + xe;
  unsigned char *cbp = y_cb_cr_411 + n;
  unsigned char *crp = cbp + n / 4;
  dtxy_t x, y;
  
  int *Cr_r_tab;
  int *Cb_b_tab;
  INT32 *Cr_g_tab;
  INT32 *Cb_g_tab;
  unsigned char *clip;
  unsigned char *rgb888e = rgb888;
  unsigned char *rgb888o = rgb888 + 3 * xe;

  Cr_r_tab = (int *)(convert->space);
  Cb_b_tab = (int *)(Cr_r_tab + 256);
  Cr_g_tab = (INT32 *)(Cb_b_tab + 256);
  Cb_g_tab = (INT32 *)(Cr_g_tab + 256);
  clip = (unsigned char *)(Cb_g_tab + 256) + 128;

  for (y=0; y<ye; y+=2)
  {
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "y=%u", y);
    for (x=0; x<xe; x+=2)
    {
	  int luminance;
	  const int cb = *cbp++;
	  const int cr = *crp++;
  	  const int rb = RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr], SCALEBITS);

      luminance = *ype++;
	  *rgb888e++ = clip[luminance + Cr_r_tab[cr]];
	  *rgb888e++ = clip[luminance + rb];
	  *rgb888e++ = clip[luminance + Cb_b_tab[cb]];

	  luminance = *ype++;
	  *rgb888e++ = clip[luminance + Cr_r_tab[cr]];
	  *rgb888e++ = clip[luminance + rb];
	  *rgb888e++ = clip[luminance + Cb_b_tab[cb]];

      luminance = *ypo++;
	  *rgb888o++ = clip[luminance + Cr_r_tab[cr]];
	  *rgb888o++ = clip[luminance + rb];
	  *rgb888o++ = clip[luminance + Cb_b_tab[cb]];

	  luminance = *ypo++;
	  *rgb888o++ = clip[luminance + Cr_r_tab[cr]];
	  *rgb888o++ = clip[luminance + rb];
	  *rgb888o++ = clip[luminance + Cb_b_tab[cb]];
	}
	ype += xe;
	ypo += xe;
    rgb888o += 3 * xe;
    rgb888e += 3 * xe;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_ycbcr_init(
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
dt_color_y_cb_cr_411_to_ycbcr_uninit(
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
dt_color_y_cb_cr_411_to_ycbcr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr_411,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *ycbcr)
{
  DT_F("dt_color_y_cb_cr_411_to_ycbcr");
  dtxy_t x, y;
  const unsigned int n = xe * ye;
  unsigned char *yp = y_cb_cr_411;
  unsigned char *cbp = yp + n;
  unsigned char *crp = yp + n + n / 4;
  unsigned char *p = ycbcr;
  unsigned char y1, y2, cb, cr;

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "here");

  for (y=0; y<ye; y+=2)
  {
	for (x=0; x<xe; x+=2)
	{
      y1 = *yp++;
      cb = *cbp++;
      y2 = *yp++;
      cr = *crp++;
      *p++ = y1;
      *p++ = cb;
      *p++ = cr;
      *p++ = y2;
      *p++ = cb;
      *p++ = cr;
	}
    cbp -= x / 2;
    crp -= x / 2;
	for (x=0; x<xe; x+=2)
	{
      y1 = *yp++;
      cb = *cbp++;
      y2 = *yp++;
      cr = *crp++;
      *p++ = y1;
      *p++ = cb;
      *p++ = cr;
      *p++ = y2;
      *p++ = cb;
      *p++ = cr;
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_r_g_b_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_r_g_b_init");
  DT_Q(dt_color_ycbcr_to_rgb_init,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_r_g_b_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_r_g_b_uninit");
  DT_Q(dt_color_ycbcr_to_rgb_uninit,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * y_cb_cr for n=2 is Y_Cb_Cr
 * r_g_b for n=2 is rgbrgb
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_r_g_b(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				/* input in form Y_Cb_Cr			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *r_g_b)				/* output in form rgbrgb 			*/
{
  const unsigned int n = xe * ye;
  unsigned char *r = r_g_b;
  unsigned char *g = r_g_b + n;
  unsigned char *b = r_g_b + 2 * n;
  unsigned char *yp = y_cb_cr;
  unsigned char *cbp = y_cb_cr + n;
  unsigned char *crp = y_cb_cr + 2 * n;
  unsigned int i;
  
  int *Cr_r_tab;
  int *Cb_b_tab;
  INT32 *Cr_g_tab;
  INT32 *Cb_g_tab;
  unsigned char *clip;

  Cr_r_tab = (int *)(convert->space);
  Cb_b_tab = (int *)(Cr_r_tab + 256);
  Cr_g_tab = (INT32 *)(Cb_b_tab + 256);
  Cb_g_tab = (INT32 *)(Cr_g_tab + 256);
  clip = (unsigned char *)(Cb_g_tab + 256) + 128;

  for (i=0; i<n; i++)
  {
	const int y  = *yp++;
	const int cb = *cbp++;
	const int cr = *crp++;
	const int rb = RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr], SCALEBITS);

	*r++ = clip[y + Cr_r_tab[cr]];
	*g++ = clip[y + rb];
	*b++ = clip[y + Cb_b_tab[cb]];
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to3_r_g_b(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *yp,
  unsigned char *cbp,
  unsigned char *crp,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b)
{
  const unsigned int n = xe * ye;
  unsigned int i;
  
  int *Cr_r_tab;
  int *Cb_b_tab;
  INT32 *Cr_g_tab;
  INT32 *Cb_g_tab;
  unsigned char *clip;

  Cr_r_tab = (int *)(convert->space);
  Cb_b_tab = (int *)(Cr_r_tab + 256);
  Cr_g_tab = (INT32 *)(Cb_b_tab + 256);
  Cb_g_tab = (INT32 *)(Cr_g_tab + 256);
  clip = (unsigned char *)(Cb_g_tab + 256) + 128;

  for (i=0; i<n; i++)
  {
	const int y  = *yp++;
	const int cb = *cbp++;
	const int cr = *crp++;
	const int rb = RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr], SCALEBITS);

	*r++ = clip[y + Cr_r_tab[cr]];
	*g++ = clip[y + rb];
	*b++ = clip[y + Cb_b_tab[cb]];
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_pseudo_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_pseudo_init");
  DT_MEMSET(convert, 0, sizeof(*convert));
  DT_Q(dt_color_y_cb_cr_to_r_g_b_init,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_pseudo_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_pseudo_uninit");
  DT_Q(dt_color_y_cb_cr_to_r_g_b_uninit,(ctl, convert));
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo)
{
  DT_F("dt_color_y_cb_cr_to_pseudo");
# if DTPRJ_XV != DT_1
  return dt_assert_defined(ctl,
    0, F, "DTPRJ_XV310A",
    "XV color conversion library");
# else

  unsigned char *rlut;
  unsigned char *glut;
  unsigned char *blut;
  unsigned int n = xe * ye;
  int xvrc;

  DT_Q(dtos_malloc2,(ctl,				/* space for separate luts 			*/
    (void **)&rlut, 256 * 3,
    F, "component luts"));
  
  glut = rlut + 256;
  blut = rlut + 512;

  xvrc = xv310a_slow_quant3(			/* quantize in ycrcb space 			*/
    y_cb_cr+2*n, 
    y_cb_cr+0*n,						/* green gets most of the bits 		*/
    y_cb_cr+1*n, 
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
	DT_Q(dt_color_y_cb_cr_to3_r_g_b,(	/* convert lut to rgb 				*/
      ctl,
      convert,
      glut, blut, rlut,
      256, 1,
      rlut, glut, blut));
    lut8 = (dtlut8_t *)					/* multiplexed lut after data 		*/
	  (pseudo + xe * ye);
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
dt_color_y_cb_cr_to3_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y,
  unsigned char *cb,
  unsigned char *cr,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
{
  DT_F("dt_color_y_cb_cr_to_pseudo");
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
  
  xvrc = xv310a_slow_quant3(			/* quantize in ycrcb space 			*/
    cr, 
    y,									/* green gets most of the bits 		*/
    cb, 
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
	DT_Q(dt_color_y_cb_cr_to3_r_g_b,(	/* convert lut to rgb 				*/
      ctl,
      convert,
      glut, blut, rlut,
      256, 1,
      rlut, glut, blut));
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
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_map_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_map_init");
  DT_MEMSET(convert, 0, sizeof(*convert));
  DT_Q(dt_color_y_cb_cr_to_r_g_b_init,(ctl, convert));
  
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
dt_color_y_cb_cr_to_map_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_y_cb_cr_to_map_uninit");
  if (convert->rlut != NULL)
    DT_Q(dtos_free2,(ctl,				/* free separate luts 				*/
      convert->rlut, 
      F, "component luts"));
  DT_Q(dt_color_y_cb_cr_to_r_g_b_uninit,(ctl, convert));
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to3_map(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y,
  unsigned char *cb,
  unsigned char *cr,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
{
  DT_F("dt_color_y_cb_cr_to_map");
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
  
  xvrc = xv310a_slow_quant3(			/* quantize in ycrcb space 			*/
    cr, 
    y,									/* green gets most of the bits 		*/
    cb, 
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
	DT_MEMCPY(convert->rlut, rlut, 256);	/* keep a ycbcr copy 				*/
	DT_MEMCPY(convert->glut, glut, 256);
	DT_MEMCPY(convert->blut, blut, 256);
	DT_Q(dt_color_y_cb_cr_to3_r_g_b,(	/* convert lut to rgb 				*/
      ctl,
      convert,
      glut, blut, rlut,
      256, 1,
      rlut, glut, blut));
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
    cr, 
    y,									
    cb, 
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





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
