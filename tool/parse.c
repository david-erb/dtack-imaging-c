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
| dttool
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: parse.c,v $ $Revision: 1.2 $");

#include <dtack/lut8.h>

/*..........................................................................*/

dt_rc_e
dttool_parse(
  dtparam_t *param,
  char *f,
  char *usage,
  int argc,
  char *argv[],
  int *n)
{
  DT_F("dttool_parse");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dt_enter_param,(		
    argc, argv, f, usage, param, n));
  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dttool_usage(
  dtparam_t *param,
  char *string)
{
  DT_F("dttool_usage");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dt_usage_say,(
    param, string));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
dttool_parse_op(						/* check ops for this program		*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **ops,
  char *msg,
  const char **start)
{
  DT_F("dttool_parse_op");
  int op;

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, &op));

  if (op == -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is not understood.", 
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (op < -1)
  {
    sprintf(msg,
      "Sorry, op \"%s\" is ambiguous.",
      param->op);
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
dttool_parse_precision(					/* check precision for this program	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char **precisions,
  char *message,
  const char **start)
{
  DT_F("dttool_parse_precision");

  if (param->precision != NULL)
  {
	dt_ntype_e ntype;
    DT_Q(dtntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, DT_ENTER_MSG_MAXLEN));
	if (message[0] != '\0')				/* bad precision? 					*/
	  *start = param->precision;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * check for argument
 *..........................................................................*/

dt_rc_e 
dttool_parse_arg_string(				/* get string argument				*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  int k,
  const char *dflt,
  char *message,
  const char **start)
{
  DT_F("dttool_parse_arg_string");
  dt_ctl_t *ctl = &param->ctl;
  
  *message = '\0';

  if (arg->flags &						/* need something? 					*/
	  DTTOOL_PARSE_FLAG_NEED ||
	  dflt == NULL)
  {
	if (arg[k].val[0] == '\0')			/* don't have anything?				*/
	  dtstr_printf(ctl, 
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something"
        " for \"%s\"",
        arg[k].desc);
  }
  else									/* arg not required? 				*/
  {
	if (arg[k].val[0] == '\0')			/* don't have anything?				*/
	  DT_Q(dtstr_printf,(ctl,			/* use the default 					*/
        arg[k].val, arg[k].val_max,
        "%s", dflt));
  }
  


  return DT_RC_GOOD;
}

/*..........................................................................
 * check for long argument
 *..........................................................................*/

dt_rc_e 
dttool_parse_arg_long(					/* get argument as long				*/
  dtparam_t *param,
  dt_enter_arg_t *arg,
  int k,
  const char *dflt,
  long *longval,
  char *message,
  const char **start)
{
  DT_F("dttool_parse_arg_long");
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dttool_parse_arg_string,(		/* make sure arg is there 			*/
    param, arg, k, dflt,
    message, start));

  if (*message == '\0')					/* no message yet?					*/
  {
	long tmp;
	int islong;	
	DT_Q(dtstr_to_long2,(ctl,			/* convert numeric arg	    		*/
      arg[k].val, longval, &islong));
	if (!islong)
	  DT_Q(dtstr_printf,(ctl,
        message, DT_ENTER_MSG_MAXLEN,
        "Sorry, \"%s\" is not an integer.",
        arg[k].val));
  }
										/* ................................ */
  if (*message != '\0')					/* something bad happened? 			*/
    *start = arg[k].desc;				/* set focus at this field 			*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttool_parse_param_color(				/* parse color						*/
  dtparam_t *param,
  const char *dflt,
  double *color,
  char *message,
  const char **start)
{
  DT_F("dttool_parse_arg_color");
  dt_ctl_t *ctl = &param->ctl;
  const char *val =
    DTPARAM_GET_VALUE(color, dflt);

  if (val == NULL)						/* no value and no default?			*/
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
      "Please enter something"
      " for \"%s\"",
      param->color_desc);
  }
  else									/* have a value or default? 		*/
  {
	int index;
	DT_Q(dtlut8_name_to_index,(ctl,		/* convert color argument 			*/
      param->color, &index));
	if (index < 0)						/* not a named color? 				*/
	{
	  long tmp;
	  int islong;
	  DT_Q(dtstr_to_long2,(ctl,			/* convert numeric arg	    		*/
        param->color, &tmp, &islong));
	  if (!islong)
	  {
	    DT_Q(dtstr_printf,(ctl,
          message, DT_ENTER_MSG_MAXLEN,
          "Sorry, \"%s\" is not a color.",
          param->color));
	  }
	  else
	    *color = DT_CAST(double, tmp);
	}
	else
      *color = DT_CAST(double, index);
  }
										/* ................................ */
  if (*message != '\0')					/* something bad happened? 			*/
    *start = param->color_desc;			/* set focus at this field 			*/

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
