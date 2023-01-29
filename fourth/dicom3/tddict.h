/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tddict.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tddict_HEADER_
#define _DC3tddict_HEADER_

#include <string.h>
#include <ctype.h>
#include "ttag.h"

// There is a lot of redundancy in these classes between the various types
// of dictionary and it would be nice to make a template or base class one
// day so long as it didn't increase the storage requirements of each
// dictionary too much.

// BTW. The global owner list just grows and grows and is never flushed
// or deleted ... will need attention in a server environment or with
// multiple datasets (as the owners may change from dataset to dataset)

const Uint16 VMUNLIMITED = (Uint16)-1;
const Uint16 VMNONE = 0;

struct DC3DataDictionaryEntry {
	Uint16		Group;
	Uint16		Element;
	char *		ValueRepresentation;
	Uint16		ValueMultiplicityMinimum;
	Uint16		ValueMultiplicityMaximum;
	char *		Keyword;
};

class DC3DataDictionary {
	struct DC3DataDictionaryEntry *Table;
public:
	DC3DataDictionary(struct DC3DataDictionaryEntry *t) { Table=t; }

	const DC3DataDictionaryEntry *FindEntry(DC3Tag Tag)
		{
			Uint16 mask = Tag.isRepeatingGroup() ? 0xf000 : 0xffff;
			DC3DataDictionaryEntry *ptr = Table;
			while (ptr && ptr->ValueRepresentation) {
				if ((Tag.getGroup() & mask) == ptr->Group
				 && Tag.getElement() == ptr->Element) 
					return ptr;
				++ptr;
			}
			return 0;
		}
	const DC3DataDictionaryEntry *FindEntry(char *keyword)
		{
			DC3DataDictionaryEntry *ptr = Table;
			while (ptr && ptr->ValueRepresentation) {
				if (strcmp(keyword,ptr->Keyword) == 0) 
					return ptr;
				++ptr;
			}
			return 0;
		}
	char *	GetValueRepresentation(DC3Tag Tag)
		{
			const DC3DataDictionaryEntry *ptr = FindEntry(Tag);
			if (ptr) return ptr->ValueRepresentation;
			else return 0;
		}
	Uint16	GetValueMultiplicityMinimum(DC3Tag Tag)
		{
			const DC3DataDictionaryEntry *ptr = FindEntry(Tag);
			if (ptr) return ptr->ValueMultiplicityMinimum;
			else return VMNONE;
		}
	Uint16	GetValueMultiplicityMaximum(DC3Tag Tag)
		{
			const DC3DataDictionaryEntry *ptr = FindEntry(Tag);
			if (ptr) return ptr->ValueMultiplicityMaximum;
			else return VMNONE;
		}
	char *	GetDescription(DC3Tag Tag)
		{
			const DC3DataDictionaryEntry *ptr = FindEntry(Tag);
		//	if (ptr) return ptr->Description;
			if (ptr) return ptr->Keyword;
			else return 0;
		}
	char *	GetKeyword(DC3Tag Tag)
		{
			const DC3DataDictionaryEntry *ptr = FindEntry(Tag);
			if (ptr) return ptr->Keyword;
			else return 0;
		}
	int	GetTag(char *keyword,DC3Tag& Tag)
		{
			const DC3DataDictionaryEntry *ptr = FindEntry(keyword);
			if (ptr) {
				DC3Tag foundtag(ptr->Group,ptr->Element);
				Tag=foundtag;
				return 0;
			}
			else return -1;
		}
};

extern class DC3DataDictionary GlobalDataDictionary;

struct DC3PrivateElementDictionaryEntry {
	Uint16		Element;
	char *		ValueRepresentation;
	Uint16		ValueMultiplicityMinimum;
	Uint16		ValueMultiplicityMaximum;
	char *		Keyword;
};

class DC3PrivateElementDictionary {
	struct DC3PrivateElementDictionaryEntry *Table;
public:
	DC3PrivateElementDictionary(struct DC3PrivateElementDictionaryEntry *t)
		{ Table=t; }

	const DC3PrivateElementDictionaryEntry *FindEntry(Uint16 element)
		{
			DC3PrivateElementDictionaryEntry *ptr = Table;
			while (ptr && ptr->ValueRepresentation) {
				if (element == ptr->Element)
					return ptr;
				++ptr;
			}
			return 0;
		}
	const DC3PrivateElementDictionaryEntry *FindEntry(char *keyword)
		{
			DC3PrivateElementDictionaryEntry *ptr = Table;
			while (ptr && ptr->ValueRepresentation) {
				if (strcmp(keyword,ptr->Keyword) == 0)
					return ptr;
				++ptr;
			}
			return 0;
		}

	char *	GetValueRepresentation(Uint16 element)
		{
			const DC3PrivateElementDictionaryEntry *ptr = 
				FindEntry(element);
			if (ptr) return ptr->ValueRepresentation;
			else return 0;
		}
	Uint16	GetValueMultiplicityMinimum(Uint16 element)
		{
			const DC3PrivateElementDictionaryEntry *ptr = 
				FindEntry(element);
			if (ptr) return ptr->ValueMultiplicityMinimum;
			else return VMNONE;
		}
	Uint16	GetValueMultiplicityMaximum(Uint16 element)
		{
			const DC3PrivateElementDictionaryEntry *ptr = 
				FindEntry(element);
			if (ptr) return ptr->ValueMultiplicityMaximum;
			else return VMNONE;
		}
	char *	GetDescription(Uint16 element)
		{
			const DC3PrivateElementDictionaryEntry *ptr =
				 FindEntry(element);
			// if (ptr) return ptr->Description;
			if (ptr) return ptr->Keyword;
			else return 0;
		}
	char *	GetKeyword(Uint16 element)
		{
			const DC3PrivateElementDictionaryEntry *ptr = 
				FindEntry(element);
			if (ptr) return ptr->Keyword;
			else return 0;
		}
	int	GetElement(char *keyword,Uint16 &element)
		{
			const DC3PrivateElementDictionaryEntry *ptr = 
				FindEntry(keyword);
			if (ptr) {
				element=ptr->Element;
				return 0;
			}
			else return -1;
		}
};

extern class DC3PrivateElementDictionary PrivateElementDictionary;

// For Private groups, algorithm is find owner in Owners table given
// a group and element, then search for group, element %0x0100, owner
// in Table.

struct DC3PrivateDictionaryEntry {
	Uint16		Group;
	Uint16		Element;
	char *		ValueRepresentation;
	Uint16		ValueMultiplicityMinimum;
	Uint16		ValueMultiplicityMaximum;
	char *		Owner;
	char *		Keyword;
};

struct DC3PrivateOwnerEntry {
	Uint16		Group;
	Uint16		ElementBlock;		// 0x1000 to 0xff00
	char *		Owner;
	struct DC3PrivateOwnerEntry *next;
};

class DC3PrivateDictionary {
	
	struct DC3PrivateDictionaryEntry *Table;
	struct DC3PrivateOwnerEntry *Owners;

	char *	GetOwner(DC3Tag tag)
		{
			struct DC3PrivateOwnerEntry *ptr = Owners;
			while (ptr && ptr->Owner) {
				if (tag.getGroup() == ptr->Group
				 && (tag.getElement()&0xff00)
					== ptr->ElementBlock
						) return ptr->Owner;
				ptr=ptr->next;
			}
			return 0;
		}
	const DC3PrivateDictionaryEntry *FindEntry(DC3Tag tag,char *owner)
		{
			DC3PrivateDictionaryEntry *ptr = Table;
			Uint16 group=tag.getGroup();
			Uint16 element=tag.getElement()&0x00ff;
			// use strncmp to avoid trailing padding mismatching
			unsigned ownerlng=strlen(owner);
			while (ownerlng && isspace(owner[ownerlng-1]))
				--ownerlng;
			while (ptr && ptr->ValueRepresentation) {
				if (group == ptr->Group
				 && element == ptr->Element
				 && owner && ptr->Owner
				 && ownerlng == strlen(ptr->Owner)
				 && strncmp(owner,ptr->Owner,ownerlng) == 0
					) return ptr;
				++ptr;
			}
			return 0;
		}
public:
	DC3PrivateDictionary(struct DC3PrivateDictionaryEntry *t)
		{ Table=t; Owners=0; }

	const DC3PrivateDictionaryEntry *FindEntry(DC3Tag tag)
		{
			char *owner=GetOwner(tag);
			return FindEntry(tag,owner);
		}
	// const DC3PrivateDictionaryEntry *FindEntry(char *keyword);

	char *	GetValueRepresentation(DC3Tag tag)
		{
			const DC3PrivateDictionaryEntry *ptr = FindEntry(tag);
			if (ptr) return ptr->ValueRepresentation;
			else return 0;
		}
	Uint16	GetValueMultiplicityMinimum(DC3Tag tag)
		{
			const DC3PrivateDictionaryEntry *ptr = FindEntry(tag);
			if (ptr) return ptr->ValueMultiplicityMinimum;
			else return VMNONE;
		}
	Uint16	GetValueMultiplicityMaximum(DC3Tag tag)
		{
			const DC3PrivateDictionaryEntry *ptr = FindEntry(tag);
			if (ptr) return ptr->ValueMultiplicityMaximum;
			else return VMNONE;
		}
	char *	GetDescription(DC3Tag tag)
		{
			const DC3PrivateDictionaryEntry *ptr = FindEntry(tag);
			// if (ptr) return ptr->Description;
			if (ptr) return ptr->Keyword;
			else return 0;
		}
	char *	GetKeyword(DC3Tag tag)
		{
			const DC3PrivateDictionaryEntry *ptr = FindEntry(tag);
			if (ptr) return ptr->Keyword;
			else return 0;
		}

	void	AddOwner(DC3Tag Tag,const char *Owner)
		{
			DC3PrivateOwnerEntry *entry = 
				new DC3PrivateOwnerEntry;

			entry->Group=Tag.getGroup();
			// elements 0x0010-0x00ff define blocks 0x1000-0xff00
			entry->ElementBlock=Tag.getElement()<<8;
			entry->Owner=new char[strlen(Owner)];
			strcpy(entry->Owner,Owner);

			entry->next=Owners;	// add to head of list
			Owners=entry;
		}
};

extern class DC3PrivateDictionary PrivateDictionary;

#endif /* _DC3tddict_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
