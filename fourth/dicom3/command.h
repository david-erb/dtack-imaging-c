/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* command.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */
#ifndef _command_HEADER_
#define _command_HEADER_

#include "tlist.h"
#include "tddictc.h"
#include "commandc.h"

class DC3CommandBase {
protected:
	DC3ListofAttributes list;
public:
	virtual void put(ostream& stream) = 0;
	virtual void write(DC3ostream& stream) = 0;
};

class DC3Command_C_STORE_RQ : public DC3CommandBase {
public:
	DC3Command_C_STORE_RQ(
		char *AffectedSOPClassUID,
		Uint16 MessageID,
		Uint16 Priority,
		Uint16 DataSetType,
		char *AffectedSOPInstanceUID,
		char *MoveOriginatorApplicationEntityTitle,
		Uint16 MoveOriginatorMessageID,
		char *MessageSetID,
		char *EndMessageSet
	);

	void put(ostream& stream)
		{
			stream << "Command C_STORE_RQ:\n" << flush;
			list.put(stream);
		}

	void write(DC3ostream& stream)
		{
			list.write(stream);
		}
};

class DC3Command_C_ECHO_RQ : public DC3CommandBase {
public:
	DC3Command_C_ECHO_RQ(
		char *AffectedSOPClassUID,
		Uint16 MessageID
	);

	void put(ostream& stream)
		{
			stream << "Command C_ECHO_RQ:\n" << flush;
			list.put(stream);
		}

	void write(DC3ostream& stream)
		{
			list.write(stream);
		}
};

#endif /* _command_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
