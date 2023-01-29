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
DT_RCSID("app/tableau $RCSfile: put_file.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>

#ifdef DTCONFIG_IMPLIED_MSW32
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <io.h>
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_file_setup(
  dtparam_t *param,
  tableau_put_t *put,
  const char *spec)
{
  DT_F("tableau_put_file");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t *fd = &put->u.file.fd;			/* for short 						*/
  dt_rc_e rc;

  fd->fd = -1;

  DT_C(dtstr_printf,(ctl,				/* prepend the the path 			*/
    put->u.file.name,
    sizeof(put->u.file.name),
    "%s/%s", param->path, spec));
  
# ifdef DTCONFIG_IMPLIED_MSW32
  {
	int sysrc;
	int e;
	sysrc = sopen(						/* open shared file 				*/
      put->u.file.name,
      O_WRONLY|O_CREAT|O_BINARY,
      SH_DENYNO,
      S_IREAD|S_IWRITE);
	e = errno;

	if (sysrc == -1)					/* file failed to open? 			*/
	  return dt_err(ctl, F,
        "sopen(%s, "
        "O_WRONLY|O_APPEND|O_BINARY, "
        "SH_DENYNO, "
        "S_IREAD|S_IWRITE)"
        DTERRNO_FMT,
        put->u.file.name,
        DTERRNO_ARG(e));

	DT_C(dtfd_fd,(ctl, fd,				/* wrap fd around shared file 		*/
      put->u.file.name, sysrc));
  }
# else
    DT_C(dtfd_open,(ctl, fd,			/* open the file 					*/
      put->u.file.name, "a"));
# endif

cleanup:
  if (rc != DT_RC_GOOD &&
	  fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));	
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_put_file(
  dtparam_t *param,
  tableau_put_t *put,
  dtimage_t *image,						/* full image 						*/
  tableau_header_t *header,				/* compressed header 				*/
  void *data,							/* compressed data 					*/
  int cc)								/* color component 					*/
{
  DT_F("tableau_put_file");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t *fd = &put->u.file.fd;			/* for short 						*/
  dt_rc_e rc = DT_RC_GOOD;

  if (data == NULL)						/* supposed to clean up? 			*/
  {
	if (fd->fd != -1 &&
		fd->fd != 0)
      DT_I(dtfd_close,(ctl, fd));		/* close file 						*/
	return rc;
  }

  DT_C(dtfd_seek,(ctl, fd,				/* go back to start of file			*/
    0, DTFD_WHENCE_SET, NULL));

  DT_C(dtfd_write_all,(ctl, fd,			/* write compression header to file	*/
      header, sizeof(*header)));

  DT_C(dtfd_write_all,(ctl, fd,			/* write data to file				*/
    data, header->l));

  DT_C(dtfd_write_all,(ctl, fd,			/* flush the file					*/
    NULL, 0));

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
