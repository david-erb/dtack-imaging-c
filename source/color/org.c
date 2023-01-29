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
#include <dtack/str.h>

#include <dtack/lut8.h>
DT_RCSID("source/color $RCSfile: org.c,v $ $Revision: 1.8 $");

/*..........................................................................*/
const char *org_alias_s[] = 
{
  "@@@nodefault@@@",
  "y",
  "luminance",
  "ycbycr",
  "YCBYCR",
  "ycbcr",
  "YCBCR",
  "Y_CB_CR",							/* this is 4:4:4 band sequential 	*/
  "y_cb_cr",
  "Y_CB_CR_411",						/* this is 4:1:1 band sequential 	*/
  "y_cb_cr_411",
  "CB",
  "cb",
  "CR",
  "cr",
  "yuyv",
  "YUYV",
  "rgb888",
  "RGB888",
  "r_g_b",
  "R_G_B",
  "rgb565",
  "RGB565",
  "rgb555",
  "RGB555",
  "PSEUDO",
  "pseudo",
  "MAP",
  "map",
  "bmp24",
  "bmp24",
  NULL
};

dt_color_org_e org_alias_v[] = 
{
  DT_COLOR_ORG_UNKNOWN,
  DT_COLOR_ORG_LUMINANCE,
  DT_COLOR_ORG_LUMINANCE,
  DT_COLOR_ORG_YCBYCR,
  DT_COLOR_ORG_YCBYCR,
  DT_COLOR_ORG_YCBCR,
  DT_COLOR_ORG_YCBCR,
  DT_COLOR_ORG_Y_CB_CR,
  DT_COLOR_ORG_Y_CB_CR,
  DT_COLOR_ORG_Y_CB_CR_411,
  DT_COLOR_ORG_Y_CB_CR_411,
  DT_COLOR_ORG_CB,
  DT_COLOR_ORG_CB,
  DT_COLOR_ORG_CR,
  DT_COLOR_ORG_CR,
  DT_COLOR_ORG_YUYV,
  DT_COLOR_ORG_YUYV,
  DT_COLOR_ORG_RGB888,
  DT_COLOR_ORG_RGB888,
  DT_COLOR_ORG_R_G_B,
  DT_COLOR_ORG_R_G_B,
  DT_COLOR_ORG_RGB565,
  DT_COLOR_ORG_RGB565,
  DT_COLOR_ORG_RGB555,
  DT_COLOR_ORG_RGB555,
  DT_COLOR_ORG_PSEUDO,
  DT_COLOR_ORG_PSEUDO,
  DT_COLOR_ORG_MAP,
  DT_COLOR_ORG_MAP,
  DT_COLOR_ORG_BMP24,
  DT_COLOR_ORG_BMP24
};

const char *org_list_name[DT_COLOR_ORG_COUNT+1] = 
{
  "luminance",
  "ycbycr",
  "ycbcr",
  "y_cb_cr",
  "y_cb_cr_411",
  "cb",
  "cr",
  "yuyv",
  "rgb888",
  "r_g_b",
  "rgb565",
  "rgb555",
  "pseudo",
  "map",
  "bmp24",
  "yuy2",
  "rgb32",
  "uyvy",
  NULL
};

double org_list_size[DT_COLOR_ORG_COUNT] = 
{
  1.0,									/* luminance only 					*/
  2.0,									/* ycbycr 							*/
  3.0,									/* ycbcr 							*/
  3.0,									/* y_cb_cr 							*/
  1.5,									/* y_cb_cr_411						*/
  1.0,									/* cb 								*/
  1.0,									/* cr 								*/
  2.0,									/* yuyv								*/
  3.0,									/* rgb888 							*/
  3.0,									/* r_g_b 							*/
  2.0,									/* rgb565 							*/
  2.0,									/* rgb555 							*/
  1.0,									/* pseudo 							*/
  1.0,									/* map	 							*/
  3.0,									/* bmp24 							*/
  2.0,									/* yuy2  */
  4.0,									/* rgb32 */
  2.0,									/* uyvy */
};

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_org_parse(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  const char *string,
  char *message,
  int max)
{
  DT_F("dt_color_org_parse");
  int choice;

  message[0] = '\0';

  DT_Q(dtstr_choice,(ctl,				/* look up org in alias list		*/
    org_alias_s,
    string, &choice));
  
  if (choice == -1)
  {
    DT_Q(dtstr_printf,(ctl, 
      message, max,
      "Sorry, color org \"%s\""
      " is not understood.", 
      string));
  }

  if (choice < -1)
  {
    DT_Q(dtstr_printf,(ctl, 
      message, max,
      "Sorry, color org \"%s\""
      " is ambiguous (%d matches).",
      string, -choice));
  }

  org->org = org_alias_v[choice];

  return DT_RC_GOOD;
}

/*..........................................................................
 * initialize org structure given only manifest content
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_org_set(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  dt_color_org_e org_e)
{
  DT_MEMSET(org, 0, sizeof(*org));
  org->org = org_e;
  return DT_RC_GOOD;
}

/*..........................................................................
 * return string describing org
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_org_string(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  char *string,
  int max)
{
  DT_F("dt_color_org_string");
  const char *format;
  if (org->org < DT_COLOR_ORG_COUNT)
	format = org_list_name[(int)org->org];
  else
    format = "unknown (0x%04x)";

  DT_Q(dtstr_printf,(ctl, string, max,
    format, org->org));
  return DT_RC_GOOD;
}

/*..........................................................................
 * return size in bytes for the given number of pixels in the given org
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dt_color_org_size(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  unsigned int npixels,
  unsigned int *nbytes)
{
  DT_F("dt_color_org_size");
  
  if (org->org == DT_COLOR_ORG_PSEUDO)
	*nbytes = (unsigned int)(npixels *
      org_list_size[(int)org->org] +
	  256 * sizeof(dtlut8_t));			/* pseudo has lut at end 			*/
  else
  if (org->org < DT_COLOR_ORG_COUNT)
	*nbytes = (unsigned int)(npixels *
      org_list_size[(int)org->org]);
  else
    return dt_err(ctl, F,
      "invalid org 0x%04x", 
      (int)org->org);

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
