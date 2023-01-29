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
  CHANGES
  19991206 DE fix 64-bit compile error
  19990803 DE added to the library
 *..........................................................................*/


/*..........................................................................
| NAME
| :dtdb_table: Function Group - database table object
|
| SUMMARY
| The database table object ~dbtable~ encapsulates the ability
| to store a two-dimensional array of non-homogeneous data.
|
| The functions in this Function Group are:
| dtdb_table_init()			instantiate dtdb_table object 	
| dtdb_table_uninit()		uninitialize dtdb_table object 	
| dtdb_table_addfield()		add field to table				
| dtdb_table_searchfield())	search for field name in table	
| dtdb_table_findfield()	find field by name in table		
| dtdb_table_getfield()		get nth field					
| dtdb_table_addrecord()	add record to table				
| dtdb_table_setcell1()		set value of cell by name		
| dtdb_table_setcell2()		set value of cell by index		
| dtdb_table_getcell1()		get data value of cell by name	
| dtdb_table_getcell2()		get data value of cell by index	
| dtdb_table_searchcell1()	search for data value in cell by index
| dtdb_table_searchcell2()	search for data value in cell by index
| dtdb_table_html_header()	format table header in html string
| dtdb_table_html_row()		format table row in html string
| dtdb_table_html_fd()		format entire table in html file
| dtdb_table_sortcell1()	sort records in table
|
| Tables are composed of zero or more fields (columns) and 
| zero or more records (rows).
| 
| A field has:
| a unique name		string by which field can be referenced
| a unique index	integer by which field can be referenced
| a data type		nomimal data type used when sorting and formatting cells
| boolean flags		specifies various options for the field
| 
| A newly created table has no fields.
| The fields must be defined into the table before adding any rows.
| A field can be referenced by its unique name or by its index.
| 
| A row has an index and an array of cells.
| A row can only be referenced by its index.
| There are the same number of cells in each record as there are fields in the table.
| A record must be added to the table before its cells are assigned.
| 
| A cell consists of a data pointer and a length.
| Unless the :DTDB_FLAG_NOCOPY: bit is set in the field,
| new space is allocated for the cell when its data value is set.
|
| One scenario is:
| 1.	create empty table
| 2.	define fields in table
| 3.	create records in table
| 4.	populate cells in records
| 5.	sort table on field
| 6.	emit table as html
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: dbtable.c,v $ $Revision: 1.8 $");

#include <dtack/list.h>
#include <dtack/db.h>
#include <dtack/str.h>
#include <dtack/os.h>

/*..........................................................................
| NAME
| dtdb_table_init() - initialize database table object
|
| SUMMARY
| Initialize the database ~table~ object.
| Use table ~name~ in debugging.
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
  const char *name,						/* table name for debugging 		*/
  const char *spec)						/* spec identifying this server 	*/
{
  DT_F("dtdb_table_init");
  dt_rc_e rc = DT_RC_GOOD;
  DT_MEMSET(table, 0,					/* clear structure 					*/
    sizeof(*table));

  table->name = name;

  DT_C(dtlist_create,(ctl,				/* create fields list				*/
    &table->fields,
    "table fields"));
  table->fields_initialized = 1;

  DT_C(dtlist_create,(ctl,				/* create records list				*/
    &table->records,
    "table records"));
  table->records_initialized = 1;

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
  unsigned int n, i;
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  if (table->records_initialized)
  {
	DT_I(dtlist_count,(ctl,
      &table->records, &n));

	for (i=0; i<n; i++)
	{
	  DT_I(dtlist_nth,(ctl,
		&table->records, i, &elem));
	  if (elem->user)
	  {
		dtdb_record_t *r =
		  (dtdb_record_t *)elem->user;
		long j;
		for (j=0; j<table->nfields; j++)
		{
		  DT_I(dtlist_nth,(ctl,
		    &table->fields, j, &elem));
		  if (elem->user)
		  {
			dtdb_field_t *f =
			  (dtdb_field_t *)elem->user;
			if (!(f->flags & DTDB_FLAG_NOCOPY))
			{
			  if (r->cells[j].data)
				DT_I(dtos_free2,(ctl,
                  (void *)r->cells[j].data, 
                  F, "cell data"));
			}
		  }
		}
        DT_I(dtos_free2,(ctl, 
          r, F, "record"));
	  }
	}
	DT_I(dtlist_destroy,(ctl,
      &table->records));
  }

  if (table->fields_initialized)
  {
	DT_I(dtlist_free,(ctl,
      &table->fields));
	DT_I(dtlist_destroy,(ctl,
      &table->fields));
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
| Have an error if ~table~ already has data records in it.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_addfield(					/* add field to table				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_type_e type,						/* data type 						*/
  dtdb_flags_t flags)					/* field flags						*/
{
  DT_F("dtdb_table_addfield");
  char *t = NULL;
  dtdb_field_t *f;
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  if (table->nrecords)					/* table has records? 				*/
	DT_CLEANUP((ctl, F,
      "cannot add fields because" 
      " table has %ld records",
      table->nrecords));

  DT_C(dtdb_table_searchfield,(ctl,
    table, name, &f));

  if (f)								/* field exists already? 			*/
	DT_CLEANUP((ctl, F,
	  "table \"%s\" already has"
      " a field named \"%s\"",
      table->name, name));

  DT_C(dtos_malloc2,(ctl,				/* space for field copy 			*/
    (void **)&f, sizeof(*f),
    F, "f"));

  DT_MEMSET(f, 0, sizeof(*f));			/* clear the field 					*/
 
  DT_C(dtos_strdup,(ctl, name, &t));
  f->name = t;
  f->type = type;
  f->flags = flags;
  f->i = table->nfields;				/* remember the field's index 		*/

  DT_C(dtlist_after,(ctl,				/* add field to list of fields 		*/
    &table->fields, DTLIST_ELEM_TAIL,
    t, f, &elem));

  table->nfields++;						/* keep local field count 			*/

cleanup:
  if (rc != DT_RC_GOOD && t)
  {
	DT_I(dtos_free2,(ctl, f, F, "field"));
	DT_I(dtos_free2,(ctl, t, F, t));
	elem->name = NULL;
	elem->user = NULL;
  }
  
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
  dtdb_field_t **field)					/* returned field					*/
{
  DT_F("dtdb_table_searchfield");
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtlist_search,(ctl,				/* add field to list of fields 		*/
    &table->fields, name, &elem));

  if (elem)
	*field = (dtdb_field_t *)elem->user;
  else
	*field = NULL;

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
  dtdb_field_t *f;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdb_table_searchfield,(ctl,		/* search for field with name 		*/
    table, name, &f));

  if (f)								/* found the field? 				*/
	*field = f;
  else
	rc = dt_err(ctl, F,
      "field name \"%s\" not found"
      " in table \"%s\"",
      name, table->name);
  
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
  dtdb_field_t **field)					/* returned field					*/
{
  DT_F("dtdb_table_getfield");
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtlist_nth,(ctl,					/* get nth item from list 			*/
    &table->fields, nth, &elem));

  if (elem)								/* not that many items? 			*/
	*field = (dtdb_field_t *)
	  elem->user;
  else
	*field = NULL;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_addrecord() - add record to database table object
|
| SUMMARY
| Return a pointer to ~*record~ newly added to ~table~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_addrecord(					/* add record to table				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long *recordindex)					/* the record added					*/
{
  DT_F("dtdb_table_addrecord");
  dtdb_record_t *record = NULL;
  long n;
  dt_rc_e rc = DT_RC_GOOD;

  n = sizeof(dtdb_record_t) +			/* memory needed for record 		*/
    (table->nfields-1) *  
    sizeof(record->cells);
  
  DT_C(dtos_malloc2,(ctl,				/* space for cells 					*/
    (void **)&record, n,
    F, "record"));

  DT_MEMSET(record, 0, n);				/* clear record 					*/

  DT_C(dtlist_after,(ctl,				/* add record to list 				*/
    &table->records, 
    DTLIST_ELEM_TAIL,
    "record", record, NULL));

  *recordindex = table->nrecords;

  table->nrecords++;					/* keep local record count 			*/
  
cleanup:
  return rc;
}


/*..........................................................................
| NAME
| dtdb_table_setcell() - set value of cell 
|
| SUMMARY
| Set ~value~ of ~cell~ in ~record~.
| Allocate and copy ~value~ if ~copy~ is nonzero.
| Have an error if ~cell~ is and invalid name or index.
| Discover length of data if ~l~ is -1.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_setcell1(					/* set value of cell by name		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* the record containing the cell	*/
  const char *name,						/* name of field 					*/
  const void *data,						/* value of cell 					*/
  long l)								/* length of value 					*/
{
  DT_F("dtdb_table_setcell1");
  dtdb_field_t *field;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdb_table_findfield,(ctl,		/* get field by its name 			*/
    table, name, &field));

  DT_C(dtdb_table_setcell2,(ctl,		/* set cell data from index 		*/
    table, recordindex, field->i,
    data, l));

cleanup:
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_setcell2(					/* set value of cell by index		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* the record containing the cell	*/
  long fieldindex,						/* index of cell 					*/
  const void *data,						/* value of cell 					*/
  long l)								/* length of value 					*/
{
  DT_F("dtdb_table_setcell2");
  dtlist_elem_t *elem;
  dtdb_record_t *record;
  dtdb_field_t *field;
  dtdb_cell_t *cell;
  dt_rc_e rc = DT_RC_GOOD;

  if (fieldindex > table->nfields)		/* field index invalid? 			*/
	DT_CLEANUP((ctl, F,
      "table \"%s\" does not have %d cells"
      ", only %d",
      table->name, fieldindex,
      table->nfields));

  DT_C(dtlist_nth,(ctl,					/* fetch record 					*/
    &table->records, recordindex, 
    &elem));
  if (!elem)							/* no such record? 					*/
	DT_CLEANUP((ctl, F,
      "table \"%s\" has no record %ld (only %ld)",
      table->name, recordindex,
      table->records.n));

  record = (dtdb_record_t *)elem->user;

  DT_C(dtlist_nth,(ctl,					/* fetch nth field 					*/
    &table->fields, fieldindex, &elem));
  field = (dtdb_field_t *)elem->user;	/* address nth field 				*/

  cell = &record->cells[fieldindex];	/* address the cell 				*/

  if (l == -1)							/* discover length? 				*/
  switch(field->type)
  {
  case DTDB_TYPE_STRING: 
    l = strlen((const char *)data)+1;
  break;
  case DTDB_TYPE_LONG: 
    l = sizeof(long);
  break;
  case DTDB_TYPE_DOUBLE: 
    l = sizeof(double);
  break;
  default:
    DT_CLEANUP((ctl, F,
      "unable to discover length for"
      " unknown-type cell"));
  }

  if (!(field->flags &					/* supposed to copy value?			*/
		DTDB_FLAG_NOCOPY))
  {
	if (cell->data)						/* we currently have a copy? 		*/
	  DT_C(dtos_free2,(ctl,				/* release copy 					*/
        (void *)cell->data, 
        F, "cell copy"));
	cell->data = NULL;

	DT_C(dtos_malloc2,(ctl,				/* space for new data 				*/
      (void **)&cell->data, l,
      F, "cell copy"));
	DT_MEMCPY((void *)cell->data, 		/* copy the data 					*/
      data, l);
  }
  else
	cell->data = data;

  cell->l = l;
  
cleanup:
  return rc;
}


/*..........................................................................
| NAME
| dtdb_table_getcell() - get data value of cell 
|
| SUMMARY
| Get ~data~ value of ~cell~ in ~record~.
| Copy value to ~data~ up to maximum length ~l~.
| Return ~actual~ data length.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_getcell1(					/* get data value of cell by name	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* the record containing the cell	*/
  const char *cell,						/* name of cell 					*/
  void *data,							/* returned data of cell 			*/
  long l,								/* maximum allowed length of data	*/
  long *actual)							/* actual length of data			*/
{
  DT_F("dtdb_table_getcell1");
  dtdb_field_t *field;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdb_table_findfield,(ctl,		/* get field by its name 			*/
    table, cell, &field));

  DT_C(dtdb_table_getcell2,(ctl,		/* set cell data from index 		*/
    table, recordindex, field->i,
    data, l, actual));

cleanup:
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_getcell2(					/* get data value of cell by index	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* the record containing the cell	*/
  long fieldindex,						/* index of cell 					*/
  void *data,							/* returned data of cell 			*/
  long l,								/* maximum allowed length of data	*/
  long *actual)							/* actual length of data			*/
{
  DT_F("dtdb_table_getcell2");
  dtlist_elem_t *elem;
  dtdb_record_t *record;
  dtdb_cell_t *cell;
  dt_rc_e rc = DT_RC_GOOD;

  if (fieldindex > table->nfields)		/* field index invalid? 			*/
	DT_CLEANUP((ctl, F,
      "table \"%s\""
      " does not have %d cells,"
      " only %d",
      table->name, fieldindex,
      table->nfields));

  DT_C(dtlist_nth,(ctl,					/* fetch record 					*/
    &table->records, recordindex, 
    &elem));
  if (!elem)							/* no such record? 					*/
	DT_CLEANUP((ctl, F,
      "table \"%s\" has no record %ld",
      table->name, recordindex));

  record = (dtdb_record_t *)elem->user;
  cell = &record->cells[fieldindex];	/* address the cell 				*/

  if (cell->data)						/* cell contains data? 				*/
  {
	DT_MEMCPY(data, cell->data,			/* copy data to caller's space 		*/
      DT_MIN(l, cell->l));
  }

  if (actual)							/* caller wants to know length? 	*/
	*actual = cell->l;

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_searchcell() - search for data value of cell 
|
| SUMMARY
| Search for ~data~ value in ~cell~ of all records.
| Return matching ~recordindex~ index or -1 if not found.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_searchcell1(					/* search data of cell by name		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *cell,						/* name of cell 					*/
  const void *data,						/* returned data of cell 			*/
  long l,								/* maximum allowed length of data	*/
  long *recordindex)					/* index of matching record			*/
{
  DT_F("dtdb_table_searchcell1");
  dtdb_field_t *field;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdb_table_findfield,(ctl,		/* search field by its name 		*/
    table, cell, &field));

  DT_C(dtdb_table_searchcell2,(ctl,		/* set cell data from index 		*/
    table, field->i,
    data, l, recordindex));

cleanup:
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_searchcell2(					/* search data value by index		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long fieldindex,						/* index of cell 					*/
  const void *data,						/* returned data of cell 			*/
  long l,								/* maximum allowed length of data	*/
  long *recordindex)					/* index of matching record			*/
{
  DT_F("dtdb_table_searchcell2");
  dtlist_elem_t *elem;
  dtdb_record_t *record;
  dtdb_cell_t *cell;
  long i;
  dt_rc_e rc = DT_RC_GOOD;

  if (fieldindex > table->nfields)			/* field index invalid? 			*/
	DT_CLEANUP((ctl, F,
      "table \"%s\""
      " does not have %d cells,"
      " only %d",
      table->name, fieldindex,
      table->nfields));

  for (i=0; i<table->nrecords; i++)
  {
	DT_C(dtlist_nth,(ctl,				/* fetch record 					*/
      &table->records, i, &elem));
    if (!elem)							/* no such record? SNH!				*/
	  DT_CLEANUP((ctl, F,
        "table \"%s\" has no record %ld",
        table->name, recordindex));

	record = (dtdb_record_t *)
	  elem->user;
	cell = &record->cells[fieldindex];	/* address the cell 				*/

	if (cell->data)						/* cell contains data? 				*/
	  if (!memcmp(data, cell->data,		/* compare data to target			*/
             DT_MIN(l, cell->l)))
		break;
  }

  if (i == table->nrecords)				/* did not find record? 			*/
	*recordindex = -1;
  else
	*recordindex = i;

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
int
compare_string(							/* compare data value of cells		*/
  void *arg,
  void *item1,
  void *item2,
  unsigned int size)
{
#define COMPARE_START														  \
  long i = (long)arg;														  \
  dtlist_elem_t *elem1 = (dtlist_elem_t *)item1;							  \
  dtlist_elem_t *elem2 = (dtlist_elem_t *)item2;							  \
  dtdb_record_t *record1 = (dtdb_record_t *)elem1->user;					  \
  dtdb_record_t *record2 = (dtdb_record_t *)elem2->user;					  \
																			  \
  if (!record1->cells[i].data &&		/* neither cell has data?			*/\
	  !record2->cells[i].data)												  \
	return 0;																  \
  if (!record1->cells[i].data)			/* first cell has no data? 			*/\
	return -1;																  \
  if (!record2->cells[i].data)			/* second cell has no data?			*/\
	return 1;

  COMPARE_START
  return strcmp(
	record1->cells[i].data,
    record2->cells[i].data);
}

/*..........................................................................
 *..........................................................................*/

static
int
compare_long(							/* compare data value of cells		*/
  void *arg,
  void *item1,
  void *item2,
  unsigned int size)
{
  COMPARE_START
	
  return
	*(long *)(record1->cells[i].data) ==
	*(long *)(record2->cells[i].data)? 0:
	(*(long *)(record1->cells[i].data) <
	 *(long *)(record2->cells[i].data)? -1: 1);
}

/*..........................................................................
 *..........................................................................*/

static
int
compare_double(							/* compare data value of cells		*/
  void *arg,
  void *item1,
  void *item2,
  unsigned int size)
{
  COMPARE_START
	
  return
	*(double *)(record1->cells[i].data) ==
	*(double *)(record2->cells[i].data)? 0:
	(*(double *)(record1->cells[i].data) <
	 *(double *)(record2->cells[i].data)? -1: 1);
}

/*..........................................................................
| NAME
| dtdb_table_sortcell() - sort rows
|
| SUMMARY
| Sort rows in ~table~ based on value of cells whose ~fieldname~ or ~fieldinde~
| is given.
| Sort from smallest first if ~ascending~ is non-zero.
| Consider empty cells as the smallest possible value.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_sortcell1(					/* sort data of cell by name		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *fieldname,				/* name of cell 					*/
  int ascending)						/* sort order 						*/
{
  DT_F("dtdb_table_sortcell1");
  dtdb_field_t *field;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdb_table_findfield,(ctl,		/* sort field by its name 		*/
    table, fieldname, &field));

  DT_C(dtdb_table_sortcell2,(ctl,		/* set cell data from index 		*/
    table, field->i, ascending));

cleanup:
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_sortcell2(					/* sort data value of cell by index	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long fieldindex,							/* index of cell 					*/
  int ascending)						/* sort order 						*/
{
  DT_F("dtdb_table_sortcell2");
  dtdb_field_t *field;
  dt1d_compare2_f *compare;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtdb_table_getfield,(ctl,
    table, fieldindex, &field));

  switch(field->type)
  {
	case DTDB_TYPE_STRING:
	  compare = compare_string;
	break;
	case DTDB_TYPE_LONG:
	  compare = compare_long;
	break;
	case DTDB_TYPE_DOUBLE:
	  compare = compare_double;
	break;
	case DTDB_TYPE_IMAGE:
	  DT_CLEANUP((ctl, F,
        "unable to sort on %s-type fields",
        "image"));
	break;
	default:
	  DT_CLEANUP((ctl, F,
        "unable to sort on %d-type fields",
        field->type));
  }

  DT_C(dtlist_sort,(ctl,
	&table-> records,
    ascending,
    compare, 
    (void *)fieldindex));				/* pass-through to compare function */

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtdb_table_html_header() - format header record into html
|
| SUMMARY
| Format field header into html and place in ~line~ up to ~max~ length.
| Emit ~options~ without change at the proper places in the
| :tr: and :th:, tags.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_html_header(					/* format header record into html	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *row_options,				/* row options 						*/
  const char *cell_options,				/* cell options 					*/
  char *line,							/* output formatted line 			*/
  int max)								/* maximum length of line 			*/
{
  DT_F("dtdb_table_html_header");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtstr_printf,(ctl,
    line, max, "<tr %s>",
    row_options? row_options: ""));

  for (i=0; i<table->nfields; i++)
  {
	dtdb_field_t *field;
	DT_C(dtdb_table_getfield,(ctl,
      table, i, &field));
	DT_C(dtstr_printf,(ctl,
      line+strlen(line),
      max-strlen(line),
      "<th %s>",
      cell_options? cell_options: ""));
	DT_C(dtstr_htmlencode,(ctl,
      field->name,
      line+strlen(line),
      max-strlen(line)));
  }

  DT_C(dtstr_printf,(ctl,
    line+strlen(line),
    max-strlen(line),
    "\r\n"));
  
cleanup:
  return rc;
}
 
/*..........................................................................
| NAME
| dtdb_table_html_row() - format record into html
|
| SUMMARY
| Format record at ~recordindex~ into html and place in ~line~ up to ~max~ length.
| Emit ~options~ without change at the proper places in the
| :tr: and :td: tags.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_html_row(					/* format record into html			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* record in table to format		*/
  const char *row_options,				/* row options 						*/
  const char *cell_options,				/* cell options 					*/
  char *line,							/* output formatted line 			*/
  int max)								/* maximum length of line 			*/
{
  DT_F("dtdb_table_html_row");
  dtlist_elem_t *elem;
  dtdb_record_t *record;
  long i;
  dt_rc_e rc = DT_RC_GOOD;

  if (recordindex > table->nrecords)
	DT_CLEANUP((ctl, F,
      "table \"%s\" does not have %d records, only %d",
	  table->name, recordindex, table->nrecords));

  if (max <= 0)
	goto cleanup;
  
  DT_C(dtstr_printf,(ctl,
    line, max, "<tr %s>",
    row_options? row_options: ""));

  DT_C(dtlist_nth,(ctl,					/* get desired record				*/
    &table->records, recordindex, 
    &elem));

  record = (dtdb_record_t *)			/* address the record 				*/
	elem->user;

  for (i=0; i<table->nfields; i++)
  {
	dtdb_field_t *field;
	DT_C(dtdb_table_getfield,(ctl,
      table, i, &field));

	DT_C(dtstr_printf,(ctl,				/* write html field separator 		*/
      line+strlen(line),
      max-strlen(line), "<td %s>",
      cell_options? cell_options: ""));

	if (record->cells[i].data)			/* cell has data? 					*/
	switch(field->type)
	{
	case DTDB_TYPE_STRING:
	  DT_C(dtstr_htmlencode,(ctl,
        record->cells[i].data,
        line+strlen(line),
        max-strlen(line)));
	break;
	case DTDB_TYPE_LONG:
	  DT_C(dtstr_printf,(ctl,
        line+strlen(line),
        max-strlen(line), "%ld",
        *(long *)(record->cells[i].data)))
	break;
	case DTDB_TYPE_DOUBLE:
	  DT_C(dtstr_printf,(ctl,
        line+strlen(line),
        max-strlen(line), "%g",
        *(double *)(record->cells[i].data)));
	break;
	case DTDB_TYPE_IMAGE:
	  DT_C(dtstr_printf,(ctl,
        line+strlen(line),
        max-strlen(line), "[image]"));
	break;
	default:
	  DT_C(dtstr_printf,(ctl,
        line+strlen(line),
        max-strlen(line), "0x%08lx l=%d", 
        record->cells[i].data, 
        record->cells[i].l));
	  break;
	}
	else								/* cell has no data? 				*/
	  DT_C(dtstr_printf,(ctl,
        line+strlen(line),
        max-strlen(line), "&nbsp;"));
  }

  DT_C(dtstr_printf,(ctl,
    line+strlen(line),
    max-strlen(line),
    "\r\n"));

cleanup:
  return rc;
}
 
/*..........................................................................
| NAME
| dtdb_table_html_fd() - format table to output file
|
| SUMMARY
| Format ~table~ into html and write to already-opened output ~fd~.
| Do not close ~fd~ when done.
| Use :stdout: if ~fd~ is :NULL:.
| Emit ~options~ without change at the proper places in the
| :table:, :tr:, :th:, and :td: tags.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdb_table_html_fd(						/* format table to output file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *table_options,			/* table options 					*/
  const char *row_options,				/* row options 						*/
  const char *cell_options,				/* cell options 					*/
  dtfd_t *fd)							/* already-opened output file 		*/
{
  DT_F("dtdb_table_html_fd");
  char s[2048];
  long i;
  dtfd_t std;
  dt_rc_e rc = DT_RC_GOOD;

  if (!fd)
  {
	fd = &std;
	DT_C(dtfd_fd,(ctl, fd,
      "stdout", fileno(stdout)));
  }

  DT_C(dtstr_printf,(ctl, s, sizeof(s),
    "<table %s>\r\n",
    table_options? table_options: ""));
  DT_C(dtfd_write_all,(ctl, fd,
    s, -1));

  DT_C(dtdb_table_html_header,(ctl,		/* print column headers 			*/
    table, row_options, cell_options, 
    s, sizeof(s)));
  DT_C(dtfd_write_all,(ctl, fd,
    s, -1));

  for (i=0; i<table->nrecords; i++)		/* for each record in table */
  {
	DT_C(dtdb_table_html_row,(ctl,
      table, i,
      row_options, cell_options, 
      s, sizeof(s)));
	DT_C(dtfd_write_all,(ctl, fd,
      s, -1));
  }

  DT_C(dtfd_write_all,(ctl, fd,
    "</table>\r\n", -1));

cleanup:
  if (fd == &std)
  {
	fd->fd = -1;						/* don't really close 				*/
	DT_I(dtfd_close,(ctl, fd));
  }
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
