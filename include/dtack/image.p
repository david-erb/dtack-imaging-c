#ifdef __cplusplus
extern "C" {
#endif
#ifdef DX_PREFIX
#  undef DX_PREFIX
#endif
#ifndef DT_PREFIX
#  define DX_PREFIX
#else
#  define DX_PREFIX DT_PREFIX
#endif
#ifdef DX_SUFFIX
#  undef DX_SUFFIX
#endif
#ifndef DT_SUFFIX
#  define DX_SUFFIX ;
#else
#  define DX_SUFFIX DT_SUFFIX
#endif
#ifdef DT_DEFARGVAL
#  undef DT_DEFARGVAL
#  define DT_DEFARGVAL(A) = (A)
#endif

/*----in assign.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_assign_va(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of args				*/
  va_list ap)							/* variable arguments 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API2
dtimage_assign(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of args				*/
  ...)									/* variable arguments 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_assign_array(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  int xe,								/* number of columns 				*/
  int ye,								/* number of rows 					*/
  dtntype_e ntype,						/* pixel type of array values		*/
  const void *array)					/* array containing values			*/
DX_SUFFIX

/*----in image.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_dup(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* original image					*/
  dtimage_t *dup)						/* new image 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_free(							/* free image data space			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image)						/* image to free					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_overlay(						/* overlay new image on old memory	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* underlying image					*/
  dtimage_t *overlay,					/* image structure to initialize 	*/
  dtxy_t x0,							/* starting column 					*/
  dtxy_t y0,							/* starting row 					*/
  dtxy_t xe,							/* overlayed image width 			*/
  dtxy_t ye)							/* overlayed image height 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_create3(						/* create and allocate image		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to create					*/
  unsigned int pagesize,				/* big memory page size 			*/
  unsigned int npages,					/* big memory access buffers needed */
  long width,							/* image width in columns			*/
  long height,							/* image height in rows 			*/
  dt_ntype_e ntype,						/* image pixel data type 			*/
  void *pre_memalloc)					/* pre-allocated data, if any	    */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_create2(						/* create and allocate image		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to create					*/
  long width,							/* image width in columns			*/
  long height,							/* image height in rows 			*/
  dt_ntype_e ntype,						/* image pixel data type 			*/
  void *memalloc)						/* pre-allocated data, if any	    */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_create(							/* create and allocate image		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to create					*/
  long width,							/* image width in columns			*/
  long height,							/* image height in rows 			*/
  dt_ntype_e ntype)						/* image pixel data type 			*/
DX_SUFFIX

/*----in import.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_import(							/* import image						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to be allocated and input 	*/
  const char *spec)						/* source specification 			*/
DX_SUFFIX

/*----in export.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_export(							/* convert and output image			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* source image to be converted 	*/
  const char *spec,						/* destination specification 		*/
  dtxy_t xpos,							/* output column 					*/
  dtxy_t ypos)							/* output row 						*/
DX_SUFFIX

/*----in read.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_raw_fd(					/* get raw image data from open fd	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned int rowsize,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_pnm_file(					/* get image from pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_wmfbmp_readfunc(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_wmf_fd(					/* get image from opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_wmf_file(					/* get image from pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_bmp_fd(					/* get image from opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_bmp_file(					/* get image from pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

/*----in readtif.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_tiff_file(	
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *filename)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_read_tiff_file(					/* get image from tif file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *filename)
DX_SUFFIX

/*----in write.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_raw_fd(					/* write raw image data on open fd	*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  unsigned int reserved,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_ascii_fd(					/* write ascii image data on fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_lj_fd(					/* put image to opened lj fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd,
  int resolution,						/* resoluton or 0 to leave as is    */
  dtxy_t xpos,							/* in units of resolution or -1	    */
  dtxy_t ypos,
  long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_lj_file(					/* put image to laserjet file		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name,
  int resolution,
  dtxy_t x,
  dtxy_t y,
  long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_fd(					/* put image to opened pgm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_file(					/* put image to pgm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_ascii_fd(				/* put image to opened pgm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pgm_ascii_file(			/* put image to pgm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_fd(					/* put image to opened pbm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_ascii_fd(				/* put image to opened pbm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_file(					/* put image to pbm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pbm_ascii_file(			/* put image to pbm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_fd(					/* put image to opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_file(					/* put image to pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_ascii_fd(				/* put image to opened pnm fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_pnm_ascii_file(			/* put image to pnm file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name)
DX_SUFFIX

/*----in writebmp.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_bmp_func(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_bmp_fd(					/* put image to opened bmp fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd,
  dtxy_t x,
  dtxy_t y,
  long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_bmp_file(					/* put image to bmp file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name,
  dtxy_t x,
  dtxy_t y,
  long flags)
DX_SUFFIX

/*----in writewmf.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_wmf_func(
  dt_ctl_t *ctl,
  void *arg,
  void *data,
  unsigned int size)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_wmf_fd(					/* put image to opened wmf fd		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtfd_t *fd,
  dtxy_t x,
  dtxy_t y,
  long flags)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_write_wmf_file(					/* put image to wmf file			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const char *name,
  dtxy_t x,
  dtxy_t y,
  long flags)
DX_SUFFIX

/*----in thresh.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_thresh_scalar(					/* threshold image by scalar		*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  double scalar,						/* threshold						*/
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_thresh_image(					/* "thresh" two images				*/
  dt_ctl_t *ctl,
  dtimage_t *input1,					/* input images						*/
  dtimage_t *input2,
  double below,							/* value for pixels below thresh	*/
  double above,							/* value for pixels at or above		*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_thresh_multi(					/* threshold image by array			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const double * const thresh,			/* threshold array					*/
  unsigned int n,
  const double * const values,			/* output array					    */
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

/*----in logical.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_logical(						/* do logical op with each pixel	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image 				*/
  unsigned long a,						/* first constant 					*/
  dtimage_t *input2,					/* second input image 				*/
  unsigned long b,						/* second constant 					*/
  unsigned long c,						/* final scalar constant 			*/
  dt_op_e op,							/* operation to perform 			*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

/*----in arith.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_arith(							/* do scalar op with each pixel		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image 				*/
  double a,								/* first image factor 				*/
  dtimage_t *input2,					/* second input image 				*/
  double b,								/* second image factor 				*/
  double c,								/* offset 							*/
  dt_op_e op,							/* operation to perform 			*/
  unsigned long assign_flags,			/* final assignment flags 			*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

/*----in check.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_check_1f(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image to be checked				*/
  const char *desc,						/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  unsigned long flags,					/* flags describing needs 			*/
  const char *f)						/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_check_2f(						/* simple two image check			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  unsigned long flags,					/* flags describing needs 			*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_check_3f(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dtimage_t *image3,				/* third image two be checked 		*/
  const char *desc3,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  unsigned long flags,					/* flags describing needs 			*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_check_1(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image to be checked				*/
  const char *desc,						/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f)						/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_check_2(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_check_3(						/* check simple things				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image to be checked		*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image to be checked 		*/
  const char *desc2,					/* image description string 		*/
  const dtimage_t *image3,				/* third image two be checked 		*/
  const char *desc3,					/* image description string 		*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f,						/* caller ID 						*/
  dtxy_t *xmax,							/* minimum of the widths 			*/
  dtxy_t *ymax)							/* minimum of the heights 			*/
DX_SUFFIX

/*----in dbg.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_to_string(						/* describe image as string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image being operated on			*/
  const char *desc,						/* image description 				*/
  char *string,							/* returned string 					*/
  int max)								/* max length of string 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_dbg_op(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image being operated on			*/
  const char *desc,						/* image description 				*/
  const char *f)						/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_dbg_values(						/* output pixel values as debug 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image whose pixels to print		*/
  const char *f,						/* caller ID 						*/
  const long mask)						/* debug mask 						*/
DX_SUFFIX

/*----in err.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_err_outside(					/* error due to pixel outside image	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image being exceeded				*/
  const char *desc,						/* image description 				*/
  const dtxy_t x,						/* the offending column				*/
  const dtxy_t y,						/* the offending row 				*/
  char *f)								/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_err_ntype(						/* error because can't do ntype		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image whose ntype is wrong		*/
  const char *desc,						/* image description 				*/
  const dt_ntype_e *ntypes,				/* list of valid data types 		*/
  const char *f)						/* caller ID 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_err_ntype_2(					/* error because ntypes differ		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image1,				/* first image						*/
  const char *desc1,					/* image description string 		*/
  const dtimage_t *image2,				/* second image 					*/
  const char *desc2,					/* image description string 		*/
  const char *f)						/* caller ID 						*/
DX_SUFFIX

/*----in fft.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_fft_double(						/* compute double-precision 2D FFT	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *output)					/* returned frequency domain image	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_fft_complex(					/* compute double-precision 2D FFT	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  int forward,							/* true if forward transform 		*/
  dtimage_t *output)					/* returned frequency domain image	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_fft_polar(						/* return fft in polar form			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *polar)						/* returned polar image 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_fft_polar2(						/* return fft in polar form			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *magnitude,					/* returned magnitude image 		*/
  dtimage_t *angle)						/* returned angle image 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_fft_magnitude(					/* return FFT magnitude				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *magnitude)					/* returned magnitude image			*/
DX_SUFFIX

/*----in fftw.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_fftw_2d(						/* compute 2D FFT					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  int isreal,							/* true if input is real valued		*/
  int forward,							/* true if forward transform 		*/
  dtimage_t *output)					/* returned frequency domain image	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_log10(							/* log 10 image						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

/*----in log.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_log(							/* log image						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_sin(							/* sine of each pixel				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cos(							/* cosine of each pixel				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_sqrt(							/* square root of each pixel		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  double factor,						/* multiplication factor		    */
  double offset,						/* scalar offset					*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_abs(							/* absolute value of each pixel		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image						*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

/*----in noise.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_noise_gauss_add(				/* add gaussian random values		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dt1d_ran_t *ran,
  double mean,
  double sigma)							/* stddev i.e. sqrt(variance)		*/
DX_SUFFIX

/*----in cast.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cast_row(						/* copy/convert one row of image	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image						*/
  dtxy_t input_y,						/* source row 						*/
  dtimage_t *output,					/* destination image				*/
  dtxy_t output_y)						/* destination row					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cast_row2(						/* copy/convert one row of image	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image						*/
  dtxy_t input_x0,						/* source offset 					*/
  dtxy_t input_dx,						/* source stride 					*/
  dtxy_t input_y,						/* source row 						*/
  dtimage_t *output,					/* destination image				*/
  dtxy_t output_x0,						/* destination offset				*/
  dtxy_t output_dx,						/* destination stride				*/
  dtxy_t output_y)						/* destination row					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cast(							/* copy/convert image to another 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image 					*/
  dtimage_t *output)					/* destination image 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cast2(							/* copy/convert image to another 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* source image 					*/
  dtxy_t input_x0,						/* source offset 					*/
  dtxy_t input_dx,						/* source stride 					*/
  dtimage_t *output,					/* destination image 				*/
  dtxy_t output_x0,						/* destination offset				*/
  dtxy_t output_dx)						/* destination stride				*/
DX_SUFFIX

/*----in col.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_col_put(
  dt_ctl_t *ctl,															  
  dtimage_t *image,						/* image to receive data 			*/
  dtxy_t col,							/* column number 					*/
  void *data,							/* data to be copied to column 		*/
  int dir)								/* traversal direction 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_col_get(
  dt_ctl_t *ctl,															  
  dtimage_t *image,						/* image to receive data 			*/
  dtxy_t col,							/* column number 					*/
  void *data,							/* data to be copied to column 		*/
  int dir)								/* traversal direction 				*/
DX_SUFFIX

/*----in convolve.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *kernel,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_boxcarr(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int kernel_size,
  dtimage_t *work,
  dtimage_t *output)
DX_SUFFIX

/*----in cooc.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cooc_sum(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int dx,
  int dy,
  dtimage_t *cooc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cooc_set(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int dx,
  int dy,
  dtimage_t *cooc)
DX_SUFFIX

/*----in cooc2.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cooc_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2,
  dtxy_t xe,
  dtxy_t ye,
  dtimage_t *cooc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cooc_any(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2,
  dtxy_t xe,
  dtxy_t ye,
  dtimage_t *cooc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cooc_mirror(
  dt_ctl_t *ctl,
  dtimage_t *cooc)
DX_SUFFIX

/*----in constant.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_constant(						/* set all pixels to constant	    */
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image							*/
  const double constant)				/* constant to set image to		    */
DX_SUFFIX

/*----in conv_l.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_s4(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  dtimage_t *output)
DX_SUFFIX

/*----in conv_d.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_double(
  dt_ctl_t *ctl,
  dtimage_t *input,
  double *kernel,
  int kxsize,
  int kysize,
  dtimage_t *output)
DX_SUFFIX

/*----in conv_s.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_bbn(					/* b08 b08 any long kernel 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_bb3(					/* b08 b08 any 3x3 long kernel		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  long * const mul,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_bb3_laplace(			/* b08 b08 laplace kernel 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  long * const mul4,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_bb3_ave(				/* b08 b08 average kernel 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  const dtxy_t x1, const dtxy_t y1,
  const dtxy_t x2, const dtxy_t y2,
  const dtntype_s4_t *kernel,
  const int kx1, const int ky1,
  const int kx2, const int ky2,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_convolve_bb(					/* special b08 b08 cases 			*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtntype_s4_t *kernel,
  int kxsize,
  int kysize,
  dtimage_t *output)
DX_SUFFIX

/*----in matrix.c:----*/


DX_PREFIX
dt_rc_e 
dtimage_lu_decompose(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* the image matrix 				*/
  dt1d_i_t *indices,
  double *d,
  int *singular)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
dtimage_lu_substitute(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* the image matrix 				*/
  dt1d_i_t *indices,
  double *b)
DX_SUFFIX

/*----in joint.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_joint(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtimage_t *joint)
DX_SUFFIX

/*----in joint2.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_joint_b08(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtxy_t xmax,
  dtxy_t ymax,
  dtimage_t *joint)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_joint_any(
  dt_ctl_t *ctl,
  dtimage_t *input1,
  dtimage_t *input2,
  dtxy_t xmax,
  dtxy_t ymax,
  dtimage_t *joint)
DX_SUFFIX

/*----in label1.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_run_label(						/* label runs by blob membership	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* input image from which runs came */
  dtrun_t *run,							/* initialized run structure 		*/
  unsigned long flags,					/* process control 					*/
  dtrun_deliver_f *deliver,				/* function to call for each blob 	*/
  void *arg)							/* user argument to pass to deliver */
DX_SUFFIX

/*----in lookup.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_lookup(							/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  double *lut,							/* transformation array				*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_lookup_b08(						/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const dt_ntype_b08_t *lut,			/* transformation array				*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_lookup_b16(						/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const dt_ntype_b16_t *lut,			/* transformation array				*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_lookup_s2(						/* transform image by lut			*/
  dt_ctl_t *ctl,
  dtimage_t *input,						/* input image						*/
  const dt_ntype_s2_t *lut,				/* transformation array				*/
  dtxy_t minval,						/* lowest transformation value		*/
  dtxy_t n,								/* number of transformation values	*/
  dtimage_t *output)					/* output image						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_lookup_hist_slow(				/* lookup image through histogram	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dthist_t *hist,						/* hist structure containing lut 	*/
  dtimage_t *output)					/* output image 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_lookup_hist_fast(				/* lookup image through histogram	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dthist_t *hist,						/* hist structure containing lut 	*/
  int *cando,							/* return if we could do it			*/
  dtimage_t *output)					/* output image 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_lookup_hist(					/* lookup image through histogram	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dthist_t *hist,						/* hist structure containing lut 	*/
  dtimage_t *output)					/* output image 					*/
DX_SUFFIX

/*----in sum.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_sum(					/* sum of pixel values				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *psum)
DX_SUFFIX

/*----in sum_rows.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_sum_rows(						/* make sum of each row				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *sum,							/* returned sum vector			    */
  dtxy_t n)								/* max length of sum vector		    */
DX_SUFFIX

/*----in sum_cols.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_sum_cols(						/* make sum of columns				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *sum,							/* returned sum vector			    */
  dtxy_t n)								/* max length of sum vector		    */
DX_SUFFIX

/*----in cmp.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_cmp(							/* compare two images			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image1,					/* first image to compare 			*/
  dtimage_t *image2,					/* second image to compare 			*/
  double epsilon,						/* maximum allowable difference 	*/
  long *cmp,							/* returned sign of difference		*/
  dtxy_t *x,							/* column location of difference 	*/
  dtxy_t *y)							/* row location 					*/
DX_SUFFIX

/*----in gen1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_rect_islands(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int separation,
  int xsize,
  int ysize)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_checkerboard(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int xsize,
  int ysize)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_color256(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_grid1(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_grid2(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_distance(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,
  dtxy_t xc,
  dtxy_t yc)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_distance_squared(
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,
  dtxy_t xc,
  dtxy_t yc)
DX_SUFFIX

/*----in gen2.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_replicate(
  dt_ctl_t *ctl,
  dtimage_t *image,
  const dt_ntype_f2_t * const kernel,
  const dtxy_t kx,
  const dtxy_t ky)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_interlock16(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_rect_tree(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int maxdepth)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_rect_spiral(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground,
  int separation)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_xsine(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_ysine(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtparam_t *param)
DX_SUFFIX

/*----in gen3.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_test1(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_test2(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_test3(
  dt_ctl_t *ctl,
  dtimage_t *image)
DX_SUFFIX

/*----in gen_kern.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_gen_kernel(
  dt_ctl_t *ctl,
  dtimage_t *kernel,
  const char *kernel_name)
DX_SUFFIX

/*----in median.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_median(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const int kx,
  const int ky,
  dtimage_t *output)
DX_SUFFIX

/*----in morph.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_dilate3x3(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_erode3x3(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
DX_SUFFIX

/*----in stats.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_variance(						/* variance of pixel values			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *variance,
  double *mean)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_stats2(							/* statistics on images				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input1,					/* first input image 				*/
  dtimage_t *input2,					/* second input image 				*/
  double constant,						/* constant							*/
  dtstats_t *stats)						/* output statistics 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_stats(							/* compute blob statistics 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to compute stats on		*/
  dtstats_t *stats)					/* return statistics 				*/
DX_SUFFIX

/*----in range.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_range(							/* find range of image values	    */
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  double *minval,
  double *maxval)
DX_SUFFIX

/*----in scan.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_run_scan(						/* scan image for runs 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtrun_t *run,							/* initialized run structure 		*/
  dtxy_t xmin,							/* upper left column of rectangle	*/
  dtxy_t ymin,							/* upper left row 					*/
  dtxy_t xmax,							/* lower right column 				*/
  dtxy_t ymax,							/* lower right row 					*/
  double invisible,						/* invisible pixel value 			*/
  int flag)								/* how to treat invisible pixel		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_blob_stats(						/* compute blob statistics 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dtstats_t *stats)						/* return statistics 				*/
DX_SUFFIX

/*----in blob.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_blob_hist(						/* accumulate blob histogram		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image of gray level values 		*/
  dtrun_t *run,							/* run from which labels were made 	*/
  dtrun_blob_t *blob,					/* blob representation 				*/
  dthist_t *hist)						/* return histogram					*/
DX_SUFFIX

/*----in run_len.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrun_col(								/* find runs in xth column			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const dtxy_t x,
  dtrun_len_t *run,						/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrun_row(								/* find runs in yth row				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  const dtxy_t y,
  dtrun_len_t *run,						/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrun_cols_max(							/* find max run in each column		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrun_len_t *maxrun,					/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtrun_rows_max(							/* find max run in each row			*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtrun_len_t *maxrun,					/* returned run vector			    */
  dtxy_t n)								/* max length of run vector		    */
DX_SUFFIX

/*----in mean.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_mean(							/* mean of pixel values				*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *mean)
DX_SUFFIX

/*----in cumul.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_cumulative(						/* make cumulative sum				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtimage_t *output,					/* output image 					*/
  unsigned long assign_flags)			/* operation control flags		    */
DX_SUFFIX

/*----in sumsq.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_sumsq(							/* sum of squared pixel values		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double *psum)
DX_SUFFIX

/*----in sumsqd.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_sumsq_diffs(					/* sum of differences squared		*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  double *psum)
DX_SUFFIX

/*----in sumsqs.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_sumsq_scalar(					/* sum of differences squared		*/
  dt_ctl_t *ctl,
  dtimage_t *image,
  double scalar,
  double *psum)
DX_SUFFIX

/*----in resample.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_resample_bilinear(				/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  dtimage_t *output)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_resample_size(					/* return resampled image size		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  const char *op,
  dtxy_t *xe,							/* returned image width 			*/
  dtxy_t *ye)							/* returned image height			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_resample_scale(					/* return resampled image size		*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xe,							/* desired image width 				*/
  dtxy_t ye,							/* desired image height				*/
  const char *op,
  double *dx,							/* returned resample scale			*/
  double *dy)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_resample_fast(					/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  const char *op,
  dtimage_t *output,
  int *cando)							/* return if we did it fast 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_resample(						/* resample to output from input	*/
  dt_ctl_t *ctl,
  dtimage_t *input,
  double dx,							/* stepsize over input image	    */
  double dy,
  const char *op,
  dtimage_t *output)
DX_SUFFIX

/*----in rotate.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_rotate(
  dt_ctl_t *ctl,
  dtimage_t *input,
  double angle,							/* angle in degrees 				*/
  dtimage_t *output)
DX_SUFFIX

/*----in transpos.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_transpose(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtimage_t *output)
DX_SUFFIX

/*----in pixel.c:----*/


DX_PREFIX
void 
DTCONFIG_API1
DTIMAGE_SETPIX1(						/* set single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double value)							/* pixel value						*/
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
DTIMAGE_SETPIXS(						/* set single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x1,							/* first pixel column location		*/
  dtxy_t x2,							/* final pixel column location		*/
  dtxy_t y,								/* pixel row location 				*/
  double value)							/* pixel value						*/
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
DTIMAGE_SETPIXN(						/* set single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  dt_ntype_def ntype_def,				/* image data type 					*/
  double value)							/* pixel value						*/
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
DTIMAGE_GETPIX1(						/* get single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to get	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double *value)						/* returned pixel value				*/
DX_SUFFIX

DX_PREFIX
void 
DTCONFIG_API1
DTIMAGE_GETPIXN(						/* get single pixel to given value	*/
  dt_rc_check_f *check,					/* return code check macro 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to get	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  dt_ntype_def ntype_def,				/* image data type 					*/
  double *value)						/* returned pixel value				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_set_pixel(						/* set single pixel to given value	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to be set	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double value)							/* pixel value						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_get_pixel(						/* get pixel value					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing pixel to get	*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  double *value)						/* pixel value returned				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_sprintf_pixel(					/* convert pixel binary to ASCII 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image containing the pixel 		*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  char *string,							/* returned ASCII string 			*/
  int stringmax)						/* max length of string 			*/
DX_SUFFIX

/*----in hist1.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_hist_alloc(						/* allocate histogram for image		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  dthist_t *hist)						/* output histogram					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_hist(							/* make histogram from image		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  dthist_t *hist)						/* output histogram					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_hist_fast(						/* make histogram from image		*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* input image						*/
  dthist_t *hist,						/* output histogram					*/
  int *cando)
DX_SUFFIX

/*----in hist2.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_hist_plot_horiz(				/* plot histogram 					*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* output image						*/
  dthist_t *hist,						/* input histogram					*/
  dtxy_t y0,							/* baseline of histogram 			*/
  dtxy_t ye,							/* height of tallest bar 			*/
  double color)
DX_SUFFIX

/*----in plot1.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_pixel_func(				/* set pixel to value				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to draw line in		    */
  dtxy_t x,								/* current pixel location 			*/
  dtxy_t y,
  void *arg)							/* caller-defined value 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_line2(						/* draw line in image			  	*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw line in		    */
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2,
  dtimage_pixel_f *func,
  void *arg)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_line(						/* draw line in image			  	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image to draw line in		    */
  dtxy_t x1,							/* starting column 					*/
  dtxy_t y1,							/* starting row 					*/
  dtxy_t x2,							/* ending column 					*/
  dtxy_t y2,							/* ending row 						*/
  double color)							/* color to set pixels to 			*/
DX_SUFFIX

/*----in plot2.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_xbars_sint32(				/* bar plot, bars at each x			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  dtntype_s4_t *bars,					/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_ybars_sint32(				/* bar plot, bars at each y			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  dtntype_s4_t *bars,					/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_xbars_double(				/* bar plot, bars at each x			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  double *bars,							/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_ybars_double(				/* bar plot, bars at each y			*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw bars in		    */
  double *bars,							/* array of bars				    */
  dtxy_t n,								/* number of bars				    */
  dtxy_t barzero,						/* where baseline of bar goes	    */
  dtxy_t barsize,						/* size of tallest bar			    */
  double color)
DX_SUFFIX

/*----in plot_txt.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtimage_plot_text_8x16(					/* draw text in image			  	*/
  dt_ctl_t *ctl,
  dtimage_t *image,						/* image to draw text in		    */
  const char *string,
  dtxy_t x0,							/* upper right of char cell		    */
  dtxy_t y0,
  double color)
DX_SUFFIX

/*----in adj.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_adj_blob(						/* extract blobs					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtxy_t xmin,							/* upper left column of rectangle	*/
  dtxy_t ymin,							/* upper left row 					*/
  dtxy_t xmax,							/* lower right column 				*/
  dtxy_t ymax,							/* lower right row 					*/
  double invisible,						/* value of invisible pixels 		*/
  unsigned long flags,					/* process control 					*/
  dtrun_deliver_f *deliver,				/* function to call for each blob 	*/
  void *arg)							/* user argument to pass to deliver */
DX_SUFFIX

/*----in adj_run.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtimage_adj_run_blob(					/* extract blobs using runs			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtxy_t xmin,							/* upper left column of rectangle	*/
  dtxy_t ymin,							/* upper left row 					*/
  dtxy_t xmax,							/* lower right column 				*/
  dtxy_t ymax,							/* lower right row 					*/
  double invisible,						/* value of invisible pixels 		*/
  unsigned long flags,					/* process control 					*/
  dtrun_deliver_f *deliver,				/* function to call for each blob 	*/
  void *arg)							/* user argument to pass to deliver */
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
