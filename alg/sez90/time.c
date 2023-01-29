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

DT_RCSID("time $RCSfile: time.c,v $ $Revision: 1.1 $");

#include <dt_sez90.h>
#include <dtack/os.h>

#define USAGE "[-Dnnnn]"

/*..........................................................................*/

int
dt_sez90_time_all(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_sez90_time");
  dt_ctl_t *ctl = &param->ctl;
										/* default variant				    */
  DT_Q(dt_sez90_time1,(param, 
    assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dt_sez90_time_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *input,
  char *id,
  dtos_time_t *mark1,
  dtos_time_t *mark2,
  int *assert_fail)
{
  static char *F = "dt_sez90_time_check_and_free";
  dt_rc_e rc;

  DT_Q(dtimage_free,(ctl,				/* free images					    */
    input));

  rc = dt_assert(ctl,
    0,
    id, "time not implemented");

  if (rc == DT_RC_GOOD)
    DT_Q(dtos_time_report,(ctl,
      mark1, mark2, id))
  else
    *assert_fail += 1;

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dt_sez90_time1(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_time1";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;
  dtos_time_t mark1, mark2;
  char id[32];

#define NCOLS 256
#define NROWS 256

  DT_Q(dtimage_create,(ctl,				/* space for input image	 	   */
    &input, NCOLS, NROWS,
    DT_NTYPE_B08));

  DT_Q(dtimage_gen_rect_tree,(ctl,		/* gen test pattern				    */
    &input, 1.0, 255.0, 999));

  DT_Q(dt_sez90_time_check_and_free,(	/* check and free				    */
    ctl, &input,
    id,
    &mark1, &mark2, assert_fail));

#undef NCOLS
#undef NROWS
    
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
