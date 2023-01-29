/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tfstream.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tfstream_HEADER_
#define _DC3tfstream_HEADER_

#include <fstream.h>

#include "tstream.h"

// NB. Use explicit open modes ios::in or ios::out otherwise the Symantec
//     default is to use cr/lf translation !!!! (ios::translated) :(

class DC3ifstream : public DC3istream {
	ifstream *		stream;
	DC3TransferSyntax *	InitialTransferSyntax;
	DC3TransferSyntax *	MetaHeaderTransferSyntax;

	DC3TransferSyntax *	GuessTransferSyntax(ostream *verbose);
public:
	DC3ifstream(const char* name,const char *ts,ostream *verbose=0)
		: DC3istream()
		{
			stream = new ifstream(name,ios::in|ios::nocreate);
			// NB. does NOT set bad if open fails until 1st read
			// therefore need to check filebuf->is_open() ...
			if (ts)
				InitialTransferSyntax = 
					new DC3TransferSyntax(ts);
			else
				InitialTransferSyntax = 
					GuessTransferSyntax(verbose);
			DC3istream::SetTransferSyntax(InitialTransferSyntax);
			MetaHeaderTransferSyntax = 0;
		}

	~DC3ifstream(void)
		{
			if (stream) delete stream;
			if (InitialTransferSyntax)
				delete InitialTransferSyntax;
			if (MetaHeaderTransferSyntax)
				delete MetaHeaderTransferSyntax;
		}

	int HaveMetaHeaderSpecifiedTransferSyntax(void)
		{
			return MetaHeaderTransferSyntax != 0;
		}
	void SetMetaHeaderSpecifiedTransferSyntax(DC3TransferSyntax *ts)
		{
			MetaHeaderTransferSyntax=ts;
		}
	void SwitchToMetaHeaderSpecifiedTransferSyntax(void)
		{
			if (MetaHeaderTransferSyntax) {
				DC3istream::SetTransferSyntax(
					MetaHeaderTransferSyntax);
			}
		}

	int CanUseMetaHeaderSpecifiedTransferSyntax(void)	{ return 1; }

	void close(void)	{ if (stream) stream->close(); }
	void clear(void)	{ if (stream) stream->clear(); }
	int  good(void)		{ return stream != 0 && stream->good(); }
	int  is_open(void)	{ return stream != 0
					&& stream->rdbuf()->is_open(); }
	int  eof(void)		{ return stream == 0 || stream->eof(); }

	int CanReposition(void)	{ return 1; }

	void unread(long n)
		{
			rdbuf()->seekoff(n,ios::cur,ios::in|ios::out);
		}

	filebuf * rdbuf(void)	{ return stream ? stream->rdbuf() : 0; }

	void read(char *p,unsigned l)	{ if (stream) stream->read(p,l); }
	void read(unsigned char *p,unsigned l)	{ read((char *)p,l); }
};

class DC3ofstream : public DC3ostream {
	ofstream *		stream;
	DC3TransferSyntax *	MetaHeaderTransferSyntax;
	DC3TransferSyntax *	NormalTransferSyntax;
	int			PreambleDone;
public:
	DC3ofstream(const char* name, const char *ts)
		{
			stream = new ofstream(name,ios::out);
			// NB. does NOT set bad if open fails until 1st read
			// therefore need to check filebuf->is_open() ...
			MetaHeaderTransferSyntax =
				new DC3TransferSyntax(DC3MetaHeaderTransferSyntax);
			NormalTransferSyntax =
				new DC3TransferSyntax(ts);
			DC3ostream::SetTransferSyntax(NormalTransferSyntax);
			PreambleDone = 0;
		}
	~DC3ofstream(void)
		{
			if (stream) delete stream;
			if (MetaHeaderTransferSyntax)
				delete MetaHeaderTransferSyntax;
			if (NormalTransferSyntax)
				delete NormalTransferSyntax;
		}

	void UseNormalTransferSyntax(void)
		{
			DC3ostream::SetTransferSyntax(NormalTransferSyntax);
		}

	void UseMetaHeaderTransferSyntax(void)
		{
			DC3ostream::SetTransferSyntax(MetaHeaderTransferSyntax);
			if (!PreambleDone) {
				write("DICM",4);
				unsigned i;
				for (i=0;i<124;++i) write(" ",1);
				write("DICM",4);
				PreambleDone=1;
			}
		}

	void close(void)	{ if (stream) stream->close(); }
	void clear(void)	{ if (stream) stream->clear(); }
	int  good(void)		{ return stream != 0 && stream->good(); }
	int  is_open(void)	{ return stream != 0
					&& stream->rdbuf()->is_open(); }
	int  eof(void)		{ return stream == 0 || stream->eof(); }

	void write(const char *p,unsigned l)
				{ if (stream) stream->write(p,l); }
	void write(const unsigned char *p,unsigned l)
				{ write((const char *)p,l); }
};

#endif /* _DC3tfstream_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
