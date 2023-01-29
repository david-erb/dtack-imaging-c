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

DT_RCSID("app/tableau $RCSfile: roll.c,v $ $Revision: 1.8 $");

#define MASK DT_DBG_MASK_OBJECT

/*..........................................................................
 * form roll file name
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_roll_name(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  char *s,
  int m)
{
  DT_F("tableau_roll_name");
  DT_Q(dtstr_printf,(ctl, s, m,
    "%s/%s.rol", path, camname));
  return DT_RC_GOOD;
}

/*..........................................................................
 * add new entry to roll file
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_roll_add(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  tableau_len_t frame)
{
  DT_F("tableau_roll_add");
  char s[128];
  dtfd_t fd;
  dt_rc_e rc;
  fd.fd = -1;
  DT_C(tableau_roll_name,(ctl,			/* form roll file name 				*/
    path, camname, s, sizeof(s)));
  DT_C(dtfd_open,(ctl, &fd, s, "w"));	/* open file 						*/
  DT_C(dtfd_write_all,(ctl, &fd,		/* write the sequence number 		*/
    &frame, sizeof(frame)));
cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
    DT_I(dtfd_close,(ctl, &fd));		/* close the file 					*/
  if (DT_DBG(ctl, MASK))
  dt_dbg(ctl, F, MASK,
    "wrote %ld to roll file %s",
    frame, s);
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_roll_get(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  tableau_len_t frame,					/* frame wanted 					*/
  tableau_len_t *newest)				/* newest available frame 			*/
{
  DT_F("tableau_roll_get");
  char s[128];
  dtfd_t fd;
  long got;
  dt_rc_e rc;
  fd.fd = -1;
  DT_C(tableau_roll_name,(ctl,			/* form roll file name 				*/
    path, camname, s, sizeof(s)));
  again:
  {
	DT_C(dtfd_open,(ctl, &fd, s, "r"));	/* open file 						*/
    DT_C(dtfd_read,(ctl, &fd,			/* read the file 					*/
      newest, sizeof(*newest), 
      &got));
    DT_C(dtfd_close,(ctl, &fd));		/* close the file 					*/
	fd.fd = -1;
	if (got < (long)sizeof(*newest) ||	/* file was being updated? 			*/
		frame > *newest)				/* or frame isn't there yet? 		*/
	{
	  DT_C(dtos_delay,(ctl, 1000));		/* wait a little while 				*/
	  goto again;
	}
  } 
cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
    DT_I(dtfd_close,(ctl, &fd));		/* close the file 					*/
  return rc;
}

/*..........................................................................
 * initialize roll file if it doesn't exist
 * scan directory for highest frame if roll file exists but has zero length
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_roll_init(
  dt_ctl_t *ctl,
  const char *path,
  const char *camname,
  const char *template)
{
  DT_F("tableau_roll_init");
  char s[128];
  int isreadable;
  dtfd_t fd;
  tableau_len_t frame;
  long got = 0;
  dt_rc_e rc;

  fd.fd = -1;

  DT_C(tableau_roll_name,(ctl,			/* form roll file name 				*/
    path, camname, s, sizeof(s)));
  DT_C(dtos_isreadable,(ctl, s, 
    &isreadable));

  if (isreadable)						/* roll file exists?				*/
  {
	DT_C(dtfd_open,(ctl, &fd, s, "r"));	/* open file 						*/
    DT_C(dtfd_read,(ctl, &fd,			/* read the file 					*/
      &frame, sizeof(frame), &got));
    DT_C(dtfd_close,(ctl, &fd));		/* close the file 					*/
	if (got == sizeof(frame) &&			/* we got a valid frame?			*/
		DT_DBG(ctl, MASK))
   	  dt_dbg(ctl, F, MASK,
        "existing roll file %s holds"
        " frame %ld",
        s, frame);
  }
  else									/* roll file does not exist? 		*/
	dt_dbg(ctl, F, MASK,
      "creating new roll file %s", s);

  if (got < (long)sizeof(tableau_len_t)) /* didn't get a frame yet?			*/
  {
	if (template != NULL)				/* we can scan for existing files? 	*/
	{
	  DT_C(tableau_dir_scan,(ctl,		/* find highest frame number 		*/
        template, &frame));
	  dt_dbg(ctl, F, MASK,
        "roll file %s clipped,"
        " found frame %ld from \"%s\"",
        s, frame, template);
	}
	else
	  frame = -1;

	DT_C(tableau_roll_add,(ctl,			/* write the frame to the file		*/
      path, camname, frame));
  }

cleanup:
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));		/* close the file 					*/
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
