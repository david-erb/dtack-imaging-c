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
DT_RCSID("app/tabdrone $RCSfile: grab.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>
#include <dtack/errno.h>

#define CR (0x0d)
#define LF (0x0a)

#ifdef DTCONFIG_IMPLIED_MSW32
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <io.h>
#endif

#define CR (0x0d)
#define LF (0x0a)


typedef long tableau_len_t;
#define TABLEAU_HEADER_CAMNAME_MAX (16)
typedef struct {
  tableau_len_t l;						/* length of encoded data 			*/
  tableau_len_t n;						/* frame sequence number 			*/
  tableau_len_t ms;						/* millisecond at acquire 			*/
  tableau_len_t xe;						/* size of decoded image 			*/
  tableau_len_t ye;
  tableau_len_t org;
  char cam[TABLEAU_HEADER_CAMNAME_MAX];
  char codec[16];
} tableau_header_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabdrone_grab_grab(
  dtparam_t *param,
  char *filename,
  int max)
{
  DT_F("tabdrone_grab_grab");
  dt_ctl_t *ctl = &param->ctl;
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_initialized = 0;
  int l;
  char *p;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, ipc,			/* contact tabgrab server 			*/
    param->input, 0, 0, 1000));
  ipc_initialized = 1;

  DT_C(dtstr_printf,(ctl,				/* prepare request for tabgrab 		*/
    filename, max,
    "grab,%ld%c%c", 
    0x7fffffff, CR, LF));

  DT_C(dtipc_write_all,(ctl, ipc,		/* give request to tabgrab 			*/
    filename, strlen(filename)));

  DT_C(dtipc_read_line,(ctl, ipc,		/* get response from tabgrab 		*/
    filename, max));

	l = strlen(filename);
	while (l > 0 &&						/* nuke the crlf's 					*/
		   filename[l-1] == LF ||
		   filename[l-1] == CR)
	  filename[--l] = '\0';

	p = strchr(filename, ',');			/* see if there is a comma			*/
	if (p != NULL)						/* there is a comma? 				*/
	  *p++ = '\0';
  
cleanup:
  if (ipc_initialized)
	DT_I(dtipc_unfind,(ctl, ipc));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabdrone_grab_allocate(
  dtparam_t *param,
  const tableau_header_t *header,
  void **data,
  long *ndata)
{
  DT_F("tabdrone_grab_allocate");
  dt_ctl_t *ctl = &param->ctl;

  if (*data == NULL ||					/* data not allocated yet? 			*/
	  *ndata < header->l)				/* given buffer is too small? 		*/
  {
	if (*data != NULL)					/* we even have a given buffer? 	*/
	  DT_Q(dtos_free2,(ctl, *data,
        F, "data"));
    DT_Q(dtos_malloc2,(ctl, data,		/* space for the data 				*/
      header->l,
      F, "data"));
	*ndata = header->l;					/* remember how big it is 			*/
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabdrone_grab_shm(
  dtparam_t *param,
  const char *filename,
  void **data,
  long *ndata,
  long *xe,
  long *ye)
{
  DT_F("tabdrone_grab_shm");
  dt_ctl_t *ctl = &param->ctl;
#ifdef DTCONFIG_IMPLIED_MSW32
  HANDLE mapping = NULL;
  HANDLE semaphore = NULL;
  tableau_header_t header;
  void *b = NULL;
  int have_semaphore = 0;
  DWORD e;
  DWORD result;
  dt_rc_e rc = DT_RC_GOOD;

  mapping = OpenFileMapping(
    FILE_MAP_READ, FALSE, filename);
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
       filename,
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
    filename+1);
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
      filename+1,
      DTERRNO_LASTES(e, s));
    goto cleanup;
  }

  result = WaitForSingleObject(			/* wait for OK to copy				*/
    semaphore, 2000);
  if (result == WAIT_TIMEOUT)
  {
    rc = dt_err(ctl, F,
      "timeout waiting for tabgrab's"
      " semaphore");
    goto cleanup;
  }
  have_semaphore = 1;
  
  memcpy(&header, b,					/* copy header from shared memory	*/
    sizeof(header));

  DT_C(tabdrone_grab_allocate,(param,	/* get space for image buffer 		*/
    &header, data, ndata));

  memcpy(*data, 						/* copy contents of buffer			*/
    (char *)b+sizeof(header), 
    header.l);

  *xe = header.xe;
  *ye = header.ye;
  
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
    "shm");
#endif
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabdrone_grab_file(
  dtparam_t *param,
  const char *filename,
  void **data,
  long *ndata,
  long *xe,
  long *ye)
{
  DT_F("tabdrone_grab_file");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  tableau_header_t header;
  dt_rc_e rc;

  fd->fd = -1;

#   ifdef DTCONFIG_IMPLIED_MSW32
    {
	  int sysrc;
	  int e;
	  sysrc = sopen(					/* open shared file 				*/
        filename,
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
           filename,
          DTERRNO_ARG(e));

	  DT_C(dtfd_fd,(ctl, fd, filename,	/* wrap fd around shared file 		*/
        sysrc));
	}
#   else
	{
      DT_C(dtfd_open,(ctl, fd,			/* open the file 					*/
        filename, "r"));
	}
#   endif

  DT_C(dtfd_read_all,(ctl, fd,			/* read header from file			*/
    &header,
    sizeof(tableau_header_t)));

  DT_C(tabdrone_grab_allocate,(param,	/* get space for image buffer 		*/
    &header, data, ndata));

  DT_C(dtfd_read_all,(ctl, fd,			/* read encoded data from file		*/
    *data, header.l));

  *xe = header.xe;
  *ye = header.ye;
  
cleanup:
  if (fd->fd != -1)
	DT_C(dtfd_close,(ctl, fd));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabdrone_grab(
  dtparam_t *param,
  void **data,
  long *ndata,
  long *xe,
  long *ye)
{
  DT_F("tabdrone_grab");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;
  char t[256];

  DT_C(tabdrone_grab_grab,(param,		/* talk to tabgrab 					*/
    t, sizeof(t)));						/* tabgrab gives us a filename 		*/

  if (t[0] == '*')						/* this is a shared memory file? 	*/
  {
	DT_C(tabdrone_grab_shm,(param,		/* get frame from shared memory 	*/
      t, data, ndata, xe, ye));
  }
  else
  {
	DT_C(tabdrone_grab_file,(param,		/* get frame from shared file 		*/
      t, data, ndata, xe, ye));
  }
  
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
