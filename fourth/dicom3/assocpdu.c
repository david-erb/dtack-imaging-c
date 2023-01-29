/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)assocpdu.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }
#include <iostream.h>

#include "assocpdu.h"
#include "tsynd.h"
#include "tabstd.h"

AssociatePDU *
BuildAssociatePDU(char *CalledAE,char *CallingAE)
{

	AssociatePDU *apdu = new AssociatePDU(CalledAE,CallingAE);
	ApplicationContextItem *aci = 
		new ApplicationContextItem("1.2.840.10008.3.1.1.1");
		// Part 7 A.2.1
	apdu->add(aci);

	unsigned numpci=0;
	{
		DC3AbstractSyntaxTableEntry *p;
		for (p=DC3AbstractSyntaxTable; p->UID; ++p) {
			if (p->Supported) {
				PresentationContextItem *pci = 
					new PresentationContextItem(++numpci);
				pci->add(new AbstractSyntaxItem(p->UID));
				{
					DC3TransferSyntaxTableEntry *p;
					for (p=DC3TransferSyntaxTable; 
					    p->UID; ++p) {
						if (p->Supported) {
							pci->add(new 
						    TransferSyntaxItem(p->UID));
						}
					}
				}
				apdu->add(pci);
			}
		}
	}
	UserInfoItem *uii=new UserInfoItem();
	uii->add(new ImplementationClassUIDItem("0.0.0.0.1"));
	apdu->add(uii);
	return apdu;
}

void
AssociatePDU::write(TransportServiceUser *Connection)
{
	Connection->write8(PDUType);
	Connection->write8(0);		// Reserved
	Connection->write32(PDULength);
	Connection->write16(ProtocolVersion);
	Connection->write16(0);		// Reserved
	Connection->writepad(CalledAPTitle,
		strlen(CalledAPTitle),16,' ');
	Connection->writepad(CallingAPTitle,
		strlen(CallingAPTitle),16,' ');
	Connection->writezero(32);	// Reserved
	subitems.write(Connection);
}

Int32
AssociatePDU::read(TransportServiceUser *Connection,unsigned char pdutype)
{
	// PDUType & reserved byte already read
	PDUType=pdutype;
	Connection->read32(&PDULength);
	Connection->read16(&ProtocolVersion);
	Connection->readskip(2);		// Reserved
	Connection->readstr(&CalledAPTitle,16);
	Connection->readstr(&CallingAPTitle,16);
	Connection->readskip(32);		// Reserved
	Int32 remaining=PDULength-68;
	Int32 done;
	if ((done=parseitems(Connection,remaining)) < 0) {
		cerr << "AssociatePDU:read parseitems failed\n";
		return -1;
	}
	return done+72;
}

Int32
AssociatePDU::parseitems(TransportServiceUser *Connection,Int32 remaining)
{
	Int32 done=0;
	while (remaining > done) {
		AssociateItem *item=0;
		Int32 idone;
		unsigned char itemtype;
		Connection->read8(&itemtype);
		Connection->readskip(1);		// Reserved
		done+=2;
		switch(itemtype) {
			case DC3ApplicationContextItemType:
				{
				ApplicationContextItem *aci = 
					new ApplicationContextItem();
				idone=aci->read(Connection,itemtype);
				item=aci;
				}
				break;
			case DC3PresentationContextAcceptItemType:
				{
				PresentationContextItem *pci = 
					new PresentationContextItem();
				idone=pci->read(Connection,itemtype);
				item=pci;
				}
				break;
			case DC3UserInfoItemType:
				{
				UserInfoItem *uii = 
					new UserInfoItem();
				idone=uii->read(Connection,itemtype);
				item=uii;
				}
				break;
			default:
				{
				Uint16 itemlength;
				Connection->read16(&itemlength);
				cerr << "AssociatePDU::parseitems "
				     << "Unexpected item type " 
				     << (unsigned)itemtype 
				     << " - skipping " << itemlength
				     << "\n" << flush;
				Connection->readskip(itemlength);
				idone=2+itemlength;
				}
				break;
		}
		if (idone < 0) {
			cerr << "ApplicationContextItem:parseitems "
			     << "read failed\n" << flush;
			return -1;
		}
		done+=idone;
		if (item) add(item);
	}

	if (remaining != done) {
		cerr << "ApplicationContextItem:parseitems length off by "
		     << remaining-done << " bytes\n";
		return -1;
	}
	return done;
}

Int32
PresentationContextItem::read(TransportServiceUser *Connection,unsigned char itemtype)
{
	// ItemType & reserved byte already read
	ItemType=itemtype;
	Connection->read16(&ItemLength);
	Connection->read8(&PresentationContextID);
	Connection->readskip(1);		// Reserved
	Connection->read8(&ResultReason);
	Connection->readskip(1);		// Reserved
	Int32 remaining=ItemLength-4;
	Int32 done;
	if ((done=parseitems(Connection,remaining)) < 0) {
		cerr << "PresentationContextItem:read parseitems failed\n";
		return -1;
	}
	return done+6;
}

Int32
PresentationContextItem::parseitems(TransportServiceUser *Connection,Int32 remaining)
{
	Int32 done=0;
	while (remaining > done) {
		PresentationContextSubItem *item=0;
		Int32 idone;
		unsigned char itemtype;
		Connection->read8(&itemtype);
		Connection->readskip(1);		// Reserved
		done+=2;
		switch(itemtype) {
			case DC3AbstractSyntaxItemType:
				{
				AbstractSyntaxItem *asi = 
					new AbstractSyntaxItem();
				idone=asi->read(Connection,itemtype);
				item=asi;
				}
				break;
			case DC3TransferSyntaxItemType:
				{
				TransferSyntaxItem *tsi = 
					new TransferSyntaxItem();
				idone=tsi->read(Connection,itemtype);
				item=tsi;
				}
				break;
			default:
				{
				Uint16 itemlength;
				Connection->read16(&itemlength);
				cerr << "PresentationContextItem::parseitems "
				     << "Unexpected item type " 
				     << (unsigned)itemtype 
				     << " - skipping " << itemlength
				     << "\n" << flush;
				Connection->readskip(itemlength);
				idone=2+itemlength;
				}
				break;
		}
		if (idone < 0) {
			cerr << "PresentationContextItem:parseitems "
			     << "read failed\n" << flush;
			return -1;
		}
		done+=idone;
		if (item) add(item);
	}

	if (remaining != done) {
		cerr << "PresentationContextItem:parseitems length off by "
		     << remaining-done << " bytes\n";
		return -1;
	}
	return done;
}

Int32
UserInfoItem::read(TransportServiceUser *Connection,unsigned char itemtype)
{
	// ItemType & reserved byte already read
	ItemType=itemtype;
	Connection->read16(&ItemLength);
	Int32 remaining=ItemLength;
	Int32 done;
	if ((done=parseitems(Connection,remaining)) < 0) {
		cerr << "UserInfoItem:read parseitems failed\n";
		return -1;
	}
	return done+2;
}

Int32
UserInfoItem::parseitems(TransportServiceUser *Connection,Int32 remaining)
{
	Int32 done=0;
	while (remaining > done) {
		UserInfoSubItem *item=0;
		Int32 idone;
		unsigned char itemtype;
		Connection->read8(&itemtype);
		Connection->readskip(1);		// Reserved
		done+=2;
		switch(itemtype) {
			case DC3MaximumLengthItemType:
				{
				MaximumLengthItem *mli = 
					new MaximumLengthItem();
				idone=mli->read(Connection,itemtype);
				item=mli;
				}
				break;
			case DC3ImplementationClassUIDItemType:
				{
				ImplementationClassUIDItem *icu = 
					new ImplementationClassUIDItem();
				idone=icu->read(Connection,itemtype);
				item=icu;
				}
				break;
			case DC3AsynchronousOperationsWindowItemType:
				{
				AsynchronousOperationsWindowItem *aow = 
					new AsynchronousOperationsWindowItem();
				idone=aow->read(Connection,itemtype);
				item=aow;
				}
				break;
			case DC3SCUSCPRoleSelectionItemType:
				{
				SCUSCPRoleSelectionItem *ssi = 
					new SCUSCPRoleSelectionItem();
				idone=ssi->read(Connection,itemtype);
				item=ssi;
				}
				break;
			case DC3ImplementationVersionNameItemType:
				{
				ImplementationVersionNameItem *ivn = 
					new ImplementationVersionNameItem();
				idone=ivn->read(Connection,itemtype);
				item=ivn;
				}
				break;
			default:
				{
				Uint16 itemlength;
				Connection->read16(&itemlength);
				cerr << "UserInfoItem::parseitems "
				     << "Unexpected item type " 
				     << (unsigned)itemtype 
				     << " - skipping " << itemlength
				     << "\n" << flush;
				Connection->readskip(itemlength);
				idone=2+itemlength;
				}
				break;
		}
		if (idone < 0) {
			cerr << "UserInfoItem:parseitems "
			     << "read failed\n" << flush;
			return -1;
		}
		done+=idone;
		if (item) add(item);
	}

	if (remaining != done) {
		cerr << "UserInfoItem:parseitems length off by "
		     << remaining-done << " bytes\n";
		return -1;
	}
	return done;
}

void
AssociateRJPDU::put(ostream& stream)
{
	char *resultstr=0;
	char *sourcestr=0;
	char *reasonstr=0;
	switch (Result) {
		case 1:	resultstr="Rejected Permanent";
			break;
		case 2:	resultstr="Rejected Transient";
			break;
		default:
			resultstr="Result value unrecognized";
			break;
	}
	switch (Source) {
		case 1:	sourcestr="DICOM UL Service User";
			switch (Reason) {
				case 1:	reasonstr="None Given";
					break;
				case 2:	reasonstr=
					"Application Context Name Unsupported";
					break;
				case 3:	reasonstr=
					"Calling AP Title Unrecognized";
					break;
				case 7:	reasonstr=
					"Called AP Title Unrecognized";
					break;
				default:
					reasonstr="Reason value unrecognized";
					break;
			}
			break;
		case 2:	sourcestr="DICOM UL Service Provider (ACSE Related)";
			switch (Reason) {
				case 1:	reasonstr="None Given";
					break;
				case 2:	reasonstr=
					"Protocol Version Unsupported";
					break;
				default:
					reasonstr="Reason value unrecognized";
					break;
			}
			break;
		case 3:	sourcestr=
			"DICOM UL Service Provider (Presentation Related)";
			switch (Reason) {
				case 1:	reasonstr="Temporary Congestion";
					break;
				case 2:	reasonstr="Local Limit Exceeded";
					break;
				default:
					reasonstr="Reason value unrecognized";
					break;
			}
			break;
		default:
			resultstr="Result value unrecognized";
			reasonstr="Reason value unrecognized";
			break;
	}

	stream << "A_ASSOCIATE_RJ PDU:\n";
	stream << "\tPDUType =\t" << PDUType << " decimal\n";
	stream << "\tPDULength =\t" << PDULength << " decimal\n";
	stream << "\tResult =\t" << (unsigned)Result 
	       << " -> " << resultstr << "\n";
	stream << "\tSource =\t" << (unsigned)Source 
	       << " -> " << sourcestr << "\n";
	stream << "\tReason =\t" << (unsigned)Reason 
	       << " -> " << reasonstr << "\n";
	stream << flush;
}

void
AssociatePDU::put(ostream& stream)
{
	stream << "A_ASSOCIATE_" 
	       << ((PDUType == DC3_A_ASSOCIATE_RQ_PDUType) ? "RQ" : "AC")
	       << " PDU:\n";
	stream << "\tPDUType   =\t\t" << (unsigned)PDUType << " decimal\n";
	stream << "\tPDULength =\t\t" << PDULength << " decimal\n";
	stream << "\tProtocolVersion =\t" << ProtocolVersion << " decimal\n";
	stream << "\tCalledAPTitle  =\t" << CalledAPTitle << "\n";
	stream << "\tCallingAPTitle =\t" << CallingAPTitle << "\n";
	stream << "\tVariable Field:\n";
	stream << flush;

	subitems.put(stream);
}

void
AssociateItemList::put(ostream& stream)
{
	first();
	while (ismore()) {
		value()->put(stream);
		next();
	}
}

void
ApplicationContextItem::put(ostream& stream)
{
	stream << "\tApplication Context Item:\n" << flush;
	stream << "\t\tItemType   =\t\t" << (unsigned)ItemType << " decimal\n" << flush;
	stream << "\t\tItemLength =\t\t" << ItemLength << " decimal\n" << flush;
	stream << "\t\tApplicationContext =\t" << ApplicationContext << "\n" << flush;
	stream << flush;
}

void
PresentationContextItem::put(ostream& stream)
{
	char *reasonstr=0;
	switch (ResultReason) {
		case 0:	reasonstr="Acceptance";
			break;
		case 1:	reasonstr="User Rejection";
			break;
		case 2:	reasonstr="No Reason (Provider Rejection)";
			break;
		case 3:	reasonstr=
			"Abstract Syntax Unsupported (Provider Rejection)";
			break;
		case 4:	reasonstr=
			"Transfer Syntaxes Unsupported (Provider Rejection)";
			break;
		default:
			reasonstr="Result/reason value unrecognized";
			break;
	}
	stream << "\tPresentation Context Item:\n";
	stream << "\t\tItemType   =\t\t" << (unsigned)ItemType << " decimal\n";
	stream << "\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\tPresentationContextID =\t" 
	       << (unsigned)PresentationContextID << " decimal\n";
	stream << "\t\tResultReason =\t\t" << (unsigned)ResultReason 
	       << " -> " << reasonstr << "\n";
	stream << "\t\tVariable Field:\n";
	stream << flush;

	subitems.put(stream);
}

void
PresentationContextSubItemList::put(ostream& stream)
{
	first();
	while (ismore()) {
		value()->put(stream);
		next();
	}
}

void
AbstractSyntaxItem::put(ostream& stream)
{
	stream << "\t\tAbstract Syntax Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tAbstractSyntax =\t" << AbstractSyntax << "\n";
	struct DC3AbstractSyntaxTableEntry *ptr = DC3AbstractSyntaxTable;
	while (ptr->UID) {
		if (strcmp(ptr->UID,AbstractSyntax) == 0) {
			stream << "\t\t\t\t\t\t(" << ptr->Description << ")\n";
			break;
		}
		++ptr;
	}
	stream << flush;
}

void
TransferSyntaxItem::put(ostream& stream)
{
	stream << "\t\tTransfer Syntax Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tTransferSyntax =\t" << TransferSyntax << "\n";
	struct DC3TransferSyntaxTableEntry *ptr = DC3TransferSyntaxTable;
	while (ptr->UID) {
		if (strcmp(ptr->UID,TransferSyntax) == 0) {
			stream << "\t\t\t\t\t\t(" << ptr->Description << ")\n";
			break;
		}
		++ptr;
	}
	stream << flush;
}

void
UserInfoItem::put(ostream& stream)
{
	stream << "\tUser Info Item:\n";
	stream << "\t\tVariable Field:\n";
	stream << flush;

	subitems.put(stream);
}

void
UserInfoSubItemList::put(ostream& stream)
{
	first();
	while (ismore()) {
		value()->put(stream);
		next();
	}
}

void
MaximumLengthItem::put(ostream& stream)
{
	stream << "\t\tMaximum Length Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tMaximumLengthReceived =\t" 
	       << MaximumLengthReceived << " decimal\n";
	stream << flush;
}

void
ImplementationClassUIDItem::put(ostream& stream)
{
	stream << "\t\tImplementation Class UID Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tImplementationClassUID =\t" 
	       << ImplementationClassUID << "\n";
	stream << flush;
}

void
ImplementationVersionNameItem::put(ostream& stream)
{
	stream << "\t\tImplementation Version Name Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tImplementationVersionName =\t" 
	       << ImplementationVersionName << "\n";
	stream << flush;
}

void
AsynchronousOperationsWindowItem::put(ostream& stream)
{
	stream << "\t\tAsynchronous Operations Window Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tMaximumNumberOperationsInvoked =\t" 
	       << MaximumNumberOperationsInvoked << " decimal\n";
	stream << "\t\t\tMaximumNumberOperationsPerformed =\t" 
	       << MaximumNumberOperationsPerformed << " decimal\n";
	stream << flush;
}

void
SCUSCPRoleSelectionItem::put(ostream& stream)
{
	stream << "\t\tSCU/SCP Role Selection Item:\n";
	stream << "\t\t\tItemType   =\t\t" 
	       << (unsigned)ItemType << " decimal\n";
	stream << "\t\t\tItemLength =\t\t" << ItemLength << " decimal\n";
	stream << "\t\t\tSOPClassUIDLength =\t" 
	       << SOPClassUIDLength << " decimal\n";
	stream << "\t\t\tSOPClassUID =\t\t" 
	       << SOPClassUID << "\n";
	stream << "\t\t\tSCURole =\t\t" 
	       << (unsigned)SCURole << " decimal\n";
	stream << "\t\t\tSCPRole =\t\t" 
	       << (unsigned)SCPRole << " decimal\n";
	stream << flush;
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
