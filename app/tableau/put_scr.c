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
DT_RCSID("app/tableau $RCSfile: put_scr.c,v $ $Revision: 1.8 $");

#define SCR (&(put)->u.scr.scr)

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_scr_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
{
  DT_F("tableau_put_scr");
  dt_ctl_t *ctl = &param->ctl;
  int i;

  DT_Q(dtscr_create,(ctl, SCR, spec)); 

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "opened screen \"%s\""
    " for image sink display",
    spec);

  DT_Q(dt_color_org_set,(ctl,			/* set up org structures 			*/
    &put->u.scr.org_y_cb_cr,
    DT_COLOR_ORG_Y_CB_CR));

  DT_Q(dt_color_org_set,(ctl,	
    &put->u.scr.org_r_g_b,
    DT_COLOR_ORG_R_G_B));

  DT_Q(dt_color_org_set,(ctl,
    &put->u.scr.org_pseudo,
    DT_COLOR_ORG_MAP));

  DT_Q(dt_color_convert_init,(ctl,		/* prepare for conversion 			*/
    &put->u.scr.convert_y_cb_cr_to_pseudo,
    &put->u.scr.org_y_cb_cr,
    &put->u.scr.org_pseudo));

  DT_Q(dt_color_convert_init,(ctl,
    &put->u.scr.convert_r_g_b_to_pseudo,
    &put->u.scr.org_r_g_b,
    &put->u.scr.org_pseudo));

  put->u.scr.convert_y_cb_cr_to_pseudo.ncells = TABLEAU_PSEUDO_CELL_COUNT; 
  put->u.scr.convert_r_g_b_to_pseudo.ncells = TABLEAU_PSEUDO_CELL_COUNT;
  
  put->u.scr.x0 = TABLEAU_X0;
  put->u.scr.y0 = TABLEAU_Y0;
	
  DT_Q(dtos_malloc2,(ctl,
    (void **)&put->u.scr.lut8,
    256 * sizeof(dtlut8_t),
    F, "lut"));

  memset(put->u.scr.pseudo_remap, 0,
    sizeof(put->u.scr.pseudo_remap));
	
  for (i=0;
	   i<TABLEAU_PSEUDO_CELL_COUNT; 
	   i++)
	put->u.scr.pseudo_remap[i] =
      TABLEAU_PSEUDO_CELL_START + i;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_scr(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)
{
  DT_F("tableau_put_scr");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t *inset = &put->u.scr.inset;
  dtimage_t *resampled = put->u.scr.resampled;
  dtimage_t *blue = &put->u.scr.blue;
  dtimage_t *red = &put->u.scr.red;
  dtxy_t xe, ye;
  char s[80];
  int i;
  
  if (data == NULL)						/* supposed to clean up? 			*/
  {
	if (inset->xe != 0)
	  DT_Q(dtimage_free,(ctl, inset));
	if (red->xe != 0)
	  DT_Q(dtimage_free,(ctl, red));
	if (blue->xe != 0)
	  DT_Q(dtimage_free,(ctl, blue));
	for (i=0; i<TABLEAU_CC_N; i++)
  	  if (&resampled[i].xe != 0)
	    DT_Q(dtimage_free,(ctl, 
          &resampled[i]));

	if (put->u.scr.convert_r_g_b_to_pseudo.
		initialized)
	DT_Q(dt_color_convert_uninit,(ctl,	/* clean up after conversion		*/
      &put->u.scr.convert_r_g_b_to_pseudo,
      &put->u.scr.org_r_g_b,
      &put->u.scr.org_pseudo));

	if (put->u.scr.convert_y_cb_cr_to_pseudo.
		initialized)
	DT_Q(dt_color_convert_uninit,(ctl,	/* clean up after conversion		*/
      &put->u.scr.convert_y_cb_cr_to_pseudo,
      &put->u.scr.org_y_cb_cr,
      &put->u.scr.org_pseudo));
	
	if (put->u.scr.lut8 != NULL)
  	  DT_Q(dtos_free2,(ctl,
        put->u.scr.lut8,
        F, "lut"));

	DT_Q(dtscr_destroy,(ctl, SCR));
	return DT_RC_GOOD;
  }

										/* ................................ */
  if (cc == TABLEAU_CC_CB)				/* this is blue?					*/
  {
	if (blue->xe != image->xe*2 ||		/* image size changed on us? 		*/
		blue->ye != image->ye*2)
	  DT_Q(dtimage_free,(ctl, blue));

	if (blue->xe == 0)					/* need new blue image? 			*/
	  DT_Q(dtimage_create,(ctl,
        blue, image->xe*2, image->ye*2,
        DT_NTYPE_B08));

	DT_Q(tableau_fast_pixrep,(ctl,		/* quickly replicate 				*/
      image, 2, blue));
	image = blue;
  }

										/* ................................ */
  if (cc == TABLEAU_CC_CR)				/* this is red?						*/
  {
	if (red->xe != image->xe*2 ||		/* image size changed on us? 		*/
		red->ye != image->ye*2)
	  DT_Q(dtimage_free,(ctl, red));

	if (red->xe == 0)					/* need new red image? 				*/
	  DT_Q(dtimage_create,(ctl,
        red, image->xe*2, image->ye*2,
        DT_NTYPE_B08));

	DT_Q(tableau_fast_pixrep,(ctl,		/* quickly replicate 				*/
      image, 2, red));
	image = red;
  }

										/* ................................ */

  xe = image->xe * param->dx;			/* displayed image size 			*/
  ye = image->ye * param->dy;

  if (header->org ==					/* rgb in a single frame? 			*/
	  DT_COLOR_ORG_RGB888)
	xe /= 3;
  
  if (resampled[cc].xe != xe ||			/* resampled size changed?			*/
	  resampled[cc].ye != ye)
  {
  	dt_dbg(ctl, F, DT_DBG_MASK_USR2,
      "old resampled[%d] %ux%u,"
      " new resampled[%d] %ux%u",
	  cc, resampled[cc].xe, 
      resampled[cc].ye,
	  cc, xe, ye);
    if (resampled[cc].xe != 0)			/* we have a resampled image? 		*/
      DT_Q(dtimage_free,(ctl,			/* free old resampled image 		*/
        &resampled[cc]));
  }

  if (resampled[cc].xe == 0)			/* we need a resampled image? 		*/
    DT_Q(dtimage_create,(ctl,			/* make space for resampled image 	*/
      &resampled[cc], xe, ye,
      DT_NTYPE_B08));

  if (param->dx == 1 &&					/* no size change? 					*/
	  param->dy == 1)
    DT_Q(dtimage_cast,(ctl, image,		/* simply copy the image 			*/
      &resampled[cc]))
  else
    DT_Q(dtimage_resample_bilinear,(	/* resample the image 				*/
      ctl, image,
      1.0/param->dx,
      1.0/param->dy,
      &resampled[cc]));

  if (cc != TABLEAU_CC_Y)				/* this is not luminance? 			*/
    return DT_RC_GOOD;

#ifdef NONO
  {
	dt_color_org_t org;
	char org_s[32];
	org.org = header->org;

    DT_Q(dt_color_org_string,(ctl,		/* make org string name 			*/
      &org, org_s, sizeof(org_s)));
	dt_dbg(ctl, F, DT_DBG_MASK_ANY,
      "displaying %s", org_s);
  }
#endif

  if (header->org ==					/* rgb in a single frame? 			*/
	  DT_COLOR_ORG_RGB888)
  {
	if (param->background != 0 &&		/* time to update the lut? 			*/
		header->n % 
		(int)param->background == 0)
	  put->u.scr.skiplut = 0;
										/* ................................ */
	if (blue->xe != image->xe/3 ||		/* image size changed on us? 		*/
		blue->ye != image->ye)
	  DT_Q(dtimage_free,(ctl, blue));

	if (blue->xe == 0)					/* need new blue image? 			*/
	  DT_Q(dtimage_create,(ctl,
        blue, image->xe/3, image->ye,
        DT_NTYPE_B08));
										/* ................................ */
	if (red->xe != image->xe/3 ||		/* image size changed on us? 		*/
		red->ye != image->ye)
	  DT_Q(dtimage_free,(ctl, red));

	if (red->xe == 0)					/* need new red image? 				*/
	  DT_Q(dtimage_create,(ctl,
        red, image->xe/3, image->ye,
        DT_NTYPE_B08));
										/* ................................ */

	DT_Q(tableau_fast_separate,(ctl,	/* quickly separate 				*/
      image->row[0].dvoid,
  	  image->xe/3,
      image->ye,
      red->row[0].dvoid,
      resampled[TABLEAU_CC_Y].row[0].dvoid,
      blue->row[0].dvoid))

	DT_Q(dt_color_convert3,(ctl,		/* convert r_g_b to pseudo			*/
      &put->u.scr.convert_r_g_b_to_pseudo,
      red->row[0].dvoid,
      resampled[TABLEAU_CC_Y].row[0].dvoid,
      blue->row[0].dvoid,
      &put->u.scr.org_r_g_b,
  	  image->xe/3,
      image->ye,
      resampled[TABLEAU_CC_Y].			/* pseudo goes back into Y image    */
        row[0].dvoid,
      put->u.scr.skiplut? NULL:			/* need to generate new lut? 		*/
        (void *)put->u.scr.lut8,
      NULL,
      &put->u.scr.org_pseudo));
	DT_Q(tableau_fast_lookup,(ctl,		/* remap pseudo into 10..246 		*/
      &resampled[TABLEAU_CC_Y],
      put->u.scr.pseudo_remap,
      &resampled[TABLEAU_CC_Y]));
  }
  else									/* not rgb888? 						*/
  if (resampled[TABLEAU_CC_CB].xe!=0 &&	/* we have the color components? 	*/
	  resampled[TABLEAU_CC_CR].xe!=0)
  {
	if (param->background != 0 &&		/* time to update the lut? 			*/
		header->n % 
		(int)param->background == 0)
	  put->u.scr.skiplut = 0;
	DT_Q(dt_color_convert3,(ctl,		/* convert y_cb_cr to pseudo		*/
      &put->u.scr.convert_r_g_b_to_pseudo,
      resampled[TABLEAU_CC_Y].row[0].dvoid,
      resampled[TABLEAU_CC_CB].row[0].dvoid,
      resampled[TABLEAU_CC_CR].row[0].dvoid,
      &put->u.scr.org_y_cb_cr,
  	  resampled[TABLEAU_CC_Y].xe,
      resampled[TABLEAU_CC_Y].ye,
      resampled[TABLEAU_CC_Y].			/* pseudo goes back into Y image    */
        row[0].dvoid,
      put->u.scr.skiplut? NULL:			/* need to generate new lut? 		*/
        (void *)put->u.scr.lut8,
      NULL,
      &put->u.scr.org_pseudo));
	DT_Q(tableau_fast_lookup,(ctl,		/* remap pseudo into 10..246 		*/
      &resampled[TABLEAU_CC_Y],
      put->u.scr.pseudo_remap,
      &resampled[TABLEAU_CC_Y]));
  }
  else
  {
	if (!put->u.scr.skiplut)
  	  for (i=0; i<256; i++)				/* 7-bit pure gray lut 				*/
	    put->u.scr.lut8[i].r =
	    put->u.scr.lut8[i].g =
	    put->u.scr.lut8[i].b = 
          DT_MIN(246, i+10);
  }

  dtstr_printf(ctl, s, sizeof(s),		/* make frame number into string 	*/
    "%08ld", header->n);
  if (inset->xe != 0 &&					/* inset is created already?		*/
	  inset->row[0].dvoid !=			/* but they changed images on us? 	*/
	  &resampled[cc].row[ 0].dvoid)
    DT_Q(dtimage_free,(ctl, inset));	/* get rid of old inset 			*/
  if (inset->xe == 0)					/* inset has been freed? 			*/
    DT_Q(dtimage_overlay,(ctl,			/* make a new inset 				*/
      &resampled[cc],
      inset, 0, 0, 8*strlen(s), 13));
  DT_Q(dtimage_constant,(ctl,			/* clear background behind number 	*/
    inset, 1));
  DT_Q(dtimage_plot_text_8x16,(ctl,		/* display number on image 			*/
    inset, s, 0, 0, 255));

										/* ................................ */
  
  DT_Q(dtscr_wait_event,(ctl,			/* poll for events					*/
    SCR, NULL, 0));

  strcpy(s, TABLEAU_TITLE_CURRENT);

#ifdef NONO
  if (put->u.scr.swap == 1)
    strcat(s, " 1");
  else
    strcat(s, " 0");
#endif

  put->u.scr.swap ^= 1;

  DT_Q(dtxchg_scr_export_image,(ctl,	/* display current					*/
    SCR, s, &resampled[cc],
    put->u.scr.x0, put->u.scr.y0));

  put->u.scr.x0 = DTIMAGE_NOPOS;		/* replace image in situ 			*/
  put->u.scr.y0 = DTIMAGE_NOPOS;
  
  if (!put->u.scr.skiplut)				/* new lut was generated?			*/
  {
    DT_Q(dtscr_export_color,(ctl,		/* output the lut 					*/
      SCR, s, put->u.scr.lut8, 
      TABLEAU_PSEUDO_CELL_START,
      TABLEAU_PSEUDO_CELL_COUNT));
	put->u.scr.skiplut = 1;				/* skip until explicitly reset 		*/
  }

#ifdef NONO
  DT_Q(dtscr_window_raise,(ctl, SCR,	/* raise this window over the other */
    s));
#endif

  DT_Q(dtscr_wait_event,(ctl,			/* poll for event					*/
    SCR, NULL, 0));

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
