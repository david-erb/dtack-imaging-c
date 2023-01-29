/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




static char *CopyrightIdentifier(void) { return "@(#)tattr.cc 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved."; }

#include <string.h>

#include "tattr.h"
#include "tddict.h"

/* ************************ General Routines ************************ */

void
DC3AttributeSkip(DC3istream& stream,Uint32 Length)
{
	// cerr << "Skipping " << dec << Length << " bytes\n" << flush;
	while (Length--) (void)stream.read8();
}

/* ************************ Methods ************************ */

void
DC3Attribute::put(ostream& stream)
{
	Tag.put(stream);
	stream << "\t VR=<" << ValueRepresentation() << ">  " << flush;
	stream << " VL=<" << hex << ValueLength << ">  " << flush;
}

void
DC3Attribute::write(DC3ostream& stream)
{
	Tag.write(stream);
	const char *vr = ValueRepresentation();
	if (stream.GetTransferSyntax()->isExplicitVR()) {
		if (vr) {
			stream.write(vr,2);
		}
		else {
			cerr << "DC3Attribute::write: Internal error - "
			     << "no known VR for attribute :(\n"
			     << flush;
			stream.write("  ",2);	// Just to keep stream in sync
		}
		if (vr[0]=='O' && (vr[1]=='B' || vr[1]=='W' || vr[1]=='X')
	 	 || vr[0]=='S' &&  vr[1]=='Q')
		{
			// Explicit OB,OW,SQ
			stream.write16(0);
			stream.write32(ValueLength);
		}
		else {
			stream.write16((Uint16)ValueLength);
		}
	}
	else {	// stream.GetTransferSyntax()->isImplicitVR()
		stream.write32(ValueLength);
	}
}

int
DC3Attribute::SingleValueNumeric(Xint& value)
{
	first();
	if (IsNumeric() || IsNumericString()) {
		value=ValueXint();
		return 0;
	}
	return -1;
}

int
DC3Attribute::SingleValueStringCopy(char * &value)
{
	first();
	if (ismore()) {
		// Almost anyone can be a string these days :)
		if (IsString()) {
			value=ValueStringCopy();
			return 0;
		}
		if (IsNumeric()) {
			value=ValueNumericStringCopy();
			return 0;
		}
	}
	return -1;
}

inline char *
strdupUnpadded(const char *from)	// leading and trailing blanks
{
	while (*from == ' ') ++from;
	unsigned length = strlen(from);
	while (length && from[length-1] == ' ') --length;
	char *to = new char [length+1];
	if (to) {
		strncpy(to,from,length);
		to[length]=0;
	}
	return to;
}

DC3ListofStrings::DC3ListofStrings(unsigned n,const char **s)
{
	// Create copies of the strings and make an array for pointers
	char **array = new char *[n];
	if (array) {
		unsigned i;
		for (i=0; i<n; ++i,++s) {
			char *p = strdupUnpadded(*s);
			if (p) {
				array[i]=p;
			}
			else {
				delete[] array;
				array=0;
				break;
			}
		}
	}
	u.strings=array;	// Null if couldn't alloc
	number=n;
}

DC3ListofStrings::DC3ListofStrings(const char *s)
{
	char *p = strdupUnpadded(s);
	u.string=p;		// Null if couldn't alloc
	number=1;
}


DC3ListofStrings::~DC3ListofStrings(void)
{
	switch (number) {
		case 0:		break;
		case 1:		if (u.string) delete[] u.string;
				break;
		default:	if (u.strings) {
					unsigned i;
					char *p;
					for (i=0; i<number; ++i) {
						p = u.strings [i];
						if (p) {
							delete[] p;
						}
					}
					delete[] u.strings;
				}
				break;
	};
}

void
DC3ListofStrings::add(const char *string)
{
	char *p = strdupUnpadded(string);
	if (number == 0) {
		u.string=p;
	}
	else if (number == 1) {
		char **newarray = new char *[number+1];
		newarray[0]=u.string;
		newarray[1]=p;
		u.strings=newarray;
	}
	else {
		char **newarray = new char *[number+1];
		unsigned i;
		for (i=0; i<number; ++i) newarray[i]=u.strings[i];
		newarray[number]=p;
		delete[] u.strings;
		u.strings=newarray;
	}
	++ number;
}

void
DC3ListofStrings::get(char *string)		// Get a '\\' delimited string
						// into u.string or u.strings
{
	unsigned length=strlen(string);
	if (length == 0) {
		number=0;
	}
	else {
		char *p;
		unsigned n;
		n=0; number=1; p=string;	// Count the delimited strings
		while (n<length) {
			if (*p == '\\') ++number;
			++p;
			++n;
		}
		if (number == 1) {
			u.string=strdupUnpadded(string);
		}
		else {
			char **array = new char *[number];
			char *start;
			unsigned index;
			n=0; index=0; start=string; p=string;
			// test is <= in order to allow for last component being null
			while (n<=length && index<number) {
				while (n<=length && *p != '\\') {
					++p; ++n;
				}
				if (n<=length) { *p='\0'; ++p; ++n; }
				array[index++]=strdupUnpadded(start);
				start=p;
			}
			u.strings=array;
		}
	}
}

void
DC3ListofStrings::read(DC3istream& stream,Uint32 length)
						// Read a '\\' delimited string
						// into u.string or u.strings
{
	if (length == 0) {
		number=0;
	}
	else {
		char *buffer = new char [length+1];
		stream.read(buffer,(int)length);
		buffer[length]=0;
		get(buffer);
		delete [] buffer;
	}
}

// NB. The presumption is that all padding is removed on reading/creating etc.
// and that all put, write & GetValueLength methods will have valid lengths
// after even padding themselves.

void
DC3ListofStrings::put(ostream& stream)
{
	stream << "<";
	DC3ListofStrings::putvalue(stream);
	stream << "> " << flush;
}

void
DC3ListofStrings::putvalue(ostream& stream)
{
	first();
	while (ismore()) {
		stream << value();
		next();
		if (ismore()) stream << "\\";
	}
	stream << flush;
}

void
DC3ListofStrings::write(DC3ostream& stream)
{
	Uint32 l=0;
	first();
	while (ismore()) {
		unsigned length=strlen(value());
		stream.write(value(),length);
		l+=length;
		next();
		if (ismore()) {
			stream.write("\\",1);
			++l;
		}
	}
	if (l % 2) stream.write(" ",1);	// All strings padded with ' '
						// except UID '\0'
}

Uint32
DC3ListofStrings::GetValueLength(void)
{
	Uint32 l=0;
	first();
	while (ismore()) {
		l+=strlen(value());
		next();
		if (ismore()) {
			++l;
		}
	}
	if (l % 2) ++l;			// All strings padded to even length
	return l;
}

void
DC3UIString::NullPaddedwrite(DC3ostream& stream)
{
	Uint32 l=0;
	first();
	while (ismore()) {
		unsigned length=strlen(value());
		stream.write(value(),length);
		l+=length;
		next();
		if (ismore()) {
			stream.write("\\",1);
			++l;
		}
	}
	if (l % 2) stream.write("",1);	// All strings padded with ' '
					// except UID '\0'
}

/* ************************ Tag Attribute Methods ************************ */

void
DC3AttributeTag::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/4);	// Two 16 bit integers (length is bytes)
		Tags=new DC3Tag[number];
		if (Tags) {
			unsigned i;
			for (i=0; i<number; ++i) Tags[i].read(stream);
		}
		// else what ?????
	}		
}

void
DC3AttributeTag::put(ostream& stream)
{
	DC3Attribute::put(stream);
	DC3AttributeTag::putvalue(stream);
	stream << "\n" << flush;
}

void
DC3AttributeTag::putvalue(ostream& stream)
{
	DC3ListofAThings<DC3Tag>::first();
	while (DC3ListofAThings<DC3Tag>::ismore()) {
		stream << "(" << hex
		       << DC3ListofAThings<DC3Tag>::value().getGroup()
		       << ","<< hex
		       << DC3ListofAThings<DC3Tag>::value().getElement()
		       << ")";
		DC3ListofAThings<DC3Tag>::next();
		if (DC3ListofAThings<DC3Tag>::ismore()) stream << ",";
	}
	stream << flush;
}

void
DC3AttributeTag::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);

	DC3ListofAThings<DC3Tag>::first();
	while (DC3ListofAThings<DC3Tag>::ismore()) {
		stream.write16(DC3ListofAThings<DC3Tag>::value().getGroup());
		stream.write16(DC3ListofAThings<DC3Tag>::value().getElement());
		DC3ListofAThings<DC3Tag>::next();
	}
}

/* ************************ Integer Methods ************************ */

void
DC3SignedLong::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/4);	// One 32 bit integer in bytes
		values=new Int32[number];
		if (values) {
			unsigned i;
			for (i=0; i<number; ++i)
				values[i]=(Int32)stream.read32();
		}
		// else what ?????
	}		
}

void
DC3SignedLong::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3SignedLong::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3SignedLong::putvalue(ostream& stream)
{
	DC3ListofNThings<Int32>::first();
	while (DC3ListofNThings<Int32>::ismore()) {
		stream << hex << DC3ListofNThings<Int32>::value();
		DC3ListofNThings<Int32>::next();
		if (DC3ListofNThings<Int32>::ismore()) stream << ",";
	}
	stream << flush;
}

void
DC3SignedLong::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);

	DC3ListofNThings<Int32>::first();
	while (DC3ListofNThings<Int32>::ismore()) {
		stream.write32((Uint32)DC3ListofNThings<Int32>::value());
		DC3ListofNThings<Int32>::next();
	}
}

void
DC3SignedShort::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/2);	// One 16 bit integer in bytes
		values=new Int16[number];
		if (values) {
			unsigned i;
			for (i=0; i<number; ++i) 
				values[i]=(Int16)stream.read16();
		}
		// else what ?????
	}		
}

void
DC3SignedShort::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3SignedShort::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3SignedShort::putvalue(ostream& stream)
{
	DC3ListofNThings<Int16>::first();
	while (DC3ListofNThings<Int16>::ismore()) {
		stream << hex << DC3ListofNThings<Int16>::value();
		DC3ListofNThings<Int16>::next();
		if (DC3ListofNThings<Int16>::ismore()) stream << ",";
	}
	stream << flush;
}
void
DC3SignedShort::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);

	DC3ListofNThings<Int16>::first();
	while (DC3ListofNThings<Int16>::ismore()) {
		stream.write16((Uint16)DC3ListofNThings<Int16>::value());
		DC3ListofNThings<Int16>::next();
	}
}

void
DC3UnsignedLong::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/4);	// One 32 bit integer in bytes
		values=new Uint32[number];
		if (values) {
			unsigned i;
			for (i=0; i<number; ++i) values[i]=stream.read32();
		}
		// else what ?????
	}		
}

void
DC3UnsignedLong::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3UnsignedLong::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3UnsignedLong::putvalue(ostream& stream)
{
	DC3ListofNThings<Uint32>::first();
	while (DC3ListofNThings<Uint32>::ismore()) {
		stream << hex << DC3ListofNThings<Uint32>::value();
		DC3ListofNThings<Uint32>::next();
		if (DC3ListofNThings<Uint32>::ismore()) stream << ",";
	}
	stream << flush;
}

void
DC3UnsignedLong::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);

	DC3ListofNThings<Uint32>::first();
	while (DC3ListofNThings<Uint32>::ismore()) {
		stream.write32(DC3ListofNThings<Uint32>::value());
		DC3ListofNThings<Uint32>::next();
	}
}

void
DC3UnsignedShort::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/2);	// One 16 bit integer in bytes
		values=new Uint16[number];
		if (values) {
			unsigned i;
			for (i=0; i<number; ++i) values[i]=stream.read16();
		}
		// else what ?????
	}		
}

void
DC3UnsignedShort::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3UnsignedShort::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3UnsignedShort::putvalue(ostream& stream)
{
	DC3ListofNThings<Uint16>::first();
	while (DC3ListofNThings<Uint16>::ismore()) {
		stream << hex << DC3ListofNThings<Uint16>::value();
		DC3ListofNThings<Uint16>::next();
		if (DC3ListofNThings<Uint16>::ismore()) stream << ",";
	}
	stream << flush;
}

void
DC3UnsignedShort::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);

	DC3ListofNThings<Uint16>::first();
	while (DC3ListofNThings<Uint16>::ismore()) {
		stream.write16(DC3ListofNThings<Uint16>::value());
		DC3ListofNThings<Uint16>::next();
	}
}

void
DC3UnspecifiedLong::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/4);	// One 32 bit integer in bytes
		values=new Uint32[number];
		if (values) {
			unsigned i;
			for (i=0; i<number; ++i) values[i]=stream.read32();
		}
		// else what ?????
	}		
}

void
DC3UnspecifiedLong::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3UnspecifiedLong::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3UnspecifiedLong::putvalue(ostream& stream)
{
	DC3ListofNThings<Uint32>::first();
	while (DC3ListofNThings<Uint32>::ismore()) {
		stream << hex << DC3ListofNThings<Uint32>::value();
		DC3ListofNThings<Uint32>::next();
		if (DC3ListofNThings<Uint32>::ismore()) stream << ",";
	}
	stream << flush;
}

void
DC3UnspecifiedLong::write(DC3ostream& stream)
{
	(void)stream;	// Shuts up compiler warning "stream not used"
	cerr << "DC3UnspecifiedLong::write: "
	     << "Error - can't write unspecified type - ignoring : ";
	GetTag().put(cerr);
	cerr << "\n" << flush;
}

void
DC3UnspecifiedShort::read(DC3istream& stream,Uint32 length)
{
	if (length == 0) {
		number=0;
	}
	else {
		number=(Uint16)(length/2);	// One 16 bit integer in bytes
		values=new Uint16[number];
		if (values) {
			unsigned i;
			for (i=0; i<number; ++i) values[i]=stream.read16();
		}
		// else what ?????
	}		
}

void
DC3UnspecifiedShort::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3UnspecifiedShort::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3UnspecifiedShort::putvalue(ostream& stream)
{
	DC3ListofNThings<Uint16>::first();
	while (DC3ListofNThings<Uint16>::ismore()) {
		stream << hex << DC3ListofNThings<Uint16>::value();
		DC3ListofNThings<Uint16>::next();
		if (DC3ListofNThings<Uint16>::ismore()) stream << ",";
	}
	stream << flush;
}

void
DC3UnspecifiedShort::write(DC3ostream& stream)
{
	(void)stream;	// Shuts up compiler warning "stream not used"
	cerr << "DC3UnspecifiedShort::write: "
	     << "Error - can't write unspecified type - ignoring : ";
	GetTag().put(cerr);
	cerr << "\n" << flush;
}

DC3UnsignedShort *
DC3UnspecifiedShort::toUnsignedShort(void)
{
	// Copy tags into a new array
	Uint16 *array = new Uint16[number];
	if (array) {
		// unsigned i;
		// for (i=0; i<n; ++i) array[i]=values[i];
		memcpy((void *)array,(void *)values,number*sizeof(Uint16));
	}
	DC3UnsignedShort *us = new DC3UnsignedShort(GetTag(),
			GetValueLength(),number,array);
	return us;
}

DC3SignedShort *
DC3UnspecifiedShort::toSignedShort(void)
{
	// Copy tags into a new array
	Int16 *array = new Int16[number];
	if (array) {
		// unsigned i;
		// for (i=0; i<n; ++i) array[i]=values[i];
		memcpy((void *)array,(void *)values,number*sizeof(Uint16));
	}
	DC3SignedShort *ss = new DC3SignedShort(GetTag(),
			GetValueLength(),number,array);
	return ss;
}

/* ************************ Large Attribute Methods ************************ */

void
DC3LargeAttribute::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "\n" << flush;
}

void
DC3LargeAttribute::read(DC3istream& stream,Uint32 vl)
{
	instream=&stream;
	*position=instream->rdbuf()->seekoff
		((streamoff)0,ios::cur,ios::in|ios::out);
	// Length is already stored in DC3Attribute.ValueLength
	DC3AttributeSkip(stream,vl);
}

void
DC3LargeAttribute::readdatastart(void)
{
	// Does NOT remember & restore current position, so can't be
	// intermingled with attribute reads

	if (instream->rdbuf()->seekpos(*position,ios::in|ios::out)
	    == streampos(EOF)) {
		cerr << "LargeAttribute::readdatastart position seek failed\n"
		     << flush;
	}
}

void
DC3LargeAttribute::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);
	writedata(stream);
}

void
DC3LargeAttribute::writedata(DC3ostream& stream,int convert)
{
	if (convert) {
		cerr << "LargeAttribute::convert specified for byte\n" << flush;
	}
	// Remembering & restoring the current position may be potentially
	// inefficient but it allows intermingling of reads & writes

	streampos oldposition=instream->rdbuf()->seekoff
		((streamoff)0,ios::cur,ios::in|ios::out);

	if (instream->rdbuf()->seekpos(*position,ios::in|ios::out)
	    == streampos(EOF)) {
		cerr << "LargeAttribute::fetch position seek failed\n" << flush;
	}
	else {
		Uint32 Length = GetValueLength();
		while (Length--) stream.write8(instream->read8());
		if (!instream->good())
			cerr << "LargeAttribute::fetch read failed\n" << flush;
		if (!stream.good())
			cerr << "LargeAttribute::write failed\n" << flush;
	}

	if (instream->rdbuf()->seekpos(oldposition,ios::in|ios::out)
	    == streampos(EOF)) {
		cerr << "LargeAttribute::fetch restore seek failed\n" << flush;
	}
}

void
DC3OtherByteString::put(ostream& stream)
{
	DC3Attribute::put(stream);
	stream << "[";
	DC3OtherByteString::putvalue(stream);
	stream << "]\n" << flush;
}

void
DC3OtherByteString::putvalue(ostream& stream)
{
	if (useinternalstorage) {
		Uint32 i=ValueLength;
		unsigned char *ptr=ibuffer;
		char prefix=' ';
		while (i--) {
			stream << prefix << hex << setfill('0') << setw(2)
			       << (unsigned)*ptr++;
			prefix=',';
		}
		stream << dec << setfill(' ');
	}
	// else forget it
}

void
DC3OtherByteString::read(DC3istream& stream,Uint32 vl)
{
	if (useinternalstorage) {
		if (vl > ValueLength) {
			cerr << "DC3OtherByteString::read "
			     << "request for " << vl
			     << " doesn't match value length "
			     << ValueLength << "\n"
			     << flush;
		}
		if (vl > ibufsize) {
			cerr << "DC3OtherByteString::read "
			     << "request for " << vl
			     << " exceeds internal data buffer size "
			     << ibufsize << "\n"
			     << flush;
			vl=ibufsize;
		}
		stream.read(ibuffer,(size_t)vl);
		if (!stream.good())
			cerr << "DC3OtherByteString::read read failed\n"
			     << flush;
	}
	else
		DC3LargeAttribute::read(stream,vl);
}

void
DC3OtherByteString::readdatastart(void)
{
	if (useinternalstorage)
		ibufptr=ibuffer;
	else
		DC3LargeAttribute::readdatastart();
}

void
DC3OtherByteString::readdatanext(Uint32 length,unsigned char * buffer)
{
	if (useinternalstorage) {
		size_t remaining=(size_t)ValueLength-(ibufptr-ibuffer);
		if (length > remaining) {
			cerr << "DC3OtherByteString::readdatanext "
			     << "request for " << length
			     << " bytes exceeds size of stored data left "
			     << remaining << "\n"
			     << flush;
			length=remaining;
		}
		memcpy(buffer,ibufptr,(size_t)length);
		ibufptr+=length;
	}
	else
		DC3LargeAttribute::readdatanext(length,buffer);
}

void
DC3OtherByteString::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);
	writedata(stream);
}

void
DC3OtherByteString::writedata(DC3ostream& stream,int convert)
{
	if (convert)
		cerr << "DC3OtherByteString::convert specified for byte\n"
		     << flush;

	if (useinternalstorage) 
		stream.write(ibuffer,(size_t)ValueLength);
	else
		DC3LargeAttribute::writedata(stream,convert);
}

void
DC3OtherWordString::readdatanext(Uint32 length,unsigned char * buffer)
{
	// NB. the length is in words not bytes.

	Uint16 *ubuffer = (Uint16 *)buffer;
	while (length--) *ubuffer++=instream->read16();
	if (!instream->good())
		cerr << "DC3OtherWordString::readdatanext read failed\n"
		     << flush;
}

void
DC3OtherWordString::write(DC3ostream& stream)
{
	DC3Attribute::write(stream);
	writedata(stream);
}

void
DC3OtherWordString::writedata(DC3ostream& stream,int convert)
{
	if (useexternalfetch)
		writedataexternal(stream,convert);
	else
		writedatainternal(stream,convert);
}

void
DC3OtherWordString::writedataexternal(DC3ostream& stream,int convert)
{
	if ((*fetchsourcelinestart)() == -1) {
		cerr << "DC3OtherWordString::writedata fetch start failed\n"
		     << flush;
	}

	Uint16 convertbit=1<<convert;
	Uint32 Length = GetValueLength()/2;	// Uint16 not byte
	while (Length > 0) {
		Uint16 *buffer;
		int length;
		if ((length=(*fetchsourceline)(&buffer)) == -1) {
			cerr << "DC3OtherWordString::writedata fetch failed\n"
			     << flush;
			break;
		}
		Length-=length;
		while (length-- > 0) {
			Uint16 value = *buffer++;
			if (convert) value^=convertbit;
			stream.write16(value);
		}
	}

	if (!stream.good())
		cerr << "DC3OtherWordString::writedata write failed\n"
		     << flush;
	if ((*fetchsourcelinedone)() == -1) {
		cerr << "DC3OtherWordString::writedata fetch done failed\n"
		     << flush;
	}
}

void
DC3OtherWordString::writedatainternal(DC3ostream& stream,int convert)
{
	// instream is already open

	instream->clear();	// Clear EOF state to allow seeking/reading

	// Remembering & restoring the current position may be potentially
	// inefficient but it allows intermingling of reads & writes

	streampos oldposition=instream->rdbuf()->seekoff
		((streamoff)0,ios::cur,ios::in|ios::out);

	// Position at start of image pixel data ...

	if (instream->rdbuf()->seekpos(*position,ios::in|ios::out)
	    == streampos(EOF)) {
		cerr << "DC3OtherWordString::fetch position seek failed\n"
		     << flush;
		return;
	}

	Uint16 convertbit=1<<convert;
	Uint32 Length = GetValueLength()/2;	// Uint16 not byte
	while (Length > 0) {
		Uint16 value=instream->read16();
		if (convert) value^=convertbit;
		stream.write16(value);
		--Length;
	}

	if (!instream->good()) {
		cerr << "DC3OtherWordString::fetch read failed\n" << flush;
		return;
	}
	if (!stream.good())
		cerr << "DC3OtherWordString::write failed\n"
		     << flush;

	// leave instream open - closed in main routine

	if (instream->rdbuf()->seekpos(oldposition,ios::in|ios::out)
	    == streampos(EOF)) {
		cerr << "DC3OtherWordString::fetch restore seek failed\n"
		     << flush;
		return;
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
