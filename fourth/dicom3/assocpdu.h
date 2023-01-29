/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* assocpdu.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */
#ifndef _assocpdu_HEADER_
#define _assocpdu_HEADER_

#include <iostream.h>
#include <string.h>

#include "glist.h"
#include "ttypes.h"
#include "pdutypes.h"
#include "transerv.h"

class AssociateItem {
	Uint16 lengthhead(void)	{ return 4; }
protected:
	unsigned char	ItemType;
	Uint16		ItemLength;
	void writehead(TransportServiceUser *Connection)
		{
			Connection->write8(ItemType);
			Connection->write8(0);		// Reserved
			Connection->write16(ItemLength);
		}
public:
	AssociateItem(void)	{}
	virtual void write(TransportServiceUser *Connection) = 0;
	virtual void put(ostream& stream) = 0;
	Uint16 length(void)
		{
			return ItemLength+lengthhead();
		}
};

class AssociateItemList		: public GENOrderedList<AssociateItem *> {
public:
	void write(TransportServiceUser *Connection)
		{
			first();
			while (ismore()) {
				value()->write(Connection);
				next();
			}
		}

	void put(ostream& stream);
};

class AssociatePDU {
	unsigned char		PDUType;
	Uint32			PDULength;
	Uint16			ProtocolVersion;
	char *			CalledAPTitle;
	char *			CallingAPTitle;
	AssociateItemList	subitems;

	Int32 parseitems(TransportServiceUser *Connection,Int32 remaining);
public:
	AssociatePDU(void)
		{
			PDUType=0;
			PDULength=0;
			ProtocolVersion=0;
			CalledAPTitle =0;
			CallingAPTitle=0;
		}
	AssociatePDU(char * calledapt,char * callingapt)
		{
			PDUType=DC3_A_ASSOCIATE_RQ_PDUType;
			PDULength=68;
			ProtocolVersion=0x0001;
			CalledAPTitle =calledapt;
			CallingAPTitle=callingapt;
		}

	void write(TransportServiceUser *Connection);

	Int32 read(TransportServiceUser *Connection,unsigned char pdutype);

	void put(ostream& stream);

	void add(AssociateItem *subitem)
		{
			subitems.insert(subitem);
			PDULength+=subitem->length();
		}
};

class ApplicationContextItem	: public AssociateItem {
	char *ApplicationContext;
public:
	ApplicationContextItem(void)
		: AssociateItem()
		{
			ApplicationContext=0;
			ItemType=0;
			ItemLength=0;
		}
	ApplicationContextItem(char *ac)
		: AssociateItem()
		{
			ApplicationContext=ac;	// should make copy ?
			ItemType=DC3ApplicationContextItemType;
			ItemLength=strlen(ApplicationContext);
		}
	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->writestr(ApplicationContext,ItemLength);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->readstr(&ApplicationContext,ItemLength);
			return ItemLength+2;
		}

	void put(ostream& stream);
};

class PresentationContextSubItem {
	Uint16 lengthhead(void)	{ return 4; }
protected:
	unsigned char	ItemType;
	Uint16		ItemLength;
	void writehead(TransportServiceUser *Connection)
		{
			Connection->write8(ItemType);
			Connection->write8(0);		// Reserved
			Connection->write16(ItemLength);
		}
public:
	PresentationContextSubItem(void) {}
	virtual void write(TransportServiceUser *Connection) = 0;
	virtual void put(ostream& stream) = 0;
	Uint16 length(void)
		{
			return ItemLength+lengthhead();
		}
};

class PresentationContextSubItemList
				: public GENOrderedList<
					PresentationContextSubItem *> {
public:
	void write(TransportServiceUser *Connection)
		{
			first();
			while (ismore()) {
				value()->write(Connection);
				next();
			}
		}

	void put(ostream& stream);
};

class PresentationContextItem	: public AssociateItem {
	unsigned char			PresentationContextID;
	unsigned char			ResultReason;
	PresentationContextSubItemList	subitems;

	Int32				parseitems(TransportServiceUser *Connection,Int32 remaining);
public:
	PresentationContextItem(void)
		: AssociateItem()
		{
			PresentationContextID=0;
			ResultReason=0;
			ItemType=0;
			ItemLength=0;
		}
	PresentationContextItem(unsigned char pcid)
		: AssociateItem()
		{
			PresentationContextID=pcid;
			ResultReason=0;
			ItemType=DC3PresentationContextRequestItemType;
			ItemLength=4;
		}
	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->write8(PresentationContextID);
			Connection->write8(0);		// Reserved
			Connection->write8(ResultReason);
			Connection->write8(0);		// Reserved
			subitems.write(Connection);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype);

	void put(ostream& stream);

	void add(PresentationContextSubItem *subitem)
		{
			subitems.insert(subitem);
			ItemLength+=subitem->length();
		}
};

class AbstractSyntaxItem	: public PresentationContextSubItem {
	char *	AbstractSyntax;
public:
	AbstractSyntaxItem(void)
		: PresentationContextSubItem()
		{
			AbstractSyntax=0;
			ItemType=0;
			ItemLength=0;
		}
	AbstractSyntaxItem(char *as)
		: PresentationContextSubItem()
		{
			AbstractSyntax=as;	// should make copy ?
			ItemType=DC3AbstractSyntaxItemType;
			ItemLength=strlen(AbstractSyntax);
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->writestr(AbstractSyntax,ItemLength);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->readstr(&AbstractSyntax,ItemLength);
			return ItemLength+2;
		}

	void put(ostream& stream);
};

class TransferSyntaxItem	: public PresentationContextSubItem {
	char *	TransferSyntax;
public:
	TransferSyntaxItem(void)
		: PresentationContextSubItem()
		{
			TransferSyntax=0;
			ItemType=0;
			ItemLength=0;
		}
	TransferSyntaxItem(char *ts)
		: PresentationContextSubItem()
		{
			TransferSyntax=ts;	// should make copy ?
			ItemType=DC3TransferSyntaxItemType;
			ItemLength=strlen(TransferSyntax);
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->writestr(TransferSyntax,ItemLength);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->readstr(&TransferSyntax,ItemLength);
			return ItemLength+2;
		}

	void put(ostream& stream);
};

class UserInfoSubItem {
	Uint16 lengthhead(void)	{ return 4; }
protected:
	unsigned char	ItemType;
	Uint16		ItemLength;
	void writehead(TransportServiceUser *Connection)
		{
			Connection->write8(ItemType);
			Connection->write8(0);		// Reserved
			Connection->write16(ItemLength);
		}
public:
	UserInfoSubItem(void) {}
	virtual void write(TransportServiceUser *Connection) = 0;
	virtual void put(ostream& stream) = 0;
	Uint16 length(void)
		{
			return ItemLength+lengthhead();
		}
};

class UserInfoSubItemList	: public GENOrderedList<UserInfoSubItem *> {
public:
	void write(TransportServiceUser *Connection)
		{
			first();
			while (ismore()) {
				value()->write(Connection);
				next();
			}
		}

	void put(ostream& stream);
};

class UserInfoItem		: public AssociateItem {
	UserInfoSubItemList	subitems;

	Int32 parseitems(TransportServiceUser *Connection,Int32 remaining);
public:
	UserInfoItem(void)
		: AssociateItem()
		{
			ItemType=DC3UserInfoItemType;
			ItemLength=0;
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			subitems.write(Connection);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype);

	void put(ostream& stream);

	void add(UserInfoSubItem *subitem)
		{
			subitems.insert(subitem);
			ItemLength+=subitem->length();
		}
};

class MaximumLengthItem		: public UserInfoSubItem {
	Uint32		MaximumLengthReceived;
public:
	MaximumLengthItem(void)
		: UserInfoSubItem()
		{
			MaximumLengthReceived=0;
			ItemType=0;
			ItemLength=0;
		}
	MaximumLengthItem(Uint32 mlr)
		: UserInfoSubItem()
		{
			MaximumLengthReceived=mlr;
			ItemType=DC3MaximumLengthItemType;
			ItemLength=4;
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->write32(MaximumLengthReceived);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->read32(&MaximumLengthReceived);
			return (validate() < 0) ? -1 : ItemLength+2;
		}

	void put(ostream& stream);

	int validate(void)
		{
			if (ItemLength != 4) {
				cerr << "MaximumLengthItem:read bad ItemLength "
				     << ItemLength << "\n" << flush;
				return -1;
			}
			else return 0;
		}
};

class ImplementationClassUIDItem	: public UserInfoSubItem {
	char *		ImplementationClassUID;
public:
	ImplementationClassUIDItem(void)
		: UserInfoSubItem()
		{
			ImplementationClassUID=0;
			ItemType=0;
			ItemLength=0;
		}
	ImplementationClassUIDItem(char *uid)
		: UserInfoSubItem()
		{
			ImplementationClassUID=uid;
			ItemType=DC3ImplementationClassUIDItemType;
			ItemLength=strlen(ImplementationClassUID);
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->writestr(ImplementationClassUID,ItemLength);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->readstr(&ImplementationClassUID,ItemLength);
			return ItemLength+2;
		}

	void put(ostream& stream);
};

class ImplementationVersionNameItem	: public UserInfoSubItem {
	char *		ImplementationVersionName;
public:
	ImplementationVersionNameItem(void)
		: UserInfoSubItem()
		{
			ImplementationVersionName=0;
			ItemType=0;
			ItemLength=0;
		}
	ImplementationVersionNameItem(char *name)
		: UserInfoSubItem()
		{
			ImplementationVersionName=name;
			ItemType=DC3ImplementationVersionNameItemType;
			ItemLength=strlen(ImplementationVersionName);
			(void)validate();
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->writestr(ImplementationVersionName,
				ItemLength);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->readstr(&ImplementationVersionName,
				ItemLength);
			return (validate() < 0) ? -1 : ItemLength+2;
		}

	void put(ostream& stream);

	int validate(void)
		{
			if (ItemLength > 16) {
				cerr << "ImplementationVersionNameItem:validate"
				     << " warning - name too long "
				     << ItemLength << "\n" << flush;
				return -1;
			}
			else return 0;
		}
};

class AsynchronousOperationsWindowItem		: public UserInfoSubItem {
	Uint16		MaximumNumberOperationsInvoked;
	Uint16		MaximumNumberOperationsPerformed;
public:
	AsynchronousOperationsWindowItem(void)
		: UserInfoSubItem()
		{
			MaximumNumberOperationsInvoked=0;
			MaximumNumberOperationsPerformed=0;
			ItemType=0;
			ItemLength=0;
		}
	AsynchronousOperationsWindowItem(Uint16 p,Uint16 i)
		: UserInfoSubItem()
		{
			MaximumNumberOperationsInvoked=i;
			MaximumNumberOperationsPerformed=p;
			ItemType=DC3AsynchronousOperationsWindowItemType;
			ItemLength=4;
		}
	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->write16(MaximumNumberOperationsInvoked);
			Connection->write16(MaximumNumberOperationsPerformed);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->read16(&MaximumNumberOperationsInvoked);
			Connection->read16(&MaximumNumberOperationsPerformed);
			return (validate() < 0) ? -1 : ItemLength+2;
		}

	void put(ostream& stream);

	int validate(void)
		{
			if (ItemLength != 4) {
				cerr << "AsynchronousOperationsWindowItem:read"
				     << " bad ItemLength "
				     << ItemLength << "\n" << flush;
				return -1;
			}
			else return 0;
		}
};

class SCUSCPRoleSelectionItem	: public UserInfoSubItem {
	char *		SOPClassUID;
	Uint16		SOPClassUIDLength;
	unsigned char	SCURole;
	unsigned char	SCPRole;
public:
	SCUSCPRoleSelectionItem(void)
		: UserInfoSubItem()
		{
			SOPClassUID=0;
			SOPClassUIDLength=0;
			SCURole=0;
			SCPRole=0;
			ItemType=0;
			ItemLength=0;
		}
	SCUSCPRoleSelectionItem(char *uid,unsigned char scu,unsigned char scp)
		: UserInfoSubItem()
		{
			SOPClassUID=uid;
			SOPClassUIDLength=strlen(SOPClassUID);
			SCURole=scu;
			SCPRole=scp;
			ItemType=DC3SCUSCPRoleSelectionItemType;
			ItemLength=SOPClassUIDLength+4;
		}

	void write(TransportServiceUser *Connection)
		{
			writehead(Connection);
			Connection->write16(SOPClassUIDLength);
			Connection->writestr(SOPClassUID,SOPClassUIDLength);
			Connection->write8(SCURole);
			Connection->write8(SCPRole);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char itemtype)
		{
			// ItemType & reserved byte already read
			ItemType=itemtype;
			Connection->read16(&ItemLength);
			Connection->read16(&SOPClassUIDLength);
			Connection->readstr(&SOPClassUID,SOPClassUIDLength);
			return (validate() < 0) ? -1 : ItemLength+2;
		}

	void put(ostream& stream);

	int validate(void)
		{
			if (ItemLength != SOPClassUIDLength+4) {
				cerr << "SCUSCPRoleSelectionItem:read"
				     << " bad lengths - ItemLength "
				     << ItemLength << " SOPClassUIDLength "
				     << SOPClassUIDLength << "\n" << flush;
				return -1;
			}
			else return 0;
		}
};

AssociatePDU *BuildAssociatePDU(char *CalledAE,char *CallingAE);

class AssociateRJPDU {
	unsigned char		PDUType;
	Uint32			PDULength;
	unsigned char		Result;
	unsigned char		Source;
	unsigned char		Reason;
public:
	AssociateRJPDU(void)
		{
			PDUType=0;
			PDULength=0;
			Result=0;
			Source=0;
			Reason=0;
		}
	AssociateRJPDU(unsigned char result,unsigned char source,
			unsigned char reason)
		{
			PDUType=DC3_A_ASSOCIATE_RJ_PDUType;
			PDULength=4;
			Result=result;
			Source=source;
			Reason=reason;
		}

	void write(TransportServiceUser *Connection)
		{
			Connection->write8(PDUType);
			Connection->write8(0);		// Reserved
			Connection->write32(PDULength);
			Connection->write8(0);		// Reserved
			Connection->write8(Result);
			Connection->write8(Source);
			Connection->write8(Reason);
		}

	Int32 read(TransportServiceUser *Connection,unsigned char pdutype)
		{
			// PDUType & reserved byte already read
			PDUType=pdutype;
			Connection->read32(&PDULength);
			Connection->readskip(1);	// Reserved
			Connection->read8(&Result);
			Connection->read8(&Source);
			Connection->read8(&Reason);
			return (validate() < 0) ? -1 : PDULength+4;
		}

	int validate(void)
		{
			if (PDULength != 4) {
				cerr << "AssociateRJPDU:read"
				     << " bad PDULength "
				     << PDULength << "\n" << flush;
				return -1;
			}
			else return 0;
		}

	void put(ostream& stream);
};

#endif /* _assocpdu_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
