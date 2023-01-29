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
| Pump from already-compressed directory.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: clip.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>

/*..........................................................................
 * wait for next available file and return its name
 *..........................................................................*/

static
dt_rc_e
tabdrone_clip_read(
  tabdrone_t *tabdrone,
  dtfd_t *fd,
  long bytes,
  long offset,
  void **buffer,
  long *nbuffer,
  long *obuffer)
{
  DT_F("tabdrone_clip_read");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  dt_rc_e rc;

  if (bytes == 0)						/* file size not known? 			*/
    DT_C(dtfd_size,(ctl, fd, obuffer))	/* get file size 					*/
  else
    *obuffer = bytes;

  if (*buffer == NULL)					/* no buffer allocated yet? 		*/
	*nbuffer = 0;

  if (*nbuffer < *obuffer)				/* buffer is too small? 			*/
  {
	if (*buffer != NULL)
	  DT_C(dtos_free2,(ctl, *buffer,
        F, "buffer"));
	DT_C(dtos_malloc2,(ctl, buffer,
      *obuffer, F, "buffer"));
	*nbuffer = *obuffer;
  }

  DT_C(dtfd_seek,(ctl, fd,				/* seek to offset 					*/
    offset, DTFD_WHENCE_SET, NULL));

  DT_C(dtfd_read_all,(ctl, fd,			/* read entire file contents 		*/
    *buffer, *obuffer));
  
cleanup:
  return rc;
}

/*..........................................................................
 * read name of next file from slideshow file
 * return zero-length filename if end of list is found
 *..........................................................................*/

static
dt_rc_e
tabdrone_clip_compensate(
  tabdrone_t *tabdrone,
  int *skip)
{
  DT_F("tabdrone_clip_compensate");
  dt_ctl_t *ctl = tabdrone->ctl;		/* environment control 				*/

  *skip = 0;
  if (tabdrone->slide_dt > 0.0)		/* synchronous output? 				*/
  {
	if (tabdrone->slide_st == 0.0)		/* first frame? 					*/
	{
	  dtos_time_t now;
	  DT_Q(dtos_time,(ctl, &now));
	  tabdrone->slide_pt = now.wall;
	  tabdrone->slide_st = now.wall;
	  tabdrone->slide_rt = 0.0;
	}
	else								/* not first frame? 				*/
	{
	  dtos_time_t now;
	  DT_Q(dtos_time,(ctl, &now));
	  tabdrone->slide_pt = now.wall -	/* playback time 					*/
		tabdrone->slide_st;
	  tabdrone->slide_rt +=			/* record time 						*/
		tabdrone->slide_dt;
	  if (tabdrone->slide_pt <			/* playing too fast? 				*/
		  tabdrone->slide_rt)
	  {
		long delay = (long)
          ((tabdrone->slide_rt-
			tabdrone->slide_pt) * 1000.0);
	    DT_Q(dtos_delay,(ctl, delay));
		dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
          "delaying %d dt=%g st=%g pt=%g rt=%g", 
          delay,
          tabdrone->slide_dt,
          tabdrone->slide_st,
          tabdrone->slide_pt,
          tabdrone->slide_rt);
	  }
	  else							
	  if (tabdrone->slide_pt -			/* playing too slow? 				*/
		  tabdrone->slide_rt >
		  tabdrone->slide_dt)
	  {
		*skip = 1;
	  }
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * read name of next file from slideshow file
 * return zero-length filename if end of list is found
 *..........................................................................*/

static
dt_rc_e
tabdrone_clip_next(
  tabdrone_t *tabdrone,
  dtfd_t *fd,				
  char *filename,
  int max,
  long *offset,
  long *bytes)
{
  DT_F("tabdrone_clip_next");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  FILE *file = fd->file;
  int l;
  int i;
  int w;
  int skip;
  double dt;
  char *p;
  dt_rc_e rc = DT_RC_GOOD;

again:
  filename[0] = '\0';	
  fgets(filename, max, file);			/* read line from slide show file 	*/
  l = strlen(filename);
  while(l > 0 &&
		(filename[l-1] == '\r' ||
		 filename[l-1] == '\n'))
    filename[--l] = '\0';

  if (!strcmp(filename, "loop"))		/* the loop directive? 				*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "loop");
	fseek(file, 0, SEEK_SET);			/* go back to start of slide show 	*/
    DT_C(tabdrone_clip_compensate,(	/* compensate for frametime 		*/
      tabdrone, &skip));
    goto again;
  }
  
  if (!strcmp(filename, "stop") ||		/* the stop directive? 				*/
	  filename[0] == '\0')
  {
	dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "stop");
	filename[0] = '\0';	
	goto cleanup;						/* indicate end of file				*/
  }
  
  l = sscanf(filename, "sleep %d", &w);
  if (l == 1)							/* the sleep directive? 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "sleep %d", w);
	DT_C(dtos_delay,(ctl, w));
    goto again;
  }
  
  l = sscanf(filename, 
    "frametime %le", &dt);
  if (l == 1)							/* the rate directive? 				*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "frametime %g", dt);
    tabdrone->slide_dt = dt;
    goto again;
  }

  *offset = 0;							/* presume start of file 			*/
  *bytes = 0;							/* presume file has one jpeg		*/
  
  p = strchr(filename, ' ');			/* point at space in filename 		*/
  if (p != NULL)						/* filename contains a space? 		*/
  {
	*p++ = '\0';						/* terminate filename 				*/
    l = sscanf(p,						/* scan for numbers after filename 	*/
      "%ld %ld", offset, bytes);
  }

  DT_C(tabdrone_clip_compensate,(		/* compensate for frametime 		*/
    tabdrone, &skip));
  if (skip)
  {
	 dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "skipping %s", filename);
	 goto again;						/* skip this frame 					*/
  }

										/* else just a filename 			*/

cleanup:

  if (rc == DT_RC_GOOD)
    dt_dbg(ctl, F, DT_DBG_MASK_IMG,
      "returning filename=\"%s\" *offset=%ld *bytes=%ld", 
      filename, *offset, *bytes);
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_clip(							/* pump clip input to output		*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
{
  DT_F("tabdrone_clip");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  dtfd_t listfd;
  dtfd_t jpegfd;
  char filename[256];
  char lastname[256];
  long i;
  long offset;
  long bytes;
  void *buffer = NULL;
  long nbuffer;
  long obuffer;
  dt_rc_e rc = DT_RC_GOOD;

  listfd.fd = -1;
  jpegfd.fd = -1;

  DT_C(dtfd_open,(ctl, &listfd, 
    param->input, "r"));

  tabdrone->slide_dt = 0.0;
  tabdrone->slide_st = 0.0;
  tabdrone->slide_pt = 0.0;
  tabdrone->slide_rt = 0.0;

  lastname[0] = '\0';
  while (1)
  {
	DT_C(tabdrone_feedback,(tabdrone,	/* check for input 					*/
      ipc));

	DT_C(tabdrone_clip_next,(tabdrone,	/* get next filename from playlist 	*/
      &listfd, 
      filename, sizeof(filename),
      &offset, &bytes));
	
	if (filename[0] == '\0')			/* end of slide show? 				*/
	  goto cleanup;
										/* ................................ */
	if (strcmp(filename, lastname))		/* new filename request? 			*/
	{
  	  if (jpegfd.fd != -1)				/* previous file is still open?		*/
  	    DT_C(dtfd_close,(ctl,			/* close previous file 				*/
          &jpegfd));
  	  DT_C(dtfd_open,(ctl, &jpegfd,		/* open new filename 				*/
        filename, "r"));
	  DTSTR_CPY(ctl, lastname, 
        filename);
	}

	if (bytes == 0)
	  DT_C(dtfd_size,(ctl, &jpegfd,		/* find file size					*/
        &bytes));


	DT_C(tabdrone_clip_read,(tabdrone,	/* read frame						*/
      &jpegfd, bytes, offset,
      &buffer, &nbuffer, &obuffer));

	DT_C(tabdrone_output,(tabdrone,		/* output frame						*/
      buffer, obuffer, 
      strrchr(filename, '.'),			/* suffix of filename 				*/
      ipc));

	DT_C(tabdrone_limit,(tabdrone));	/* enforce time limit 				*/
  }

cleanup:
  if (buffer != NULL)
	DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));

  if (jpegfd.fd != -1)
	DT_I(dtfd_close,(ctl, &jpegfd));

  if (listfd.fd != -1)
	DT_I(dtfd_close,(ctl, &listfd));
  
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
