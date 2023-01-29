/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/t.h>

DT_RCSID("test $RCSfile: check.c,v $ $Revision: 1.8 $");

/*..........................................................................
| NAME
| dtt_check - check condition
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| As yet undocumented.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtt_check(
  dt_ctl_t *ctl,
  int cond,
  char *format,
  ...)
{
  DT_F("dtt_check");
  dt_rc_e rc;
  va_list ap;
  va_start(ap, format);
  DT_G(dt_assert_va,(ctl, cond, F, 
    format, ap));
  va_end(ap);
  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtt_check_op(							/* check ops for this program		*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **ops,
  char *msg,
  const char **start)
{
  DT_F("dtt_check_op");
  int op;
  DT_Q(dtt_check_op2,(ctl, param, ops,
    &op, msg, start));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtt_check_op2(							/* check ops for this program		*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **ops,
  int *op,
  char *msg,
  const char **start)
{
  DT_F("dtt_check_op");

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, op));

  if (*op == -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is not understood.", 
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (*op < -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is ambiguous"
      " (%d possibilities).",
      param->op, -(*op));
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtt_check_datadir(						/* make sure -datadir is directory	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  char *msg,
  const char **start)
{
  DT_F("dtt_check_datadir");
  int isdir = 0;
  char *filename;
# define FILENAME "testdata.id"
  int isreadable;

  if (param->datadir == NULL)
  {
    sprintf(msg,
      "Sorry, no value was given"
      " for the datadir parameter.");
    *start = param->datadir_desc;		/* put cursor at this field		    */
    return DT_RC_GOOD;
  }
  DT_Q(dtos_isdir,(ctl,					/* check if it is a directory		*/
    param->datadir, &isdir));

  if (isdir == 0)
  {
    sprintf(msg,
      "Sorry, \"%s\" is not a directory.",
      param->datadir);
    *start = param->datadir_desc;		/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  DT_Q(dtos_malloc2,(ctl,				/* space for name of telltale file	*/
    (void **)&filename,
	strlen(param->datadir)+
    strlen(FILENAME)+2,
    F, "test datadir identifier"));
  strcpy(filename, param->datadir);
  strcat(filename, "/");
  strcat(filename, FILENAME);
  DT_Q(dtos_isreadable,(ctl, filename,	/* see if telltale file is in dir 	*/
	&isreadable));

  if (isreadable == 0)
  {
    sprintf(msg,
      "Sorry, \"%s\" not found in \"%s\".",
      FILENAME, param->datadir);
    *start = param->datadir_desc;		/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  DT_Q(dtos_free2,(ctl, filename,
    F, "test datadir identifier"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * check for argument
 *..........................................................................*/

dt_rc_e 
dtt_check_arg(							/* get argument						*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  int k,
  void *extra,
  char *message,
  const char **start)
{
  dt_ctl_t *ctl = &param->ctl;
  
  *message = '\0';

  if (arg[k].flags &					/* need something? 					*/
	  DTT_CHECK_FLAG_NEED)
  {
	if (!strcmp(arg[k].val, ""))		/* we have a source pattern? 		*/
	  dtstr_printf(ctl, 
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something"
        " for \"%s\"",
        arg[k].desc);
  }

  if (*message != '\0')					/* something bad happened? 			*/
    *start = arg[k].desc;				/* set focus at this field 			*/

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
