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
| uclient - streaming transfer
| 
|.SYNOPSIS
|:uclient -op [server | command] [params]
|
|.SUMMARY
| Generic streaming client/server.
| 
|.PARAMETERS
| :-op:			operation
| :-connect:	network address to adopt or seek
| :-input:		input
| :-ident:		http basic authentication username:password (optional)
| :-name:		filename to post (optional)
|dtinclude(params1.dx)
|
|.DESCRIPTION
| The program does one operation per invocation.
| The operations are:
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/uclient $RCSfile: uclient.c,v $ $Revision: 1.8 $");
#undef DTPRJ_CONSOLE
#define DTPRJ_CONSOLE DT_0				/* make this a non-console app 		*/

#include <dtack/main.h>
#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/dg.h>
#include <dtack/str.h>
#include <dtack/rev.h>
#include <dtack/os.h>

#include <uclient.use>

typedef struct {						/* application structure			*/
  int op;
} uclient_app_t;

/*..........................................................................
 *..........................................................................*/

static
void
uclient_stamp(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  char *s,
  int lmax)
{
  int pid;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/
  strftime(s, lmax,						/* express as string 				*/
    "%Y%m%d.%H%M.%S", t);

  dtos_getpid(ctl, &pid);

#ifdef DTCONFIG_IMPLIED_MSW
#define PIDFORMAT "0x%08lx"
#else
#define PIDFORMAT "%5d"
#endif

  dtstr_printf(ctl,						/* tack on the caller name and pid	*/
    s+strlen(s), lmax-strlen(s),
    " [" PIDFORMAT "]%s%s", pid, 
    caller? " ": "",
    caller? caller: "");
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
uclient_dbg(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  long class_bits,						/* debug message class 				*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  uclient_stamp(ctl, caller, 
    s, sizeof(s));
  ctl->dbg = NULL;						/* don't come back here 			*/
  dt_dbg_va(ctl, s, class_bits,			/* let dt_dbg_va() do all the work 	*/
    format, ap);
  ctl->dbg = uclient_dbg;				/* come back here next time 		*/
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e
DTCONFIG_API1
uclient_err(							/* handle errors					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  dt_rc_e rc;
  char s[64];
  char t[8096];
  const char *logfail = "";
  uclient_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->err = NULL;						/* don't come back here 			*/
  dtstr_printf_va(ctl, t, sizeof(t),
    format, ap);

  if (ctl->user &&						/* special logging?				    */
	  *(char *)ctl->user)
  {
	FILE *file;
	file = fopen((const char *)ctl->user, "wt");
	if (file)
	{
	  fputs(t, file);
	  fclose(file);
	}
	else
	{
	  logfail = " (special log failed)";
	}
	ctl->user = NULL;
  }
  rc = dt_err(ctl, s, "%s%s", t, logfail);			/* log the error 					*/
  ctl->err = uclient_err;				/* come back here next time 		*/
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

void
DTCONFIG_API1
uclient_say(							/* output console message			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *caller,					/* caller's name 					*/
  const char *format,					/* message format, like sprintf() 	*/
  va_list ap)							/* format substitutions 			*/
{
  char s[64];
  uclient_stamp(ctl, caller, 
    s, sizeof(s));

  ctl->say = NULL;						/* don't come back here 			*/
  dt_say_va(ctl, s,
    format, ap);
  ctl->say = uclient_say;				/* come back here next time 		*/
}

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

dt_rc_e 
DTCONFIG_API1
uclient_transact(
  dtparam_t *param)
{
  DT_F("uclient_transact");
  dt_ctl_t *ctl = &param->ctl;
  char *request = NULL;
  int request_length;
  dtdg_client_t _dgclient, *dgclient = &_dgclient;
  dtfd_t ifd;
  int dgclient_initialized = 0;
  int is_available;
  char host[256];
  char boundary[256];
  long mask = DT_DBG_MASK_REPLY;
  dtlist_t header_fields;
  dtlist_elem_t *elem;
  long content_length;
  char code[32];
  dt_rc_e rc;

  DTFD_CLEAR(ctl, &ifd);

  DT_C(dtlist_create,(ctl, 
    &header_fields, "header fields"));

  request_length = 32 +					/* extra space for HTTP stuff 		*/
    strlen(param->input);

  DT_C(dtos_malloc2,(ctl,				/* space for request string 		*/
    (void **)&request, 
    request_length, 
    F, "request"));

  if (strstr(param->op, "post"))		/* posting the request?				*/
  {	
	dt1d_ran_t ran;
	double r1, r2;
	char t[2048];
	DT_C(dtstr_printf,(ctl,				/* prepend http GET 				*/
      request, request_length,
      "POST /%s HTTP/1.0", param->input));
	DT_C(dtos_futil_size,(ctl, param->name, &content_length));
	DT_C(dtfd_open,(ctl, &ifd, param->name, "r"));

#if 0
										/* send the RFC-1867 header			*/
    dt1d_ran_uniform_init(&ran, time(NULL));
	dt1d_ran_uniform(&ran, &r1);
	dt1d_ran_uniform(&ran, &r2);
	DT_C(dtstr_printf,(ctl, boundary, sizeof(boundary),
      "--%ld--------------------%ld",
	  (long)(r1*1000000), (long)(r2*1000000)));

	DT_C(dtstr_printf,(ctl, t, sizeof(t),
      "multipart/form-data; boundary=%s", boundary+2));

	DT_C(dtlist_after,(ctl,
      &header_fields, DTLIST_ELEM_TAIL,
	  "Content-Type", t, NULL));
#else
	DT_C(dtlist_after,(ctl,
      &header_fields, DTLIST_ELEM_TAIL,
	  "Content-Type", "application/octet-stream", NULL));
#endif

	DT_C(dtstr_printf,(ctl, t, sizeof(t),
	  "%ld", content_length));
	DT_C(dtlist_after,(ctl,
      &header_fields, DTLIST_ELEM_TAIL,
      "Content-length", t, NULL));
  }
  else
  {
	DT_C(dtstr_printf,(ctl,				/* prepend http GET 				*/
      request, request_length,
      "GET /%s HTTP/1.0", param->input));
  }

										/* ................................ */

  if (param->ident && *param->ident)
  {
	/* Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ==					*/

	const char *name = param->ident;
	const char *b = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	  "abcdefghijklmnopqrstuvwxyz0123456789+/"; 
	int i, j, l, n;
	char s[4096];
	n = strlen(param->ident);
	strcpy(s, "Basic ");
	j = strlen(s);
	for (i=0; i<n; i+=3)
    {
	  int l = (name[i] << 16) +
        ((i+1 < n? name[i+1]: 0) << 8) +
        ((i+2 < n? name[i+2]: 0));
	  s[j++] = b[(l >> 18) & 0x3f];
	  s[j++] = b[(l >> 12) & 0x3f];
	  s[j++] = b[(l >>  6) & 0x3f];
	  s[j++] = b[(l      ) & 0x3f];
	}
	if (n % 3 == 1)
	  s[j-2] = '=';
	if (n % 3 > 0)
	  s[j-1] = '=';
	s[j] = '\0';
	DT_C(dtlist_after,(ctl,
      &header_fields, DTLIST_ELEM_TAIL,
      "Authorization", s, NULL));
  }

										/* ................................ */
  {
#   define ROCKET "rocket,"
	char *q;
	const char *p = strstr(
      param->connect, ROCKET);
	if (p)
	  p += strlen(ROCKET);
	else
	  p = param->connect;
	DT_C(dtstr_cpy,(ctl, host, sizeof(host), p));
	q = strstr(host, ":");
	if (q) *q = '\0';
	DT_C(dtlist_after,(ctl,
      &header_fields, DTLIST_ELEM_TAIL,
      "Host", (char *)host, NULL));
  }
										/* ................................ */
  DT_C(dtdg_client_init2,(ctl,			/* send request to server 			*/
    dgclient,
    param->connect, request, 
    &header_fields,
    param->dt));
  dgclient_initialized = 1;

										/* ................................ */
  if (strstr(param->op, "post"))		/* posting the request?				*/
  {
	long sent = 0;
#if 0
	char t[2048];
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, boundary, strlen(boundary)));
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, "\r\n", 2));
										/* send the RFC-1867 subheader		*/
	DT_C(dtstr_printf,(ctl, t, sizeof(t),
      "form-data; name=\"file\"; filename=\"%s\"", 
	  param->name));
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, t, strlen(t)));
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, "\r\n", 2));
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, "\r\n", 2));
#endif

	while(1)
    {
#     define BMAX (8192)
	  char b[BMAX];
	  long got;

	  DT_C(dtstr_printf,(ctl, b, BMAX,	/* make a yield message				*/
		"send data loop %ld%",
		(long)(0.5+(100.0*sent)/content_length), "%"));
	  DT_C(dt_yield,(ctl, "%s", b));

	  DT_C(dtfd_read,(ctl, &ifd,		/* read the next chunk				*/
	    b, BMAX, &got));
	  if (!got)	
		break;
	  DT_C(dtipc_write_all,(ctl,		/* send the next chunk				*/
        dgclient->ipc, b, got));

	  sent += got;
	}

	DT_C(dt_yield,(ctl, 
	  "body finished 100%s", "%"));

#if 0
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, boundary, strlen(boundary)));
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, "--\r\n", 4));
	DT_C(dtipc_write_all,(ctl, dgclient->ipc, "\r\n", 2));
#endif
  }

  DT_C(dtdg_client_wait_header,(ctl,	/* wait for header to appear		*/
    dgclient, param->dt,
    &is_available));
  if (!is_available)
  {
	rc = dt_err(ctl, F,
      "server %s not responding",
      param->connect);
	goto cleanup;
  }
  
  DT_C(dtdg_header_get,(ctl,			/* pick off the header 				*/
    dgclient, param->dt));
  
  DT_C(dt_yield,(ctl, "got header"));

  DT_C(dtdg_client_get_response,(ctl,	/* pick off the response			*/
    dgclient, param->dt,
    (long)param->max, NULL));

  DT_C(dt_yield,(ctl, "got response"));

  DT_C(dtlist_nth,(ctl,					/* examine first line of header 	*/
    &dgclient->header_list, 0, &elem));

  DT_C(dtstr_extract,(ctl, elem->name, 
    " ", 1, code, sizeof(code)));

  if (strcmp(code, "200"))
    DT_CLEANUP((ctl, F,
  	  "invalid HTTP response \"%s\""
      " (code \"%s\")", elem->name, code));

  if (param->variant &&					/* error message field is defined?	*/
	  param->variant[0])
  {
	const char *message;
    DT_C(dtdg_client_response_search,(	/* check for error message field	*/
      ctl, dgclient, 
      param->variant, &message));
    if (message)
	  DT_CLEANUP((ctl, F, "%s", message));
  }

  if (param->screen && param->screen[0])
  {
    const char *rcvalue;
    long rclong;
    DT_C(dtdg_client_response_find,(ctl, dgclient, param->screen, &rcvalue));
    DT_C(dtstr_to_long,(ctl, rcvalue, &rclong));
    rc = (dt_rc_e)rclong;
  }

cleanup:
  if (request != NULL)
    DT_I(dtos_free2,(ctl, request,
      F, "request"));

  if (dgclient_initialized)
  DT_I(dtdg_client_uninit,(ctl,
    dgclient));

  DT_I(dtfd_close,(ctl, &ifd));

  DT_I(dtlist_destroy,(ctl,
    &header_fields));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
uclient_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *message,
  const char **start)
{
  DT_F("uclient_arg_check");

  if (!param->connect ||
	  !*param->connect)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
	  "enter the host ip:port -connect");
    *start = param->connect_desc;
	return DT_RC_GOOD;
  }

  if (!param->input ||
	  !*param->input)
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
	  "enter the URL file part as -input");
    *start = param->input_desc;
	return DT_RC_GOOD;
  }

  if (strstr(param->op, "post") &&
	  (!param->name ||
	   !*param->name))
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
	  "enter filename to post as -name");
    *start = param->name_desc;
	return DT_RC_GOOD;
  }

  if (strstr(param->op, "save") &&
	  (!param->output ||
	   !*param->output))
  {
	dtstr_printf(ctl, 
      message, DT_ENTER_MSG_MAXLEN,
	  "enter filename to save as -output");
    *start = param->output_desc;
	return DT_RC_GOOD;
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
uclient_rev(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "uclient app";
  if (rev_major != NULL)
    *rev_major = 3;
  if (rev_minor != NULL)
    *rev_minor = 3;
  if (rev_date != NULL)
    *rev_date = 20091108;
  return DT_RC_GOOD;
}


static
dtrev_f *revs[] = {
  uclient_rev,
  dtdg_rev,
  dtipc_rocket_rev,
  NULL
};

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/
  
DTMAIN1(uclient)
{
  DT_F("uclient");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  uclient_app_t uclient_app;
  long rep;
  dtos_time_t mark1, mark2;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));			/* init param structure             */

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_PRIMARY |
    DTPARAM_FLAG_LENGTHY,
    &param->connect,					/* ipc spec							*/
      "Ipc spec", NULL, 
      NULL,
	&param->name,						/* filename to post					*/
      "Post filename", NULL, 
      "",
    &param->input,						/* input command					*/
      "URL filepart", NULL, 
      NULL,
    &param->ident,						/* http basic authorization			*/
      "Ident", NULL, 
      "",
  	&param->screen,						/* return code field name			*/
      "RC field", NULL, 
      "",
	&param->variant,					/* return message field name		*/
      "Message field", NULL, 
      "",
	&param->max,						/* max lines to return				*/
      "Max lines", NULL, 
      "300",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY,
    &param->dt,							/* reply timeout					*/
      "Reply timeout", NULL, 
      "2000",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY,
    &param->n,							/* number of loops, 0 for forever	*/
      "Loops", NULL, 
      "1",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY,
    &param->mu,							/* delay between loops				*/
      "Period", NULL, 
      "60000",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->op, 
      "Operation", NULL, 
      "normal",
    NULL));

  DT_C(dtparam_set,(param,
    DTPARAM_FLAG_SECONDARY |
    DTPARAM_FLAG_LENGTHY,
    &param->dialog, 
      "First error", NULL, 
      "",
    NULL));

  arg[0].desc = NULL;                   /* no positional arguments          */

  DT_C(dt_enter_parse,(					/* parse command line				*/
    argc, argv, F, usage, param,
    arg, uclient_arg_check, 
    &uclient_app));
										/* ................................ */
  ctl->dbg = uclient_dbg;				/* special logging needs 			*/
  ctl->err = uclient_err;
  ctl->say = uclient_say;
										/* ................................ */
  

  DT_C(dtos_time,(ctl, &mark1));

  if (DT_ABBREV(param->op, "rev"))		/* op is to say rev and quit?		*/
  {
	DT_C(dtrev_say,(ctl, revs));
  }
  else
  {
	long i;
	dtrev_dbg(ctl, DT_DBG_MASK_USR1, revs);

	  
	if (param->dialog &&				/* wanting first-error log?			*/
		*param->dialog)
	{
	  FILE *file;
	  file = fopen(param->dialog, "wt");
	  if (file)
	  {
		fclose(file);
		ctl->user = (void *)param->dialog;
	  }
	  else
		DT_CLEANUP((ctl, F, "unable to log to %s", param->dialog));
	}

	for (i=0; param->n==0 || i<param->n; i++)
	{
	  DT_G(uclient_transact,(param));
	  if (param->n == 0 ||
		  i < param->n-1)
	  {
		const long nap = 1000;
		long k;
		for (k=0; k<param->mu; k+=nap)
		{
		  DT_C(dt_yield,(ctl, "loop %ld, waking in %ld", i, (long)param->mu-k));
		  DT_C(dtos_delay,(ctl, nap));
		}
	  }
	}
  }
  
  DT_C(dtos_time,(ctl, &mark2));
  dtos_time_report(ctl,
    &mark1, &mark2, param->input);
  
										/* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/
    
  ctl->dbg = NULL;
  ctl->err = NULL;
  ctl->say = NULL;
  
  {
	int check_fail = 0;
    DT_I(dt_heapmon_uninit,(ctl,		/* check all memory is freed        */
      &check_fail));
    if (rc == DT_RC_GOOD &&
		check_fail)						/* any failed assertions?           */
      rc = DT_RC_BAD;
  }

  dt_dbg(ctl, F, 0, NULL);				/* close files 						*/
  dt_say(ctl, F, NULL);
  dt_err(ctl, F, NULL);

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
