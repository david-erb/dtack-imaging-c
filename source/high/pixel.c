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
| TITLE
| Pixel Access
|
|!Summary
| Provide access to the pixels of an image.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: pixel.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>
#include <dtack/str.h>


#ifdef NONO

/*..........................................................................
| NAME
| DTIMAGE_SETPIX1() - set single pixel to given value
| 
| SUMMARY
| Set a single pixel in an image to a given value.
| Cast the double precision value to the native data type of the image.
| Do not verify that pixel position is within the image extent.
| This macro may generate a large amount of object code.
| 
| END
 *..........................................................................*/

void 
DTCONFIG_API1
DTIMAGE_SETPIX1(						/* set single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double value)							/* pixel value						*/
{
}

/*..........................................................................
| NAME
| DTIMAGE_SETPIXS() - set adjacent pixels to given value
| 
| SUMMARY
| Set adjacent pixels ~x1~ to ~x2~, inclusive,
| on a row in an image to a given value.
| Cast the double precision value to the native data type of the image.
| Do not verify that pixel positions are within the image extent.
| This macro may generate a large amount of object code.
| 
| END
 *..........................................................................*/

void 
DTCONFIG_API1
DTIMAGE_SETPIXS(						/* set single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x1,							/* first pixel column location		*/
  dtxy_t x2,							/* final pixel column location		*/
  dtxy_t y,								/* pixel row location 				*/
  double value)							/* pixel value						*/
{
}

/*..........................................................................
| NAME
| DTIMAGE_SETPIXN() - set single pixel to given value
| 
| SUMMARY
| Set a single pixel in an image to a given value.
| Cast the double precision value to the given TYPEDEF.
| Do not verify that pixel position is within the image extent.
| This macro is much smaller and faster than DTIMAGE_SET_PIXEL().
| 
| END
 *..........................................................................*/

void 
DTCONFIG_API1
DTIMAGE_SETPIXN(						/* set single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  dt_ntype_def ntype_def,				/* image data type 					*/
  double value)							/* pixel value						*/
{
}

/*..........................................................................
| NAME
| DTIMAGE_GETPIX1() - get single pixel to given value
| 
| SUMMARY
| Fetch a single pixel from the given image.
| Convert the value from the native data type of the image
| to double precision for output.
| Do not verify that pixel position is within the image extent.
| This macro may generate a large amount of object code.
| 
| END
 *..........................................................................*/

void 
DTCONFIG_API1
DTIMAGE_GETPIX1(						/* get single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to get	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double *value)						/* returned pixel value				*/
{
}

/*..........................................................................
| NAME
| DTIMAGE_GETPIXN() - get single pixel value
| 
| SUMMARY
| Fetch a single pixel from the given image.
| Convert the value from the TYPEDEF type to double precision for output.
| Do not verify that pixel position is within the image extent.
| This macro is much smaller and faster than DTIMAGE_GET_PIXEL().
| 
| END
 *..........................................................................*/

void 
DTCONFIG_API1
DTIMAGE_GETPIXN(						/* get single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to get	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  dt_ntype_def ntype_def,				/* image data type 					*/
  double *value)						/* returned pixel value				*/
{
}

#endif

/*..........................................................................
| NAME
| dtimage_set_pixel() - set single pixel to given value
| 
| SUMMARY
| Set a single pixel in an image to a given value.
| Cast the double precision value to the native data type of the image.
| Have an error if the pixel location is outside the image extent.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_set_pixel(						/* set single pixel to given value	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double value)							/* pixel value						*/
{
  DT_F("dtimage_set_pixel");
  dt_rc_e rc = DT_RC_GOOD;
  static dt_ntype_e ntypes[] = {
    DT_NTYPE_B01,
    DT_NTYPE_B08,
    DT_NTYPE_B16,
    DT_NTYPE_B32,
    DT_NTYPE_S1,
    DT_NTYPE_S2,
    DT_NTYPE_S4,
    DT_NTYPE_F1,
    DT_NTYPE_F2,
    DT_NTYPE_NULL
  };
  int i;

  if (x >= image->xe ||
      y >= image->ye)
    return dt_err(ctl, F,
      "pixel loc (%u,%u) outside image (0,0)-(%u,%u)",
	  x, y, image->xe, image->ye);
  
  for (i=0; 
       ntypes[i] != DT_NTYPE_NULL &&
       ntypes[i] != image->ntype;
       i++);
  if (ntypes[i] == DT_NTYPE_NULL)
    return dtimage_err_ntype(ctl,		/* report on ntype problem			*/
      image, "image",
      ntypes, F);

  DTIMAGE_SETPIX1(DT_Q, ctl, image,
    x, y, value);

  return rc;
}

/*..........................................................................
| NAME
| dtimage_get_pixel() - get single pixel value from image
| 
| SUMMARY
| Fetch a single pixel from the given image.
| Convert the value from the native data type of the image
| to digital precision for output.
| Have an error if the pixel location is outside the image extent.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_get_pixel(						/* get pixel value					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to get	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double *value)						/* pixel value returned				*/
{
  DT_F("dtimage_get_pixel");
  dt_rc_e rc = DT_RC_GOOD;
  static dt_ntype_e ntypes[] = {
    DT_NTYPE_B01,
    DT_NTYPE_B08,
    DT_NTYPE_B16,
    DT_NTYPE_B32,
    DT_NTYPE_S1,
    DT_NTYPE_S2,
    DT_NTYPE_S4,
    DT_NTYPE_F1,
    DT_NTYPE_F2,
    DT_NTYPE_NULL
  };
  int i;

  if (x >= image->xe ||
      y >= image->ye)
    return dt_err(ctl, F,
      "pixel loc (%u,%u) outside image (0,0)-(%u,%u)",
	  x, y, image->xe, image->ye);
  
  for (i=0; 
       ntypes[i] != DT_NTYPE_NULL &&
       ntypes[i] != image->ntype;
       i++);
  if (ntypes[i] == DT_NTYPE_NULL)
    return dtimage_err_ntype(ctl,		/* report on ntype problem			*/
      image, "image",
      ntypes, F);

  DTIMAGE_GETPIX1(DT_Q, ctl, image,
    x, y, value);

  return rc;
}

/*..........................................................................
| NAME
| dtimage_sprintf_pixel() - convert binary pixel value to ASCII
| 
| SUMMARY
| Fetch a single pixel from the given image,
| and convert its binary value to ASCII.
| The ASCII form of the output string depends on the image data type.
| Silently truncate the output string if not long enough.
| Have an error if the pixel location is outside the image extent.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_sprintf_pixel(					/* convert pixel binary to ASCII 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing the pixel 		*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  char *string,							/* returned ASCII string 			*/
  int stringmax)						/* max length of string 			*/
{
  DT_F("dtimage_sprintf_pixel");
  double v;
  DT_Q(dtimage_get_pixel,(ctl,
    image, x, y, &v));
  switch(image->ntype)
  {
    case DT_NTYPE_B01:
      dtstr_printf(ctl, string, stringmax, " %d", (int)v);
    break;
    case DT_NTYPE_B08:
      dtstr_printf(ctl, string, stringmax, " 0x%02x", (int)v);
    break;
    case DT_NTYPE_B16:
      dtstr_printf(ctl, string, stringmax, " 0x%04x", (int)v);
    break;
    case DT_NTYPE_B32:
      dtstr_printf(ctl, string, stringmax, " 0x%08lx", (long)v);
    break;
    case DT_NTYPE_S1:
      dtstr_printf(ctl, string, stringmax, " %3d", (int)v);
    break;
    case DT_NTYPE_S2:
      dtstr_printf(ctl, string, stringmax, " %5d", (int)v);
    break;
    case DT_NTYPE_S4:
      dtstr_printf(ctl, string, stringmax, " %8ld", (long)v);
    break;
    case DT_NTYPE_F1:
      dtstr_printf(ctl, string, stringmax, " %13e", v);
    break;
    case DT_NTYPE_F2:
      dtstr_printf(ctl, string, stringmax, " %13e", v);
    break;
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
