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

/*----in liner.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_fd_line(						/* get next line from file			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  char **line)							/* return line contents 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_line_iscomment(					/* see if line is a comment			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *line,						/* line to be examined 				*/
  const char *comment,					/* string of comment characters		*/
  int *iscomment)						/* returned comment indicator 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_line_split(						/* split line into key and value	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separator 			*/
  char **key,							/* returned key string 				*/
  char **value)							/* returned value string 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_line_list(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separators			*/
  dtlist_t *list)						/* list to add line to 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_fd_list(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_file_list(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *filename,					/* name of file to read 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_line_list2(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separators			*/
  dtlist_t *list)						/* list to add line to 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_fd_list2(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_file_list2(						/* make list from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *filename,					/* name of file to read 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  dtlist_t *list)						/* list to which lines are added 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_line_string(					/* make string from lines in file	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *line,							/* the line to be parsed 			*/
  const char *comment,					/* list of comment characters 		*/
  const char *separators,				/* the key:value separators			*/
  char **string)						/* string to be updated 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtliner_fd_string(						/* make string from lines in file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtfd_t *fd,							/* file object descriptor 			*/
  const char *comment,					/* string of comment characters		*/
  const char *separators,				/* the key:value separator 			*/
  char **string)						/* string to be appended 			*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
