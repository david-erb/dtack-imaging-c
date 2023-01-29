/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* ttag.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3ttag_HEADER_
#define _DC3ttag_HEADER_

#include "tstream.h"
#include "ttypes.h"

class DC3Tag {
	Uint16	Group;
	Uint16	Element;
public:
	DC3Tag(void)			{ Group=0; Element=0; }
	DC3Tag(Uint16 g, Uint16 e)	{ Group=g; Element=e; }

	DC3Tag(const DC3Tag& t)		{ Group=t.Group; Element=t.Element; }
	void operator=(const DC3Tag& t)	{ Group=t.Group; Element=t.Element; }

	int operator==(const DC3Tag& t2)
		{ return Group == t2.Group && Element == t2.Element; }
	int operator<(const DC3Tag& t2)
		{ return Group < t2.Group
		      || (Group == t2.Group && Element < t2.Element); }
	int operator<=(const DC3Tag& t2)
		{ return Group < t2.Group
		      || (Group == t2.Group && Element <= t2.Element); }
	int operator>(const DC3Tag& t2)
		{ return Group > t2.Group
		      || (Group == t2.Group && Element > t2.Element); }
	int operator>=(const DC3Tag& t2)
		{ return Group > t2.Group
		      || (Group == t2.Group && Element >= t2.Element); }

	Uint16	getElement(void)	{ return Element; }
	Uint16	getGroup(void)		{ return Group; }

	// Private Groups ... see PS3.5-1993-7.1

	int	isPrivateGroup(void)	{ return (Group % 2) == 1
					      && Group > 0x0007
					      && Group != 0xffff; }

	int	isPrivateOwner(void)	{ return Element >= 0x0010
					      && Element <= 0x00ff; }

	int	isPrivateTag(void)	{ return Element >= 0x1000
					      && Element <= 0xffff; }

	int	isRepeatingGroup(void)	{ return (Group >= 0x5000 
					       && Group <= 0x501e)
					      || (Group >= 0x6000 
					       && Group <= 0x601e); }

	void	putNoDesc(ostream& stream);
	void	put(ostream& stream);
	void	read(DC3istream& stream);
	void	write(DC3ostream& stream);
	void	unread(DC3istream& stream);
};

#define DC3TagFromName(name)	\
		DC3TagFromConstants(DC3##name##_GROUP,DC3##name##_ELEMENT)

inline DC3Tag
DC3TagFromConstants(Uint16 group,Uint16 element)
{
	DC3Tag tag (group,element);
	return tag;
}

#endif /* _DC3ttag_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
