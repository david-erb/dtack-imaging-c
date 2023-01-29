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
DT_RCSID("src/low $RCSfile: mem.c,v $ $Revision: 1.2 $");
#include <dtack/cstack.h>

static unsigned long stack0 = 0;
static unsigned long stack_highwater = 0;
static unsigned long deferred_highwater = 0;
static char deferred_caller[64] = {0};

/*..........................................................................
 * mark stack base
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_init(
  dt_ctl_t *ctl)
{
  void *p = (void *)&ctl;
  stack0 = DT_PTR_TO_ULONG(p);
  stack_highwater = stack0;
  deferred_caller[0] = '\0';
#ifdef NONO
  printf("%s: stack0 0x%08lx highwater 0x%08lx\n",
    "dtcstack_init", stack0, stack0-stack_highwater);
#endif
}

/*..........................................................................
 * mark current location if high water
 * print nothing
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_mark(
  dt_ctl_t *ctl)
{
  void *p = (void *)&ctl;
  unsigned long stack1 = DT_PTR_TO_ULONG(p);
  if (stack0 == 0 || ctl == NULL)
    return;
  if (stack1 < stack_highwater)
	stack_highwater = stack1;
}

/*..........................................................................
 * mark current location if high water
 * print nothing
 * save caller for deferred printing
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_mark2(
  dt_ctl_t *ctl,
  const char *F)
{
  void *p = (void *)&ctl;
  unsigned long stack1 = DT_PTR_TO_ULONG(p);
  if (stack0 == 0 || ctl == NULL)
    return;
  if (stack1 < stack_highwater)
  {
	const char *f = F && strrchr(F, ' ')? strrchr(F, ' ')+1: F;
	strncpy(deferred_caller, 
      f? f: "unknown",
      sizeof(deferred_caller)-1);
	deferred_caller[sizeof(deferred_caller)-1] = '\0';
	deferred_highwater = stack_highwater;
	stack_highwater = stack1;
  }
}

/*..........................................................................
 * mark current location if high water
 * print current stack use if this is high water and cstack flag is on
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_query(
  dt_ctl_t *ctl,
  unsigned long *pstack1,
  unsigned long *pstack_highwater)
{
  void *p = (void *)&ctl;
  unsigned long stack1 = DT_PTR_TO_ULONG(p);
  *pstack1 = stack0-stack1;
  *pstack_highwater = stack0-stack_highwater;
}

/*..........................................................................
 * mark current location if high water
 * print current stack use if this is high water and cstack flag is on
 *..........................................................................*/

static
void
do_print(
  dt_ctl_t *ctl,
  const char *F,
  long mask,
  const char *what,
  unsigned long stack1,
  unsigned long stack_highwater1)
{
  dt_dbg(ctl, F, mask,
    "stack %s base 0x%08lx use %lu old highwater %lu",
	what,
    stack0, stack0-stack1, stack0-stack_highwater1);
}

/*..........................................................................
 * mark current location if high water
 * print current stack use if given flag is on
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_dbg(
  dt_ctl_t *ctl,
  const char *F,
  long mask)
{
  void *p = (void *)&ctl;
  unsigned long stack1 = DT_PTR_TO_ULONG(p);
  if (stack0 == 0 || ctl == NULL)
    return;

  do_print(ctl, F, mask,
    "usage", stack1, stack_highwater);
  
  if (stack1 < stack_highwater)
	stack_highwater = stack1;
}

/*..........................................................................
 * mark current location if high water
 * print current stack use if this is high water and cstack flag is on
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_highwater(
  dt_ctl_t *ctl,
  const char *F)
{
  void *p = (void *)&ctl;
  unsigned long stack1 = DT_PTR_TO_ULONG(p);

  if (stack0 == 0 || ctl == NULL)
    return;

  if (deferred_caller[0] != '\0')
  {
	do_print(ctl, deferred_caller, DT_DBG_MASK_CSTACK,
      "deferred", stack_highwater, deferred_highwater);
	deferred_caller[0] = '\0';
  }

  if (stack1 < stack_highwater)
  {
	do_print(ctl, F, DT_DBG_MASK_CSTACK,
      "highwater", stack1, stack_highwater);
	stack_highwater = stack1;
  }
}

/*..........................................................................
 * mark current location if high water
 * print current stack use always
 *..........................................................................*/

void
DTCONFIG_API1
dtcstack_say(
  dt_ctl_t *ctl,
  const char *F,
  long mask)
{
  void *p = (void *)&ctl;
  unsigned long stack1 = DT_PTR_TO_ULONG(p);
  if (stack0 == 0 || ctl == NULL)
    return;

#ifdef NONO
  dt_say(ctl, F,
    FORMAT, "usage", ARGS);
#endif
  if (stack1 < stack_highwater)
	stack_highwater = stack1;
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
