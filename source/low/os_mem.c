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
  CHANGES
  19990723 DE pre-filter debug
  19980718 DE changed free to DT_FREE
 *..........................................................................*/



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
#include <dtack/os.h>
#include <dtack/heap.h>

DT_RCSID("source/low $RCSfile: os_mem.c,v $ $Revision: 1.2 $");

#define LIMIT_CHECK(CTL, HEAP, SIZE, F) \
  (DT_RC_GOOD == dtheap_check_limit(CTL, HEAP, SIZE, F))

#if DTPRJ_STRATOS_MEMCHECK >= 1			/* using MemCheck?					*/
										/* rely on mc_set_location()		*/
										/* in the DT_Q() family of macros	*/
										/* to set source location		    */
#  undef malloc
#  undef free
#  undef realloc
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_strdup(							/* duplicate string					*/
  dt_ctl_t *ctl,
  const char *src,
  char **dest)
{
  DT_F("dtos_strdup");
  char tmp[32];
  int l = sizeof(tmp) - 4;
  int i;

  for (i=0; i<l && src[i]; i++)
    if (isprint(src[i]))
      tmp[i] = src[i];
    else
      tmp[i] = '.';

  if (i == l)
  {
    tmp[i++] = '.';
    tmp[i++] = '.';
    tmp[i++] = '.';
  }

  tmp[i] = '\0';

  l = strlen(src) + 1;

  DT_Q(dtos_malloc2,(ctl,				/* get space for new string 		*/
    (void **)dest, l, F, tmp));

  DT_MEMCPY(*dest, src, l);				/* copy contents to new string 		*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_strdup3(							/* duplicate/contatenate strings	*/
  dt_ctl_t *ctl,
  const char *src1,
  const char *src2,
  const char *src3,
  char **dest)
{
  DT_F("dtos_strdup");
  char tmp[32];
  int l = sizeof(tmp) - 4;
  int i;

  for (i=0; i<l && src1[i]; i++)
    if (isprint(src1[i]))
      tmp[i] = src1[i];
    else
      tmp[i] = '.';

  if (i == l)
  {
    tmp[i++] = '.';
    tmp[i++] = '.';
    tmp[i++] = '.';
  }

  tmp[i] = '\0';

  l = strlen(src1) + strlen(src2) + strlen(src3) + 1;

  DT_Q(dtos_malloc2,(ctl,
    (void **)dest, l, F, tmp));

  strcpy(*dest, src1);
  strcat(*dest, src2);
  strcat(*dest, src3);

  return DT_RC_GOOD;
}


#ifdef  mc_set_location
#undef  mc_set_location
#endif
#ifdef  _mc_set_location
#undef  _mc_set_location
#endif

#define mc_set_location()
#define _mc_set_location(FILE, LINE)

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_malloc3(							/* malloc space, NULL if fail		*/
  dt_ctl_t *ctl,
  void **ptr,
  const long size,
  const char *caller,
  const char *desc)
{
  DT_F("dtos_malloc3");
  void *p;

  if (size <= 0)
    return dt_err(ctl, caller,
      "malloc(%ld) for %s has an invalid amount",
        size, desc);

  if (size > DTCONFIG_MALLOC_MAX)
    return dt_err(ctl, caller,
      "malloc(%ld) for %s is more than DTCONFIG_MALLOC_MAX %lu",
        size, desc, DTCONFIG_MALLOC_MAX);

  if (ctl != NULL)						/* we have environment control? 	*/
    DT_Q(dtheap_check,(ctl,				/* check heap integrity 			*/
      ctl->heap, caller));

  if (ctl != NULL &&
      ctl->heap != NULL &&
      !LIMIT_CHECK(ctl, 
         ctl->heap, size, caller))
    p = NULL;
  else
    p = malloc((size_t)size);

  if (p != NULL &&
      DT_PTR_TO_ULONG(p)+size > 
	  DTCONFIG_MEM_TOP)
  {
    DT_FREE(p);
    return dt_err(ctl, caller,
      "malloc(%ld) gave ptr 0x%08lx-0x%08lx above mem top 0x%08lx for %s",
      size, 
      DT_PTR_TO_ULONG(p),
      DT_PTR_TO_ULONG(p) + size - 1,
      DTCONFIG_MEM_TOP,
      desc);
  }

  if (DT_DBG(ctl, DT_DBG_MASK_MEMTRACE))
  dt_dbg(ctl, caller,
    DT_DBG_MASK_MEMTRACE,
    "malloc(%ld) = 0x%08lx-0x%08lx for %s",
    size, 
    DT_PTR_TO_ULONG(p),
    DT_PTR_TO_ULONG(p) + size - 1,
    desc); 
    
  *ptr = p;

  if (p != NULL &&						/* allocation succeeded? 			*/
	  ctl != NULL &&					/* we have environment control? 	*/
      ctl->heap != NULL)				/* we are doing heap monitoring? 	*/
    DT_Q(dtheap_track_alloc,(ctl,		/* track this allocation 			*/
        ctl->heap, 
        p, size, caller, desc));

  return DT_RC_GOOD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_malloc2(							/* malloc space, return in ptr		*/
  dt_ctl_t *ctl,
  void **ptr,
  const long size,
  const char *caller,
  const char *desc)
{
  DT_F("dtos_malloc2");
  void *p;
  if (size > DTCONFIG_MALLOC_MAX)
    return dt_err(ctl, caller, 
      "malloc(%ld) for %s is more than DTCONFIG_MALLOC_MAX %lu",
        size, desc, DTCONFIG_MALLOC_MAX);

  if (ctl != NULL)						/* we have environment control? 	*/
    DT_Q(dtheap_check,(ctl,				/* check heap integrity 			*/
      ctl->heap, caller));

  if (ctl != NULL &&
      ctl->heap != NULL &&
      !LIMIT_CHECK(ctl, 
         ctl->heap, size, caller))
    p = NULL;
  else
    p = malloc((size_t)size);

  if (p == NULL)
  {
    dt_rc_e rc = dt_err(ctl, caller,
      "malloc(%lu) failed for %s"
      " (ctl->heap 0x%08lx)", 
      size, desc, 
      DT_PTR_TO_ULONG(ctl->heap));

	if (DT_DBG(ctl,						/* MEMSTATS debugging is on?	    */
        DT_DBG_MASK_MEMSTATS))
      DT_I(dt_heapmon_report,(ctl,		/* spit out report				    */
        (char *)caller));
    return rc;
  }

  if (DT_PTR_TO_ULONG(p)+size >			/* this really shouldn't happen	    */
	  DTCONFIG_MEM_TOP)
  {
    DT_FREE(p);
    return dt_err(ctl, caller,
      "malloc(%ld) gave ptr"
      " 0x%08lx-0x%08lx above"
      " mem top 0x%08lx for %s",
      size, 
      DT_PTR_TO_ULONG(p),
      DT_PTR_TO_ULONG(p) + size - 1,
      DTCONFIG_MEM_TOP,
      desc);
  }

  if (DT_DBG(ctl, DT_DBG_MASK_MEMTRACE))
  dt_dbg(ctl, caller, 
    DT_DBG_MASK_MEMTRACE,
    "malloc(%lu) = 0x%08lx-0x%08lx for %s",
    size, 
    DT_PTR_TO_ULONG(p),
    DT_PTR_TO_ULONG(p) + size - 1,
    desc); 
    
  *ptr = p;

  if (ctl != NULL &&					/* we have environment control? 	*/
      ctl->heap != NULL)				/* we are doing heap monitoring? 	*/
    DT_Q(dtheap_track_alloc,(ctl,		/* track this allocation 			*/
      ctl->heap, *ptr, size, 
      caller, desc));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_malloc(							/* malloc space, return in ptr		*/
  dt_ctl_t *ctl,
  void **ptr,
  const long size)
{
  DT_F("dtos_malloc");
  void *p;
  if (size > DTCONFIG_MALLOC_MAX)
    return dt_err(ctl, F, 
      "malloc(%lu) is more than DTCONFIG_MALLOC_MAX %lu",
        size, DTCONFIG_MALLOC_MAX);

  if (ctl != NULL)						/* we have environment control? 	*/
    DT_Q(dtheap_check,(ctl,				/* check heap integrity 			*/
      ctl->heap, F));

  if (ctl != NULL &&					/* we have environment control? 	*/
      ctl->heap != NULL &&				/* we are doing heap monitoring? 	*/
      !LIMIT_CHECK(ctl,					/* we have hit artificial limit? 	*/
         ctl->heap, size, F))
    p = NULL;
  else
    p = malloc((size_t)size);

  if (p == NULL)						/* memory allocation failed? 		*/
  {
    dt_rc_e rc = dt_err(ctl, F,			/* complain about it 				*/
      "malloc(%lu)", size); 
	if (DT_DBG(ctl,						/* MEMSTATS debugging is on?	    */
        DT_DBG_MASK_MEMSTATS))
      DT_I(dt_heapmon_report,(ctl, F));	/* spit out report				    */
    return rc;
  }

  if (DT_PTR_TO_ULONG(p)+size >			/* this really shouldn't happen	    */
	  DTCONFIG_MEM_TOP)
  {
    DT_FREE(p);
    return dt_err(ctl, F,
      "malloc(%ld) gave ptr"
      " 0x%08lx-0x%08lx above"
      " mem top 0x%08lx",
      size, 
      DT_PTR_TO_ULONG(p),
      DT_PTR_TO_ULONG(p) + size - 1,
      DTCONFIG_MEM_TOP);
  }

  if (DT_DBG(ctl, DT_DBG_MASK_MEMTRACE))
  dt_dbg(ctl, F,						/* debug message for this allocation */
    DT_DBG_MASK_MEMTRACE,
    "malloc(%lu) = 0x%08lx-0x%08lx",
    size, 
    DT_PTR_TO_ULONG(p),
    DT_PTR_TO_ULONG(p) + size - 1);
    
  *ptr = p;								/* return allocated pointer 		*/

  if (ctl != NULL &&					/* we have environment control? 	*/
      ctl->heap != NULL)				/* we are doing heap monitoring? 	*/
    DT_Q(dtheap_track_alloc,(ctl,		/* track this allocation 			*/
      ctl->heap, 
      p, size, F, "unknown"));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_realloc3(							/* realloc space, NULL if fail		*/
  dt_ctl_t *ctl,
  void *oldptr,
  void **newptr,
  const long size,
  const char *caller,
  const char *desc)
{
  DT_F("dtos_realloc3");
  void *p;
  if (size <= 0)
    return dt_err(ctl, caller, 
      "realloc(0x%08lx, %ld) for %s has an invalid amount",
        DT_PTR_TO_ULONG(oldptr), size, desc);

  if (size > DTCONFIG_MALLOC_MAX)
    return dt_err(ctl, caller, 
      "realloc(0x%08lx, %ld) for %s"
      " is more than DTCONFIG_MALLOC_MAX %lu",
        DT_PTR_TO_ULONG(oldptr), size, desc,
        DTCONFIG_MALLOC_MAX);

  if (ctl != NULL)						/* we have environment control? 	*/
    DT_Q(dtheap_check,(ctl,				/* check heap integrity 			*/
      ctl->heap, caller));

  if (ctl != NULL &&
      ctl->heap != NULL &&
      !LIMIT_CHECK(ctl, ctl->heap, 
         size, caller))
    p = NULL;
  else
    p = realloc(oldptr, (size_t)size);	/* use native call to do realloc    */

  if (DT_PTR_TO_ULONG(p)+size > DTCONFIG_MEM_TOP)
  {
    DT_FREE(p);
    return dt_err(ctl, caller,
      "remalloc(0x%08lx, %ld)"
      " gave ptr 0x%08lx-0x%08lx above mem top 0x%08lx for %s",
      DT_PTR_TO_ULONG(oldptr), size, 
      DT_PTR_TO_ULONG(p),
      DT_PTR_TO_ULONG(p) + size - 1,
      DTCONFIG_MEM_TOP,
      desc);
  }

  if (DT_DBG(ctl, DT_DBG_MASK_MEMTRACE))
  dt_dbg(ctl, caller, 
    DT_DBG_MASK_MEMTRACE,
    "realloc(0x%08lx, %ld) ="
    " 0x%08lx-0x%08lx for %s",
    DT_PTR_TO_ULONG(oldptr), size, 
    DT_PTR_TO_ULONG(p),
    DT_PTR_TO_ULONG(p) + size - 1,
    desc); 
    
  *newptr = p;

  if (p != NULL &&						/* allocation succeeded? 			*/
	  ctl != NULL &&					/* we have environment control? 	*/
      ctl->heap != NULL)				/* we are doing heap monitoring? 	*/
  {
    DT_Q(dtheap_track_free,(ctl,		/* free old pointer				    */
      ctl->heap, oldptr));
    DT_Q(dtheap_track_alloc,(ctl,		/* add new pointer				    */
      ctl->heap, 
      p, size, caller, desc));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_free(								/* free allocated space				*/
  dt_ctl_t *ctl,
  void *ptr)
{
  DT_F("dtos_free");

  if (DT_DBG(ctl, DT_DBG_MASK_MEMTRACE))
  dt_dbg(ctl, F, 
    DT_DBG_MASK_MEMTRACE,
    "free(0x%08lx)", DT_PTR_TO_ULONG(ptr));

  DT_FREE(ptr);

  if (ctl != NULL)						/* we have environment control? 	*/
    DT_Q(dtheap_check,(ctl,				/* check heap integrity 			*/
      ctl->heap, F));

  if (ctl != NULL &&
      ctl->heap != NULL)
    DT_Q(dtheap_track_free,(ctl, 
      ctl->heap, ptr));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtos_free2(								/* free allocated space				*/
  dt_ctl_t *ctl,
  void *ptr,
  const char *caller,
  const char *desc)
{
  DT_F("dtos_free2");
  if (DT_DBG(ctl, DT_DBG_MASK_MEMTRACE))
  dt_dbg(ctl, caller, 
    DT_DBG_MASK_MEMTRACE,
    "free(0x%08lx) for %s", 
    DT_PTR_TO_ULONG(ptr), desc);

  DT_FREE(ptr);

  if (ctl != NULL)						/* we have environment control? 	*/
    DT_Q(dtheap_check,(ctl,				/* check heap integrity 			*/
      ctl->heap, caller));

  if (ctl != NULL &&
      ctl->heap != NULL)
    DT_Q(dtheap_track_free,(ctl, 
      ctl->heap, ptr));

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
