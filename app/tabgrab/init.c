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

#include <tabgrab.h>
DT_RCSID("app/tabgrab $RCSfile: init.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>
#include <dtack/msw.h>

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
tabgrab_uninit(
  tabgrab_t *tabgrab)
{
  DT_F("tabgrab_uninit");
  dt_ctl_t *ctl = tabgrab->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (TABGRAB_IS_SHM(tabgrab))			/* we are using memory mapped file? */
  {
# ifdef DTCONFIG_IMPLIED_MSW32			/* on Win32? 						*/
	if (tabgrab->lock != NULL)
	  CloseHandle(
        (HANDLE)tabgrab->lock);
	tabgrab->lock = 0;

	if (tabgrab->b != NULL)				/* buffer got mapped? 				*/
	  UnmapViewOfFile(tabgrab->b);
	tabgrab->b = NULL;

	if (tabgrab->handle != 0)			/* file mapping created? 			*/
	  CloseHandle(
        (HANDLE)tabgrab->handle);
	tabgrab->handle = 0;
# endif
  }
  else
  {
	if (tabgrab->b != NULL)				/* buffer got allocated? 			*/
	  DT_I(dtos_free2,(ctl,
        tabgrab->b, F, "buffer"));
  }

  if (tabgrab->fd.fd != -1)
    DT_I(dtfd_close,(ctl, &tabgrab->fd));

  if (tabgrab->fb_initialized)
    DT_I(dtfb_uninit,(ctl, &tabgrab->fb));

  memset(tabgrab, 0, sizeof(*tabgrab));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabgrab_init(
  tabgrab_t *tabgrab,
  dtparam_t *param)
{
  DT_F("tabgrab_init");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  memset(tabgrab, 0, sizeof(*tabgrab));
  tabgrab->ctl = ctl;
  tabgrab->param = param;
  tabgrab->fd.fd = -1;

										/* ................................ */
  DT_C(dtfb_init,(ctl, &tabgrab->fb, 
    param->input));

  tabgrab->l = tabgrab->fb.rowsize *
    tabgrab->fb.ye;
  tabgrab->fb_initialized = 1;

  DT_C(dtstr_printf,(ctl,				/* remember the filename			*/
    tabgrab->filename,
    sizeof(tabgrab->filename),
    "%s", param->output));

  if (TABGRAB_IS_SHM(tabgrab))			/* we are using memory mapped file? */
  {
# ifdef DTCONFIG_IMPLIED_MSW32
	HANDLE handle;
	long size;
	LPVOID b;
	HANDLE lock;
	DWORD e;
	size = sizeof(tableau_header_t) +	/* big enough for header also		*/
      tabgrab->l;
	handle = CreateFileMapping(
      (HANDLE)0xffffffff,				/* in-memory file 					*/
      NULL,
      PAGE_READWRITE,
      0, size,	
      TEXT((tabgrab->filename)));		/* name of shared file 				*/
	e = GetLastError();
	if (handle == NULL ||				/* file mapping creation failed? 	*/
		e == ERROR_ALREADY_EXISTS)
	{
	  rc = dt_err(ctl, F,
        "CreateFileMapping("
        "(HANDLE)0xffffffff, "
        "NULL, "
        "PAGE_READWRITE, "
        "0, %ld, \"%s\""
        DTERRNO_LASTF,
        size,
        tabgrab->filename,
        DTERRNO_LASTE(e));
	  goto cleanup;
	}

	b = MapViewOfFile(					/* get pointer to mapped file 		*/
      handle,
      FILE_MAP_READ | FILE_MAP_WRITE,
      0, 0, 0);
	e = GetLastError();
	if (b == NULL)						/* could not get mapped pointer? 	*/
	{
	  rc = dt_err(ctl, F,
 	    "MapViewOfFile("
        "0x%08lx, "
        "FILE_MAP_READ | FILE_MAP_WRITE, "
        "0, 0, 0)"
        DTERRNO_LASTF,
        handle, 
        DTERRNO_LASTE(e));
	  goto cleanup;
	}
	
	lock = CreateSemaphore(NULL,		/* get us a semaphore 				*/
      0, 1, tabgrab->filename+1);
	e = GetLastError();
	if (lock == NULL)					/* could not get semaphore?			*/
	{
	  rc = dt_err(ctl, F,
	    "CreateSemaphore(NULL, "
        "0, 1, \"%s\")"
        DTERRNO_LASTF,
        tabgrab->filename+1,
        DTERRNO_LASTE(e));
	  goto cleanup;
	}

	ReleaseSemaphore(lock, 1,			/* release access					*/
      NULL);

	tabgrab->handle = (long)handle;
	tabgrab->b = (void *)b;
	tabgrab->lock = (long)lock;
# else
	rc = dt_err(ctl, F,
      "memory mapped files"
      " not implemented on this OS");
	goto cleanup;
# endif
  }
  else									/* not using memory mapped file? 	*/
  {
# ifdef DTCONFIG_IMPLIED_MSW32			/* on Win32? 						*/
	int sysrc;
	int e;
	sysrc = sopen(						/* open shared file 				*/
      tabgrab->filename,
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
        tabgrab->filename,
        DTERRNO_ARG(e));

	DT_C(dtfd_fd,(ctl, &tabgrab->fd,	/* wrap fd around shared file 		*/
      tabgrab->filename, sysrc));
# else 									/* no in Win32? 					*/
    DT_C(dtfd_open,(ctl, &tabgrab->fd,	/* open the file normally			*/
      tabgrab->filename, "w"));
# endif

    DT_C(dtos_malloc2,(ctl,				/* get space for buffer 			*/
      &tabgrab->b, tabgrab->l,
      F, "buffer"));
  }

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(tabgrab_uninit,(tabgrab));
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
