/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tcondt.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */
#ifndef _DC3tcondt_HEADER_
#define _DC3tcondt_HEADER_

#include <string.h>

#include "tlist.h"
#include "tabstc.h"

inline int ElementPresent(DC3ListofAttributes *list,DC3Tag tag)
{
	return list->find(tag) ? 1 : 0;
}

inline int ElementPresentMasked(DC3ListofAttributes *list,DC3Tag tag,Uint16 mask)
{
	Uint16 g = tag.getGroup() & mask;
	Uint16 e = tag.getElement();
	list->first();
	while (list->ismore()) {
		DC3Tag t = list->value()->GetTag();
		if ((t.getGroup() & mask) == g && t.getElement() == e) return 1;
		list->next();
	}
	return 0;
}

inline int GroupPresent(DC3ListofAttributes *list,DC3Tag tag)
{
	Uint16 g = tag.getGroup();
	list->first();
	while (list->ismore()) {
		DC3Tag t = list->value()->GetTag();
		if (t.getGroup() == g) return 1;
		list->next();
	}
	return 0;
}

inline int GroupPresentMasked(DC3ListofAttributes *list,DC3Tag tag,Uint16 mask)
{
	Uint16 g = tag.getGroup() & mask;
	list->first();
	while (list->ismore()) {
		DC3Tag t = list->value()->GetTag();
		if ((t.getGroup() & mask) == g) return 1;
		list->next();
	}
	return 0;
}

inline int StringValueMatch(DC3ListofAttributes *list,DC3Tag tag,char *string)
{
	int match=0;
	DC3Attribute *attr=list->find(tag);
	if (attr && attr->GetValueMultiplicity() == 1 && attr->IsString()) {
		char *value;
		if (attr->SingleValueStringCopy(value) != -1) {
			if (value) {
				if (strcmp(value,string) == 0)
					match=1;
				delete value;
			}
		}
	}
	return match;
}

inline int BinaryValueGet(DC3ListofAttributes *list,DC3Tag tag,Xint& rvalue)
{
	int match=0;
	DC3Attribute *attr=list->find(tag);
	if (attr && attr->GetValueMultiplicity() == 1 && attr->IsNumeric()) {
		Xint value;
		if (attr->SingleValueNumeric(value) != -1) {
			rvalue=value;
			match=1;
		}
	}
	return match;
}

#endif /* _DC3tcondt_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
