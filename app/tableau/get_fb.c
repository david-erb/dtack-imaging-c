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
DT_RCSID("app/tableau $RCSfile: get_fb.c,v $ $Revision: 1.8 $");

#define FB  (&(get)->u.fb.fb)

#define CINDEX ((get)->u.fb.cam.current_index)

extern
dt_rc_e
DTCONFIG_API1
tableau_fast_decimate(
  dt_ctl_t *ctl,
  unsigned char *s,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *d);

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_fb(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_fb");
  dt_ctl_t *ctl = &param->ctl;
  unsigned char *raw = get->u.fb.raw;
  unsigned long *accumulator = get->u.fb.accumulator;
  unsigned char *component = get->u.fb.component;
  unsigned int sigma;
  unsigned int n = FB->xe * FB->ye;
  unsigned int i;

  if (*header == NULL)					/* don't have an old header? 		*/
  DT_Q(dtos_malloc2,(ctl,				/* allocate space for header 		*/
    (void **)header,
    sizeof(tableau_header_t),
    F, "header"));

  memset(*header, 0,					/* clear header 					*/
    sizeof(tableau_header_t));
  (*header)->n = get->u.fb.frame;
  strcpy((*header)->codec,				/* deliver as uncompressed data 	*/
    TABLEAU_CODEC_UNCOMPRESSED);

  while(1)
  {
	DT_Q(tableau_cam_wait,(param,		/* wait for scheduled time 			*/
      &get->u.fb.cam));

	if (CINDEX != -1)					/* we know which cam to fire?		*/
	  break;
    
	DT_Q(tableau_cam_next,(param,		/* identify next cam				*/
      &get->u.fb.cam));

	if (CINDEX != -1)					/* another cam fires today? 		*/
	  DT_Q(dtfb_source,(ctl, FB,		/* switch to next cam source		*/
        get->u.fb.cam.spigot[CINDEX]));
  }

  DT_Q(dtstr_printf,(ctl,				/* copy cam's name to header		*/
    (*header)->cam,
    sizeof((*header)->cam),
    "%s", 
    get->u.fb.cam.name[CINDEX]));

  (*header)->l = n * 3;					/* max bytes we are returning		*/

  if (*l < (*header)->l)				/* given buffer is too small? 		*/
  {
	if (*l != 0)						/* we even have a given buffer? 	*/
	  DT_Q(dtos_free2,(ctl, *data,
        F, "frame buffer data"));
	*l = 0;
    DT_Q(dtos_malloc2,(ctl, data,		/* space for the data 				*/
      DT_CAST(unsigned int, 
        (*header)->l),
      F, "frame buffer data"));
	*l = (*header)->l;					/* remember how big it is 			*/
  }

  if (get->u.fb.cc == TABLEAU_CC_N)		/* ready for a new frame?			*/
  {
    for (sigma=0; 
	     sigma==0 || 
		 sigma<param->sigma; 
	     sigma++)
    {
      DT_Q(dtfb_control,(ctl, FB,		/* start acquiring again			*/
        DTFB_CONTROL_ACQUIRE));

      DT_Q(dtfb_getblock,(ctl, FB,		/* suck bytes out 					*/
        raw, 0, 0, FB->xe, FB->ye));
	
	  if (param->sigma > 1)				/* we are doing frame averaging? 	*/
	  if (sigma == 0)					/* first time through? 				*/
	    for (i=0; i<n; i++)
	      accumulator[i] = raw[i];
	  else
	    for (i=0; i<n; i++)
	      accumulator[i] += raw[i];
	}
										/* ................................ */
	DT_Q(tableau_cam_next,(param,		/* identify next cam				*/
      &get->u.fb.cam));

	if (CINDEX != -1)					/* another cam fires today? 		*/
	  DT_Q(dtfb_source,(ctl, FB,		/* switch to next cam source		*/
        get->u.fb.cam.spigot[CINDEX]));
										/* ................................ */

	if (sigma == 2)
      for (i=0; i<n; i++)
	    raw[i] = accumulator[i] >> 1;
	else
	if (sigma == 4)
      for (i=0; i<n; i++)
	    raw[i] = accumulator[i] >> 2;
	else
	if (sigma > 1)
	{
	  const unsigned long t = sigma;
      for (i=0; i<n; i++)
	    raw[i] = accumulator[i] / t;
	}

    if (get->u.fb.convert.initialized)
	  DT_Q(dt_color_convert,(ctl,		/* convert raw to component org		*/
        &get->u.fb.convert,
        raw, &FB->org,
        FB->xe, FB->ye,
        component, 
        &get->u.fb.org_component));

    if (FB->org.org !=					/* fb is giving component video? 	*/
		DT_COLOR_ORG_LUMINANCE &&
		get->u.fb.org_component.org !=	/* and we need to return color? 	*/
		DT_COLOR_ORG_LUMINANCE &&
		get->u.fb.org_component.org != 
		DT_COLOR_ORG_YCBCR &&
		get->u.fb.org_component.org != 
		DT_COLOR_ORG_RGB888)
	  get->u.fb.cc = TABLEAU_CC_CB;		/* return blue first				*/
	else
	  get->u.fb.cc = TABLEAU_CC_Y;		/* presume we have mono only 		*/
  }
  
  if (get->u.fb.cc == TABLEAU_CC_CB)	/* time to return blue?				*/
  {
	get->u.fb.org.org =					/* this time deliver blue 			*/
  	  DT_COLOR_ORG_CB;
	get->u.fb.cc = TABLEAU_CC_CR;		/* next time read red 				*/

    DT_Q(tableau_fast_decimate,(ctl,	/* copy blue part to output buffer 	*/
      component+n, FB->xe, FB->ye,
      *data));
    (*header)->l = n/4;					/* bytes we are returning 			*/
    (*header)->xe = FB->xe/2;			/* picture size 					*/
    (*header)->ye = FB->ye/2;
  }
  else
  if (get->u.fb.cc == TABLEAU_CC_CR)	/* time to return red?				*/
  {
	get->u.fb.org.org =					/* this time deliver red			*/
  	  DT_COLOR_ORG_CR;
	get->u.fb.cc = TABLEAU_CC_Y;		/* next time read luminance			*/

    DT_Q(tableau_fast_decimate,(ctl,	/* copy red part to output buffer 	*/
      component+2*n, FB->xe, FB->ye,
      *data));
    (*header)->l = n/4;					/* bytes we are returning 			*/
    (*header)->xe = FB->xe/2;			/* picture size 					*/
    (*header)->ye = FB->ye/2;
  }
  else									/* time to return luminance? 		*/
  {
	if (get->u.fb.org_component.org ==	/* and we need to return color? 	*/
		DT_COLOR_ORG_YCBCR ||
		get->u.fb.org_component.org ==
		DT_COLOR_ORG_RGB888)
	{
	  get->u.fb.org.org =				/* deliver components 3-wide		*/
  	    get->u.fb.org_component.org;
      (*header)->l = n * 3;				/* bytes we are returning 			*/
	  (*header)->xe = FB->xe * 3;		/* picture size 					*/
	  (*header)->ye = FB->ye;
	}
	else
	{
	  get->u.fb.org.org =				/* this time deliver luminance		*/
  	    DT_COLOR_ORG_LUMINANCE;
      (*header)->l = n;					/* bytes we are returning 			*/
	  (*header)->xe = FB->xe;			/* picture size 					*/
	  (*header)->ye = FB->ye;
	}
    get->u.fb.frame++;
	get->u.fb.cc = TABLEAU_CC_N;		/* next time get a new frame		*/

    memcpy(*data, component, 			/* copy luminance to output buffer 	*/
      (*header)->l);
  }
  
  (*header)->org = get->u.fb.org.org;	/* return org type we deliver	    */

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_fb_free(
  dtparam_t *param,
  tableau_get_t *get)
{
  DT_F("tableau_get_fb_free");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (get->u.fb.component != NULL &&
	  get->u.fb.component != get->u.fb.raw)
    DT_I(dtos_free2,(ctl,
      get->u.fb.component,
      F, "components"));

  if (get->u.fb.accumulator != NULL)
    DT_I(dtos_free2,(ctl,
      get->u.fb.accumulator,
      F, "accumulator"));

  if (get->u.fb.raw != NULL)
    DT_I(dtos_free2,(ctl,
      get->u.fb.raw,
      F, "raw"));

  if (get->u.fb.convert.initialized)
  {
	DT_I(dt_color_convert_uninit,(ctl,
      &get->u.fb.convert,
      &FB->org,
      &get->u.fb.org_component));
  }

  if (get->u.fb.cam.initialized)
    DT_I(tableau_cam_uninit,(param,		/* release camera resources 		*/
      &get->u.fb.cam));

  DT_I(dtfb_uninit,(ctl, FB));			/* release frame buffer resources 	*/
  
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_fb_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
{
  DT_F("tableau_get_fb_setup");
  dt_ctl_t *ctl = &param->ctl;
  unsigned int n;
  unsigned int nbytes;
  char message[80];

  DT_Q(dtfb_init,(ctl, FB, spec));
  DT_Q(dtfb_control,(ctl, FB,			/* reset the pig					*/
    DTFB_CONTROL_RESET));

  DT_Q(tableau_cam_init,(param, 		/* get camera config 				*/
    &get->u.fb.cam));

  CINDEX = -1;							/* start new cam day 				*/

  n = FB->xe * FB->ye;
  DT_Q(dt_color_org_size,(ctl,			/* size of raw pixels 				*/
    &FB->org, n, &nbytes));
  DT_Q(dtos_malloc2,(ctl,				/* space for raw fb bytes 			*/
    (void **)&get->u.fb.raw, 
    nbytes, F, "raw"));

  if (param->sigma > 1)					/* doing frame averaging? 			*/
  DT_Q(dtos_malloc2,(ctl,				/* space for accumulated bytes 		*/
    (void **)&get->u.fb.accumulator,
    n * sizeof(*get->u.fb.accumulator),
    F, "accumulator"));
  
  DT_Q(dt_color_org_parse,(ctl,			/* component form to convert into 	*/
    &get->u.fb.org_component, 
    param->color,
    message, sizeof(message)));
  if (message[0] != '\0')
    return dt_err(ctl, F,
      "%s", message);

  if (get->u.fb.org_component.org !=	/* conversion is required? 			*/
	  FB->org.org)
  {									
    DT_Q(dt_color_org_size,(ctl,		/* size of converted pixels 		*/
      &get->u.fb.org_component, 
      n, &nbytes));
    DT_Q(dtos_malloc2,(ctl,				/* space for converted bytes 		*/
      (void **)&get->u.fb.component,
      nbytes, F, "component"));

    DT_Q(dt_color_convert_init,(ctl,	/* prepare for color conversion 	*/
      &get->u.fb.convert, &FB->org, 
      &get->u.fb.org_component));
  }
  else									/* no conversion required? 			*/
    get->u.fb.component =
	  get->u.fb.raw;

  get->u.fb.cc = TABLEAU_CC_N;			/* prime the pump 					*/

  get->function = tableau_get_fb;
  get->free = tableau_get_fb_free;

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
