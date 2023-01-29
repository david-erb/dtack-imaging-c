/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tnstream.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tnstream_HEADER_
#define _DC3tnstream_HEADER_

//#include <memory.h>	// for memcpy()

#include "tstream.h"
#include "transerv.h"

#ifdef CRAP
class DC3instream : public DC3istream {
public:
	DC3instream(const char *ts)
		: DC3istream()
		{
			DC3TransferSyntax *TS = new DC3TransferSyntax(ts);
			DC3istream::SetTransferSyntax(TS);
		}

	~DC3instream(void)
		{
			DC3TransferSyntax *TS = DC3istream::GetTransferSyntax();
			if (TS) delete TS;
		}

	int HaveMetaHeaderSpecifiedTransferSyntax(void)		{ return 0; }
	void SetMetaHeaderSpecifiedTransferSyntax(DC3TransferSyntax *ts) {}
	void SwitchToMetaHeaderSpecifiedTransferSyntax(void)	{}
	int CanUseMetaHeaderSpecifiedTransferSyntax(void)	{ return 0; }

	void close(void)	{}
	void clear(void)	{}
	int  good(void)		{ return 1; }
	int  eof(void)		{ return 0; }

	int CanReposition(void)	{ return 0; }

	void unread(long n)	{}
	filebuf * rdbuf(void)	{ return 0; }

	void read(char *p,unsigned l)	{ Deliberate Error }
	void read(unsigned char *p,unsigned l)	{ Deliberate Error }
};
#endif /* CRAP */

class DC3onstream : public DC3ostream {
	TransportServiceUser *	Connection;
	unsigned char	PresentationContextID;
	unsigned char	CommandOrDataFragment;	// 0=data,1=command
	char *		PduBuffer;
	char *		PduPtr;
	unsigned 	PduBufferLength;
public:
	DC3onstream(const char *ts,TransportServiceUser *connection,
		unsigned char pcid,unsigned maxlength=16384)
		{
			// needs allocation failure check
			DC3TransferSyntax *TS = new DC3TransferSyntax(ts);
			DC3ostream::SetTransferSyntax(TS);
			Connection=connection;
			PresentationContextID=pcid;
			CommandOrDataFragment=0;	// Default is data
			PduBufferLength=maxlength;
			PduBuffer=new char[PduBufferLength];
			PduPtr=PduBuffer;
		}

	~DC3onstream(void)
		{
			DC3TransferSyntax *TS = DC3ostream::GetTransferSyntax();
			if (TS) delete TS;
		}

	void UseNormalTransferSyntax(void)	{}
	void UseMetaHeaderTransferSyntax(void)	{}

	void close()		{}
	void clear()		{}
	int  good()		{ return 1; }
	int  eof()		{ return 0; }

	void write(const unsigned char *p,unsigned l)
		{ write((const char *)p,l); }

	void write(const char *p,unsigned l)
		{
			// should check or handle l > PduBufferLength ?
			if (PduPtr-PduBuffer+l > PduBufferLength) flush();
			memcpy(PduPtr,p,l);
			PduPtr+=l;
		}

	void flush(unsigned char lastfragment=0)	// Default is not last
		{
			int bufferlength=PduPtr-PduBuffer;

			// write P-DATA-TF PDU header
			Connection->write8(0x04);	// PDUType
			Connection->write8(0);		// Reserved
			Connection->write32(4+2+bufferlength);

			// write Presentation Data Value Item header
			Connection->write32(2+bufferlength);
			Connection->write8(PresentationContextID);

			// write Message Control Header
			unsigned char mch=0;
			mch|=(CommandOrDataFragment)	? 1 : 0;
			mch|=(lastfragment)		? 2 : 0;
			Connection->write8(mch);

			if (bufferlength > 0)
				Connection->writestr(PduBuffer,bufferlength);

			PduPtr=PduBuffer;
		}

	void SetFragmentType(unsigned char fragtype)	// 0=data,1=command
		{
			CommandOrDataFragment=fragtype;
		}
};

#endif /* _DC3tnstream_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
