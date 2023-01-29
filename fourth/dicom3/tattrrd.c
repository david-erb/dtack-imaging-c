/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tattrrd.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

//#define JUSTSKIPBAD
#define MAXBADVALUELENGTH 512		// Arbitrary

#include <string.h>

#include "tattr.h"
#include "tattrrd.h"
#include "tddict.h"
#include "tddictc.h"

void
DC3AttributeRead(DC3istream& stream,
	int reportlevel,		// 0=none,1=fatal errors,2=warnings
	DC3Tag *rtag,			// return tag read
	DC3Attribute **rattr,		// return new attribute or 0
	int *rerr			// 0=ok,1=fail
	)
{
	char vre[3],*vrd,*vru;

	DC3Tag Tag;
	Tag.read(stream);

	if (!stream.good()) {	// Failed read ... presume EOF & exit gracefully
		*rattr=0;
		*rerr=0;
		return;
	}

	if (stream.HaveMetaHeaderSpecifiedTransferSyntax() 
	 && Tag.getGroup() != 0x0002) {
		if (stream.CanReposition()) {
			Tag.unread(stream);
			stream.SwitchToMetaHeaderSpecifiedTransferSyntax();
			Tag.read(stream);
		}
		else {
			cerr << "DC3AttributeRead: Can't unread to "
			     << "use metaheader specified transfer syntax\n"
			     << flush;
			*rattr=0;
			*rerr=1;
			return;
		}
	}

	// Private Groups ... for now handle only ValueRepresentation issues
	//                (updating dictionary is handled after attr is read)

	if (Tag.isPrivateGroup()) {
		if (Tag.isPrivateOwner()) {
			vrd="LO";		// PS3.5-7.8.1
		}
		else if (Tag.isPrivateTag()) {
			// owner will have been entered by now
			vrd=PrivateDictionary.
				GetValueRepresentation(Tag);
		}
		else {
			// things that are same for all groups ...
			vrd=PrivateElementDictionary.
				GetValueRepresentation(Tag.getElement());
		}
	}
	else {
		vrd=GlobalDataDictionary.GetValueRepresentation(Tag);
	}

	//Tag.put(cerr); cerr << "Dictionary VR (" << vrd << ")\n" << flush;

	if (stream.GetTransferSyntax()->isExplicitVR()) {
		stream.read(vre,2);
		vre[2]=0;
		if (!vrd) {
			if (reportlevel >= 2) {
				Tag.put(cerr);
				cerr << "No Such Attribute in Dictionary"
				     << " - Assuming Explicit VR OK\n"
				     << flush;
			}
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
				if (reportlevel >= 2) {
				   Tag.put(cerr);
				   cerr << "Explicit VR (" << vre
			  	   << ") != Data Dictionary (" << vrd << ")"
			     	   << " - Assuming Explicit VR OK\n" << flush;
				}
			}
		}
		vru=vre;	// Use Explicit VR
	}
	else {			// Must depend on Implicit VR being OK
		if (vrd) {
			vru=vrd;
		}
		else {
			if (reportlevel >= 2) {
				Tag.put(cerr);
				cerr << "No Such Attribute in Dictionary"
				     << " - Implicit VR - Ignoring it\n"
				     << flush;
			}
			vru="BAD";	// NOT 0 - allows fall thru switch, etc.
		}
	}
	//Tag.put(cerr); cerr << "Using VR (" << vru << ")\n" << flush;

	// Get appropriate Value Length (16 or 32) ...

	Uint32 ValueLength;

	if (stream.GetTransferSyntax()->isImplicitVR()) {
		ValueLength=stream.read32();
	}
	else {
		if (vru) {	// Explicit OB,OW,SQ
			if (vru[0]=='O' && (vru[1]=='B' || vru[1]=='W')
			 || vru[0]=='S' &&  vru[1]=='Q') {
				(void)stream.read16();
				ValueLength=stream.read32();
			}
			else {
				ValueLength=stream.read16();
			}
		}
		else {
			if (reportlevel >= 1) {
				Tag.put(cerr);
				cerr << "Internal Error - No explicit VR\n"
				     << flush;
			}
			// Should abort but ...
			ValueLength=stream.read16();
		}
	}

	// Based on VR, whether explicit or from dictionary,
	// create a NEW attribute of the proper type and return it
	DC3Attribute *attr = 0;
	int err=0;
	int badlength=0;
	switch (vru[0]) {
		case 'A':	switch (vru[1]) {
				case 'E': {
					DC3ApplicationEntity *a;
					a=new DC3ApplicationEntity
						(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'S': {
					DC3AgeString *a;
					a=new DC3AgeString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'T': {
					DC3AttributeTag *a;
					a=new DC3AttributeTag(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'C':	switch (vru[1]) {
				case 'S': {
					DC3CodeString *a;
					a=new DC3CodeString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'D':	switch (vru[1]) {
				case 'A': {
					DC3DateString *a;
					a=new DC3DateString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'S': {
					DC3DecimalString *a;
					a=new DC3DecimalString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'T': {
					DC3DateTimeString *a;
					a=new DC3DateTimeString
						(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'F':	switch (vru[1]) {
				case 'L':
					DC3AttributeSkip(stream,ValueLength);
					break;
				case 'D':
					DC3AttributeSkip(stream,ValueLength);
					break;
				default:	err=1; break;
				}
				break;
		case 'I':	switch (vru[1]) {
				case 'S': {
					DC3IntegerString *a;
					a=new DC3IntegerString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'L':	switch (vru[1]) {
				case 'O': {
					DC3LongString *a;
					a=new DC3LongString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'T': {
					DC3LongText *a;
					a=new DC3LongText(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'O':	switch (vru[1]) {
				case 'B': {
					DC3OtherByteString *a;
					a=new
					  DC3OtherByteString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'W': {
					if (ValueLength%2) {
						badlength=1;
						break;
					}
					DC3OtherWordString *a;
					a=new
					  DC3OtherWordString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'X':
					if (stream.GetTransferSyntax()
					   ->isEncapsulated()) {
						DC3OtherByteString *a;
						a=new
					  	DC3OtherByteString
							(Tag,ValueLength);
						a->read(stream,ValueLength);
						attr=a;
					}
					else {
						if (ValueLength%2) {
							badlength=1;
							break;
						}
						DC3OtherWordString *a;
						a=new
					  	DC3OtherWordString
							(Tag,ValueLength);
						a->read(stream,ValueLength);
						attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'P':	switch (vru[1]) {
				case 'N': {
					DC3PersonName *a;
					a=new DC3PersonName(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'R':	if (vru[1] == 'E' && vru[2] == 'T') {
					if (reportlevel >= 2) {
					    Tag.put(cerr);
					    cerr << "Retired Attribute\n"
					         << flush;
					}
					DC3AttributeSkip(stream,ValueLength);
					err=0;	// Don't have to stop stream
				}
				else {
					err=1;
				}
				break;
		case 'S':	switch (vru[1]) {
				case 'H': {
					DC3ShortString *a;
					a=new DC3ShortString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'L': {
					if (ValueLength%4) {
						badlength=1;
						break;
					}
					DC3SignedLong *a;
					a=new DC3SignedLong(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'Q':
					break;
				case 'S': {
					if (ValueLength%2) {
						badlength=1;
						break;
					}
					DC3SignedShort *a;
					a=new DC3SignedShort(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'T': {
					DC3ShortText *a;
					a=new DC3ShortText(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'X':
					DC3AttributeSkip(stream,ValueLength);
					break;
				default:	err=1; break;
				}
				break;
		case 'T':	switch (vru[1]) {
				case 'M': {
					DC3TimeString *a;
					a=new DC3TimeString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'U':	switch (vru[1]) {
				case 'I': {
					DC3UIString *a;
					a=new DC3UIString(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'L': {
					if (ValueLength%4) {
						badlength=1;
						break;
					}
					DC3UnsignedLong *a;
					a=new DC3UnsignedLong(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'S': {
					if (ValueLength%2) {
						badlength=1;
						break;
					}
					DC3UnsignedShort *a;
					a=new DC3UnsignedShort(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		case 'X':	switch (vru[1]) {
				case 'L': {
					if (ValueLength%4) {
						badlength=1;
						break;
					}
					DC3UnspecifiedLong *a;
					a=new DC3UnspecifiedLong
						(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				case 'S': {
					if (ValueLength%2) {
						badlength=1;
						break;
					}
					DC3UnspecifiedShort *a;
					a=new DC3UnspecifiedShort
						(Tag,ValueLength);
					a->read(stream,ValueLength);
					attr=a;
					}
					break;
				default:	err=1; break;
				}
				break;
		default:
				if (ValueLength > MAXBADVALUELENGTH) {
					Tag.put(cerr);
					cerr << "**** Unrecognized VR "
					     << "and too long to display("
					     << ValueLength
					     << ") ... probably wrong "
					     << "transfer syntax\n"
					     << flush;
					DC3AttributeSkip(stream,ValueLength);
				}
				else {
#ifdef JUSTSKIPBAD
					if (reportlevel >= 2) {
						Tag.put(cerr);
						cerr << "**** Skipping bad VR"
						     << ValueLength << "\n"
						     << flush;
					}
					DC3AttributeSkip(stream,ValueLength);
#else /* JUSTSKIPBAD */
					// assume string
					{
						DC3LongText a(Tag,ValueLength);
						a.read(stream,ValueLength);
						Tag.put(cerr);
						cerr << "**** Unrecognized VR "
						     << "... assume LT ...\n"
						     << flush;
						a.put(cerr);
					}
#endif /* JUSTSKIPBAD */
				}
				err=1;
				break;
	}
	if (badlength) {
		Tag.put(cerr);
		cerr << "**** Incorrect value length for binary ("
		     << ValueLength << ") skipping specified length\n"
		     << flush;
		DC3AttributeSkip(stream,ValueLength);
		err=1;
	}
	if (err && reportlevel >= 2) {
			Tag.put(cerr);
			cerr << "**** Unusable VR (" << vru << ")\n" << flush;
	}
	if (!stream.good()) {
		if (reportlevel >= 1) {
			Tag.put(cerr);
			cerr << "**** Read failed in middle of an attribute\n"
			     << flush;
		}
		attr=0; err=1;
	}


	static DC3Tag TransferSyntaxUIDTag(DC3TransferSyntaxUID_GROUP,
			DC3TransferSyntaxUID_ELEMENT);

	if (Tag == TransferSyntaxUIDTag) {
		if (!attr || err || !attr->IsString()) {
			Tag.put(cerr);
			cerr << "**** Bad TransferSyntaxUID attribute\n"
			     << flush;
			err=1;
		}
		else {
			const char *uid=attr->ValueString();
			DC3TransferSyntax *ts = new DC3TransferSyntax(uid);
			if (!ts->isValid()) {
				Tag.put(cerr);
				cerr << "**** TransferSyntaxUID is invalid\n"
				     << flush;
				err=1;
			}
			else {
				stream.SetMetaHeaderSpecifiedTransferSyntax(ts);
			}
		}
	}

	// Private Groups ... update dictionary with new Private Owner blocks

	if (Tag.isPrivateGroup() && Tag.isPrivateOwner()) {
		if (!attr || err || !attr->IsString()) {
			Tag.put(cerr);
			cerr << "**** Bad Private Owner definition attribute\n"
			     << flush;
			err=1;
		}
		else {
			PrivateDictionary.
				AddOwner(Tag,attr->ValueString());
		}
	}

	*rattr=attr;
	*rerr=err;
	*rtag=Tag;	// return so that sequence can be checked
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
