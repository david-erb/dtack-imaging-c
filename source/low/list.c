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
  20000104 DE init after malloc and realloc (purify)
  19990810 DE added sort function
  19990803 DE added nth function
  19980525 DE print elem name as it is freed
  19980724 DE fixed bug in after() when expanding list
 *..........................................................................*/



/*..........................................................................
| TITLE
| The :list: Function Group
| 
|!Summary
| The :dtlist: functions provide a convenient means of
| holding any type of object in a doubly linked list.
|
| The :dtlist_t: data structure used by all these functions
| contains no public members.
|
| Each object in a list is associated with a :dtlist_elem_t: structure.
| This small structure has the public member ~elem->name~,
| which is the search key.
| The element structure also has the public member ~elem->user~ which is
| a void pointer to the element's user-supplied data.
| The caller must provide the space for both the name and the data.
|
| Two special values may be used in place of an element structure pointer.
| These are :DTLIST_ELEM_HEAD: and :DTLIST_ELEM_TAIL:.
| They refer to the head and tail of the list, respectively.
|
| This Function Group contains these functions:
| dtlist_create()		create new list			   
| dtlist_resize()		create new list			   
| dtlist_count()		number of elements in list 
| dtlist_nth()			return nth element from list
| dtlist_delete()		delete elem from list	   
| dtlist_after()		add element after given elem
| dtlist_front()		bring element to front
| dtlist_back()			put element at end of list 
| dtlist_search()		see if element is in list  
| dtlist_find()			find element in list		   
| dtlist_default_s()	see if element is in list  
| dtlist_default_l()	see if element is in list  
| dtlist_next()			get next element			   
| dtlist_prev()			get prev element			   
| dtlist_append()		append one list to other	   
| dtlist_free()			free all element names and 
| dtlist_destroy()		free list structure		   
| dtlist_fwrite()		write all elements of list 
| dtlist_fread()		read all elements of list  
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: list.c,v $ $Revision: 1.1 $");
#include <dtack/list.h>
#include <dtack/str.h>
#include <dtack/os.h>

/*..........................................................................
| NAME
| dtlist_create() - create new list
| 
| SUMMARY
| Create and initialize an empty list.
| Allocate space for a small number of elements.
| The caller should invoke dtlist_destroy() to free this space.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_create(							/* create new list				    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *listname)					/* name of list for debug 			*/
{
  DT_F("dtlist_create");
  dtlist_elem_t *elem;
  DT_Q(dtos_malloc2,(ctl, 
    (void **)&elem, 
    DTLIST_INITIAL_MAX * sizeof(*elem),
    F, listname));
  memset(elem, 0,						/* init to keep purify happy        */
	DTLIST_INITIAL_MAX * sizeof(*elem));
  list->name = listname;
  list->m = DTLIST_INITIAL_MAX;
  list->n = 0;
  list->elem = elem;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_resize() - change max number of elems
| 
| SUMMARY
| Change the maximum number of elems allowed in ~list~ by ~factor~.
| Have an error if ~factor~ is less than or equal to 0.
| Have an error if elements would be lost by shrinking the list.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_resize(							/* create new list				    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  double factor)						/* list size change factor 			*/
{
  DT_F("dtlist_resize");
  unsigned int m;
  dtlist_elem_t *elem;

  if (factor <= 0.0)					/* check factor validity 			*/
    return dt_err(ctl, F,
      "invalid factor %g",
      factor);
	  
  m = (unsigned int)(list->m * factor);	/* new size 						*/
  
  if (m < list->n)						/* shrinking list would lose elems? */
    return dt_err(ctl, F,
      "new size %u for %s"
      " too small for population %u",
      m, list->name, list->n);

  DT_Q(dtos_realloc3,(ctl,				/* attempt to resize 				*/
    list->elem, (void **)&elem, 
    m * sizeof(*elem),
    F, list->name));

  if (elem == NULL)
    return dt_err(ctl, F,
      "%s realloc failed",
      list->name);

  memset(elem+list->n, 0,				/* init to keep purify happy        */
	(m-list->n) * sizeof(*elem));

  list->m = m;
  list->elem = elem;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_count() - return number of elements in the list
| 
| SUMMARY
| Return the number of elements currently in the list.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_count(							/* number of elements in list		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  unsigned int *count)					/* returned number of elements 		*/
{
  *count = list->n;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_count() - return nth element in the list
| 
| SUMMARY
| Return the nth element currently in ~list~.
| Return :NULL: if ~list~ contains fewer than ~nth~ elements.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_nth(								/* get nth element					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  unsigned int nth,						/* element wanted 					*/
  dtlist_elem_t **elem)					/* returned element 				*/
{
  if (nth >= list->n)
	*elem = NULL;
  else
	*elem = &list->elem[nth];

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtlist_ind(								/* get index of given elem		    */
  dt_ctl_t *ctl,
  dtlist_t *list,
  dtlist_elem_t *elem,
  unsigned int *nth)
{
  DT_F("dtlist_ind");
  if (elem < &list->elem[0] ||
      elem > &list->elem[list->n-1])
    return dt_err(ctl, F,
      "elem \"%s\" not in %s address range",
      elem->name, list->name);
  *nth = (unsigned int)(elem - list->elem);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_delete() - delete element from list
| 
| SUMMARY
| Delete the element whose address is given.
| The members ~elem->name~ and ~elem->user~ are overwritten,
| however, any space they may point to is not freed.
| Return an error if the element is not in the list
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_delete(							/* delete elem from list		    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem)					/* element to delete 				*/
{
  DT_F("dtlist_delete");
  unsigned int i, nth;
  if (list->n == 0)
    return dt_err(ctl, F,
      "%s already empty",
      list->name);
  DT_Q(dtlist_ind,(ctl, list, elem,		/* find index of element		    */
    &nth));
  for (i=nth; i<list->n-1; i++)
    list->elem[i] = list->elem[i+1];
  list->n--;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_after() - add new element after given element
| 
| SUMMARY
| Add the new element after the given element.
| Attach the given ~name~ and ~user~ to the new element.
| Do not check for duplicate names.
| Return an error if the given element is not in the list.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_after(							/* add element after given element  */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element to add after				*/
  const char *name,						/* name of the new element 			*/
  void *user,							/* user data for the new element 	*/
  dtlist_elem_t **newelem)				/* returned new element pointer 	*/
{
  DT_F("dtlist_after");
  unsigned int i, nth;
  
  if (elem != DTLIST_ELEM_HEAD &&		/* not adding at head or tail? */
	  elem != DTLIST_ELEM_TAIL)
    DT_Q(dtlist_ind,(ctl, list, elem,	/* find index of element		    */
      &nth));

  if (list->n == list->m)				/* list already full? 				*/
    DT_Q(dtlist_resize,(ctl, list,		/* grow the list 					*/
      1.5));							/* slow exponential growth factor 	*/
  
  if (elem == DTLIST_ELEM_HEAD)
  {
    for (i=list->n; i>0; i--)
      list->elem[i] = list->elem[i-1];
    nth = 0;
  }
  else
  if (elem == DTLIST_ELEM_TAIL)
    nth = list->n;
  else
  {
    for (i=list->n; i>nth+1; i--)
      list->elem[i] = list->elem[i-1];
	nth++;
  }
  list->elem[nth].name = name;
  list->elem[nth].user = user;
  if (newelem != NULL)
    *newelem = &list->elem[nth];
  list->n++;
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

#ifdef NONO

static
dt_rc_e
dtlist_exchange(						/* exchange element with one after  */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem)
{
  DT_F("dtlist_exchange");
  dtlist_elem_t tmp;
  unsigned int nth;
  if (elem == DTLIST_ELEM_HEAD)
    return dt_err(ctl, F,
      "can't exchange head in %s",
      list->name);
  else
  if (elem == DTLIST_ELEM_TAIL)
    return dt_err(ctl, F,
      "can't exchange tail in %s",
      list->name);
  else
    DT_Q(dtlist_ind,(ctl, list, elem,	/* find index of element		    */
      &nth));
  if (nth == list->n-1)
    return dt_err(ctl, F,
      "can't exchange last element in %s",
      list->name);
  tmp = list->elem[nth];
  list->elem[nth] = list->elem[nth+1];
  list->elem[nth+1] = tmp;
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
| NAME
| dtlist_front() - bring element to front of list
| 
| SUMMARY
| Bring the given element to the front (beginning) of the list.
| Return its new element address.
| Return an error if the given element is not in the list.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_front(							/* bring element to front of list	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element to bring to front 		*/
  dtlist_elem_t **newelem)				/* element in its new position 		*/
{
  DT_F("dtlist_front");
  dtlist_elem_t tmp = *elem;
  DT_Q(dtlist_delete,(ctl, list,
    elem));
  DT_Q(dtlist_after,(ctl, list,
    DTLIST_ELEM_HEAD,
    tmp.name, tmp.user, newelem));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_back() - bring element to back of list
| 
| SUMMARY
| Bring the given element to the back (end) of the list.
| Return its new element address.
| Return an error if the given element is not in the list.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_back(							/* put element at end of list		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element to bring to back			*/
  dtlist_elem_t **newelem)				/* element in its new position 		*/
{
  DT_F("dtlist_back");
  dtlist_elem_t tmp = *elem;
  DT_Q(dtlist_delete,(ctl, list,
    elem));
  DT_Q(dtlist_after,(ctl, list,
    DTLIST_ELEM_TAIL,
    tmp.name, tmp.user, newelem));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_search() - search for element in list
| 
| SUMMARY
| Attempt to find the element with the given name.
| If the name is in the list multiple times, 
| return the one closest to the front of the list.
| Return no error if not found,
| however return a NULL pointer.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_search(							/* see if element is in list	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  dtlist_elem_t **elem)					/* returned element pointer 		*/
{
  unsigned int i;
  for (i=0; i<list->n; i++)				/* search list linearly			    */
    if (!strcmp(name, 
          list->elem[i].name))
      break;
  if (i == list->n)
    *elem = NULL;
  else
    *elem = &list->elem[i];
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_find() - find element in list
| 
| SUMMARY
| Return the element with the given name.
| If the name is in the list multiple times, 
| return the one closest to the front of the list.
| Return an error if no element with that name exists.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_find(							/* find element in list			    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  dtlist_elem_t **elem)					/* returned element pointer 		*/
{
  DT_F("dtlist_find");
  DT_Q(dtlist_search,(ctl, list,
    name, elem));						/* see if it is in the list		    */
  if (*elem == NULL)					/* not in the list?				    */
    return dt_err(ctl, F,
      "could not find \"%s\" in %s",
      name? name: "NULL",
      list->name);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_default_s() - search for element data as string
| 
| SUMMARY
| Attempt to find the element with the given name.
| If the name is in the list multiple times, 
| return the one closest to the front of the list.
| Return ~default~ if not found, otherwise return element's data.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_default_s(						/* see if element is in list	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  const char *dflt,						/* default string value 			*/
  const char **value)					/* returned element's value */
{
  DT_F("dtlist_default_s");
  dtlist_elem_t *elem;
  DT_Q(dtlist_search,(ctl, list, name,	/* search for name 					*/
    &elem));
  if (elem != NULL)						/* name found? 						*/
    *value = (const char *)elem->user;
  else
    *value = dflt;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_default_s() - search for element data as ASCII integer
| 
| SUMMARY
| Attempt to find the element with the given name.
| If the name is in the list multiple times, 
| return the one closest to the front of the list.
| Return ~default~ if not found, otherwise return element's data
| translated to a long value.
| Have an error if the value cannot be translated.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_default_l(						/* see if element is in list	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  const char *name,						/* name of element to search for 	*/
  long dflt,							/* default long value 				*/
  long *value)							/* returned element's value 		*/
{
  DT_F("dtlist_default_l");
  dtlist_elem_t *elem;
  DT_Q(dtlist_search,(ctl, list, name,	/* search for name 					*/
    &elem));
  if (elem != NULL)						/* name found? 						*/
	DT_Q(dtstr_to_long,(ctl,
      (const char *)elem->user,
      value))
  else
    *value = dflt;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_next() - get next element
| 
| SUMMARY
| Return the element following the given element.
| If there is no following element, return NULL.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_next(							/* get next element					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element whose successor to get 	*/
  dtlist_elem_t **next)					/* returned element pointer 		*/
{
  DT_F("dtlist_next");
  unsigned int nth;
  if (elem == NULL)
    *next = NULL;
  else
  if (list->n == 0)
    *next = NULL;
  else
  if (elem == DTLIST_ELEM_HEAD)
    *next = &list->elem[0];
  else
  if (elem == DTLIST_ELEM_TAIL)
    *next = NULL;
  else
  {
    DT_Q(dtlist_ind,(ctl, list, elem,	/* find index of element		    */
      &nth));
    if (nth == list->n-1)
      *next = NULL;
    else
      *next = &list->elem[nth+1];
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_prev() - get previous element
| 
| SUMMARY
| Return the element preceding the given element.
| If there is no preceding element, return NULL.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_prev(							/* get prev element					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  dtlist_elem_t *elem,					/* element whose predecessor to get */
  dtlist_elem_t **prev)					/* returned element pointer 		*/
{
  DT_F("dtlist_prev");
  unsigned int nth;
  if (elem == NULL)
    *prev = NULL;
  else
  if (list->n == 0)
    *prev = NULL;
  else
  if (elem == DTLIST_ELEM_HEAD)
    *prev = NULL;
  else
  if (elem == DTLIST_ELEM_TAIL)
    *prev = &list->elem[list->n-1];
  else
  {
    DT_Q(dtlist_ind,(ctl, list, elem,	/* find index of element		    */
      &nth));
    if (nth == 0)
      *prev = NULL;
    else
      *prev = &list->elem[nth-1];
  }
  return DT_RC_GOOD;
}


/*..........................................................................
| NAME
| dtlist_append() - append one list at end of other
| 
| SUMMARY
| Append all elements of ~list1~ to the end of ~list2~.
| Allocate new name and user data with ~tos_malloc().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_append(							/* append one list to other			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list1,						/* source list object 				*/
  dtlist_t *list2,						/* dest list object 				*/
  unsigned int size)					/* size of user data 				*/
{
  DT_F("dtlist_append");
  unsigned int i;

  if (list1->m == 0)					/* list not created? 				*/
    return DT_RC_GOOD;
  if (list2->m == 0)					/* list not created? 				*/
    return DT_RC_GOOD;
	
  for (i=0; i<list1->n; i++)
  {
	const char *name1 = list1->elem[i].name;
	void *user1 = list1->elem[i].user;
	char *name2;
	void *user2;

	if (name1 != NULL)
	  DT_Q(dtos_strdup,(ctl, name1,
        &name2))
	else
	  name2 = NULL;

	if (user1 != NULL)
	{
	  DT_Q(dtos_malloc2,(ctl, &user2,
        size, F, "user data"));
	  DT_MEMCPY(user2, user1, size);
	}
	else
	  user2 = NULL;
	
	DT_Q(dtlist_after,(ctl,
      list2, DTLIST_ELEM_TAIL,
      name2, user2, NULL));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_free() - free all element names and user data
| 
| SUMMARY
| Presume that all element
| names and user data have been allocated by dtos_malloc().
| For all elements in the list, call dtos_free() on ~elem->name~ and
| ~elem->user~.
| Also make the list empty.
| Please note that this function frees space which has not been
| allocated by this Function Group.
| It is a convenience function only.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_free(							/* free all element names and data	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list)						/* list object 						*/
{
  DT_F("dtlist_free");
  unsigned int i;

  if (list->m == 0)						/* list not created? 				*/
    return DT_RC_GOOD;

  for (i=0; i<list->n; i++)
  {
	if (list->elem[i].name != NULL)
      DT_Q(dtos_free2,(ctl, (char *)list->elem[i].name, 
        F, (char *)list->elem[i].name));
	if (list->elem[i].user != NULL)
      DT_Q(dtos_free2,(ctl, list->elem[i].user, 
        F, "elem->user"));
  }

  list->n = 0;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_destroy() - destroy list structure
| 
| SUMMARY
| Free space allocated for the list structure.
| This is the dual of dtlist_create().
| Do not free space pointed to by any ~elem->name~ or ~elem->user~.
| Make the list structure itself invalid.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_destroy(							/* free list structure				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list)						/* list object 						*/
{
  DT_F("dtlist_destroy");

  if (list->m == 0)						/* list not created? 				*/
    return DT_RC_GOOD;

  DT_Q(dtos_free2,(ctl, 
    list->elem, 
    F, list->name));

  DT_MEMSET(list, 0, sizeof(*list));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtlist_sort() - sort list
| 
| SUMMARY
| Sort ~list~ using given ~compare~ function.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlist_sort(							/* free list structure				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtlist_t *list,						/* list object 						*/
  int ascending,						/* sort order flag 					*/
  dt1d_compare2_f *compare,				/* comparison function 				*/
  void *arg)							/* passthrough arg to comparison 	*/
{
  DT_F("dtlist_sort");

  DT_Q_1D(dt1d_sort_struct2,(			/* call low level sort function 	*/
    list->elem,
    sizeof(*list->elem),  
    list->n,
    compare, arg, ascending));
    
  return DT_RC_GOOD;
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
