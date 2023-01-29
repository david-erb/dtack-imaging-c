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

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/list.h>

DT_RCSID("source/low $RCSfile: list_io.c,v $ $Revision: 1.1 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_fwrite(							/* write all elements of list		*/
  dt_ctl_t *ctl,
  dtlist_t *list,
  char *filename,
  unsigned int size)
{
  DT_F("dtlist_fwrite");
  dtfd_t fd;
  dtlist_elem_t *elem;
  unsigned long l;
  dt_rc_e rc;

  DT_Q(dtfd_open,(ctl, &fd,				/* open and set file to zero length	*/
    filename, "w"));

  DT_C(dtfd_write_all,(ctl, &fd,		/* write magic						*/
    DTLIST_MAGIC1, 
    sizeof(DTLIST_MAGIC1)));

  l = size;
  DT_C(dtfd_write_all,(ctl, &fd,		/* write size of user data		    */
    &l, sizeof(l)));

  l = list->n;
  DT_C(dtfd_write_all,(ctl, &fd,		/* write number of elements		    */
    &l, sizeof(l)));

  elem = DTLIST_ELEM_HEAD;
  while (elem)
  {
    DT_C(dtlist_next,(ctl, list,		/* locate next elem in list		    */
      elem, &elem));
    if (elem)							/* there is a next elem?		    */
	{
      l = strlen(elem->name) + 1;
      DT_C(dtfd_write_all,(ctl, &fd,	/* write length of name			    */
        &l, sizeof(l)));
      DT_C(dtfd_write_all,(ctl, &fd,	/* write name itself			    */
        elem->name, l));
      DT_C(dtfd_write_all,(ctl, &fd,	/* write user data				    */
        elem->user, size));
	}
  }

cleanup:       
  DT_Q(dtfd_close,(ctl, &fd));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_fread(							/* read all elements of list		*/
  dt_ctl_t *ctl,
  dtlist_t *list,
  char *filename,
  unsigned int *size)
{
  DT_F("dtlist_fread");
  dtfd_t fd;
  int i, n;
  unsigned long l;
  char *elem_name = NULL;
  void *elem_user = NULL;
  char magic[sizeof(DTLIST_MAGIC1)];
  dt_rc_e rc, retrc;

  DT_Q(dtfd_open,(ctl, &fd,				/* open existing file				*/
    filename, "r"));

  DT_C(dtfd_read_all,(ctl, &fd,			/* read magic						*/
    magic, sizeof(magic)));

  if (strcmp(magic, DTLIST_MAGIC1))
  {
	rc = dt_err(ctl, F,
      "\"%s\" not a list file (bad magic)",
      filename);
    goto cleanup;
  }

  DT_C(dtfd_read_all,(ctl, &fd,			/* read size of each element		*/
    &l, sizeof(l)));
  *size = (unsigned int)l;

  DT_C(dtfd_read_all,(ctl, &fd,			/* read number of elements			*/
    &l, sizeof(l)));
  n = (int)l;

  for (i=0; i<n; i++)
  {
	DT_C(dtfd_read_all,(ctl, &fd,		/* read length of name			    */
      &l, sizeof(l)));
	  DT_C(dtos_malloc2,(ctl,			/* space for name				    */
        (void **)&elem_name, l,
        F, "elem name"));
	DT_C(dtfd_read_all,(ctl, &fd,		/* read name					    */
      elem_name, l));
	DT_C(dtos_malloc2,(ctl,				/* space for user data			    */
      &elem_user, *size,
      F, "elem user"));
	DT_C(dtfd_read_all,(ctl, &fd,		/* read user data				    */
      elem_user, *size));
	DT_C(dtlist_after,(ctl, list,		/* add to list					    */
      DTLIST_ELEM_TAIL,
      elem_name, elem_user, NULL));
    elem_name = NULL;					/* don't clean up these values		*/
    elem_user = NULL;
  }

cleanup:
  retrc = rc;

  if (elem_name != NULL)
    DT_G(dtos_free2,(ctl, elem_name,
      F, "partial elem name"));
  if (retrc == DT_RC_GOOD)
    retrc = rc;

  if (elem_user != NULL)
    DT_G(dtos_free2,(ctl, elem_user,
      F, "partial elem user"));
  if (retrc == DT_RC_GOOD)
    retrc = rc;

  DT_G(dtfd_close,(ctl, &fd));
  if (retrc == DT_RC_GOOD)
    retrc = rc;

  return retrc;
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
