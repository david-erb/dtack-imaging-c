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
U_R("app/umplay $RCSfile: stuff.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND
#include <signal.h>
#endif

#define BUFSIZE (4000)

#define COMMAND(CMD, VALUE, SPEED)		\
{										\
  umplay_deccmd_t deccmd;				\
  memset(&deccmd, 0, sizeof(deccmd));	/* clear command structure 			*/\
  deccmd.cmd = DECCMD_ ## CMD;			/* prepare command					*/\
  deccmd.u.playSpeed = (SPEED);			/* always give play speed			*/\
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,	\
    "stuffing %s (%d) %s %s",			\
    #CMD, DECCMD_ ## CMD, 				\
    strlen(VALUE)? VALUE: "",			\
    strlen(VALUE)? #SPEED: "");			\
  U_C(dtfd_write_all,(ctl,				/* write command into fifo 			*/\
    &command_fifo, 						\
    (void **)&deccmd, sizeof(deccmd)));	\
  fflush(command_fifo.file);			\
}

static void sigpipe(int signum) { }

/* ------------------------------------------------------------------------
 * requires input and variant
 * ignores connect
 * ------------------------------------------------------------------------ */

u_rc_e
U_API
umplay_op_stuff(
  dtparam_t *param)
{
  U_F("umplay_op_stuff_transact");
  u_ctl_t *ctl = &param->ctl;
  umplay_t *umplay = NULL;
  dtfd_t mpeg_file;
  dtfd_t data_fifo;
  dtfd_t command_fifo;
  dtfd_t reply_fifo;
  void *buf = NULL;
  void (*sigpipe_previous)(int) = NULL;
  int init = 1;
  long limit = 0;
  long amount = 0;
  dtlist_elem_t *elem;
  u_rc_e rc;
  
  U_C(umplay_init,(param, &umplay));	/* init umplay structure			*/

  mpeg_file.fd = -1;
  data_fifo.fd = -1;
  command_fifo.fd = -1;
  reply_fifo.fd = -1;

  U_C(dtfd_open,(ctl, &mpeg_file,		/* open mpeg file for input 		*/
    param->input, "r"));
  
  {
	unsigned long decade;
    U_C(dtfd_read_all,(ctl,				/* check out nci file signature 	*/
      &mpeg_file,
      (void **)&decade, 
      sizeof(decade)));
	U_C(dtfd_seek,(ctl,					/* seek back to start of file 		*/
      &mpeg_file, 0,
      DTFD_WHENCE_SET, NULL));
	if (decade == 0xdecade00 ||			/* nci file signature found? 		*/
		decade == 0x00decade)
  	  U_C(dtfd_seek,(ctl,				/* seek test file signature			*/
        &mpeg_file, 400,
        DTFD_WHENCE_SET, NULL));
  }
  
  sigpipe_previous =					/* server exit may nuke our pipes 	*/
    signal(SIGPIPE, sigpipe);

  U_C(dtfd_open,(ctl, &data_fifo,		/* open data fifo					*/
    umplay->data_fifo_name, "w"));

  U_C(dtfd_open,(ctl, &command_fifo,	/* open command fifo				*/
    umplay->command_fifo_name, "w"));

  U_C(dtfd_open,(ctl, &reply_fifo,		/* open reply fifo					*/
    umplay->reply_fifo_name, "r"));
  
  U_C(dtos_malloc2,(ctl, &buf,			/* allocate buffer 					*/
    BUFSIZE, F, "buf"));

  elem = DTLIST_ELEM_HEAD;				/* start reading at head of list 	*/
  while(1)
  {
	int isnumber;
	long got;
	long written;

	if (amount >= limit)				/* byte limit reached?				*/
	{
	  DT_C(dtlist_next,(ctl,			/* get next command entry 			*/
        &umplay->config_list,
        elem, &elem));
	  if (elem == NULL)					/* command list exhausted? 			*/
	    break;
	  DT_C(dtstr_to_long2,(ctl,			/* expect data to be a number 		*/
        (char *)elem->user, &limit,
        &isnumber));
	  if (!isnumber)
	  {
		rc = dt_err(ctl, F,
          "invalid number \"%s\" in %s",
          elem->user, umplay->config_filename);
	    goto cleanup;
	  }

#     define IS_CMD(CMD) \
        (!strcmp(elem->name, CMD))

	  if (IS_CMD("forward") && init)
		COMMAND(INITPARAM, "speed", 0)	/* send normal init command			*/
	  else
	  if (IS_CMD("fforward") && init)
		COMMAND(INITPARAM, "speed", 2)	/* send fast init command			*/
	  else
	  if (IS_CMD("sforward") && init)
		COMMAND(INITPARAM, "speed", -2)	/* send slow init command			*/
	  else
	  if (IS_CMD("forward") && !init)
		COMMAND(PLAY, "speed", 0)		/* send play command 				*/
	  else
	  if (IS_CMD("fforward") && !init)
		COMMAND(PLAY, "speed", 2)		/* send fast forward command		*/
	  else
	  if (IS_CMD("sforward") && !init)
		COMMAND(PLAY, "speed", -2)		/* send slow motion command 		*/
	  else
	  if (IS_CMD("nop"))
		COMMAND(NOP, "", 0)				/* send nop command					*/
	  else
	  if (IS_CMD("pause"))
	  {
		COMMAND(PAUSE, "duration",		/* send pause command 				*/
          limit);
		U_C(dtos_delay,(ctl,			/* wait for a bit 					*/
          limit));
		continue;						/* force next command 				*/
	  }
	  else
	  if (IS_CMD("starve"))
	  {
		dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
          "starving duration %d",	
          limit);
		U_C(dtos_delay,(ctl,			/* wait for a bit 					*/
          limit));
		continue;						/* force next command 				*/
	  }
	  else
	  if (IS_CMD("stop"))
		break;
	  else
	  if (IS_CMD("exit"))
	  {
		COMMAND(EXIT, "", 0);			/* send exit command 				*/
	  }
	  else
	  {
		rc = dt_err(ctl, F,
          "invalid command \"%s\" in %s",
          elem->name, 
          umplay->config_filename);
	    goto cleanup;
	  }
	  init = 0;
	  amount = 0;
	}

	U_C(dtfd_read,(ctl, &mpeg_file,		/* read data from mpeg file 		*/
      buf, BUFSIZE, &got));
	if (got == 0)						/* end of input file reached? 		*/
	  break;
	U_C(dtfd_write,(ctl,				/* write data into data fifo		*/
      &data_fifo, buf, got, &written));
	amount += written;
	if (written != got)					/* go until fifo dries up 			*/
	  break;
  }

cleanup:
  if (buf != NULL)
    U_I(dtos_free2,(ctl, buf,
      F, "buf"));
  if (reply_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &reply_fifo));
  if (command_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &command_fifo));
  if (data_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &data_fifo));
  if (mpeg_file.fd != -1)
    U_I(dtfd_close,(ctl, &mpeg_file));

  if (sigpipe_previous != NULL)
    signal(SIGPIPE, sigpipe_previous);

  if (umplay != NULL)
    U_I(umplay_uninit,(param, umplay));
  
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
