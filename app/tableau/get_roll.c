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
DT_RCSID("app/tableau $RCSfile: get_roll.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_roll(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_roll");
  dt_ctl_t *ctl = &param->ctl;
  tableau_len_t newest;
  int isreadable;
  char fn[128];
  dtfd_t fd;
  dt_rc_e rc;

  fd.fd = -1;

  DT_Q(tableau_roll_init,(ctl,			/* init roll file 					*/
    get->u.roll.dir, "getroll", NULL));

  if (get->u.roll.cc == TABLEAU_CC_N)	/* ready for a new frame?			*/
  {
	DT_C(tableau_roll_get,(ctl,			/* wait for next luminance frame	*/
      get->u.roll.dir,
      "getroll",
      get->u.roll.frame,   
      &newest));

	dt_dbg(ctl, F, DT_DBG_MASK_ANY,
      "get->u.roll.frame=%d, newest=%d",
      get->u.roll.frame, newest);
	
	if (get->u.roll.frame == -1)
	   get->u.roll.frame = newest;

	dtstr_printf(ctl, fn, sizeof(fn),	/* make blue filename 				*/
      "%s/%08ld.cb.%s", 
      get->u.roll.dir, 
      get->u.roll.frame,
      get->u.roll.suffix);
	DT_C(dtos_isreadable,(ctl, fn,		/* see if blue file exists 			*/
      &isreadable));
	if (isreadable)						/* blue file exists?				*/
	  get->u.roll.cc = TABLEAU_CC_CB;	/* presume we have component color 	*/
	else
	  get->u.roll.cc = TABLEAU_CC_Y;	/* presume we have mono only 		*/
  }

  if (get->u.roll.cc == TABLEAU_CC_CB)	/* time to read blue?				*/
  {
	get->u.roll.cc = TABLEAU_CC_CR;		/* next time read red 				*/
  }
  else
  if (get->u.roll.cc == TABLEAU_CC_CR)	/* time to read red?				*/
  {
	dtstr_printf(ctl, fn, sizeof(fn),	/* make red filename 				*/
      "%s/%08ld.cr.%s", 
      get->u.roll.dir, 
      get->u.roll.frame,
      get->u.roll.suffix);
	get->u.roll.cc = TABLEAU_CC_Y;		/* next time read luminance			*/
  }
  else									/* time to read luminance? 			*/
  {
	dtstr_printf(ctl, fn, sizeof(fn),	/* make luminance filename			*/
      "%s/%08ld.%s", 
      get->u.roll.dir, 
      get->u.roll.frame,
      get->u.roll.suffix);
	get->u.roll.frame++;				/* simple advance				    */
	get->u.roll.suffix = "enc";			/* after this get encoded files 	*/
	get->u.roll.cc = TABLEAU_CC_N;		/* next time get a new frame		*/
  }

  if (*header == NULL)					/* don't have an old header? 		*/
  DT_C(dtos_malloc2,(ctl,				/* allocate space for header 		*/
    (void **)header,
    sizeof(tableau_header_t),
    F, "header"));
  
  DT_C(dtfd_open,(ctl, &fd, fn, "r"));	/* open encoded file to read		*/

  DT_C(dtfd_read_all,(ctl, &fd,			/* read header from file			*/
    *header,
    sizeof(tableau_header_t)));
  
  if (*l < (*header)->l)				/* given buffer is too small? 		*/
  {
	if (*l != 0)						/* we even have a given buffer? 	*/
	  DT_Q(dtos_free2,(ctl, *data,
        F, "roll file data"));
	*l = 0;
    DT_Q(dtos_malloc2,(ctl, data,		/* space for the data 				*/
      DT_CAST(unsigned int, 
        (*header)->l),
      F, "roll file data"));
	*l = (*header)->l;					/* remember how big it is 			*/
  }
	 
  DT_C(dtfd_read_all,(ctl, &fd,			/* read encoded data from file		*/
    *data, (*header)->l));
  
cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));		/* close file 						*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_roll_free(
  dtparam_t *param,
  tableau_get_t *get)
{
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_roll_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
{
  DT_F("tableau_get_roll_setup");
  dt_ctl_t *ctl = &param->ctl;

  int isdir;
  dtstr_printf(ctl,						/* make roll directory name 		*/
     get->u.roll.dir,
    sizeof(get->u.roll.dir), 
    "%s/%s",
    param->path,
	strchr(spec, ',')+1);
  DT_Q(dtos_isdir,(ctl,					/* make sure directory exists 		*/
    get->u.roll.dir, &isdir));
  if (!isdir)
 	return dt_err(ctl, F,
      "\"%s\" is not a directory",
      get->u.roll.dir);

  get->u.roll.frame =					/* starting frame 					*/
    DT_CAST(tableau_len_t,
      param->offset);
	
  get->u.roll.suffix = "cur";			/* start by reading full file		*/

  get->u.roll.cc = TABLEAU_CC_N;		/* first time get a new frame 		*/

  get->function = tableau_get_roll;
  get->free = tableau_get_roll_free;

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
