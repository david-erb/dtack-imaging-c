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
| TITLE
| The :phys: Function Group
| 
|!Summary
| The :dtphys: functions work together to map physical memory.
|
| The :dtphys_t: data structure used by all these functions
| contains the public members ~physaddr~, ~virtaddr~, and ~size~.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: phys.c,v $ $Revision: 1.2 $");
#include <dtack/phys.h>

#define DESC "physical memory access"

#ifdef DTCONFIG_IMPLIED_QNX4
#  ifndef DTDEPEND
#    include <sys/mman.h>
#  endif
#endif

/*..........................................................................
| NAME
| dtphys_open - open a file
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtphys_open(							/* open a file						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtphys_t *phys,						/* phys object descriptor 			*/
  long physaddr,
  long size)
{
  DT_F("dtphys_open");
#ifdef DTCONFIG_IMPLIED_QNX4
  int sysrc;
  long virtaddr;
  dt_rc_e rc = DT_RC_GOOD;

  DT_MEMSET(phys, '\0',					/* clear structure memory			*/
    sizeof(&phys));

  sysrc = shm_open("Physical",
    O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
  if (sysrc == -1)
  {
	rc = dt_err(ctl, F,
      "shm_open(\"Physical\","
      " O_RDWR) failed errno %d",
      errno);
	goto cleanup;
  }
  phys->fd = sysrc;

  virtaddr = (long)mmap(0, size,
    PROT_READ|PROT_WRITE,
    MAP_SHARED, phys->fd,
    physaddr);
  if (virtaddr == -1)
  {
	rc = dt_err(ctl, F,
      "mmap(0, 4096,"
      " PROT_READ|PROT_WRITE,",
      " MAP_SHARED, %d, 0x%08lx)"
      " failed errno %d",
       phys->fd, physaddr, errno);
	goto cleanup;
  }

  phys->virtaddr = (void *)virtaddr;
  phys->physaddr = physaddr;
  phys->size = size;
 
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "opened 0x%08lx"
    " at virtaddr 0x%08lx"
    " on fd %d size %ld",
    phys->physaddr, 
    (unsigned long)phys->virtaddr,
    phys->fd, phys->size);


cleanup:
  if (rc != DT_RC_GOOD)
    DT_I(dtphys_close,(ctl, phys));

  return rc;
#else
  return dt_err_notlinked(ctl, F, DESC);  
#endif
}

/*..........................................................................
| NAME
| dtphys_close() - close and keep file
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtphys_close(							/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtphys_t *phys)						/* phys object descriptor 			*/
{
  DT_F("dtphys_close");
#ifdef DTCONFIG_IMPLIED_QNX4
  int sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "closing 0x%08lx"
    " at virtaddr 0x%08lx"
    " on fd %d size %ld",
    phys->physaddr, 
    (unsigned long)phys->virtaddr,
    phys->fd, phys->size);

  if (phys->virtaddr != NULL)			/* mmap had been done? 				*/
  {
	sysrc = munmap(phys->virtaddr,
      phys->size);
	if (sysrc != 0)
	  rc = dt_err(ctl, F,
        "munmap(0x%08lx, %ld)"
        " failed errno %d",
        phys->virtaddr, phys->size);
  }
  if (phys->fd != 0)
  {
	sysrc = close(phys->fd);
	if (sysrc != 0)
      rc = dt_err(ctl, F,
        "close(%d) failed errno %d",
        phys->fd, errno);
  }

  DT_MEMSET(phys, '\0', sizeof(*phys));	/* clear structure memory			*/

  return rc;
#else
  return dt_err_notlinked(ctl, F, DESC);  
#endif
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
