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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("app/tableau $RCSfile: cook.c,v $ $Revision: 1.8 $");

extern tableau_cook_f **tableau_cook_methods;
extern const char **tableau_cook_choices;

static long kernel_3x3_ave[] = {1,1,1, 1,1,1, 1,1,1};

#ifndef DTDEPEND
#  include <time.h>
#endif

/*..........................................................................
 * cook raw image
 * clean up if raw is NULL
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_raw(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
{
  if (raw == NULL)
    return DT_RC_GOOD;
  *cooked = *raw;
  return DT_RC_GOOD;
}

#ifdef NONO
/*..........................................................................
 * cook image from GE camcorder
 * clean up if raw is NULL
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_ge(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
{
  DT_F("tableau_cook_ge");
  dt_ctl_t *ctl = &param->ctl;
  int didit;
  dt_rc_e rc = DT_RC_GOOD;;
  
  if (raw == NULL)						/* supposed to clean up? 			*/
    goto cleanup;

  DT_C(dtimage_check_1f,(ctl, raw,
    "raw image", 
    tableau_ntypes, 0, F));

  if (cooked->xe != raw->xe)			/* image size changing? 			*/
  {
	if (cooked->xe != 0)
	  DT_C(dtimage_free,(ctl, cooked));
    DT_C(dtimage_create,(ctl, cooked,
      raw->xe, raw->ye, DT_NTYPE_B08));
  }

  DT_C(dtimage_convolve_long_b08,(ctl,	/* lopass filter					*/
    raw, kernel_3x3_ave, 3, 3, 9, 
    cooked, &didit));

  {
	const dtxy_t xe = cooked->xe;
	const dtxy_t ye = cooked->ye;
	dtxy_t y;
	memcpy(cooked->row[0].dvoid,
		   cooked->row[1].dvoid,
           cooked->xe);
	memcpy(cooked->row[ye-1].dvoid,
		   cooked->row[ye-2].dvoid,
           cooked->xe);
	memset(cooked->row[0].dvoid, 0, cooked->xe);
	memset(cooked->row[1].dvoid, 0, cooked->xe);
	memset(cooked->row[ye-1].dvoid, 0, cooked->xe);
	memset(cooked->row[ye-2].dvoid, 0, cooked->xe);
	for (y=1; y<ye-1; y++)
	{
	  cooked->row[y].b08[0] =
	  cooked->row[y].b08[1] = 0;
	  cooked->row[y].b08[xe-1] =
	  cooked->row[y].b08[xe-2] = 0;
	}
	
  }

  DT_C(dtimage_check_1f,(ctl, cooked,
    "cooked image", 
    tableau_ntypes, 0, F));

cleanup:
  if (raw == NULL ||
	  rc != DT_RC_GOOD)
  {
    if (cooked != NULL &&
		cooked->xe != 0)
      DT_I(dtimage_free,(ctl, cooked));
  }
  return rc;
}
#endif

/*..........................................................................
 * time stamp image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_stamp_it(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int y0,
  dt_color_org_t *org,
  const char *format)
{
  DT_F("tableau_cook_stamp_it");
  char stamp[256];
  dt_rc_e rc;

  if (strlen(format) > 128)				/* ridiculous format? 				*/
  {
	strcpy(stamp, "format too long");
  }
  else
  {
	time_t clock = time(NULL);			/* current time 					*/
	struct tm *t = localtime(&clock);	/* format into fields 				*/
    strftime(stamp, sizeof(stamp),		/* format fields into string 		*/
      format, t);
  }

  DT_C(tableau_annotate,(ctl,
    image, y0, org, stamp));

cleanup:

  return rc;
}

/*..........................................................................
 * time stamp image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_stamp(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
{
  DT_F("tableau_cook_stamp");
  dt_ctl_t *ctl = &param->ctl;
  const char *format;
  dt_rc_e rc = DT_RC_GOOD;;
  
  if (raw == NULL)						/* supposed to clean up? 			*/
    goto cleanup;

  DT_C(dtimage_check_1f,(ctl, raw,
    "raw image", 
    tableau_ntypes, 0, F));

  *cooked = *raw;						/* use source image space 			*/

  format = strchr(cook_arg->spec, ',');
  if (format != NULL)					/* spec has a comma? 				*/
    format++;							/* skip the comma 					*/
  else									/* spec as no comma? 				*/
    format = "%A %d %b %y %H:%M:%S %Z";	/* use default format 				*/
	  
  DT_C(tableau_cook_stamp_it,(ctl,		/* add time stamp 					*/
    cooked, -cooked->ye,				/* at bottom of image 				*/
    org, format));

cleanup:
  return rc;
}

/*..........................................................................
 * resample image and optionally cut off last 16 rows
 * this is obsolete, replaced by trim
 * leave here because customers are using it
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_resize(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
{
  DT_F("tableau_cook_resize");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t *resampled = &cook_arg->resampled;
  dtxy_t y;
  int n, k;
  int ychop;
  const char *format;
  dt_rc_e rc = DT_RC_GOOD;;
  
  if (raw == NULL)						/* supposed to clean up? 			*/
    goto cleanup;

  DT_C(dtimage_check_1f,(ctl, raw,
    "raw image", 
    tableau_ntypes, 0, F));

  n = sscanf(cook_arg->spec,			/* special resize factor? 			*/
    "resize%d-%d", &k, &ychop);
  if (n < 1)
	k = 2;
  if (n < 2)
	ychop = 0;

  if (org->org == DT_COLOR_ORG_YCBCR ||	/* color image? 					*/
	  org->org == DT_COLOR_ORG_RGB888)
  {
	dtxy_t newxe = raw->xe / k;
	dtxy_t newye = (raw->ye-ychop) / k;
	newxe -= newxe % 3;

	if (resampled->xe > 0 &&
		(resampled->xe < newxe ||		/* image size got bigger? 			*/
		 resampled->ye < newye))
	  DT_C(dtimage_free,(ctl, 
        resampled));
	if (resampled->xe == 0)				/* need to allocate new image? 		*/
	  DT_C(dtimage_create,(ctl, 
        resampled,
        newxe, newye,
        DT_NTYPE_B08));
	for (y=0; y<newye; y++)
	{
	  unsigned char * const q = raw->row[y*k].b08;
	  unsigned char * const p = resampled->row[y].b08;
	  dtxy_t xp;
	  dtxy_t xq = 0;
	  for (xp=0; xp<newxe; xp+=3, xq+=3*k)
	  {
		p[xp+0] = q[xq+0];
		p[xp+1] = q[xq+1];
		p[xp+2] = q[xq+2];
	  }
	}
  }
  else
  {
    if (resampled->xe != 0)				/* we have already resampled? 		*/
	  DT_C(dtimage_free,(ctl, 
        resampled));
  
    DT_C(dtimage_resample,(ctl,			/* resample new image 				*/
      raw, 2.0, 2.0, "nearest", 
      resampled));
  }

  *cooked = *resampled;					/* use resampled image space 		*/

  format = strchr(cook_arg->spec, ',');
  if (format != NULL)					/* spec has a comma? 				*/
  {
    format++;							/* skip the comma 					*/
    DT_C(tableau_cook_stamp_it,(ctl,	/* add time stamp 					*/
      cooked, -cooked->ye,				/* at bottom of image 				*/
      org, format));
  }

cleanup:
  if (raw == NULL ||
	  rc != DT_RC_GOOD)
  {
    if (cooked != NULL &&
		cooked->xe != 0)
      DT_I(dtimage_free,(ctl, cooked));
  }
  return rc;
}

/*..........................................................................
 * time stamp image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_trim(
  dtparam_t *param,
  tableau_cook_arg_t *cook_arg,			/* cooker args						*/
  dtimage_t *raw,						/* raw image location 				*/
  dt_color_org_t *org,
  dtimage_t *cooked)
{
  DT_F("tableau_cook_trim");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t *resampled = &cook_arg->resampled;
  dtxy_t x0;
  dtxy_t y0;
  dtxy_t x1;
  dtxy_t y1;
  dtxy_t stride;
  dtxy_t y;
  int n;
  const char *format;
  dt_rc_e rc = DT_RC_GOOD;;
  
  if (raw == NULL)						/* supposed to clean up? 			*/
    goto cleanup;

  DT_C(dtimage_check_1f,(ctl, raw,
    "raw image", 
    tableau_ntypes, 0, F));

  n = sscanf(cook_arg->spec,			/* looks like a trim request? 		*/
    "trim/%d/%d/%d/%d/%d",
    &x0, &y0, &x1, &y1, &stride);
  x0 *= 3;
  x1 *= 3;

  if (n != 5)
  {
	rc = dt_err(ctl, F, "invalid trim request \"%s\" (n=%d)",
      cook_arg->spec, n);
	goto cleanup;
  }

  if (org->org == DT_COLOR_ORG_YCBCR ||	/* color image? 					*/
	  org->org == DT_COLOR_ORG_RGB888)
  {
	dtxy_t xe = raw->xe - x0 - x1;
	dtxy_t ye = raw->ye - y0 - y1;
	xe /= stride;
	xe -= xe % 3;
	ye /= stride;

	if (resampled->xe > 0 &&
		(resampled->xe < xe ||			/* image size got bigger? 			*/
		 resampled->ye < ye))
	  DT_C(dtimage_free,(ctl, 
        resampled));
	if (resampled->xe == 0)				/* need to allocate new image? 		*/
	  DT_C(dtimage_create,(ctl, 
        resampled, xe, ye,
        DT_NTYPE_B08));
	for (y=0; y<ye; y++)
	{
	  unsigned char * const q = raw->row[y0+y*stride].b08;
	  unsigned char * const p = resampled->row[y].b08;
	  dtxy_t xp;
	  dtxy_t xq = x0;
	  for (xp=0; xp<xe; xp+=3, xq+=3*stride)
	  {
		p[xp+0] = q[xq+0];
		p[xp+1] = q[xq+1];
		p[xp+2] = q[xq+2];
	  }
	}
  }
  else
  {
    if (resampled->xe != 0)				/* we have already resampled? 		*/
	  DT_C(dtimage_free,(ctl, 
        resampled));
  
    DT_C(dtimage_resample,(ctl,			/* resample new image 				*/
      raw, 2.0, 2.0, "nearest", 
      resampled));
  }

  *cooked = *resampled;					/* use resampled image space 		*/

  format = strchr(cook_arg->spec, ',');
  if (format != NULL)					/* spec has a comma? 				*/
  {
    format++;							/* skip the comma 					*/
    DT_C(tableau_cook_stamp_it,(ctl,	/* add time stamp 					*/
      cooked, -cooked->ye,				/* at bottom of image 				*/
      org, format));
  }

cleanup:
  if (raw == NULL ||
	  rc != DT_RC_GOOD)
  {
    if (cooked != NULL &&
		cooked->xe != 0)
      DT_I(dtimage_free,(ctl, cooked));
  }
  return rc;
}

/*..........................................................................
 * look up cook routine by name
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_cook_from_name(
  dtparam_t *param,
  const char *name,
  tableau_cook_f **cook)
{
  DT_F("tableau_cook_from_name");
  dt_ctl_t *ctl = &param->ctl;
  char s[128], *t;
  int n;
  dtxy_t x0, y0, x1, y1, stride;
  int choice;
 
  dtstr_printf(ctl, s, sizeof(s),		/* local copy of name			    */
    "%s", name);

  t = strchr(s, ',');					/* see if name has a comma 			*/
  if (t != NULL)
    *t = '\0';							/* chop off the comma			    */

  n = sscanf(s, "trim/%d/%d/%d/%d/%d",	/* looks like a trim request? 		*/
    &x0, &y0, &x1, &y1, &stride);
  if (n == 5)
	*cook = tableau_cook_trim;
  else
  {
	DT_Q(dtstr_choice,(ctl,				/* look up cook type		    	*/
      tableau_cook_choices, 
      s, &choice));

	if (choice == -1)					/* unknown type?				    */
	  return dt_err(ctl, F,
        "unknown recipe \"%s\"", name);
	else
	if (choice == -2)					/* ambiguous type?				    */
	  return dt_err(ctl, F,
        "ambiguous recipe \"%s\"", name);

	*cook =							
      tableau_cook_methods[choice];
  }

  return DT_RC_GOOD;
}
/*.........................................................................*/

static const char *cook_choices[] = {
  "raw",
  "stamp",
  "resize2",
  "resize3",
  "resize4",
  "resize5",
  "resize6",
  "resize7",
  "resize8",
  "resize2-16",
  "resize3-16",
  "resize4-16",
  "resize5-16",
  "resize6-16",
  "resize7-16",
  "resize8-16",
  NULL
};
static tableau_cook_f *cook_methods[] = {
  tableau_cook_raw,
  tableau_cook_stamp,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize,
  tableau_cook_resize
};

tableau_cook_f **tableau_cook_methods = cook_methods;
const char **tableau_cook_choices = cook_choices;






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
