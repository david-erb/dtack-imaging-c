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
#include <dtack/mem.h>

DT_RCSID("source/low $RCSfile: mem_xms.c,v $ $Revision: 1.2 $");

#if DTPRJ_TXMS != DT_1

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemtxms_init(							/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemtxms_init");
  return dt_err_notlinked(ctl,
    F, "txms (by TechniLib Company)");  
}

#else

#include <dttxms.h>

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
dtmemtxms_err(
  dt_ctl_t *ctl,
  const char *f,
  const char *xmmfuncname,
  unsigned char error)
{
  const char *s;
  switch(error)
  {
#define CASE(ERROR, MESSAGE) case ERROR: s = MESSAGE; break	
    CASE(0x80, "the function is not implemented");
    CASE(0x81, "a VDISK device is detected");
    CASE(0x82, "an A20 error occurs");
    CASE(0x8E, "a general driver error occurs");
    CASE(0x8F, "an unrecoverable driver error occurs");
    CASE(0x90, "the HMA does not exist");
    CASE(0x91, "the HMA is already in use");
    CASE(0x92, "DX is less than the /HMAMIN= parameter");
    CASE(0x93, "the HMA is not allocated");
    CASE(0x94, "the A20 line is still enabled");
    CASE(0xA0, "all extended memory is allocated");
    CASE(0xA1, "all available extended memory handles are in use");
    CASE(0xA2, "the handle is invalid");
    CASE(0xA3, "the SourceHandle is invalid");
    CASE(0xA4, "the SourceOffset is invalid");
    CASE(0xA5, "the DestHandle is invalid");
    CASE(0xA6, "the DestOffset is invalid");
    CASE(0xA7, "the Length is invalid");
    CASE(0xA8, "the move has an invalid overlap");
    CASE(0xA9, "a parity error occurs");
    CASE(0xAA, "the block is not locked");
    CASE(0xAB, "the block is locked");
    CASE(0xAC, "the block's lock count overflows");
    CASE(0xAD, "the lock fails");
    CASE(0xB0, "a smaller UMB is available");
    CASE(0xB1, "no UMBs are available");
    CASE(0xB2, "the UMB segment number is invalid");
    default: s = "unknown";
  }

  return dt_err(ctl, f,
    "xms error: %s (0x%02x) in %s",
    s, error, xmmfuncname);
}

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

static
dt_rc_e
dtmemtxms_uninit(						/* uninit an "xms" memsource   	    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_F("dtmemtxms_uninit");
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
dtmemtxms_open(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  const char *name,
  int seqnum,
  long total)
{
  DT_F("dtmemtxms_open");
  dtmemtxms_t *memxms = &mem->u.txms;
  int error;
  unsigned int freemem, maxfree;
  unsigned int totalk;

  error = getfreexmsmem(&freemem,			/* query available memory 			*/
    &maxfree);
  if (error)
    return dtmemtxms_err(ctl, F,
      "getfreexmsmem", error);

  totalk = (unsigned int)
    ((total+1023L) / 1024L);
  if (freemem < totalk)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "need %uK bytes,"
      " but free is %uK/%uK",
      totalk, maxfree, freemem);
    mem->gotspace = 0;
  }
  else
  {
    unsigned int newhandle;
    mem->type = DTMEM_TYPE_TXMS;		/* remember our type			    */
	
    error = getxmsmem(totalk,			/* allocate space 					*/
      &newhandle);
    if (error)
      return dtmemtxms_err(ctl, F,
        "getxmsmem", error);

    error = getfreexmsmem(&freemem,		/* how much memory now? 			*/
      &maxfree);
    if (error != 0)
      return dtmemtxms_err(ctl, F,
        "getfreexmsmem", error);

	dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
      "handle 0x%04x size %uK"
      " free now %uK/%uK", 
      newhandle, totalk, 
      maxfree, freemem);

    memxms->handle = newhandle;			/* we have a new handle			    */
    mem->gotspace = 1;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemtxms_close(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem)
{
  DT_F("dtmemtxms_close");
  dtmemtxms_t *memxms = &mem->u.txms;
  int error;
  
  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
    "freeing handle 0x%04x", 
    memxms->handle);

  error = freexmsmem(memxms->handle);
  if (error)
    return dtmemtxms_err(ctl, F,
      "freexmsmem", error);

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

static
dt_rc_e
dtmemtxms_resize(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long total)
{
  DT_F("dtmemtxms_resize");
  int error;

#define REALLOC_DOES_NOT_WORK
# ifdef REALLOC_DOES_NOT_WORK
  {
    dtmem_t newmem = *mem;
    long size = DT_MIN(total,			/* may be shrinking 				*/
      mem->total);

    DT_Q(dtmemtxms_open,(ctl, &newmem,	/* get space for new total 			*/
      mem->name, 0, total));

    error = movexmsmem(size,			/* copy from old space to new 		*/
      mem->u.txms.handle, 0,
      newmem.u.txms.handle, 0);

    if (error)
	{
	  dt_rc_e rc;
      rc = dtmemtxms_err(ctl, F,
        "movexmsmem", error);
      DT_I(dtmemtxms_close,(ctl,		/* free new memory 					*/
		&newmem));
	  return rc;
	}

    DT_Q(dtmemtxms_close,(ctl, mem));	/* free old memory 					*/
	
	*mem = newmem;
  }
# else
  {
    return dt_err(ctl, F,
      "true realloc not implemented");
  }
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemtxms_write(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemtxms_write");
  int error;

  error = movexmsmem(size,	
    0, (unsigned long)buf,
    mem->u.txms.handle, offset);

  if (error)
    return dtmemtxms_err(ctl, F,
      "movexmsmem", error);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemtxms_read(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemtxms_read");
  int error;

  error = movexmsmem(size,	
    mem->u.txms.handle, offset,
    0, (unsigned long)buf);

  if (error)
    return dtmemtxms_err(ctl, F,
      "movexmsmem", error);

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemtxms_init(							/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemtxms_init");
  int error;
  unsigned int freemem, maxfree;

  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being initialized"
    " pagesize %u npages %u",
    name, pagesize, npages);

  error = initxms();
  if (error)							/* init XMM library				    */
    return dtmemtxms_err(ctl, F,
      "initxms", error);

  error = getfreexmsmem(&freemem,			/* query available memory 			*/
    &maxfree);
  if (error)
    return dtmemtxms_err(ctl, F,
      "getfreexmsmem", error);

  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
    "txms"
    " has %uK/%uK",
    maxfree, freemem);

  DT_Q(dtmemsource_init,(ctl,			/* do common init for memsources	*/
    memsource, name,
    maxusage, pagesize, npages,
    DTMEMXMS_MAXUSAGE_DEFAULT,
    DTMEMXMS_PAGESIZE_DEFAULT,
    DTMEMXMS_NPAGES_DEFAULT));

  memsource->uninit = dtmemtxms_uninit;
  memsource->alloc  = dtmemstd_alloc;	/* xms uses standard paging		    */
  memsource->open   = dtmemtxms_open;
  memsource->resize = dtmemtxms_resize;
  memsource->close  = dtmemtxms_close;
  memsource->write  = dtmemtxms_write;
  memsource->read   = dtmemtxms_read;
  
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
