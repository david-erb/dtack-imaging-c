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
  19991217 DE changed some long's to 4-byte int's in favor of 64-bit CPU
 *..........................................................................*/

/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: conv_l.c,v $ $Revision: 1.1 $");
#include <dtack/os.h>
#include <dtack/mem.h>

typedef struct {
  dtntype_s4_t kvalue;
  dtxy_t dx;
  dtxy_t dy;
} s4_list_t;

/*..........................................................................
 * compare two list elements
 *..........................................................................*/

static
int
s4_list_compare(
  void *e1,
  void *e2,
  unsigned int size)
{
  const dtntype_s4_t k1 = ((s4_list_t *)e1)->kvalue;
  const dtntype_s4_t k2 = ((s4_list_t *)e2)->kvalue;
  if (k1 == k2)
    return 0;
  else
  if (k1 > k2)
    return 1;
  else
    return -1;
}

/*..........................................................................*/
static
dt_rc_e
dtimage_convolve_s4_sdmk(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_s4_sdmk");
  
#  define KTYPE dt_ntype_s4_t
#  define KTYPE_E DT_NTYPE_S4
#  define KTYPE_LIST_T s4_list_t
#  define KTYPE_LIST_COMPARE s4_list_compare
  
#  include <convolve.i>
  
#  undef KTYPE
#  undef KTYPE_E
#  undef KTYPE_LIST_T
#  undef KTYPE_LIST_COMPARE
  
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_convolve_s4(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  dtimage_t *output)
{
  DT_F("dtimage_convolve_s4");

  DT_Q(dtimage_convolve_s4_sdmk,(ctl,
    input,
    kernel, kxsize, kysize,
    output));

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
