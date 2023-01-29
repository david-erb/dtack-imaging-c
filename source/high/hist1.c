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
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  19980924 DE bug not checking *cando
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: hist1.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................
| allocate histogram suitable for given image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_hist_alloc(						/* allocate histogram for image		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  dthist_t *hist)						/* output histogram					*/
{
  DT_F("dtimage_hist_alloc");
  dthist_bin_t width;
  double min, max;

  if (image->ntype == DT_NTYPE_B01)		/* single-bit image?				*/
  {
	width = 2;
	min = 0;
	max = 2;
  }
  else
  if (image->ntype == DT_NTYPE_B08)		/* 8-bit unsigned image?			*/
  {
	width = 256;						/* use a direct mapped histogram 	*/
	min = 0;
	max = 256;
  }
  else
  if (image->ntype == DT_NTYPE_S1)
  {
	width = 256;				
	min = dt_ntype_minval[image->ntype];
	max = dt_ntype_maxval[image->ntype];
  }
  else									/* higher precision image? 			*/
  {
	DT_Q(dtimage_range,(ctl, image,		/* use image's dynamic range 		*/
      &min, &max));
    if (DTNTYPE_ISFLOAT(image->ntype))
  	  width = 1000;		
    else								/* non-floating data?			    */
    {
      max++;							/* make scale integral			    */
      width = DT_CAST(dthist_bin_t, 
        DT_MIN(max-min, 1000));
    }
  }

  DT_Q(dthist_alloc,(ctl, hist, width,	/* actually allocate 				*/
    min, max));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_hist(							/* make histogram from image		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  dthist_t *hist)						/* output histogram					*/
{
  DT_F("dtimage_hist");
  dtxy_t xmax = image->xe;
  dtxy_t ymax = image->ye;
  dtxy_t y = 0;
  void *p = NULL;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dthist_ntype_list,					/* allow hist ntypes only 			*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_C(dthist_clear,(ctl, hist));		/* clear frequency array		    */
  
  for (y=0; y<ymax; y++)				/* one row at a time 				*/
  {
	DTIMAGE_GETROW2(DT_C, ctl, image, y, &p);
    DT_C(dthist_add_values,(ctl, hist,
      p, 0, xmax, image->ntype));
	DTIMAGE_UNMAPROW2(DT_C, ctl, image, y);
	p = NULL;
  }

cleanup:
  if (p != NULL)
    DTIMAGE_UNMAPROW2(DT_I, ctl, image, y);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_hist_fast(						/* make histogram from image		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  dthist_t *hist,						/* output histogram					*/
  int *cando)
{
  DT_F("dtimage_hist_fast");
  dtxy_t xmax = image->xe;
  dtxy_t ymax = image->ye;
  dtxy_t y = 0;
  void *p = NULL;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dthist_ntype_list,					/* allow hist ntypes only 			*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_C(dthist_clear,(ctl, hist));		/* clear frequency array		    */
  
  for (y=0; y<ymax; y++)				/* one row at a time 				*/
  {
	DTIMAGE_GETROW2(DT_C, ctl, image, y, &p);
    DT_C(dthist_add_values_fast,(ctl, hist,
      p, 0, xmax, image->ntype, cando));

	DTIMAGE_UNMAPROW2(DT_C, ctl, image, y);
	p = NULL;
	if (*cando == 0)					/* couldn't do it fast? 			*/
	  goto cleanup;
  }

cleanup:
  if (p != NULL)
    DTIMAGE_UNMAPROW2(DT_I, ctl, image, y);
  return rc;
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
