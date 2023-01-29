/*..........................................................................
| NAME
| :dtsem: Function Group - semaphore operations
|
| SUMMARY
| The semaphore object ~sem~ encapsulates the ability
| of a process to post and wait on semaphores.
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000507 DE added to the library
  19991231 DE created
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: sem.c,v $ $Revision: 1.8 $");
#include <dtack/sem.h>
#include <dtack/str.h>
#include <dtack/errno.h>

#if defined(DTCONFIG_IMPLIED_MSW32)
#include <dtack/msw.h>

/*..........................................................................
| NAME
| dtsem_create() - create semaphore object
|
| SUMMARY
| Create the semaphore object ~sem~.
| If ~id~ is 0, the semaphore will be private.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsem_create(							/* create new semaphore				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* returned semaphore structure 	*/
  int key)								/* system-wide semaphore id 		*/
{
  DT_F("dtsem_create");
  const char *name;
  char name2[32];
  HANDLE handle;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)							/* unnamed semaphore? 				*/
	name = NULL;
  else									/* named semaphore? 				*/
  {
	DT_C(dtstr_printf,(ctl,				/* win32 wants a string 			*/
      name2, sizeof(name2),
      "dtsem%08d", key));
	name = name2;
  }

  memset(sem, 0, sizeof(*sem));			/* clear semaphore structure 		*/

  handle = CreateSemaphore(
    NULL,								/* security attributes 				*/
	0,									/* initial count 					*/
	0x7fffffff,							/* maximum count 					*/
    name);								/* semaphore's name 				*/
  if (!handle)							/* system call failed? 				*/
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "CreateSemaphore(NULL, 0, 0x7fffffff, \"%s\")" 
      DTERRNO_LASTFS,
      name? name: "NULL",
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }

  sem->key = key;
  sem->sid = (int)handle;
  sem->created = 1;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "created 0x%08lx %d:0x%08lx", 
    sem, sem->key, handle);

  sem->initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtsem_destroy,(ctl, sem));

  return rc;
}

/*..........................................................................
| NAME
| dtsem_destroy() - destroy semaphore object
|
| SUMMARY
| Free all resources associated with ~sem~ object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsem_destroy(							/* release sem object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem)							/* sem object 						*/
{
  DT_F("dtsem_destroy");
  HANDLE handle = (void *)sem->sid;
  dt_rc_e rc = DT_RC_GOOD;
  if (sem->created)						/* sem object got created? 			*/
  {
	BOOL sysrc;
	sysrc = CloseHandle(handle);
	if (!sysrc)
	{
	  char s[256];
	  DWORD e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "CloseHandle(0x%08lx) on skey %d" 
        DTERRNO_LASTFS,
        handle, sem->key,
       DTERRNO_LASTES(e, s));
	}
  }
  memset(sem, 0, sizeof(*sem));
  
  return rc;
}

/*..........................................................................
| NAME
| dtsem_post() - post semaphore object
|
| SUMMARY
| Increment semaphore count.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsem_post(								/* post sem object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* sem object 						*/
  int count)							/* post count 						*/
{
  DT_F("dtsem_post");
  HANDLE handle = (void *)sem->sid;
  BOOL sysrc;
  long oldcount;
  dt_rc_e rc = DT_RC_GOOD;

  if (!sem->initialized)				/* sem object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");
  if (count <= 0)
	return dt_err(ctl, F,
      "invalid count %d", count);
  
  sysrc = ReleaseSemaphore(handle,		/* increase semaphore count 		*/
    count, &oldcount);
  if (!sysrc)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "ReleaseSemaphore(0x%08lx) on skey %d" 
      DTERRNO_LASTFS,
      handle, sem->key,
      DTERRNO_LASTES(e, s));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_ASYNC))
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "posted %d to 0x%08lx %d:0x%08lx, now %ld", 
    count, sem, sem->key, handle, count+oldcount);

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtsem_wait() - wait on semaphore object
|
| SUMMARY
| Decrement semaphore count.	
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsem_wait(								/* wait sem object 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* sem object 						*/
  int count)							/* wait count 						*/
{
  DT_F("dtsem_wait");
  HANDLE handle = (void *)sem->sid;
  DWORD sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (!sem->initialized)				/* sem object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");
  if (count <= 0)
	return dt_err(ctl, F,
      "invalid count %d", count);
  
  sysrc = WaitForSingleObject(handle, 0);
  if (sysrc != WAIT_FAILED &&
	  sysrc != WAIT_TIMEOUT)
  {
    if (DT_DBG(ctl, DT_DBG_MASK_ASYNC))
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "immediately got 0x%08lx %d:0x%08lx", 
      sem, sem->key, handle);
	goto cleanup;
  }

  if (DT_DBG(ctl, DT_DBG_MASK_ASYNC))
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "waiting for 0x%08lx %d:0x%08lx", 
     sem, sem->key, handle);

  sysrc = WaitForSingleObject(handle, INFINITE);
  if (sysrc == WAIT_FAILED)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "WaitForSingleObject(0x%08lx, INFINITE) on 0x%08lx %d" 
      DTERRNO_LASTFS,
      handle, sem, sem->key,
      DTERRNO_LASTES(e, s));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_ASYNC))
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "got 0x%08lx %d:0x%08lx", 
    sem, sem->key, handle);

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtsem_get_count() - return current semaphore count
|
| SUMMARY
| Return current semaphore count.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsem_get_count(						/* get current semaphore count		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* returned semaphore structure 	*/
  int *count)
{
  DT_F("dtsem_get_count");
  return dt_err(ctl, F, "not implemented");
}

/*..........................................................................
| NAME
| dtsem_get_waiters() - return current semaphore waiters
|
| SUMMARY
| Return current semaphore waiters.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtsem_get_waiters(						/* get current semaphore waiters		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtsem_t *sem,							/* returned semaphore structure 	*/
  int *waiters)
{
  DT_F("dtsem_get_waiters");
  return dt_err(ctl, F, "not implemented");
}

#endif 									/* end if WIN32 					*/
