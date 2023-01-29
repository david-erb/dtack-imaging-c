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

#include <umplay.h>
U_R("app/umplay $RCSfile: umplay.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_init(
  dtparam_t *param,
  umplay_t **umplayp)
{
  U_F("umplay_setup");
  u_ctl_t *ctl = &param->ctl;
  umplay_t *umplay = NULL;
  dtfd_t fd;
  char *path = NULL;
  char *p;
  int n;
  u_rc_e rc;

  fd.fd = -1;

  U_C(dtos_malloc2,(ctl,				/* space for big umplay structure 	*/
    (void **)&umplay, sizeof(*umplay),
    F, "umplay"));
  *umplayp = umplay;

  memset(umplay, 0, sizeof(*umplay));	/* clear big umplay structure 		*/

  ctl->user = (void *)umplay;			/* start special message handling 	*/
  
  umplay->fd.fd = -1;					/* mark stuff uninitialized 		*/
  umplay->command_fifo.fd = -1;			
  umplay->data_fifo.fd = -1;			
  umplay->param = param;
  umplay->ctl = ctl;
										/* ................................ */
  if (param->path_flag & 
	  DTPARAM_FLAG_VALUE)
  {
	DT_C(dtos_strdup,(ctl, param->path,	/* place to parse fifo names		*/
      &path));
	DT_C(dtstr_tokenize,(ctl, path, 	/* names separated by "+" 			*/
      '+', &n));
	if (n != 3)							/* must be three fifo names 		*/
	{
	  rc = dt_err(ctl, F,
        "%s is not in the form"
        " datafifo,cmdfifo,repfifo",
        param->path);
	  goto cleanup;
	}
	p = path;
    DT_C(dtstr_printf,(ctl,				/* extract data fifo name 			*/
      umplay->data_fifo_name,
      sizeof(umplay->data_fifo_name),
      "%s", p));
	p += strlen(p) + 1;
	DT_C(dtstr_printf,(ctl,				/* extract command fifo name 		*/
      umplay->command_fifo_name,
      sizeof(umplay->command_fifo_name),
      "%s", p));
	p += strlen(p) + 1;
	DT_C(dtstr_printf,(ctl,				/* extract reply fifo name			*/
      umplay->reply_fifo_name,
      sizeof(umplay->reply_fifo_name),
      "%s", p));
  }
										/* ................................ */
  DT_C(dtlist_create,(ctl,
    &umplay->config_list, 
    "config list"));
  
  if (param->config != NULL &&			/* config file is given? 			*/
	  param->config[0] !='\0')
  {
	DT_C(dtstr_printf,(ctl,				/* extract data fifo name 			*/
      umplay->config_filename,
      sizeof(umplay->config_filename),
      "%s", param->config));

	DT_C(dtfd_open,(ctl, &fd,			/* open config file 				*/
      param->config, "r"));

	DT_C(dtliner_fd_list,(ctl, &fd,		/* make list from file 				*/
      UMPLAY_CONFIG_COMMENT, 
      UMPLAY_CONFIG_SEPARATOR, 
      &umplay->config_list));
  
	DT_C(dtfd_close,(ctl, &fd));		/* close the file 					*/
  }
										/* ................................ */
cleanup:
  if (path != NULL)
    DT_I(dtos_free2,(ctl, path,
      F, "path"));
  if (fd.fd != -1)
    DT_I(dtfd_close,(ctl, &fd));

  if (rc != U_RC_GOOD)
  {
    U_I(umplay_uninit,(param, umplay));	
	*umplayp = NULL;
  }

  return rc;
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umplay_uninit(
  dtparam_t *param,
  umplay_t *umplay)
{
  U_F("umplay_uninit");
  u_ctl_t *ctl = &param->ctl;
  u_rc_e rc = U_RC_GOOD;

  ctl->user = NULL;						/* cancel special message handling 	*/

  if (umplay != NULL)
  {
    if (UMPLAY_HAS_KEYBD(umplay))
      U_I(dtkeybd_uninit,(ctl, &umplay->keybd));

    if (UMPLAY_HAS_FD(umplay))
      U_I(dtfd_close,(ctl, &umplay->fd));

    if (UMPLAY_HAS_REPLY_FIFO(umplay))
      U_I(dtfd_close,(ctl, &umplay->reply_fifo));

    if (UMPLAY_HAS_COMMAND_FIFO(umplay))
      U_I(dtfd_close,(ctl, &umplay->command_fifo));

    if (UMPLAY_HAS_DATA_FIFO(umplay))
      U_I(dtfd_close,(ctl, &umplay->data_fifo));

	if (UMPLAY_HAS_DATA_IPC(umplay))	/* we have a data pipe open? 		*/
	{
	  U_I(dtipc_unfind,(ctl, 
        &umplay->data_ipc));
	}

	if (UMPLAY_HAS_COMMAND_IPC(umplay))	/* we have a command pipe open? 	*/
	{
	  U_I(dtipc_unregister,(ctl, 
        &umplay->command_ipc));
	}

	if (umplay->config_list.elem)		/* list was initialized? 			*/
	{
	  U_I(dtlist_free,(ctl,				/* free list elements 				*/
        &umplay->config_list));
	  U_I(dtlist_destroy,(ctl,			/* destroy list itself 				*/
        &umplay->config_list));
	}
    U_I(dtos_free2,(ctl, umplay,
      F, "umplay"));
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
