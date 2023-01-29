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
DT_RCSID("source/low $RCSfile: memparse.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmem_parse(							/* parse memory configuration		*/
  dt_ctl_t *ctl,
  const char *config,
  char *name,
  int maxlen,
  long *pmaxusage,
  unsigned int *ppagesize,
  unsigned int *pnpages,
  const char **end)
{
  DT_F("dtmem_parse");
  long maxusage = 0;
  unsigned long pagesize = 0;
  unsigned long npages = 0;
  int i;
  const char *t, *tlast;
  char *e;
  char *msg;
  char msg_space[32];

  DTCSTACK_HIGHWATER;

  for (i=0, t=config;					/* scan for name				    */
       *t != '\0' &&
       *t != DT_SEPARATOR_SPEC &&
       *t != DT_SEPARATOR_LIST;
       i++, t++)
  {
    if (i > maxlen-2)
    {
      msg = "name too long";
      goto error;
    }
    name[i] = *t;
  }
  name[i] = '\0';						/* terminate name string		    */
  if (*t == '\0' ||
      *t == DT_SEPARATOR_LIST)
    goto done;

  tlast = t + 1;
  maxusage = strtol(tlast, &e, 0);		/* extract maxusage				    */
  t = e;
  if (t == tlast)
    maxusage = 0; 
  if (*t == 'k' || *t == 'K')			/* given in K?					    */
  {
    maxusage *= 1024L;
    t++;
  }
  else
  if (*t == 'm' || *t == 'M')			/* given in M?					    */
  {
    maxusage *= 1024L * 1024L;
    t++;
  }
  if (*t == '\0' || 
      *t == DT_SEPARATOR_LIST)
    goto done;
  if (*t != DT_SEPARATOR_SPEC)
    goto delim;

  tlast = t + 1;
  pagesize = strtol(tlast, &e, 0);		/* extract pagesize				    */
  t = e;
  if (t == tlast)
    pagesize = 0; 
  if (*t == '\0' || 
      *t == DT_SEPARATOR_LIST)
    goto done;
  if (*t != DT_SEPARATOR_SPEC)
    goto delim;
  if (pagesize != (unsigned int)pagesize)
  {
    msg = "pagesize too large";
    goto error;
  }

  tlast = t + 1;
  npages = strtol(tlast, &e, 0);		/* extract number of pages			*/
  t = e;
  if (t == tlast)
    npages = 0; 
  if (*t == '\0' || 
      *t == DT_SEPARATOR_LIST)
    goto done;
  if (*t != DT_SEPARATOR_SPEC)
    goto delim;
  if (npages != (unsigned int)npages)
  {
    msg = "npages too large";
    goto error;
  }

done:
  if (*t == DT_SEPARATOR_LIST)
    t++;

  *pmaxusage = maxusage;
  *ppagesize = (unsigned int)pagesize;
  *pnpages   = (unsigned int)npages;
  *end = t;

  dt_dbg(ctl, F, DT_DBG_MASK_PAGE,
    "\"%s\" maxusage %ld, pagesize %u, npages %u",
    name, *pmaxusage, *ppagesize, *pnpages);

  return DT_RC_GOOD;

delim:
  if (isprint(*t))
    sprintf(msg = msg_space, "invalid delimeter '%c'", *t);
  else
    sprintf(msg = msg_space, "invalid delimeter '0x%02x'", *t);

error:
  return dt_err(ctl, F, "error in mem config \"%s\" at char %u: %s",
    config,
    (unsigned int)(t - config),
    msg);
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
