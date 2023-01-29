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
| TITLE
| Test: dttos
| 
|!Test Overview
|_index overview
| This program tests principal functions of the :dttos: family.
| Tests are not designed to be exhaustive,
| rather they are representative of the most common usage of the library.
| Some tests are automatic and self-checking,
| while others require observation by a person.
|
| Each time the test is run, it is given an
|_hyper Operation Parameter, operation
| to perform and a number of
|_hyper Repetitions Parameter, repetitions
| to perform it.
|
|!Test Operations
|_index operations
| Each invocation of the program does one of the following operations:
| file		file utility functions
| delay0	100 millisecond sleeps
| delay1	1 second sleeps
| delay2	2 second sleeps
| all		all of the above, in that order
| You tell the program which operation to exercise with the
|_hyper Operation Parameter, operation.
|
|!Test Parameters
|_index parameters
| Each invocation of the program accepts any of the following optional
| parameters:
| Operation		operation selection
| Repetitions	repetition count
| Debug Mask	level of debug output
|
|.Operation Parameter
| The operation parameter selects the operation to be performed.
| Allowed values for the parameter are:
| :file:	file utility functions
| :delay0:	100 millisecond sleeps
| :delay1:	1 second sleeps
| :delay2:	2 second sleeps
| :all:		all of the above, in that order
| Default value for the parameter is :delay1:.
| The operation will be repeated according to the number
| given in the
|_hyper Repetitions Parameter, Repetitions Parameter.
|
|.Repetitions Parameter
| The repetitions parameter controls the number of times the
|_hyper Operation Parameter, operation
| is repeated.
| 
| END
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

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttos.c,v $ $Revision: 1.5 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "file", "delay", "delay0", "delay1", "delay2", 
  NULL};

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttos_delay(
  dt_ctl_t *ctl,
  long milliseconds,
  int *assert_fail)
{
  DT_F("dttos_delay");
  dtos_time_t mark1, mark2;
  long elapsed;
  char status[32];

  DT_Q(dtstr_printf,(ctl, status,
    sizeof(status), "Delaying %ld ms",
    milliseconds));

  DT_Q(dt_yield,(ctl, status));
  DT_Q(dtos_time,(ctl, &mark1));
  DT_Q(dtos_delay,(ctl, milliseconds));
  DT_Q(dtos_time,(ctl, &mark2));

  elapsed = (long)
   ((mark2.wall - mark1.wall) * 1000.0 + 0.5);

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      elapsed >= milliseconds &&
      elapsed <= (milliseconds+200), 
      F, "elapsed %ld < %ld < %ld",
      milliseconds, 
      elapsed, 
      milliseconds+200);

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttos_file(
  dt_ctl_t *ctl,
  const char *tmpdir,		   
  int *assert_fail)
{
  DT_F("dttos_file");
  char filename1[128];
  char filename2[128];
  FILE *file;
  int isreadable;
  dt_rc_e rc;

  DT_Q(dtstr_printf,(ctl,				/* make first filename		    	*/
    filename1, sizeof(filename1),
    "%s/dttos1.tmp", tmpdir));

  DT_Q(dtos_isreadable,(ctl, filename1,	/* make sure file does not exist    */
    &isreadable));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      isreadable == 0, F,
      "tmp file \"%s\" unique",
      filename1);

  file = fopen(filename1, "w");			/* create the file				    */
  if (file == NULL)
    return dt_err(ctl, F,
      "fopen(\"%s\", \"w\") errno %d",
      filename1, errno);
  fprintf(file, "stuff\n");				/* write stuff in file			    */
  fclose(file);							/* finish with file				    */

  DT_C(dtos_isreadable,(ctl, filename1,	/* make sure file exists now		*/
    &isreadable));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      isreadable != 0, F,
      "tmp file \"%s\" now there",
      filename1); 
										/* ................................	*/

  DT_Q(dtstr_printf,(ctl,				/* make second filename		    	*/
    filename2, sizeof(filename2),
    "%s/dttos2.tmp", tmpdir));

  DT_Q(dtos_isreadable,(ctl, filename2,	/* make sure file does not exist    */
    &isreadable));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      isreadable == 0, F,
      "tmp file \"%s\" unique",
      filename2);

  file = fopen(filename2, "w");			/* create the file				    */
  if (file == NULL)
    return dt_err(ctl, F,
      "fopen(\"%s\", \"w\") errno %d",
      filename2, errno);
  fprintf(file, "stuff\n");				/* write stuff in file			    */
  fclose(file);							/* finish with file				    */

  DT_C(dtos_isreadable,(ctl, filename2,	/* make sure file exists now		*/
    &isreadable));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      isreadable != 0, F,
      "tmp file \"%s\" now there",
      filename2); 

cleanup:
  DT_I(dtos_unlink,(ctl, filename1));
  DT_I(dtos_unlink,(ctl, filename2));

  DT_Q(dtos_isreadable,(ctl, filename1,	/* make sure file is gone			*/
    &isreadable));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      isreadable == 0, F,
      "tmp file \"%s\" cleaned",
      filename1);

  DT_Q(dtos_isreadable,(ctl, filename2,	/* make sure file is gone			*/
    &isreadable));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      isreadable == 0, F,
      "tmp file \"%s\" cleaned",
      filename2);

  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttos_arg_check(						/* check params for this program	*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttos_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttos)
{
  DT_F("dttos");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "all",
    &param->reps, NULL, NULL, "2",
    &param->output, "Tmp dir", NULL, "/tmp",
    NULL));

  arg[0].desc = NULL;					/* no arguments					    */
  
  DT_GI(dt_enter_parse,(				/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttos_arg_check, NULL));
										/* ................................ */

  {
    long reps;
    for (reps=0; reps<param->reps; reps++)
	{
	  DTT_YIELD_REPS(DT_G, ctl,			/* yield at start of repetition 	*/
        param, reps);
 	  
      if (DT_ABBREV("all", param->op) ||
          DT_ABBREV("file", param->op))
  	  {
        DT_GI(dttos_file,(ctl,			/* test file-related functions	    */
		  param->output,
          &assert_fail));
	  }

      if (DT_ABBREV("all", param->op) ||
          DT_ABBREV("delay", param->op) ||
          DT_ABBREV("delay0", param->op))
  	  {
        int i;
        for (i=0; i<10; i++)
          DT_GI(dttos_delay,(ctl,		/* test delay function			    */
            100, &assert_fail));
	  }

      if (DT_ABBREV("all", param->op) ||
          DT_ABBREV("delay", param->op) ||
          DT_ABBREV("delay1", param->op))
	  {
        int i;
        for (i=0; i<5; i++)
          DT_GI(dttos_delay,(ctl,		/* test delay function			    */
            1000, &assert_fail));
	  }

      if (DT_ABBREV("all", param->op) ||
          DT_ABBREV("delay", param->op) ||
          DT_ABBREV("delay2", param->op))
	  {
        int i;
        for (i=0; i<2; i++)
          DT_GI(dttos_delay,(ctl,		/* test delay function			    */
            2000, &assert_fail));
	  }
	}
  }

										/* ................................ */

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
    return rc;
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
