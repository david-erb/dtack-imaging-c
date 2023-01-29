/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tsyn.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }


#include "tsyn.h"

#include "tsynd.h"

#include <string.h>

#ifdef TESTSYN
#include <iostream.h>
#endif /* TESTSYN */

DC3TransferSyntax::DC3TransferSyntax(const char *newUID)
{
	struct DC3TransferSyntaxTableEntry *ptr = DC3TransferSyntaxTable;
	while (ptr->UID) {
		if (strcmp(ptr->UID,newUID) == 0) {
			// Found a match
			UID=ptr->UID;
			Description=ptr->Description;
			Endian=ptr->Endian;
			VRType=ptr->VRType;
			Encapsulated=ptr->Encapsulated;
			return;
		}
		++ptr;
	}
	// Not in the table !
	// Fill entry with NULL value which can be tested as failure
	UID=0;
	return;
}


DC3TransferSyntax::~DC3TransferSyntax(void)
{
// Don't need to free anything as ptrs were into static DC3TransferSyntaxTable
}

#ifdef TESTSYN

int
main()
{
	char c,buffer[100];

	cin.tie(&cout);
	while (cin.get(buffer,100,'\n')) {
		cin.get(c); // Discard terminator !
		DC3TransferSyntax test(buffer);
		if (test.isValid()) {
			cout	<< "Found "
				<< test.Describe()
				<< " Endian = "
				<< (test.isLittleEndian()?"Little":"Big")
				<< " VRType = "
				<< (test.isImplicitVR()?"Implicit":"Explicit")
				<< " Encapsulated = "
				<< (test.isEncapsulated()?"Yes":"No")
				<< "\n";
		}
		else {
			cout << "Null\n";
		}
	}
}

#endif /* TESTSYN */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
