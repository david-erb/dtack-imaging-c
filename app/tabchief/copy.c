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
DT_RCSID("app/tabchief $RCSfile: copy.c,v $ $Revision: 1.8 $");

#define BSIZE (64000)

/* ------------------------------------------------------------------------
 * server copyto means write to local disk
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_copyto(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_copyto");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  void *b = NULL;
  long expect;
  long total = 0;
  long got;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtfd_open,(ctl, fd,
    arg, "w"));

  DT_C(dtos_malloc2,(ctl, &b,
    BSIZE, F, "copy buffer"));

  DT_C(dtipc_read_some,(ctl, ipc, 		/* total comes first 				*/
    b, 10, &got));
  if (got != 10)
  {
	rc = dt_err(ctl, F,
      "got %ld but needed 10 for total",
      &got);
	goto cleanup;
  }

  ((unsigned char *)b)[10] = '\0';
  DT_C(dtstr_to_long,(ctl, b,			/* convert to number 				*/
    &expect));

  total = 0;
  while (total < expect)
  {
	long got;
	DT_C(dtipc_read_some,(ctl, ipc,		/* read from socket 				*/
      b, DT_MIN(BSIZE, expect-total),
      &got));
	if (got == 0)
	  break;
	DT_C(dtfd_write_all,(ctl, fd,		/* write to local file 				*/
      b, got));
	total += got;
  }

  if (total != expect)
  {
    rc = dt_err(ctl, F,					/* reply with confirmation 			*/
      "expected %ld and got %ld",
      expect, total);
	goto cleanup;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
    "copied %ld to \"%s\"",
    total, arg);

cleanup:
  {
    tabchief_t *tabchief = 
      (tabchief_t *)ctl->user;
	if (tabchief != NULL &&
		!tabchief->response[0])			/* no problems yet? 				*/
      DT_I(dtstr_printf,(ctl,
        tabchief->response,
        sizeof(tabchief->response),
        "got all %ld bytes", total));
  }

  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "copy buffer"));
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/* ------------------------------------------------------------------------
 * client copyto means read from local disk
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_client_copyto(
  dtparam_t *param,
  dtipc_t *ipc)
{
  DT_F("tabchief_client_copyto");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  void *b = NULL;
  long total;
  long got;
  dt_rc_e rc;

  fd->fd = -1;

  if (!(param->input_flag &	
	   DTPARAM_FLAG_VALUE))
  {
	rc = dt_err(ctl, F,
      "need -input");
	goto cleanup;
  }

  if (!(param->output_flag &	
	   DTPARAM_FLAG_VALUE))
  {
	rc = dt_err(ctl, F,
      "need -output");
	goto cleanup;
  }

  DT_C(dtfd_open,(ctl, fd,				/* open local file for reading 		*/
    param->input, "r"));
  DT_C(dtfd_seek,(ctl, fd, 0,			/* get file size 					*/
    DTFD_WHENCE_END, &total));
  DT_C(dtfd_seek,(ctl, fd, 0,			/* return to start of file 			*/
    DTFD_WHENCE_SET, NULL));

  DT_C(dtos_malloc2,(ctl, &b,			/* space for buffer 				*/
    BSIZE, F, "copy buffer"));

  DT_C(dtstr_printf,(ctl, b, BSIZE,		/* format http header 				*/
    "GET /copyto?%s HTTP/1.0\n0x%08lx",
    param->output, total));

  DT_C(dtipc_write_all,(ctl, ipc,		/* http header first 				*/
    b, strlen((char *)b)));

  while(1)
  {
	long got;
	DT_C(dtfd_read,(ctl, fd,			/* read from local file				*/
      b, BSIZE, &got));
	if (got == 0)
	  break;
	DT_C(dtipc_write_all,(ctl, ipc,		/* write to socket					*/
      b, got));
	total += got;
  }

  DT_C(dtipc_read_some,(ctl, ipc,		/* read confirmation from socket 	*/
    b, BSIZE-1, &got));
  ((char *)b)[got] = '\0';

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,		/* print confirmation				*/
    "server response: %s", (char *)b);

cleanup:
  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "copy buffer"));
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/* ------------------------------------------------------------------------
 * server copyfrom means write to local disk
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_server_copyfrom(
  dtparam_t *param,
  dtipc_t *ipc,
  const char *command,
  const char *arg)
{
  DT_F("tabchief_server_copyfrom");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  void *b = NULL;
  long total = 0;
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtfd_open,(ctl, fd,				/* open local file for reading 		*/
    arg, "r"));
  DT_C(dtfd_seek,(ctl, fd, 0,			/* get file size 					*/
    DTFD_WHENCE_END, &total));
  DT_C(dtfd_seek,(ctl, fd, 0,			/* return to start of file 			*/
    DTFD_WHENCE_SET, NULL));

  DT_C(dtos_malloc2,(ctl, &b,			/* space for buffer 				*/
    BSIZE, F, "copy buffer"));

  DT_C(dtstr_printf,(ctl, b, BSIZE,		/* format size						*/
    "0x%08lx", total));

  DT_C(dtipc_write_all,(ctl, ipc,		/* write file size first			*/
    b, strlen(b)));

  total = 0;
  while(1)
  {
	long got;
	DT_C(dtfd_read,(ctl, fd,			/* read from local file				*/
      b, BSIZE, &got));
	if (got == 0)
	  break;
	DT_C(dtipc_write_all,(ctl, ipc,		/* write to socket					*/
      b, got));
	total += got;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
    "copied %ld from \"%s\"",
    total, arg);

cleanup:
  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "copy buffer"));
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

  return rc;
}

/* ------------------------------------------------------------------------
 * client copyfrom means read from local disk
 * ------------------------------------------------------------------------ */

dt_rc_e
DTCONFIG_API1
tabchief_client_copyfrom(
  dtparam_t *param,
  dtipc_t *ipc)
{
  DT_F("tabchief_client_copyfrom");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  void *b = NULL;
  long total;
  long expect;
  long got;
  dt_rc_e rc;

  fd->fd = -1;

  if (!(param->input_flag &	
	   DTPARAM_FLAG_VALUE))
  {
	rc = dt_err(ctl, F,
      "need -input");
	goto cleanup;
  }

  if (!(param->output_flag &	
	   DTPARAM_FLAG_VALUE))
  {
	rc = dt_err(ctl, F,
      "need -output");
	goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl, &b,			/* space for buffer 				*/
    BSIZE, F, "copy buffer"));

  DT_C(dtstr_printf,(ctl, b, BSIZE,		/* format http header 				*/
    "GET /copyfrom?%s HTTP/1.0\n",
    param->input));

  DT_C(dtipc_write_all,(ctl, ipc,			/* write http header first 			*/
    b, strlen((char *)b)));

  DT_C(dtipc_read_some,(ctl, ipc, b, 10,		/* total comes back first 			*/
    &got));
  if (got != 10)
  {
	rc = dt_err(ctl, F,
      "got %ld but needed 10 for total",
      got);
	goto cleanup;
  }

  ((unsigned char *)b)[10] = '\0';
  DT_C(dtstr_to_long,(ctl, b,			/* convert to number 				*/
    &expect));

  DT_C(dtfd_open,(ctl, fd,				/* open local file for writing 		*/
    param->output, "w"));

  total = 0;
  while (total < expect)
  {
	DT_C(dtipc_read_some,(ctl, ipc,		/* read from socket 				*/
      b, DT_MIN(BSIZE, expect-total),
      &got));
	if (got == 0)
	  break;
	DT_C(dtfd_write_all,(ctl, fd,		/* write to local file 				*/
      b, got));
	total += got;
  }

  if (total != expect)
  {
    rc = dt_err(ctl, F,				
      "expected %ld and got %ld",
      expect, total);
	goto cleanup;
  }

cleanup:
  if (b != NULL)
    DT_I(dtos_free2,(ctl, b,
      F, "copy buffer"));
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));

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
