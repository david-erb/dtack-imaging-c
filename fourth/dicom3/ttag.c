/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)ttag.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <iostream.h>
#include <iomanip.h>

#include "ttag.h"
#include "tddict.h"

void
DC3Tag::putNoDesc(ostream& stream)
{
	stream << "(" << hex << setfill('0') << setw(6) << Group
	       << "," << hex << setfill('0') << setw(6) << Element
	       << ") " << setfill(' ') << flush;
}

void
DC3Tag::put(ostream& stream)
{
	putNoDesc(stream);

	char *vr;
	char *desc;

	if (isPrivateGroup()) {
		if (isPrivateOwner()) {
			vr="LO";		// PS3.5-7.8.1
			desc="PrivateCreator";
		}
		else if (isPrivateTag()) {
			vr=PrivateDictionary.
				GetValueRepresentation(*this);
			desc=PrivateDictionary.
				GetDescription(*this);
		}
		else {
			vr=PrivateElementDictionary.
				GetValueRepresentation(Element);
			desc=PrivateElementDictionary.
				GetDescription(Element);
		}
	}
	else {
		vr=GlobalDataDictionary.GetValueRepresentation(*this);
		desc=GlobalDataDictionary.GetDescription(*this);
	}
	stream << (vr ? vr : "") << " ";
	stream << (desc ? desc : "") << " " << flush;
}

void
DC3Tag::read(DC3istream& stream)
{
	Group=stream.read16();
	Element=stream.read16();
}

void
DC3Tag::write(DC3ostream& stream)
{
	if (Group == 0x0002)
		stream.UseMetaHeaderTransferSyntax();
	else
		stream.UseNormalTransferSyntax();

	stream.write16(Group);
	stream.write16(Element);
}

void
DC3Tag::unread(DC3istream& stream)
{
	stream.unread(-4);
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
