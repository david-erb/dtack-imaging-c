/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* prescntx.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */
#ifndef _prescntx_HEADER_
#define _prescntx_HEADER_

#ifdef TEST
#include <iostream.h>
#endif /* TEST */

//#include "glist.h"

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
				if (e->item) delete (T *)e->item;
				delete e;
				e=n;
			}
		}

	int	insert(T tvalue)	// Insert copy of tvalue
					// copy will be deleted when list is
					// original will not
		{
			T *t = new T;
			if (t) *t=tvalue;
			return insert(t);
		}

	int	insert(T *tptr)		// Insert without copy
					// *tptr will be deleted when list is
		{
			GENSimpleListEntry *e = new GENSimpleListEntry;
			if (e && tptr) {
				// Insert at head of list
				e->item=(void *)tptr;
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

class PresentationContextEntry {
public:
	unsigned char	PresentationContextID;
	char *		AbstractSyntax;
	char *		TransferSyntax;

	PresentationContextEntry(unsigned char pcid,char *as,char *ts)
		{
			PresentationContextID=pcid;
			AbstractSyntax=as;		// Does NOT copy string
			TransferSyntax=ts;
		}
	// No need for destructor ... nothing saved
};

class PresentationContextEntryPointer {
	PresentationContextEntry *Pointer;
public:
	PresentationContextEntryPointer(void)
		{
			Pointer=0;
		}
	PresentationContextEntryPointer(PresentationContextEntry *p)
		{
			Pointer=p;
		}
	~PresentationContextEntryPointer(void)
		{
			if (Pointer) delete Pointer;
		}
	PresentationContextEntry *GetPointer()	{ return Pointer; }
};

class PresentationContextList
	: protected GENSimpleList<PresentationContextEntryPointer>
{
	PresentationContextEntry *value()
		{
			return GENSimpleList<PresentationContextEntryPointer>::
				value().GetPointer();
		}
	PresentationContextEntry *findbyID(unsigned char pcid)
		{
			PresentationContextEntry *e = 0;
			first();
			while (ismore()) {
				if (value()
				 && value()->PresentationContextID == pcid) {
					e=value();
					break;
				}
				next();
			}
			return e;
		}
	PresentationContextEntry *findbyAbstractSyntax(char *as)
		{
			PresentationContextEntry *e = 0;
			first();
			while (ismore()) {
				if (value() && value()->AbstractSyntax && as
				 && strcmp(value()->AbstractSyntax,as) == 0) {
					e=value();
					break;
				}
				next();
			}
			return e;
		}
public:
	int Add(unsigned char pcid,char *as,char *ts)
		{
			PresentationContextEntry *p = new
				PresentationContextEntry(pcid,as,ts);
			PresentationContextEntryPointer *pcp = new
				PresentationContextEntryPointer(p);
			return GENSimpleList<PresentationContextEntryPointer>::
				insert(pcp);
			// both the entry & pointer will be deleted when
			// the list is
		}

	char * GetAbstractSyntaxForPresentationContextID(unsigned char pcid)
		{
			PresentationContextEntry *p = 0;
			char *s = 0;
			if ((p=findbyID(pcid))) s=p->AbstractSyntax;
			return s;
		}
	char * GetTransferSyntaxForPresentationContextID(unsigned char pcid)
		{
			PresentationContextEntry *p = 0;
			char *s = 0;
			if ((p=findbyID(pcid))) s=p->TransferSyntax;
			return s;
		}

	void SetAbstractSyntaxForPresentationContextID(
			unsigned char pcid,char *as)
		{
			PresentationContextEntry *p = 0;
			if ((p=findbyID(pcid))) p->AbstractSyntax=as;
		}
	void SetTransferSyntaxForPresentationContextID(
			unsigned char pcid,char *ts)
		{
			PresentationContextEntry *p = 0;
			if ((p=findbyID(pcid))) p->TransferSyntax=ts;
		}

	int GetPresentationContextIDForAbstractSyntax(char *as)
		{
			PresentationContextEntry *p = 0;
			int pcid=-1;
			if ((p=findbyAbstractSyntax(as)))
				pcid=p->PresentationContextID;
			return pcid;
		}
	char * GetTransferSyntaxForAbstractSyntax(char *as)
		{
			PresentationContextEntry *p = 0;
			char *ts = 0;
			if ((p=findbyAbstractSyntax(as)))
				ts=p->TransferSyntax;
			return ts;
		}
#ifdef TEST
	void Put(ostream& stream)
		{
			first();
			while (ismore()) {
				if (value()) {
					stream 
					  << (int)value()->PresentationContextID
					  << " <" << value()->AbstractSyntax 
					  << "> <" << value()->TransferSyntax 
					  << ">\n" << flush;
				}
				else {
					stream << "No value\n" << flush;
				}
				next();
			}
		}
#endif /* TEST */
};

#ifdef TEST
int
main(int argc,char **argv)
{

	PresentationContextList list;
	list.Add(1,"AS One",	"TS One");
	list.Add(2,0,		"TS Two");
	list.Add(3,"AS Three",	0);

	list.Put(cerr);
	cerr << "\n" << flush;

	unsigned pcid;
	for (pcid=1; pcid<=4; ++pcid) {
		char *as=list.GetAbstractSyntaxForPresentationContextID(pcid);
		char *ts=list.GetTransferSyntaxForPresentationContextID(pcid);
		cerr << pcid << " <" << as << "> <" << ts << ">\n" << flush;
	}
	cerr << "\n" << flush;

	list.SetAbstractSyntaxForPresentationContextID(2,"New AS Two");
	list.SetTransferSyntaxForPresentationContextID(3,"New TS Three");

	list.Put(cerr);
	cerr << "\n" << flush;

	for (pcid=1; pcid<=4; ++pcid) {
		char *as=list.GetAbstractSyntaxForPresentationContextID(pcid);
		char *ts=list.GetTransferSyntaxForPresentationContextID(pcid);
		cerr << pcid << " <" << as << "> <" << ts << ">\n" << flush;
	}
	cerr << "\n" << flush;

	{
	char *as = "New AS Two";
	cerr << "For AS <" << as 
	     << "> pcid is "
	     << list.GetPresentationContextIDForAbstractSyntax(as)
	     << " and TS is <" << list.GetTransferSyntaxForAbstractSyntax(as)
	     << ">\n" << flush;
	cerr << "\n" << flush;
	}

	{
	char *as = "Not a real AS";
	cerr << "For AS <" << as 
	     << "> pcid is "
	     << list.GetPresentationContextIDForAbstractSyntax(as)
	     << " and TS is <" << list.GetTransferSyntaxForAbstractSyntax(as)
	     << ">\n" << flush;
	cerr << "\n" << flush;
	}

}
#endif /* TEST */

#endif /* _prescntx_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
