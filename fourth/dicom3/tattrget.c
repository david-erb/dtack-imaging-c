/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tattrget.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <string.h>

#include "tattr.h"
#include "tddict.h"

void
DC3AttributeGet(char *keyword,char *values,DC3Attribute **rattr,int *rerr)
{
	const DC3DataDictionaryEntry *entry =
		GlobalDataDictionary.FindEntry(keyword);

	if (!entry) {
		cerr << "DC3AttributeGet: No such keyword <"
		     << keyword << ">\n" << flush;
		*rattr=0;
		*rerr=1;
		return;
	}

	char *vru = entry->ValueRepresentation;
	DC3Tag Tag(entry->Group,entry->Element);

	// Don't know the value length yet - set by the attribute get()

	// Based on VR from the dictionary,
	// create a NEW attribute of the proper type and return it
	DC3Attribute *attr = 0;
	int err=0;
	istrstream ist(values);
	switch (vru[0]) {
		case 'A':	switch (vru[1]) {
				case 'E': {
					DC3ApplicationEntity *a;
					a=new DC3ApplicationEntity
						(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'S': {
					DC3AgeString *a;
					a=new DC3AgeString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'T': {
#ifdef CRAP
					DC3AttributeTag *a;
					a=new DC3AttributeTag(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
#endif /* CRAP */
					}
					err=1;
					break;
				default:	err=1; break;
				}
				break;
		case 'C':	switch (vru[1]) {
				case 'S': {
					DC3CodeString *a;
					a=new DC3CodeString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'D':	switch (vru[1]) {
				case 'A': {
					DC3DateString *a;
					a=new DC3DateString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'S': {
					DC3DecimalString *a;
					a=new DC3DecimalString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'T': {
					DC3DateTimeString *a;
					a=new DC3DateTimeString
						(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'F':	switch (vru[1]) {
				case 'L':
					break;
				case 'D':
					break;
				default:
					break;
				}
				err=1;
				break;
		case 'I':	switch (vru[1]) {
				case 'S': {
					DC3IntegerString *a;
					a=new DC3IntegerString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'L':	switch (vru[1]) {
				case 'O': {
					DC3LongString *a;
					a=new DC3LongString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'T': {
					DC3LongText *a;
					a=new DC3LongText(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'O':	switch (vru[1]) {
				case 'B':
					break;
				case 'W':
					break;
				case 'X':
					break;
				default:
					break;
				}
				err=1;
				break;
		case 'P':	switch (vru[1]) {
				case 'N': {
					DC3PersonName *a;
					a=new DC3PersonName(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'R':	if (vru[1] == 'E' && vru[2] == 'T') {
					Tag.put(cerr);
					cerr << "Retired Attribute\n" << flush;
				}
				err=1;
				break;
		case 'S':	switch (vru[1]) {
				case 'H': {
					DC3ShortString *a;
					a=new DC3ShortString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'L': {
					DC3SignedLong *a;
					a=new DC3SignedLong(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
					}
					break;
				case 'Q':
					break;
				case 'S': {
					DC3SignedShort *a;
					a=new DC3SignedShort(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
					}
					break;
				case 'T': {
					DC3ShortText *a;
					a=new DC3ShortText(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'X':
					err=1;
					break;
				default:	err=1; break;
				}
				break;
		case 'T':	switch (vru[1]) {
				case 'M': {
					DC3TimeString *a;
					a=new DC3TimeString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'U':	switch (vru[1]) {
				case 'I': {
					DC3UIString *a;
					a=new DC3UIString(Tag,(Uint32)0);
					a->get(values);
					attr=a;
					}
					break;
				case 'L': {
					DC3UnsignedLong *a;
					a=new DC3UnsignedLong(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
					}
					break;
				case 'S': {
					DC3UnsignedShort *a;
					a=new DC3UnsignedShort(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'X':	switch (vru[1]) {
				case 'L': {
					DC3UnspecifiedLong *a;
					a=new DC3UnspecifiedLong
						(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
					}
					break;
				case 'S': {
					DC3UnspecifiedShort *a;
					a=new DC3UnspecifiedShort
						(Tag,(Uint32)0);
					a->get(ist);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		default:
				err=1;
				break;
	}
	*rattr=attr;
	*rerr=err;

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
