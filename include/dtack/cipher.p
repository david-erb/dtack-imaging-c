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

/*----in cipher.c:----*/


DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtcipher_init(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  const char *password1,
  const char *password2)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtcipher_uninit(
  dt_ctl_t *ctl,
  dtcipher_t *cipher)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtcipher_header(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  dtcipher_header_t *header)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtcipher_encrypt(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  char *in,
  int insize,
  char *out,
  int outsize)
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtcipher_decrypt(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  char *in,
  int insize,
  char *out,
  int outsize)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtcipher_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
