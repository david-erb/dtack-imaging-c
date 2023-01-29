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
DT_RCSID("src/low $RCSfile: memsourc.c,v $ $Revision: 1.2 $");
#include <dtack/mem.h>
#include <dtack/str.h>

/*..........................................................................
 * this function should not allocate anything
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemsource_init(						/* common init for all memsources    */
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages,
  long maxusage_default,
  unsigned int pagesize_default,
  unsigned int npages_default)
{
  DT_F("dtmemsource_init");

  if (strlen(name)+1 >					/* check for name length 			*/
      sizeof(memsource->name))
	return dt_err(ctl, F,
      "memsource name \"%s\""
      " too long (limit is %d)",
      name, sizeof(memsource->name)-1);
										/* clear the structure 				*/
  DT_MEMSET(memsource, 0,
    sizeof(*memsource));
  
  strcpy(memsource->name, name);

  if (maxusage == 0)					/* caller not limiting usage?		*/
    maxusage = maxusage_default;
  if (pagesize == 0)					/* caller not limiting page size?	*/
    pagesize = pagesize_default;
  if (npages == 0)						/* caller not limiting pages?	    */
    npages = npages_default;

  memsource->maxusage = maxusage;
  memsource->pagesize = pagesize;
  memsource->npages = npages;
  memsource->nallocs = 0;				/* nothing currently allocated		*/
  memsource->nextalloc = 0;				/* next allocation number		    */
  return DT_RC_GOOD;
}

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemsource_uninit(						/* common uninit for all memsources	*/
  dt_ctl_t *ctl,
  dtmemsource_t *memsource)
{
  DT_MEMSET(memsource, 0,
    sizeof(*memsource));
  return DT_RC_GOOD;
}

/*..........................................................................
 * uninit does not free any allocations which may be outstanding
 * outstanding allocations must be freed before the source is uninitted!
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemsource_report(		
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *f)
{
  int i;

  for (i=0; i<DT_CTL_MEMSOURCES; i++)
  if (memsource[i].alloc)				/* this one is initialized?			*/
	dt_dbg(ctl, f, DT_DBG_MASK_ANY,
      "%d. \"%s\" (%u,%u) next %d, blocks %d usage %ld/%ld",
	  i,
      memsource[i].name,
      memsource[i].pagesize,
      memsource[i].npages,
      memsource[i].nextalloc,
      memsource[i].nallocs,
      memsource[i].usage,
      memsource[i].maxusage);
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemsource_dbg(		
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *f,
  const char *desc)
{
  char alloc[32];

	dt_dbg(ctl, f, DT_DBG_MASK_ANY,
      "%s name \"%s\"",
      desc, memsource->name);

	dt_dbg(ctl, f, DT_DBG_MASK_ANY,
      "  pagesize %u npages %u next %d, nallocs %d usage %ld/%ld",
      memsource->pagesize,
      memsource->npages,
      memsource->nextalloc,
      memsource->nallocs,
      memsource->usage,
      memsource->maxusage);
		   
    if (memsource->alloc == NULL)
      strcpy(alloc, "NULL");
    else
    if (memsource->alloc == dtmemram_alloc)
      strcpy(alloc, "dtmemram_alloc");
    else
    if (memsource->alloc == dtmemstd_alloc)
      strcpy(alloc, "dtmemstd_alloc");
    else
      sprintf(alloc, "0x%08lx", (unsigned long)memsource->alloc);
		   
	dt_dbg(ctl, f, DT_DBG_MASK_ANY,
      "  alloc function %s",
      alloc);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmemsource_string(		
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  char *string,
  int max)
{
  DT_F("dtmemsource_string");
  *string = '\0';
  if (memsource->alloc)					/* this one is allocated? 			*/
    if (memsource->maxusage)			/* this one has an allocation cap? 	*/
      DT_Q(dtstr_printf,(ctl, 
        string, max,
        "%8s used %d chunks,"
        " %ld bytes of %ld (%ld%%)\n",
        memsource->name,
        memsource->nallocs,
        memsource->usage,
        memsource->maxusage,
        (100L*memsource->usage) /
        memsource->maxusage))
	else								/* no allocation cap? 				*/
      DT_Q(dtstr_printf,(ctl, 
        string, max,
        "%8s used %d chunks,"
        " %ld bytes\n",
        memsource->name,
        memsource->nallocs,
        memsource->usage))

  return DT_RC_GOOD;
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
