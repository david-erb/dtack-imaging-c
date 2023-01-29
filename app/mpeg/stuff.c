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

#include <umdemo.h>
U_R("app/umdemo $RCSfile: stuff.c,v $ $Revision: 1.8 $");

#define BUFSIZE (4000)
#define PAUSE_SIZE (100000)
#define PAUSE_DELAY (2000)

#define COMMAND(CMD)					\
{										\
  umdemo_deccmd_t deccmd;				\
  memset(&deccmd, 0, sizeof(deccmd));	/* clear command structure 			*/\
  deccmd.cmd = (CMD);					/* prepare command					*/\
  U_C(dtfd_write_all,(ctl,				/* write command into fifo 			*/\
    &command_fifo, 						\
    (void **)&deccmd, sizeof(deccmd)));	\
  fflush(command_fifo.file);			\
}

/* ------------------------------------------------------------------------
 * requires input and variant
 * ignores connect
 * ------------------------------------------------------------------------ */

u_rc_e 
U_API
umdemo_op_stuff(
  dtparam_t *param)
{
  U_F("umdemo_op_stuff");
  u_ctl_t *ctl = &param->ctl;
  dtfd_t mpeg_file;
  dtfd_t data_fifo;
  dtfd_t command_fifo;
  dtfd_t reply_fifo;
  void *buf = NULL;
  long pause_size = 0;
  u_rc_e rc;

  mpeg_file.fd = -1;
  data_fifo.fd = -1;
  command_fifo.fd = -1;

  U_C(dtfd_open,(ctl, &mpeg_file,		/* open mpeg file for input 		*/
    param->input, "r"));

  U_C(dtfd_open,(ctl, &data_fifo,		/* open data fifo					*/
    param->output, "w"));

  U_C(dtfd_open,(ctl, &command_fifo,	/* open command fifo				*/
    param->connect, "w"));

  U_C(dtfd_open,(ctl, &reply_fifo,		/* open reply fifo					*/
    param->path, "r"));
  
  COMMAND(DECCMD_INITPARAM);			/* send init command 				*/
  
  U_C(dtos_malloc2,(ctl, &buf,			/* allocate buffer 					*/
    BUFSIZE, F, "buf"));

  while(1)
  {
	long got;
	U_C(dtfd_read,(ctl, &mpeg_file,		/* read data from mpeg file 		*/
      buf, BUFSIZE, &got));
	if (got == 0)
	  break;
	U_C(dtfd_write_all,(ctl,			/* write data into data fifo		*/
      &data_fifo, buf, got));


	pause_size += BUFSIZE;
	if (pause_size > PAUSE_SIZE)		/* time to pause? 					*/
	{
      COMMAND(DECCMD_PAUSE);			/* send pause command 				*/
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "pausing %0.1 seconds",
        (double)PAUSE_DELAY / 1000.0);
      U_C(dtos_delay,(ctl,				/* wait for a bit 					*/
        PAUSE_DELAY));
      COMMAND(DECCMD_PLAY);				/* send play command 				*/
	  pause_size = pause_size % 
        BUFSIZE;
	}
  }
  
cleanup:
  if (buf != NULL)
    U_I(dtos_free2,(ctl, buf,
      F, "buf"));
  if (command_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &command_fifo));
  if (data_fifo.fd != -1)
    U_I(dtfd_close,(ctl, &data_fifo));
  if (mpeg_file.fd != -1)
    U_I(dtfd_close,(ctl, &mpeg_file));

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
