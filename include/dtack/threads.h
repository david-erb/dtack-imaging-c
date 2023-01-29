/*..........................................................................
| NAME
| :dtthreads: Function Group - thread manager object
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000507 DE added to the library
 *..........................................................................*/

typedef struct {
  dtlist_t list;
  int created;
  const char *listname;
  long nextid;							/* next available thread id number 	*/
} dtthreads_t;

#include <dtack/threads.p>
