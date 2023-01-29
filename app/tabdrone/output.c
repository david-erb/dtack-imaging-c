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

#include <tabdrone.h>
DT_RCSID("app/tabdrone $RCSfile: output.c,v $ $Revision: 1.8 $");

#define BOUNDARY "The-Data-Boundary"	/* the server push separator 		*/

#define FLUSH							\
  DT_C(tabdrone_output_flush,(			\
     tabdrone, ipc, 					\
     emission, strlen(emission)));		\
  emission[0] = '\0';

#define EMIT(S) 						\
  if (strlen(emission)+strlen((S)) > 	\
	  sizeof(emission)-1)				\
  {FLUSH;}								\
  strcat(emission, (S))	

#define MIMETYPE_AU    "audio/basic"
#define MIMETYPE_JPEG  "image/jpeg"

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabdrone_output_flush(
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc,
  const void *buffer,
  long n)
{
  DT_F("tabdrone_output_flush");
  dt_ctl_t *ctl = tabdrone->ctl;
  long g;

  DT_Q(dtipc_write_some,(ctl, ipc, 	
    buffer, n, &g));	
  if (g < n)
  {
	dt_dbg(ctl, F,
      DT_DBG_MASK_COMMAND|
	  DT_DBG_MASK_USR1, 
      "client closed her end");
    return DT_RC_STOP;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabdrone_output_header(
  tabdrone_t *tabdrone,					/* application state 				*/
  dtipc_t *ipc)
{
  DT_F("tabdrone_output_header");
  dt_ctl_t *ctl = tabdrone->ctl;
  char emission[256];
  dt_rc_e rc;

  emission[0] = '\0';					/* clear emit buffer 				*/
  EMIT("HTTP/1.0 200\r\n");				/* write http response header 		*/
  EMIT(
    "Content-type: multipart/x-mixed-replace;boundary=");
  EMIT(BOUNDARY);
  EMIT("\r\n");
  EMIT("\r\n");
  FLUSH;

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_output(
  tabdrone_t *tabdrone,					/* application state 				*/
  const void *buffer,
  long nbuffer,
  const char *suffix,
  dtipc_t *ipc)
{
  DT_F("tabdrone_output");
  dt_ctl_t *ctl = tabdrone->ctl;
  char emission[256];
  char s[64];
  const char *mimetype = MIMETYPE_JPEG;	/* presume jpeg 					*/
  int isvideo = 1;						/* presume video frame 				*/
  dt_rc_e rc;

  if (tabdrone->multiheader_sent == 0)
  {
	DT_C(tabdrone_output_header,(
      tabdrone, ipc));
    DT_C(dtos_time,(ctl,				/* get wall clock time at start 	*/
	  &tabdrone->start_time));
	tabdrone->multiheader_sent = 1;
  }

  DT_C(dtos_time,(ctl,					/* wall time at last frame sent		*/
    &tabdrone->last_time));

  emission[0] = '\0';					/* clear emit buffer 				*/
  EMIT("--");
  EMIT(BOUNDARY);
  EMIT("\r\n");

  if (suffix != NULL &&
	  !strcmp(suffix, ".au"))			/* filename suffix is .au? 			*/
  {
	mimetype = MIMETYPE_AU;				/* presume audio mime 				*/
	isvideo = 0;
  }

  EMIT("Content-type: ");				/* write server push sub header  	*/
  EMIT(mimetype);
  EMIT("\r\n");
  
  EMIT("Content-length: ");
  dtstr_printf(ctl, s, sizeof(s),
    "%ld", nbuffer);
  EMIT(s);
  EMIT("\r\n");

										/* ................................ */
  if (tabdrone->param->max > 0)			/* time limit imposed? 				*/
  {
	dtos_time_t now;
	DT_C(dtos_time,(ctl, &now));
    EMIT("Tableau-seconds-remaining: ");
    dtstr_printf(ctl, s, sizeof(s),
      "%ld", (long)(
      tabdrone->param->max - 
      (now.wall -
	   tabdrone->start_time.wall))); 
	EMIT(s);
	EMIT("\r\n");
  }

										/* ................................ */
  if (isvideo &&
	  tabdrone->param->max < 0)			/* frame limit imposed? 			*/
  {
    EMIT("Tableau-frames-remaining: ");
    dtstr_printf(ctl, s, sizeof(s),
      "%ld", (long)(
      -tabdrone->param->max)-
	  tabdrone->frames_sent);
	EMIT(s);
	EMIT("\r\n");
  }
										/* ................................ */
  if (tabdrone->delay !=				/* need to confirm delay?			*/
	  tabdrone->delay_old)
  {
    EMIT("Tableau-delay: ");
    dtstr_printf(ctl, s, sizeof(s),
      "%ld", (long)(
      tabdrone->delay * 100.0 /			/* convert milliseconds to percent 	*/
      (double)tabdrone->param->w));
	tabdrone->delay_old =
	  tabdrone->delay;
	EMIT(s);
	EMIT("\r\n");
  }

  EMIT("\r\n");							/* blank separator 					*/
  FLUSH;

  DT_C(tabdrone_output_flush,(tabdrone,	/* write data to push				*/
    ipc, buffer, nbuffer));

  EMIT("\r\n");
  EMIT("--");
  EMIT(BOUNDARY);
  EMIT("\r\n");
  FLUSH;

  if (isvideo)
	tabdrone->frames_sent++;

cleanup:
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
