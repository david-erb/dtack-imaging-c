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
DT_RCSID("app/tabdrone $RCSfile: live.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>

#ifndef DTDEPEND
#include <sys/types.h> // this and next for file info.
#include <sys/stat.h>
#endif

/*..........................................................................
 * wait for next available file and return its name
 *..........................................................................*/

static
dt_rc_e
tabdrone_live_mtime(
  tabdrone_t *tabdrone,
  const char *filename,
  long *mtime)
{
  DT_F("tabdrone_live_mtime");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;

  int sysrc;
  struct stat fstat;
  int e;
  
  sysrc = stat(filename, &fstat);
  e = errno;
  if (sysrc != 0)
	dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "stat(\"%s\") failed"
      DTERRNO_FMT,
      filename,
      DTERRNO_ARG(e));
  else
	*mtime = fstat.st_mtime;
	
  return DT_RC_GOOD;
}

/*..........................................................................
 * wait for next available file and return its name
 *..........................................................................*/

static
dt_rc_e
tabdrone_live_read(
  tabdrone_t *tabdrone,
  const char *filename,
  void **buffer,
  long *nbuffer,
  long *obuffer)
{
  DT_F("tabdrone_live_read");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  dtfd_t fd;
  dt_rc_e rc;
  fd.fd = -1;

  DT_C(dtfd_open,(ctl, &fd,				/* open encoded file to read		*/
    filename, "r"));	

  DT_C(dtfd_size,(ctl, &fd, obuffer));	/* get file size 					*/

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

  DT_C(dtfd_read_all,(ctl, &fd,			/* read entire file contents 		*/
    *buffer, *obuffer));
  
cleanup:
  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));
	
  return rc;
}

/*..........................................................................
 * pump audio if any
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_audio_check(
  tabdrone_t *tabdrone,
  dtipc_t *ipc)
{
  DT_F("tabdrone_audio_check");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  long mtime = 0;
  void *buffer = NULL;
  long nbuffer;
  long obuffer;
  dt_rc_e rc;

  DT_C(tabdrone_live_mtime,(tabdrone,	/* get mtime of audio file 			*/
    param->variant, &mtime)); 

  if (mtime == 0 ||						/* audio doesn't exist right now? 	*/
	  mtime == tabdrone->audio_mtime)	/* already sent this one? 			*/
	goto cleanup;

  DT_C(tabdrone_live_read,(tabdrone,	/* read audio file					*/
    param->variant,
    &buffer, &nbuffer, &obuffer));
	
  DT_C(tabdrone_output,(tabdrone,		/* output frame						*/
    buffer, obuffer, 
    strrchr(param->variant, '.'),		/* suffix of filename 				*/
    ipc));

  if (tabdrone->audio_mtime == 0)		/* first audio? 					*/
  {
	const char *s;
	char *filename;
	char *p;
	s = strrchr(param->variant, '.');	/* save audio file suffix 			*/
    DT_C(dtos_strdup3,(ctl,				/* space for audio filename 		*/
      param->input, "", s, 
      &filename));
	p = strchr(filename, '%');			/* find '%' in video frame pattern 	*/
    if (p != NULL)
    {
	  while(p > filename &&				/* back up over '.' before '%' 		*/
			p[-1] == '.')
		p--;
	  *p = '\0';
	  strcat(p, s);						/* add audio suffix 				*/
	}
	DT_C(dtos_free2,(ctl,				/* release welcome filename 		*/
      (char *)param->variant,
      F, param->variant));
	param->variant = filename;			/* remember audio filename 			*/

	DT_C(tabdrone_live_mtime,(			/* get mtime of existing audio file	*/
      tabdrone,
      param->variant, 
      &tabdrone->audio_mtime));
  }
  else
	tabdrone->audio_mtime = mtime;
  
cleanup:
  if (buffer != NULL)
	DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));

  return rc;
}

/*..........................................................................
 * wait for next available file and return its name
 *..........................................................................*/

static
dt_rc_e
tabdrone_live_wait(
  tabdrone_t *tabdrone,
  dtipc_t *ipc,
  tabdrone_frame_t *pframe,
  int first,
  char *filename,
  int max)
{
  DT_F("tabdrone_live_wait");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  int frame = *pframe + 2;				/* frame we need to see				*/
  int isreadable;
  int skipped;
  long wait_max = 180000;				/* max wait in milliseconds 		*/
  long wait_inc = 250;					/* wait increment in milliseconds 	*/
  if (wait_inc > wait_max)
	wait_inc = wait_max;

  DT_Q(tabdrone_audio_check,(			/* pump audio if any				*/
    tabdrone, ipc));

  while (1)
  {
	dtstr_printf(ctl, filename, max,	/* make filename from template 		*/
      param->input, frame);
	DT_Q(dtos_isreadable,(ctl,			/* see if file exists 				*/
      filename, &isreadable));
	if (!isreadable)					/* this frame not readable yet?		*/
	  break;
	frame++;							/* advance to next file 			*/
  } 

  skipped = frame - *pframe;
  if (!first && skipped == 2)			/* new frame not here yet? 			*/
  {
	long waited = 0;
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "waiting for %s", filename);
	while (1)
	{
	  DT_Q(dtos_delay,(ctl, wait_inc));	/* sleep a short while				*/
      DT_Q(tabdrone_audio_check,(		/* pump audio if any				*/
        tabdrone, ipc));
	  DT_Q(dtos_isreadable,(ctl,		/* see if file exists now			*/
        filename, &isreadable));
	  if (isreadable)					/* got it? 							*/
	    break;
	  waited += wait_inc;
	  if (waited >= wait_max)
	    return dt_err(ctl, F,
          "waited too long"
          " for new frame %s", 
          filename);
	}
  }
  else
  {
	frame--;
	if (skipped > 3)
      dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
        "skipped %d frame%s",
        skipped-3, skipped==4? "": "s");
  }

  frame--;								/* back up to last complete file 	*/
  dtstr_printf(ctl, filename, max,		/* make filename from template 		*/
    param->input, frame);

  *pframe = frame;
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "doing %s", filename);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_live(							/* pump live input to output		*/
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
{
  DT_F("tabdrone_live");
  dt_ctl_t *ctl = tabdrone->ctl;
  dtparam_t *param = tabdrone->param;
  tabdrone_frame_t frame;
  char filename[512];
  void *buffer = NULL;
  long nbuffer;
  long obuffer;
  int first = 1;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
	"scanning for first jpeg"
    " pattern=\"%s\"", param->input);
  DT_C(tabdrone_dir_scan,(tabdrone,		/* scan for first frame 			*/
    param->input, &frame));

  while(1)								/* frame loop 						*/
  {
	DT_C(tabdrone_feedback,(tabdrone,	/* check for input 					*/
      ipc));

	DT_C(tabdrone_live_wait,(tabdrone,	/* wait for new frame to arrive 	*/
      ipc, &frame, first,
      filename, sizeof(filename)));
	first = 0;

	DT_C(tabdrone_live_read,(tabdrone,	/* read frame						*/
      filename, 
      &buffer, &nbuffer, &obuffer));

	DT_C(tabdrone_output,(tabdrone,		/* output frame						*/
      buffer, obuffer, 
      strrchr(filename, '.'),			/* suffix of filename 				*/
      ipc));

	DT_C(tabdrone_limit,(tabdrone));	/* enforce time limit 				*/

	DT_C(dtos_delay,(ctl,				/* delay if required 				*/
	  tabdrone->delay));
  }
  
cleanup:
  if (buffer != NULL)
	DT_I(dtos_free2,(ctl, buffer,
      F, "buffer"));
  
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
