/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)command.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }
#include "command.h"

#define MakeDC3Tag(aname) \
		DC3Tag aname##tag(DC3##aname##_GROUP,DC3##aname##_ELEMENT);

#define MakeDC3Attribute(attr,atype,aname,vl,v) \
		MakeDC3Tag(aname); \
		DC3##atype *attr = new DC3##atype(aname##tag,vl,v);

DC3Command_C_STORE_RQ::DC3Command_C_STORE_RQ(
		char *AffectedSOPClassUID,
		Uint16 MessageID,
		Uint16 Priority,
		Uint16 DataSetType,
		char *AffectedSOPInstanceUID,
		char *MoveOriginatorApplicationEntityTitle,
		Uint16 MoveOriginatorMessageID,
		char *MessageSetID,
		char *EndMessageSet
	)
{
	{
		MakeDC3Attribute(a,UIString,AffectedSOPClassUID,
			strlen(AffectedSOPClassUID),AffectedSOPClassUID);
		list.insert(a);
	}
	{
		Uint16 command=DC3CommandField_C_STORE_RQ;
		MakeDC3Attribute(a,UnsignedShort,CommandField,2,&command);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,UnsignedShort,MessageID,2,&MessageID);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,UnsignedShort,Priority,2,&Priority);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,UnsignedShort,DataSetType,2,&DataSetType);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,UIString,AffectedSOPInstanceUID,
			strlen(AffectedSOPInstanceUID),AffectedSOPInstanceUID);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,ApplicationEntity,
			MoveOriginatorApplicationEntityTitle,
			strlen(MoveOriginatorApplicationEntityTitle),
			MoveOriginatorApplicationEntityTitle);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,UnsignedShort,MoveOriginatorMessageID,
			2,&MoveOriginatorMessageID);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,ShortString,MessageSetID,
			strlen(MessageSetID),MessageSetID);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,ShortString,EndMessageSet,
			strlen(EndMessageSet),EndMessageSet);
		list.insert(a);
	}
	list.FixCommand();
}

DC3Command_C_ECHO_RQ::DC3Command_C_ECHO_RQ(
		char *AffectedSOPClassUID,
		Uint16 MessageID
	)
{
	{
		MakeDC3Attribute(a,UIString,AffectedSOPClassUID,
			strlen(AffectedSOPClassUID),AffectedSOPClassUID);
		list.insert(a);
	}
	{
		Uint16 command=DC3CommandField_C_ECHO_RQ;
		MakeDC3Attribute(a,UnsignedShort,CommandField,2,&command);
		list.insert(a);
	}
	{
		MakeDC3Attribute(a,UnsignedShort,MessageID,2,&MessageID);
		list.insert(a);
	}
	{
		Uint16 nodatasetpresent=0x0101;
		MakeDC3Attribute(a,UnsignedShort,DataSetType,
			2,&nodatasetpresent);
		list.insert(a);
	}
	list.FixCommand();
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
