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

/*----in str.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_choice(							/* find string in list of strings 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **choices,					/* list of strings to search 		*/
  const char *string,					/* string to be found 				*/
  int *choice)							/* returned index of match 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_choice2(							/* find string in list of strings 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **choices,					/* list of strings to search 		*/
  const char *string,					/* string to be found 				*/
  int *choice,							/* returned index of match 			*/
  char *message,						/* returned error message 			*/
  int lmax)								/* input max error message length 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_to_double(						/* convert string to double			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  double *value)						/* returned converted value 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_to_double2(						/* try to convert string to double	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  double *value,						/* returned converted value 		*/
  int *isnumber)						/* conversion success indicator 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_to_long(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  long *value)							/* returned converted value 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_to_long2(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  long *value,							/* returned converted value 		*/
  int *isnumber)						/* conversion success indicator 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_to_ulong(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  unsigned long *value)							/* returned converted value 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_to_ulong2(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  unsigned long *value,							/* returned converted value 		*/
  int *isnumber)						/* conversion success indicator 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_tokenize(							/* separate string into tokens	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be separated 			*/
  char separator,						/* character separator 				*/
  int *ntokens)							/* returned number of tokens 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_find_white(						/* find next whitespace character 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **white)							/* returned pointer to next white 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_find_nonwhite(					/* find next non-whitespace char	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **nonwhite)						/* returned pointer to non-white 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_find_quote(						/* find next quote character 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **quote)							/* returned pointer to next quote 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e 
DTCONFIG_API1
dtstr_argv(								/* make argv from string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to be parsed 				*/
  char **argv,							/* returned array of token pointers	*/
  int maxargv,							/* maximum tokens to return 		*/
  int *argc)							/* returned number of tokens found 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_printf_va(						/* vprintf-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *format,					/* format string with %-signs 		*/
  va_list ap)							/* variable argument list 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API2
dtstr_printf(							/* printf-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *format,					/* format string with %-signs 		*/
  ...)									/* variable argument list 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_cpy(								/* strcpy-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *input)					/* input string						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_parsechar(
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* input string 					*/
  char *c,								/* output character					*/
  char *message,						/* return error message 			*/
  int message_max)						/* maximum length of error message 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_printchar(
  dt_ctl_t *ctl,						/* environment control 				*/
  char c,								/* input character					*/
  char *out,							/* output string 					*/
  int lmax)								/* maximum length of output string 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_printable(
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* input string 					*/
  int l,								/* length of input string 			*/
  char *out,							/* output string 					*/
  int lmax)								/* maximum length of output string 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_print_ntype(						/* give string for ntype			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const void *v,						/* input numeric value pointer		*/
  dt_ntype_e ntype,						/* precision of the numeric value 	*/
  char *out,							/* output string buffer				*/
  int lmax)								/* maximum length of output 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_print_op(							/* give string for op				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_op_e op,							/* op to make string for 			*/
  char *out,							/* output string buffer				*/
  int lmax)								/* maximum length of output 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_extract(							/* extract field					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to extract from 			*/
  const char *separator,				/* field separator string 			*/
  long nth,								/* field index wanted 				*/
  char *field,							/* where field returned 			*/
  int max)								/* max length of return string 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_htmlencode(						/* replace html special characters	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *input,					/* input string 					*/
  char *html,							/* html string to write to			*/
  int max)								/* maximum output length 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_cgiencode(						/* replace cgi special characters	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *input,					/* input string 					*/
  char *cgi,							/* cgi string to write to			*/
  int max)								/* maximum output length 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtstr_cgidecode(						/* replace cgi special characters	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *p)								/* input/output string 				*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
