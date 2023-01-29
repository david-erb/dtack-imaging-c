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
  CHANGES
  20010104 DE fix error checking for crlf on end of line (purify)
  19991207 DE added return argument for number of bytes read
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: line.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/liner.h>
#include <dtack/str.h>
#include <dtack/os.h>
  
#define CR 0x0d
#define LF 0x0a
static char nl[3] = {CR, LF, '\0'};

/*..........................................................................
| NAME
| dtdg_get_line() - receive single response line
|
| SUMMARY
| Wait for next response line from partner.
| If no response line is currently available, then block.
| Allocate space at ~*line~ and place the line there.
| Trim off a trailing CRLF.
| Rely on the caller to free this space.
|
| Return ~*line~ as NULL if the partner has terminated the conversation.
| 
| Have an error if the response line does not contain a CRLF.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_get_line(							/* receive single response line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_t *dg,							/* this dtdg object 				*/
  char **line)							/* returned line 					*/
{
  DT_F("dtdg_get_line");
  DT_Q(dtdg_get_line2,(ctl, dg, -1, 0, line, NULL));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdg_get_line2(							/* receive single response line		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdg_t *dg,							/* this dtdg object 				*/
  long timeout,
  long maxbytes,						/* max bytes to read total			*/
  char **line,							/* returned line 					*/
  long *got)							/* number of bytes read 			*/
{
  DT_F("dtdg_get_line2");
  int l;
  dt_rc_e rc;

  if (maxbytes == 0)
	maxbytes = DTDG_RESPONSE_MAXLEN;
  
  *line = NULL;
  DT_C(dtos_malloc2,(ctl,				/* get space for the line 			*/
    (void **)line,
    maxbytes,
    F, "line"));

  DT_C(dtipc_read_line2,(ctl,			/* read the request line 			*/
    dg->ipc, timeout, *line,
    maxbytes));

  l = strlen(*line);

  if (got)								/* caller wants this? 				*/
	*got = l;							/* number of bytes read 			*/

  if (l == 0)							/* partner has ended conversation? 	*/
  {
	DT_I(dtos_free2,(ctl, *line,
      F, "line"));
	*line = NULL;
	goto cleanup;
  }

  while (l > 0 &&
		 ((*line)[l-1] == LF ||
		  (*line)[l-1] == CR))
    (*line)[--l] = '\0';				/* nuke the crlf's 					*/

  {
	void *line2;
	DT_C(dtos_realloc3,(ctl, *line,		/* shrink space 					*/
      &line2, strlen(*line)+1,
      F, "line"));
	if (line2 != NULL)					/* realloc succeeded? 				*/
	  *line = (char *)line2;
  }

cleanup:
  if (rc != DT_RC_GOOD &&
	  *line != NULL)
  {
	DT_I(dtos_free2,(ctl, *line,
      F, "line"));
	*line = NULL;
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
