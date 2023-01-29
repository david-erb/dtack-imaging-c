/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tmodulet.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <string.h>

#include "tmodulet.h"

// Data element routines for use in building modules ...

// Could be methods of DC3Attribute ...

static int
ValidateVR(DC3Attribute *attr,const char *module,const char *element)
{
	DC3Tag tag=attr->GetTag();
	const char *vrd=GlobalDataDictionary.GetValueRepresentation(tag);
	const char *vre=attr->ValueRepresentation();

	if (!vrd) {
		cerr << "Internal Error - No Such Element in Dictionary"
		     << " Data Element <"
		     << element
		     << "> in Module <"
		     << module
		     << ">\n" << flush;
		return 0;
	}
	else {
		if (strncmp(vre,vrd,2) != 0
		    && !( strncmp(vrd,"OX",2) == 0
		          && (strncmp(vre,"OB",2) == 0
		              || strncmp(vre,"OW",2) == 0)
		        )
		    && !( strncmp(vrd,"XS",2) == 0
		          && (strncmp(vre,"US",2) == 0
		              || strncmp(vre,"SS",2) == 0)
		        )
		    && !( strncmp(vrd,"XL",2) == 0
		          && (strncmp(vre,"UL",2) == 0
		              || strncmp(vre,"SL",2) == 0)
		        )
		    ) {
			cerr << "Protocol Violation - "
			     << "Invalid Value Representation "
			     <<  vre << " (" << vrd
			     << " required)"
			     << " Data Element <"
			     << element
			     << "> in Module <"
			     << module
			     << ">\n" << flush;
			return 0;
		}
		else {
			return 1;
		}
	}
}

static int
ValidateVM(DC3Attribute *attr,Uint16 multiplicity,
		const char *module,const char *element)
{
	DC3Tag tag=attr->GetTag();
	Uint16 vm=attr->GetValueMultiplicity();
	Uint16 dictmin=GlobalDataDictionary.GetValueMultiplicityMinimum(tag);
	Uint16 dictmax=GlobalDataDictionary.GetValueMultiplicityMaximum(tag);
	Uint16 errmin,errmax;
	int err=0;
	char *source;
	if (multiplicity == VMNONE) {	// ie. don't override dictionary
		if (vm < dictmin || vm >dictmax) {
			err=1;
			errmin=dictmin;
			errmax=dictmax;
			source="Dictionary";
		}
	}
	else {
		if (vm != multiplicity) {
			errmin=errmax=multiplicity;
			err=1;
			source="Module Definition";
		}
	}
	if (err) {
		cerr << "Protocol Violation - "
		     << "Invalid Value Multiplicity "
		     << dec << vm << " (" << errmin;
		if (errmin != errmax) {
			if (errmax == VMUNLIMITED)
				cerr << "-n";
			else
				cerr << "-" << errmax;
		}
		cerr << " required in " << source << ")"
		     << " Data Element <"
		     << element
		     << "> in Module <"
		     << module
		     << ">\n" << flush;
		return 0;
	}
	else {
		return 1;
	}
}

// Could be methods of DC3ListofAttributes, but too specific to module stuff ...

inline void
DC3ViolationMessage(const char *error,const char *elementtype,
		const char *module,const char *element)
{
	cerr << "Protocol Violation - "
	     << error
	     << " "
	     << elementtype
	     << " Data Element <"
	     << element
	     << "> in Module <"
	     << module
	     << ">\n" << flush;
}

inline void
DC3WarningMessage(const char *error,const char *elementtype,
		const char *module,const char *element)
{
	cerr << "Warning - "
	     << error
	     << " "
	     << elementtype
	     << " Data Element <"
	     << element
	     << "> in Module <"
	     << module
	     << ">\n" << flush;
}

inline void
DC3ValidMessage(const char *elementtype,
		const char *module,const char *element)
{
#ifdef REPORTVALID
	cerr << "Valid Element - "
	     << elementtype
	     << " Data Element <"
	     << element
	     << "> in Module <"
	     << module
	     << ">\n" << flush;
#else /* REPORTVALID */
	(void)elementtype; (void)module; (void)element;
#endif /* REPORTVALID */
}

DC3Attribute *
getDataElementUntyped(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element)
{
	DC3Attribute *attr = 0;

	// No type - from Normalized module - presume is required

	{
		attr=list->find(tag);
		if (!attr) {
			DC3ViolationMessage("Missing",
				"Normalized Required",module,element);
		}
		else {
			{
				if (!ValidateVR(attr,module,element)) {
					DC3ViolationMessage(
						"Invalid Value Representation",
						"Normalized Required",
						module,element);
					attr=0;
				}
				else {
					if (!ValidateVM(attr,multiplicity,
							module,element)) {
						DC3ViolationMessage(
						"Invalid Value Multiplicity",
							"Normalized Required",
							module,element);
						attr=0;
					}
					else {
						DC3ValidMessage("Normalized",
							module,element);
						// attr->Reference();
					}
				}
			}
		}
	}
	return attr;
}

DC3Attribute *
getDataElementType1(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element)
{
	DC3Attribute *attr = 0;

	// Type 1 - Required Data Element

	{
		attr=list->find(tag);
		if (!attr) {
			DC3ViolationMessage("Missing",
				"Type 1 Required",module,element);
		}
		else {
			{
				if (!ValidateVR(attr,module,element)) {
					DC3ViolationMessage(
						"Invalid Value Representation",
						"Type 1 Required",
						module,element);
					attr=0;
				}
				else {
					if (!ValidateVM(attr,multiplicity,
							module,element)) {
						DC3ViolationMessage(
						"Invalid Value Multiplicity",
							"Type 1 Required",
							module,element);
						attr=0;
					}
					else {
						DC3ValidMessage(
							"Type 1 Required",
							module,element);
						// attr->Reference();
					}
				}
			}
		}
	}
	return attr;
}

DC3Attribute *
getDataElementType1C(DC3ListofAttributes *list,DC3Tag tag,
		int (*condition)(DC3ListofAttributes *),
		Uint16 multiplicity,const char *module,const char *element)
{
	DC3Attribute *attr = 0;

	// Type 1C - Conditional Data Element

	{
		attr=list->find(tag);
		if (!attr) {
			if (condition && (*condition)(list)) {
				DC3ViolationMessage("Missing",
					"Type 1C Conditional",module,element);
			}
		}
		else {
			if (condition && !(*condition)(list)) {
				DC3WarningMessage("Unnecessary",
					"Type 1C Conditional",module,element);
			}
			{
				if (!ValidateVR(attr,module,element)) {
					DC3ViolationMessage(
					"Invalid Value Representation",
						"Type 1C Conditional",
						module,element);
					attr=0;
				}
				else {
					if (!ValidateVM(attr,multiplicity,
							module,element)) {
						DC3ViolationMessage(
						"Invalid Value Multiplicity",
							"Type 1C Conditional",
							module,element);
						attr=0;
					}
					else {
						DC3ValidMessage(
							"Type 1C Conditional",
							module,element);
						// attr->Reference();
					}
				}
			}
		}
	}
	return attr;
}

DC3Attribute *
getDataElementType2(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element)
{
	DC3Attribute *attr = 0;

	// Type 2 - Required Data Element

	{
		attr=list->find(tag);
		if (!attr) {
			DC3ViolationMessage("Missing",
				"Type 2 Required",module,element);
		}
		else {
			// Allowed to be null, but must be good if present
			if (attr->GetValueLength() != 0) {
				if (!ValidateVR(attr,module,element)) {
					DC3ViolationMessage(
					"Invalid Value Representation",
						"Type 2 Required",
						module,element);
					attr=0;
				}
				else {
					if (!ValidateVM(attr,multiplicity,
							module,element)) {
						DC3ViolationMessage(
						"Invalid Value Multiplicity",
							"Type 2 Required",
							module,element);
						attr=0;
					}
					else {
						DC3ValidMessage(
							"Type 2 Required",
							module,element);
						// attr->Reference();
					}
				}
			}
		}
	}
	return attr;
}

DC3Attribute *
getDataElementType2C(DC3ListofAttributes *list,DC3Tag tag,
		int (*condition)(DC3ListofAttributes *),
		Uint16 multiplicity,const char *module,const char *element)
{
	DC3Attribute *attr = 0;

	// Type 2C - Conditional Data Element

	{
		attr=list->find(tag);
		if (!attr) {
			if (condition && (*condition)(list)) {
				DC3ViolationMessage("Missing",
					"Type 2C Conditional",module,element);
			}
		}
		else {
			if (condition && !(*condition)(list)) {
				DC3WarningMessage("Unnecessary",
					"Type 2C Conditional",module,element);
			}
			// Allowed to be null, but must be good if present
			if (attr->GetValueLength() != 0) {
				if (!ValidateVR(attr,module,element)) {
					DC3ViolationMessage(
					"Invalid Value Representation",
						"Type 2C Conditional",
						module,element);
					attr=0;
				}
				else {
					if (!ValidateVM(attr,multiplicity,
							module,element)) {
						DC3ViolationMessage(
						"Invalid Value Multiplicity",
							"Type 2C Conditional",
							module,element);
						attr=0;
					}
					else {
						DC3ValidMessage(
							"Type 2C Conditional",
							module,element);
						// attr->Reference();
					}
				}
			}
		}
	}
	return attr;
}

DC3Attribute *
getDataElementType3(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element)
{
	DC3Attribute *attr = 0;

	// Type 3 - Optional Data Element

	{
		attr=list->find(tag);
		if (attr) {
			// Allowed to be null, but must be good if present
			if (attr->GetValueLength() != 0) {
				if (!ValidateVR(attr,module,element)) {
					DC3ViolationMessage(
					"Invalid Value Representation",
						"Type 3 Optional",
						module,element);
					attr=0;
				}
				else {
					if (!ValidateVM(attr,multiplicity,
							module,element)) {
						DC3ViolationMessage(
						"Invalid Value Multiplicity",
							"Type 3 Optional",
							module,element);
						attr=0;
					}
					else {
						DC3ValidMessage(
							"Type 3 Optional",
							module,element);
						// attr->Reference();
					}
				}
			}
		}
	}
	return attr;
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
