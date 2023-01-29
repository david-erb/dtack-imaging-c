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
  19990105 DE test for big endian machine before reading data files
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
DT_RCSID("test $RCSfile: dttconv.c,v $ $Revision: 1.7 $");
#include <dtack/mem.h>

#define USAGE "[-Dnnnn]"

static dtntype_s4_t h1d_kernel[3] = {
  1,1,1
};
static dtntype_s4_t ave_kernel[9] = {
  1,1,1,
  1,1,1,
  1,1,1
};
static dtntype_s4_t highpass_kernel[9] = {
  -1,-1,-1,
  -1, 8,-1,
  -1,-1,-1
};
static dtntype_s4_t laplace_kernel[9] = {
   0,-1, 0,
  -1, 4,-1,
   0,-1, 0 
};
static dtntype_s4_t cross_kernel[9] = {
   0, 1, 0,
   1, 1, 1,
   0, 1, 0 
};

static dtntype_s4_t h1d_kernel4[4] = {
  1,1,1,1
};
static dtntype_s4_t ave_kernel4[16] = {
  1,1,1,1,
  1,1,1,1,
  1,1,1,1,
  1,1,1,1
};


static dtntype_s4_t h1d_kernel5[5] = {
  1,1,1,1,1
};
static dtntype_s4_t ave_kernel5[25] = {
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1
};
static dtntype_s4_t laplace_kernel5[25] = {
   0, 0,-1, 0, 0,
   0, 0,-1, 0, 0,
  -1,-1, 8,-1,-1,
   0, 0,-1, 0, 0,
   0, 0,-1, 0, 0
};
static dtntype_s4_t cross_kernel5[25] = {
   0, 0, 1, 0, 0,
   0, 0, 1, 0, 0,
   1, 1, 1, 1, 1,
   0, 0, 1, 0, 0,
   0, 0, 1, 0, 0
};

static double h1d_kernel5_double[5] = {
  1,1,1,1,1
};
static double ave_kernel5_double[25] = {
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1,
  1,1,1,1,1
};
static double laplace_kernel5_double[25] = {
   0, 0,-1, 0, 0,
   0, 0,-1, 0, 0,
  -1,-1, 8,-1,-1,
   0, 0,-1, 0, 0,
   0, 0,-1, 0, 0
};
static double cross_kernel5_double[25] = {
   0, 0, 1, 0, 0,
   0, 0, 1, 0, 0,
   1, 1, 1, 1, 1,
   0, 0, 1, 0, 0,
   0, 0, 1, 0, 0
};
static dtntype_s4_t ave_kernel7[49] = {
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,
  1,1,1,1,1,1,1
};

#define BIG (512)

static const char *ops[] = {"all", "fast", "time", NULL};

#ifdef DTCONFIG_IMPLIED_DOS16
#define PTR_TO_ULONG(P) \
  ((unsigned long)(FP_SEG((void *)(P))) * 16UL + \
   (unsigned long)(FP_OFF((void *)(P))))
#else
#define PTR_TO_ULONG(P) \
  ((unsigned long)(P))
#endif

/*..........................................................................
 *..........................................................................*/
static
int
cpu_big_endian()						/* true if big-endian at runtime	*/
{
  static unsigned short word = 0x0102;
  return *(char *)&word == 0x01;
}

/*..........................................................................
 * create image with given form factor and populate with given data
 *..........................................................................*/
static 
dt_rc_e
dttconv_make(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  void *data)
{
  DT_F("dttconv_make");
  dtxy_t y;

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    image, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, image, y, &p);
    DT_MEMCPY(p, data, image->rowsize);
    DTIMAGE_PUTROW(ctl, image, y);
    data = (char *)data + image->rowsize;
  }

  return DT_RC_GOOD;
}
/*..........................................................................
 * check that given image contains the expected data
 *..........................................................................*/
static 
dt_rc_e
dttconv_check_and_free(
  dt_ctl_t *ctl,
  dtimage_t *conv,
  void *conv_data,
  char *f,
  char *id,
  int *assert_fail)
{
  DT_F("dttconv_check_and_free");
  dtimage_t conv_expect;
  long cmp;

  DT_Q(dttconv_make,(ctl,				/* allocate expected conv image		*/
     &conv_expect, 
     conv->xe, conv->ye,				/* same size and shape as actual    */
     conv->ntype,
     conv_data));

  DT_Q(dtimage_cmp,(ctl,				/* compare expected to actual	    */
    conv, &conv_expect, 0,
    &cmp, NULL, NULL));

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, cmp == 0,
      f, "%s", id);

  if (cmp != 0)							/* appears to be a problem? 		*/
    DT_Q(dtimage_dbg_values,(ctl,
      conv, NULL, DT_DBG_MASK_PIXEL));

  DT_Q(dtimage_free,(ctl,				/* free expected image			    */
    &conv_expect));

  return DT_RC_GOOD;
}

/*..........................................................................
 * convolve very simple input image with a number of kernels
 *..........................................................................*/
static
dt_rc_e
dttconv1(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttconv1");
  dtimage_t input;
  dtimage_t conv;

#define NCOLS 7
#define NROWS 7
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,
    0,0,1,1,1,0,0,
    0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t cross_data[NCOLS*NROWS] = {
      0,   0,   0,   0,   0,   0,   0,
      0,   0,   1,   1,   1,   0,   0,
      0,   1,   3,   4,   3,   1,   0,
      0,   1,   4,   5,   4,   1,   0,
      0,   1,   3,   4,   3,   1,   0,
      0,   0,   1,   1,   1,   0,   0,
      0,   0,   0,   0,   0,   0,   0
  };
  static dt_ntype_b08_t h1d_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,
    0,1,2,3,2,1,0,
    0,1,2,3,2,1,0,
    0,1,2,3,2,1,0,
    0,0,0,0,0,0,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t ave_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,1,2,3,2,1,0,
    0,2,4,6,4,2,0,
    0,3,6,9,6,3,0,
    0,2,4,6,4,2,0,
    0,1,2,3,2,1,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_s1_t laplace_data[NCOLS*NROWS] = {
      0,   0,   0,   0,   0,   0,   0,
      0,   0,  -1,  -1,  -1,   0,   0,
      0,  -1,   2,   1,   2,  -1,   0,
      0,  -1,   1,   0,   1,  -1,   0,
      0,  -1,   2,   1,   2,  -1,   0,
      0,   0,  -1,  -1,  -1,   0,   0,
      0,   0,   0,   0,   0,   0,   0
  };

  DT_Q(dttconv_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS, DT_NTYPE_B08));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &h1d_kernel[0], 3, 1,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &h1d_data[0], F,
    "horizontal 1d", assert_fail));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &ave_kernel[0], 3, 3,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &ave_data[0], F,
    "average", assert_fail));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &cross_kernel[0], 3, 3,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &cross_data[0], F,
    "cross", assert_fail));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &laplace_kernel[0], 3, 3,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &laplace_data[0], F,
    "laplace", assert_fail));
  
  DT_Q(dtimage_free,(ctl, &conv));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 * convolve an input image with a number of different kernels
 *..........................................................................*/
static
dt_rc_e
dttconv2(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttconv2");
  dtimage_t input;
  dtimage_t conv;

#define NCOLS 7
#define NROWS 7
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t h1d_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,2,3,3,3,2,0,
    0,2,3,3,3,2,0,
    0,2,3,3,3,2,0,
    0,2,3,3,3,2,0,
    0,2,3,3,3,2,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t ave_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,4,6,6,6,4,0,
    0,6,9,9,9,6,0,
    0,6,9,9,9,6,0,
    0,6,9,9,9,6,0,
    0,4,6,6,6,4,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t cross_data[NCOLS*NROWS] = {
      0,   0,   0,   0,   0,   0,   0,
      0,   3,   4,   4,   4,   3,   0,
      0,   4,   5,   5,   5,   4,   0,
      0,   4,   5,   5,   5,   4,   0,
      0,   4,   5,   5,   5,   4,   0,
      0,   3,   4,   4,   4,   3,   0,
      0,   0,   0,   0,   0,   0,   0
  };
  static dt_ntype_b08_t laplace_data[NCOLS*NROWS] = {
      0,   0,   0,   0,   0,   0,   0,
      0,   2,   1,   1,   1,   2,   0,
      0,   1,   0,   0,   0,   1,   0,
      0,   1,   0,   0,   0,   1,   0,
      0,   1,   0,   0,   0,   1,   0,
      0,   2,   1,   1,   1,   2,   0,
      0,   0,   0,   0,   0,   0,   0
  };

  DT_Q(dttconv_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS, DT_NTYPE_B08));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &h1d_kernel[0], 3, 1,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &h1d_data[0], F,
    "horizontal 1d", assert_fail));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &ave_kernel[0], 3, 3,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &ave_data[0], F,
    "average", assert_fail));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &cross_kernel[0], 3, 3,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &cross_data[0], F,
    "cross", assert_fail));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &laplace_kernel[0], 3, 3,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &laplace_data[0], F,
    "laplace", assert_fail));
  
  DT_Q(dtimage_free,(ctl, &conv));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttconv4(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttconv4");
  dtimage_t input;
  dtimage_t conv;

#define NCOLS 7
#define NROWS 7
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t h1d_data[NCOLS*NROWS] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x03,0x04,0x04,0x03,0x00,
	0x00,0x00,0x03,0x04,0x04,0x03,0x00,
	0x00,0x00,0x03,0x04,0x04,0x03,0x00,
	0x00,0x00,0x03,0x04,0x04,0x03,0x00,
	0x00,0x00,0x03,0x04,0x04,0x03,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };
  static dt_ntype_b08_t ave_data[NCOLS*NROWS] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x09,0x0c,0x0c,0x09,0x00,
	0x00,0x00,0x0c,0x10,0x10,0x0c,0x00,
	0x00,0x00,0x0c,0x10,0x10,0x0c,0x00,
	0x00,0x00,0x09,0x0c,0x0c,0x09,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };

  DT_Q(dttconv_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS, DT_NTYPE_B08));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &h1d_kernel4[0], 4, 1,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &h1d_data[0], F,
    "horizontal 1d", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &ave_kernel4[0], 4, 4,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &ave_data[0], F,
    "average", assert_fail));
  
  DT_Q(dtimage_free,(ctl, &conv));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttconv5(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttconv5");
  dtimage_t input;
  dtimage_t conv;

#define NCOLS 7
#define NROWS 7
  static dt_ntype_b08_t input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_b08_t h1d_data[NCOLS*NROWS] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x04,0x05,0x04,0x00,0x00,
    0x00,0x00,0x04,0x05,0x04,0x00,0x00,
    0x00,0x00,0x04,0x05,0x04,0x00,0x00,
    0x00,0x00,0x04,0x05,0x04,0x00,0x00,
    0x00,0x00,0x04,0x05,0x04,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };
  static dt_ntype_b08_t ave_data[NCOLS*NROWS] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x10,0x14,0x10,0x00,0x00,
	0x00,0x00,0x14,0x19,0x14,0x00,0x00,
	0x00,0x00,0x10,0x14,0x10,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };
  static dt_ntype_b08_t cross_data[NCOLS*NROWS] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x08,0x07,0x00,0x00,
	0x00,0x00,0x08,0x09,0x08,0x00,0x00,
	0x00,0x00,0x07,0x08,0x07,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };
  static dt_ntype_b08_t laplace_data[NCOLS*NROWS] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x02,0x01,0x02,0x00,0x00,
	0x00,0x00,0x01,0x00,0x01,0x00,0x00,
	0x00,0x00,0x02,0x01,0x02,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };

  DT_Q(dttconv_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_B08,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS, DT_NTYPE_B08));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));

  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &h1d_kernel5[0], 5, 1,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &h1d_data[0], F,
    "horizontal 1d", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &ave_kernel5[0], 5, 5,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &ave_data[0], F,
    "average", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &cross_kernel5[0], 5, 5,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &cross_data[0], F,
    "cross", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_s4,(ctl,		/* do convolution				    */
    &input, &laplace_kernel5[0], 5, 5,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &laplace_data[0], F,
    "laplace", assert_fail));
  
  DT_Q(dtimage_free,(ctl, &conv));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dttconv6(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttconv6");
  dtimage_t input;
  dtimage_t conv;

#define NCOLS 7
#define NROWS 7
  static dt_ntype_f2_t input_data[NCOLS*NROWS] = {
    0,0,0,0,0,0,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,1,1,1,1,1,0,
    0,0,0,0,0,0,0
  };
  static dt_ntype_f2_t h1d_data[NCOLS*NROWS] = {
     0, 0, 0, 0, 0, 0, 0,
     0, 0, 4, 5, 4, 0, 0,
     0, 0, 4, 5, 4, 0, 0,
     0, 0, 4, 5, 4, 0, 0,
     0, 0, 4, 5, 4, 0, 0,
     0, 0, 4, 5, 4, 0, 0,
     0, 0, 0, 0, 0, 0, 0
  };
  static dt_ntype_f2_t ave_data[NCOLS*NROWS] = {
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0,16,20,16, 0, 0,
	 0, 0,20,25,20, 0, 0,
	 0, 0,16,20,16, 0, 0,
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0
  };
  static dt_ntype_f2_t cross_data[NCOLS*NROWS] = {
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 7, 8, 7, 0, 0,
	 0, 0, 8, 9, 8, 0, 0,
	 0, 0, 7, 8, 7, 0, 0,
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0
  };
  static dt_ntype_f2_t laplace_data[NCOLS*NROWS] = {
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 2, 1, 2, 0, 0,
	 0, 0, 1, 0, 1, 0, 0,
	 0, 0, 2, 1, 2, 0, 0,
	 0, 0, 0, 0, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0
  };

  DT_Q(dttconv_make,(ctl, &input,		/* make input image				    */
    NCOLS, NROWS, DT_NTYPE_F2,
    &input_data[0]));

  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS, DT_NTYPE_F2));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));

  DT_Q(dtimage_convolve_double,(ctl,	/* do convolution				    */
    &input, &h1d_kernel5_double[0], 5, 1,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &h1d_data[0], F,
    "horizontal 1d", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_double,(ctl,	/* do convolution				    */
    &input, &ave_kernel5_double[0], 5, 5,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &ave_data[0], F,
    "average", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_double,(ctl,	/* do convolution				    */
    &input, &cross_kernel5_double[0], 5, 5,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &cross_data[0], F,
    "cross", assert_fail));

  DT_Q(dtimage_constant,(ctl,			/* clear output image				*/
    &conv, 0.0));
  DT_Q(dtimage_convolve_double,(ctl,	/* do convolution				    */
    &input, &laplace_kernel5_double[0], 5, 5,
    &conv));
  DT_Q(dttconv_check_and_free,(
    ctl, &conv, &laplace_data[0], F,
    "laplace", assert_fail));
  
  DT_Q(dtimage_free,(ctl, &conv));

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttconv_big(
  dt_ctl_t *ctl,
  const char *desc,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  long n,
  int *assert_fail)
{
  DT_F("dttconv_big");
  dtimage_t input;
  dtimage_t conv;
  dtos_time_t mark1, mark2;
  long i;
  int j;
  int didit;
  double kdouble[49];
  char id[64];

#define NCOLS BIG
#define NROWS BIG

  DT_Q(dtimage_create,(ctl, &input,
    NCOLS, NROWS, DT_NTYPE_B08));
  DT_Q(dtimage_constant,(ctl, &input,	/* fill image with non-zero			*/
    1.0));

										/* ................................ */
  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS,
    DT_NTYPE_B08));

#ifdef NONO
  DT_Q(dtos_time,(ctl, &mark1));
  didit = 0;
  if (input.ntype == DT_NTYPE_B08 &&
	  conv.ntype == DT_NTYPE_B08)
  for (i=0; i<n; i++)
  {
    DT_Q(dtimage_convolve_s4_b08,		/* do convolution fast			    */
      (ctl,
       &input, kernel, kxsize, kysize,
       1, &conv, &didit));
	if (!didit) 
	  break;
  }
  DT_Q(dtos_time,(ctl, &mark2));

  if (didit)
  {
    sprintf(id, "%s long*  (%ld)", 
      desc, n);
    DT_Q(dtt_time_report,(ctl,			/* report time					    */
      &mark1, &mark2, id));
  }
#endif

										/* ................................ */
  DT_Q(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_Q(dtimage_convolve_s4,(ctl,	/* do convolution slow			    */
      &input, kernel, kxsize, kysize,
      &conv));
  }
  DT_Q(dtos_time,(ctl, &mark2));
  sprintf(id, "%s long   (%ld)", 
	desc, n);
  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q(dtimage_free,(ctl, &conv));

										/* ................................ */
  DT_Q(dtimage_create,(ctl, &conv,		/* space for convolution output 	*/
    NCOLS, NROWS,
    DT_NTYPE_F2));

  for (j=0; j<kxsize*kysize; j++)		/* convert kernel to double 		*/
    kdouble[j] = kernel[j];

  DT_Q(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_Q(dtimage_convolve_double,(ctl,	/* do convolution				    */
      &input, kdouble, kxsize, kysize,
      &conv));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  sprintf(id, "%s double (%ld)", desc, n);
  DT_Q(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_Q(dtimage_free,(ctl, &conv));
										/* ................................ */

  DT_Q(dtimage_free,(ctl, &input));

#undef NCOLS
#undef NROWS
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dttconv_known(
  dtparam_t *param,
  const char *input_name,
  const char *expect_name,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  long n,
  int *assert_fail)
{
  DT_F("dttconv_known");
  dt_ctl_t *ctl = &param->ctl;
  char input_name2[128];
  char expect_name2[128];
  dtimage_t input;
  dtimage_t expect;
  dtimage_t output;
  dtos_time_t mark1, mark2;
  long i;
  long cmp;
  char id[64];
  dt_rc_e rc;

      DT_C(dtstr_printf,(ctl, input_name2,
        sizeof(input_name2), "%s/%s",
        param->datadir, input_name));

      DT_C(dtstr_printf,(ctl, expect_name2,
        sizeof(expect_name2), "%s/%s",
        param->datadir, expect_name));

  sprintf(id, "%s (%ld)", 
      expect_name, n);
  
  input.xe = 0;
  expect.xe = 0;
  output.xe = 0;

  DT_C(dtimage_import,(ctl, &input,
    input_name2));

  DT_C(dtimage_create,(ctl, &output,	/* space for convolution output 	*/
    input.xe, input.ye,
    DT_NTYPE_S2));
  DT_Q(dtimage_constant,(ctl, &output,	/* clear output edges 				*/
    0.0));
  DT_C(dtos_time,(ctl, &mark1));
  for (i=0; i<n; i++)
  {
    DT_C(dtimage_convolve_s4,(ctl,		/* do convolution				    */
      &input, kernel, 
      kxsize, kysize, 
      &output));
  }
  DT_C(dtos_time,(ctl, &mark2));

  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

  DT_C(dtimage_free,(ctl, &input));		/* free input space 				*/
  input.xe = 0;

  DT_C(dtimage_import,(ctl, &expect,	/* read expected image 				*/
    expect_name2));

  DT_C(dtimage_cmp,(ctl, &expect,		/* compare output to expected 		*/
    &output, 0, &cmp, NULL, NULL));

  if (cmp != 0)
  {
	DT_Q(dtimage_dbg_values,(ctl,
      &output, "output", DT_DBG_MASK_PIXEL));
	DT_Q(dtimage_dbg_values,(ctl,
      &expect, "expect", DT_DBG_MASK_PIXEL));
  }

  *assert_fail +=
    DT_RC_GOOD != dt_assert(
      ctl, cmp == 0,
      F, "%s cmp %ld (0)", id, cmp);

cleanup:
  if (expect.xe)
  DT_I(dtimage_free,(ctl, &expect));

  if (output.xe)
  DT_I(dtimage_free,(ctl, &output));

  if (input.xe)
  DT_I(dtimage_free,(ctl, &input));

  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttconv_arg_check(          				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttconv_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttconv)
{
  DT_F("dttconv");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;					/* failed assertion count		    */
  dt_enter_arg_t arg[1];
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->datadir, NULL, NULL, NULL,
    &param->input, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "4",
    NULL));

  arg[0].desc = NULL;					/* no arguments 					*/
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttconv_arg_check, NULL));

  while (rc == DT_RC_GOOD)
  {
    if (!strcmp(param->op, "all") ||
        !strcmp(param->op, "fast"))
    {
      DT_GI(dttconv1,(ctl, &assert_fail));
      DT_GI(dttconv2,(ctl, &assert_fail));
      DT_GI(dttconv4,(ctl, &assert_fail));
      DT_GI(dttconv5,(ctl, &assert_fail));
      DT_GI(dttconv6,(ctl, &assert_fail));
    }

    if (!strcmp(param->op, "fast"))
	  param->n = 1;

    if (!strcmp(param->op, "all") ||
        !strcmp(param->op, "time"))
    {
	  int *a = &assert_fail;
      DT_GI(dttconv_big,(ctl, "ave 3x3    ", ave_kernel, 3, 3, param->n/2, a));
      DT_GI(dttconv_big,(ctl, "ave 5x5    ", ave_kernel5, 5, 5, param->n/2, a));
      DT_GI(dttconv_big,(ctl, "ave 7x7    ", ave_kernel7, 5, 5, param->n/2, a));
      DT_GI(dttconv_big,(ctl, "laplace 3x3", laplace_kernel, 3, 3, param->n/2, a));
      DT_GI(dttconv_big,(ctl, "laplace 5x5", laplace_kernel5, 5, 5, param->n/2, a));
      DT_GI(dttconv_big,(ctl, "h1d 3x1    ", h1d_kernel, 3, 1, param->n/2, a));
      DT_GI(dttconv_big,(ctl, "h1d 5x1    ", h1d_kernel5, 5, 1, param->n/2, a));
	}

	if (!cpu_big_endian())				/* little endian machine? 			*/
	{
      DT_GI(dttconv_known,(param,
        "mad256/t5.pgm", "mad256/t5high3.pgm",
        highpass_kernel, 3, 3,
        param->n, &assert_fail));

      DT_GI(dttconv_known,(param,
        "mad256/t5.pgm", "mad256/t5ave5.pgm",
        ave_kernel5, 5, 5,
        param->n, &assert_fail));

      DT_GI(dttconv_known,(param,
        "mad256/t5.pgm", "mad256/t5ave7.pgm",
        ave_kernel7, 7, 7,
        param->n, &assert_fail));

      DT_GI(dttconv_known,(param,
        "mad256/t5.pgm", "mad256/t5ave3.pgm",
        ave_kernel, 3, 3,
        param->n, &assert_fail));
	}

    DT_G(dt_interactive_after,(			/* make post display				*/
      ctl, param, USAGE, arg,
      dttconv_arg_check, NULL, rc));
  } 

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

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
