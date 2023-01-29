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

DT_RCSID("source/color $RCSfile: ycbycr.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_luminance_init(
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
dt_color_ycbycr_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * ycbycr for n=2 is Ycbycr
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,				/* input in form Ycbycr				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
{
  unsigned int i;
  const unsigned int n = xe * ye;

  for (i=0; i<n; i++)
  {
	*luminance++ = *ycbycr++;			/* get the luminance 				*/
	ycbycr++;							/* skip the chrominance 			*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#define INT32 long
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))
#define SCALEBITS	16	/* speediest right-shift on some machines */
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))

dt_rc_e
DTCONFIG_API1
dt_color_ycbcr_to_rgb_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_ycbcr_to_rgb_init");
  unsigned int n;
  int i;
  
  int *Cr_r_tab;
  int *Cb_b_tab;
  INT32 *Cr_g_tab;
  INT32 *Cb_g_tab;
  unsigned char *clip;
  long x2;

  DT_MEMSET(convert, 0, sizeof(*convert));

  n = 256 * (
    sizeof(*Cr_r_tab) +
    sizeof(*Cb_b_tab) +
    sizeof(*Cr_g_tab) +
    sizeof(*Cb_g_tab)) + 512;

  DT_Q(dtos_malloc2,(ctl,
    &convert->space, n,
    F, "conversion tables"));

  Cr_r_tab = (int *)(convert->space);
  Cb_b_tab = (int *)(Cr_r_tab + 256);
  Cr_g_tab = (INT32 *)(Cb_b_tab + 256);
  Cb_g_tab = (INT32 *)(Cr_g_tab + 256);
  clip = (unsigned char *)(Cb_g_tab + 256) + 128;

  for (i = 0; i <= 255; i++) 
  {
    /* i is the actual input pixel value, in the range 0..MAXJSAMPLE */
    /* The Cb or Cr value we are thinking of is x = i - MAXJSAMPLE/2 */
    x2 = 2*i - 255;	/* twice x */
    /* Cr=>R value is nearest int to 1.40200 * x */
    Cr_r_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.40200/2) * x2 + ONE_HALF, SCALEBITS);
    /* Cb=>B value is nearest int to 1.77200 * x */
    Cb_b_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.77200/2) * x2 + ONE_HALF, SCALEBITS);
    /* Cr=>G value is scaled-up -0.71414 * x */
    Cr_g_tab[i] = (- FIX(0.71414/2)) * x2;
    /* Cb=>G value is scaled-up -0.34414 * x */
    /* We also add in ONE_HALF so that need not do it in inner loop */
    Cb_g_tab[i] = (- FIX(0.34414/2)) * x2 + ONE_HALF;
  }

  for (i = -128; i < 0; i++)
	clip[i] = 0;
  for (i = 0; i < 255; i++)
	clip[i] = i;
  for (i = 255; i < 512-128; i++)
	clip[i] = 255;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_ycbcr_to_rgb_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_ycbcr_to_rgb_uninit");

  if (convert->space != NULL)
    DT_Q(dtos_free2,(ctl,
      convert->space,
      F, "conversion tables"));

  DT_MEMSET(convert, 0, sizeof(*convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_ycbycr_to_rgb888_init");
  DT_Q(dt_color_ycbcr_to_rgb_init,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
{
  DT_F("dt_color_ycbycr_to_rgb888_uninit");
  DT_Q(dt_color_ycbcr_to_rgb_uninit,(ctl, convert));
  return DT_RC_GOOD;
}

/*..........................................................................
 * ycbycr for n=2 is Ycbycr
 * rgb888 for n=2 is rgbrgb
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,					/* input in form Ycbycr				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)					/* output in form rgbrgb 			*/
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

  for (i=0; i<n; i+=2)
  {
	const int y1 = *ycbycr++;
	const int cb = *ycbycr++;
	const int y2 = *ycbycr++;
	const int cr = *ycbycr++;
	const int rb = RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr], SCALEBITS);

	*rgb888++ = clip[y1 + Cr_r_tab[cr]];
	*rgb888++ = clip[y1 + rb];
	*rgb888++ = clip[y1 + Cb_b_tab[cb]];
	
	*rgb888++ = clip[y2 + Cr_r_tab[cr]];
	*rgb888++ = clip[y2 + rb];
	*rgb888++ = clip[y2 + Cb_b_tab[cb]];
  }

  return DT_RC_GOOD;
}



/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_init(
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
dt_color_ycbycr_to_y_cb_cr_uninit(
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
dt_color_ycbycr_to_y_cb_cr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *y_cb_cr)
{
  DT_F("dt_color_ycbycr_to_y_cb_cr");
  unsigned int i;
  const unsigned int n = xe * ye;
  unsigned char *yp = y_cb_cr;
  unsigned char *cbp = y_cb_cr + n;
  unsigned char *crp = cbp + n;
  unsigned char tmp;

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "back to old code");

  for (i=0; i<n; i+=2)
  {
	*yp++  = *ycbycr++;					/* get the luminance 				*/
	tmp    = *ycbycr++;					/* get Cb 							*/
	if ((i/xe)&1)
      tmp  = *(ycbycr - 2 * xe - 1);
	*cbp++ = tmp;						/* replicate Cb */
	*cbp++ = tmp;
	*yp++  = *ycbycr++;					/* get the next luminance			*/
	tmp    = *ycbycr++;					/* get Cr 							*/
	if ((i/xe)&1)
      tmp  = *(ycbycr - 2 * xe - 1);
	*crp++ = tmp;						/* replicate Cr 					*/
	*crp++ = tmp;
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_411_init(
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
dt_color_ycbycr_to_y_cb_cr_411_uninit(
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
dt_color_ycbycr_to_y_cb_cr_411(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *y_cb_cr)
{
  dtxy_t x, y;
  const unsigned int n = xe * ye;
  unsigned char *yp = y_cb_cr;
  unsigned char *cbp = y_cb_cr + n;
  unsigned char *crp = cbp + n / 4;


  for (y=0; y<ye; y+=2)
  {
	for (x=0; x<xe; x+=2)
	{
	  *yp++  = *ycbycr++;				/* get the luminance 				*/
	  *cbp++ = *ycbycr++;				/* get Cb 							*/
	  *yp++  = *ycbycr++;				/* get the next luminance			*/
	  *crp++ = *ycbycr++;				/* get Cr 							*/
	}
	for (x=0; x<xe; x+=2)
	{
	  *yp++  = *ycbycr++;				/* get the luminance 				*/
	  ycbycr++;							/* ignore Cb 						*/
	  *yp++  = *ycbycr++;				/* get the next luminance			*/
	  ycbycr++;							/* ignore Cr						*/
	}
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_ycbcr_init(
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
dt_color_ycbycr_to_ycbcr_uninit(
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
dt_color_ycbycr_to_ycbcr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *ycbcr)
{
  dtxy_t x, y;
  unsigned char *p = ycbcr;
  unsigned char y1, y2, cb, cr;

  for (y=0; y<ye; y+=2)
  {
	unsigned char *q = p;				/* even row 						*/
	for (x=0; x<xe; x+=2)
	{
      y1 = *ycbycr++;
      cb = *ycbycr++;
      y2 = *ycbycr++;
      cr = *ycbycr++;
      *p++ = y1;
      *p++ = cb;
      *p++ = cr;
      *p++ = y2;
      *p++ = cb;
      *p++ = cr;
	}
	for (x=0; x<xe; x+=2)
	{
      y1 = *ycbycr++;
            ycbycr++;					/* skip odd row blue 				*/
      cb = q[1];						/* get blue from row above 			*/
      y2 = *ycbycr++;
            ycbycr++;					/* skip odd row read 				*/
      cr = q[2];						/* get rid from row above 			*/
      q += 6;							/* skip two triplets on row above 	*/
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





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
