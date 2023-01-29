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

DT_RCSID("esc94 $RCSfile: time.c,v $ $Revision: 1.2 $");

#include <dt_esc94.h>
#include <dtack/os.h>

#define USAGE "[-Dnnnn]"


/*..........................................................................*/

dt_rc_e
dt_esc94_time_all(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_esc94_time");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dt_esc94_time1,(param,			/* generated images				    */
    assert_fail));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    assert_fail));

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_esc94_time_one(
  dtparam_t *param,
  dtimage_t *source,
  char *id,
  int *assert_fail)
{
  DT_F("dt_esc94_time_one");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t result;
  dtos_time_t mark1, mark2;
  dt_rc_e rc = DT_RC_GOOD;

  result.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for result image	    	*/
    &result, source->xe, source->ye,
    DT_NTYPE_B08));

  DT_Q(dtos_time,(ctl, &mark1));
  DT_GI(dt_esc94,(param, source,		/* do the operation				    */
    &result));
  DT_Q(dtos_time,(ctl, &mark2));

  DT_I(dtimage_free,(ctl, &result));	/* don't check result			    */

  DT_Q(dtos_time_report,(ctl,
    &mark1, &mark2, id))

  return rc;
}

/*..........................................................................*/

dt_rc_e
dt_esc94_time1(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dt_esc94_time1");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t source;
  dt_rc_e rc = DT_RC_GOOD;


  param->max = 255;						/* we will use 8-bit images		    */
  param->min = 0;

  param->t = 0.0;						/* give consistent results		    */
  param->k = 1.0;
  param->lambda = 0.0;
  param->variant = "fast";

  source.xe = 0;
  DT_GI(dtimage_create,(ctl,			/* space for source image		    */
    &source, 1024, 512,
    DT_NTYPE_B08));

  DT_GI(dtimage_gen_test1,(ctl,			/* make source image			    */
    &source));

  DT_GI(dt_esc94_time_one,(param,		/* run the timing				    */
    &source, 
    "dt_esc94", assert_fail));

  DT_I(dtimage_free,(ctl, &source));

  return rc;
}
/*..........................................................................*/






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
