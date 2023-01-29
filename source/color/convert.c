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

DT_RCSID("source/color $RCSfile: convert.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_convert_cant(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  dt_color_org_t *org1,
  dt_color_org_t *org2)
{
  DT_F("dt_color_org_convert_cant");
  char s1[32];
  char s2[32];
  dt_color_org_string(ctl,
    org1, s1, sizeof(s1));
  dt_color_org_string(ctl,
    org2, s2, sizeof(s2));
  return dt_err(ctl, F,
    "can't convert between "
	"%s (%d) and %s (%d)", 
    s1, org1->org, 
    s2, org2->org);
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_convert_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  dt_color_org_t *org1,
  dt_color_org_t *org2)
{
  DT_F("dt_color_org_convert_init");

# define CASE(ORG1, ORG2) ((int)(ORG1) * 1000 + (int)(ORG2))
  long value = CASE(org1->org, org2->org);
  switch(value)
  {
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_ycbycr_to_luminance_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_ycbycr_to_rgb888_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_Y_CB_CR):
	  DT_Q(dt_color_ycbycr_to_y_cb_cr_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_YCBCR):
	  DT_Q(dt_color_ycbycr_to_ycbcr_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_Y_CB_CR_411):
	  DT_Q(dt_color_ycbycr_to_y_cb_cr_411_init,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_YUYV,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_yuyv_to_luminance_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YUYV,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_yuyv_to_rgb888_init,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_RGB565,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_rgb565_to_luminance_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_RGB565,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_rgb565_to_rgb888_init,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_RGB555,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_rgb555_to_luminance_init,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_RGB555,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_rgb555_to_rgb888_init,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_RGB888,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_rgb888_to_pseudo_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_Y_CB_CR):
	  DT_Q(dt_color_r_g_b_to_y_cb_cr_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_r_g_b_to_pseudo_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_MAP):
	  DT_Q(dt_color_r_g_b_to_map_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_LUMINANCE):
	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_y_cb_cr_to_luminance_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_y_cb_cr_to_rgb888_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_y_cb_cr_411_to_rgb888_init,(ctl,
        convert));
  	break;
	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_YCBCR):
	  DT_Q(dt_color_y_cb_cr_411_to_ycbcr_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_y_cb_cr_to_pseudo_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_R_G_B):
	  DT_Q(dt_color_y_cb_cr_to_r_g_b_init,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_MAP):
	  DT_Q(dt_color_y_cb_cr_to_map_init,(ctl,
        convert));
  	break;

	default:
	  DT_Q(dt_color_convert_cant,(
	    ctl, convert, org1, org2));
  }

  convert->initialized = 1;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_convert_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  dt_color_org_t *org1,
  dt_color_org_t *org2)
{
  DT_F("dt_color_org_convert_init");
  long value = CASE(org1->org, org2->org);
  switch(value)
  {
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_ycbycr_to_luminance_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_ycbycr_to_rgb888_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_Y_CB_CR):
	  DT_Q(dt_color_ycbycr_to_y_cb_cr_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_YCBCR):
	  DT_Q(dt_color_ycbycr_to_ycbcr_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_Y_CB_CR_411):
	  DT_Q(dt_color_ycbycr_to_y_cb_cr_411_uninit,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_YUYV,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_yuyv_to_luminance_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_YUYV,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_yuyv_to_rgb888_uninit,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_RGB565,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_rgb565_to_luminance_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_RGB565,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_rgb565_to_rgb888_uninit,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_RGB555,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_rgb555_to_luminance_uninit,(ctl,
        convert));
	break;
	case CASE(DT_COLOR_ORG_RGB555,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_rgb555_to_rgb888_uninit,(ctl,
        convert));
	break;

	case CASE(DT_COLOR_ORG_RGB888,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_rgb888_to_pseudo_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_Y_CB_CR):
	  DT_Q(dt_color_r_g_b_to_y_cb_cr_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_r_g_b_to_pseudo_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_MAP):
	  DT_Q(dt_color_r_g_b_to_map_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_LUMINANCE):
	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_y_cb_cr_to_luminance_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_y_cb_cr_to_rgb888_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_y_cb_cr_411_to_rgb888_uninit,(ctl,
        convert));
  	break;
	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_YCBCR):
	  DT_Q(dt_color_y_cb_cr_411_to_ycbcr_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_y_cb_cr_to_pseudo_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_R_G_B):
	  DT_Q(dt_color_y_cb_cr_to_r_g_b_uninit,(ctl,
        convert));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_MAP):
	  DT_Q(dt_color_y_cb_cr_to_map_uninit,(ctl,
        convert));
  	break;

	default:
	  DT_Q(dt_color_convert_cant,(
	    ctl, convert, org1, org2));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_convert(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  void *vb1,
  dt_color_org_t *org1,
  dtxy_t xe,
  dtxy_t ye,
  void *vb2,
  dt_color_org_t *org2)
{
  DT_F("dt_color_org_convert");
  unsigned char *b1 = (unsigned char *)vb1;
  unsigned char *b2 = (unsigned char *)vb2;
# define CASE(ORG1, ORG2) ((int)(ORG1) * 1000 + (int)(ORG2))
  long value = CASE(org1->org, org2->org);
  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "converting %d -> %d",
    org1->org, org2->org);
  switch(value)
  {
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_ycbycr_to_luminance,(ctl,
        convert, b1, xe, ye, b2));
  	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_ycbycr_to_rgb888,(ctl,
        convert, b1, xe, ye, b2));
  	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_Y_CB_CR):
	  DT_Q(dt_color_ycbycr_to_y_cb_cr,(ctl,
        convert, b1, xe, ye, b2));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_YCBCR):
	  DT_Q(dt_color_ycbycr_to_ycbcr,(ctl,
        convert, b1, xe, ye, b2));
	break;
	case CASE(DT_COLOR_ORG_YCBYCR,
			  DT_COLOR_ORG_Y_CB_CR_411):
	  DT_Q(dt_color_ycbycr_to_y_cb_cr_411,(ctl,
        convert, b1, xe, ye, b2));
	break;
	
	case CASE(DT_COLOR_ORG_YUYV,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_yuyv_to_luminance,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_YUYV,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_yuyv_to_rgb888,(ctl,
        convert, b1, xe, ye, b2));
  	break;
	
	case CASE(DT_COLOR_ORG_RGB565,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_rgb565_to_luminance,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_RGB565,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_rgb565_to_rgb888,(ctl,
        convert, b1, xe, ye, b2));
  	break;
	
	case CASE(DT_COLOR_ORG_RGB555,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_rgb555_to_luminance,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_RGB555,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_rgb555_to_rgb888,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_RGB888,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_rgb888_to_pseudo,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_LUMINANCE):
	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_LUMINANCE):
	  DT_Q(dt_color_y_cb_cr_to_luminance,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_y_cb_cr_to_rgb888,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_RGB888):
	  DT_Q(dt_color_y_cb_cr_411_to_rgb888,(ctl,
        convert, b1, xe, ye, b2));
  	break;
	case CASE(DT_COLOR_ORG_Y_CB_CR_411,
			  DT_COLOR_ORG_YCBCR):
	  DT_Q(dt_color_y_cb_cr_411_to_ycbcr,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_y_cb_cr_to_pseudo,(ctl,
        convert, b1, xe, ye, b2));
  	break;

	default:
	  DT_Q(dt_color_convert_cant,(
	    ctl, convert, org1, org2));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_convert3(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  void *vb1a,
  void *vb1b,
  void *vb1c,
  dt_color_org_t *org1,
  dtxy_t xe,
  dtxy_t ye,
  void *vb2a,
  void *vb2b,
  void *vb2c,
  dt_color_org_t *org2)
{
  DT_F("dt_color_org_convert3");
  long value = CASE(org1->org, org2->org);
  unsigned char *b1a = (unsigned char *)vb1a;
  unsigned char *b1b = (unsigned char *)vb1b;
  unsigned char *b1c = (unsigned char *)vb1c;
  unsigned char *b2a = (unsigned char *)vb2a;
  unsigned char *b2b = (unsigned char *)vb2b;
  unsigned char *b2c = (unsigned char *)vb2c;

  switch(value)
  {
	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_r_g_b_to3_pseudo,(ctl,
        convert, b1a, b1b, b1c, xe, ye, b2a, b2b));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_MAP):
	  DT_Q(dt_color_r_g_b_to3_map,(ctl,
        convert, b1a, b1b, b1c, xe, ye, b2a, b2b));
  	break;

	case CASE(DT_COLOR_ORG_R_G_B,
			  DT_COLOR_ORG_Y_CB_CR):
	  DT_Q(dt_color_r_g_b_to3_y_cb_cr,(ctl,
        convert, b1a, b1b, b1c, xe, ye, b2a, b2b, b2c));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_R_G_B):
	  DT_Q(dt_color_y_cb_cr_to3_r_g_b,(ctl,
        convert, b1a, b1b, b1c, xe, ye, b2a, b2b, b2c));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_PSEUDO):
	  DT_Q(dt_color_y_cb_cr_to3_pseudo,(ctl,
        convert, b1a, b1b, b1c, xe, ye, b2a, b2b));
  	break;

	case CASE(DT_COLOR_ORG_Y_CB_CR,
			  DT_COLOR_ORG_MAP):
	  DT_Q(dt_color_y_cb_cr_to3_map,(ctl,
        convert, b1a, b1b, b1c, xe, ye, b2a, b2b));
  	break;

	default:
	  DT_Q(dt_color_convert_cant,(
	    ctl, convert, org1, org2));
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
