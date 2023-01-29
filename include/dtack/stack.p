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

/*----in stack.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_init2(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  unsigned int elsize,
  long initial_depth,
  long maximum_depth,
  unsigned int pagesize,
  unsigned int npages,
  char *F)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_init(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  unsigned int elsize,
  long initial_depth,
  long maximum_depth,
  char *F)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_prealloc(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  unsigned int elsize,
  void *vmemalloc,
  long maximum_depth,
  char *F)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_push(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  void **el,
  dtstack_code_e *code)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_pop(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  void **el,
  dtstack_code_e *code)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_peek(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_poke(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_peek1(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_poke1(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long pos,								/* stack position id			    */
  void **el)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_free(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  char *F)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_set(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  dtstack_set_flag_e flag,
  long value)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstack_print(
  dt_ctl_t *ctl,
  dtstack_t *stack,
  char *F)
DX_SUFFIX

/*----in stack_io.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstack_write(							/* write stack to file				*/
  dt_ctl_t *ctl,
  dtstack_t *stack,
  dtfd_t *fd)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstack_read(							/* read stack from file				*/
  dt_ctl_t *ctl,
  dtstack_t *stack,
  long maximum_depth,					/* depth limit for this read	    */
  dtfd_t *fd,
  dtstack_code_e *code,
  int *peof)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
