/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tmodulet.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */
#ifndef _DC3tmodulet_HEADER_
#define _DC3tmodulet_HEADER_

#include "tlist.h"
#include "tstrval.h"
#include "tbinval.h"
#include "tvfyval.h"

class DC3Module {
public:
};

DC3Attribute *getDataElementUntyped(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element);
DC3Attribute *getDataElementType1(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element);
DC3Attribute *getDataElementType1C(DC3ListofAttributes *list,DC3Tag tag,
		int (*condition)(DC3ListofAttributes *),
		Uint16 multiplicity,const char *module,const char *element);
DC3Attribute *getDataElementType2(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element);
DC3Attribute *getDataElementType2C(DC3ListofAttributes *list,DC3Tag tag,
		int (*condition)(DC3ListofAttributes *),
		Uint16 multiplicity,const char *module,const char *element);
DC3Attribute *getDataElementType3(DC3ListofAttributes *list,
		DC3Tag tag,Uint16 multiplicity,
		const char *module,const char *element);

#endif /* _DC3tmodulet_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
