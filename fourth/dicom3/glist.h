/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* glist.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _GENglist_HEADER_
#define _GENglist_HEADER_

struct GENSimpleListEntry {		// Should really be nested in
					// GENSimpleList, but CC-3.01 won't
					// allow nested classes in templates !
	void *item;	// Don't know type so have to use void pointer :(
	GENSimpleListEntry* next;
};

template<class T>
class GENSimpleList {
	GENSimpleListEntry *head;
	GENSimpleListEntry *ptr;
public:
	GENSimpleList(void)		{ head=0; }
	virtual ~GENSimpleList(void)
		{
			GENSimpleListEntry *e,*n;
			e=head;
			while (e) {
				n=e->next;
				if (e->item) delete e->item;
				delete e;
				e=n;
			}
		}

	int	insert(T tvalue)
		{
			GENSimpleListEntry *e = new GENSimpleListEntry;
			T *t = new T;
			if (e && t) {
				// Insert at head of list
				*t=tvalue;
				e->item=(void *)t;
				e->next=head;
				head=e;
				
				first();	// prevent disaster if calls
						//   intermingled (illegal)
				return 0;	// success
			}
			else {
				return 1;	// failure - no memory
			}
		}

	// Used as { first(); while (ismore()) { value(); next(); } }

	void	first(void)	{ ptr=head; }
	void	next(void)	{ if (ptr) ptr=ptr->next; }
	int	ismore(void)	{ return ptr != 0; }

	T	value(void)	{ return *(T *)(ptr->item); }
};

template<class T>
class GENOrderedList {
	GENSimpleListEntry *head;
	GENSimpleListEntry *tail;
	GENSimpleListEntry *ptr;
public:
	GENOrderedList(void)		{ head=0; tail=0; }
	virtual ~GENOrderedList(void)
		{
			GENSimpleListEntry *e,*n;
			e=head;
			while (e) {
				n=e->next;
				if (e->item) delete e->item;
				delete e;
				e=n;
			}
		}

	int	insert(T tvalue)
		{
			GENSimpleListEntry *e = new GENSimpleListEntry;
			T *t = new T;
			if (e && t) {
				*t=tvalue;
				e->item=(void *)t;
				// Insert at tail of list
				e->next=0;
				if (!head) head=e;	// 1st time
				if (tail)  tail->next=e;
				tail=e;
				
				first();	// prevent disaster if calls
						//   intermingled (illegal)
				return 0;	// success
			}
			else {
				return 1;	// failure - no memory
			}
		}

	// Used as { first(); while (ismore()) { value(); next(); } }

	void	first(void)	{ ptr=head; }
	void	next(void)	{ if (ptr) ptr=ptr->next; }
	int	ismore(void)	{ return ptr != 0; }

	T	value(void)	{ return *(T *)(ptr->item); }
};

template<class T>
class GENSortedList {
	GENSimpleListEntry *head;
	GENSimpleListEntry *ptr;
public:
	GENSortedList(void)		{ head=0; }
	virtual ~GENSortedList(void)
		{
			GENSimpleListEntry *e,*n;
			e=head;
			while (e) {
				n=e->next;
				if (e->item) delete e->item;
				delete e;
				e=n;
			}
		}

	int	insert(T tvalue)
		{
			GENSimpleListEntry *e = new GENSimpleListEntry;
			T *t = new T;
			if (e && t) {
				*t=tvalue;
				e->item=(void *)t;
				// Insert into list in ascending order
				GENSimpleListEntry *test,*last;
				test=head;
				last=0;
				while (test && *(T *)(test->item) <= tvalue) {
					last=test;
					test=test->next;
				}
				e->next=test;
				if (last) last->next=e;
				else head=e;

				first();	// prevent disaster if calls
						//   intermingled (illegal)
				return 0;	// success
			}
			else {
				return 1;	// failure - no memory
			}
		}

	void	remove(T tvalue)		// Removes ALL matches
		{
			GENSimpleListEntry *test,*last;
			test=head;
			last=0;
			while (test) {
				if (*(T *)(test->item) == tvalue) {
					if (last) last->next=test->next;
					else head=test->next;
					last=test;
					test=test->next;
					if (last->item) delete last->item;
					delete last;
					first();	// prevent disaster if
							// calls intermingled
				}
				else {
					last=test;
					test=test->next;
				}
			}
		}

	// Used as { first(); while (ismore()) { value(); next(); } }

	// MUST NOT BE USED TOGETHER WITH insert() or remove()

	void	first(void)	{ ptr=head; }
	void	next(void)	{ if (ptr) ptr=ptr->next; }
	int	ismore(void)	{ return ptr != 0; }

	T	value(void)	{ return *(T *)(ptr->item); }
};

#endif /* _GENglist_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
