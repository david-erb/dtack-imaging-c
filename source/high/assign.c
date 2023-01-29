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
#include <dtack/mem.h>
#include <dtack/os.h>

DT_RCSID("source/high $RCSfile: assign.c,v $ $Revision: 1.1 $");

/*..........................................................................
| NAME
| dtimage_assign() - assign pixels from variable arguments
| 
| SUMMARY
| Assign pixel values from variable arguments.
| Assume there are exactly ~xe~ times ~ye~ arguments of type ~ntype~.
| Ignore pixels from argument list outside the size of the image.
| Likewise, do not touch image pixels image outside the argument range.
| Convert pixel types according to standard Dtack type conversion rules.
|
| These are slow functions and should not be used for large images.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_assign_va(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of args				*/
  va_list ap)							/* variable arguments 				*/
{
  DT_F("dtimage_assign_va");
  dtxy_t x, y;
  dt_rc_e rc;

#define CASE(E, T)						/* enumeration->datatype case 		*/\
  case E: v = va_arg(ap, T); break;

  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe; x++)
	{
	  double v;
	  switch(ntype)
	  {
		DTNTYPE_CASES					/* fetch value from args 			*/
		default:
		  return dt_err(ctl, F,
            "unrecognized ntype %d", 
            ntype);
	  }
	  if (x < image->xe &&
		  y < image->ye)
	  {
		DT_Q(dtimage_set_pixel,(ctl,	/* assign value to pixel 			*/
          image, x, y, v));
	  }
	}
  }

  return DT_RC_GOOD;
}
#undef CASE

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtimage_assign(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of args				*/
  ...)									/* variable arguments 				*/
{
  DT_F("dtimage_assign");

  va_list ap;
  va_start(ap, ntype);

  DT_Q(dtimage_assign_va,(ctl, image,	/* use other function 				*/
    xe, ye, ntype, ap));
  
  va_end(ap);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_assign_array() - assign pixels from array
| 
| SUMMARY
| Assign pixel values taken from ~array~ which is of type ~ntype~.
| Assume there are exactly ~xe~ times ~ye~ elements.
| Ignore pixels from array outside the size of the image.
| Likewise, do not touch image pixels image outside the array range.
| Convert pixel types according to standard Dtack type conversion rules.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_assign_array(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of array values		*/
  const void *array)					/* array containing values			*/
{
  DT_F("dtimage_assign_array");
  dtxy_t x, y;
  int i = 0;

#define CASE(E, T)						/* enumeration->datatype case 		*/\
  case E: v = (double)((T *)array)[i++]; break;

  for (y=0; y<ye; y++)
  {
	for (x=0; x<xe; x++)
	{
	  double v;
	  switch(ntype)
	  {
		DTNTYPE_CASES					/* fetch value from args 			*/
		default:
		  return dt_err(ctl, F,
            "unrecognized ntype %d", 
            ntype);
	  }
	  if (x < image->xe &&
		  y < image->ye)
		DT_Q(dtimage_set_pixel,(ctl,	/* assign value to pixel 			*/
          image, x, y, v));
	}
  }

  return DT_RC_GOOD;
}
#undef CASE





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
