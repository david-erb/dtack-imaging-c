/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tlist.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tlist_HEADER_
#define _DC3tlist_HEADER_

#include <iostream.h>

#include "glist.h"
#include "tattr.h"
#include "tddict.h"
#include "tddictc.h"

// In order for the sorted list template to know how to sort one needs
// to define a type that can order itself, hence this ...

class DC3AttributePointer {
	DC3Attribute *Pointer;
public:
	DC3AttributePointer(void)		{ Pointer=0; }
	DC3AttributePointer(DC3Attribute *p)	{ Pointer=p; }

	DC3AttributePointer(const DC3AttributePointer& t)
		{ Pointer=t.Pointer; }
	void operator=(const DC3AttributePointer& t)
		{ Pointer=t.Pointer; }

	int operator==(const DC3AttributePointer& t2)
		{ return Pointer->GetTag() == t2.Pointer->GetTag(); }
	int operator<(const DC3AttributePointer& t2)
		{ return Pointer->GetTag() <  t2.Pointer->GetTag(); }
	int operator<=(const DC3AttributePointer& t2)
		{ return Pointer->GetTag() <= t2.Pointer->GetTag(); }
	int operator>(const DC3AttributePointer& t2)
		{ return Pointer->GetTag() >  t2.Pointer->GetTag(); }
	int operator>=(const DC3AttributePointer& t2)
		{ return Pointer->GetTag() >= t2.Pointer->GetTag(); }

	DC3Attribute *	GetPointer()	{ return Pointer; }
};	

class DC3ListofAttributes : public GENSortedList<DC3AttributePointer> {
	DC3Attribute *getSingleValuedAttribute(DC3Tag tag);
	DC3Attribute *getSingleValuedAttribute(char *keyword);
public:
	int	insert(DC3Attribute *p)
		{
			DC3AttributePointer ap(p);
			return GENSortedList<DC3AttributePointer>::insert(ap);
		}

	DC3Attribute *	value(void)
		{
			return GENSortedList<DC3AttributePointer>::
				value().GetPointer();
		}

	DC3Attribute *	find(DC3Tag tag)
		{
			DC3Attribute *p = 0;
			first();
			while (ismore()) {
				if (value()->GetTag() == tag) {
					p=value();
					break;
				}
				next();
			}
			return p;
		}

	void	RemoveMetaHeaderGroup(void);
	void	Fix(char *outsyntaxUID,int useMetaHeader=1);
	void	FixCommand(void);
	void	PrivateRemove(void);

	void	Delete(DC3ListofAttributes& deletelist);
	void	Replace(DC3ListofAttributes& replacelist);

	void	put(ostream& stream);
	void	write(DC3ostream& stream);

	int getSingleValueXint(DC3Tag tag,Xint& value,int verbose=0);
	int getSingleValueXint(char *keyword,Xint& value,int verbose=0);
	int getSingleValueUint16(DC3Tag tag,Uint16& value,int verbose=0);
	int getSingleValueUint16(char *keyword,Uint16& value,int verbose=0);
	int getSingleValueStringCopy(DC3Tag tag,char * &value,int verbose=0);
	int getSingleValueStringCopy(char *keyword,char * &value,int verbose=0);
};

#endif /* _DC3tlist_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
