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
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: mem_disk.c,v $ $Revision: 1.2 $");

#include <dtack/mem.h>
#include <dtack/os.h>

#include <dtack/prj.h>						/* need this for memdisk		    */

#if DTPRJ_MEMDISK != DT_1

/*..........................................................................
 *..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dtmemdisk_init(							/* init a "disk" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemdisk_init");
  return dt_err_notlinked(ctl,
    F, "disk paging");
}

#else

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

static
dt_rc_e
dtmemdisk_uninit(						/* uninit a "disk" memsource   	    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_F("dtmemdisk_uninit");
  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being uninitialized", 
    memsource->name);
  DT_Q(dtmemsource_uninit,(ctl,			/* do common uninit for memsources	*/
    memsource));
  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 *..........................................................................*/

static
dt_rc_e
dtmemdisk_open(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  const char *name,
  int seqnum,
  long total)
{
  DT_F("dtmemdisk_open");
  dtmemdisk_t *memdisk = &mem->u.disk;
  dt_rc_e rc;

  mem->type = DTMEM_TYPE_DISK;			/* remember our type			    */

  sprintf(mem->name, name, seqnum);		/* make up a filename			    */

  DT_Q(dtfd_open,(ctl, &memdisk->fd,	/* open the disk memory file	    */
    mem->name, "wr"));

  DT_G(dtfd_seek,(ctl, &memdisk->fd,	/* stretch out the disk file		*/
    total-1, DTFD_WHENCE_SET, NULL));

  if (rc == DT_RC_GOOD)
  DT_G(dtfd_write_all,(ctl, 
    &memdisk->fd, &rc, 1));

  if (rc != DT_RC_GOOD)
    DT_I(dtfd_delete,(ctl,
      &memdisk->fd));

  memdisk->total = total;				/* remember the size the file is    */

  mem->gotspace = 1;					/* assumes disk space unlimited	    */

  return rc;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

static
dt_rc_e
dtmemdisk_resize(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long total)
{
  DT_F("dtmemdisk_resize");
  dtmemdisk_t *memdisk = &mem->u.disk;
  dt_rc_e rc = DT_RC_GOOD;

  if (total > memdisk->total)			/* allocation being increased?	    */
  {
    DT_GI(dtfd_seek,(ctl, &memdisk->fd,	/* stretch out the disk file		*/
      total-1, DTFD_WHENCE_SET, NULL));

    DT_GI(dtfd_write_all,(ctl, 
      &memdisk->fd, &rc, 1));

    if (rc == DT_RC_GOOD)
	{
      mem->gotspace = 1;
      memdisk->total = total;			/* remember the size the file is    */
	}
    else
      mem->gotspace = 0;
  }
  else
  if (total < memdisk->total)			/* allocation being decreased?	    */
  {
    mem->gotspace = 1;					/* just leave the file too big	    */
    memdisk->total = total;				/* remember the size the file is    */
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemdisk_close(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem)
{
  DT_F("dtmemdisk_close");
  dtmemdisk_t *memdisk = &mem->u.disk;

  DT_Q(dtfd_delete,(ctl, &memdisk->fd));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemdisk_write(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemdisk_write");
  dtmemdisk_t *memdisk = &mem->u.disk;

  DT_Q(dtfd_seek,(ctl,					/* set file pointer to offset	    */
    &memdisk->fd, offset,
    DTFD_WHENCE_SET, NULL));

  DT_Q(dtfd_write_all,(ctl,				/* write buffer					    */
    &memdisk->fd, buf, size));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemdisk_read(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemdisk_read");
  dtmemdisk_t *memdisk = &mem->u.disk;

  DT_Q(dtfd_seek,(ctl,					/* set file pointer to offset	    */
    &memdisk->fd, offset,
    DTFD_WHENCE_SET, NULL));

  DT_Q(dtfd_read_all,(ctl,				/* read buffer					    */
    &memdisk->fd, buf, size));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemdisk_init(							/* init a "disk" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemdisk_init");
  dt_dbg(ctl, F, 
    DT_DBG_MASK_MEMX|DT_DBG_MASK_PAGE,
    "\"%s\" being initialized"
    " pagesize %u npages %u",
    name, pagesize, npages);

  DT_Q(dtmemsource_init,(ctl,			/* do common init for memsources	*/
    memsource, name,
    maxusage, pagesize, npages,
    DTMEMDISK_MAXUSAGE_DEFAULT,
    DTMEMDISK_PAGESIZE_DEFAULT,
    DTMEMDISK_NPAGES_DEFAULT));

  memsource->uninit = dtmemdisk_uninit;
  memsource->alloc  = dtmemstd_alloc;	/* disk uses standard paging	    */
  memsource->open   = dtmemdisk_open;
  memsource->resize = dtmemdisk_resize;
  memsource->close  = dtmemdisk_close;
  memsource->write  = dtmemdisk_write;
  memsource->read   = dtmemdisk_read;
  
  return DT_RC_GOOD;
}

#endif












/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
