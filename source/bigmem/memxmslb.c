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

#if DTPRJ_XMSLB != DT_1

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemxmslb_init(						/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemxmslb_init");
  return dt_err_notlinked(ctl,
    F, "xmslb (by Michael Graff)");  
}

#else

#define REALLOC_DOES_NOT_WORK

#ifdef DTCONFIG_BORLAND
#  define DTCONFIG_SAVEREGS _saveregs
#else
#  define DTCONFIG_SAVEREGS __saveregs
#endif

#include <dtack/xmslb.h>

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e dtmemxmslb_err(
  dt_ctl_t *ctl,
  const char *f,
  const char *xmmfuncname,
  int error)
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
    CASE(0xff, "no XMS driver found");
    default: s = "unknown";
  }

  return dt_err(ctl, f,
    "xms error: %s (0x%02x) in %s",
    s, error, xmmfuncname);
}
/*..........................................................................
 *..........................................................................*/
static
void 
DTCONFIG_SAVEREGS
r_setup(
  int *error)
{
  if (XMS_Setup())
    *error = 0;
  else
    *error = 0xff;
}
/*..........................................................................
 *..........................................................................*/
static
void 
DTCONFIG_SAVEREGS
r_freemem(
  unsigned int *freemem,
  unsigned int *totmem,
  int *error)
{
  *error = XMS_FreeMem(freemem, totmem);
}
/*..........................................................................
 *..........................................................................*/
static
void 
DTCONFIG_SAVEREGS
r_version(
  unsigned int *protocol,
  unsigned int *internal,
  unsigned int *hmem,
  int *error)
{
  *error = XMS_Version(protocol, internal, hmem);
}
/*..........................................................................
 *..........................................................................*/
static
void 
DTCONFIG_SAVEREGS
r_alloc(
  unsigned int size,
  unsigned int *handle,
  int *error)
{
  *error = XMS_AllocEMB(size, handle);
}
/*..........................................................................
 *..........................................................................*/
#ifndef REALLOC_DOES_NOT_WORK

static
void 
DTCONFIG_SAVEREGS
r_realloc(
  unsigned int handle,
  unsigned int size,
  int *error)
{
  *error = XMS_ReallocEMB(handle, size);
}

#endif
/*..........................................................................
 *..........................................................................*/
static
void 
DTCONFIG_SAVEREGS
r_move(
  struct EMMMoveStruct *r,
  int *error)
{
  *error = XMS_MoveEMB(r);
}
/*..........................................................................
 *..........................................................................*/
static
void 
DTCONFIG_SAVEREGS
r_free(
  unsigned int handle,
  int *error)
{
  *error = XMS_FreeEMB(handle);
}

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

static
dt_rc_e
dtmemxmslb_uninit(						/* uninit an "xms" memsource   	    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_F("dtmemxmslb_uninit");
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
dtmemxmslb_open(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  const char *name,
  int seqnum,
  long total)
{
  DT_F("dtmemxmslb_open");
  dtmemxmslb_t *memxms = &mem->u.xmslb;
  int error;
  unsigned int freemem, totmem;
  unsigned int totalk;

  r_freemem(&freemem,					/* query available memory 			*/
    &totmem, &error);
  if (error)
    return dtmemxmslb_err(ctl, F,
      "XMS_FreeMem", error);

  totalk = (unsigned int)
    ((total+1023L) / 1024L);
  if (freemem < totalk)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "need %uK bytes,"
      " but free is %uK/%uK",
      totalk, freemem, totmem);
    mem->gotspace = 0;
  }
  else
  {
    XMShandle newhandle;
    mem->type = DTMEM_TYPE_XMSLB;		/* remember our type			    */
	
    r_alloc(totalk,						/* allocate space 					*/
      &newhandle, &error);
    if (error)
      return dtmemxmslb_err(ctl, F,
        "XMS_AllocEMB", error);

    r_freemem(&freemem,					/* how much memory now? 			*/
      &totmem, &error);
    if (error != 0)
      return dtmemxmslb_err(ctl, F,
        "XMS_FreeMem", error);

	dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
      "handle 0x%04x size %uK"
      " free now %uK/%uK", 
      newhandle, totalk, 
      freemem, totmem);

    memxms->handle = newhandle;			/* we have a new handle			    */
    mem->gotspace = 1;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemxmslb_close(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem)
{
  DT_F("dtmemxmslb_close");
  dtmemxmslb_t *memxms = &mem->u.xmslb;
  int error;
  
  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
    "freeing handle 0x%04x", 
    memxms->handle);

  r_free(memxms->handle, &error);

  if (error)
    return dtmemxmslb_err(ctl, F,
      "XMS_FreeEMB", error);

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

static
dt_rc_e
dtmemxmslb_resize(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long total)
{
  DT_F("dtmemxmslb_resize");
  int error;

# ifdef REALLOC_DOES_NOT_WORK
  {
    dtmem_t newmem = *mem;
    long size = DT_MIN(total,			/* may be shrinking 				*/
      mem->total);
	struct EMMMoveStruct r;

    DT_Q(dtmemxmslb_open,(ctl, &newmem,	/* get space for new total 			*/
      mem->name, 0, total));

	r.TransferLength = size;
	r.SourceHandle = mem->u.xmslb.handle;
	r.SourceOffset = 0;
	r.DestHandle = newmem.u.xmslb.handle;
	r.DestOffset = 0;

    r_move(&r, &error);					/* copy from old space to new 		*/

    if (error)
	{
	  dt_rc_e rc;
      rc = dtmemxmslb_err(ctl, F,
        "XMS_MoveEMB", error);
      DT_I(dtmemxmslb_close,(ctl,		/* free new memory 					*/
		&newmem));
	  return rc;
	}

    DT_Q(dtmemxmslb_close,(ctl, mem));	/* free old memory 					*/
	
	*mem = newmem;
  }
# else
  {
    dtmemxmslb_t *memxms = &mem->u.xmslb;
    unsigned int freemem, totmem;
    unsigned int totalk;

    r_freemem(&freemem,			/* query available memory 			*/
      &totmem, &error);
    if (error)
      return dtmemxmslb_err(ctl, F,
        "XMS_FreeMem", error);

    totalk = (unsigned int)
      ((total+1023L) / 1024L);
    if (freemem < totalk)
    {
      dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
        "need %uK bytes, but largest" 
        " block only has %uK",
        totalk, freemem);
      mem->gotspace = 0;
    }
    else
    {
      r_realloc(memxms->handle,			/* resize the space				    */
        totalk, &error);
      if (error)						/* resizing failed?				    */
	    return dtmemxmslb_err(ctl, F,	/* give up 							*/
          "XMS_Realloc", error);

      r_freemem(&freemem,				/* how much memory now? 			*/
        &totmem, &error);
      if (error != 0)
        return dtmemxmslb_err(ctl, F,
          "XMS_FreeMem", error);

  	  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
        "handle 0x%04x size %uK"
        " free now %uK/%uK", 
        memxms->handle, totalk, 
        freemem, totmem);

      mem->gotspace = 1;
	}
  }
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemxmslb_write(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemxmslb_write");
  dtmemxmslb_t *memxms = &mem->u.xmslb;
  struct EMMMoveStruct r;
  int error;

  r.TransferLength = size;
  r.SourceHandle = 0;
  r.SourceOffset = (unsigned long)buf;
  r.DestHandle = memxms->handle;
  r.DestOffset = offset;

  r_move(&r, &error);
  
  if (error)
    return dtmemxmslb_err(ctl, F,
      "XMS_MoveEMB", error);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemxmslb_read(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemxmslb_read");
  dtmemxmslb_t *memxms = &mem->u.xmslb;
  struct EMMMoveStruct r;
  int error;

  r.TransferLength = size;
  r.SourceHandle = memxms->handle;
  r.SourceOffset = offset;
  r.DestHandle = 0;
  r.DestOffset = (unsigned long)buf;

  r_move(&r, &error);
  
  if (error)
    return dtmemxmslb_err(ctl, F,
      "XMS_MoveEMB", error);

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmemxmslb_init(						/* init a "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemxmslb_init");
  int error;
  unsigned int protocol, internal, hmem;
  unsigned int freemem, totmem;

  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being initialized"
    " pagesize %u npages %u",
    name, pagesize, npages);

  r_setup(&error);						/* init XMM library				    */
  if (error)			
    return dtmemxmslb_err(ctl, F,
      "XMS_Setup", error);

  r_version(&protocol,					/* query driver version 			*/
    &internal, &hmem, &error);
  if (error)
    return dtmemxmslb_err(ctl, F,
      "XMS_Version", error);

  r_freemem(&freemem, &totmem, &error);	/* query available memory 			*/
  if (error)
    return dtmemxmslb_err(ctl, F,
      "XMS_FreeMem", error);

  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
    "xmslb v%04x i0x%04x"
    " has %uK/%uK",
    protocol, internal, 
    freemem, totmem);

  DT_Q(dtmemsource_init,(ctl,			/* do common init for memsources	*/
    memsource, name,
    maxusage, pagesize, npages,
    DTMEMXMS_MAXUSAGE_DEFAULT,
    DTMEMXMS_PAGESIZE_DEFAULT,
    DTMEMXMS_NPAGES_DEFAULT));

  memsource->uninit = dtmemxmslb_uninit;
  memsource->alloc  = dtmemstd_alloc;	/* xms uses standard paging		    */
  memsource->open   = dtmemxmslb_open;
  memsource->resize = dtmemxmslb_resize;
  memsource->close  = dtmemxmslb_close;
  memsource->write  = dtmemxmslb_write;
  memsource->read   = dtmemxmslb_read;
  
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
