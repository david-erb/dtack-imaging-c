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
| USAGE
| dtlev85 - algorithm executive
| 
|.SYNOPSIS
|:dtlev85 [parameters] gray segmented [output]
| 
|.SUMMARY
| Characterize the goodness of a segmentation.
| A uniformity number for each area is printed to the console.
| Can be used to produce an output image where each pixel value is
| proportional to the uniformity of the pixel's blob.
| Algorithm due to Levine and Nazif, 1985.
| 
|.ARGUMENTS
| :gray:	   	name of the original gray-level image (required)
| :segmented:	name of the image which has been segmented (required)
| :output:		name of output image (optional, ignored if not :-op blob:)
|
|.PARAMETERS
| :-op total:			all non-background pixels are one area
|						(this is the default if :-op: is omitted)
| :-op label:			each label is a separate area
| :-op blob:			each blob is a separate area
| :-op test:			self-tests, prints nothing if all pass
| :-op time:			benchmark and display execution times
| :-background: ~bv~	segmented pixels to be considered background 
|						(default 0, ignored if not :-op total:)
| :-min: ~min~			minimum area for blob to be considered
|						(default 0, ignored if not :-op blob:)
| :-max: ~max~			maximum area for blob to be considered
|						(default unlimited, ignored if not :-op blob:)
| :-precision: ~type~	precision of :output: image
|						(default :float:, ignored if not :-op blob:)
| :-connect: ~4 or 8~	4-connectivity or 8-connectivity (default 4)
|dtinclude(params1.dx)
| 
|.DESCRIPTION
| Presumes that ~gray~ image has been segmented in some way to produce
| the corresponding ~segmented~ image in which
| the value of each pixel is a region label.
| All 4- or 8-connected pixels in with the same label are considered
| to be part of the same region.
| Regions are grouped into areas according to the :-op: parameter.
| A uniformity number is printed for each area.
| 
| The uniformity numbers printed by this program go through the dt_say()
| interface.
| This interface normally forces the output text to go straight to the console.
| Since stdout is bypassed, redirecting it will not capture the text output.
| To capture the text in a file, use the :-sayto: ~filename~ parameter.
|
| For the operations :total:, :label:, and :blob:,
| regions are formed by connected components analysis.
| All pixels which are 4- or 8-connected
| and have identical values in the ~segmented~ image
| are considered part of the same region.
| The value of the :-connect: parameter decides the connectivity.
|
| For :-op total:,
| all non-background pixels are assumed part of a single area.
| Only one uniformity number is printed.
| Background pixels are those whose value in the ~segmented~ image are ~bv~.
| The parameters :-min:, :-max:, and :-precision: are ignored for this operation.
| ~output~ is ignored and no output image is produced.
|
| For :-op label:,
| regions are grouped into areas according to their label values.
| The label for each pixel is its value in the ~segmented~ image.
| All regions consisting of pixels having the same label are considered
| part of the same area.
| One uniformity is printed for each unique label.
| The parameters :-background:, :-min:, :-max:, and :-precision: 
| are ignored for this operation.
| ~output~ is ignored and no output image is produced.
|
| For :-op blob:,
| each region is considered an area of its own.
| That is, each area has only one region in it.
| One uniformity number is printed for each blob, except
| any blob whose size is less than ~min~ or greater than or
| equal ~max~ is silently omitted.
| The parameter :-background: is ignored for this operation.
| 
| If ~output~ is given,
| then an output image is produced.
| This output image will have a size according to the smallest
| dimensions of ~gray~ and ~segmented~.
| 
| The data type of the output image will be set according to ~type~.
| The values of the pixels in the output image
| are set according to the uniformity of the blob
| in which the pixel resides.
| If the data ~type~ is :float: or :double:,
| then the uniformity values are stored directly into the pixels.
| Otherwise the uniformity values are scaled
| to use the full dynamic range of the data type.
| For example, a uniformity of 1.0 is given a value of 255
| give the ~type~ is :uchar:.
|
| The :blob: operation interprets the value of the :-background:
| as the uniformity value to use for pixels in blobs which
| are too small or too large, according to ~min~ and ~max~.
| The value is clipped to the range 0.0 to 1.0.
| If not given, 1.0 is assumed.
| 
| On unix-like systems, the name for ~output~
| may be a filename or a window name.
| If the name is the special value "stdout",
| then the image may be piped into another program.
| 
| For :-op test:,
| the program runs its self-test diagnostics.
| It normally prints nothing if all tests are successful.
| To have it print even successful test results, use :-D assert:.
| The parameters :-background:, :-min:, :-max:, and :-precision: 
| are ignored for this operation.
| ~output~ is ignored and no output image is produced.
| 
| For :-op time:,
| the program runs its benchmark timing tests.
| The resulting execution times are printed as debug.
| The parameters :-background:, :-min:, :-max:, and :-precision: 
| are ignored for this operation.
| ~output~ is ignored and no output image is produced.
|
| This program normally exits with a value of 0.
| However, if an error was found during execution,
| or one of the test or timing operations had failed assertions,
| then the program returns a value of 1.
|
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("alg/lev85 $RCSfile: cmd.c,v $ $Revision: 1.7 $");
#include <dtlev85.h>
#include <dtack/stack.h>
#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/mem.h>

#include <dtlev85.use>

# define OP_TOTAL   "total"
# define OP_LABEL   "label"
# define OP_BLOB    "blob"

# define OP_TEST    "test"
# define OP_TIME    "time"
# define OP_QTIME   "qtime"


#define OUT_BACKGROUND (0.5)
#define OUT_BUILDING (0.0)

static const char *ops[] = {
  OP_TOTAL,
  OP_LABEL,
  OP_BLOB,
  OP_TEST,
  OP_TIME,
  OP_QTIME,
  NULL
};

typedef struct {						/* passed to label callback 		*/
  int n;
} my_label_arg_t;

static dtrun_deliver_f					/* the blob callback routine 		*/
  dtlev85_cmd_blob_deliver;

typedef struct {						/* passed to blob callback 			*/
  double background;
  dtlev85_t total_lev85;				/* accumulate total uniformity		*/
  dtparam_t *param;
  dtimage_t *gray;
  dtimage_t *output;
  unsigned long minarea;
  unsigned long maxarea;
} my_blob_arg_t;

extern
dt1d_rc_e
dt1d_set(
  dt1d_ntype_e ntype,
  void * const in,
  const dt1d_i_t i1,
  const dt1d_i_t i2,
  double value);

/*..........................................................................
| dtlev85_cmd() - command line interface to algorithm
 *..........................................................................*/

dt_rc_e
dtlev85_cmd(
  int argc,
  char *argv[],
  dtparam_t *param)
{
  DT_F("dtlev85_cmd");
  dt_ctl_t *ctl = &param->ctl;
# define ARG_MAXLEN 128
  char gray[ARG_MAXLEN];
  char segmented[ARG_MAXLEN];
  char output[ARG_MAXLEN];
  dt_enter_arg_t arg[4];
  dt_rc_e rc = DT_RC_GOOD;

  memset(param, 0, sizeof(*param));

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param,				/* defaults for secondary params 	*/
    DTPARAM_FLAG_PRIMARY,
    &param->op, NULL, NULL, OP_TOTAL,
    NULL));

  DT_GI(dtparam_set,(param,				/* defaults for more params			*/
    DTPARAM_FLAG_SECONDARY,
    &param->background, 
      "Segmented background",  
      NULL, "0",
    &param->connect, 
      "Connect (4 or 8)",  
      NULL, "4",
    &param->min, 
      "Minimum region area",  
      NULL, "0",
    &param->max, 
      "Maximum region area",
      NULL, "0",
    &param->precision, 
      "Output precision",
      NULL, "float",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Gray image", NULL, 
      gray, sizeof(gray),
    "Segmented image", NULL, 
      segmented, sizeof(segmented),
    "Output image", NULL, 
      output, sizeof(output),
    NULL));

  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, "dtlev85", 
    usage, param,
    arg, dtlev85_cmd_check, NULL));
										/* ................................ */
  while (rc == DT_RC_GOOD)
  {
    if (DT_ABBREV(OP_TOTAL, param->op))	/* ................................ */
	{
      DT_GI(dtlev85_cmd_total,	
        (param, gray, segmented));
	}
	else
    if (DT_ABBREV(OP_LABEL, param->op))	/* ................................ */
	{
	  DT_GI(dtlev85_cmd_label,(
        param, gray, segmented));
	}
	else
    if (DT_ABBREV(OP_BLOB, param->op))	/* ................................ */
	{
	  DT_GI(dtlev85_cmd_blob,(
        param, gray, segmented,
        output,
        DT_CAST(dtxy_t,					/* image exported x position 		*/
          DTPARAM_GET_VALUE(x,			/* from -x arg 						*/
            DTIMAGE_NOPOS)),			/* or use window system default 	*/
        DT_CAST(dtxy_t,					/* image exported y position 		*/
          DTPARAM_GET_VALUE(y,			/* from -y arg 						*/
            DTIMAGE_NOPOS))));			/* or use window system default 	*/
	}
	else
    if (DT_ABBREV(OP_TEST, param->op))	/* ................................ */
	{
      int assert_fail = 0;
      DT_GI(dtlev85_test_all,(param,	/* do the operation				    */
        &assert_fail));

      if (rc == DT_RC_GOOD &&			/* test worked?					    */
          assert_fail)					/* but some results wrong?		    */
        rc = DT_RC_BAD;
	}
	else
    if (DT_ABBREV(OP_TIME, param->op)||	/* ................................ */
		DT_ABBREV(OP_QTIME, param->op))
	{
      int assert_fail = 0;
	  if (!DT_ABBREV(OP_QTIME,			/* don't explicitly want it quiet? 	*/
			 param->op))
  	    ctl->dbg_mask |= 
          DT_DBG_MASK_TIME;
      DT_GI(dtlev85_time_all,(param,	/* do the operation				    */
        &assert_fail));

      if (rc == DT_RC_GOOD &&			/* test worked?					    */
          assert_fail)					/* but some results wrong?		    */
        rc = DT_RC_BAD;
	}
	else
	  rc = dt_err(ctl, F,
        "internal error:"
        " no case for -op %s",
        param->op);

    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, usage, arg,
      dtlev85_cmd_check, NULL, rc));

  } 

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  {
	int check_fail = 0;
    DT_I(dt_heapmon_uninit,(ctl,		/* check all memory is freed        */
      &check_fail));
    if (check_fail)						/* any failed assertions?           */
      rc = DT_RC_BAD;
    else
      rc = DT_RC_GOOD;
  }

  return rc;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtlev85_cmd_check(						/* check params for this program	*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("dtlev85_cmd_check");
  int op;
  int i;

  DT_Q(dtstr_choice,(ctl, ops,			/* make sure op is understood	    */
    param->op, &op));

  if (op == -1)
  {
    DT_Q(dtstr_printf,(ctl,
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, op \"%s\" is not understood", 
      param->op));
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  if (op < -1)
  {
    DT_Q(dtstr_printf,(ctl,
      message, DT_ENTER_MSG_MAXLEN,
      "Sorry, op \"%s\" is ambiguous",
      param->op));
    *start = param->op_desc;			/* put cursor at this field		    */
    return DT_RC_GOOD;
  }

  {
    const char *connect =
      DTPARAM_GET_VALUE(connect, "4");

    if (strcmp(connect, "4") &&			/* check connectivity 				*/
	    strcmp(connect, "8"))
  	{
      DT_Q(dtstr_printf,(ctl,
        message, DT_ENTER_MSG_MAXLEN,
	    "Sorry, connect \"%s\""
        " is not 4 or 8",
        connect));
	  *start = param->connect_desc;
  	  return DT_RC_GOOD;
	}
  }

  {
    const char *precision =
      DTPARAM_GET_VALUE(precision, 
        "float");
	dt_ntype_e ntype;
    DT_Q(dt_ntype_lookup,(ctl,			/* get desired output precision	    */
      precision, &ntype,
      message, DT_ENTER_MSG_MAXLEN));
	if (message[0] != '\0')				/* bad precision? 					*/
	{
	  *start = param->precision;
  	  return DT_RC_GOOD;
	}
  }

  if (strcmp(param->op, OP_TEST) &&		/* test and time have no args	    */
      strcmp(param->op, OP_QTIME) &&
      strcmp(param->op, OP_TIME))
  for (i=0; i<2; i++)					/* check first two args for a value	*/
  {
    int l = strlen(arg[i].val);
    int j;

    for (j=0; j<l; j++)					/* search value for non-blank	    */
      if (arg[i].val[j] != ' ')
        break;
    if (j == l)
    {
      DT_Q(dtstr_printf,(ctl,
        message, DT_ENTER_MSG_MAXLEN,
        "Please enter something for \"%s\"",
        arg[i].desc));
      *start = arg[i].val;				/* position cursor on this field    */
      return DT_RC_GOOD;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_cmd_total(						/* import images and do total		*/
  dtparam_t *param,
  const char *gray_name,
  const char *segmented_name)
{
  DT_F("dtlev85_cmd_total");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t gray, segmented;			/* image structures					*/
  dtlev85_t lev85;
  dtos_time_t mark1, mark2;
  dt_rc_e rc;

  gray.xe = 0;
  segmented.xe = 0;

  DT_C(dtimage_import,(ctl,				/* get gray image					*/
    &gray, gray_name));

  DT_C(dtimage_import,(ctl,				/* get segmented image				*/
    &segmented, segmented_name));

  DT_Q(dtos_time,(ctl, &mark1));
  DT_C(dtlev85_total,(param,			/* compute uniformity				*/
    &gray, &segmented,
    param->background,
    &lev85));
  DT_Q(dtos_time,(ctl, &mark2));

  DT_Q(dtos_time_report,(ctl,			/* report how long it took			*/
    &mark1, &mark2, "dtlev85_label"));

  dt_say(ctl, NULL,							/* print uniformity				    */
    "total uniformity %0.5f"
    " over %ld regions",
    lev85.u, lev85.n);
  
cleanup:
  if (segmented.xe)						/* segmented image got allocated?	*/
  DT_I(dtimage_free,(ctl,				/* free it							*/
    &segmented));

  if (gray.xe)							/* gray image got allocated?	    */
  DT_I(dtimage_free,(ctl,				/* free it						    */
    &gray));

  return rc;
}

/*..........................................................................*/

dt_rc_e 
dtlev85_cmd_label_callback(				/* called once or each unique label */
  dt_ctl_t *ctl,
  void *arg,
  dtlev85_t *lev85)
{
  my_label_arg_t * myarg =
    (my_label_arg_t *)arg;
  myarg->n++;							/* keep count of labels encountered */
  dt_say(ctl, NULL,
    "  label %5g uniformity %0.5f,"
    " regions %4ld, total area %6ld",
	lev85->label,
    lev85->u,
    lev85->n,
    lev85->Aa);
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_cmd_label(						/* import images and do labels		*/
  dtparam_t *param,
  const char *gray_name,
  const char *segmented_name)
{
  DT_F("dtlev85_cmd_label");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t segmented, gray;			/* image structures					*/
  my_label_arg_t myarg;
  dtos_time_t mark1, mark2;
  dt_rc_e rc;

  gray.xe = 0;
  segmented.xe = 0;

  DT_C(dtimage_import,(ctl,				/* get gray image					*/
    &gray, gray_name));

  DT_C(dtimage_import,(ctl,				/* get segmented image				*/
    &segmented, segmented_name));
  
  myarg.n = 0;
  DT_Q(dtos_time,(ctl, &mark1));
  DT_C(dtlev85_label,(					/* do callback for each label		*/
    param, &gray, &segmented,
    dtlev85_cmd_label_callback,
    &myarg));
  DT_C(dtos_time,(ctl, &mark2));
  
  dt_say(ctl, NULL,
    "%d labels encountered",
    myarg.n);

  DT_C(dtos_time_report,(ctl,			/* report how long it took			*/
    &mark1, &mark2, "dtlev85_label"));

cleanup:
  if (segmented.xe)						/* segmented image got allocated?	*/
    DT_I(dtimage_free,(ctl,				/* free it							*/
      &segmented));

  if (gray.xe)							/* gray image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free it						    */
      &gray));

  return rc;
}
	
/*..........................................................................
| Called for each pixel-blob by dtrun_label().
| This function should illustrate how to compute uniformity.
| If you are already doing some kind of blob computation,
| simply merge the code below into your existing callback routine.
| Building the total uniformity is for example only.
 *..........................................................................*/

static
dt_rc_e
dtlev85_cmd_blob_deliver(				/* per-blob delivery callback  		*/
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dtlev85_cmd_blob_deliver");
  my_blob_arg_t *myarg =				/* extricate our argument 			*/
    (my_blob_arg_t *)arg;
  dtlev85_t lev85;						/* local structure to use 			*/
  dt_stats_t stats;

  DT_Q(dtlev85_init,(myarg->param,		/* init structure for this blob 	*/
    &lev85));

  stats.have = 0;						/* no stats computed yet 			*/
  stats.want = 0;						/* don't want anything extra 		*/

  DT_Q(dtlev85_blob,(					/* start single-blob uniformity		*/
    myarg->param, 
    &lev85, 
    myarg->gray,
    run, blob,
    &stats));

  if ((myarg->minarea == 0 ||
       lev85.Aa >= myarg->minarea) &&	/* blob was big enough? 			*/
	  (myarg->maxarea == 0 ||
	   lev85.Aa <= myarg->maxarea))		/* blob was small enough? 			*/
  {
    double label;
	DT_Q(dtlev85_compute,(myarg->param,	/* compute blob's uniformity		*/
  	  &lev85));

    dt_say(ctl, NULL,					/* print out blob's uniformity 		*/
      "  blob %4ld: label %6g"
      " area %6ld, fmin %6g, fmax %6g,"
      " uniformity %7.5f",
	  myarg->total_lev85.n, 
      blob->label,
      lev85.Aa,
      lev85.fmin,
      lev85.fmax,
      lev85.u);

	DT_Q(dtlev85_combine,(myarg->param,	/* contribute this one-blob region	*/
      &lev85,							/*   to total uniformity we are 	*/
      &myarg->total_lev85,				/*   building 						*/
      &myarg->total_lev85));
  }
  else
    lev85.u = myarg->param->background;
  
  if (myarg->output->xe)				/* we are coloring an output image? */
  {
	dtimage_t *output = myarg->output;	/* for short 						*/
	double color;
	dtrun_id_t p;
	dtrun_elem_t *e;

    if (DT_NTYPE_ISFLOAT(				/* output image is floating? 		*/
		  output->ntype))
      color = lev85.u;					/* color with uniformity directly 	*/
    else								/* output image is integer? 		*/
      color = lev85.u *					/* spread uniformity over 			*/
        dt_ntype_maxval[output->ntype];	/* entire dynamic range 			*/
	
    for (p = blob->phead;				/* point to first run in chain		*/
		 p != DTRUN_NOID;				/* until end of chain 				*/
		 p = e->p)						/* point to next run in chain	    */
	{
	  void *v;
  	  DTRUN_PEEK(DT_Q, ctl,				/* access run in blob chain 	 	*/
        run, p, &e);
	  DTIMAGE_GETROW(ctl, output, e->y,	/* access row 						*/
        &v);
	  dt1d_set(output->ntype, v,		/* plop color values in				*/
        e->x, e->x+e->l+1, color);
	  DTIMAGE_UNMAPROW(ctl, image,		/* release row 						*/
        e->y);
	}
  }
    
  return DT_RC_GOOD;					/* keep doing blobs				    */
}

/*..........................................................................*/

dt_rc_e
dtlev85_cmd_blob(						/* import images and do uniformity	*/
  dtparam_t *param,
  const char *gray_name,
  const char *segmented_name,
  const char *output_name,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtlev85_cmd_blob");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t segmented, gray, output;	/* image structures					*/
  const char *precision =
    DTPARAM_GET_VALUE(precision, 
      "float");
  const char *connect =
    DTPARAM_GET_VALUE(connect, "4");
  dt_ntype_e ntype;
  my_blob_arg_t myarg;
  dt_rc_e rc;
  
  {
	char message[80];
    DT_C(dt_ntype_lookup,(ctl,			/* get desired output precision	    */
      param->precision, &ntype,
      message, sizeof(message)));
    if (message[0])
      return dt_err(ctl, F, "%s", 
        message);
  }

  if (strcmp(connect, "4") &&			/* check ~param->connect~. 		   	*/
	  strcmp(connect, "8"))
	return dt_err(ctl, F,
      "invalid param->connect \"%s\"",
      connect);

  gray.xe = 0;
  segmented.xe = 0;
  output.xe = 0;

  DT_C(dtimage_import,(ctl,				/* get gray image					*/
    &gray, gray_name));

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    &gray, "gray",
    DTLEV85_GRAY_NTYPE_LIST,			/* gray ntypes are constrained		*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    goto cleanup;

  DT_C(dtimage_import,(ctl,				/* get segmented image				*/
    &segmented, segmented_name));

  if (output_name != NULL &&			/* output image desired? 			*/
	  strlen(output_name) != 0)
  {
    DT_C(dtimage_create3,(ctl,			/* get space for output image 		*/
      &output, 0, 1,
      DT_MIN(gray.xe, segmented.xe),	
      DT_MIN(gray.ye, segmented.ye),
      ntype, NULL));
  }

  DT_C(dtlev85_init,(param,				/* prepare for uniformity computing */
    &myarg.total_lev85));
  myarg.param = param;
  myarg.gray = &gray;
  myarg.output = &output;
  myarg.background = DT_MIN(1.0, DT_MAX(0.0,
    DTPARAM_GET_VALUE(background, 
      1.0)));
  myarg.minarea = 
    DT_CAST(unsigned long,
      DTPARAM_GET_VALUE(min, 0));
  myarg.maxarea = 
    DT_CAST(unsigned long,
      DTPARAM_GET_VALUE(max, 0));
  
  DT_C(dtimage_adj_blob,(ctl,			/* do connected components 			*/
    &segmented,							/* connect this image				*/
    0, 0,								/* starting at upper left 			*/
    DT_MIN(gray.xe, segmented.xe),		/* using only intersection 			*/
    DT_MIN(gray.ye, segmented.ye),
    0.0,								/* invisible is don't-care 			*/
	connect[0] == '4'?					/* select connectivity 				*/
      DTIMAGE_ADJ_FLAG_4:
      DTIMAGE_ADJ_FLAG_8,
    dtlev85_cmd_blob_deliver, &myarg));	/* call our function for each blob 	*/

  DT_C(dtlev85_compute,(param,			/* compute total uniformity 		*/
    &myarg.total_lev85));

  dt_say(ctl, NULL,
    "%7ld blobs ------"
    " total area %6ld,"
    " fmin %6g, fmax %6g,"
    " uniformity %7.5f",
    myarg.total_lev85.n, 
    myarg.total_lev85.Aa,
    myarg.total_lev85.fmin,
    myarg.total_lev85.fmax,
    myarg.total_lev85.u);

  if (output.xe)						/* we colored an output image? 		*/
    DT_C(dtimage_export,(ctl,			/* output resulting image 			*/
      &output, output_name, 
      xpos, ypos));

cleanup:
  if (output.xe)						/* output image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free it						    */
      &output));

  if (segmented.xe)						/* segmented image got allocated?	*/
    DT_I(dtimage_free,(ctl,				/* free it							*/
      &segmented));

  if (gray.xe)							/* gray image got allocated?	    */
    DT_I(dtimage_free,(ctl,				/* free it						    */
      &gray));

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
