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
| ustate_* - ustate internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustate $RCSfile: ustatelib.c,v $ $Revision: 1.8 $");
#include <dtack/list.h>
#include <ustate.h>

#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/liner.h>

#include <dtack/label.h>

/*..........................................................................
 * initialize ustate structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustate_init_config(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate,						/* ustate object 					*/
  const char *config_filename)			/* config file to read 				*/
{
  DT_F("ustate_init_config");
  dtfd_t fd;
  int isreadable = 0;
  dt_rc_e rc;

  fd.fd = -1;

  memset(ustate, 0, sizeof(*ustate));	/* clear the structure 				*/
  
  DT_C(dtlist_create,(ctl,
    &ustate->list, "ustate"));

  if (config_filename != NULL)			/* non-NULL config filename? 		*/
    DT_C(dtos_isreadable,(ctl,
      config_filename, &isreadable));
  
  if (isreadable)						/* config file exists? 				*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
      "reading config file %s",
      config_filename);

    DT_C(dtfd_open,(ctl, &fd,			/* open config file 				*/
      config_filename, "r"));

    DT_C(dtliner_fd_list,(ctl, &fd,		/* make list from file 				*/
      USTATE_CONFIG_COMMENT, 
      USTATE_CONFIG_SEPARATOR, 
      &ustate->list));
  
    DT_C(dtfd_close,(ctl, &fd));		/* close the file 					*/
  }
  else
  {
	if (config_filename == NULL)
	  config_filename = "NULL";
    dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
      "can't open config file %s",
       config_filename);
  }

  DT_C(dtos_strdup,(ctl,				/* remember the filename 			*/ 
    config_filename,
    (char **)&ustate->config_filename));

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  if (rc != DT_RC_GOOD)
    DT_I(ustate_uninit,(ctl, ustate));

  return rc;
}

/*..........................................................................
 * search for ident.program.attribute value
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustate_search(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate,						/* ustate object 					*/
  const char *ident,					/* object's instance ident			*/
  const char *program,					/* program's name 					*/
  const char *attribute,				/* attribute name 					*/
  const char *dflt,						/* default value 					*/
  const char **value)					/* returned value 					*/
{
  DT_F("ustate_search");
  char key[256];
  dtlist_elem_t *elem;

  DT_Q(dtstr_printf,(ctl,				/* form search key 					*/
    key, sizeof(key),
    "%s.%s.%s",
    ident, program, attribute));

  DT_Q(dtlist_search,(ctl,				/* search for ident					*/
    &ustate->list, key, &elem));

  if (elem == NULL)						/* key not found in list?			*/
	*value = dflt;						/* use default value 				*/
  else									/* key found in list? 				*/
    *value = (const char *)elem->user;	/* use value from list 				*/
	
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "%s=\"%s\"%s",
    key, *value, elem? "": " (default)");

  return DT_RC_GOOD;
}

/*..........................................................................
 * search for ident.program.attribute value
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustate_find(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate,						/* ustate object 					*/
  const char *ident,					/* object's instance ident			*/
  const char *program,					/* program's name 					*/
  const char *attribute,				/* attribute name 					*/
  const char *dflt,						/* default value 					*/
  const char **value)					/* returned value 					*/
{
  DT_F("ustate_find");
  char key[256];
  dtlist_elem_t *elem;

  DT_Q(dtstr_printf,(ctl,				/* form search key 					*/
    key, sizeof(key),
    "%s.%s.%s",
    ident, program, attribute));

  DT_Q(dtlist_search,(ctl,				/* search for ident					*/
    &ustate->list, key, &elem));

  if (elem == NULL)						/* key not found in list?			*/
  {
	if (dflt == NULL)
	  return dt_err(ctl, F, 
        DT_LABEL_NOTFOUND
        " %s does not have a default",
        key);
	*value = dflt;						/* use default value 				*/
  }
  else									/* key found in list? 				*/
    *value = (const char *)elem->user;	/* use value from list 				*/
	
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "%s=\"%s\"%s",
    key, *value, elem? "": " (default)");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
ustate_uninit(
  dt_ctl_t *ctl,						/* environment control 				*/
  ustate_t *ustate)						/* ustate object 					*/
{
  DT_F("ustate_uninit");
  dt_rc_e rc = DT_RC_GOOD;

  if (ustate != NULL)
  {
	if (ustate->config_filename != NULL)
	  DT_I(dtos_free2,(ctl, 
        (void *)ustate->config_filename,
        F, "ustate config_filename"));

	if (ustate->list.elem)				/* list was initialized? 			*/
	{
	  DT_I(dtlist_free,(ctl,			/* free list elements 				*/
        &ustate->list));
	  DT_I(dtlist_destroy,(ctl,			/* destroy list itself 				*/
        &ustate->list));
	}

	memset(ustate, 0, sizeof(*ustate));	/* clear the structure 				*/
  }

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
