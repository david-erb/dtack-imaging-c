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
| Windows-on-disk support routines.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>

DT_RCSID("source/low $RCSfile: dsk.c,v $ $Revision: 1.2 $");

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/dsk.h>

/*..........................................................................
| request:       input base:         output name:
| FILEBASE       window name         length min 1, max 8, only valid file chars
| FILENAME       filebase            full filename
| LIST           ignored             full filename of list file
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtdsk_makename(							/* make filename depending on type  */
  dt_ctl_t *ctl,
  const char *dirname,
  int type,
  const char *base,
  char *filename,
  unsigned int max)
{
  DT_F("dtdsk_makename");
  char *suffix;
  unsigned int l;

  switch(type)
  {
	case DTDSK_FILEBASE:
	{
	  char tmpfile[128];				/* tmp space for filename check 	*/
	  char tmp[9];
	  int i;
	  for (i=0; i<1000; i++)
	  {
		int isdir;
		int isreadable;
		DT_Q(dtos_isdir,(ctl,			/* make sure of directory			*/
          dirname, &isdir));
		if (!isdir)
		  return dt_err(ctl, F,
            "dtdsk server \"%s\""
            " is not a directory",
            dirname);
		sprintf(tmp, "%s%03d",			/* base name to try 				*/
          DTDSK_LIST_FILENAME, i);
		DT_Q(dtdsk_makename,(ctl,		/* make a filename out of it		*/
          dirname, DTDSK_FILENAME,
          tmp, tmpfile,
          sizeof(tmpfile)));
		DT_Q(dtos_isreadable,(ctl,		/* see if this one exists 			*/
          tmpfile, &isreadable));
        if (!isreadable)				/* does not exist yet? 				*/
		  break;
	  }
      if (i == 1000)
        return dt_err(ctl, F,
          "unable to make unique"
          " filename like \"%s\"",
          filename);
	  strcpy(filename, tmp);
      return DT_RC_GOOD;
	}
	case DTDSK_FILENAME: suffix = "DAT"; break;
	case DTDSK_COLORMAP: suffix = "CLR"; break;
	case DTDSK_LIST:     suffix = "LST"; base = DTDSK_LIST_FILENAME; break;
    default:             suffix = "XXX";
  }

  if (strlen(base) > 8)
    return dt_err(ctl, F, 
      "\"%s\" is too long"
      " to be used as a filename,"
      " must be 8 or less",
      base);

  l = strlen(dirname) + 1 + 8 + 1 + 3 + 1;
  if (l > max)
    return dt_err(ctl, F,
      "filename length %u for \"%s\" longer than max buffer %u",
      l, dirname, max);

  sprintf(filename, "%s/%s.%s",
    dirname, base, suffix);

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdsk_getlist(
  dt_ctl_t *ctl,
  const char *dirname,
  dtlist_t *list)
{
  DT_F("dtdsk_getlist");
  char filename[128];
  int isreadable;
  unsigned int size;
  dt_rc_e rc;

  DT_Q(dtdsk_makename,(ctl,				/* filename for the window list	   	*/
    dirname, DTDSK_LIST, NULL,
    filename, sizeof(filename)));

  DT_G(dtos_isreadable,(ctl,			/* see if window list file exists   */
    filename, &isreadable));

  if (rc == DT_RC_GOOD)
  DT_G(dtlist_create,(ctl, list,		/* make a window list			    */
    DTDSK_LIST_NAME));

  if (rc == DT_RC_GOOD && isreadable)
  {
    DT_G(dtlist_fread,(ctl, list,		/* read the current window list	    */
      filename, &size));

    if (rc == DT_RC_GOOD)
    if (size != sizeof(dtdsk_win_t))
      rc = dt_err(ctl, F,
        "got elemsize %u"
        " from window list \"%s\""
        " but expected %u",
        size,
		filename,
        sizeof(dtdsk_win_t));

    if (rc != DT_RC_GOOD)
    {
      DT_I(dtlist_free,(ctl, list));	/* free all list elements 			*/
      DT_I(dtlist_destroy,(ctl, list));	/* free list structure itself 		*/
    }
  }

  return rc;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdsk_putlist(
  dt_ctl_t *ctl,
  const char *dirname,
  dtlist_t *list)
{
  DT_F("dtdsk_putlist");
  char filename[128];

  DT_Q(dtdsk_makename,(ctl,				/* filename for the window list	    */
    dirname, DTDSK_LIST, NULL,
    filename, sizeof(filename)));

  DT_Q(dtlist_fwrite,(ctl, list,		/* write list to disk		    	*/
    filename, 
    sizeof(dtdsk_win_t)));

  return DT_RC_GOOD;
}

/*..........................................................................
| delete one window from list and remove data file
| no error if not in list or data file does not exist
| name==NULL means delete all
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdsk_delete(
  dt_ctl_t *ctl,
  const char *dirname,
  const char *name)
{
  DT_F("dtdsk_delete");
  dtlist_t list;
  dtlist_elem_t *elem;
  char filename[128];
  int found = 0;
  int isreadable;
  dt_rc_e rc;
  
  DT_Q(dtdsk_getlist,(ctl, dirname,		/* read list of windows from file	*/
    &list));
										/* ................................ */
  elem = DTLIST_ELEM_HEAD;				/* start at head of list 			*/
  do {
    DT_C(dtlist_next,(ctl, &list,		/* fetch next window structure		*/
      elem, &elem));
    if (elem != NULL)					/* not end of list?				    */
	if (name == NULL ||					/* supposed to kill them all?	    */
        !strcmp(name,					/* or it's the one we want? 		*/
          (char *)elem->name))
	{
	  dtdsk_win_t *win = 
        (dtdsk_win_t *)elem->user;
      DT_C(dtdsk_makename,(ctl,			/* filename for the window itself   */
        dirname, DTDSK_FILENAME,
        win->filebase,
        filename, sizeof(filename)));
      DT_C(dtos_isreadable,(ctl,		/* see if window file exists   		*/
        filename, &isreadable));
      if (isreadable)
        DT_C(dtos_unlink,(ctl,			/* remove the window file			*/
          filename));
      DT_C(dtos_free2,(ctl, 
        (char *)elem->name,
        F, "elem->name"));
      DT_C(dtos_free2,(ctl,
        elem->user,
        F, "elem->user"));
      if (name != NULL)					/* just doing one?				    */
	    DT_C(dtlist_delete,(ctl, &list,	/* delete window element from list 	*/
          elem));
	  found = 1;
      if (name != NULL)
        break;
	}
  } while (elem != NULL);				/* go until first match or list end */

  if (name != NULL)						/* just doing one?				    */
  {
    if (found)							/* we found and deleted an entry 	*/
      DT_C(dtdsk_putlist,(ctl, dirname,	/* replace updated list in file 	*/
        &list));
  }
  else
  {
    DT_C(dtdsk_makename,(ctl,			/* filename for the list file	   	*/
      dirname, DTDSK_LIST, NULL,
      filename, sizeof(filename)));
	DT_C(dtos_isreadable,(ctl,			/* see if list file exists   		*/
      filename, &isreadable));
	if (isreadable)
  	  DT_C(dtos_unlink,(ctl,			/* remove the list file				*/
        filename));
  }

cleanup:
  if (name != NULL)						/* didn't free them one at a time 	*/
    DT_I(dtlist_free,(ctl, &list));		/* free list					    */
  DT_I(dtlist_destroy,(ctl, &list));	/* destroy list					    */

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
