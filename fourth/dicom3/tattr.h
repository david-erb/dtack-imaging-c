/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tattr.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tattr_HEADER_
#define _DC3tattr_HEADER_

#include <fstream.h>
#include <strstream.h>
#include <iomanip.h>
#include <string.h>

#include "ttypes.h"
#include "ttag.h"
#include "tstream.h"

typedef Uint32 Xint;	// generic unsigned big enough to hold anything

/* ************************ The Attribute Base Class ************************ */

class DC3Attribute {
	DC3Tag		Tag;
protected:
	Uint32		ValueLength;
public:
	DC3Attribute(DC3Tag t,Uint32 vl)
			{ Tag=t; ValueLength=vl; }

	virtual ~DC3Attribute() {}

	DC3Tag		GetTag(void)			{ return Tag; }
	Uint32		GetValueLength(void)		{ return ValueLength; }
	virtual Uint16	GetValueMultiplicity(void)	{ return 0; }

	virtual void	put(ostream& stream);
	virtual void	putvalue(ostream& stream) = 0;
	virtual void	write(DC3ostream& stream);
	virtual void	writedata(DC3ostream& stream,int convert=0)
			{ (void)stream; (void)convert; }

	virtual void	readdatastart(void)		{}
	virtual void	readdatanext(Uint32 length,unsigned char * buffer)
			{ (void)length; (void)buffer; }

	virtual const char *ValueRepresentation(void)	{ return 0; }

	virtual void	first(void)			{}
	virtual void	next(void)			{}
	virtual int	ismore(void)			{ return 0; }

	virtual Uint16	ValueUint16(void)		{ return 0; }	// :(

	virtual int	IsNumeric(void)			{ return 0; }
	virtual Xint	ValueXint(void)			{ return 0; }
	virtual void	AddXint(Xint value)		{ (void)value; }

	virtual int	IsString(void)			{ return 0; }
	virtual int	IsNumericString(void)		{ return 0; }
	virtual const char *ValueString(void)		{ return 0; }
	virtual char *	ValueStringCopy(void)		{ return 0; }
	virtual char *	ValueNumericStringCopy(void)	{ return 0; }
	virtual void	AddStringCopy(const char *value)	{ (void)value; }

	int		SingleValueNumeric(Xint& value);
	int		SingleValueStringCopy(char * &value);
};

/* ************************ General Routines ************************ */

void	DC3AttributeSkip(DC3istream& stream,Uint32 Length);

void	DC3AttributeRead(DC3istream& stream,DC3Attribute **rattr,int *rerr);

void	DC3AttributeGet(char *keyword,char *values,
			DC3Attribute **rattr,int *rerr);

/* ************************ String Attributes ************************ */

class DC3ListofStrings {
	union {
		char **strings;	// Array of '\0' terminated strings
		char *string;	// Single '\0' terminated string
	} u;
	char **ptr;
	int count;
	unsigned number;
protected:
//public:
	DC3ListofStrings(unsigned number,const char **s);
	DC3ListofStrings(const char *s);
	DC3ListofStrings(void)	{ number=0; }
	virtual ~DC3ListofStrings();

	// Used as { first(); while (ismore()) { use it(); next(); } }

	void	first(void)	{ count=0; if (number > 1) ptr=u.strings; }
	void	next(void)	{ ++count; if (number > 1) ++ptr; }
	int	ismore(void)	{ return count < number; }

	const char *value(void)	{ const char *s = 0;
				  switch (number) {
					case 0:		break;
					case 1:		s=u.string; break;
					default:	s=*ptr; break;
				  };
				  return s;
				}

	void	add(const char *s);

	void	get(char *string);
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 length);
	void	write(DC3ostream& stream);
	Uint32	GetValueLength(void);
	Uint16	GetValueMultiplicity(void)	{ return number; }
};

class DC3StringAttribute : public DC3Attribute, public DC3ListofStrings {
public:
	DC3StringAttribute(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3Attribute(t,vl)
		, DC3ListofStrings(vm,s) {
			ValueLength=DC3ListofStrings::GetValueLength();
		}
	DC3StringAttribute(DC3Tag t,Uint32 vl,const char *s)
		: DC3Attribute(t,vl)
		, DC3ListofStrings(s) {
			ValueLength=DC3ListofStrings::GetValueLength();
		}
	DC3StringAttribute(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofStrings() {
			ValueLength=DC3ListofStrings::GetValueLength();
		}
	void	get(char *string) {
			DC3ListofStrings::get(string);
			ValueLength=DC3ListofStrings::GetValueLength();
		}
	void	put(ostream& stream) {
			DC3Attribute::put(stream);
			DC3ListofStrings::put(stream);
			stream << "\n" << flush;
		}
	void	putvalue(ostream& stream) {
			DC3ListofStrings::putvalue(stream);
		}
	void	read(DC3istream& stream,Uint32 vl) {
			DC3ListofStrings::read(stream,vl);
			ValueLength=DC3ListofStrings::GetValueLength();
		}
	void	write(DC3ostream& stream) {
			DC3Attribute::write(stream);
			DC3ListofStrings::write(stream);
		}

	void	first(void)		{ DC3ListofStrings::first(); }
	void	next(void)		{ DC3ListofStrings::next(); }
	int	ismore(void)		{ return DC3ListofStrings::ismore(); }

	void	add(const char *s) {	// use of this name needs to go.
			DC3ListofStrings::add(s);
			ValueLength=DC3ListofStrings::GetValueLength();
		}

	int	IsString(void)		{ return 1; }
	const char * ValueString(void)	{ return DC3ListofStrings::value(); }
	char *	ValueStringCopy(void) { 
			const char *from=DC3ListofStrings::value();
			char *to;
			if (from) {
				to=new char [strlen(from)+1];
				strcpy(to,from);
			}
			else to=0;
			return to;
		}
	char *	ValueNumericStringCopy(void) 	{ return ValueStringCopy(); }
	void	AddStringCopy(const char *value)	{ add(value); }

	Xint	ValueXint(void)	{ 	// Useful iff IsNumericString() is true
			Xint i;
			const char *str = ValueString();
			//istrstream(str) >> i;
			istrstream((char *)str) >> i;	// OSF is broken
			return i;
		}
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofStrings::GetValueMultiplicity(); }
};

class DC3ApplicationEntity : public DC3StringAttribute {
public:
	DC3ApplicationEntity
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3ApplicationEntity
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3ApplicationEntity
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "AE"; }
};

class DC3AgeString : public DC3StringAttribute {
public:
	DC3AgeString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3AgeString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3AgeString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "AS"; }

	int	IsNumericString(void)		{ return 1; }
};

class DC3CodeString : public DC3StringAttribute {
public:
	DC3CodeString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3CodeString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3CodeString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "CS"; }
};

class DC3DateString : public DC3StringAttribute {
public:
	DC3DateString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3DateString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3DateString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "DA"; }
};

class DC3DecimalString : public DC3StringAttribute {
public:
	DC3DecimalString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3DecimalString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3DecimalString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "DS"; }

	int	IsNumericString(void)		{ return 1; }
};

class DC3DateTimeString : public DC3StringAttribute {
public:
	DC3DateTimeString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3DateTimeString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3DateTimeString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "DT"; }
};

class DC3IntegerString : public DC3StringAttribute {
public:
	DC3IntegerString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3IntegerString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3IntegerString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "IS"; }

	int	IsNumericString(void)		{ return 1; }
};

class DC3LongString : public DC3StringAttribute {
public:
	DC3LongString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3LongString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3LongString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "LO"; }
};

class DC3LongText : public DC3StringAttribute {
public:
	DC3LongText
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3LongText
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3LongText
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "LT"; }
};

class DC3PersonName : public DC3StringAttribute {
public:
	DC3PersonName
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3PersonName
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3PersonName
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "PN"; }
};

class DC3ShortString : public DC3StringAttribute {
public:
	DC3ShortString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3ShortString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3ShortString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "SH"; }
};

class DC3ShortText : public DC3StringAttribute {
public:
	DC3ShortText
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3ShortText
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3ShortText
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "ST"; }
};

class DC3TimeString : public DC3StringAttribute {
public:
	DC3TimeString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3TimeString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3TimeString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	const char *ValueRepresentation(void)
		{ return "TM"; }
};

class DC3UIString : public DC3StringAttribute {
	void	NullPaddedwrite(DC3ostream& stream);
public:
	DC3UIString
		(DC3Tag t,Uint32 vl,unsigned vm,const char **s)
		: DC3StringAttribute(t,vl,vm,s) {}
	DC3UIString
		(DC3Tag t,Uint32 vl,const char *s)
		: DC3StringAttribute(t,vl,s) {}
	DC3UIString
		(DC3Tag t,Uint32 vl)
		: DC3StringAttribute(t,vl) {}

	void	write(DC3ostream& stream) {	// Needs different padding
		DC3Attribute::write(stream);
		DC3UIString::NullPaddedwrite(stream);
		}

	const char *ValueRepresentation(void)
		{ return "UI"; }
};

/* ************************ Tag Attributes ************************ */

/* Template notes ...

	g++ 2.5.7 has trouble with constructor declaration of the
	form "DC3ListofNThings<T>(unsigned n,T *at) {}" so leave
	out the <T> and both g++ and CC-3.01 are happy.

	the instantiation rules used by CC-3.01 with separate interface
	and implementation are so messy, just keep everything here
	(which also speeds things up at link time no end as everything
	is already instantiated)

	CC-3.01 won't inline for statements.
*/

template<class T>
class DC3ListofAThings {
	T *ptr;
	int count;
protected:
	unsigned number;	// yuck !! (for the read routines)
	T *Tags;		// worse !!!
//public:
	DC3ListofAThings(unsigned n,T *at) {
		// Copy tags into a new array
		T *array = new T[n];
		if (array) {
			// unsigned i;
			// for (i=0; i<n; ++i) array[i]=at[i];
			memcpy((void *)array,(void *)at,n*sizeof(T));
		}
		Tags=array;		// Null if couldn't alloc
		number=n;	
	}
	~DC3ListofAThings()
		{
			if (Tags) delete [] Tags;
		}

	// Used as { first(); while (ismore()) { use it(); next(); } }

	void	first(void)	{ count=0; if (number > 0) ptr=Tags; }
	void	next(void)	{ ++count; if (number > 0) ++ptr; }
	int	ismore(void)	{ return count < number; }

	T	 value(void)	{ T *p = 0;
				  switch (number) {
					case 0:		break;
					default:	p=ptr; break;
				  };
				  return *p;
				}
	Uint16	GetValueMultiplicity(void)	{ return number; }
};

class DC3AttributeTag : public DC3Attribute, public DC3ListofAThings<DC3Tag> {
public:
	DC3AttributeTag
		(DC3Tag t,Uint32 vl,unsigned vm,DC3Tag *at)
		: DC3Attribute(t,vl)
		, DC3ListofAThings<DC3Tag>(vm,at)
		{}
	DC3AttributeTag(DC3Tag t,Uint32 vl,DC3Tag *at)
		: DC3Attribute(t,vl)
		, DC3ListofAThings<DC3Tag>(1,at)
		{}
	DC3AttributeTag(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofAThings<DC3Tag>(0,0)
		{}

	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "AT"; }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofAThings<DC3Tag>::GetValueMultiplicity(); }
};


/* ************************ Integer Attributes ************************ */

template<class T>
class DC3ListofNThings {
	T *ptr;
	int count;
protected:
	unsigned number;	// yuck !! (for the read routines)
	T *values;		// worse !!!
//public:
	DC3ListofNThings(unsigned n,T *at) {
		// Copy tags into a new array
		T *array = new T[n];
		if (array) {
			// unsigned i;
			// for (i=0; i<n; ++i) array[i]=at[i];
			memcpy((void *)array,(void *)at,n*sizeof(T));
		}
		values=array;		// Null if couldn't alloc
		number=n;	
	}
	~DC3ListofNThings()
		{
			if (values) delete [] values;
		}

	// Used as { first(); while (ismore()) { use it(); next(); } }

	void	first(void)	{ count=0; if (number > 0) ptr=values; }
	void	next(void)	{ ++count; if (number > 0) ++ptr; }
	int	ismore(void)	{ return count < number; }

	T	value(void)	{ T *p = 0;
				  switch (number) {
					case 0:		break;
					default:	p=ptr; break;
				  };
				  return *p;
				}

	int	IsNumeric(void)		{ return 1; }
	Xint	ValueXint(void)		{ return value(); }
	void	AddXint(Xint value) { 
			++number;
			T *newarray = new T[number];
			unsigned i;
			if (values) {
				i=0;
				while (i<number-1) {
					newarray[i]=values[i];
					++i;
				}
				delete [] values;
			}
			values=newarray;
			values[number-1]=(T)value;
		}

	char *	ValueNumericStringCopy(void) { 
			ostrstream ost;
			ost << dec << value() << '\0';
			return ost.str();
		}

	void	get(istream& stream) {
		T value;
		char delimiter;
		unsigned n = 0;
		T *array = 0;
		while (1) {
			stream >> value;
			if (stream) {
				++n;
				T *newarray = new T[n];
				unsigned i;
				if (array) {
					i=0;
					while (i<n-1) {
						newarray[i]=array[i];
						++i;
					}
					delete [] array;
				}
				array=newarray;
				array[n-1]=value;
				stream >> delimiter;
			}
			else break;
		}
		number=n;
		values=array;
	}
	Uint16	GetValueMultiplicity(void)	{ return number; }
};

class DC3SignedLong : public DC3Attribute, public DC3ListofNThings<Int32> {
public:
	DC3SignedLong
		(DC3Tag t,Uint32 vl,unsigned vm,Int32 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Int32>(vm,at)
		{}
	DC3SignedLong(DC3Tag t,Uint32 vl,Int32 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Int32>(1,at)
		{}
	DC3SignedLong(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Int32>(0,0)
		{}

	void	get(istream& stream)	{
					DC3ListofNThings<Int32>::get(stream);
					ValueLength=number*4;
					}
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "SL"; }

	void	first(void)	{ DC3ListofNThings<Int32>::first(); }
	void	next(void)	{ DC3ListofNThings<Int32>::next(); }
	int	ismore(void)	{ return DC3ListofNThings<Int32>::ismore(); }
	Int32	value(void)	{ return DC3ListofNThings<Int32>::value(); }
	int	IsNumeric(void)
			{ return DC3ListofNThings<Int32>::IsNumeric(); }
	Xint	ValueXint(void)	
			{ return DC3ListofNThings<Int32>::ValueXint(); }
	void	AddXint(Xint value) 
			{ DC3ListofNThings<Int32>::AddXint(value); }
	char *	ValueNumericStringCopy(void)
		{ return DC3ListofNThings<Int32>::ValueNumericStringCopy(); }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofNThings<Int32>::GetValueMultiplicity(); }
};

class DC3SignedShort : public DC3Attribute, public DC3ListofNThings<Int16> {
public:
	DC3SignedShort
		(DC3Tag t,Uint32 vl,unsigned vm,Int16 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Int16>(vm,at)
		{}
	DC3SignedShort(DC3Tag t,Uint32 vl,Int16 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Int16>(1,at)
		{}
	DC3SignedShort(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Int16>(0,0)
		{}

	void	get(istream& stream)	{
					DC3ListofNThings<Int16>::get(stream);
					ValueLength=number*2;
					}
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "SS"; }

	void	first(void)	{ DC3ListofNThings<Int16>::first(); }
	void	next(void)	{ DC3ListofNThings<Int16>::next(); }
	int	ismore(void)	{ return DC3ListofNThings<Int16>::ismore(); }
	Int16	value(void)	{ return DC3ListofNThings<Int16>::value(); }
	int	IsNumeric(void)
			{ return DC3ListofNThings<Int16>::IsNumeric(); }
	Xint	ValueXint(void) 
			{ return DC3ListofNThings<Int16>::ValueXint(); }
	void	AddXint(Xint value) 
			{ DC3ListofNThings<Int16>::AddXint(value); }
	char *	ValueNumericStringCopy(void)
		{ return DC3ListofNThings<Int16>::ValueNumericStringCopy(); }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofNThings<Int16>::GetValueMultiplicity(); }
};

class DC3UnsignedLong : public DC3Attribute, public DC3ListofNThings<Uint32> {
public:
	DC3UnsignedLong
		(DC3Tag t,Uint32 vl,unsigned vm,Uint32 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint32>(vm,at)
		{}
	DC3UnsignedLong(DC3Tag t,Uint32 vl,Uint32 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint32>(1,at)
		{}
	DC3UnsignedLong(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint32>(0,0)
		{}

	void	get(istream& stream)	{
					DC3ListofNThings<Uint32>::get(stream);
					ValueLength=number*4;
					}
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "UL"; }

	void	first(void)	{ DC3ListofNThings<Uint32>::first(); }
	void	next(void)	{ DC3ListofNThings<Uint32>::next(); }
	int	ismore(void)	{ return DC3ListofNThings<Uint32>::ismore(); }
	Uint32	value(void)	{ return DC3ListofNThings<Uint32>::value(); }
	int	IsNumeric(void)
			{ return DC3ListofNThings<Uint32>::IsNumeric(); }
	Xint	ValueXint(void) 
			{ return DC3ListofNThings<Uint32>::ValueXint(); }
	void	AddXint(Xint value) 
			{ DC3ListofNThings<Uint32>::AddXint(value); }
	char *	ValueNumericStringCopy(void)
		{ return DC3ListofNThings<Uint32>::ValueNumericStringCopy(); }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofNThings<Uint32>::GetValueMultiplicity(); }
};

class DC3UnsignedShort : public DC3Attribute, public DC3ListofNThings<Uint16> {
public:
	DC3UnsignedShort
		(DC3Tag t,Uint32 vl,unsigned vm,Uint16 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint16>(vm,at)
		{}
	DC3UnsignedShort(DC3Tag t,Uint32 vl,Uint16 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint16>(1,at)
		{}
	DC3UnsignedShort(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint16>(0,0)
		{}

	void	get(istream& stream)	{
					DC3ListofNThings<Uint16>::get(stream);
					ValueLength=number*2;
					}
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "US"; }

	Uint16	ValueUint16(void)
			{ return DC3ListofNThings<Uint16>::value(); }

	void	first(void)	{ DC3ListofNThings<Uint16>::first(); }
	void	next(void)	{ DC3ListofNThings<Uint16>::next(); }
	int	ismore(void)	{ return DC3ListofNThings<Uint16>::ismore(); }
	Uint16	value(void)	{ return DC3ListofNThings<Uint16>::value(); }
	int	IsNumeric(void)
			{ return DC3ListofNThings<Uint16>::IsNumeric(); }
	Xint	ValueXint(void) 
			{ return DC3ListofNThings<Uint16>::ValueXint(); }
	void	AddXint(Xint value) 
			{ DC3ListofNThings<Uint16>::AddXint(value); }
	char *	ValueNumericStringCopy(void)
		{ return DC3ListofNThings<Uint16>::ValueNumericStringCopy(); }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofNThings<Uint16>::GetValueMultiplicity(); }
};

class DC3UnspecifiedLong : public DC3Attribute, public DC3ListofNThings<Uint32> {
public:
	DC3UnspecifiedLong
		(DC3Tag t,Uint32 vl,unsigned vm,Uint32 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint32>(vm,at)
		{}
	DC3UnspecifiedLong(DC3Tag t,Uint32 vl,Uint32 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint32>(1,at)
		{}
	DC3UnspecifiedLong(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint32>(0,0)
		{}

	void	get(istream& stream)	{
					DC3ListofNThings<Uint32>::get(stream);
					ValueLength=number*4;
					}
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "XL"; }

	void	first(void)	{ DC3ListofNThings<Uint32>::first(); }
	void	next(void)	{ DC3ListofNThings<Uint32>::next(); }
	int	ismore(void)	{ return DC3ListofNThings<Uint32>::ismore(); }
	Uint32	value(void)	{ return DC3ListofNThings<Uint32>::value(); }
	int	IsNumeric(void)
			{ return DC3ListofNThings<Uint32>::IsNumeric(); }
	Xint	ValueXint(void) 
			{ return DC3ListofNThings<Uint32>::ValueXint(); }
	void	AddXint(Xint value) 
			{ DC3ListofNThings<Uint32>::AddXint(value); }
	char *	ValueNumericStringCopy(void)
		{ return DC3ListofNThings<Uint32>::ValueNumericStringCopy(); }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofNThings<Uint32>::GetValueMultiplicity(); }
};

class DC3UnspecifiedShort : public DC3Attribute,public DC3ListofNThings<Uint16> {
public:
	DC3UnspecifiedShort
		(DC3Tag t,Uint32 vl,unsigned vm,Uint16 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint16>(vm,at)
		{}
	DC3UnspecifiedShort(DC3Tag t,Uint32 vl,Uint16 *at)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint16>(1,at)
		{}
	DC3UnspecifiedShort(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		, DC3ListofNThings<Uint16>(0,0)
		{}

	DC3UnsignedShort *	toUnsignedShort(void);
	DC3SignedShort *	toSignedShort(void);

	void	get(istream& stream)	{
					DC3ListofNThings<Uint16>::get(stream);
					ValueLength=number*2;
					}
	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	write(DC3ostream& stream);

	const char *ValueRepresentation(void)
		{ return "XS"; }

	void	first(void)	{ DC3ListofNThings<Uint16>::first(); }
	void	next(void)	{ DC3ListofNThings<Uint16>::next(); }
	int	ismore(void)	{ return DC3ListofNThings<Uint16>::ismore(); }
	Uint16	value(void)	{ return DC3ListofNThings<Uint16>::value(); }
	int	IsNumeric(void)
			{ return DC3ListofNThings<Uint16>::IsNumeric(); }
	Xint	ValueXint(void) 
			{ return DC3ListofNThings<Uint16>::ValueXint(); }
	void	AddXint(Xint value) 
			{ DC3ListofNThings<Uint16>::AddXint(value); }
	char *	ValueNumericStringCopy(void)
		{ return DC3ListofNThings<Uint16>::ValueNumericStringCopy(); }
	Uint16	GetValueMultiplicity(void)
		{ return DC3ListofNThings<Uint16>::GetValueMultiplicity(); }
};

/* ************************ Large Attributes ************************ */

class DC3LargeAttribute : public DC3Attribute {		// No VM > 1
protected:
	DC3istream	*instream;
	streampos	*position;	// Sigh ! g++,sun are a simple type
					// but Symantec is a class and I
					// can't get it to find a constructor
	// position=stream->seekoff((streamoff)0,ios::cur,ios::in|ios::out);
	// position=stream->seekpos(position,ios::in|ios::out);
	// streampos(EOF) is error
public:
	DC3LargeAttribute(DC3Tag t,Uint32 vl)
		: DC3Attribute(t,vl)
		{ position=new streampos(EOF); }
	virtual ~DC3LargeAttribute()
		{ delete position; }

	void	put(ostream& stream);
	void	putvalue(ostream& stream) 		{ (void)stream; }
	void	read(DC3istream& stream,Uint32 vl);
	void	readdatastart(void);
	void	write(DC3ostream& stream);
	void	writedata(DC3ostream& stream,int convert=0);
	Uint16	GetValueMultiplicity(void)		{ return 1; }
};

class DC3OtherByteString : public DC3LargeAttribute {
	int	useinternalstorage;
	size_t	ibufsize;
	unsigned char *	ibuffer;
	unsigned char *	ibufptr;
public:
	DC3OtherByteString
		(DC3Tag t,Uint32 vl)
		: DC3LargeAttribute(t,vl)
		{
			if (vl > 1024 || vl == 0xffffffff || vl == 0) {
				useinternalstorage=0;
				ibufsize=0;
				ibuffer=0;
				ibufptr=0;
			}
			else {
				useinternalstorage=1;
				ibufsize=(size_t)vl;
				ibuffer=new unsigned char[ibufsize];
				ibufptr=ibuffer;
			}
		}

	DC3OtherByteString
		(DC3Tag t,Uint32 vl,unsigned char *data)
		: DC3LargeAttribute(t,vl)
		{
			useinternalstorage=1;
			ibufsize=(size_t)vl;
			ibuffer=new unsigned char[ibufsize];
			ibufptr=ibuffer;
			memcpy(ibuffer,data,ibufsize);
		}

	void	put(ostream& stream);
	void	putvalue(ostream& stream);
	void	read(DC3istream& stream,Uint32 vl);
	void	readdatastart(void);
	void	readdatanext(Uint32 length,unsigned char * buffer);
	void	write(DC3ostream& stream);
	void	writedata(DC3ostream& stream,int convert=0);

	const char *ValueRepresentation(void)
		{ return "OB"; }
};

class DC3OtherWordString : public DC3LargeAttribute {
	int	useexternalfetch;
	int	(*fetchsourcelinestart)(void);
	int	(*fetchsourceline)(Uint16 **buffer);
	int	(*fetchsourcelinedone)(void);
	void	writedatainternal(DC3ostream& stream,int convert);
	void	writedataexternal(DC3ostream& stream,int convert);
public:
	DC3OtherWordString
		(DC3Tag t,Uint32 vl)
		: DC3LargeAttribute(t,vl)
		{
			useexternalfetch=0;
		}

	DC3OtherWordString
		(DC3Tag t,Uint32 vl,
		int (*rls)(void),
		int (*rl)(Uint16 **buffer),
		int (*rld)(void))
		: DC3LargeAttribute(t,vl)
		{
			useexternalfetch=1;
			fetchsourcelinestart	= rls;
			fetchsourceline		= rl;
			fetchsourcelinedone	= rld;
		}

	void	readdatanext(Uint32 length,unsigned char * buffer);
	void	write(DC3ostream& stream);
	void	writedata(DC3ostream& stream,int convert=0);

	const char *ValueRepresentation(void)
		{ return "OW"; }
};

#endif /* _DC3tattr_HEADER_ */

// FL
// FD
// SQ





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
