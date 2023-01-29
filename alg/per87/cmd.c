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
| dts_per87 sample program - Perez&Gonzolez 1987
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

static char rcsid[] = "util $RCSfile: cmd.c,v $ $Revision: 1.3 $";

#include <dtack/base.h>
#include <dt_per87.h>

#define USAGE "[params] input output"

static dt_rc_e local_parse(				/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

#define KERNEL_SIZE (3)

static double kernel_double[			/* gradient filter coefficients		*/
  KERNEL_SIZE] = {-1.0, 0.0, 1.0};

static long kernel_uchar[				/* gradient filter coefficients		*/
  KERNEL_SIZE] = {-1, 0, 1};

/*..........................................................................*/

static char *F = "dt_per87_cmd";

int
dt_per87_cmd(
  int argc,
  char *argv[])
{
  dtimage_t input, output;				/* image structures					*/
  dtparam_t param = {0};				/* parameter structure				*/
  dt_ctl_t *ctl = &param.ctl;
  dt_op_e op;
  int n;
  dt_rc_e rc;

  rc = local_parse(&param,				/* parse parameters				   	*/
    argc, argv, &n);
  if (rc != DT_RC_GOOD) return rc;

  argc -= n;
  argv += n;
  if (argc != 2)						/* check remaining args				*/
    return dt_interactive_usage(
      &param, USAGE);

  DT_Q(dtimage_import,(ctl,				/* get input image					*/
    &input, argv[0]));

  if (!strcmp(param.op, "min"))			/* select operation				    */
    op = DT_OP_MIN;
  else
  if (!strcmp(param.op, "max"))			
    op = DT_OP_MAX;
  else
  if (!strcmp(param.op, "ave"))			
    op = DT_OP_AVE;
  else
  if (!strcmp(param.op, "row"))			
    op = DT_OP_ROW;
  else
  if (!strcmp(param.op, "col"))			
    op = DT_OP_COL;
  else
    rc = dt_err(ctl, F,
      "op \"%s\" not one of: min, max, ave, row or col",
      param.op);

  output.xe = 0;
  if (rc == DT_RC_GOOD)
  DT_G(dtimage_create,(ctl,				/* new image same size as old one	*/
    &output, input.xe, input.ye,
    input.ntype));

  if (rc == DT_RC_GOOD)
  if (!strncmp("double",				/* double precision desired?	    */
       param.precision,
       strlen(param.precision)))
  {
    DT_G(dt_per87_double,(&param,		/* compute threshold image			*/
      &input, &output,
      kernel_double, KERNEL_SIZE));
  }
  else	
  if (!strncmp("uchar",					/* 8-bit integer precision desired?	*/
       param.precision,
       strlen(param.precision)))
  {
    DT_G(dt_per87_uchar,(&param,		/* compute threshold image			*/
      &input, &output,
      kernel_uchar, KERNEL_SIZE));
  }
  else
    rc = dt_err(ctl, F,
      "precision \"%s\" not one of: uchar or double",
      param.precision);

  if (rc == DT_RC_GOOD)
  DT_Q(dtimage_export,(ctl,				/* output resulting image			*/
    &output, argv[1], -1, -1));

  if (output.xe != 0)
  DT_Q(dtimage_free,(ctl,				/* free images						*/
    &output));

  DT_Q(dtimage_free,(ctl,	
    &input));

  DT_Q(dtparam_free,(&param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/

static dt_rc_e local_parse(				/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(op, "ave");
  DTPARAM_DEFAULT(t, 128);
  DTPARAM_DEFAULT(dz, 3);
  DTPARAM_DEFAULT(b, 4);
  DTPARAM_DEFAULT(n, 2);
  DTPARAM_DEFAULT(offset, 0);
  DTPARAM_DEFAULT(precision, "uchar");
  DTPARAM_DEFAULT(dbg, 0);

  rc = dt_interactive_parse(			/* go parse args					*/
    argc, argv, F, USAGE,
    param, n);

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
