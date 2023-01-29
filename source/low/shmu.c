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
DT_RCSID("source/low $RCSfile: shm.c,v $ $Revision: 1.8 $");
#include <dtack/shm.h>
#include <dtack/errno.h>

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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
  int sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (id == 0)
	id = (int)IPC_PRIVATE;

  memset(shm, 0, sizeof(*shm));			/* clear shared memory structure 		*/
  
  sysrc = shmget(						/* get new shared memory 				*/
    (key_t)id, 
    length,								/* length of shared memory			*/
    0666 | IPC_CREAT);
  e = errno;
  if (sysrc == -1)						/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "shmget(%d, 1, 0666|IPC_CREAT)" 
      DTERRNO_FMT,
      id,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  shm->sid = sysrc;
  shm->created = 1;

  shm->address = shmat(					/* attach new shared memory 		*/
    shm->sid,
    NULL,								/* don't care which address			*/
    0);									/* flags 							*/
  e = errno;
  if (shm->address == (void *)(-1))		/* system call failed? 				*/
  {
	rc = dt_err(ctl, F,
      "shmat(%d, NULL, 0)"
      DTERRNO_FMT,
      shm->sid,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  shm->attached = 1;
  
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "created shmid x%08lx",
    shm->sid);

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtshm_destroy,(ctl, shm));

  return rc;
}

/*..........................................................................
| NAME
| dtshm_destroy() - destroy shared memory object
|
| SUMMARY
| Free all resources associated with ~shm~ object.
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
  
  if (shm->attached)
  {
	sysrc = shmdt(						/* initialize new shared memory 		*/
      shm->address);
	e = errno;
	if (sysrc == -1)					/* system call failed? 				*/
    {
	  rc = dt_err(ctl, F,
        "shmdt(0x%08lx)"
        DTERRNO_FMT,
        shm->address,
        DTERRNO_ARG(e));
	}
  }
  
  if (shm->created)						/* shm object got created? 			*/
  {
	struct shmid_ds ds;

    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "destroying shmid 0x%08lx",
      shm->sid);

	sysrc = shmctl(						/* destroy shared memory 			*/
      shm->sid,
      IPC_STAT,							/* cmd 								*/
      &ds);								/* arg 								*/
	e = errno;
	if (sysrc == -1)					/* system call failed? 				*/
    {
	  rc = dt_err(ctl, F,
        "shmctl(%d, 0, IPC_STAT, &ds"
        DTERRNO_FMT,
        shm->sid,
        DTERRNO_ARG(e));
    }
	else
    if (ds.shm_nattch > 0)
	  rc = dt_err(ctl, F,
        "attempt to delete with nonzero"
        " attach count %d",
        ds.shm_nattch);

	sysrc = shmctl(						/* destroy shared memory 			*/
      shm->sid,
      IPC_RMID,							/* cmd 								*/
      NULL);							/* arg 								*/
	e = errno;
	if (sysrc == -1)					/* system call failed? 				*/
    {
	  rc = dt_err(ctl, F,
        "shmctl(%d, 0, IPC_RMID, NULL"
        DTERRNO_FMT,
        shm->sid,
        DTERRNO_ARG(e));
    }
  }

  memset(shm, 0, sizeof(*shm));
  
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
  return dt_err(ctl, F, "not implemented");
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
  return dt_err(ctl, F, "not implemented");
}

#endif
