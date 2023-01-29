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

DT_RCSID("source/low $RCSfile: mem_ems.c,v $ $Revision: 1.2 $");

#if DTPRJ_EMSIF != DT_1

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemems_init(							/* init a "ems" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemems_init");
  return dt_err_notlinked(ctl,
    F, "Expanded Memory (EMS)");  
}

#else

#include <dtack/emsif.h>

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e 
dtmemems_err(
  dt_ctl_t *ctl,
  const char *f,
  const char *emmfuncname)
{
  return dt_err(ctl, f,
    "EMM error 0x%02x in %s",
    _EMMerror, emmfuncname);
}

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

static
dt_rc_e
dtmemems_uninit(						/* uninit an "ems" memsource   	    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_F("dtmemems_uninit");
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
dtmemems_open(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  const char *name,
  int seqnum,
  long total)
{
  DT_F("dtmemems_open");
  dtmemems_t *memems = &mem->u.ems;
  unsigned long coreleft;

  coreleft = EMMcoreleft();
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMcoreleft");

  if (coreleft < total)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "need %lu bytes, but EMS manager only has %lu",
      total, coreleft);
    mem->gotspace = 0;
  }
  else
  {
    mem->type = DTMEM_TYPE_EMS;			/* remember our type			    */
    memems->handle = EMMalloc(total);
    if (_EMMerror)
      return dtmemems_err(ctl, F,
        "EMMalloc");
    mem->gotspace = 1;
    mem->total = total;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

static
dt_rc_e
dtmemems_resize(						/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long total)
{
  DT_F("dtmemems_resize");
  dtmemems_t *memems = &mem->u.ems;
  unsigned long coreleft;

  coreleft = EMMcoreleft();
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMcoreleft");

  if (coreleft < total-mem->total)
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
      "need %lu more bytes, but EMS manager only has %lu",
      total-mem->total, coreleft);
    mem->gotspace = 0;
  }
  else
  {
    int newhandle;
    newhandle = EMMrealloc(				/* resize the space				    */
      memems->handle, total);
    if (_EMMerror)						/* resizing failed?				    */
      return dtmemems_err(ctl, F,
        "EMMalloc");
    memems->handle = newhandle;			/* we have a new handle			    */
    mem->gotspace = 1;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemems_close(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem)
{
  DT_F("dtmemems_close");
  dtmemems_t *memems = &mem->u.ems;

  EMMfree(memems->handle);
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMfree");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemems_write(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemems_write");
  dtmemems_t *memems = &mem->u.ems;

  EMMcopyto(size,
    (unsigned char *)buf, memems->handle, offset);
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMcopyto");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtmemems_read(							/*  */
  dt_ctl_t *ctl,
  dtmem_t *mem,
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dtmemems_read");
  dtmemems_t *memems = &mem->u.ems;

  EMMcopyfrom(size,
    memems->handle, offset, (unsigned char *)buf);
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMcopyfrom");

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemems_init(							/* init a "ems" memsource		    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtmemems_init");
  unsigned long coreleft;
  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" being initialized pagesize %u npages %u",
    name, pagesize, npages);

  EMMlibinit();							/* init EMM library				    */
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMlibinit");

  DT_Q(dtmemsource_init,(ctl,			/* do common init for memsources	*/
    memsource, name,
    maxusage, pagesize, npages,
    DTMEMEMS_MAXUSAGE_DEFAULT,
    DTMEMEMS_PAGESIZE_DEFAULT,
    DTMEMEMS_NPAGES_DEFAULT));

  coreleft = EMMcoreleft();
  if (_EMMerror)
    return dtmemems_err(ctl, F,
      "EMMcoreleft");

  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "EMS memory manager has %lu bytes available",
    coreleft);

  memsource->uninit = dtmemems_uninit;
  memsource->alloc  = dtmemstd_alloc;	/* ems uses standard paging		    */
  memsource->open   = dtmemems_open;
  memsource->resize = dtmemems_resize;
  memsource->close  = dtmemems_close;
  memsource->write  = dtmemems_write;
  memsource->read   = dtmemems_read;
  
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
