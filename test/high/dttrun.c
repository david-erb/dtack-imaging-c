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
  CHANGES
  19991217 DE fixed some format statements
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
DT_RCSID("test $RCSfile: dttrun.c,v $ $Revision: 1.9 $");
#include <dtack/runf.h>

#include <dtack/mem.h>


#define USAGE "[-Dnnnn]"

										/* ................................ */

static const char *ops[] = {"all",
  "fast", "1d", "scan", "label", "file",
  "time",
  NULL};
										/* ................................ */

/*..........................................................................*/
static 
dt_rc_e
dttrun_make(
  dt_ctl_t *ctl,
  dtimage_t *source,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttrun_make");
  dtxy_t y;
  unsigned int rowsize = (unsigned int)
    (ncols * dt_ntype_sizeof[ntype]);

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    source, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, source, y, &p);
    DT_MEMCPY(p, data, rowsize);
    DTIMAGE_PUTROW(ctl, source, y);
    data = (char *)data + rowsize;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttrun_free(
  dt_ctl_t *ctl,
  dtimage_t *source)
{
  DT_F("dttrun_compare_and_free");

  DT_Q(dtimage_free,(ctl,				/* free images					    */
    source));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttrunf_labels(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrunf_t *run,
  unsigned int nruns,
  dtntype_s4_t *actual_label,
  char *F,
  char *id)
{
  unsigned int i;
  int c;
  unsigned int n = DT_MIN(nruns, (unsigned int)run->runs);
  dtrunf_elem_t *p;

  if (ctl->dbg_mask & DT_DBG_MASK_PIXEL || actual_label != NULL)
  for (i=0; i<n; i++)
  {
    long h, p, l;

    if (run->row[0][i].h == NULL)
      l = -1;
    else
    if (run->row[0][i].h->h == NULL)
      l = -1;
    else
      l = run->row[0][i].h->h - run->row[0];
	if (actual_label != NULL)
      actual_label[i] = l;

    if (ctl->dbg_mask & DT_DBG_MASK_PIXEL)
	{
      unsigned int x = run->row[0][i].x;
      unsigned int y = run->row[0][i].y;
	  if (run->row[0][i].h == NULL)
        h = -1;
	  else
        h = run->row[0][i].h - run->row[0];
	  if (run->row[0][i].p == NULL)
        p = -1;
	  else
        p = run->row[0][i].p - run->row[0];
	  if (run->head == &run->row[0][i])
        c = '*';
	  else
	  if (run->row[0][i].h == &run->row[0][i])
        c = '+';
	  else
        c = ' ';
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "%s %3d. %ch %3ld  p %3ld  label (h->h) %3ld  0x%02x",
        id, i, c,
        h, p, l,
		image->row[y].b08[x]);
	}
  }

  p = run->head;
  if (ctl->dbg_mask & (DT_DBG_MASK_PIXEL | DT_DBG_MASK_OBJECT))
  while (p != NULL)
  {
	char s[256];
    dtrunf_elem_t *h = p->h;
    sprintf(s, "%ld", (long)(p-run->row[0]));
    for (p=p->p; p != NULL && p->h->h == h->h; p=p->p)
	{
	  char tmp[16];
	  sprintf(tmp, " %ld", (long)(p-run->row[0]));
	  if (sizeof(s)-strlen(s) < sizeof(tmp)+6)
	  {
		strcat(s, "...");
		break;
	  }
	  else
	    strcat(s, tmp);
	}
	dt_dbg(ctl, F,
      DT_DBG_MASK_PIXEL  | DT_DBG_MASK_OBJECT,
      "%s blob: %s", id, s);
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttrun_file(
  dt_ctl_t *ctl,
  const char *filename)
{					
  DT_F("dttrun_file");
  dtimage_t source;
  dtrunf_t run;
  dt_ntype_b08_t data[40*40];
  int ncols, nrows;
  int nruns, nblobs;
  unsigned long flags;
  dtntype_s4_t *expect_labels = NULL;
  dtntype_s4_t *actual_labels = NULL;
  dtntype_s4_t *expect_areas = NULL;
  dtntype_s4_t *expect_sums = NULL;
  char desc[64];
  FILE *file;
  long cmp;
  int good;
  dt_rc_e rc;

  file = fopen(filename, "r");
  if (file == NULL)
    return dt_err(ctl, F,
      "unable to open \"%s\" to read", filename);

  DT_C(dtos_malloc2,(ctl,
    (void **)&expect_labels, 
    256*sizeof(*expect_labels),
    F, "expect_labels"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&actual_labels, 
    256*sizeof(*actual_labels),
    F, "actual_labels"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&expect_areas, 
    256*sizeof(*expect_areas),
    F, "expect_areas"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&expect_sums, 
    256*sizeof(*expect_sums),
    F, "expect_sums"));

  while(1)
  {
    DT_C(dttutil_read_runadj,(ctl, file,
      desc, 
      &ncols, &nrows, &nruns, &nblobs,
      expect_labels,
      expect_areas,
      expect_sums,
  	  data,
	  &flags,
 	  &good));
    if (!good)
      break;

	if (flags == 0 || 
		flags == DTIMAGE_ADJ_FLAG_4)
	{
      DT_C(dttrun_make,(ctl,			/* make and clear images		    */
        &source,
        ncols, nrows, DT_NTYPE_B08,
      data));

      DT_C(dtrunf_alloc_struct,(ctl,	/* get space for runs			    */
        &source, &run));

      DT_C(dtrunf_scan,(ctl,			/* scan image for runs			    */
        &source, &run, 0.0, -1));

      DT_C(dtrunf_label,(ctl,			/* make run labels				    */
        &source, DTIMAGE_ADJ_FLAG_4,
        &run));
										/* can check whole output in single	*/
										/*   pass since only one segment    */
										/*   is involved				  	*/

      dt_assert(ctl, run.runs == (unsigned int)nruns, F,
        "%s runs %ld (%ld)", desc, run.runs, nruns);
      dt_assert(ctl, run.blobs == (unsigned int)nblobs, F,
        "%s blobs %ld (%ld)", desc, run.blobs, nblobs);
      dttrunf_labels(ctl, &source,
        &run, nruns,
        actual_labels, F, desc);
      DT_C_1D(dt1d_cmp,(
        DTNTYPE_S4,
	    actual_labels, nruns,
        expect_labels, nruns,
        0, &cmp));
      dt_assert(ctl, cmp == 0, F,
        "%s labels (cmp=%ld)",
	    desc, cmp);

      DT_C(dtrunf_free_all,(ctl,	/* free run space				    */
        &source, &run));

      DT_C(dttrun_free,(ctl, 
        &source));
	}
  }

cleanup:
  if (expect_sums)
    DT_I(dtos_free2,(ctl, expect_sums, F, "expect_sums"));
  if (expect_areas)
    DT_I(dtos_free2,(ctl, expect_areas, F, "expect_areas"));
  if (actual_labels)
    DT_I(dtos_free2,(ctl, actual_labels, F, "actual_labels"));
  if (expect_labels)
    DT_I(dtos_free2,(ctl, expect_labels, F, "expect_labels"));

  fclose(file);

  return rc;
}

/*..........................................................................*/

static
dt_rc_e
dttrun_1d(
  dt_ctl_t *ctl)
{
  DT_F("dttrun_1d");
  dtimage_t source;
  long cmp;
  char desc[128];
  dt_ntype_e ntype =
    (sizeof(dtrunf_len_t) == 2)?
    DT1D_NTYPE_S2:
    DT1D_NTYPE_S4;
  dtxy_t x, y;

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "simple test"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       0,0,0,0, 0,0, 0,0,0,0,
	   1,1,1,1, 1,1, 1,1,1,1,
	   1,0,0,0, 0,0, 0,0,0,0,
	   0,1,0,0, 0,0, 0,0,0,0,
	   1,0,0,0, 0,0, 0,0,0,1,
	   1,0,1,0, 1,0, 1,0,1,0,
	   0,1,0,1, 0,1, 0,1,0,1,
	   1,1,1,1, 1,1, 1,1,1,0,
	   0,1,1,1, 1,1, 1,1,1,0,
	   0,0,0,0, 0,0, 0,0,0,1
    };
    static dtrunf_len_t expect_x[NROWS][NCOLS+1] = {
       { 1, -10,  0,  0,  0,  0,  0,  0,  0,  0,  0},
       { 1,  10,  0,  0,  0,  0,  0,  0,  0,  0,  0},
       { 2,   1, -9,  0,  0,  0,  0,  0,  0,  0,  0},
       { 3,  -1,  1, -8,  0,  0,  0,  0,  0,  0,  0},
       { 3,   1, -8,  1,  0,  0,  0,  0,  0,  0,  0},
       {10,   1, -1,  1, -1,  1, -1,  1, -1,  1, -1},
       {10,  -1,  1, -1,  1, -1,  1, -1,  1, -1,  1},
       { 2,   9, -1,  0,  0,  0,  0,  0,  0,  0,  0},
       { 3,  -1,  8, -1,  0,  0,  0,  0,  0,  0,  0},
       { 2,  -9,  1,  0,  0,  0,  0,  0,  0,  0,  0}
    };
    static dtrunf_len_t expect_y[NROWS][NCOLS+1] = {
       { 7,  -1,  2, -1,  2, -1,  1, -2,  0,  0,  0},
       { 7,  -1,  1, -1,  1, -2,  3, -1,  0,  0,  0},
       { 7,  -1,  1, -3,  1, -1,  2, -1,  0,  0,  0},
       { 5,  -1,  1, -4,  3, -1,  0,  0,  0,  0,  0},

       { 7,  -1,  1, -3,  1, -1,  2, -1,  0,  0,  0},
       { 5,  -1,  1, -4,  3, -1,  0,  0,  0,  0,  0},

       { 7,  -1,  1, -3,  1, -1,  2, -1,  0,  0,  0},
       { 5,  -1,  1, -4,  3, -1,  0,  0,  0,  0,  0},
       { 7,  -1,  1, -3,  1, -1,  2, -1,  0,  0,  0},
       { 8,  -1,  1, -2,  1, -1,  1, -2,  1,  0,  0}
    };
    static dtrunf_len_t expect_xmax[NROWS] = {
      0, 10, 1, 1,   1, 1,   1, 9, 8, 1
	};
    static dtrunf_len_t expect_ymax[NROWS] = {
      2, 3, 2, 3,    2, 3,   2, 3, 2, 1
	};

    dtrunf_len_t actual[NCOLS+1];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));
 										/* ................................ */
    DT_MEMSET(actual, 0,					/* clear output					    */
     sizeof(actual));
    DT_Q(dtrun_rows_max,(ctl,
      &source, actual, NROWS));

      DT_Q_1D(dt1d_cmp,(ntype,				/* compare to expect_xmax		    */
        actual, NROWS,
	    expect_xmax, NROWS,
        0, &cmp))

    sprintf(desc, "%s row max", DESC);
    if (cmp == 0)
      dt_assert(ctl, cmp == 0, F,
         "%s", desc);
    else
    {
      unsigned int wrong = abs((unsigned int)cmp) - 1;
      dt_assert(ctl, cmp == 0, F,
        "%s [%ld] actual %ld, expect_xmax %ld",
        desc, wrong,
        actual[wrong], expect_xmax[wrong]);  
	}

										/* ................................ */
    for (y=0; y<NROWS; y++)
	{
      DT_MEMSET(actual, 0,					/* clear output					    */
        sizeof(actual));

      DT_Q(dtrun_row,(ctl,				/* make run of row			  		*/
        &source, y, actual, NCOLS+1));

        DT_Q_1D(dt1d_cmp,(ntype,			/* compare to expect_x			    */
          actual, NCOLS+1,
          expect_x[y], NCOLS+1,
          0, &cmp))

      sprintf(desc, "%s row %ld",
		DESC, (long)y);
      if (cmp == 0)
        dt_assert(ctl, cmp == 0, F,
          "%s", desc);
      else
	  {
	    unsigned int wrong = abs((unsigned int)cmp) - 1;
        dt_assert(ctl, cmp == 0, F,
          "%s [%ld] actual %ld, expect_xmax %ld",
          desc, wrong,
          actual[wrong], expect_xmax[wrong]);  
	  }
	}

										/* ................................ */
    DT_MEMSET(actual, 0,					/* clear output					    */
     sizeof(actual));
    DT_Q(dtrun_cols_max,(ctl,
      &source, actual, NCOLS));

      DT_Q_1D(dt1d_cmp,(ntype,
        actual, NCOLS,
	    expect_ymax, NCOLS,
        0, &cmp))

    sprintf(desc, "%s col max", DESC);
    if (cmp == 0)
      dt_assert(ctl, cmp == 0, F,
         "%s", desc);
    else
    {
      unsigned int wrong = abs((unsigned int)cmp) - 1;
      dt_assert(ctl, cmp == 0, F,
        "%s [%ld] actual %ld, expect_ymax %ld",
        desc, wrong,
        actual[wrong], expect_ymax[wrong]);  
	}
										/* ................................ */
    for (x=0; x<NCOLS; x++)
	{
      DT_MEMSET(actual, 0,					/* clear output					    */
        sizeof(actual));

      DT_Q(dtrun_col,(ctl,				/* make run of col			  		*/
        &source, x, actual, NCOLS+1));

      DT_Q_1D(dt1d_cmp,(ntype,				/* compare to expect_y			    */
        actual, NCOLS+1,
        expect_y[x], NCOLS+1,
        0, &cmp));

      sprintf(desc, "%s col %ld",
		DESC, (long)x);
      if (cmp == 0)
        dt_assert(ctl, cmp == 0, F,
          "%s", desc);
      else
	  {
	    unsigned int wrong = abs((unsigned int)cmp) - 1;
        dt_assert(ctl, cmp == 0, F,
          "%s [%ld] actual %ld, expect_y %ld",
          desc, wrong,
          actual[wrong], expect_y[x][wrong]);  
	  }
	}
										/* ................................ */

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef DESC
    
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttrun_scan(
  dt_ctl_t *ctl)
{
  DT_F("dttrun_scan");
  dtimage_t source;
  dtrunf_t run;
  long cmp;
  int i;
  dt_ntype_e ntype =
    (sizeof(dtrunf_len_t) == 2)?
    DT1D_NTYPE_S2:
    DT1D_NTYPE_S4;

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define NRUNS 35
#define DESC "simple"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       0,0,0,0,0, 1,1,1,1,1,
       1,1,1,1,1, 0,0,0,0,0,
       2,2,2,2,2, 1,1,1,1,1,
       0,1,0,1,0, 1,0,1,0,1,
	   1,0,0,0,0, 0,0,0,0,0,

       1,2,3,4,5, 6,7,8,9,0,
       0,0,0,0,0, 0,0,0,0,0,
	   1,1,1,1,1, 1,1,1,1,1,
       0,0,0,0,0, 0,0,0,0,1,
       2,0,0,0,0, 0,0,0,0,1,
    };
      
    static dtntype_s4_t expect_x[NRUNS] = {
       0, 5,
       0, 5,
       0, 5,
       0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
       0, 1,

       0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
       0,
       0,
       0, 9,
       0, 1, 9};
    static unsigned int expect_n[NROWS] = {
       2,
       2,
       2,
       10,
       2,

       10,
       1,
       1,
       2,
       3};
    dtntype_s4_t actual_x[NRUNS];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));
 										/* ................................ */
    DT_Q(dtrunf_alloc_struct,(ctl,		/* get space for runs			    */
      &source, &run));

    DT_Q(dtrunf_scan,(ctl,				/* scan image for runs			    */
      &source, &run, 0.0, 0));

										/* can check whole output in single	*/
										/*   pass since only one segment    */
										/*   is involved				  	*/

    for (i=0; i<NRUNS; i++)
      actual_x[i] = run.row[0][i].x;
    DT_Q_1D(dt1d_cmp,(					/* compare to expect_x			    */
      DTNTYPE_S4,
	  actual_x, NRUNS,
      expect_x, NRUNS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s run x", DESC);

    DT_Q_1D(dt1d_cmp,(ntype,			/* compare to expect_n			    */
      run.n, NROWS,
	  expect_n, NROWS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s run n", DESC);

										/* ................................ */

    DT_Q(dtrunf_free_all,(ctl,			/* free run space				    */
      &source, &run));

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef NRUNS
#undef DESC
    
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttrun_label(
  dt_ctl_t *ctl)
{
  DT_F("dttrun_label");
  dtimage_t source;
  dtrunf_t run;
  long cmp;
  int i;
  dt_ntype_e ntype =
    (sizeof(dtrunf_len_t) == 2)?
    DT1D_NTYPE_S2:
    DT1D_NTYPE_S4;

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define NRUNS 13
#define DESC "intwined"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       0,0,0,0,1,1,1,0,0,0,
       0,0,0,9,9,9,1,0,0,0,
       0,0,0,9,1,9,1,0,0,0,
       0,0,0,9,1,1,1,0,0,0,
       0,0,0,9,9,9,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,1,0,0,0,0,0,0,0,
       1,1,1,1,1,0,0,0,0,1
	};
	static dtntype_s4_t expect_label[NRUNS] = {
	  4, 1, 4, 1, 4, 1, 4, 1, 4, 1, 10, 10, 12
	};
	dtntype_s4_t actual_x[NRUNS];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));

    DT_Q(dtrunf_alloc_struct,(ctl,	/* get space for runs			    */
      &source, &run));

    DT_Q(dtrunf_scan,(ctl,			/* scan image for runs			    */
      &source, &run, 0.0, -1));

    DT_Q(dtrunf_label,(ctl,		/* make run labels				    */
      &source, 0, &run));
										/* can check whole output in single	*/
										/*   pass since only one segment    */
										/*   is involved				  	*/

    dt_assert(ctl, run.runs == NRUNS, F,
      "%s runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 4, F,
      "%s blobs (%ld)", DESC, run.blobs);
    dttrunf_labels(ctl, &source,
      &run, NRUNS,
      actual_x, F, DESC);
    DT_Q_1D(dt1d_cmp,(
      DTNTYPE_S4,
	  actual_x, NRUNS,
      expect_label, NRUNS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s", DESC);

										/* ................................ */

    DT_Q(dtrunf_free_all,(ctl,		/* free run space				    */
      &source, &run));

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef NRUNS
#undef DESC
    
  }
  {										/* ................................ */
#define NCOLS 11
#define NROWS 10
#define NRUNS 33
#define DESC "simple"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       0,0,0,0,0, 1,1,1,1,1,1,
       1,1,1,1,1, 0,0,0,0,0,0,
       2,2,2,2,2, 3,3,3,3,3,3,
       3,3,3,3,3, 3,2,2,2,2,2,
       2,2,2,2,2, 3,3,3,3,3,3,

       2,2,1,2,2, 2,2,2,2,2,2,
       2,1,1,1,5, 2,5,2,5,2,5,
       2,2,1,2,5, 5,5,2,5,5,5,
       2,2,2,2,2, 2,5,5,5,2,2,
       2,2,2,2,2, 2,2,2,2,2,3
    };
    static dtntype_s4_t expect_label[NRUNS] = {
       0, 1,
       2, 3,
       4, 5,
       5, 7,
       8, 5,

       8, 11, 8,
       8, 11, 15, 8, 15, 8,          15, 8, 15,
       8, 11, 8, 15, 8,              15,
       8, 15, 8,
       8, 32};
      
    static dtntype_s4_t expect_x[NRUNS] = {
	  0, 5,
	  0, 5,
	  0, 5,
	  0, 6,
	  0, 5,

	  0, 2, 3, 
	  0, 1, 4, 5, 6, 7, 8, 9, 10,
	  0, 2, 3, 4, 7, 8,
	  0, 6, 9,
	  0, 10};

    static unsigned int expect_n[NROWS] = {
       2,
       2,
       2,
       2,
       2,

       3,
       9,
       6,
       3,
       2};

    dtntype_s4_t actual_x[NRUNS];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));
 										/* ................................ */
    DT_Q(dtrunf_alloc_struct,(ctl,		/* get space for runs			    */
      &source, &run));

    DT_Q(dtrunf_scan,(ctl,				/* scan image for runs			    */
      &source, &run, 0.0, 0));

    DT_Q(dtrunf_label,(ctl,				/* make run labels				    */
      &source, 0, &run));
										/* can check whole output in single	*/
										/*   pass since only one segment    */
										/*   is involved				  	*/

    for (i=0; i<NRUNS; i++)
      actual_x[i] = run.row[0][i].x;
    DT_Q_1D(dt1d_cmp,(					/* compare to expect_x			    */
      DTNTYPE_S4,
	  actual_x, NRUNS,
      expect_x, NRUNS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s x (cmp=%ld)", DESC, cmp);

    dt_assert(ctl, run.runs == NRUNS, F,
      "%s runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 11, F,
      "%s blobs (%ld)", DESC, run.blobs);

    DT_Q_1D(dt1d_cmp,(ntype,				/* compare to expect_n			    */
      run.n, NROWS,
	  expect_n, NROWS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s n (cmp=%ld)", DESC, cmp);

    dttrunf_labels(ctl, &source,
      &run, NRUNS,
      actual_x, F, DESC);
    DT_Q_1D(dt1d_cmp,(					/* compare to expect_labels		    */
      DTNTYPE_S4,
      actual_x, NRUNS,
	  expect_label, NRUNS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s labels (cmp=%ld)", DESC, cmp);

										/* ................................ */

    DT_Q(dtrunf_free_all,(ctl,		/* free run space				    */
      &source, &run));

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef NRUNS
#undef DESC
    
  }

  {										/* ................................ */
#define NCOLS 4
#define NROWS 4
#define NRUNS 10
#define DESC "hole"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       1,1,1,1,
       1,0,2,1,
	   1,2,0,1,
	   1,1,1,1
    };
	static dtntype_s4_t expect_label[NRUNS] = {
	  0, 0, 2, 3, 0, 0, 6, 7, 0, 0
	};
	dtntype_s4_t actual_x[NRUNS];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));

    DT_Q(dtrunf_alloc_struct,(ctl,	/* get space for runs			    */
      &source, &run));

    DT_Q(dtrunf_scan,(ctl,			/* scan image for runs			    */
      &source, &run, 0.0, 0));

    DT_Q(dtrunf_label,(ctl,				/* make run labels				    */
      &source, 0, &run));
										/* can check whole output in single	*/
										/*   pass since only one segment    */
										/*   is involved				  	*/

    dt_assert(ctl, run.runs == NRUNS, F,
      "%s runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 5, F,
      "%s blobs (%ld)", DESC, run.blobs);
    dttrunf_labels(ctl, &source,
      &run, NRUNS,
      actual_x, F, DESC);
    DT_Q_1D(dt1d_cmp,(
      DTNTYPE_S4,
	  actual_x, NRUNS,
      expect_label, NRUNS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s", DESC);

										/* ................................ */

    DT_Q(dtrunf_free_all,(ctl,		/* free run space				    */
      &source, &run));

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef NRUNS
#undef DESC
    
  }

  {										/* ................................ */
#define NCOLS 7
#define NROWS 4
#define NRUNS 10
#define DESC "quad join"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       1,0,1,0,1,0,1,
       1,1,1,0,1,0,1,
	   0,0,1,1,1,0,1,
	   0,0,0,0,1,1,1
    };
	static dtntype_s4_t expect_label[NRUNS] = {
	  0, 0,0,0, 0,0,0, 0,0,0
	};
	dtntype_s4_t actual_x[NRUNS];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));

    DT_Q(dtrunf_alloc_struct,(ctl,	/* get space for runs			    */
      &source, &run));

    DT_Q(dtrunf_scan,(ctl,			/* scan image for runs			    */
      &source, &run, 1.0, 1));

    DT_Q(dtrunf_label,(ctl,				/* make run labels				    */
      &source, 0, &run));
										/* can check whole output in single	*/
										/*   pass since only one segment    */
										/*   is involved				  	*/

    dt_assert(ctl, run.runs == NRUNS, F,
      "%s runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 1, F,
      "%s blobs (%ld)", DESC, run.blobs);
    dttrunf_labels(ctl, &source,
      &run, NRUNS,
      actual_x, F, DESC);
    DT_Q_1D(dt1d_cmp,(
      DTNTYPE_S4,
	  actual_x, NRUNS,
      expect_label, NRUNS,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s labels (cmp=%ld)", DESC, cmp);

										/* ................................ */

    DT_Q(dtrunf_free_all,(ctl,		/* free run space				    */
      &source, &run));

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef NRUNS
#undef DESC
    
  }


  {										/* ................................ */
#define NCOLS 4
#define NROWS 4
#define DESC "select"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       1,1,1,1,
       1,0,1,1,
	   1,1,0,1,
	   1,1,1,1
    };
	static dtntype_s4_t expect_label[8] = {
	  0, 0, 2, 0, 0, 5, 0, 0
	};
	dtntype_s4_t actual_x[8];

    DT_Q(dttrun_make,(ctl,				/* make and clear images		    */
      &source,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0]));

    DT_Q(dtrunf_alloc_struct,(ctl,	/* get space for runs			    */
      &source, &run));

 										/* ................................ */
    DT_Q(dtrunf_scan,(ctl,			/* scan image for runs			    */
      &source, &run, 1.0, 1));

    DT_Q(dtrunf_label,(ctl,		/* make run labels				    */
      &source, 0, &run));
    dttrunf_labels(ctl, &source,
      &run, 6,
      actual_x, F, DESC " scan for 1");
    dt_assert(ctl, run.runs == 6, F,
      "%s scan for 1 runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 1, F,
      "%s scan for 1 blobs (%ld)", DESC, run.blobs);

    DT_Q(dtrunf_free_runs,(ctl,	/* free run space				    */
      &source, &run));

 										/* ................................ */
    DT_Q(dtrunf_scan,(ctl,				/* scan image for runs			    */
      &source, &run, 1.0, -1));

    DT_Q(dtrunf_label,(ctl,				/* make run labels				    */
      &source, 0, &run));
    dttrunf_labels(ctl, &source,
      &run, 2,
      actual_x, F, DESC " scan for not 1");
    dt_assert(ctl, run.runs == 2, F,
      "%s scan for not 1 runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 2, F,
      "%s scan for not 1 blobs (%ld)", DESC, run.blobs);

    DT_Q(dtrunf_free_runs,(ctl,			/* free run space				    */
      &source, &run));

 										/* ................................ */
    DT_Q(dtrunf_scan,(ctl,		/* scan image for all runs	   	    */
      &source, &run, 0.0, 0));

    DT_Q(dtrunf_label,(ctl,				/* make run labels				    */
      &source, 0, &run));
    dttrunf_labels(ctl, &source,
      &run, 8,
      actual_x, F, DESC " label for all");
    dt_assert(ctl, run.runs == 8, F,
      "%s label for all runs (%ld)", DESC, run.runs);
    dt_assert(ctl, run.blobs == 3, F,
      "%s label for all blobs (%ld)", DESC, run.blobs);
    DT_Q_1D(dt1d_cmp,(
      DTNTYPE_S4,
	  actual_x, 8,
      expect_label, 8,
      0, &cmp));
    dt_assert(ctl, cmp == 0, F,
      "%s labels", DESC);

    DT_Q(dtrunf_free_runs,(ctl,			/* free run space				    */
      &source, &run));

										/* ................................ */

    DT_Q(dtrunf_free_struct,(ctl,	/* free run space				    */
      &source, &run));

    DT_Q(dttrun_free,(ctl, 
      &source));

#undef NCOLS
#undef NROWS
#undef DESC
    
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 * one big, maximally convoluted blob
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttrun_time(
  dt_ctl_t *ctl,
  dtimage_t *image,
  long nblobs,
  void *arg,
  const char *desc,
  long loops)
{
  DT_F("dttrun_time");
  dtrunf_t run;
  long loop;
  dtos_time_t mark1, mark2;
  char f[64];
  
  if (image->flags &
	  DTIMAGE_FLAG_PAGED)
    return DT_RC_GOOD;

  DT_Q(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
    DT_Q(dtrunf_alloc_struct,(	/* get space for runs			    */
      ctl, image, &run));
	DT_Q(dtrunf_scan,(ctl,		/* scan image for runs			    */
      image, &run, 0.0, -1));
	DT_Q(dtrunf_label,(ctl,		/* make run labels				    */
      image, 0, &run));
    DT_Q(dtrunf_free_all,(ctl,	/* free space we used 				*/
      image, &run));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  dt_assert(ctl, 
    nblobs == -1 ||						/* blob count is not unknown? 		*/
    run.blobs == (unsigned)nblobs,		/* check blob counts			    */
    F, "%s blobs %ld (%ld)",
    desc, run.blobs, nblobs);

  sprintf(f, "%s (%2ld)", desc,			/* description of synthetic blob    */
    loops);
  dtt_time_report(ctl,
    &mark1, &mark2, f);

  return DT_RC_GOOD;    
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttrun_arg_check(          				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttrun_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttrun)
{
  DT_F("dttrun");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char testdata[128];
  int assert_fail = 0;                  /* failed assertion count           */
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_C(dtparam_init,(param));          /* init param structure             */

  DT_C(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->datadir, NULL, NULL, NULL,
    &param->input, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "5",
    &param->w, NULL, NULL, "256",
    &param->h, NULL, NULL, "256",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttrun_arg_check, NULL));
                                        /* ................................ */

  DT_C(dtstr_printf,(ctl, testdata,
    sizeof(testdata), "%s/%s",
    param->datadir, 
    param->input? param->input:
    "runadj.dat"));

  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {

      if (!strcmp(param->op, "fast") ||
          !strcmp(param->op, "1d") ||
          !strcmp(param->op, "all"))
      {
		DT_C(dttrun_1d,(ctl));			/* simple 1d stuff					*/
      }

      if (!strcmp(param->op, "fast") ||
          !strcmp(param->op, "scan") ||
          !strcmp(param->op, "all"))
      {
		DT_C(dttrun_scan,(ctl));		/* scanning only					*/
      }

      if (!strcmp(param->op, "fast") ||
          !strcmp(param->op, "label") ||
          !strcmp(param->op, "all"))
      {
		DT_C(dttrun_label,(ctl));		/* scanning and labelling			*/
      }

      if (!strcmp(param->op, "fast") ||
          !strcmp(param->op, "file") ||
          !strcmp(param->op, "all"))
      {
		DT_C(dttrun_file,(ctl,			/* read from file					*/
		  testdata));
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_C(dtt_image_list,(param,
          dtt_blob_list,
          param->input,
          DT_NTYPE_B08,
          dttrun_time, NULL));
      }
    }
  }

                                        /* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving          */

  DT_I(dt_heapmon_uninit,(ctl,          /* check all memory is freed        */
    &assert_fail));

  if (assert_fail)                      /* any failed assertions?           */
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
