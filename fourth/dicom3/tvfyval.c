/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tvfyval.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }
#include <iostream.h>

#include "tvfyval.h"
#include "tddict.h"

static void
showElementDecription(ostream& stream,DC3Attribute *attr)
{
	DC3Tag tag = attr->GetTag();
	char *desc=GlobalDataDictionary.GetDescription(tag);
	stream << (desc ? desc : "");
}

void
DC3VerifyStringDefinedTerms(DC3Attribute *attr,char *(*method)(char *value))
{
	if (attr) {
		char *value;
		if (attr->SingleValueStringCopy(value) != -1 && value) {
			char *desc;
			if (value && (desc=(*method)(value))) {
#ifdef REPORTVALID
				cerr << "Info - Recognized Defined Term <"
				     << value << "> is <" << desc
				     << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
#endif /* REPORTVALID */
				delete desc;
			}
			else {
				cerr << "Warning - Unrecognized Defined Term <"
				     << value << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
			}
			delete value;
		}
		// If no value, value multiplicity error flagged elsewhere
	}
}

void
DC3VerifyStringEnumValues(DC3Attribute *attr,char *(*method)(char *value))
{
	if (attr) {
		char *value;
		if (attr->SingleValueStringCopy(value) != -1 && value) {
			char *desc;
			if (value && (desc=(*method)(value))) {
#ifdef REPORTVALID
				cerr << "Info - Recognized Enumerated Value <"
				     << value << "> is <" << desc
				     << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
#endif /* REPORTVALID */
				delete desc;
			}
			else {
				cerr << "Error - Unrecognized Enumerated"
				     << " Value <"
				     << value << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
			}
			delete value;
		}
		// If no value, value multiplicity error flagged elsewhere
	}
}

void
DC3VerifyBinaryEnumValues(DC3Attribute *attr,char *(*method)(Uint16 value))
{
	if (attr) {
		Xint xvalue;
		Uint16 value;
		if (attr->SingleValueNumeric(xvalue) != -1) {
			value=(Uint16)xvalue;
			char *desc;
			if ((desc=(*method)(value))) {
#ifdef REPORTVALID
				cerr << "Info - Recognized Enumerated Value <"
				     << hex << value << "> is <" << desc
				     << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
#endif /* REPORTVALID */
				delete desc;
			}
			else {
				cerr << "Error - Unrecognized Enumerated"
				     << " Value <" << hex << value
				     << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
			}
		}
		// If no value, value multiplicity error flagged elsewhere
	}
}

void
DC3VerifyBinaryBitMap(DC3Attribute *attr,char *(*method)(Uint16 value))
{
	if (attr) {
		Xint xvalue;
		Uint16 value;
		if (attr->SingleValueNumeric(xvalue) != -1) {
			value=(Uint16)xvalue;
			char *desc;
			if ((desc=(*method)(value))) {
#ifdef REPORTVALID
				cerr << "Info - Recognized BitMap <" 
				     << hex << value << "> is <" << desc 
				     << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
#endif /* REPORTVALID */
				delete desc;
			}
			else {
				cerr << "Error - Unrecognized Values"
				     << " in BitMap <" << hex << value
				     << "> for Data Element <";
				showElementDecription(cerr,attr);
				cerr << ">\n" << flush;
			}
		}
		// If no value, value multiplicity error flagged elsewhere
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
