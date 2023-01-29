/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tlist.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <string.h>

#include "tlist.h"
#include "tddictc.h"
#include "tsyn.h"	// for Fix() fixedlength() calculations only
#include "tsync.h"	// for Fix() fixedlength() calculations only

static Uint32
fixedlength(DC3TransferSyntax *outsyntax,DC3Attribute *attr)
{
	// returned the length in bytes of the fixed part of an element
	// ie. that part other than that returned by attr->GetValueLength()
	// obviously this is dictated by the Value Representation which
	// in turn depends on the Transfer Syntax

	// see also tattrrd.cc:DC3AttributeRead()

	Uint32 length=4;	// length of tag itself
	// work out length of "value length" field
	if (outsyntax->isImplicitVR()) {
		length+=4;
	}
	else if (outsyntax->isExplicitVR()) {
		const char *vru=attr->ValueRepresentation();
		if (vru) {	// Explicit OB,OW,SQ
			if (vru[0]=='O' && (vru[1]=='B' || vru[1]=='W')
			 || vru[0]=='S' &&  vru[1]=='Q') {
				length+=6;
			}
			else {
				length+=2;
			}
		}
		else {
			(attr->GetTag()).put(cerr);
			cerr << "Internal Error - No explicit VR"
			     << " while calculating attribute length\n"
			     << flush;
			// Should abort but ...
			length+=2;
		}
		length+=2;	// add length of explicit VR itself
	}
	return length;
}


// Handle any special cases required to determine Value Representation

// May need to consult:
//	- other attributes specifying representation
//	- transfer syntax

// If PixelRepresentation = 0 (unsigned) or 1 (signed), can determine:
//
// XS	SmallestImagePixelValue
// XS	LargestImagePixelValue
// XS	SmallestPixelValueInSeries
// XS	LargestPixelValueInSeries
// XS	PixelPaddingValue

// It is not clear how the LUT representation should be specified, and
// furthermore the Decriptors have 3 values :( - assume PixelRepresentation:

// XS	RedPaletteColorLookupTableDescriptor
// XS	GreenPaletteColorLookupTableDescriptor
// XS	BluePaletteColorLookupTableDescriptor
// XS	RedPaletteColorLookupTableData
// XS	GreenPaletteColorLookupTableData
// XS	BluePaletteColorLookupTableData
// XS	LUTDescriptor
// XS	LUTData


// If AudioSampleFormat = 0 (16 bit signed LSB 1st interleaved per channel)
// If AudioSampleFormat = 1 (8 bit signed LSB 1st interleaved per channel)
//
// OX	AudioSampleData

// If DateValueRepresentation is nn then CurveData is:
//	0 - US
//	1 - SS
//	2 - FL
//	3 - FD
//	4 - SL
//
//	Unless implict VR in which case it is OB little endian !
//
// OX	CurveData

// These depend only on the transfer syntax:
//	- Implicit VR Little Endian -> OW
//	- Explicit VR Little Endian -> OW
//	- Explicit VR Big Endian -> OW
//	- Encapsulated Pixel Data -> OB

// OX	PixelData

void
DC3ListofAttributes::RemoveMetaHeaderGroup(void)
{
	first();	// If list altered will re-first() !
	while (ismore()) {
		DC3Attribute *attr=value();
		Uint16 group = attr->GetTag().getGroup();
		if (group == 0x0002) {
			remove(attr);
			first();
			continue;
		}
		else if (group > 0x0002) break;
		next();
	}
}

void
DC3ListofAttributes::Fix(char *outsyntaxUID,int useMetaHeader)
{
	Xint pixelrepresentation;
	DC3Tag pixreptag (DC3PixelRepresentation_GROUP,
		    DC3PixelRepresentation_ELEMENT);
	if (getSingleValueXint(pixreptag,pixelrepresentation) == -1) {
		cerr << "DC3ListofAttributes::Fix: Error - "
		     << "no PixelRepresentation attribute :(\n"
		     << flush;
		// So give up for now ...
	}
	else {
		first();		// If list altered will re-first() !
		while (ismore()) {
			DC3Attribute *attr = value();
			const char *vr = attr->ValueRepresentation();
			if (vr && strcmp(vr,"XS") == 0) {
				DC3UnspecifiedShort *unp = 
					(DC3UnspecifiedShort *)attr;
				switch (pixelrepresentation) {
					case 0:	{
						DC3UnsignedShort *usp =
						   unp->toUnsignedShort();
						remove(unp);
						insert(usp);
						first();
						}
						continue;
					case 1:	{
						DC3SignedShort *ssp =
						   unp->toSignedShort();
						remove(unp);
						insert(ssp);
						first();
						}
						continue;
					default:
						cerr
						 << "DC3ListofAttributes::Fix: "
						 << "Error - Invalid "
						 << "PixelRepresentation <"
						 << pixelrepresentation
						 << ">\n" << flush;
						break;
				}
			}
			next();
		}
	}

	// insert new metaheader attributes if not already there ...
	//
	// NB. The caller should use RemoveMetaHeaderGroup() to
	// remove anything lingering from a previous copy, then
	// explicitly insert any attributes that are not to default
	// to the values added here.

	if (useMetaHeader) {
		{
			DC3Tag tag(
				DC3TransferSyntaxUID_GROUP,
				DC3TransferSyntaxUID_ELEMENT);

			if (!find(tag))
				insert(new DC3UIString(tag,
					strlen(outsyntaxUID),
					outsyntaxUID)
				);
		}
		{
			DC3Tag tag(
				DC3FileMetaInformationVersion_GROUP,
				DC3FileMetaInformationVersion_ELEMENT);

			unsigned char value[2];
			value[0]=0x00;
			value[1]=0x01;		// Version 1

			if (!find(tag))
				insert(new DC3OtherByteString(tag,2,value));
		}
		{
			DC3Tag msuidtag(
				DC3MediaStorageSOPClassUID_GROUP,
				DC3MediaStorageSOPClassUID_ELEMENT);

			if (!find(msuidtag)) {
				DC3Tag uidtag(
					DC3SOPClassUID_GROUP,
					DC3SOPClassUID_ELEMENT);

				DC3Attribute *uidattr = find(uidtag);

				char *uid;

				if (uidattr
				 && uidattr->SingleValueStringCopy(uid) == 0) {
					insert(new DC3UIString(msuidtag,strlen(uid),uid));
					if (uid) delete[] uid;
				}
				else {
					cerr << "DC3ListofAttributes::Fix: Error - "
					     << "building file meta information - "
					     << "SOPClassUID missing or bad :(\n"
					     << flush;
				}
			}
		}
		{
			DC3Tag msuidtag(
				DC3MediaStorageSOPInstanceUID_GROUP,
				DC3MediaStorageSOPInstanceUID_ELEMENT);

			if (!find(msuidtag)) {
				DC3Tag uidtag(
					DC3SOPInstanceUID_GROUP,
					DC3SOPInstanceUID_ELEMENT);

				DC3Attribute *uidattr = find(uidtag);

				char *uid;

				if (uidattr
				 && uidattr->SingleValueStringCopy(uid) == 0) {
					insert(new DC3UIString(msuidtag,strlen(uid),uid));
					if (uid) delete[] uid;
				}
				else {
					cerr << "DC3ListofAttributes::Fix: Error - "
					     << "building file meta information - "
					     << "SOPInstanceUID missing or bad :(\n"
					     << flush;
				}
			}
		}
#ifndef IMPLEMENTATIONCLASSUID
#define IMPLEMENTATIONCLASSUID	"0.0.0.0"
#endif
		{
			DC3Tag tag(
				DC3ImplementationClassUID_GROUP,
				DC3ImplementationClassUID_ELEMENT);

			if (!find(tag))
				insert(new DC3UIString(tag,
					strlen(IMPLEMENTATIONCLASSUID),
					IMPLEMENTATIONCLASSUID)
				);
		}
#ifndef IMPLEMENTATIONVERSIONNAME
#define IMPLEMENTATIONVERSIONNAME	"NOTSPECIFIED"
#endif
		{
			DC3Tag tag(
				DC3ImplementationVersionName_GROUP,
				DC3ImplementationVersionName_ELEMENT);

			if (!find(tag))
				insert(new DC3ShortString(tag,
					strlen(IMPLEMENTATIONVERSIONNAME),
					IMPLEMENTATIONVERSIONNAME)
				);
		}
#ifndef SOURCEAPPLICATIONENTITYTITLE
#define SOURCEAPPLICATIONENTITYTITLE	"NOTSPECIFIED"
#endif
		{
			DC3Tag tag(
				DC3SourceApplicationEntityTitle_GROUP,
				DC3SourceApplicationEntityTitle_ELEMENT);

			if (!find(tag))
				insert(new DC3ApplicationEntity(tag,
					strlen(SOURCEAPPLICATIONENTITYTITLE),
					SOURCEAPPLICATIONENTITYTITLE)
				);
		}
	}
	else {
		RemoveMetaHeaderGroup();
	}

	// Fixup and/or insert group lengths ... must be second last thing

	// insert a dummy LengthToEnd element in case not present ...
	// (otherwise the length of group 0008 would be wrong)

	DC3Tag LengthToEndtag(DC3LengthToEnd_GROUP,DC3LengthToEnd_ELEMENT);

	{
		DC3Attribute *oldattr = find(LengthToEndtag);
		if (oldattr) remove(oldattr);
		Uint32 length=0;
		DC3UnsignedLong *ulp = new
			DC3UnsignedLong(LengthToEndtag,4,&length);
		insert(ulp);
	}

	DC3TransferSyntax normalsyntax(outsyntaxUID);
	DC3TransferSyntax metaheadersyntax(DC3DefaultTransferSyntax);
	DC3TransferSyntax *outsyntax = 0;

	{
		// each (nnnn,0000) group length element
		// contains the length in bytes of the rest of the group
		// not including the length element itself

		Uint16 doinggroup=0;
		Uint32 length=0;
		first();
		while (ismore()) {
			DC3Attribute *attr = value();
			DC3Tag tag = attr->GetTag();

			// tag.put(cerr); cerr << " length fixup\n" << flush;

			if (tag.getGroup() == doinggroup) {
				if (tag.getElement() == 0) {
					// this is group length element
					// delete old one & restart list
					// cerr << "removing\n" << flush;
					remove(attr);
					first();
				}
				else {
					// add element length to group length
					if (doinggroup == 0x0002)
						outsyntax=&metaheadersyntax;
					else
						outsyntax=&normalsyntax;
					length+=(attr->GetValueLength()
						+fixedlength(outsyntax,attr));
					next();
				}
			}
			else if (tag.getGroup() > doinggroup) {
				if (length) {
					// last group really existed, so
					// make & add new length element
					DC3Tag newtag(doinggroup,0);
					DC3UnsignedLong *ulp = new 
					    DC3UnsignedLong(newtag,4,&length);
					insert(ulp);
					first();
				}
				doinggroup=tag.getGroup();
				length=0;
				// don't next() ... go around again !
			}
			else {
				// tag.getGroup() < doinggroup so already done
				next();
			}

		}
		// ugly ! ugly ! deal with last group :(
		if (length) {
			// last group really existed, so
			// make & add new length element
			DC3Tag newtag(doinggroup,0);
			DC3UnsignedLong *ulp = new 
			    DC3UnsignedLong(newtag,4,&length);
			insert(ulp);
		}
	}

	// Fixup LengthToEnd element ... must be last thing done

	{
		Uint32 length=0;
		first();
		while (ismore()) {
			DC3Attribute *attr = value();
			DC3Tag tag = attr->GetTag();

			if (tag > LengthToEndtag) {
				// add element length to group length
				length+=(attr->GetValueLength()
					+fixedlength(&normalsyntax,attr));
			}
			next();
		}

		DC3UnsignedLong *ulp = new
			DC3UnsignedLong(LengthToEndtag,4,&length);
		DC3Attribute *oldattr = find(LengthToEndtag);
		if (oldattr) remove(oldattr);
		insert(ulp);
	}
}

void
DC3ListofAttributes::FixCommand(void)
{
	// Fixup and insert group length ... only command group present

	DC3TransferSyntax commandsyntax(DC3DefaultTransferSyntax);

	{
		// each (nnnn,0000) group length element
		// contains the length in bytes of the rest of the group
		// not including the length element itself

		Uint32 length=0;
		first();
		while (ismore()) {
			DC3Attribute *attr = value();
			DC3Tag tag = attr->GetTag();

			length+=(attr->GetValueLength()
			+fixedlength(&commandsyntax,attr));
			next();
		}
		if (length) {
			// last group really existed, so
			// make & add new length element
			DC3Tag newtag(
				DC3GroupLength_GROUP,
				DC3GroupLength_ELEMENT);
			DC3UnsignedLong *ulp = new 
			    DC3UnsignedLong(newtag,4,&length);
			insert(ulp);
		}
	}
}

void
DC3ListofAttributes::PrivateRemove(void)
{
	first();
	while (ismore()) {
		DC3Attribute *attr = value();
		DC3Tag tag = attr->GetTag();
		if (tag.isPrivateGroup()) remove(attr);
		next();
	}
}

// Delete any existing attributes in deletelist

void
DC3ListofAttributes::Delete(DC3ListofAttributes& deletelist)
{
	// NB. The exising attribute is removed (and hence destroyed)

	deletelist.first();
	while (deletelist.ismore()) {
		DC3Attribute *deleteattr = deletelist.value();
		DC3Tag tag = deleteattr->GetTag();
		DC3Attribute *oldattr = find(tag);
		if (oldattr) remove(oldattr);
		deletelist.next();
	}
}

// Replace any existing attributes new ones in replacelist

void
DC3ListofAttributes::Replace(DC3ListofAttributes& replacelist)
{
	// NB. The exising attribute is removed (and hence destroyed)
	//     The replacement attribute is inserted in the list
	//     BUT IS NOT COPIED so don't "remove" it from the
	//     replacelist !

	replacelist.first();
	while (replacelist.ismore()) {
		DC3Attribute *replaceattr = replacelist.value();
		DC3Tag tag = replaceattr->GetTag();
		DC3Attribute *oldattr = find(tag);
		if (oldattr) remove(oldattr);
		insert(replaceattr);
		replacelist.next();
	}
}

void
DC3ListofAttributes::put(ostream& stream)
{
	first();
	while (ismore()) {
		DC3Attribute *attr = value();
		attr->put(stream);
		next();
	}
}

void
DC3ListofAttributes::write(DC3ostream& stream)
{
	first();
	while (ismore()) {
		DC3Attribute *attr = value();
		attr->write(stream);
		next();
	}
}

// Single value routines ... find a tag or keyword and get value if only one

DC3Attribute *
DC3ListofAttributes::getSingleValuedAttribute(DC3Tag tag)
{
	DC3Attribute *attr=find(tag);
	if (attr) {
		attr->first();
		if (attr->ismore()) {
			DC3Attribute *firstattr=attr;
			attr->next();
			if (attr->ismore()) {
				char *keyword=
				    GlobalDataDictionary.GetKeyword(tag);
				cerr << "getSingleValuedAttribute: <"
				     << (keyword ? keyword : "")
				     << "> has more than one value\n"
				     << flush;
			}
			return firstattr;
		}
		else {
			char *keyword=GlobalDataDictionary.GetKeyword(tag);
			cerr << "getSingleValuedAttribute: <"
			     << (keyword ? keyword : "")
			     << "> attribute has no value\n"
			     << flush;
		}
	}
	else {
		char *keyword=GlobalDataDictionary.GetKeyword(tag);
		cerr << "getSingleValuedAttribute: <"
		     << (keyword ? keyword : "")
		     << "> no such attribute present\n" << flush;
	}

	return 0;
}


DC3Attribute *
DC3ListofAttributes::getSingleValuedAttribute(char *keyword)
{
	DC3Tag tag;
	if (GlobalDataDictionary.GetTag(keyword,tag) != -1) {
		return getSingleValuedAttribute(tag);
	}
	else {
		cerr << "getSingleValuedAttribute: Keyword <" 
		     << (keyword ? keyword : "")
		     << "> not in dictionary\n"
		     << flush;
	}
	return 0;
}

int
DC3ListofAttributes::getSingleValueXint(DC3Tag tag,Xint& value,int verbose)
{
	DC3Attribute *attr = getSingleValuedAttribute(tag);
	if (!attr) return -1;
	int success=attr->SingleValueNumeric(value);
	if (success == -1) {
		cerr << "getSingleValueXint: "
		     << "Empty or not numeric binary/string VR"
		     << "\n" << flush;
	}
	else {
		if (verbose) {
			char *keyword=GlobalDataDictionary.GetKeyword(tag);
			cerr << "getSingleValueXint: "
			     << (keyword ? keyword : "")
			     << " = " << value << "\n" << flush;
		}
	}
	return success;
}

int
DC3ListofAttributes::getSingleValueXint(char *keyword,Xint& value,int verbose)
{
	DC3Attribute *attr = getSingleValuedAttribute(keyword);
	if (!attr) return -1;
	int success=attr->SingleValueNumeric(value);
	if (success == -1) {
		cerr << "getSingleValueXint: "
		     << "Empty or not numeric binary/string VR"
		     << "\n" << flush;
	}
	else {
		if (verbose) {
			cerr << "getSingleValueXint: "
			     << (keyword ? keyword : "")
			     << " = " << value << "\n" << flush;
		}
	}
	return success;
}

int
DC3ListofAttributes::getSingleValueUint16(DC3Tag tag,Uint16& value,int verbose)
{
	Xint i;
	int success=getSingleValueXint(tag,i,verbose);
	if (success != -1) value=(Uint16)i;
	return success;
}

int
DC3ListofAttributes::getSingleValueUint16(char *keyword,Uint16& value,
				int verbose)
{
	Xint i;
	int success=getSingleValueXint(keyword,i,verbose);
	if (success != -1) value=(Uint16)i;
	return success;
}

int
DC3ListofAttributes::getSingleValueStringCopy(DC3Tag tag,char * &value,int verbose)
{
	DC3Attribute *attr = getSingleValuedAttribute(tag);
	if (!attr) return -1;
	int success=attr->SingleValueStringCopy(value);
	if (success == -1) {
		cerr << "getSingleValueStringCopy: "
		     << "Empty or not string VR"
		     << "\n" << flush;
	}
	else {
		if (verbose) {
			char *keyword=GlobalDataDictionary.GetKeyword(tag);
			cerr << "getSingleValueStringCopy: "
			     << (keyword ? keyword : "")
			     << " = " << value << "\n" << flush;
		}
	}
	return success;
}

int
DC3ListofAttributes::getSingleValueStringCopy(char *keyword,char * &value,int verbose)
{
	DC3Attribute *attr = getSingleValuedAttribute(keyword);
	if (!attr) return -1;
	int success=attr->SingleValueStringCopy(value);
	if (success == -1) {
		cerr << "getSingleValueStringCopy: "
		     << "Empty or not string VR"
		     << "\n" << flush;
	}
	else {
		if (verbose) {
			cerr << "getSingleValueStringCopy: "
			     << (keyword ? keyword : "")
			     << " = " << value << "\n" << flush;
		}
	}
	return success;
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
