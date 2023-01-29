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
DT_RCSID("source/high $RCSfile: err.c,v $ $Revision: 1.1 $");

/*..........................................................................
| NAME
| dtimage_err_outside() - output extent exceeded error message
| 
| SUMMARY
| Output a consistently formatted error message
| indicating that a referenced pixel location is outside some image's extent.
| Even though given the offending position, do not check the extent:
| just issue an error message.
| Always return an error.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_err_outside(					/* error due to pixel outside image	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image being exceeded				*/
  const char *desc,						/* image description 				*/
  const dtxy_t x,						/* the offending column				*/
  const dtxy_t y,						/* the offending row 				*/
  char *f)								/* caller ID 						*/
{
  return dt_err(ctl, f, 
    "pixel (%d,%d) outside %s image size, (%d,%d)",
    x, y,
    desc,
	image->xe, image->ye);
}

/*..........................................................................
| NAME
| dtimage_err_ntype() - output an improper datatype error message
| 
| SUMMARY
| Output a consistently formatted error message
| indicating that a particular data type is invalid for some operation.
| Presume that the ~image->ntype~ is the offending data type.
| Expect the list of valid data types to be NULL terminated.
| Always return an error.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_err_ntype(						/* error because can't do ntype		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image whose ntype is wrong		*/
  const char *desc,						/* image description 				*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f)						/* caller ID 						*/
{
  if ((int)image->ntype > 0 &&
	  (int)image->ntype <=
	  dt_ntype_count)
    return dt_err(ctl, f, 
      "operation not defined for %s type %s",
      desc? desc: "image",
      dt_ntype_string[image->ntype]);
  else
    return dt_err(ctl, f, 
      "operation not defined for %s unknown type 0x%04x",
      desc? desc: "image",
      (unsigned int)image->ntype);
}

/*..........................................................................
| NAME
| dtimage_err_ntype_2() - output an incompatible image error message
| 
| SUMMARY
| Output a consistently formatted error message
| indicating that the data types of two given images are not compatible.
| Always return an error.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_err_ntype_2(					/* error because ntypes differ		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image						*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image 					*/
  const char *desc2,					/* image description string 		*/
  const char *f)						/* caller ID 						*/
{
  return dt_err(ctl, f, 
    "%s type %s not compatable with %s type %s",
    desc1,
    dt_ntype_string[image1->ntype],
    desc2,
    dt_ntype_string[image2->ntype]);
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
