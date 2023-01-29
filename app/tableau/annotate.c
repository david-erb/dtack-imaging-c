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

DT_RCSID("app/tableau $RCSfile: annotate.c,v $ $Revision: 1.8 $");

/*..........................................................................
 * place string in image
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_annotate(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int y0,
  dt_color_org_t *org,
  const char *string)
{
  DT_F("tableau_annotate");
  dtimage_t _inset, *inset = &_inset;
  dtxy_t x, xe, y;
  char t[128];
  dt_rc_e rc;

  inset->xe = 0;
  if (org->org == DT_COLOR_ORG_YCBCR ||	/* need certain pixel organization */
	  org->org == DT_COLOR_ORG_RGB888)
  {
	xe = image->xe / 3;
  }
  else
  if (org->org == DT_COLOR_ORG_LUMINANCE)
  {
	xe = image->xe;
  }
  else
  {
	char org_s[32];
	DT_C(dt_color_org_string,(ctl,
      org, org_s, sizeof(org_s)));
    rc = dt_err(ctl, F,
      "cannot annotate %s images",
      org_s);
	goto cleanup;
  }

  DT_C(dtimage_create,(ctl, inset,		/* create inset 					*/
    xe, 15, DT_NTYPE_B08));

  DT_C(dtimage_constant,(ctl, inset,	/* clear inset 						*/
    0));

  DT_C(dtstr_printf,(ctl, t, sizeof(t),
    "%s", string));
  if (strlen(t) > inset->xe/8u)
  {
	t[inset->xe/8] = '\0';
    x = 0;
  }
  else
    x = (inset->xe - strlen(t)*8)/2;	/* put string at horizontal center 	*/
  
  DT_C(dtimage_plot_text_8x16,(ctl,		/* plot string 						*/
    inset, string, x, 0, 255));

  if (y0 < 0)							/* align bottom of string? 			*/
    y0 = -y0 - inset->ye;				/* put string at bottom 				*/
  
  for (y=0; y<inset->ye; y++)			/* copy string to destination 		*/
  {
	dt_ntype_b08_t *p = 
      image->row[y0+y].b08;
	dt_ntype_b08_t *d = 
      inset->row[y].b08;
	if (org->org == DT_COLOR_ORG_YCBCR)
	for (x=0; x<inset->xe; x++)
	{
	  *p++ = *d++;
	  *p++ = 128;
	  *p++ = 128;
	}
	else
	if (org->org == DT_COLOR_ORG_RGB888)
	for (x=0; x<inset->xe; x++)
	{
	  const unsigned char dconst = *d++;
	  *p++ = dconst;
	  *p++ = dconst;
	  *p++ = dconst;
	}
	else
	if (org->org == DT_COLOR_ORG_LUMINANCE)
	for (x=0; x<inset->xe; x++)
	{
	  *p++ = *d++;
	}
  }

cleanup:
  DT_I(dtimage_free,(ctl, inset));

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
