/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)uplayer.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }
#include "uplayer.h"
#include "assocpdu.h"

int
AssociationUser::A_Associate(void)
{
	Connection->connect();

	AssociatePDU *A_Associate_RQ_PDU = 
		BuildAssociatePDU(CalledAE,CallingAE);
	cerr << "Proposed A_ASSOCIATE_RQ\n" << flush;
	A_Associate_RQ_PDU->put(cerr);
	cerr << "Sending A_ASSOCIATE_RQ\n" << flush;
	A_Associate_RQ_PDU->write(Connection);

	unsigned char pdutype;
	Connection->read8(&pdutype);
	Connection->readskip(1);

	switch(pdutype) {
		case DC3_A_ASSOCIATE_RQ_PDUType:
			cerr << "Received A_ASSOCIATE_RQ\n" << flush;
			return 1;
		case DC3_A_ASSOCIATE_AC_PDUType:
			cerr << "Received A_ASSOCIATE_AC\n" << flush;
			{
			AssociatePDU *A_Associate_AC_PDU=new AssociatePDU();
			(void)A_Associate_AC_PDU->read(Connection,pdutype);
			A_Associate_AC_PDU->put(cerr);
			}
			return 0;
		case DC3_A_ASSOCIATE_RJ_PDUType:
			cerr << "Received A_ASSOCIATE_RJ\n" << flush;
			{
			AssociateRJPDU *A_Associate_RJ_PDU=new AssociateRJPDU();
			(void)A_Associate_RJ_PDU->read(Connection,pdutype);
			A_Associate_RJ_PDU->put(cerr);
			}
			return 1;
		case DC3_P_DATA_TF_PDUType:
			cerr << "Received P_DATA_TF\n" << flush;
			return 1;
		case DC3_A_RELEASE_RQ_PDUType:
			cerr << "Received A_RELEASE_RQ\n" << flush;
			return 1;
		case DC3_A_RELEASE_RP_PDUType:
			cerr << "Received A_RELEASE_RP\n" << flush;
			return 1;
		case DC3_A_ABORT_PDUType:
			cerr << "Received A_ABORT\n" << flush;
			return 1;
		default:
			cerr << "Unrecognized PDU type " 
			     << (unsigned)pdutype << "\n" << flush;
			return 1;
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
