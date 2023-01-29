/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tuidgen.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */


#ifndef _DC3tuidgen_HEADER_
#define _DC3tuidgen_HEADER_

#include <strstream.h>
#include <iomanip.h>

#ifdef __SC__
#include <types.h>
#include <time.h>
#else /* __SC__ */
#include <sys/types.h>
#include <sys/time.h>
#endif /* __SC__ */

#ifndef DC3UIDGEN_ROOT
#define	DC3UIDGEN_ROOT		"0.0.0.0"		// :(
#endif /* DC3UIDGEN_ROOT */

#define	DC3UIDGEN_INSTANCE_SOP			".1"
#define	DC3UIDGEN_INSTANCE_STUDY		".2"
#define	DC3UIDGEN_INSTANCE_SERIES		".3"
#define	DC3UIDGEN_FRAMEOFREFERENCE		".4"

class DC3GeneratedUID {
	ostrstream	ost;
	unsigned	terminated;
	const char *	stamp;
protected:
	void	SetSOPInstance()	{ ost << DC3UIDGEN_INSTANCE_SOP; }
	void	SetStudyInstance()	{ ost << DC3UIDGEN_INSTANCE_STUDY; }
	void	SetSeriesInstance()	{ ost << DC3UIDGEN_INSTANCE_SERIES; }
	void	SetFrameOfReference()	{ ost << DC3UIDGEN_FRAMEOFREFERENCE; }

	void	SetUnsigned(unsigned u)	{ ost << "." << u; }
public:
	DC3GeneratedUID(void)
		{
			ost << DC3UIDGEN_ROOT;
			stamp=0;
			terminated=0;
		}
	DC3GeneratedUID(const char *s)
		{
			ost << DC3UIDGEN_ROOT;
			stamp=s;
			terminated=0;
		}

	char *	GetUID(void)	// NB. Once GetUID is called, can't add to UID
		{
			if (!terminated) {
				ost << ".";
				if (stamp) ost << stamp;
				else ost << dec << time(0);
				ost << '\0';
				terminated=1;
			}
			return ost.str();
		}
};

class DC3GeneratedSOPInstanceUID : public DC3GeneratedUID {
public:
	DC3GeneratedSOPInstanceUID(void)
		: DC3GeneratedUID()
		{
			SetSOPInstance();
		}
	DC3GeneratedSOPInstanceUID(const char *s)
		: DC3GeneratedUID(s)
		{
			SetSOPInstance();
		}
	DC3GeneratedSOPInstanceUID
			(unsigned study,unsigned series,unsigned image)
		: DC3GeneratedUID()
		{
			SetSOPInstance();
			SetUnsigned(study);
			SetUnsigned(series);
			SetUnsigned(image);
		}
	DC3GeneratedSOPInstanceUID
			(const char *s,unsigned study,unsigned series,
				unsigned image)
		: DC3GeneratedUID(s)
		{
			SetSOPInstance();
			SetUnsigned(study);
			SetUnsigned(series);
			SetUnsigned(image);
		}
};

class DC3GeneratedStudyInstanceUID : public DC3GeneratedUID {
public:
	DC3GeneratedStudyInstanceUID(unsigned study)
		: DC3GeneratedUID()
		{
			SetStudyInstance();
			SetUnsigned(study);
		}
	DC3GeneratedStudyInstanceUID(const char *s,unsigned study)
		: DC3GeneratedUID(s)
		{
			SetStudyInstance();
			SetUnsigned(study);
		}
};

class DC3GeneratedSeriesInstanceUID : public DC3GeneratedUID {
public:
	DC3GeneratedSeriesInstanceUID(unsigned study,unsigned series)
		: DC3GeneratedUID()
		{
			SetSeriesInstance();
			SetUnsigned(study);
			SetUnsigned(series);
		}
	DC3GeneratedSeriesInstanceUID(const char *s,unsigned study,
					unsigned series)
		: DC3GeneratedUID(s)
		{
			SetSeriesInstance();
			SetUnsigned(study);
			SetUnsigned(series);
		}
};

class DC3GeneratedFrameOfReferenceUID : public DC3GeneratedUID {
public:
	DC3GeneratedFrameOfReferenceUID(void)
		: DC3GeneratedUID()
		{
			SetFrameOfReference();
		}
	DC3GeneratedFrameOfReferenceUID(const char *s)
		: DC3GeneratedUID(s)
		{
			SetFrameOfReference();
		}
	DC3GeneratedFrameOfReferenceUID(unsigned study)
		: DC3GeneratedUID()
		{
			SetFrameOfReference();
			SetUnsigned(study);
		}
	DC3GeneratedFrameOfReferenceUID(const char *s,unsigned study)
		: DC3GeneratedUID(s)
		{
			SetFrameOfReference();
			SetUnsigned(study);
		}
	DC3GeneratedFrameOfReferenceUID(unsigned study,unsigned series)
		: DC3GeneratedUID()
		{
			SetFrameOfReference();
			SetUnsigned(study);
			SetUnsigned(series);
		}
	DC3GeneratedFrameOfReferenceUID(const char *s,unsigned study,
						unsigned series)
		: DC3GeneratedUID(s)
		{
			SetFrameOfReference();
			SetUnsigned(study);
			SetUnsigned(series);
		}
};

#endif /* _DC3tuidgen_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
