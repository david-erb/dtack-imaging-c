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
DT_RCSID("app/tableau $RCSfile: get_file.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>
#include <dtack/msw.h>

#ifdef DTCONFIG_IMPLIED_MSW32
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <io.h>
#endif

#define CR (0x0d)
#define LF (0x0a)

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tableau_get_file_grab(
  dtparam_t *param,
  tableau_get_t *get,
  char *filename,
  int max)
{
  DT_F("tableau_get_file_grab");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_initialized = 0;
  int l;
  char *p;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, ipc,			/* contact tabgrab server 			*/
    get->u.file.name, 0, 0, 1000));
  ipc_initialized = 1;

  DT_C(dtstr_printf,(ctl,				/* prepare request for tabgrab 		*/
    filename, max,
    "grab,%ld%c%c", 
    get->u.file.frame, CR, LF));

  DT_C(dtipc_write_all,(ctl, ipc,		/* give request to tabgrab 			*/
    filename, strlen(filename)));

  DT_C(dtipc_read_line,(ctl, ipc,		/* get response from tabgrab 		*/
    filename, max));

	l = strlen(filename);
	while (l > 0 &&						/* nuke the crlf's 					*/
		   filename[l-1] == LF ||
		   filename[l-1] == CR)
	  filename[--l] = '\0';

	get->u.file.frame++;
	p = strchr(filename, ',');			/* see if there is a comma			*/
	if (p != NULL)						/* there is a comma? 				*/
	{
	  long v;
	  *p++ = '\0';
	  DT_G(dtstr_to_long,(ctl, p, &v));	/* see if a number follows			*/
      get->u.file.frame = v;
	}
  
cleanup:
  if (ipc_initialized)
	DT_I(dtipc_unfind,(ctl, ipc));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_file_shm(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_file_shm");
  dt_ctl_t *ctl = &param->ctl;
#ifdef DTCONFIG_IMPLIED_MSW32
  char t[256];
  HANDLE mapping = NULL;
  HANDLE semaphore = NULL;
  void *b = NULL;
  int have_semaphore = 0;
  DWORD e;
  DWORD result;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(tableau_get_file_grab,(param,	/* talk to tabgrab 					*/
    get, t, sizeof(t)));

  if (*header == NULL)					/* don't have an old header? 		*/
  DT_C(dtos_malloc2,(ctl,				/* allocate space for header 		*/
    (void **)header,
    sizeof(tableau_header_t),
    F, "header"));
  
  mapping = OpenFileMapping(
    FILE_MAP_READ, FALSE, t);
  if (mapping == NULL)			 		/* could not open mapped file?		*/
  {
	char s[256];
	e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
    rc = dt_err(ctl, F,
      "OpenFileMapping("
      "FILE_MAP_READ, FALSE, "
      "\"%s\")"
      DTERRNO_LASTFS,
      t,
      DTERRNO_LASTES(e, s));
    goto cleanup;
  }

  b = MapViewOfFile(					/* get pointer to mapped file		*/
    mapping,
    FILE_MAP_READ,
    0, 0, 0);
  if (b == NULL)						/* could not get mapped pointer?	*/
  {
	char s[256];
	e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
    rc = dt_err(ctl, F,
      "MapViewOfFile("
      "0x%08lx, "
      "FILE_MAP_READ, "
      "0, 0, 0)"
      DTERRNO_LASTFS,
      mapping, 
      DTERRNO_LASTES(e, s));
    goto cleanup;
  }

  semaphore = OpenSemaphore(			/* access the semaphore				*/
    SEMAPHORE_ALL_ACCESS, FALSE,
    t+1);
  if (semaphore == NULL)				/* could not get semaphore?			*/
  {
	char s[256];
	e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
    rc = dt_err(ctl, F,
      "OpenSemaphore("
      "SEMAPHORE_ALL_ACCESS, FALSE, "
      "\"%s\")"
      DTERRNO_LASTFS,
      t+1,
      DTERRNO_LASTES(e, s));
    goto cleanup;
  }

  result = WaitForSingleObject(			/* wait for OK to copy				*/
    semaphore, 2000);
  if (result == WAIT_TIMEOUT)
  {
    rc = dt_err(ctl, F,
      "timeout waiting for semaphore");
    goto cleanup;
  }
  have_semaphore = 1;
  
  memcpy(*header, b,					/* copy header from shared memory	*/
    sizeof(**header));

  if (*l < (*header)->l)				/* given buffer is too small? 		*/
  {
	if (*l != 0)						/* we even have a given buffer? 	*/
	  DT_Q(dtos_free2,(ctl, *data,
        F, "file data"));
	*l = 0;
    DT_Q(dtos_malloc2,(ctl, data,		/* space for the data 				*/
      DT_CAST(unsigned int, 
        (*header)->l),
      F, "file data"));
	*l = (*header)->l;					/* remember how big it is 			*/
  }

  memcpy(*data, 						/* copy contents of buffer			*/
    (char *)b+sizeof(tableau_header_t), 
    *l);

cleanup:
  if (have_semaphore)
    ReleaseSemaphore(					/* signal copy is done				*/
      semaphore, 1, NULL);
  if (semaphore != NULL)
    CloseHandle(semaphore);
  if (b != NULL)
    UnmapViewOfFile(b);
  if (mapping != NULL)
    CloseHandle(mapping);
  return rc;
#else
  return dt_err_notlinked(ctl, F, 
    "shm support");
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_file_normal(
  dtparam_t *param,
  tableau_get_t *get,
  tableau_header_t **header,
  void **data,
  long *l)
{
  DT_F("tableau_get_file_normal");
  dt_ctl_t *ctl = &param->ctl;
  char t[256];
  dtfd_t *fd = &get->u.file.fd;			/* for short 						*/
  dt_rc_e rc;

  DT_C(tableau_get_file_grab,(param,	/* talk to tabgrab 					*/
    get, t, sizeof(t)));

  if (*header == NULL)					/* don't have an old header? 		*/
  DT_C(dtos_malloc2,(ctl,				/* allocate space for header 		*/
    (void **)header,
    sizeof(tableau_header_t),
    F, "header"));
  
  DT_C(dtfd_seek,(ctl, fd,				/* go back to start of file			*/
    0, DTFD_WHENCE_SET, NULL));

  DT_C(dtfd_read_all,(ctl, fd,			/* read header from file			*/
    *header,
    sizeof(tableau_header_t)));

  if (*l < (*header)->l)				/* given buffer is too small? 		*/
  {
	if (*l != 0)						/* we even have a given buffer? 	*/
	  DT_Q(dtos_free2,(ctl, *data,
        F, "file data"));
	*l = 0;
    DT_Q(dtos_malloc2,(ctl, data,		/* space for the data 				*/
      DT_CAST(unsigned int, 
        (*header)->l),
      F, "file data"));
	*l = (*header)->l;					/* remember how big it is 			*/
  }

  DT_C(dtfd_read_all,(ctl, fd,			/* read encoded data from file		*/
    *data, (*header)->l));
  
cleanup:

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_file_free(
  dtparam_t *param,
  tableau_get_t *get)
{
  DT_F("tableau_get_file_free");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t *fd = &get->u.file.fd;			/* for short 						*/
  dt_rc_e rc = DT_RC_GOOD;

  if (fd->fd != -1 &&
	  fd->fd != 0)
    DT_I(dtfd_close,(ctl, fd));			/* close file 						*/

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_get_file_setup(
  dtparam_t *param,
  tableau_get_t *get,
  const char *spec)
{
  DT_F("tableau_get_file_setup");
  dt_ctl_t *ctl = &param->ctl;
  char t[256];
  dtfd_t *fd = &get->u.file.fd;			/* for short 						*/
  dt_rc_e rc;

  fd->fd = -1;
  DT_C(dtstr_printf,(ctl,				/* copy ipc spec to structure 		*/
    get->u.file.name,
    sizeof(get->u.file.name),
    "%s", spec));

  DT_C(tableau_get_file_grab,(param,	/* talk to tabgrab 					*/
    get, t, sizeof(t)));				/* tabgrab gives us a filename 		*/

  if (t[0] == '*')						/* this is a shared memory file? 	*/
  {
	get->function = tableau_get_file_shm;
  }
  else
  {
#   ifdef DTCONFIG_IMPLIED_MSW32
    {
	  int sysrc;
	  int e;
	  sysrc = sopen(					/* open shared file 				*/
        t,
        O_RDONLY|O_BINARY,
        SH_DENYNO,
        S_IREAD|S_IWRITE);
	  e = errno;

	  if (sysrc == -1)					/* 	file failed to open? 			*/
	    return dt_err(ctl, F,
          "sopen(%s, "
          "O_RDONLY|O_BINARY, "
          "SH_DENYNO, "
          "S_IREAD|S_IWRITE)"
          DTERRNO_FMT,
          t,
          DTERRNO_ARG(e));

	  DT_C(dtfd_fd,(ctl, fd, t,			/* wrap fd around shared file 		*/
        sysrc));
	}
#   else
    DT_C(dtfd_open,(ctl, fd,			/* open the file 					*/
      t, "r"));
#   endif
	get->function = tableau_get_file_normal;
  }

  get->free = tableau_get_file_free;
  
cleanup:
  if (rc != DT_RC_GOOD &&
	  fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));			/* close file 						*/

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
