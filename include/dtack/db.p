#ifdef __cplusplus
extern "C" {
#endif
#ifdef DX_PREFIX
#  undef DX_PREFIX
#endif
#ifndef DT_PREFIX
#  define DX_PREFIX
#else
#  define DX_PREFIX DT_PREFIX
#endif
#ifdef DX_SUFFIX
#  undef DX_SUFFIX
#endif
#ifndef DT_SUFFIX
#  define DX_SUFFIX ;
#else
#  define DX_SUFFIX DT_SUFFIX
#endif
#ifdef DT_DEFARGVAL
#  undef DT_DEFARGVAL
#  define DT_DEFARGVAL(A) = (A)
#endif

/*----in dbtable.c:----*/


DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_init(						/* instantiate dtdb_table object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* table name for debugging 		*/
  const char *spec)						/* spec identifying this server 	*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_uninit(						/* uninitialize dtdb_table object 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table)					/* this dtdb_table object 			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_addfield(					/* add field to table				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_type_e type,						/* data type 						*/
  dtdb_flags_t flags)					/* field flags						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_searchfield(					/* search for field name in table	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_field_t **field)					/* returned field					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_findfield(					/* find field by name in table		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *name,						/* name for field in this table 	*/
  dtdb_field_t **field)					/* return field						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_getfield(					/* get nth field					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long nth,								/* field number wanted				*/
  dtdb_field_t **field)					/* returned field					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_addrecord(					/* add record to table				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long *recordindex)					/* the record added					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_setcell1(					/* set value of cell by name		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* the record containing the cell	*/
  const char *name,						/* name of field 					*/
  const void *data,						/* value of cell 					*/
  long l)								/* length of value 					*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_setcell2(					/* set value of cell by index		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long recordindex,						/* the record containing the cell	*/
  long fieldindex,						/* index of cell 					*/
  const void *data,						/* value of cell 					*/
  long l)								/* length of value 					*/
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_searchcell1(					/* search data of cell by name		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *cell,						/* name of cell 					*/
  const void *data,						/* returned data of cell 			*/
  long l,								/* maximum allowed length of data	*/
  long *recordindex)					/* index of matching record			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_searchcell2(					/* search data value by index		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long fieldindex,						/* index of cell 					*/
  const void *data,						/* returned data of cell 			*/
  long l,								/* maximum allowed length of data	*/
  long *recordindex)					/* index of matching record			*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_sortcell1(					/* sort data of cell by name		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *fieldname,				/* name of cell 					*/
  int ascending)						/* sort order 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_sortcell2(					/* sort data value of cell by index	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  long fieldindex,							/* index of cell 					*/
  int ascending)						/* sort order 						*/
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_html_header(					/* format header record into html	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *row_options,				/* row options 						*/
  const char *cell_options,				/* cell options 					*/
  char *line,							/* output formatted line 			*/
  int max)								/* maximum length of line 			*/
DX_SUFFIX

DX_PREFIX
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
DX_SUFFIX

DX_PREFIX
dt_rc_e
DTCONFIG_API1
dtdb_table_html_fd(						/* format table to output file		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtdb_table_t *table,					/* this dtdb_table object 			*/
  const char *table_options,			/* table options 					*/
  const char *row_options,				/* row options 						*/
  const char *cell_options,				/* cell options 					*/
  dtfd_t *fd)							/* already-opened output file 		*/
DX_SUFFIX

/* ---------------------- */
#undef DT_DEFARGVAL
#undef DX_SUFFIX
#undef DX_PREFIX
#define DT_DEFARGVAL(A)
#ifdef __cplusplus
}
#endif
