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

#if DTPRJ_XMSIF != DT_1

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemxmsif_init(						/* init an "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemxmsif_init");
  return dt_err_notlinked(ctl,
    F, "xmsif (by James W. Birdsall)");
}

#else

#include <dtack/xmsif.h>

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e dtmemxmsif_err(
  dt_ctl_t *ctl,
  const char *f,
  const char *xmmfuncname)
{
  const char *s;
  switch(_XMMerror)
  {
#define CASE(ERROR) case ERROR: s = #ERROR; break;	
    CASE(XMM_NOINIT          )
    CASE(XMM_UMBHUGE         )
    CASE(XMM_BADPTR          )
    CASE(XMM_ELTOOBIG        )
    CASE(XMM_SKTOOBIG        )
    CASE(XMM_BADVERS         )

    CASE(XMM_UNIMP           )
    CASE(XMM_VDISK           )
    CASE(XMM_A20ERROR        )
    CASE(XMM_GENERROR        )
    CASE(XMM_UNRECERROR      )

    CASE(XMM_NOHMA           )
    CASE(XMM_HMAUSED         )
    CASE(XMM_HMATOOBIG       )
    CASE(XMM_HMANOALLOC      )
    CASE(XMM_A20STILLEN      )

    CASE(XMM_NOFREEX         )
    CASE(XMM_NOFREEXHAN      )
    CASE(XMM_BADXHAN         )
    CASE(XMM_BADSRCHAN       )
    CASE(XMM_BADSRCOFF       )
    CASE(XMM_BADDESTHAN      )
    CASE(XMM_BADDESTOFF      )
    CASE(XMM_BADLENGTH       )
    CASE(XMM_COPYOVERLAP     )
    CASE(XMM_PARITY          )
    CASE(XMM_NOLOCK          )
    CASE(XMM_LOCKED          )
    CASE(XMM_TOOMANYLOCKS    )
    CASE(XMM_LOCKFAIL        )

    CASE(XMM_UMBSMALLER      )
    CASE(XMM_NOFREEUMB       )
    CASE(XMM_BADUMBHAN       )
    default: s = "unknown";
  }

  return dt_err(ctl, f,
    "XMM error %s (0x%02x) in %s",
    s, _XMMerror, xmmfuncname);
}

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

static
dt_rc_e
dtmemxmsif_uninit(						/* uninit an "xms" memsource   	    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_F("dtmemxmsif_uninit");
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
dtmemxmsif_open(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  const char *name,
  int seqnum,
  long total)
{
  DT_F("dtmemxmsif_open");
  dtmemxmsif_t *memxms = &mem->u.xmsif;
  unsigned long coreleft;
  unsigned long allcoreleft;
  unsigned long coreleft_after;
  unsigned long allcoreleft_after;

  coreleft = XMMcoreleft();
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMcoreleft");

  allcoreleft = XMMallcoreleft();
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMallcoreleft");

  if (coreleft < total)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "need %lu bytes,"
      " but xms only has %luK/%luK",
      total, 
      coreleft / 1024LU,
      allcoreleft / 1024LU);
    mem->gotspace = 0;
  }
  else
  {
    mem->type = DTMEM_TYPE_XMSIF;		/* remember our type			    */
    memxms->handle = XMMalloc(total);
    if (_XMMerror)
      return dtmemxmsif_err(ctl, F,
        "XMMalloc");

    coreleft_after = XMMcoreleft();
    if (_XMMerror)
      return dtmemxmsif_err(ctl, F,
        "XMMcoreleft");

    allcoreleft_after = XMMallcoreleft();
    if (_XMMerror)
      return dtmemxmsif_err(ctl, F,
        "XMMallcoreleft");

    mem->gotspace = 1;
	dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
      "new 0x%04x size %ld"
      " free now %luK/%luK", 
      memxms->handle, total, 
      coreleft_after / 1024LU,
      allcoreleft_after / 1024LU);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemxmsif_close(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem)
{
  DT_F("dtmemxmsif_close");
  dtmemxmsif_t *memxms = &mem->u.xmsif;
  
  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
    "freeing handle 0x%04x", 
    memxms->handle);

  XMMfree(memxms->handle);
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMfree");

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

static
dt_rc_e
dtmemxmsif_resize(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long total)
{
  DT_F("dtmemxmsif_resize");

#define REALLOC_DOES_NOT_WORK
# ifdef REALLOC_DOES_NOT_WORK
  {
    dtmem_t newmem = *mem;
    long size = DT_MIN(total,			/* may be shrinking 				*/
      mem->total);

    DT_Q(dtmemxmsif_open,(ctl, &newmem,	/* get space for new total 			*/
      mem->name, 0, total));

    _XMMcopy(size,						/* copy from old space to new 		*/
      mem->u.xmsif.handle, 0,
      newmem.u.xmsif.handle, 0);

    if (_XMMerror)
	{
	  dt_rc_e rc;
      rc = dtmemxmsif_err(ctl, F,
        "_XMMcopy");
      DT_I(dtmemxmsif_close,(ctl,		/* free new memory 					*/
		&newmem));
	  return rc;
	}

    DT_Q(dtmemxmsif_close,(ctl, mem));	/* free old memory 					*/
	
	*mem = newmem;
  }
# else
  {
	dtmemxmsif_t *memxms = &mem->u.xmsif;
    unsigned long coreleft;
    int retry;

    coreleft = XMMcoreleft();
    if (_XMMerror)
      return dtmemxmsif_err(ctl, F,
        "XMMcoreleft");

    if (coreleft < total-mem->total)
    {
      dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
        "need %lu more bytes, but XMS manager only has %lu",
        total-mem->total, coreleft);
      mem->gotspace = 0;
    }
    else
    for (retry=0; retry<2; retry++)		/* try twice 						*/
    {
      int newhandle = 0;
      newhandle = XMMrealloc(			/* resize the space				    */
        memxms->handle, total);
	  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
        "old 0x%04x"
        " new 0x%04x size %ld"
        " core %lu %02x", 
        memxms->handle,
        newhandle, total, coreleft,
        _XMMerror);
      if (_XMMerror)					/* resizing failed?				    */
	    if (retry == 0)					/* first try? 						*/
          continue;						/* try again 						*/
  	    else							/* second try? 						*/
          return dtmemxmsif_err(ctl, F,	/* give up 							*/
            "XMMrealloc");
      memxms->handle = newhandle;		/* we have a new handle			    */
      mem->gotspace = 1;
	  break;							/* can quit retrying			    */
    }
  }
#endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemxmsif_write(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemxmsif_write");
  dtmemxmsif_t *memxms = &mem->u.xmsif;

  XMMcopyto(size,
    (unsigned char *)buf, memxms->handle, offset);
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMcopyto");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemxmsif_read(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemxmsif_read");
  dtmemxmsif_t *memxms = &mem->u.xmsif;

  XMMcopyfrom(size,
    memxms->handle, offset, (unsigned char *)buf);
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMcopyfrom");

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemxmsif_init(						/* init an "xms" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemxmsif_init");
  unsigned long coreleft;
  unsigned long allcoreleft;
  
  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being initialized"
    " pagesize %u npages %u",
    name, pagesize, npages);

  XMMlibinit();							/* init XMM library				    */
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMlibinit");

  DT_Q(dtmemsource_init,(ctl,			/* do common init for memsources	*/
    memsource, name,
    maxusage, pagesize, npages,
    DTMEMXMS_MAXUSAGE_DEFAULT,
    DTMEMXMS_PAGESIZE_DEFAULT,
    DTMEMXMS_NPAGES_DEFAULT));

  coreleft = XMMcoreleft();
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMcoreleft");

  allcoreleft = XMMallcoreleft();
  if (_XMMerror)
    return dtmemxmsif_err(ctl, F,
      "XMMallcoreleft");

  dt_dbg(ctl, F, DT_DBG_MASK_MEMX,
    "%s (%s %s) has %luK/%luK",
	xmsif_vers_vers,
	xmsif_vers_date,
	xmsif_vers_time,
    coreleft / 1024LU,
    allcoreleft / 1024LU);

  memsource->uninit = dtmemxmsif_uninit;
  memsource->alloc  = dtmemstd_alloc;	/* xms uses standard paging		    */
  memsource->open   = dtmemxmsif_open;
  memsource->resize = dtmemxmsif_resize;
  memsource->close  = dtmemxmsif_close;
  memsource->write  = dtmemxmsif_write;
  memsource->read   = dtmemxmsif_read;
  
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
