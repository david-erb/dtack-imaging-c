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
DT_RCSID("test $RCSfile: test.c,v $ $Revision: 1.6 $");

#include <dt_sez90.h>

#define USAGE "[-Dnnnn]"

#define MAXGRAY 256
#define EPS (1e-5)						/* for comparison of doubles	    */
#define T (1.0/3.0)

#define NO_FLAGS 0
#define DO_ENDS 1

/*..........................................................................*/

dt_rc_e
dt_sez90_test_all(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test";
  dtparam_t param2 = *param;			/* local copy for our use		    */
  dt_ctl_t *ctl = &param2.ctl;

  DT_Q(dt_heapmon_init,(ctl));			/* init memory checking			    */

  param2.n_flag = 0;					/* set params we use to defaults    */
  param2.dx_flag = 0;

  DT_Q(dt_sez90_test01,(&param2, assert_fail));
  DT_Q(dt_sez90_test02,(&param2, assert_fail));
  DT_Q(dt_sez90_test03,(&param2, assert_fail));
  DT_Q(dt_sez90_test04,(&param2, assert_fail));
  DT_Q(dt_sez90_test05,(&param2, assert_fail));
  DT_Q(dt_sez90_test06,(&param2, assert_fail));
  DT_Q(dt_sez90_test07,(&param2, assert_fail));
  DT_Q(dt_sez90_test08,(&param2, assert_fail));
  DT_Q(dt_sez90_test09,(&param2, assert_fail));
  DT_Q(dt_sez90_test10,(&param2, assert_fail));
  DT_Q(dt_sez90_test11,(&param2, assert_fail));
  DT_Q(dt_sez90_test12,(&param2, assert_fail));

  DT_Q(dt_sez90_test31,(&param2, assert_fail));
  DT_Q(dt_sez90_test32,(&param2, assert_fail));
  DT_Q(dt_sez90_test33,(&param2, assert_fail));
  DT_Q(dt_sez90_test34,(&param2, assert_fail));

  DT_Q(dt_sez90_test35,(&param2, assert_fail));

  DT_Q(dt_sez90_test41,(&param2, assert_fail));

  DT_Q(dt_sez90_test51,(&param2, assert_fail));
  DT_Q(dt_sez90_test52,(&param2, assert_fail));

  DT_Q(dt_sez90_test61,(&param2, assert_fail));
  DT_Q(dt_sez90_test62,(&param2, assert_fail));

  DT_Q(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_test_peakcheck(
  dtparam_t *param,
  double *hist,
  int ngray,
  double *pds,
  int *expect_start,
  int *expect_max,
  int *expect_end,
  int flag,
  int npeak,
  char *F,
  int *assert_fail)
{
  dt_ctl_t *ctl = &param->ctl;
  char mark[256];
  char id[32];
  int start, max, end;
  int i;

  DT_Q(dt_sez90_mark,(param, hist,		/* mark complete peaks			    */
    ngray, pds, mark));

  if (flag & DO_ENDS)					/* supposed to do ends analysis?    */
  DT_Q(dt_sez90_ends,(param, hist,
    ngray, mark));

  dt_sez90_mark_dbg(param,				/* print out peak values		    */
    hist, ngray, mark,
    F, DT_DBG_MASK_PIXEL);

  sprintf(id, "%s(%s)", F,
    flag & DO_ENDS? "1": "0");

  end = 0;
  for (i=0; i<npeak; i++)
  {
    DT_Q(dt_sez90_peak_next,(param,
      mark, ngray, end,
      &start, &max, &end));
    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl, 
        start == expect_start[i] &&
        max == expect_max[i] &&
        end == expect_end[i],
        id, "%d. start %2ld (%2ld), max %2ld (%2ld), end %2ld (%2ld)",
        i, start, expect_start[i],
        max, expect_max[i],
        end, expect_end[i]);
  }
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_test_pdscheck(
  dtparam_t *param,
  int n,
  double *hist,
  int ngray,
  double *expect_pds,
  char *F,
  int *assert_fail)
{
  dt_ctl_t *ctl = &param->ctl;
  double pds[MAXGRAY];
  int a = *assert_fail;
  int i;

  DTPARAM_DEFAULT_P(param, n, n);		/* set n parameter				    */

  DT_Q(dt_sez90_pds,(param,				/* calculate the pds			    */
    hist, ngray, pds));
  for (i=0; i<ngray; i++)
  {
	if (fabs(pds[i] - expect_pds[i]) < EPS)
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
		"  pds[%2d]. %6.3f (%6.3f)",
        i, pds[i], expect_pds[i]);
    else
      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 0, F,
		  "pds[%2d]. %6.3f (%6.3f)",
          i, pds[i], expect_pds[i]);
  }

  if (a == *assert_fail)				/* nothing failed?				    */
    dt_assert(ctl, 1, F,				/* give assertion to mark passage	*/
      "%d elements correct", i);

  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test01(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test01";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 16
#define NPEAK 4
  static double hist[NGRAY] = {
    0, 0, 0,  0,  0,  0, 1, 0, 0, 0,  0,  0, 1, 0, 0, 1};
  static double pds[NGRAY] = {
    0, 0, 1, -1, -2, -1, 1, 2, 3, 2, -1, -2, 1, 0, 0, -1};
  static int start[NPEAK] = {3, 10, 16, 16};
  static int max[NPEAK]   = {6, 12, 16, 16};
  static int end[NPEAK]   = {8, 12, 16, 16};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_test02(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test02";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 19
#define NPEAK 4
  static double hist[NGRAY] = {
    0, 0,  0,  1,  2, 1, 0, 0,  0,  0,  1, 2, 0, 0,  0,  0,  0, 0, 0};
  static double pds[NGRAY] = {
    0, 0, -1, -2, -1, 1, 2, 1, -1, -2, -1, 1, 2, 1, -1, -2, -1, 0, 0};
  static int startn[NPEAK] = { 8, 19, 19, 19};
  static int maxn[NPEAK]   = {11, 19, 19, 19};
  static int endn[NPEAK]   = {12, 19, 19, 19};

  static int starte[NPEAK] = {0,  8, 14, 19};
  static int maxe[NPEAK]   = {5, 11, 18, 19};
  static int ende[NPEAK]   = {6, 12, 18, 19};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    startn, maxn, endn, NO_FLAGS,
    NPEAK, F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    starte, maxe, ende, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test03(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test03";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 19
#define NPEAK 4
  static double hist[NGRAY] = {
    0, 0, 0, 0, 0,  0,  0,  0, 1, 0, 0,  0,  0,  2, 1, 0, 0, 0, 0};
  static double pds[NGRAY] = {
    0, 0, 1, 2, 1, -1, -2, -1, 1, 2, 1, -1, -2, -1, 1, 2, 1, 0, -1};
  static int start[NPEAK] = {5, 11, 19, 19};
  static int max[NPEAK]   = {8, 14, 19, 19};
  static int end[NPEAK]   = {9, 15, 19, 19};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test04(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test04";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 10
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static double pds[NGRAY] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static int start[NPEAK] = {10, 10, 10};
  static int max[NPEAK]   = {10, 10, 10};
  static int end[NPEAK]   = {10, 10, 10};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test05(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test05";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 10
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static double pds[NGRAY] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  static int start[NPEAK] = {10, 10, 10};
  static int max[NPEAK]   = {10, 10, 10};
  static int end[NPEAK]   = {10, 10, 10};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test06(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test06";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 10
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  static double pds[NGRAY] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,};
  static int start[NPEAK] = {10, 10, 10};
  static int max[NPEAK]   = {10, 10, 10};
  static int end[NPEAK]   = {10, 10, 10};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test07(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test07";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 9
#define NPEAK 3
  static double hist[NGRAY] = {
    1, 0, 0, 0, 0, 0, 0, 1, 0};
  static double pds[NGRAY] = {
    0, -1, -1, 1, 1, 1, -1, -1, -1};
  static int startn[NPEAK] = {9, 9, 9};
  static int maxn[NPEAK]   = {9, 9, 9};
  static int endn[NPEAK]   = {9, 9, 9};

  static int starte[NPEAK] = {0, 6, 9};
  static int maxe[NPEAK]   = {3, 8, 9};
  static int ende[NPEAK]   = {3, 8, 9};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    startn, maxn, endn, NO_FLAGS,
    NPEAK, F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    starte, maxe, ende, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test08(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test08";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 9
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0, 0, 1, 0, 0, 0, 0};
  static double pds[NGRAY] = {
    1, 1, 1, -1, -1, -1, 1, 1, 1};
  static int start[NPEAK] = {9, 9, 9};
  static int max[NPEAK]   = {9, 9, 9};
  static int end[NPEAK]   = {9, 9, 9};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test09(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test09";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 20
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0,   0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  1,  0};
  static double pds[NGRAY] = {
    1, 1, -1, -1, 0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 1, 0, -1, -1, -1};
  static int start[NPEAK] = { 2, 20, 20};
  static int max[NPEAK]   = { 4, 20, 20};
  static int end[NPEAK]   = { 7, 20, 20};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test10(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test10";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 17
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0,  1,  1,  1,  0,  0, 0,  0,  0,  1,  0,  0, 0, 0, 0};
  static double pds[NGRAY] = {
    1, 1, 0, -1, -2, -3, -2, -1, 0, -1, -2, -3, -2, -1, 0, 1, -1};
  static int start[NPEAK] = {2,   17, 17};
  static int max[NPEAK]   = {14,  17, 17};
  static int end[NPEAK]   = {15,  17, 17};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test11(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test11";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 15
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0,  0,  0,  0, 1, 0, 0,  0,  1,  0, 0, 0, 0};
  static double pds[NGRAY] = {
    1, 1, 0, -1, -2, -1, 0, 1, 0, -1, -2, -1, 0, 1, -1};
  static int start[NPEAK] = {2,  8, 15};
  static int max[NPEAK]   = {6, 12, 15};
  static int end[NPEAK]   = {7, 13, 15};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test12(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test12";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 11
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0,  0,  0,  0, 1,  0,  0,  1, 0, 0};
  static double pds[NGRAY] = {
    1, 1, -1, -2, -1, 1, -1, -2, -1, 1, -1};
  static int start[NPEAK] = {2,  6, 11};
  static int max[NPEAK]   = {5,  9, 11};
  static int end[NPEAK]   = {5,  9, 11};

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NGRAY
#undef NPEAK
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test31(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test31";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 12
#define N 3
  static double hist[NGRAY] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  static double pds[NGRAY] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, hist, NGRAY, pds,
    F, assert_fail));

#undef NGRAY
#undef N
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test32(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test32";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 11
#define N 3
  static double hist[NGRAY] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  static double pds[NGRAY] = {
    0, 
    -1.0/3.0, -1.0/3.0, -1.0/3.0,
    -1.0/3.0, -1.0/3.0, -1.0/3.0,
    -1.0/3.0, -1.0/3.0, -1.0/3.0,
    0};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, hist, NGRAY, pds,
    F, assert_fail));

#undef NGRAY
#undef N
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test33(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test33";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 13
#define N 3
#define NPEAK 3
  static double hist[NGRAY] = {
    3, 2, 1, 2, 3, 4, 5, 4, 3, 2, 1, 2, 3};
  static double pds[NGRAY] = {
    0,
    1.0/3.0,
    -1.0/3.0, 
    -1.0/3.0, -1.0/3.0, -1.0/3.0,
    1.0/3.0,   1.0/3.0,  1.0/3.0, 1.0/3.0,
    -1.0/3.0, 
    -1.0/3.0, 
    0};
  static int start[NPEAK] = {2, 13, 13};
  static int max[NPEAK]   = {6, 13, 13};
  static int end[NPEAK]   = {6, 13, 13};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, hist, NGRAY, pds,
    F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
#undef NGRAY
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test34(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test34";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 9
#define N 3
#define NPEAK 3
  static double hist[NGRAY] = {
    0, 0, 0, 0, 1, 0, 0, 0, 1};
  static double pds[NGRAY] = {
    0, 0, 0, -1.0/3.0, 1.0/3.0, 0, 0, -1.0/3.0, 0};
  static int startn[NPEAK] = {9, 9, 9};
  static int maxn[NPEAK]   = {9, 9, 9};
  static int endn[NPEAK]   = {9, 9, 9};
  static int starte[NPEAK] = {0, 5, 9};
  static int maxe[NPEAK]   = {4, 8, 9};
  static int ende[NPEAK]   = {4, 8, 9};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, hist, NGRAY, pds,
    F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    startn, maxn, endn, NO_FLAGS,
    NPEAK, F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    starte, maxe, ende, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
#undef NGRAY
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test35(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test35";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 16
#define N 3
#define NPEAK 3
  static double hist[NGRAY] = {
    0,  0,  1,  2, 3, 2, 1,  0,  0,  1,  2, 3, 2, 1, 0, -1};
  static double pds[NGRAY] = {
    0, -T, -T, -T, T, T, T,  0, -T, -T, -T, T, T, T, T, 0};
  static int startn[NPEAK] = {16, 16, 16};
  static int maxn[NPEAK]   = {16, 16, 16};
  static int endn[NPEAK]   = {16, 16, 16};

  static int starte[NPEAK] = {0, 7, 16};
  static int maxe[NPEAK]   = {4, 11, 16};
  static int ende[NPEAK]   = {4, 15, 16};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, hist, NGRAY, pds,
    F, assert_fail));

  DTPARAM_DEFAULT_P(param, dx, 1);		/* set closeness parameter		    */

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    startn, maxn, endn, NO_FLAGS,
    NPEAK, F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    hist, NGRAY, pds,
    starte, maxe, ende, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
#undef NGRAY
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test41(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test41";
  dt_ctl_t *ctl = &param->ctl;
#define NGRAY 17
#define N 3
#define NPEAK 4
  static double hist[NGRAY] = {
    0,  0,  1,  2, 3, 2, 1,  0,  0,  1,   2,   4,  2, 1, 0, 0, 1};
  static double pds[NGRAY] = {
    0, -T, -T, -T, T, T, T,  0, -T, -T, -2*T, 2*T, T, T, 0, -T, 0};
  static int startn[NPEAK] = { 7, 17, 17, 17};
  static int maxn[NPEAK]   = {11, 17, 17, 17};
  static int endn[NPEAK]   = {11, 17, 17, 17};

  static int starte[NPEAK] = { 0,  7, 14, 17};
  static int maxe[NPEAK]   = { 4, 11, 16, 17};
  static int ende[NPEAK]   = { 4, 11, 16, 17};

  static int startm[NPEAK] = { 0, 17, 17, 17};
  static int maxm[NPEAK]   = {11, 17, 17, 17};
  static int endm[NPEAK]   = {16, 17, 17, 17};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, hist, NGRAY, pds,
    F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,	/* no end peaks					    */
    hist, NGRAY, pds,
    startn, maxn, endn, NO_FLAGS,
    NPEAK, F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,	/* end peaks before merging		    */
    hist, NGRAY, pds,
    starte, maxe, ende, DO_ENDS,
    NPEAK, F, assert_fail));

  DTPARAM_DEFAULT_P(param, dx, 3);		/* set closeness parameter		    */

  DT_Q(dt_sez90_test_peakcheck,(param,	/* end peaks after merging		    */
    hist, NGRAY, pds,
    startm, maxm, endm, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
#undef NGRAY
    
  return DT_RC_GOOD;
}


/*.........................................................................
 * the numbers below were calculated and printed by:
 *   sez901a -Dpixel -w 64 -n 15 :plot
 *.........................................................................	*/

#define NSINE1 64
static double sine1_hist[NSINE1] = {
 1.00000, 1.19509, 1.38268, 1.55557, 1.70711, 1.83147, 1.92388, 1.98079,
 2.00000, 1.98079, 1.92388, 1.83147, 1.70711, 1.55557, 1.38268, 1.19509,
 1.00000, 0.80491, 0.61732, 0.44443, 0.29289, 0.16853, 0.07612, 0.01921,
 0.00000, 0.01921, 0.07612, 0.16853, 0.29289, 0.44443, 0.61732, 0.80491,
 1.00000, 1.19509, 1.38268, 1.55557, 1.70711, 1.83147, 1.92388, 1.98079,
 2.00000, 1.98079, 1.92388, 1.83147, 1.70711, 1.55557, 1.38268, 1.19509,
 1.00000, 0.80491, 0.61732, 0.44443, 0.29289, 0.16853, 0.07612, 0.01921,
 0.00000, 0.01921, 0.07612, 0.16853, 0.29289, 0.44443, 0.61732, 0.80491
};
static double sine1_pds[NSINE1] = {
 0.00000,-0.06253,-0.17156,-0.29728,-0.40039,-0.43773,-0.36872,-0.16156,
 0.16156, 0.47847, 0.77700, 1.04567, 1.27415, 1.45366, 1.57732, 1.64036,
 1.64036, 1.57732, 1.45366, 1.27415, 1.04567, 0.77700, 0.47847, 0.16156,
-0.16156,-0.47847,-0.77700,-1.04567,-1.27415,-1.45366,-1.57732,-1.64036,
-1.64036,-1.57732,-1.45366,-1.27415,-1.04567,-0.77700,-0.47847,-0.16156,
 0.16156, 0.47847, 0.77700, 1.04567, 1.27415, 1.45366, 1.57732, 1.64036,
 1.64036, 1.57732, 1.45366, 1.27415, 1.04567, 0.77700, 0.47847, 0.16156,
-0.16156,-0.36872,-0.43773,-0.40039,-0.29728,-0.17156,-0.06253, 0.00000
};

/*..........................................................................*/

dt_rc_e
dt_sez90_test51(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test51";
  dt_ctl_t *ctl = &param->ctl;
#define N 15
#define NPEAK 3
  static int start[NPEAK] = {24, 64, 64};
  static int max[NPEAK]   = {40, 64, 64};
  static int end[NPEAK]   = {47, 64, 64};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, sine1_hist, NSINE1, sine1_pds,
    F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    sine1_hist, NSINE1, sine1_pds,
    start, max, end, NO_FLAGS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dt_sez90_test52(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test51";
  dt_ctl_t *ctl = &param->ctl;
#define N 15
#define NPEAK 5
  static int start[NPEAK] = {0,  24, 56, 64, 64};
  static int max[NPEAK]   = {8,  40, 63, 64, 64};
  static int end[NPEAK]   = {15, 47, 63, 64, 64};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, sine1_hist, NSINE1, sine1_pds,
    F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    sine1_hist, NSINE1, sine1_pds,
    start, max, end, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
    
  return DT_RC_GOOD;
}

/*.........................................................................
 * the numbers below were calculated and printed by:
 *   sez901a -Dpixel -w 128 -n 7 -omega 6 :plot
 *.........................................................................	*/

#define NSINE2 128
static double sine2_hist[NSINE2] = {
 1.00000, 1.29028, 1.55557, 1.77301, 1.92388, 1.99518, 1.98079, 1.88192,
 1.70711, 1.47140, 1.19509, 0.90198, 0.61732, 0.36561, 0.16853, 0.04306,
 0.00000, 0.04306, 0.16853, 0.36561, 0.61732, 0.90198, 1.19509, 1.47140,
 1.70711, 1.88192, 1.98079, 1.99518, 1.92388, 1.77301, 1.55557, 1.29028,
 1.00000, 0.70972, 0.44443, 0.22699, 0.07612, 0.00482, 0.01921, 0.11808,
 0.29289, 0.52860, 0.80491, 1.09802, 1.38268, 1.63439, 1.83147, 1.95694,
 2.00000, 1.95694, 1.83147, 1.63439, 1.38268, 1.09802, 0.80491, 0.52860,
 0.29289, 0.11808, 0.01921, 0.00482, 0.07612, 0.22699, 0.44443, 0.70972,
 1.00000, 1.29028, 1.55557, 1.77301, 1.92388, 1.99518, 1.98079, 1.88192,
 1.70711, 1.47140, 1.19509, 0.90198, 0.61732, 0.36561, 0.16853, 0.04306,
 0.00000, 0.04306, 0.16853, 0.36561, 0.61732, 0.90198, 1.19509, 1.47140,
 1.70711, 1.88192, 1.98079, 1.99518, 1.92388, 1.77301, 1.55557, 1.29028,
 1.00000, 0.70972, 0.44443, 0.22699, 0.07612, 0.00482, 0.01921, 0.11808,
 0.29289, 0.52860, 0.80491, 1.09802, 1.38268, 1.63439, 1.83147, 1.95694,
 2.00000, 1.95694, 1.83147, 1.63439, 1.38268, 1.09802, 0.80491, 0.52860,
 0.29289, 0.11808, 0.01921, 0.00482, 0.07612, 0.22699, 0.44443, 0.70972
};
static double sine2_pds[NSINE2] = {
 0.00000,-0.08843,-0.21370,-0.28891,-0.13655, 0.02757, 0.18932, 0.33476,
 0.45137, 0.52911, 0.56128, 0.54512, 0.48201, 0.37739, 0.24027, 0.08246,
-0.08246,-0.24027,-0.37739,-0.48201,-0.54512,-0.56128,-0.52911,-0.45137,
-0.33476,-0.18932,-0.02757, 0.13655, 0.28891, 0.41639, 0.50801, 0.55588,
 0.55588, 0.50801, 0.41639, 0.28891, 0.13655,-0.02757,-0.18932,-0.33476,
-0.45137,-0.52911,-0.56128,-0.54512,-0.48201,-0.37739,-0.24027,-0.08246,
 0.08246, 0.24027, 0.37739, 0.48201, 0.54512, 0.56128, 0.52911, 0.45137,
 0.33476, 0.18932, 0.02757,-0.13655,-0.28891,-0.41639,-0.50801,-0.55588,
-0.55588,-0.50801,-0.41639,-0.28891,-0.13655, 0.02757, 0.18932, 0.33476,
 0.45137, 0.52911, 0.56128, 0.54512, 0.48201, 0.37739, 0.24027, 0.08246,
-0.08246,-0.24027,-0.37739,-0.48201,-0.54512,-0.56128,-0.52911,-0.45137,
-0.33476,-0.18932,-0.02757, 0.13655, 0.28891, 0.41639, 0.50801, 0.55588,
 0.55588, 0.50801, 0.41639, 0.28891, 0.13655,-0.02757,-0.18932,-0.33476,
-0.45137,-0.52911,-0.56128,-0.54512,-0.48201,-0.37739,-0.24027,-0.08246,
 0.08246, 0.24027, 0.37739, 0.48201, 0.54512, 0.56128, 0.52911, 0.45137,
 0.33476, 0.18932, 0.02757,-0.13655,-0.28891,-0.21370,-0.08843, 0.00000
};


/*..........................................................................*/

dt_rc_e
dt_sez90_test61(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test61";
  dt_ctl_t *ctl = &param->ctl;
#define N 7
#define NPEAK 8
  static int start[NPEAK] = { 0, 16, 37, 59, 80, 101, 123, 128};
  static int max[NPEAK]   = { 5, 27, 48, 69, 91, 112, 127, 128};
  static int end[NPEAK]   = {10, 31, 53, 74, 95, 117, 127, 128};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, sine2_hist, NSINE2, sine2_pds,
    F, assert_fail));

  DT_Q(dt_sez90_test_peakcheck,(param,
    sine2_hist, NSINE2, sine2_pds,
    start, max, end, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
    
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e
dt_sez90_test62(
  dtparam_t *param,
  int *assert_fail)
{
  static char *F = "dt_sez90_test62";
  dt_ctl_t *ctl = &param->ctl;
#define N 7
#define NPEAK 2
  static int start[NPEAK] = {  0, 128};
  static int max[NPEAK]   = { 48, 128};
  static int end[NPEAK]   = {127, 128};

  DT_Q(dt_sez90_test_pdscheck,(param,
    N, sine2_hist, NSINE2, sine2_pds,
    F, assert_fail));

  DTPARAM_DEFAULT_P(param, dx, 7);		/* set closeness parameter		    */

  DT_Q(dt_sez90_test_peakcheck,(param,
    sine2_hist, NSINE2, sine2_pds,
    start, max, end, DO_ENDS,
    NPEAK, F, assert_fail));

#undef NPEAK
#undef N
    
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
