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

static char rcsid[] = "test $RCSfile: dttran.c,v $ $Revision: 1.2 $";

#include <dtack/t.h>

static dt_rc_e dttplot_bars(dt_ctl_t *);
static dt_rc_e dttplot_lines(dt_ctl_t *);

#define USAGE "[-Dnnnn]"

/*..........................................................................*/
static 
dt_rc_e
dttplot_make_and_clear(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtimage_t *expect,
  long ncols,
  long nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttplot_make_and_clear");
  void *segs[2];
  long segsizes[2];

  segs[0] = data;
  segs[1] = NULL;
  segsizes[0] = nrows;
  segsizes[1] = 0;

  DT_Q(dtimage_create2,(ctl,			/* make expected image			    */
    expect, ncols, nrows, ntype,
    segs, segsizes));

  DT_Q(dtimage_create,(ctl,				/* space for new image			    */
    image, ncols, nrows, ntype));

  DT_Q(dtimage_constant,(ctl,			/* clear new image				    */
    image, 0.0));

  return DT_RC_GOOD;
}
/*..........................................................................*/
static 
dt_rc_e
dttplot_compare_and_free(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtimage_t *expect,
  char *desc)
{
  DT_F("dttplot_compare_and_free");
  long cmprc;

  DT_Q(dtimage_cmp,(ctl,				/* compare images				  	*/
    image, expect, 0,
    &cmprc, NULL, NULL));
  dtt_check(ctl, cmprc == 0,
    "%s %d (0)", desc, cmprc);
  DT_Q(dtimage_free,(ctl,				/* free images					    */
    expect));
  DT_Q(dtimage_free,(ctl,
    image));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttplot)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;
  int n;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dt_interactive_parse,(			/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc != 0)
    return dt_interactive_usage(
      param, USAGE);

  DT_G(dttplot_bars,(ctl));				/* test bars routine(s)			    */
  DT_G(dttplot_lines,(ctl));			/* test line routine(s)			    */

  return DT_RC_GOOD;
}
/*..........................................................................*/

static
dt_rc_e
dttplot_bars(
  dt_ctl_t *ctl)
{
  dtimage_t image, expect;

  {										/* ................................ */
#define N 10
#define NTYPE DT_NTYPE_B08
#define DESC "b08"
    static dt_ntype_b08_t expect_data[NROWS][NCOLS] = {
       0,0,1,0,0,
       0,0,1,0,0,
       0,0,1,0,0,
       0,0,1,0,0,
       0,0,1,0,0,
       1,0,1,0,0,
       1,0,1,0,0,
       1,0,1,0,0,
       1,0,1,0,1,
       1,0,1,1,1,
       1,1,1,1,1,
       0,0,0,0,0,
       0,0,0,0,0,
       0,0,0,0,0,
       0,0,0,0,0,
       0,0,0,0,0,
       0,0,0,0,0,
       0,0,0,0,0,
       0,0,0,0,0
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS+OFFSET, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_double,(ctl, /* make plot					  	*/
      &image, &bars[0], NCOLS, OFFSET,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC));

#undef N
    
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
