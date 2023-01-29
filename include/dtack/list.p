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

/*----in list.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_create(							/* create new list				    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *listname)					/* name of list for debug 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_resize(							/* create new list				    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  double factor)						/* list size change factor 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_count(							/* number of elements in list		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  unsigned int *count)					/* returned number of elements 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_nth(								/* get nth element					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  unsigned int nth,						/* element wanted 					*/
  dtlist_elem_t **elem)					/* returned element 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_delete(							/* delete elem from list		    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem)					/* element to delete 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_after(							/* add element after given element  */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element to add after				*/
  const char *name,						/* name of the new element 			*/
  void *user,							/* user data for the new element 	*/
  dtlist_elem_t **newelem)				/* returned new element pointer 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_front(							/* bring element to front of list	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element to bring to front 		*/
  dtlist_elem_t **newelem)				/* element in its new position 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_back(							/* put element at end of list		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element to bring to back			*/
  dtlist_elem_t **newelem)				/* element in its new position 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_search(							/* see if element is in list	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  dtlist_elem_t **elem)					/* returned element pointer 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_find(							/* find element in list			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  dtlist_elem_t **elem)					/* returned element pointer 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_default_s(						/* see if element is in list	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  const char *dflt,						/* default string value 			*/
  const char **value)					/* returned element's value */
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_default_l(						/* see if element is in list	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  long dflt,							/* default long value 				*/
  long *value)							/* returned element's value 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_next(							/* get next element					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element whose successor to get 	*/
  dtlist_elem_t **next)					/* returned element pointer 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_prev(							/* get prev element					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element whose predecessor to get */
  dtlist_elem_t **prev)					/* returned element pointer 		*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_append(							/* append one list to other			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list1,						/* source list object 				*/
  dtlist_t *list2,						/* dest list object 				*/
  unsigned int size)					/* size of user data 				*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_free(							/* free all element names and data	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list)						/* list object 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_destroy(							/* free list structure				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list)						/* list object 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_sort(							/* free list structure				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  int ascending,						/* sort order flag 					*/
  dt1d_compare2_f *compare,				/* comparison function 				*/
  void *arg)							/* passthrough arg to comparison 	*/
DX_SUFFIX

/*----in list_io.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_fwrite(							/* write all elements of list		*/
  dt_ctl_t *ctl,
  dtlist_t *list,
  char *filename,
  unsigned int size)
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtlist_fread(							/* read all elements of list		*/
  dt_ctl_t *ctl,
  dtlist_t *list,
  char *filename,
  unsigned int *size)
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
