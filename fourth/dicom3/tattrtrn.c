/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tattrtrn.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include "tattrtrn.h"

// The order of the arguments is not necessarily intuitive
// but is consistent with memcpy(), strcpy() etc.

void DC3VRTranslate(DC3Attribute& to,DC3Attribute& from)
{
	void (*method)(DC3Attribute& to,DC3Attribute& from);

	method=0;
	if (from.IsNumeric()) {
		if (to.IsNumeric()) method=DC3VRTranslate_NumericFromNumeric;
		if (to.IsString())  method=DC3VRTranslate_StringFromNumeric;
	}
	if (from.IsNumericString()) {
		if (to.IsNumeric()) method=DC3VRTranslate_NumericFromString;
	}
	if (from.IsString()) {
		if (to.IsString())  method=DC3VRTranslate_StringFromString;
	}
	if (method) {
		(*method)(to,from);
	}
	else {
		cerr << "Can't translate <"
		     << from.ValueRepresentation()
		     << "> to <"
		     << to.ValueRepresentation()
		     << ">\n" << flush;
	}
}
		
void
DC3VRTranslate_StringFromString(DC3Attribute& to,DC3Attribute& from)
{
	from.first();
	while (from.ismore()) {
		to.AddStringCopy(from.ValueString());
		from.next();
	}
}

void
DC3VRTranslate_NumericFromString(DC3Attribute& to,DC3Attribute& from)
{
	from.first();
	while (from.ismore()) {
		// attribute methods handle length/sign stuff
		// Xint is a generic unsigned big enough to hold anything
		Xint value=from.ValueXint();
		to.AddXint(value);
		from.next();
	}
}

void
DC3VRTranslate_StringFromNumeric(DC3Attribute& to,DC3Attribute& from)
{
	from.first();
	while (from.ismore()) {
		// attribute methods handle length/sign stuff
		char *value=from.ValueNumericStringCopy();
		to.AddStringCopy(value);
		delete value;
		from.next();
	}
}

void
DC3VRTranslate_NumericFromNumeric(DC3Attribute& to,DC3Attribute& from)
{
	from.first();
	while (from.ismore()) {
		// attribute methods handle length/sign stuff
		// Xint is a generic unsigned big enough to hold anything
		Xint value=from.ValueXint();
		to.AddXint(value);
		from.next();
	}
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
