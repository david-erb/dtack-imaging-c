/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
| NAME
| :dtdb_table: Function Group - database table object
|
| SUMMARY
| The datagram server object ~dgserver~ encapsulates the ability
| of a process to receive datagram requests.
|
| A companion object called ~dgclient~ provides the ability for a process
| to make requests of a ~dgserver~.
| 
| From the server's point of view, the conversation goes like this:
| 1.	call dtdb_table_init()
| 2.	call dtdb_table_is_request() to poll for incoming requests
| 3.	call dtdb_table_get_request() to get incoming request string
| 4.	call dtdb_table_response() multiple times until response is complete
| 5.	call dtdb_table_done() to finish response
| 6.	go back to dtdb_table_is_request() step
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/dg $RCSfile: dgserver.c,v $ $Revision: 1.8 $");

#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/str.h>
#include <dtack/os.h>

#define CR 0x0d
#define LF 0x0a

/*..........................................................................
| NAME
| dtdb_table_init() - initialize database table object
|
| SUMMARY
| Initialize the database ~table~ object.
| Expect ~spec~ to the of the form #scheme,subspec#.
| Have an error if #scheme# is not one of a predefined set.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_init(						/* instantiate dtdb_table object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *spec)						/* spec identifying this server 	*/
{
  DT_F("dtdb_table_init");
  dt_rc_e rc = DT_RC_GOOD;
  DT_MEMSET(table, 0,					/* clear structure 					*/
    sizeof(*table));

  DT_C(dtlist_create,(ctl,				/* create fields list				*/
    &table->_fields,
    "table fields"));
  table->_fields_initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	DT_I(dtdb_table_uninit,(ctl, table));
  }

  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_uninit() - uninitialize database table object
|
| SUMMARY
| Release all resources used by the database ~table~ object.
| The ~table~ object may not be used again unless passed to dtdb_table_init().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_uninit(						/* uninitialize dtdb_table object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table)					/* this dtdb_table object 			*/
{
  DT_F("dtdb_table_uninit");
  dt_rc_e rc = DT_RC_GOOD;

  if (table->_fields_initialized)
  {
	DT_I(dtlist_free,(ctl,
      &table->_fields));
	DT_I(dtlist_destroy,(ctl,
      &table->_fields));
  }

  DT_MEMSET(table, 0,					/* clear structure 					*/
    sizeof(*table));

  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_addfield() - add field to database table object
|
| SUMMARY
| Add ~field~ named ~name~ to ~table~.
| Have an error if ~name~ is already a field in ~table~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_addfield(					/* add field to table				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_field_t *field)					/* the field to add 				*/
{
  DT_F("dtdb_table_addfield");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_searchfield() - get field by name from database table object
|
| SUMMARY
| Return a pointer to ~*field~ named ~name~ from ~table~.
| Return ~*field~ set to :NULL: if ~name~ is not a field in ~table~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_searchfield(					/* search for field name in table	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_field_t **field)					/* return field						*/
{
  DT_F("dtdb_table_searchfield");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_findfield() - get field by name from database table object
|
| SUMMARY
| Return a pointer to ~*field~ named ~name~ from ~table~.
| Have an error if ~name~ is not a field in ~table~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_findfield(					/* find field by name in table		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_field_t **field)					/* return field						*/
{
  DT_F("dtdb_table_findfield");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_getfield() - get nth field
|
| SUMMARY
| Return a pointer to nth ~*field~ from ~table~.
| Return :NULL: if ~table~ has fewer then ~nth~ fields.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_getfield(					/* get nth field					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long nth,								/* field number wanted				*/
  dtdb_field_t **field)					/* return field						*/
{
  DT_F("dtdb_table_getfield");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_addrecord() - add record to database table object
|
| SUMMARY
| Return a pointer to nth ~*field~ from ~table~.
| Return :NULL: if ~table~ has fewer then ~nth~ fields.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_addrecord(					/* add record to table				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  dtdb_record_t *record)				/* the record to add 				*/
{
  DT_F("dtdb_table_addrecord");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_addrecord() - get nth record from table
|
| SUMMARY
| Return a pointer to nth ~*record~ from ~table~.
| Return :NULL: if ~table~ has fewer then ~nth~ records.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_getrecord(					/* get nth record from table		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long nth,								/* record number wanted				*/
  dtdb_record_t **record)				/* the record to add 				*/
{
  DT_F("dtdb_table_addrecord");
  dt_rc_e rc = DT_RC_GOOD;

cleanup:
  return rc;
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
