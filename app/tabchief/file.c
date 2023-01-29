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

#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: file.c,v $ $Revision: 1.8 $");

#define BSIZE (1024)
#define TSIZE (1600)					/* tail size in bytes 				*/


/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_catfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *filename,
  unsigned long flags)
{
  DT_F("tabchief_server_catfile");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  unsigned char *b = NULL;
  long total = 0;
  int first = 1;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtfd_open,(ctl, fd,				/* open local file for reading 		*/
    filename, "r"));
  DT_C(dtfd_seek,(ctl, fd, 0,			/* get file size 					*/
    DTFD_WHENCE_END, &total));
  
  DT_C(dtfd_seek,(ctl, fd, 0,			/* return to start of file 			*/
    DTFD_WHENCE_SET, NULL));

  if (flags & TABCHIEF_FLAG_TAIL &&		/* want end part?					*/
	  total > TSIZE)					/* and file is big enough? 			*/
	DT_C(dtfd_seek,(ctl, fd, -TSIZE,	/* seek to near the end				*/
      DTFD_WHENCE_END, NULL));
  
  if (flags & TABCHIEF_FLAG_SIZE)		/* want the size displayed?			*/
    DT_C(tabchief_reply,(param, ipc,	/* tell the size 					*/
      "file \"%s\" size is %ld bytes",
      filename, total));
	   
  if (flags & TABCHIEF_FLAG_PRE)	
    DT_C(tabchief_reply,(param, ipc,
      "<listing>"));

  DT_C(dtos_malloc2,(ctl, (void **)&b,	/* space for buffer 				*/
    BSIZE, F, "copy buffer"));

  while(1)
  {
	long got, i;
	DT_C(dtfd_read,(ctl, fd,			/* read from local file				*/
      b, BSIZE, &got));
	if (got == 0)
	  break;

	if (first &&
		flags & TABCHIEF_FLAG_TAIL &&	/* want end part					*/
		total > TSIZE)
	{
	  for (i=1; i<got; i++)
	    if (b[i-1] == '\n')
	      break;
	  first = 0;
	}
	else
	  i = 0;
  
	if (i == got)
	  i = 0;

	DT_C(dtipc_write_all,(ctl, ipc,		/* write to socket					*/
      b+i, got-i));
  }

cleanup:
  if (flags & TABCHIEF_FLAG_PRE)	
    DT_C(tabchief_reply,(param, ipc,
      "</listing>"));

  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "copy buffer"));
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_showfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_showfile");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(tabchief_server_catfile,(param,
    ipc, arg, TABCHIEF_FLAGS_SHOW));
  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_tailfile(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_tailfile");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(tabchief_server_catfile,(param,
    ipc, arg, TABCHIEF_FLAGS_TAIL));
  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_html(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_html");
  dt_ctl_t *ctl = &param->ctl;

  tabchief_reply(param, ipc, "HTTP/1.0 200 OK");
  tabchief_reply(param, ipc, "Server: Tabchief by Tableau Web");
  tabchief_reply(param, ipc, "Content-type: text/html");
  tabchief_reply(param, ipc, "");

  DT_Q(tabchief_server_catfile,(param,
    ipc, arg, TABCHIEF_FLAGS_RAW));

  return DT_RC_GOOD;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_gif(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_gif");
  dt_ctl_t *ctl = &param->ctl;

  tabchief_reply(param, ipc, "HTTP/1.0 200 OK");
  tabchief_reply(param, ipc, "Server: Tabchief by Tableau Web");
  tabchief_reply(param, ipc, "Content-type: image/%s", command);
  tabchief_reply(param, ipc, "");

  DT_Q(tabchief_server_catfile,(param,
    ipc, arg, TABCHIEF_FLAGS_RAW));

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
