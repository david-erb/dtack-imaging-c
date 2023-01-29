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
| Checking/Error/Debug
|
|!Summary
| Provide convenience functions for image type checking,
| error reporting, and operational debugging.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: check.c,v $ $Revision: 1.1 $");

/*..........................................................................
| NAME
| dtimage_check_1f() - simple image check
| 
| SUMMARY
| Check that an image's data type is in a given NULL-terminated
| list of types.
| Bypass this check if ~ntypes~ is NULL.
| If the :DTIMAGE_FLAG_PAGED: bit in ~flags~ is not set,
| verify that the image is not marked as paged.
| Have an error if any check fails.
| Issue a :DT_DBG_MASK_IMG: debug message.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_check_1f(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image to be checked				*/
  const char *desc,						/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  unsigned long flags,					/* flags describing needs 			*/
  const char *f)						/* caller ID 						*/
{
  int i;

  if (desc)								/* caller is giving a description? 	*/
    dtimage_dbg_op(ctl, image,			/* debug operation					*/
      desc, f);

  if (!(flags & DTIMAGE_FLAG_PAGED) &&	/* caller is disallowing paging?    */
      image->flags &					/* but image is paged			    */
      DTIMAGE_FLAG_PAGED)
    return dt_err(ctl, f,
      "paged images not suppored");

#if 0
  if (!(flags & DTIMAGE_FLAG_SEGMENTED) &&	/* caller is disallowing paging?    */
      image->flags &					/* but image is paged			    */
      DTIMAGE_FLAG_SEGMENTED)
    return dt_err(ctl, f,
      "segmented images not suppored");
#endif

  if (ntypes == NULL)					/* caller has given an ntype list?	*/
    ntypes = dt_ntype_list;

  for (i=0; ntypes[i] != 0; i++)		/* scan list of valid ntypes		*/
    if (image->ntype == ntypes[i])
      break;  

  if (ntypes[i] == 0)
    return dtimage_err_ntype(ctl,		/* can't do op for ntype			*/
      image, desc, ntypes, f);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_check_2f() - simple two image check
| 
| SUMMARY
| Check that two images' data types are in a given NULL-terminated
| list of types.
| Bypass this check if ~ntypes~ is NULL.
| If the :DTIMAGE_FLAG_PAGED: bit in ~flags~ is not set,
| verify that neither image is not marked as paged.
| Verify that the two images have the same data type.
| Have an error if any check fails.
| Issue a :DT_DBG_MASK_IMG: debug message.
| Compute the smaller of the two images in both dimensions.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_check_2f(						/* simple two image check			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  unsigned long flags,					/* flags describing needs 			*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
{
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check first image				*/
    image1, desc1, ntypes, flags, f);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check second image			 	*/
    image2, desc2, ntypes, flags, f);
  if (rc != DT_RC_GOOD)
    return rc;

  if (ntypes != NULL &&					/* caller has specified ntypes?	    */
      image1->ntype != image2->ntype)	/* ntypes of images must agree		*/
    return dtimage_err_ntype_2(ctl,
      image1, desc1, image2, desc2, f);

  if (xmax != NULL)						/* caller wants this?			    */
    *xmax = DT_MIN(image1->xe,			/* limit op to intersection			*/
      image2->xe);					
  if (ymax != NULL)
    *ymax = DT_MIN(image1->ye,
      image2->ye);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_check_3f() - simple three image check
| 
| SUMMARY
| Check that three images' data types are in a given NULL-terminated
| list of types.
| Bypass this check if ~ntypes~ is NULL.
| If the :DTIMAGE_FLAG_PAGED: bit in ~flags~ is not set,
| verify that none of the images is not marked as paged.
| Verify that the three images have the same data type.
| Have an error if any check fails.
| Issue a :DT_DBG_MASK_IMG: debug message.
| Compute the smaller of the three images in both dimensions.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_check_3f(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dtimage_t *image3,				/* third image two be checked 		*/
  const char *desc3,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  unsigned long flags,					/* flags describing needs 			*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
{
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check first image				*/
    image1, desc1, ntypes, flags, f);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check second image			 	*/
    image2, desc2, ntypes, flags, f);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check third image			 	*/
    image3, desc3, ntypes, flags, f);
  if (rc != DT_RC_GOOD)
    return rc;

  if (image1->ntype != image2->ntype)	/* ntypes of images must agree		*/
    return dtimage_err_ntype_2(ctl,
      image1, desc1, image2, desc2, f);

  if (image1->ntype != image3->ntype)
    return dtimage_err_ntype_2(ctl,
      image1, desc1, image3, desc3, f);

  if (xmax != NULL)						/* caller wants this?			    */
    *xmax = DT_MIN(image1->xe,			/* limit op to intersection			*/
      DT_MIN(image2->xe, image3->xe));					
  if (ymax != NULL)
    *ymax = DT_MIN(image1->ye,
      DT_MIN(image2->ye, image3->ye));					

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_check_1() - simple image check
| 
| SUMMARY
| Call dtimage_check_1f() with ~flags~ set to 0.
| Presume that the caller will not operate properly on paged images
| so an error will be caused if the image is marked as paged.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_check_1(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image to be checked				*/
  const char *desc,						/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f)						/* caller ID 						*/
{
  return dtimage_check_1f(ctl, image,
    desc, ntypes, 0, f);
}

/*..........................................................................
| NAME
| dtimage_check_2() - simple two image check
| 
| SUMMARY
| Call dtimage_check_2f() with ~flags~ set to 0.
| Presume that the caller will not operate properly on paged images
| so an error will be caused if either image is marked as paged.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_check_2(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
{
  return dtimage_check_2f(ctl,
    image1, desc1, image2, desc2,
    ntypes, 0, f, xmax, ymax);
}

/*..........................................................................
| NAME
| dtimage_check_3() - simple three image check
| 
| SUMMARY
| Call dtimage_check_3f() with ~flags~ set to 0.
| Presume that the caller will not operate properly on paged images
| so an error will be caused if any image is marked as paged.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_check_3(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dtimage_t *image3,				/* third image two be checked 		*/
  const char *desc3,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
{
  return dtimage_check_3f(ctl,
    image1, desc1, 
    image2, desc2,
    image3, desc3,
    ntypes, 0, f, xmax, ymax);
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
