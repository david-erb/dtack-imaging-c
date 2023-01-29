#ifdef __cplusplus
extern "C" {
#endif

/*----in dschedl.c:----*/

dt_rc_e
DTCONFIG_API1
dsched_database(
  dtparam_t *param,
  const char *id,
  char *filename,
  int max);
void
DTCONFIG_API1
dsched_init(		
  dsched_t *dsched,
  dtparam_t *param,
  dtipc_t *ipc);
void
DTCONFIG_API1
dsched_uninit(		
  dsched_t *dsched);
void
DTCONFIG_API1
dsched_bitset(							/* turn on bits						*/
  dsched_t *dsched,
  int start,
  int end,
  unsigned char *bitmap);
void
DTCONFIG_API1
dsched_bitmap(							/* convert rules to 86400-bit map	*/
  dsched_t *dsched,
  int wantday,
  unsigned char *bitmap);
void
DTCONFIG_API1
dsched_natural_time(					/* time in natural language			*/
  dsched_t *dsched,
  int seconds,
  char *t);
char *
dsched_natural_expand(					/* time in natural language			*/
  dsched_t *dsched,
  const char *string,
  char *t);
void
DTCONFIG_API1
dsched_natural(							/* rule in natural language			*/
  dsched_t *dsched,
  int rule,
  char *s,
  int max);
FILE *
dsched_open(
  dsched_t *dsched,
  const char *filename,
  const char *mode);
void
DTCONFIG_API1
dsched_time_to_seconds(					/* convert HH:MM.SS to seconds  	*/
  dsched_t *dsched,
  const char *string,
  int *seconds);
void
DTCONFIG_API1
dsched_check(							/* check rule						*/
  dsched_t *dsched,
  const char *day,
  const char *start,
  const char *end,
  const char *period,
  const char *duration,
  int *ok);
void
DTCONFIG_API1
dsched_rule_free(						/* delete rule from structure 		*/
  dsched_t *dsched,
  dsched_rule_t *rule);
void
DTCONFIG_API1
dsched_add(								/* add rule to structure 			*/
  dsched_t *dsched,
  const char *day,
  const char *start,
  const char *end,
  const char *period,
  const char *duration);
void
DTCONFIG_API1
dsched_load(							/* load structure from database 	*/
  dsched_t *dsched,
  const char *database);
dt_rc_e
DTCONFIG_API1
dsched_camlist(
  dt_ctl_t *ctl,
  const char *config,
  dtlist_t *list);
#ifdef __cplusplus
}
#endif
