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
DT_RCSID("source/high $RCSfile: gen_kern.c,v $ $Revision: 1.1 $");
#include <dtack/str.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_gen_kernel(
  dt_ctl_t *ctl,
  dtimage_t *kernel,
  const char *kernel_name)
{
  DT_F("dtimage_gen_kernel");
  int choice;
  DT_Q(dtstr_choice,(ctl, dtimage_gen_kernel_names, kernel_name, &choice));

  if (choice == -1)
  {
    return dt_err(ctl, F,
      "Sorry, kernel name \"%s\" is not understood.", 
      kernel_name);
  }

  if (choice < -1)
  {
    return dt_err(ctl, F,
      "Sorry, kernel name \"%s\" is ambiguous (%d matches).",
      kernel_name, -choice);
  }

  if (!strcmp(dtimage_gen_kernel_names[choice], "average"))
  {
	DT_Q(dtimage_constant,(ctl, kernel, 1.0));
  }
  else
  if (!strcmp(dtimage_gen_kernel_names[choice], "laplace"))
  {
	DT_Q(dtimage_constant,(ctl, kernel, 0.0));
	DT_Q(dtimage_set_pixel,(ctl, kernel, kernel->xe/2,            0, -1));
	DT_Q(dtimage_set_pixel,(ctl, kernel, kernel->xe/2, kernel->ye-1, -1));
	DT_Q(dtimage_set_pixel,(ctl, kernel, kernel->xe/2, kernel->ye/2,  4));
	DT_Q(dtimage_set_pixel,(ctl, kernel,            0, kernel->ye/2, -1));
	DT_Q(dtimage_set_pixel,(ctl, kernel, kernel->xe-1, kernel->ye/2, -1));
  }
  else
  if (!strcmp(dtimage_gen_kernel_names[choice], "highpass"))
  {
	DT_Q(dtimage_constant,(ctl, kernel, -1.0));
	DT_Q(dtimage_set_pixel,(ctl, kernel, 
      kernel->xe/2, 
      kernel->ye/2,
      kernel->xe * kernel->ye - 1));
  }
  else
  if (!strcmp(dtimage_gen_kernel_names[choice], "highboost"))
  {
	DT_Q(dtimage_constant,(ctl, kernel, -1.0));
	DT_Q(dtimage_set_pixel,(ctl, kernel, 
      kernel->xe/2, 
      kernel->ye/2,
      kernel->xe * kernel->ye));
  }

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
