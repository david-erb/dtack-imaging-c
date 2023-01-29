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

/*----in font.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtfont_fixed_char(						/* draw char in memory			  	*/
  dt_ctl_t *ctl,
  char c,
  void *memalloc,
  dt_ntype_e ntype,
  dtxy_t x,								/* upper right of char cell		    */
  dtxy_t y,
  double color,
  dtfont_fixed_t *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtfont_fixed_string(
  dt_ctl_t *ctl,
  const char *string,
  void *memalloc,
  dt_ntype_e ntype,
  dtxy_t x0,							/* upper right of first char cell   */
  dtxy_t y0,
  double color,
  dtfont_fixed_t *desc)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtfont_8x16_string(						/* draw text in in memory			*/
  dt_ctl_t *ctl,
  const char *string,
  void *memalloc,
  dt_ntype_e ntype,
  dtxy_t x0,							/* upper right of char cell		    */
  dtxy_t y0,
  double color)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
