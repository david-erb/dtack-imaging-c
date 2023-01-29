/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tattrmk.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tattrmk_HEADER_
#define _DC3tattrmk_HEADER_

#define	setDecimalFloat		dec

//#define	setDecimalFloat		dec				\
//				<< setiosflags(ios::fixed)	\
//	    			<< setiosflags(ios::showpoint)	\
//				<< setprecision(6)

#define	setDecimalSigned	dec
#define	setDecimalUnsigned	dec

inline double
RoundDouble(double v)
{
	// rint() rounds x  to  an  integral  value  according  to  the
	// current IEEE rounding direction.

	return rint(v*10000)/10000;
}

inline DC3DecimalString *
MakeDecimalStringFromDouble(DC3Tag tag,double v)
{
	ostrstream ost;

	ost << setDecimalFloat
	    << RoundDouble(v)
	    << ends;

	char *s=ost.str();
	DC3DecimalString *a = new DC3DecimalString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3DecimalString *
MakeDecimalStringFromTwoDouble(DC3Tag tag,double v1,double v2)
{
	ostrstream ost;

	ost << setDecimalFloat
	    << RoundDouble(v1)
	    << "\\"
	    << RoundDouble(v2)
	    << ends;

	char *s=ost.str();
	DC3DecimalString *a = new DC3DecimalString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3DecimalString *
MakeDecimalStringFromThreeDouble(DC3Tag tag,double v1,double v2,double v3)
{
	ostrstream ost;

	ost << setDecimalFloat
	    << RoundDouble(v1)
	    << "\\"
	    << RoundDouble(v2)
	    << "\\"
	    << RoundDouble(v3)
	    << ends;

	char *s=ost.str();
	DC3DecimalString *a = new DC3DecimalString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3DecimalString *
MakeDecimalStringFromSixDouble(DC3Tag tag,
	double v1,double v2,double v3,double v4,double v5,double v6)
{
	ostrstream ost;

	ost << setDecimalFloat
	    << RoundDouble(v1)
	    << "\\"
	    << RoundDouble(v2)
	    << "\\"
	    << RoundDouble(v3)
	    << "\\"
	    << RoundDouble(v4)
	    << "\\"
	    << RoundDouble(v5)
	    << "\\"
	    << RoundDouble(v6)
	    << ends;

	char *s=ost.str();
	DC3DecimalString *a = new DC3DecimalString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}


inline DC3DecimalString *
MakeDecimalStringFromUnsigned(DC3Tag tag,unsigned long v)
{
	ostrstream ost;

	ost << setDecimalUnsigned
	    << v
	    << ends;

	char *s=ost.str();
	DC3DecimalString *a = new DC3DecimalString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3DecimalString *
MakeDecimalStringFromSigned(DC3Tag tag,signed long v)
{
	ostrstream ost;

	ost << setDecimalSigned
	    << v
	    << ends;

	char *s=ost.str();
	DC3DecimalString *a = new DC3DecimalString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3IntegerString *
MakeIntegerStringEmpty(DC3Tag tag)
{
	DC3IntegerString *a = new DC3IntegerString(tag,0,"");
	return a;
}

inline DC3IntegerString *
MakeIntegerStringFromUnsigned(DC3Tag tag,unsigned long v)
{
	ostrstream ost;

	ost << setDecimalUnsigned
	    << v
	    << ends;

	char *s=ost.str();
	DC3IntegerString *a = new DC3IntegerString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3DateString *
MakeDateStringEmpty(DC3Tag tag)
{
	DC3DateString *a = new DC3DateString(tag,0,"");
	return a;
}

inline DC3DateString *
MakeDateStringFromThreeValues(DC3Tag tag,unsigned yyyy,unsigned mm,unsigned dd)
{
	ostrstream ost;

	ost << dec << setfill('0');
	if (yyyy < 100) ost << (yyyy<70?"20":"19") << setw(2);
	else 		ost << setw(4);
	ost << yyyy
	    << setw(2) << mm
	    << setw(2) << dd
	    << ends;

	char *s = ost.str();
	DC3DateString *a = new DC3DateString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3TimeString *
MakeTimeStringEmpty(DC3Tag tag)
{
	DC3TimeString *a = new DC3TimeString(tag,0,"");
	return a;
}

inline DC3TimeString *
MakeTimeStringFromThreeValues(DC3Tag tag,unsigned hh,unsigned mm,unsigned ss)
{
	ostrstream ost;

	ost << dec << setfill('0')
	    << setw(2) << hh
	    << setw(2) << mm
	    << setw(2) << ss
	    << ends;

	char *s = ost.str();
	DC3TimeString *a = new DC3TimeString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3CodeString *
MakeCodeStringFromThreeStringValues(DC3Tag tag,const char *s1,const char *s2,const char *s3)
{
	ostrstream ost;

	ost << s1
	    << "\\"
	    << s2
	    << "\\"
	    << s3
	    << ends;

	char *s=ost.str();
	DC3CodeString *a = new DC3CodeString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3CodeString *
MakeCodeStringFromTwoStringValues(DC3Tag tag,const char *s1,const char *s2)
{
	ostrstream ost;

	ost << s1
	    << "\\"
	    << s2
	    << ends;

	char *s=ost.str();
	DC3CodeString *a = new DC3CodeString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3CodeString *
MakeCodeStringFromConcatTwoStrings(DC3Tag tag,const char *s1,const char *s2)
{
	ostrstream ost;

	ost << s1
	    << s2
	    << ends;

	char *s=ost.str();
	DC3CodeString *a = new DC3CodeString(tag,strlen(s),s);
	if (s) delete[] s;
	return a;
}

inline DC3CodeString *
MakeCodeStringFromString(DC3Tag tag,const char *s)
{
	DC3CodeString *a = new DC3CodeString(tag,strlen(s),s);
	return a;
}

inline DC3CodeString *
MakeCodeStringEmpty(DC3Tag tag)
{
	DC3CodeString *a = new DC3CodeString(tag,0,"");
	return a;
}

inline DC3ShortString *
MakeShortStringEmpty(DC3Tag tag)
{
	DC3ShortString *a = new DC3ShortString(tag,0,"");
	return a;
}

inline DC3ShortString *
MakeShortStringFromString(DC3Tag tag,const char *s)
{
	DC3ShortString *a = new DC3ShortString(tag,strlen(s),s);
	return a;
}

inline DC3LongString *
MakeLongStringEmpty(DC3Tag tag)
{
	DC3LongString *a = new DC3LongString(tag,0,"");
	return a;
}

inline DC3LongString *
MakeLongStringFromString(DC3Tag tag,const char *s)
{
	DC3LongString *a = new DC3LongString(tag,strlen(s),s);
	return a;
}

inline DC3UIString *
MakeUIStringFromString(DC3Tag tag,const char *s)
{
	DC3UIString *a = new DC3UIString(tag,strlen(s),s);
	return a;
}

inline DC3PersonName *
MakePersonNameEmpty(DC3Tag tag)
{
	DC3PersonName *a = new DC3PersonName(tag,0,"");
	return a;
}

inline DC3PersonName *
MakePersonNameFromString(DC3Tag tag,const char *s)
{
	DC3PersonName *a = new DC3PersonName(tag,strlen(s),s);
	return a;
}

inline DC3UnsignedShort *
MakeUnsignedShortFromUint16(DC3Tag tag,Uint16 i)
{
	DC3UnsignedShort *a = new DC3UnsignedShort(tag,2,&i);
	return a;
}

inline DC3SignedShort *
MakeSignedShortFromInt16(DC3Tag tag,Int16 i)
{
	DC3SignedShort *a = new DC3SignedShort(tag,2,&i);
	return a;
}


#endif /* _DC3tattrmk_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
