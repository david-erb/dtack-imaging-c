/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tstream.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tstream_HEADER_
#define _DC3tstream_HEADER_

#include <fstream.h>	// ugly++ - for virtual filebuf * rdbuf(void) = 0;
			// (SC2 has filebuf in fstream.h, g++ in streambuf.h)

#include "ttypes.h"
#include "tsyn.h"
#include "tsync.h"

class DC3istream {
	DC3TransferSyntax *	TransferSyntax;
	unsigned char 		buffer[4];
protected:
	void SetTransferSyntax(DC3TransferSyntax *transfersyntax)
		{
			TransferSyntax=transfersyntax;
		}
public:
	DC3istream(void)
		{
			TransferSyntax = 0;
		}

	virtual ~DC3istream(void) {}

	DC3TransferSyntax *GetTransferSyntax(void)
		{
			return TransferSyntax;
		}

	virtual int HaveMetaHeaderSpecifiedTransferSyntax(void) = 0;
	virtual void SetMetaHeaderSpecifiedTransferSyntax(
			DC3TransferSyntax *ts) = 0;
	virtual void SwitchToMetaHeaderSpecifiedTransferSyntax(void) = 0;
	virtual int CanUseMetaHeaderSpecifiedTransferSyntax(void) = 0;

	virtual void close(void) = 0;
	virtual void clear(void) = 0;
	virtual int  good(void) = 0;
	virtual int  eof(void) = 0;

	virtual int  CanReposition(void) = 0;
	virtual void unread(long n) = 0;
	virtual filebuf * rdbuf(void) = 0;  // ugly++ - #include <streambuf.h>

	virtual void read(char *p,unsigned l) = 0;
	virtual void read(unsigned char *p,unsigned l) = 0;

	unsigned char read8(void)
		{
			unsigned char u;
			read(buffer,1);
			u = (unsigned char)buffer[0];
			return u;
		}
	Uint16 read16(void)
		{
			Uint16 u;
			read(buffer,2);
			if (TransferSyntax->isBigEndian()
			 || TransferSyntax->isBigBadEndian()) {
				u =  (Uint16)buffer[0];
				u <<= 8;
				u |= (Uint16)buffer[1];
			}
			else {
				u =  (Uint16)buffer[1];
				u <<= 8;
				u |= (Uint16)buffer[0];
			}
			return u;
		}
	Uint32 read32(void)
		{
			Uint32 u;
			read(buffer,4);
			if (TransferSyntax->isBigEndian()) {
				u =  (Uint32)buffer[0];
				u <<= 8;
				u |= (Uint32)buffer[1];
				u <<= 8;
				u |= (Uint32)buffer[2];
				u <<= 8;
				u |= (Uint32)buffer[3];
			}
			else if (TransferSyntax->isBigBadEndian()) {
				u =  (Uint32)buffer[2];
				u <<= 8;
				u |= (Uint32)buffer[3];
				u <<= 8;
				u |= (Uint32)buffer[0];
				u <<= 8;
				u |= (Uint32)buffer[1];
			}
			else {
				u =  (Uint32)buffer[3];
				u <<= 8;
				u |= (Uint32)buffer[2];
				u <<= 8;
				u |= (Uint32)buffer[1];
				u <<= 8;
				u |= (Uint32)buffer[0];
			}
			return u;
		}
};

class DC3ostream {
	DC3TransferSyntax *	TransferSyntax;
	unsigned char 		buffer[4];
protected:
	void SetTransferSyntax(DC3TransferSyntax *transfersyntax)
		{
			TransferSyntax=transfersyntax;
		}
public:
	DC3ostream(void)
		{
			TransferSyntax = 0;
		}

	virtual ~DC3ostream(void) {}

	DC3TransferSyntax *GetTransferSyntax(void)
		{
			return TransferSyntax;
		}

	virtual void UseNormalTransferSyntax(void) = 0;
	virtual void UseMetaHeaderTransferSyntax(void) = 0;

	virtual void close(void) = 0;
	virtual void clear(void) = 0;
	virtual int  good(void) = 0;
	virtual int  eof(void) = 0;

	virtual void write(const char *p,unsigned l) = 0;
	virtual void write(const unsigned char *p,unsigned l)	= 0;

	void write8(unsigned char u)
		{
			buffer[0]=(unsigned char)u;
			write(buffer,1);
		}
	void write16(Uint16 u)
		{
			if (TransferSyntax->isBigEndian()
			 || TransferSyntax->isBigBadEndian()) {
				buffer[0]=(unsigned char)(u>>8);
				buffer[1]=(unsigned char)u;
			}
			else {
				buffer[1]=(unsigned char)(u>>8);
				buffer[0]=(unsigned char)u;
			}
			write(buffer,2);
		}
	void write32(Uint32 u)
		{
			if (TransferSyntax->isBigEndian()) {
				buffer[0]=(unsigned char)(u>>24);
				buffer[1]=(unsigned char)(u>>16);
				buffer[2]=(unsigned char)(u>>8);
				buffer[3]=(unsigned char)u;
			}
			else if (TransferSyntax->isBigBadEndian()) {
				buffer[2]=(unsigned char)(u>>24);
				buffer[3]=(unsigned char)(u>>16);
				buffer[0]=(unsigned char)(u>>8);
				buffer[1]=(unsigned char)u;
			}
			else {
				buffer[3]=(unsigned char)(u>>24);
				buffer[2]=(unsigned char)(u>>16);
				buffer[1]=(unsigned char)(u>>8);
				buffer[0]=(unsigned char)u;
			}
			write(buffer,4);
		}
};

#endif /* _DC3tstream_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
