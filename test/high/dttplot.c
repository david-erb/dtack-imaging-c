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

#include <dtack/t.h>
#include <dtack/mem.h>

DT_RCSID("test $RCSfile: dttplot.c,v $ $Revision: 1.8 $");

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  NULL};

/*..........................................................................*/
static 
dt_rc_e
dttplot_make_and_clear(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtimage_t *expect,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttplot_make_and_clear");
  dtxy_t y;

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    expect, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, expect, y, &p);
    DT_MEMCPY(p, data, expect->rowsize);
    DTIMAGE_PUTROW(ctl, expect, y);
    data = (char *)data +
	  expect->rowsize;
  }

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
  DT_Q(dtimage_dbg_values,(ctl,			/* optionally print out values	    */
    image, F, DT_DBG_MASK_PIXEL));
  dtt_check(ctl, cmprc == 0,
    "%s %d (0)", desc, cmprc);
  DT_Q(dtimage_free,(ctl,				/* free images					    */
    expect));
  DT_Q(dtimage_free,(ctl,
    image));

  return DT_RC_GOOD;
}
/*..........................................................................*/

static
dt_rc_e
dttplot_bars(
  dt_ctl_t *ctl)
{
  DT_F("dttplot_bars");
  dtimage_t image, expect;

  {										/* ................................ */
#define NCOLS 9
#define NROWS 9
#define DESC "xbars with no offset"
    static dt_ntype_b08_t data[(NCOLS+1)*(NROWS+2)] = {
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,1,0,
       0,0,0,0,0,0,0,1,1,0,
       0,0,0,0,0,0,1,1,1,0,
       0,0,0,0,0,1,1,1,1,0,
       0,0,0,0,1,1,1,1,1,0,
       0,0,0,1,1,1,1,1,1,0,
       0,0,1,1,1,1,1,1,1,0,
       0,1,1,1,1,1,1,1,1,0,
       1,1,1,1,1,1,1,1,1,0
	};
    static dtntype_s4_t long_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};
    static double double_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+1, NROWS+2, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_sint32,(ctl,	/* make plot					  	*/
      &image, &long_bars[0], NCOLS, 0,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " long"));

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+1, NROWS+2, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_double,(ctl, /* make plot					  	*/
      &image, &double_bars[0], NCOLS, 0,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " double"));

#undef NCOLS
#undef NROWS
#undef DESC
    
  }

  {										/* ................................ */
#define NCOLS 9
#define NROWS 9
#define DESC "ybars with no offset"
    static dt_ntype_b08_t data[(NCOLS+1)*(NROWS+1)] = {
       1,0,0,0,0,0,0,0,0,0,
       1,1,0,0,0,0,0,0,0,0,
       1,1,1,0,0,0,0,0,0,0,
       1,1,1,1,0,0,0,0,0,0,
       1,1,1,1,1,0,0,0,0,0,
       1,1,1,1,1,1,0,0,0,0,
       1,1,1,1,1,1,1,0,0,0,
       1,1,1,1,1,1,1,1,0,0,
       1,1,1,1,1,1,1,1,1,0,
       0,0,0,0,0,0,0,0,0,0
	};
    static dtntype_s4_t long_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};
    static double double_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+1, NROWS+1, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_ybars_sint32,(ctl,	/* make plot					  	*/
      &image, &long_bars[0], NCOLS, 0,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " long"));

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+1, NROWS+1, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_ybars_double,(ctl, /* make plot					  	*/
      &image, &double_bars[0], NCOLS, 0,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " double"));

#undef NCOLS
#undef NROWS
#undef DESC
    
  }
  {										/* ................................ */
#define NCOLS 5
#define NROWS 11
#define DESC "xbars scaled with no offset"
    static dt_ntype_b08_t data[NCOLS*NROWS] = {
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
       1,1,1,1,1
	};
    static dtntype_s4_t long_bars[NCOLS] = {
      50, 4, 100, 10, 20
	};
    static double double_bars[NCOLS] = {
      50, 4, 100, 10, 20
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_sint32,(ctl, /* make plot					  	*/
      &image, &long_bars[0], NCOLS, 0,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " long"));

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_double,(ctl, /* make plot					  	*/
      &image, &double_bars[0], NCOLS, 0,
      NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " double"));

#undef NCOLS
#undef NROWS
#undef DESC
    
  }

  {										/* ................................ */
#define NCOLS 9
#define NROWS 9
#define OFFSET 2
#define DESC "xbars with offset"
    static dt_ntype_b08_t data[(NCOLS+1)*(NROWS+1+OFFSET)] = {
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,1,0,
       0,0,0,0,0,0,0,1,1,0,
       0,0,0,0,0,0,1,1,1,0,
       0,0,0,0,0,1,1,1,1,0,
       0,0,0,0,1,1,1,1,1,0,
       0,0,0,1,1,1,1,1,1,0,
       0,0,1,1,1,1,1,1,1,0,
       0,1,1,1,1,1,1,1,1,0,
       1,1,1,1,1,1,1,1,1,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0
	};
    static dtntype_s4_t long_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};
    static double double_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+1, NROWS+1+OFFSET, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_sint32,(ctl, /* make plot					  	*/
      &image, &long_bars[0],
      NCOLS, OFFSET, NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC " long"));

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+1, NROWS+1+OFFSET, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_double,(ctl, /* make plot					  	*/
      &image, &double_bars[0],
      NCOLS, OFFSET, NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC " double"));

#undef NCOLS
#undef NROWS
#undef OFFSET
#undef DESC
    
  }

  {										/* ................................ */
#define NCOLS 9
#define NROWS 9
#define OFFSET 3
#define DESC "ybars with offset"
    static dt_ntype_b08_t data[(NCOLS+OFFSET)*NROWS] = {
       0,0,0,1,0,0,0,0,0,0,0,0,
       0,0,0,1,1,0,0,0,0,0,0,0,
       0,0,0,1,1,1,0,0,0,0,0,0,
       0,0,0,1,1,1,1,0,0,0,0,0,
       0,0,0,1,1,1,1,1,0,0,0,0,
       0,0,0,1,1,1,1,1,1,0,0,0,
       0,0,0,1,1,1,1,1,1,1,0,0,
       0,0,0,1,1,1,1,1,1,1,1,0,
       0,0,0,1,1,1,1,1,1,1,1,1
	};
    static dtntype_s4_t long_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};
    static double double_bars[NCOLS] = {
      0,1,2,3,4,5,6,7,8
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+OFFSET, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_ybars_sint32,(ctl,	/* make plot					  	*/
      &image, &long_bars[0], 
      NCOLS, OFFSET, NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " long"));

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS+OFFSET, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_ybars_double,(ctl, /* make plot					  	*/
      &image, &double_bars[0], 
      NCOLS, OFFSET, NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " double"));

#undef NCOLS
#undef NROWS
#undef OFFSET
#undef DESC
    
  }
  {										/* ................................ */
#define NCOLS 5
#define NROWS 11
#define OFFSET 8
#define DESC "xbars scaled with offset"
    static dt_ntype_b08_t data[NCOLS*(NROWS+OFFSET)] = {
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
    static dtntype_s4_t long_bars[NCOLS] = {
      50, 4, 100, 10, 20
	};
    static double double_bars[NCOLS] = {
      50, 4, 100, 10, 20
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS+OFFSET, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_sint32,(ctl, /* make plot					  	*/
      &image, &long_bars[0], 
      NCOLS, OFFSET, NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " long"));

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS+OFFSET, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_xbars_double,(ctl, /* make plot					  	*/
      &image, &double_bars[0], 
      NCOLS, OFFSET, NROWS, 1.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, 
      DESC " double"));

#undef NCOLS
#undef NROWS
#undef OFFSET
#undef DESC
    
  }
  return DT_RC_GOOD;
}
/*..........................................................................*/

static
dt_rc_e
dttplot_lines(
  dt_ctl_t *ctl)
{
  DT_F("dttplot_lines");
  dtimage_t image, expect;

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "crossed lines"
    static dt_ntype_b08_t data[NCOLS*NROWS] = {
       0,0,0,0,0,0,0,0,0,0,
       0,4,0,2,0,0,3,0,1,0,
       0,0,4,0,2,3,0,1,0,0,
       0,0,0,4,3,2,1,0,0,0,
       0,0,0,3,4,1,2,0,0,0,
       0,0,3,0,1,4,0,2,0,0,
       0,3,0,1,0,0,4,0,2,0,
       0,0,1,0,0,0,0,4,0,0,
       0,1,0,0,0,0,0,0,4,0,
       0,0,0,0,0,0,0,0,0,0
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_line,(ctl,		/* make plot					  	*/
      &image, 1, 8, 8, 1, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 8, 6, 3, 1, 2.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 6, 1, 1, 6, 3.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 1, 1, 8, 8, 4.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }
  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "orthogonal lines"
    static dt_ntype_b08_t data[NCOLS*NROWS] = {
       1,1,1,1,1,1,1,1,1,1,
       1,0,0,0,0,0,0,0,0,1,
       1,0,3,2,2,2,2,4,0,1,
       1,0,3,0,0,0,0,4,0,1,
       1,0,3,0,6,7,0,4,0,1,
       1,0,3,0,8,9,0,4,0,1,
       1,0,3,0,0,0,0,4,0,1,
       1,0,5,5,5,5,5,5,0,1,
       1,0,0,0,0,0,0,0,0,1,
       1,1,1,1,1,1,1,1,1,1
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_line,(ctl,		/* make plot					  	*/
      &image, 0, 0, 0, 9, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 0, 9, 9, 9, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 9, 9, 9, 0, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 9, 0, 0, 0, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 2, 2, 7, 2, 2.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 7, 2, 7, 7, 4.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 2, 2, 2, 7, 3.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 2, 7, 7, 7, 5.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 4, 4, 4, 4, 6.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 5, 4, 5, 4, 7.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 4, 5, 4, 5, 8.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 5, 5, 5, 5, 9.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "shallow lines"
    static dt_ntype_b08_t data[NCOLS*NROWS] = {
       0,0,0,0,0,0,0,0,0,0,
       0,1,1,1,1,0,0,0,0,0,
       0,0,0,0,0,1,1,1,1,0,
       0,0,0,0,0,3,3,3,3,0,
       0,3,3,3,3,0,0,0,0,0,
       0,0,0,0,0,4,4,4,4,0,
       0,4,4,4,4,0,0,0,0,0,
       0,2,2,2,2,0,0,0,0,0,
       0,0,0,0,0,2,2,2,2,0,
       0,0,0,0,0,0,0,0,0,0
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_line,(ctl,		/* make plot					  	*/
      &image, 1, 1, 8, 2, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 8, 8, 1, 7, 2.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 1, 4, 8, 3, 3.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 8, 5, 1, 6, 4.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "steep lines"
    static dt_ntype_b08_t data[NCOLS*NROWS] = {
       0,0,0,0,0,0,0,0,0,0,
       0,1,0,2,0,0,3,0,4,0,
       0,1,0,2,0,0,3,0,4,0,
       0,1,0,2,0,0,3,0,4,0,
       0,1,0,2,0,0,3,0,4,0,
       0,0,1,0,2,3,0,4,0,0,
       0,0,1,0,2,3,0,4,0,0,
       0,0,1,0,2,3,0,4,0,0,
       0,0,1,0,2,3,0,4,0,0,
       0,0,0,0,0,0,0,0,0,0,
	};

    DT_Q(dttplot_make_and_clear,(ctl,	/* make and clear images		    */
      &image, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &data[0]));

    DT_Q(dtimage_plot_line,(ctl,		/* make plot					  	*/
      &image, 1, 1, 2, 8, 1.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 4, 8, 3, 1, 2.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 6, 1, 5, 8, 3.0));
    DT_Q(dtimage_plot_line,(ctl,
      &image, 7, 8, 8, 1, 4.0));

    DT_Q(dttplot_compare_and_free,(ctl,	/* compare and free images		    */
      &image, &expect, DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

    return DT_RC_GOOD;    
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttplot_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttplot_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttplot)
{
  DT_F("dttplot");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking             */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want  	*/
    &param->op, NULL, NULL, "fast",
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, USAGE, param,
    arg, dttplot_arg_check, NULL));
                                        /* ................................ */
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "fast"))
      {
		DT_C(dttplot_bars,(ctl));		/* test bars routine(s)			    */
		DT_C(dttplot_lines,(ctl));		/* test line routine(s)			    */
      }
	}
  }

                                        /* ................................ */
cleanup:
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
