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
DT_RCSID("lev85 $RCSfile: test.c,v $ $Revision: 1.7 $");
#include <dtlev85.h>
#include <dtack/stack.h>
#include <dtack/mem.h>

#include <dtack/str.h>

#define USAGE "[-Dnnnn]"

#define LABEL_ARG_MAX (20)
typedef struct {
  int count;
  double v[LABEL_ARG_MAX];
  double u[LABEL_ARG_MAX];
  long   n[LABEL_ARG_MAX];
} label_arg_t;

#define HIST_ARG_MAX (20)
typedef struct {
  dtparam_t *param;
  dtimage_t *gray;
  dtimage_t *segmented;
  dthist_t *hist;
  dtlev85_t lev85;
  int count;					
  unsigned long p[HIST_ARG_MAX];
  double v[HIST_ARG_MAX];
  double u[HIST_ARG_MAX];
  long   a[HIST_ARG_MAX];
} hist_arg_t;

#define BLOB_ARG_MAX (20)
typedef struct {
  dtparam_t *param;
  dtimage_t *gray;
  dtimage_t *segmented;
  int count;					
  unsigned long p[BLOB_ARG_MAX];
  double v[BLOB_ARG_MAX];
  double u[BLOB_ARG_MAX];
  long   a[BLOB_ARG_MAX];
} blob_arg_t;

#define EPS (0.0000001)

#define TOTAL(GRAY_NTYPE, SEGMENTED_NTYPE, BACKGROUND, UNIFORMITY, NREGIONS) \
  DT_Q(dtlev85_test_total,(param, \
    gray_data, \
    GRAY_NTYPE, \
    segmented_data, \
    SEGMENTED_NTYPE, \
    NCOLS, NROWS, \
    BACKGROUND, \
    UNIFORMITY, \
    NREGIONS, \
    assert_fail, F));

#define LABEL(GRAY_NTYPE, SEGMENTED_NTYPE) \
  DT_Q(dtlev85_test_label,(param, \
    gray_data, \
    GRAY_NTYPE, \
    segmented_data, \
    SEGMENTED_NTYPE, \
    NCOLS, NROWS, \
    nlabels, \
    label_value, \
    label_uniformity, \
    label_nregions, \
    assert_fail, F));

#define HIST(GRAY_NTYPE, SEGMENTED_NTYPE, N) \
  DT_Q(dtlev85_test_hist,(param, \
    gray_data, \
    GRAY_NTYPE, \
    segmented_data, \
    SEGMENTED_NTYPE, \
    NCOLS, NROWS, \
    background ## N, \
    nblobs ## N, \
    label ## N, \
    uniformity ## N, \
    assert_fail, F));

#define BLOB(GRAY_NTYPE, SEGMENTED_NTYPE, N) \
  DT_Q(dtlev85_test_blob,(param, \
    gray_data, \
    GRAY_NTYPE, \
    segmented_data, \
    SEGMENTED_NTYPE, \
    NCOLS, NROWS, \
    background ## N, \
    nblobs ## N, \
    label ## N, \
    uniformity ## N, \
    assert_fail, F));

/*..........................................................................*/

int
dtlev85_test_all(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test");
  dt_ctl_t *ctl = &param->ctl;

    DT_Q(dtlev85_test1,(param, 
      assert_fail));

    DT_Q(dtlev85_test2,(param,
      assert_fail));

    DT_Q(dtlev85_test3,(param,
      assert_fail));

    DT_Q(dtlev85_test4,(param,
      assert_fail));

    DT_Q(dtlev85_test5,(param,
      assert_fail));

    DT_Q(dtlev85_test6,(param,
      assert_fail));

  return DT_RC_GOOD;
}


/*..........................................................................*/

static
dt_rc_e
dtlev85_test_sort(
  dt_ctl_t *ctl,
  unsigned long *p,
  int n,
  int *sorted)
{
  int i, j;
  for (i=0; i<n; i++)
  {
	int first = 1;
    for (j=0; j<n; j++)
	  if (p[j] != 0xffffffffUL)
	    if (first || p[j] < p[sorted[i]])
		{
          sorted[i] = j;
		  first = 0;
		}
	p[sorted[i]] = 0xffffffffUL;
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dtlev85_test_make(
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned int ncols,
  unsigned int nrows,
  dt_ntype_e image_ntype,
  void *data,
  dt_ntype_e data_ntype)
{
  static char *F = "dtlev85_test_make";
  dtxy_t y;
  unsigned int rowsize = (unsigned int)
    (ncols * dt_ntype_sizeof[data_ntype]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    image, ncols, nrows, 
    image_ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, image, y, &p);
	DT_Q(dt1d_cast,(
      data, ncols, data_ntype,
      p, ncols, image_ntype));
    DTIMAGE_PUTROW(ctl, image, y);
    data = (char *)data + rowsize;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_test_total(
  dtparam_t *param,
  dt_ntype_b08_t *gray_data,
  dt_ntype_e gray_ntype,
  dt_ntype_b08_t *segmented_data,
  dt_ntype_e segmented_ntype,
  dtxy_t ncols,
  dtxy_t nrows,
  double background,
  double expect_uniformity,
  long   expect_nregions,
  int *assert_fail,
  char *f)
{
  static char *F = "dtlev85_test_total";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t gray;
  dtimage_t segmented;
  dtlev85_t lev85;
  dt_rc_e rc;

  segmented.xe = 0;
  gray.xe = 0;

  DT_C(dtlev85_test_make,(ctl,			/* make gray image				    */
    &gray, ncols, nrows, 
    gray_ntype,
    gray_data, DT_NTYPE_B08));

  DT_C(dtlev85_test_make,(ctl,			/* make segmented image			    */
    &segmented, ncols, nrows, 
    segmented_ntype,
    segmented_data, DT_NTYPE_B08));

  DT_C(dtlev85_total,(param,			/* do algorithm						*/
    &gray, &segmented, background,
    &lev85));

  *assert_fail += DT_RC_GOOD != dt_assert(ctl,
    fabs(expect_uniformity - lev85.u) < EPS &&
    expect_nregions == lev85.n,
    f, 
    "%-3s %-3s total uniformity %0.7f (%0.7f),"
    " nregions %ld (%ld)",
    dt_ntype_string[gray_ntype],
    dt_ntype_string[segmented_ntype],
    lev85.u, expect_uniformity,
    lev85.n, expect_nregions);

cleanup:
  if (segmented.xe != 0)
    DT_I(dtimage_free,(ctl,
      &segmented));
  if (gray.xe != 0)
    DT_I(dtimage_free,(ctl,
      &gray));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dtlev85_test_label_callback(	
  dt_ctl_t *ctl,						/* environment control structure 	*/
  void *callback_arg,					/* passed through from application 	*/
  dtlev85_t *lev85)						/* input computed uniformity 		*/
{
  label_arg_t *arg = 
    (label_arg_t *)callback_arg;
  if (arg->count < LABEL_ARG_MAX)
  {
    arg->v[arg->count] = lev85->label;
    arg->u[arg->count] = lev85->u;
    arg->n[arg->count] = lev85->n;
  }
  arg->count++;
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_test_label(
  dtparam_t *param,
  dt_ntype_b08_t *gray_data,
  dt_ntype_e gray_ntype,
  dt_ntype_b08_t *segmented_data,
  dt_ntype_e segmented_ntype,
  dtxy_t ncols,
  dtxy_t nrows,
  int    nlabels,
  double *expect_value,
  double *expect_uniformity,
  long   *expect_nregions,
  int *assert_fail,
  char *f)
{
  static char *F = "dtlev85_test_label";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t gray;
  dtimage_t segmented;
  label_arg_t label_arg;
  int i;
  dt_rc_e rc;

  segmented.xe = 0;
  gray.xe = 0;

  DT_C(dtlev85_test_make,(ctl,			/* make gray image				    */
    &gray, ncols, nrows, 
    gray_ntype,
    gray_data, DT_NTYPE_B08));

  DT_C(dtlev85_test_make,(ctl,			/* make segmented image			    */
    &segmented, ncols, nrows, 
    segmented_ntype,
    segmented_data, DT_NTYPE_B08));

  memset(&label_arg, 0, 
    sizeof(label_arg));
  
  DT_C(dtlev85_label,(param,			/* do algorithm						*/
    &gray, &segmented,
    dtlev85_test_label_callback, 
    &label_arg));

  for (i=0; i<DT_MIN(nlabels, label_arg.count); i++)
  {
    *assert_fail += DT_RC_GOOD != dt_assert(ctl,
      expect_value[i] == label_arg.v[i] &&
      expect_uniformity[i] == label_arg.u[i] &&
      expect_nregions[i] == label_arg.n[i],
      f, 
      "%-3s %-3s label[%d] %g (%g),"
      " uniformity %0.5f (%0.5f),"
      " nregions %ld (%ld)",
      dt_ntype_string[gray_ntype],
      dt_ntype_string[segmented_ntype],
      i,
      label_arg.v[i], expect_value[i],
      label_arg.u[i], expect_uniformity[i],
      label_arg.n[i], expect_nregions[i]);
  }

  *assert_fail += DT_RC_GOOD != dt_assert(ctl,
    nlabels == label_arg.count,
    f, "%-3s %-3s label count %d (%d)",
    dt_ntype_string[gray_ntype],
    dt_ntype_string[segmented_ntype],
    label_arg.count, nlabels);

cleanup:
  if (segmented.xe != 0)
    DT_I(dtimage_free,(ctl,
      &segmented));
  if (gray.xe != 0)
    DT_I(dtimage_free,(ctl,
      &gray));
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtlev85_test_hist_deliver(	
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dtlev85_test_hist_deliver");
  hist_arg_t *hist_arg =				/* extricate our argument 			*/
    (hist_arg_t *)arg;
  dtrun_elem_t *e;
  double label;
  dtlev85_t lev85;

  DTRUN_PEEK(DT_Q, ctl, run,			/* access head run in blob chain 	*/
	blob->phead, &e);
  DTIMAGE_GETPIX1(DT_Q, ctl,			/* fetch pixel 						*/
    hist_arg->segmented,				/* from image being labeled 		*/
    e->x, e->y, &label);

  DT_Q(dthist_clear,(ctl,				/* clear histogram 					*/
    hist_arg->hist));

  DT_Q(dtimage_blob_hist,(ctl,			/* compute histogram of blob 		*/
    hist_arg->gray, run, blob,
    hist_arg->hist));

  DT_Q(dtlev85_init,(hist_arg->param,	/* init new lev85 structure 		*/
    &lev85));

  DT_Q(dtlev85_hist,(					/* add histogram to uniformity		*/
    hist_arg->param, 
    &lev85, hist_arg->hist));

  DT_Q(dtlev85_compute,(				/* compute final uniformity			*/
    hist_arg->param, &lev85)); 

  if (hist_arg->count < HIST_ARG_MAX)	/* record results for this blob 	*/
  {
	hist_arg->p[hist_arg->count] = 
      (unsigned long)e->x + 
      (unsigned long)e->y * 
      (unsigned long)hist_arg->gray->xe;
    hist_arg->u[hist_arg->count] = lev85.u;
    hist_arg->a[hist_arg->count] = lev85.Aa;
	hist_arg->v[hist_arg->count] = label;
  }

  hist_arg->count++;

  return DT_RC_GOOD;
}
/*..........................................................................*/

dt_rc_e
dtlev85_test_hist(
  dtparam_t *param,
  dt_ntype_b08_t *gray_data,
  dt_ntype_e gray_ntype,
  dt_ntype_b08_t *segmented_data,
  dt_ntype_e segmented_ntype,
  dtxy_t ncols,
  dtxy_t nrows,
  double background,
  int    nblobs,
  double *expect_label,
  double *expect_uniformity,
  int *assert_fail,
  char *f)
{
  static char *F = "dtlev85_test_hist";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t gray;
  dtimage_t segmented;
  hist_arg_t hist_arg;
  dthist_t hist;
  int sorted[HIST_ARG_MAX];
  int j;
  dt_rc_e rc;

  segmented.xe = 0;
  gray.xe = 0;
  hist.freq = NULL;

  DT_C(dtlev85_test_make,(ctl,			/* make gray image				    */
    &gray, ncols, nrows, 
    gray_ntype,
    gray_data, DT_NTYPE_B08));

  DT_C(dtlev85_test_make,(ctl,			/* make segmented image			    */
    &segmented, ncols, nrows, 
    segmented_ntype,
    segmented_data, DT_NTYPE_B08));

  DT_C(dtimage_hist_alloc,(ctl,			/* get space for histogram 			*/
    &gray, &hist));

  memset(&hist_arg, 0,					
    sizeof(hist_arg));
  hist_arg.param = param;				/* init communication structure 	*/
  hist_arg.gray = &gray;
  hist_arg.segmented = &segmented;
  hist_arg.hist = &hist;

  DT_C(dtimage_adj_blob,(ctl,			/* find and deliver blobs			*/
    &segmented,
    0, 0, gray.xe, gray.ye,			
    background, DTIMAGE_ADJ_FLAG_NOT,
    dtlev85_test_hist_deliver,		
    &hist_arg));

  dtlev85_test_sort(ctl, hist_arg.p,	/* sort by blob pixel position 		*/
    DT_MIN(hist_arg.count, 
    HIST_ARG_MAX),
    sorted);

  for (j=0; j<DT_MIN(nblobs, hist_arg.count); j++)
  {
    int i = sorted[j];
    *assert_fail += DT_RC_GOOD != dt_assert(ctl,
      expect_label[j] == hist_arg.v[i] &&
      expect_uniformity[j] == hist_arg.u[i],
      f, 
      "%-3s %-3s hist[%d] %g (%g),"
      " uniformity %0.5f (delta %e)",
      dt_ntype_string[gray_ntype],
      dt_ntype_string[segmented_ntype],
      i,
      hist_arg.v[i], expect_label[j],
      hist_arg.u[i],
      hist_arg.u[i]-expect_uniformity[j]);
  }

  *assert_fail += DT_RC_GOOD != dt_assert(ctl,
    nblobs == hist_arg.count,
    f, "%-3s %-3s hist count %d (%d)",
    dt_ntype_string[gray_ntype],
    dt_ntype_string[segmented_ntype],
    hist_arg.count, nblobs);

cleanup:
  if (hist.freq != NULL)
    DT_I(dthist_free,(ctl,
      &hist));
  if (segmented.xe != 0)
    DT_I(dtimage_free,(ctl,
      &segmented));
  if (gray.xe != 0)
    DT_I(dtimage_free,(ctl,
      &gray));
  return rc;
}


/*..........................................................................*/
static
dt_rc_e
dtlev85_test_blob_deliver(	
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dtlev85_test_blob_deliver");
  blob_arg_t *blob_arg =				/* extricate our argument 			*/ 
    (blob_arg_t *)arg;
  dtrun_elem_t *e;
  double label;
  dtlev85_t lev85;
  dt_stats_t stats;

  DTRUN_PEEK(DT_Q, ctl, run,			/* access head run in blob chain 	*/
	blob->phead, &e);
  DTIMAGE_GETPIX1(DT_Q, ctl,			/* fetch pixel 						*/
    blob_arg->segmented,				/* from image being labeled 		*/
    e->x, e->y, &label);

  DT_Q(dtlev85_init,(blob_arg->param,	/* init new lev85 structure 		*/
    &lev85));

  stats.have = 0;						/* no stats computed yet 			*/
  stats.want = 0;						/* don't want anything extra 		*/

  DT_Q(dtlev85_blob,(					/* simply add blob to this lev85 	*/
    blob_arg->param, 
    &lev85,
	blob_arg->gray,
    run, blob, &stats));

  DT_Q(dtlev85_compute,(				/* compute blob's uniformity		*/
    blob_arg->param, &lev85)); 

  if (blob_arg->count < BLOB_ARG_MAX)	/* record result for this blob 		*/
  {
	blob_arg->p[blob_arg->count] = 
      (unsigned long)e->x + 
      (unsigned long)e->y * 
      (unsigned long)blob_arg->gray->xe;
    blob_arg->u[blob_arg->count] = lev85.u;
    blob_arg->a[blob_arg->count] = lev85.Aa;
	blob_arg->v[blob_arg->count] = label;
  }

  blob_arg->count++;

  return DT_RC_GOOD;
}
/*..........................................................................*/
dt_rc_e
dtlev85_test_blob(
  dtparam_t *param,
  dt_ntype_b08_t *gray_data,
  dt_ntype_e gray_ntype,
  dt_ntype_b08_t *segmented_data,
  dt_ntype_e segmented_ntype,
  dtxy_t ncols,
  dtxy_t nrows,
  double background,
  int    nblobs,
  double *expect_label,
  double *expect_uniformity,
  int *assert_fail,
  char *f)
{
  static char *F = "dtlev85_test_blob";
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t gray;
  dtimage_t segmented;
  blob_arg_t blob_arg;
  int sorted[BLOB_ARG_MAX];
  int j;
  dt_rc_e rc;

  segmented.xe = 0;
  gray.xe = 0;

  DT_C(dtlev85_test_make,(ctl,			/* make gray image				    */
    &gray, ncols, nrows, 
    gray_ntype,
    gray_data, DT_NTYPE_B08));

  DT_C(dtlev85_test_make,(ctl,			/* make segmented image			    */
    &segmented, ncols, nrows, 
    segmented_ntype,
    segmented_data, DT_NTYPE_B08));

  memset(&blob_arg, 0, 
    sizeof(blob_arg));
  blob_arg.param = param;				/* init communication structure 	*/
  blob_arg.gray = &gray;
  blob_arg.segmented = &segmented;

  DT_C(dtimage_adj_blob,(ctl,			/* find and deliver blobs			*/
    &segmented,
    0, 0, gray.xe, gray.ye,			
    background, DTIMAGE_ADJ_FLAG_NOT,
    dtlev85_test_blob_deliver,		
    &blob_arg));

  dtlev85_test_sort(ctl, blob_arg.p,	/* sort by blob pixel position 		*/
    DT_MIN(blob_arg.count, BLOB_ARG_MAX),
    sorted);

  for (j=0; j<DT_MIN(nblobs, blob_arg.count); j++)
  {
    int i = sorted[j];
    *assert_fail += DT_RC_GOOD != dt_assert(ctl,
      expect_label[j] == blob_arg.v[i] &&
      expect_uniformity[j] == blob_arg.u[i],
      f, 
      "%-3s %-3s blob[%d] %g (%g),"
      " uniformity %0.5f (delta %e)",
      dt_ntype_string[gray_ntype],
      dt_ntype_string[segmented_ntype],
      i,
      blob_arg.v[i], expect_label[j],
      blob_arg.u[i],
      blob_arg.u[i]-expect_uniformity[j]);
  }

  *assert_fail += DT_RC_GOOD != dt_assert(ctl,
    nblobs == blob_arg.count,
    f, "%-3s %-3s blob count %d (%d)",
    dt_ntype_string[gray_ntype],
    dt_ntype_string[segmented_ntype],
    blob_arg.count, nblobs);

cleanup:
  if (segmented.xe != 0)
    DT_I(dtimage_free,(ctl,
      &segmented));
  if (gray.xe != 0)
    DT_I(dtimage_free,(ctl,
      &gray));
  return rc;
}

/*..........................................................................*/

dt_rc_e
dtlev85_test1(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test1");
  dt_ctl_t *ctl = &param->ctl;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  int nlabels = 2;
  double label_value[] = {0, 1};
  double label_uniformity[] = {1.000000, 0.500000};
  long   label_nregions[] = {1, 2};

  double background1 = 9.0;
  int nblobs1 = 3;
  double label1[] = {0, 1, 1};
  double uniformity1[] = {1.000000, 0.500000, 0.500000};

  double background2 = 0.0;
  int nblobs2 = 2;
  double label2[] = {1, 1};
  double uniformity2[] = {0.500000, 0.500000};

  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  9.0, 0.984375, 3);

  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 1.0, 1.000000, 1);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 0.0, 0.500000, 2);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 9.0, 0.984375, 3);

  TOTAL(DT_NTYPE_B08, DT_NTYPE_F2 , 9.0, 0.984375, 3);
  TOTAL(DT_NTYPE_B16, DT_NTYPE_F2 , 9.0, 0.984375, 3);
  
  LABEL(DT_NTYPE_B08, DT_NTYPE_B08);
  LABEL(DT_NTYPE_B16, DT_NTYPE_F2);
  LABEL(DT_NTYPE_F2,  DT_NTYPE_B08);
  LABEL(DT_NTYPE_F2,  DT_NTYPE_F2);
  LABEL(DT_NTYPE_S1,  DT_NTYPE_B08);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_B08, DT_NTYPE_F2, 1);
  HIST(DT_NTYPE_B16, DT_NTYPE_F2, 1);
  HIST(DT_NTYPE_S2,  DT_NTYPE_F2, 1);
  HIST(DT_NTYPE_F2,  DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_F2,  DT_NTYPE_F2, 1);
  HIST(DT_NTYPE_S1,  DT_NTYPE_B08, 1);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 2);
  HIST(DT_NTYPE_S1,  DT_NTYPE_B08, 2);

  BLOB(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_B16, DT_NTYPE_F2, 1);
  BLOB(DT_NTYPE_F2,  DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_F2,  DT_NTYPE_F2, 1);

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_test2(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test2");
  dt_ctl_t *ctl = &param->ctl;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,
    0,0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,
    0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  int nlabels = 5;
  double label_value[] = {0, 1, 2, 3, 4};
  double label_uniformity[] = {1.000000, 0.5, 0.5, 0.5, 0.5};
  long   label_nregions[] = {1, 2, 2, 2, 2};

  double background1 = 9.0;
  int nblobs1 = 9;
  double label1[] = {0, 1, 2, 3, 4, 1, 2, 3, 4};
  double uniformity1[] = {1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

  double background2 = 2.0;
  int nblobs2 = 7;
  double label2[] = {0, 1, 3, 4, 1, 3, 4};
  double uniformity2[] = {1.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  9.0, 0.9843750, 9);

  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 1.0, 0.9879032, 7);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 2.0, 0.9879032, 7);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 0.0, 0.5000000, 8);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 9.0, 0.9843750, 9);

  LABEL(DT_NTYPE_B08, DT_NTYPE_B08);
  LABEL(DT_NTYPE_B08, DT_NTYPE_F2);
  LABEL(DT_NTYPE_S1, DT_NTYPE_F1);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 2);
  HIST(DT_NTYPE_S1,  DT_NTYPE_B08, 2);

  BLOB(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_S1,  DT_NTYPE_S1,  1);

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_test3(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test3");
  dt_ctl_t *ctl = &param->ctl;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,2,2,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  int nlabels = 1;
  double label_value[] = {0};
  double label_uniformity[] = {0.861328125};
  long   label_nregions[] = {1};

  double background1 = 9.0;
  int nblobs1 = 1;
  double label1[] = {0};
  double uniformity1[] = {0.861328125};

  double background2 = 0.0;
  int nblobs2 = 0;
  double *label2 = NULL;
  double *uniformity2 = NULL;

  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 9.0, 0.861328125, 1);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 0.0, 0.0,         0);

  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  9.0, 0.861328125, 1);
  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  0.0, 0.0,         0);

  LABEL(DT_NTYPE_B08, DT_NTYPE_B08);
  LABEL(DT_NTYPE_B08, DT_NTYPE_F2);
  LABEL(DT_NTYPE_F2,  DT_NTYPE_B08);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 2);

  BLOB(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_S1, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_S2, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_F1, DT_NTYPE_B08, 1);

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtlev85_test4(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test4");
  dt_ctl_t *ctl = &param->ctl;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
    0,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,
    0,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,
    0,8,8,8,8,8,8,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,8,8,8,4,4,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  };

  int nlabels = 1;
  double label_value[] = {0};
  double label_uniformity[] = {0.8125};
  long   label_nregions[] = {1};

  double background1 = 9.0;
  int nblobs1 = 1;
  double label1[] = {0};
  double uniformity1[] = {0.8125};

  double background2 = 0.0;
  int nblobs2 = 0;
  double *label2 = NULL;
  double *uniformity2 = NULL;

  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 9.0, 0.8125, 1);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 0.0, 0.0,    0);

  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  9.0, 0.8125, 1);
  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  0.0, 0.0,    0);

  LABEL(DT_NTYPE_B08, DT_NTYPE_B08);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 2);

  BLOB(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_B08, DT_NTYPE_S1, 1);
  BLOB(DT_NTYPE_B08, DT_NTYPE_B16, 1);
  BLOB(DT_NTYPE_B08, DT_NTYPE_S2, 1);

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................*
 * worst case variance, all regions contain fmax and fmin
 *..........................................................................*/

dt_rc_e
dtlev85_test5(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test5");
  dt_ctl_t *ctl = &param->ctl;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    3,3,7,7,3,3,7,7,3,3,7,7,3,3,7,7,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3,
    7,7,3,3,7,7,3,3,7,7,3,3,7,7,3,3
  };
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1,
    2,2,2,2,1,1,1,1,2,2,2,2,1,1,1,1
  };

  int nlabels = 2;
  double label_value[] = {1, 2};
  double label_uniformity[] = {0.5, 0.5};
  long   label_nregions[] = {8, 8};

  double background1 = 9.0;
  int nblobs1 = 16;
  double label1[] = {1, 2, 1, 2, 2, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 1};
  double uniformity1[] = {0.5, 0.5, 0.5, 0.5, 
                          0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5};

  double background2 = 1.0;
  int nblobs2 = 8;
  double label2[] = {2, 2, 2, 2, 2, 2, 2, 2};
  double uniformity2[] = {0.5, 0.5, 0.5, 0.5, 
                          0.5, 0.5, 0.5, 0.5};

  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 9.0, 0.5, 16);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 1.0, 0.5,  8);

  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  9.0, 0.5, 16);
  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  1.0, 0.5,  8);

  LABEL(DT_NTYPE_B08, DT_NTYPE_B08);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 2);

  BLOB(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_S1,  DT_NTYPE_B08, 1);
  BLOB(DT_NTYPE_S1,  DT_NTYPE_B16, 1);

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 * having half the image with 0 variance raises the total uniformity to .75
 *..........................................................................*/

dt_rc_e
dtlev85_test6(
  dtparam_t *param,
  int *assert_fail)
{
  DT_F("dtlev85_test6");
  dt_ctl_t *ctl = &param->ctl;

#define NCOLS 16
#define NROWS 16
  static dt_ntype_b08_t gray_data[NCOLS*NROWS] = {
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,3,3,7,7,3,3,7,7,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3,
    5,5,5,5,5,5,5,5,7,7,3,3,7,7,3,3
  };
  static dt_ntype_b08_t segmented_data[NCOLS*NROWS] = {
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1,
    3,3,3,3,3,3,3,3,2,2,2,2,1,1,1,1
  };

  int nlabels = 3;
  double label_value[] = {3, 1, 2};
  double label_uniformity[] = {1.0, 0.5, 0.5};
  long   label_nregions[] = {1, 4, 4};

  double background1 = 9.0;
  int nblobs1 = 9;
  double label1[] = {3, 1, 2, 2, 1, 1, 2, 2, 1};
  double uniformity1[] = {1.0,
                          0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5};

  double background2 = 1.0;
  int nblobs2 = 5;
  double label2[] = {3, 2, 2, 2, 2};
  double uniformity2[] = {1.0,
                          0.5, 0.5, 0.5, 0.5};

  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 9.0, 0.75,       9);
  TOTAL(DT_NTYPE_B08, DT_NTYPE_B08, 1.0, 0.83333333, 5);

  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  9.0, 0.75,       9);
  TOTAL(DT_NTYPE_F2,  DT_NTYPE_F2,  1.0, 0.83333333, 5);

  LABEL(DT_NTYPE_B08, DT_NTYPE_B08);
  LABEL(DT_NTYPE_S1,  DT_NTYPE_B08);

  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 1);
  HIST(DT_NTYPE_B08, DT_NTYPE_B08, 2);

  BLOB(DT_NTYPE_B08, DT_NTYPE_B08, 1);

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
