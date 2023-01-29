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
  20000508 DE removed dummy functions for windows (now in semw.c)
  20000228 DE added dummy functions for windows
  20000104 DE properly declare union semun for egcs
  19981213 DE can't pass NULL for senum
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: sem.c,v $ $Revision: 1.8 $");
#include <dtack/sem.h>
#include <dtack/errno.h>

#if !defined(DTCONFIG_IMPLIED_MSW32)
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#if defined(DTCONFIG_SG5) || defined(DTCONFIG_SP1) || \
  (defined(__GNUC_MINOR__) && __GNUC_MINOR__ > 90)
typedef union semun {
  int val;
  struct semid_ds *buf;
  ushort *array;
} semun;
#endif

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
  int sysrc;
  int e;
  union semun arg;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)
	key = (int)IPC_PRIVATE;


  memset(sem, 0, sizeof(*sem));			/* clear semaphore structure 		*/
  
  sysrc = semget(						/* get new semaphore 				*/
    (key_t)key, 
    1,									/* number of semaphores in set 		*/
    0666 | IPC_CREAT);
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "semget(%d, 1, 0666|IPC_CREAT)" 
      DTERRNO_FMT,
      key,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  sem->key = key;
  sem->sid = sysrc;
  sem->created = 1;

  arg.val = 0;
  sysrc = semctl(						/* initialize new semaphore 		*/
    sem->sid,
    0,									/* semnum							*/
    SETVAL,								/* cmd 								*/
    arg);								/* arg 								*/
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "semctl(%d, 0, IPC_SETVAL, {val=0}"
      DTERRNO_FMT,
      sem->sid,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "created key=%d sid=%d", 
    sem->key, sem->sid);

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
  dt_rc_e rc = DT_RC_GOOD;
  if (sem->created)						/* sem object got created? 			*/
  {
    union semun arg;
	int sysrc;
	int e;

    DT_MEMSET(&arg, 0, sizeof(arg));
	sysrc = semctl(						/* remove semaphore 				*/
      sem->sid,
      0,								/* semnum							*/
      IPC_RMID,							/* cmd 								*/
      arg);								/* arg 								*/
	e = errno;
	if (sysrc == -1)					/* system call failed? 				*/
    {
	  rc = dt_err(ctl, F,
        "semctl(%d, 0, IPC_RMID, NULL"
        DTERRNO_FMT,
        sem->sid,
        DTERRNO_ARG(e));
    }
	else
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "destroyed key=%d sid=%d", 
        sem->key, sem->sid);
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
  struct sembuf sb;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (!sem->initialized)				/* sem object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");
  if (count <= 0)
	return dt_err(ctl, F,
      "invalid count %d", count);

  sb.sem_num = 0;
  sb.sem_op = count;
  sb.sem_flg = 0;

  sysrc = semop(						/* get new semaphore 				*/
    sem->sid,
    &sb,
    1);									/* number of semaphores in set 		*/
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "semop(%d, {%d, %d, %d}, 1)" 
      DTERRNO_FMT,
      sem->sid, 
      sb.sem_num,
      sb.sem_op,
      sb.sem_flg,
      DTERRNO_ARG(e));
	goto cleanup;
  }

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
  struct sembuf sb;
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (!sem->initialized)				/* sem object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");
  if (count <= 0)
	return dt_err(ctl, F,
      "invalid count %d", count);

  sb.sem_num = 0;
  sb.sem_op = -count;
  sb.sem_flg = 0;

  sysrc = semop(						/* get new semaphore 				*/
    sem->sid,
    &sb,
    1);									/* number of semaphores in set 		*/
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "semop(%d, {%d, %d, %d}, 1)" 
      DTERRNO_FMT,
      sem->sid, 
      sb.sem_num,
      sb.sem_op,
      sb.sem_flg,
      DTERRNO_ARG(e));
	goto cleanup;
  }

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
  int sysrc;
  int e;
  union semun arg;
  dt_rc_e rc = DT_RC_GOOD;

  if (!sem->initialized)				/* sem object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");

  arg.val = 0;
  sysrc = semctl(						/* initialize new semaphore 		*/
    sem->sid,
    0,									/* semnum							*/
    GETVAL,								/* cmd 								*/
    arg);								/* arg 								*/
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "semctl(%d, 0, IPC_GETVAL, {val=0}"
      DTERRNO_FMT,
      sem->sid,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  *count = sysrc;
cleanup:

  return rc;
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
  int sysrc;
  int e;
  union semun arg;
  dt_rc_e rc = DT_RC_GOOD;

  if (!sem->initialized)				/* sem object got created? 			*/
	return dt_err(ctl, F,
      "uninitialized structure");

  arg.val = 0;
  sysrc = semctl(						/* initialize new semaphore 		*/
    sem->sid,
    0,									/* semnum							*/
    GETNCNT,							/* cmd 								*/
    arg);								/* arg 								*/
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "semctl(%d, 0, IPC_GETZCNT, {val=0}"
      DTERRNO_FMT,
      sem->sid,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  *waiters = sysrc;
cleanup:

  return rc;
}
#endif
