/*..........................................................................
| NAME
| :dtshm: Function Group - shared memory operations
|
| SUMMARY
| The shared memory object ~shm~ encapsulates the ability
| of a process to share memory with other processes.
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: shmw.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>
#include <dtack/shm.h>
#include <dtack/str.h>
#include <dtack/errno.h>

#ifdef DTCONFIG_IMPLIED_MSW32

#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <io.h>

/*..........................................................................
| NAME
| dtshm_create() - create shared memory object
|
| SUMMARY
| Create the shared memory object ~shm~.
| If ~id~ is 0, the shared memory will be private.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtshm_create(							/* create new shared memory			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm,							/* returned shared memory structure	*/
  int id,								/* system-wide shared memory id 	*/
  long length)
{
  DT_F("dtshm_create");
  char name[256];
  HANDLE handle;
  long size;
  LPVOID b;
  DWORD e;
  dt_rc_e rc = DT_RC_GOOD;

  if (id == 0)
	DT_CLEANUP((ctl, F,
      "private shm not supported"));

  memset(shm, 0, sizeof(*shm));			/* clear shared memory structure 	*/

  DT_C(dtstr_printf,(ctl, 
    name, sizeof(name),
    "dtshm-%08lx", (unsigned long)id));
  
  handle = CreateFileMapping(
    INVALID_HANDLE_VALUE,				/* in-memory file 					*/
    NULL,
    PAGE_READWRITE,
    0, length,	
    TEXT((name)));						/* name of shared file 				*/
  e = GetLastError();
  if (handle == NULL ||					/* file mapping creation failed? 	*/
	  e == ERROR_ALREADY_EXISTS)
  {
	char s[4096];
    DTERRNO_LASTS(e, s, sizeof(s));
	DT_CLEANUP((ctl, F,
      "CreateFileMapping("
      "INVALID_HANDLE_VALUE, "
      "NULL, "
      "PAGE_READWRITE, "
      "0, %ld, \"%s\")"
      DTERRNO_LASTFS,
      length,
      name,
      DTERRNO_LASTES(e, s)));
  }

  shm->created = 1;

  b = MapViewOfFile(handle,				/* get pointer to mapped file 		*/
    FILE_MAP_READ | FILE_MAP_WRITE,
    0, 0, 0);
  e = GetLastError();
  if (b == NULL)						/* could not get mapped pointer? 	*/
  {
	char s[4096];
    DTERRNO_LASTS(e, s, sizeof(s));
	DT_CLEANUP((ctl, F,
      "MapViewOfFile("
      "0x%08lx, "
      "FILE_MAP_READ | FILE_MAP_WRITE, "
      "0, 0, 0)"
      DTERRNO_LASTFS,
      handle, 
      DTERRNO_LASTES(e, s)));
  }

  shm->attached = 1;

  shm->sid = (long)handle;
  shm->address = (void *)b;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "%s handle x%08lx",
    name, shm->sid);

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtshm_destroy,(ctl, shm));

  return rc;
}

/*..........................................................................
| NAME
| dtshm_access() - access shared memory object
|
| SUMMARY
| Access the shared memory object ~shm~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtshm_access(							/* access existing shared memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm,							/* returned shared memory structure	*/
  int id)								/* system-wide shared memory id 	*/
{
  DT_F("dtshm_access");
  char name[256];
  HANDLE handle;
  long size;
  LPVOID b;
  DWORD e;
  dt_rc_e rc = DT_RC_GOOD;

  memset(shm, 0, sizeof(*shm));			/* clear shared memory structure 	*/

  DT_C(dtstr_printf,(ctl, 
    name, sizeof(name),
    "dtshm-%08lx", (unsigned long)id));
  
  handle = OpenFileMapping(
    FILE_MAP_ALL_ACCESS,
    TRUE,
    TEXT((name)));						/* name of shared file 				*/
  e = GetLastError();
  if (handle == NULL)					/* file mapping creation failed? 	*/
  {
	char s[4096];
    DTERRNO_LASTS(e, s, sizeof(s));
	DT_CLEANUP((ctl, F,
      "OpenFileMapping("
      "FILE_MAP_ALL_ACCESS, "
      "TRUE, "
      "\"%s\")"
      DTERRNO_LASTFS,
      name,
      DTERRNO_LASTES(e, s)));
  }

  shm->created = 1;

  b = MapViewOfFile(handle,				/* get pointer to mapped file 		*/
    FILE_MAP_READ | FILE_MAP_WRITE,
    0, 0, 0);
  e = GetLastError();
  if (b == NULL)						/* could not get mapped pointer? 	*/
  {
	char s[4096];
    DTERRNO_LASTS(e, s, sizeof(s));
	DT_CLEANUP((ctl, F,
      "MapViewOfFile("
      "0x%08lx, "
      "FILE_MAP_READ | FILE_MAP_WRITE, "
      "0, 0, 0)"
      DTERRNO_LASTFS,
      handle, 
      DTERRNO_LASTES(e, s)));
  }

  shm->attached = 1;

  shm->sid = (long)handle;
  shm->address = (void *)b;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "accessed %s on handle 0x%08lx",
    name, shm->sid);

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtshm_destroy,(ctl, shm));

  return rc;
}

/*..........................................................................
| NAME
| dtshm_release() - release shared memory object
|
| SUMMARY
| Free all resources associated with ~shm~ object which was accessed.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtshm_release(							/* release shm object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm)							/* shm object 						*/
{
  DT_F("dtshm_release");
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (shm->attached)					/* shm address got attached? 		*/
  {
	UnmapViewOfFile(shm->address);
  }
  
  if (shm->created)						/* shm object got created? 			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "releasing handle 0x%08lx",
      shm->sid);
	CloseHandle(
      (HANDLE)shm->sid);
  }

  memset(shm, 0, sizeof(*shm));
  
  return rc;
}

/*..........................................................................
| NAME
| dtshm_destroy() - destroy shared memory object
|
| SUMMARY
| Free all resources associated with ~shm~ object which was created.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtshm_destroy(							/* release shm object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtshm_t *shm)							/* shm object 						*/
{
  DT_F("dtshm_destroy");
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (shm->attached)					/* shm address got attached? 		*/
  {
	UnmapViewOfFile(shm->address);
  }
  
  if (shm->created)						/* shm object got created? 			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "destroying handle 0x%08lx",
      shm->sid);
	CloseHandle(
      (HANDLE)shm->sid);
  }

  memset(shm, 0, sizeof(*shm));
  
  return rc;
}

#endif
