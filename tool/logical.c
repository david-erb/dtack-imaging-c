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
| dttool_logical utility -
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

DT_RCSID("tool $RCSfile: logical.c,v $ $Revision: 1.9 $");

#define USAGE "[params] input op scalar|image output"

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

#define OR 1
#define XOR 2
#define AND 3

/*..........................................................................*/

DTTOOL_MAIN(dttool_logical)
{
  dtimage_t image1;						/* image structure					*/
  dtimage_t image2;
  
  dt_ctl_t *ctl = &param->ctl;
  char *arg_input1;
  char *arg_input2;
  char *arg_op;
  char *arg_output;
  double scalar;
  int isnumber;
  int op;
  dt_rc_e rc = DT_RC_GOOD;
  int n;

  DT_Q(parse,(param,					/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (rc == DT_RC_GOOD)
  if (argc != 4)						/* check positional args			*/
    DT_G(dttool_usage,(param, USAGE));

  arg_input1 = argv[0];					/* get positionals				    */
  arg_op = argv[1];
  arg_input2 = argv[2];
  arg_output = argv[3];

  if (rc == DT_RC_GOOD)
  if (!strcmp(arg_op, "or")) op = OR;
  else
  if (!strcmp(arg_op, "xor")) op = XOR;
  else
  if (!strcmp(arg_op, "and")) op = AND;
  else
    rc = dt_err(ctl, F,
      "invalid op \"%s\"", arg_op);

  image1.xe = 0;
  image2.xe = 0;

  DT_GI(dtimage_import,(ctl,			/* get first input image			*/
    &image1, arg_input1));

  if (rc == DT_RC_GOOD)
  dtstr_to_double2(ctl,					/* try to convert input2 to double	*/
    arg_input2, &scalar, &isnumber);

  if (isnumber)							/* input2 is a number?			    */
  {
    switch(op)							/* do scalar operation			    */
	{
	case OR:
      DT_GI(dtimage_logical,(ctl,	
        &image1, 0xffffffffL,
        &image1, 0, 
        (unsigned long)scalar, 
        DT_OP_OR, &image1));
    break;
	case AND:
      DT_GI(dtimage_logical,(ctl,	
        &image1, 0xffffffffL,
        &image1, 0xffffffffL,
        (unsigned long)scalar, 
        DT_OP_AND, &image1));
    break;
	case XOR:
      DT_GI(dtimage_logical,(ctl,	
        &image1, 0xffffffffL,
        &image1, 0, 
        (unsigned long)scalar, 
        DT_OP_XOR, &image1));
    break;
    }
  }
  else									/* input2 is not a number?		    */
  {
    DT_GI(dtimage_import,(ctl,			/* get input2 as image				*/
      &image2, arg_input2));
    switch(op)							/* do image operation			    */
	{
    case OR:
      DT_GI(dtimage_logical,(ctl,	
        &image1, 0xffffffffL,
        &image2, 0xffffffffL,
        (unsigned long)scalar, 
        DT_OP_OR, &image1));
    break;
    case AND:
      DT_GI(dtimage_logical,(ctl,	
        &image1, 0xffffffffL,
        &image2, 0xffffffffL,
        (unsigned long)scalar, 
        DT_OP_AND, &image1));
    break;
    case XOR:
      DT_GI(dtimage_logical,(ctl,	
        &image1, 0xffffffffL,
        &image2, 0xffffffffL,
        (unsigned long)scalar, 
        DT_OP_XOR, &image1));
    break;
    }
  }

  DT_GI(dtimage_export,(ctl,			/* output resulting image			*/
    &image1, arg_output, DTIMAGE_NOPOS, DTIMAGE_NOPOS));
 
  if (image2.xe)						/* second image got allocated?	    */
    dtimage_free(ctl, &image2);			/* free second image				*/

  if (image1.xe)						/* first image got allocated?	    */
    dtimage_free(ctl, &image1);			/* free first image					*/

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */
										
										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dttool_parse,(param, F, USAGE,		/* go parse args					*/
    argc, argv, n));

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
