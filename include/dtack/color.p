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

/*----in convert.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_convert_cant(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  dt_color_org_t *org1,
  dt_color_org_t *org2)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_convert_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  dt_color_org_t *org1,
  dt_color_org_t *org2)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_convert_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  dt_color_org_t *org1,
  dt_color_org_t *org2)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_convert(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  void *vb1,
  dt_color_org_t *org1,
  dtxy_t xe,
  dtxy_t ye,
  void *vb2,
  dt_color_org_t *org2)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_convert3(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,			/* conversion tables 				*/
  void *vb1a,
  void *vb1b,
  void *vb1c,
  dt_color_org_t *org1,
  dtxy_t xe,
  dtxy_t ye,
  void *vb2a,
  void *vb2b,
  void *vb2c,
  dt_color_org_t *org2)
DX_SUFFIX

/*----in org.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_org_parse(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  const char *string,
  char *message,
  int max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_org_set(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  dt_color_org_e org_e)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_org_string(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  char *string,
  int max)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_org_size(
  dt_ctl_t *ctl,
  dt_color_org_t *org,
  unsigned int npixels,
  unsigned int *nbytes)
DX_SUFFIX

/*----in rgb.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb565,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb565_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb565,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb555,				/* input in form Ycbycr				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb555_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb555,				/* input in form RGB				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
DX_SUFFIX

/*----in rgb888.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb888,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_pseudo_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_pseudo_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_rgb888_to_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *rgb888,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo)
DX_SUFFIX

/*----in r_g_b.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r_g_b,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_pseudo_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_pseudo_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to3_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_map_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_map_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to3_map(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_y_cb_cr_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to_y_cb_cr_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_r_g_b_to3_y_cb_cr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *y,
  unsigned char *cb,
  unsigned char *cr)
DX_SUFFIX

/*----in ycbycr.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,				/* input in form Ycbycr				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbcr_to_rgb_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbcr_to_rgb_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,					/* input in form Ycbycr				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)					/* output in form rgbrgb 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *y_cb_cr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_411_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_411_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_y_cb_cr_411(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *y_cb_cr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_ycbcr_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_ycbcr_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_ycbycr_to_ycbcr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *ycbycr,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *ycbcr)
DX_SUFFIX

/*----in y_cb_cr.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				/* input in form Y_Cb_Cr			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				/* input in form Y_Cb_Cr			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr_411,			/* input in form Y_Cb_Cr_411			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_ycbcr_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_ycbcr_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_411_to_ycbcr(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr_411,	
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *ycbcr)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_r_g_b_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_r_g_b_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_r_g_b(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				/* input in form Y_Cb_Cr			*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *r_g_b)				/* output in form rgbrgb 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to3_r_g_b(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *yp,
  unsigned char *cbp,
  unsigned char *crp,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *r,
  unsigned char *g,
  unsigned char *b)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_pseudo_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_pseudo_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y_cb_cr,				
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to3_pseudo(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y,
  unsigned char *cb,
  unsigned char *cr,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_map_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to_map_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_y_cb_cr_to3_map(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *y,
  unsigned char *cb,
  unsigned char *cr,
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *pseudo,
  unsigned char *lut)
DX_SUFFIX

/*----in yuyv.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_luminance_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_luminance_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_luminance(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *yuyv,					/* input in form YUYV				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *luminance)				/* output in form YY				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_rgb888_init(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_rgb888_uninit(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dt_color_yuyv_to_rgb888(
  dt_ctl_t *ctl,
  dt_color_convert_t *convert,
  unsigned char *yuyv,					/* input in form YUYV				*/
  dtxy_t xe,
  dtxy_t ye,
  unsigned char *rgb888)				/* output in form rgbrgb 			*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
