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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000717 DE added tabrobot_preset_load() function
  19990517 DE added param->precision to presets filename generation
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("app/tabrobot $RCSfile: preset.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
tabrobot_preset_set(
  dtparam_t *param,
  const char *preset,
  const char *desc)
{
  DT_F("tabrobot_preset_set");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t fd;
  char filename[256];
  dtlist_t list;
  int list_created = 0;
  dtlist_elem_t *elem = DTLIST_ELEM_HEAD;
  int found = 0;
  dt_rc_e rc;

  fd.fd = -1;

  if (param->config == NULL)
	return dt_err(ctl, F,
      "no presets file configured");


  if (param->precision)
	DT_C(dtstr_printf,(ctl,				/* form filename 					*/
      filename, sizeof(filename),
      "%s/%s", 
      param->config, 
      param->precision))
  else
	DT_C(dtstr_printf,(ctl,				/* form filename 					*/
      filename, sizeof(filename),
      "%s", 
      param->config));

  DT_C(dtfd_open,(ctl, &fd,				/* open filename to read 			*/
    filename, "r"));
  DT_C(dtlist_create,(ctl, &list,		/* create list to read into 		*/
    "presets"));
  list_created = 1;
  DT_C(dtliner_fd_list,(ctl, &fd,		/* read file into list 				*/
    "#", "=", &list));
  DT_C(dtfd_close,(ctl, &fd));			/* close file 						*/
  fd.fd = -1;

  DT_C(dtfd_open,(ctl, &fd,				/* reopen file for writing 			*/
    filename, "w"));

  while(elem)							/* scan element list 				*/
  {
	DT_C(dtlist_next,(ctl, &list,		/* get next element 				*/
      elem, &elem));
	if (elem != NULL)					/* not last element? 				*/
	{
	  const char *olddesc =				/* reference old description 		*/
        (char *)elem->user;
	  DT_C(dtfd_write_all,(ctl, &fd,	/* write preset number 				*/
        elem->name, 
        strlen(elem->name)));
	  DT_C(dtfd_write_all,(ctl, &fd,	/* separator 						*/
        "=", 1));
	  if (!strcmp(elem->name, preset))	/* this is our preset? 				*/
	  {
		DT_C(dtfd_write_all,(ctl, &fd,	/* write new description 			*/
          desc, strlen(desc)));
		found = 1;
	  }
	  else								/* not our preset? 					*/
		DT_C(dtfd_write_all,(ctl, &fd,	/* write old description 			*/
          olddesc, strlen(olddesc)))	/* end of line 						*/
	  DT_C(dtfd_write_all,(ctl, &fd,
        "\r\n", 2));
	}
  }
  if (!found)
  {
	DT_C(dtfd_write_all,(ctl, &fd,		/* write preset number 				*/
      preset,
      strlen(preset)));
	DT_C(dtfd_write_all,(ctl, &fd,		/* separator 						*/
      "=", 1));
	DT_C(dtfd_write_all,(ctl, &fd,		/* write new description 			*/
       desc, strlen(desc)));
	DT_C(dtfd_write_all,(ctl, &fd,
      "\r\n", 2));
  }
cleanup:
  if (list_created)
  {
	DT_I(dtlist_free,(ctl, &list));
	DT_I(dtlist_destroy,(ctl, &list));
  }
  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_preset_get(
  dtparam_t *param,
  const char *preset,
  char *desc,
  int max)
{
  DT_F("tabrobot_preset_get");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t fd;
  char filename[256];
  dtlist_t list;
  int list_created = 0;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  fd.fd = -1;

  if (param->config == NULL)
	return dt_err(ctl, F,
      "no presets file configured");

  if (param->precision)
	DT_C(dtstr_printf,(ctl,				/* form filename 					*/
      filename, sizeof(filename),
      "%s/%s", 
      param->config, 
      param->precision))
  else
	DT_C(dtstr_printf,(ctl,				/* form filename 					*/
      filename, sizeof(filename),
      "%s", 
      param->config));

  DT_C(dtfd_open,(ctl, &fd,				/* open filename to read 			*/
    filename, "r"));
  DT_C(dtlist_create,(ctl, &list,		/* create list to read into 		*/
    "presets"));
  list_created = 1;
  DT_C(dtliner_fd_list,(ctl, &fd,		/* read file into list 				*/
    "#", "=", &list));
  DT_C(dtfd_close,(ctl, &fd));			/* close file 						*/
  fd.fd = -1;

  DT_C(dtlist_search,(ctl, &list,
    preset, &elem));
  if (elem == NULL)
	DT_C(dtstr_printf,(ctl, 
      desc, max, ""))
  else
	DT_C(dtstr_printf,(ctl, 
      desc, max, "%s",
      (const char *)elem->user));

cleanup:
  if (list_created)
  {
	DT_I(dtlist_free,(ctl, &list));
	DT_I(dtlist_destroy,(ctl, &list));
  }
  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));

  return rc;
}

/*..........................................................................
 * Load preset list if ~mtime~ is changed.
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_preset_load(
  dtparam_t *param,
  dtlist_t *list,
  long *mtime)
{
  DT_F("tabrobot_preset_load");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t fd;
  char filename[256];
  int isdir, isreadable;
  long newmtime;
  dtlist_elem_t *elem;
  dt_rc_e rc;

  DTFD_CLEAR(ctl, &fd);

  if (param->config == NULL)
	return dt_err(ctl, F,
      "no presets file configured");

  if (param->precision)
	DT_C(dtstr_printf,(ctl,				/* form filename 					*/
      filename, sizeof(filename),
      "%s/%s", 
      param->config, 
      param->precision))
  else
	DT_C(dtstr_printf,(ctl,				/* form filename 					*/
      filename, sizeof(filename),
      "%s", 
      param->config));

  DT_C(dtos_isdir,(ctl, filename,
    &isdir));
  if (isdir)
	DT_CLEANUP((ctl, F,
      "\"%s\" is a directory", 
      filename));

  DT_C(dtos_isreadable,(ctl, filename,
    &isreadable));
  if (!isreadable)
	DT_CLEANUP((ctl, F,
      "\"%s\" is not a readable file", 
      filename));

  DT_C(dtos_futil_mtime,(ctl,			/* return file's last mod time		*/
    filename, &newmtime));
  if (newmtime == 0)
	DT_CLEANUP((ctl, F,
      "could not get mtime of file"
      " \"%s\"", filename));
  if (*mtime == newmtime)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "\"%s\" has not changed", filename);
	goto cleanup;
  }

  *mtime = newmtime;

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "reading \"%s\"", filename);

  DT_C(dtfd_open,(ctl, &fd,				/* open filename to read 			*/
    filename, "r"));
  DT_C(dtlist_free,(ctl, list));		/* free previous list 				*/
  DT_C(dtliner_fd_list,(ctl, &fd,		/* read file into list 				*/
    "#", "=", list));

cleanup:
  if (DTFD_ISOPEN(ctl, &fd))
	DT_I(dtfd_close,(ctl, &fd));

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
