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
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>

DT_RCSID("test $RCSfile: dttalpha.c,v $ $Revision: 1.4 $");

#include <dtack/note.h>
#include <dtack/dialog.h>
#include <dtack/alpha.h>

#include <dtack/mem.h>

#include <dtack/cons.h>

#include <dtack/prj.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"wait", "all", NULL};

dt_enter_check_f dttalpha_arg_check;

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttalpha_cmp(
  dt_ctl_t *ctl,
  char *filename1,
  char *filename2,
  unsigned int size,
  long *cmp)
{
  DT_F("dttalpha_cmp");
  dt_rc_e rc = DT_RC_GOOD;
  dtfd_t fd1;
  dtfd_t fd2;
  void *buf1 = NULL;
  void *buf2 = NULL;
  long got1, got2;
  char *s1, *e1;
  char *s2, *e2;
  int n1, n2;
  int line;
  int i;

  *cmp = 1;
  DT_MEMSET(&fd1, 0, sizeof(fd1));
  DT_MEMSET(&fd2, 0, sizeof(fd2));
  DT_C(dtfd_open,(ctl, &fd1,			/* open first text file			   	*/
    filename1, "rt"));
  DT_C(dtfd_open,(ctl, &fd2,			/* open second text file			*/
    filename2, "rt"));
  DT_C(dtos_malloc2,(ctl,				/* space for first file input	    */
    &buf1, size+1, F, "buf1"));
  DT_C(dtos_malloc2,(ctl,				/* space for second file input	    */
    &buf2, size+1, F, "buf2"));

  DT_C(dtfd_read,(ctl, &fd1, buf1,		/* read first file				    */
    size+1, &got1));
  *cmp = 1;
  if ((unsigned)got1 > size)
  {
    dt_assert(ctl, 0, F,
      "\"%s\" size > %u",
      filename1, size);
    goto cleanup;
  }
  DT_C(dtfd_read,(ctl, &fd2, buf2,		/* read second file				    */
    size+1, &got2));
  if (got2 != got1)
  {
    dt_assert(ctl, 0, F,
      "\"%s\" size > %ld",
      filename2, got1);
    goto cleanup;
  }

  s1 = buf1;
  s2 = buf2;
  s1[(int)got1] = '\0';
  s2[(int)got2] = '\0';
  line = 0;
  do {
    e1 = strchr(s1, '\n');				/* find end of first line			*/
    if (e1)
      n1 = (int)(e1-s1);
    else
      n1 = strlen(s1);

    e2 = strchr(s2, '\n');				/* find end of second line		    */
    if (e2)
      n2 = (int)(e2-s2);
    else
      n2 = strlen(s2);

    if (n1 != n2)						/* lines not same length?		    */
	{
      dt_assert(ctl, 0, F,
        "\"%s\" line %d length %d (expected %d)",
        filename2, line, n1, n2);
      goto cleanup;
	}
    else								/* lines same length			    */
    for (i=0; i<n1; i++)
      if (s1[i] != s2[i])
	  {
        dt_assert(ctl, 0, F,
          "\"%s\" line %d char %d '%c' (expected '%c')",
          filename2, line, i, s1[i], s2[i]);
        goto cleanup;
	  }
    s1 = e1 + 1;
    s2 = e2 + 1;
    line++;
  } while (e1 && *s1);

  *cmp = 0;

cleanup:
  if (buf2)
    DT_I(dtos_free2,(ctl, buf2,
      F, "buf2"));
  if (buf1)
    DT_I(dtos_free2,(ctl, buf1,
      F, "buf1"));
  if (fd2.filename)
    DT_I(dtfd_close,(ctl, &fd2));
  if (fd1.filename)
    DT_I(dtfd_close,(ctl, &fd1));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttalpha_cons_opfunc(
  dt_ctl_t *ctl,
  dtcons_op_e op,
  void *arg1,
  void *arg2)
{
  DT_F("dttalpha_cons_opfunc");
  dtalpha_t *alpha = arg1;
  dt_key_t key = 0;

  switch(op)
  {
    case DTCONS_OP_ENTER:				/* ................................	*/
      DT_Q(dtdialog_hide,(ctl,			/* un-hide the dialog			    */
        &alpha->dialog,
        DTDIALOG_HIDE_FLAG_OFF));
      DT_Q(dtalpha_update,(ctl,			/* send an update to the alpha	    */
        alpha, &key));
      if (arg2)
        *(dt_key_t *)arg2 = key;
    break;
    case DTCONS_OP_LEAVE:				/* ................................	*/
      DT_Q(dtdialog_hide,(ctl,
        &alpha->dialog,
        DTDIALOG_HIDE_FLAG_CLEAR));
    break;
    case DTCONS_OP_STATUS:				/* ................................	*/
      DT_Q(dtalpha_status,(ctl,
        alpha, arg2));
    break;
    case DTCONS_OP_UPDATE:				/* ................................	*/
      DT_Q(dtalpha_update,(ctl,
        alpha, &key));
      if (arg2)
        *(dt_key_t *)arg2 = key;
    break;
    case DTCONS_OP_INPUT:				/* ................................	*/
      DT_Q(dtalpha_input,(ctl,
        alpha, &key));
      if (arg2)
        *(dt_key_t *)arg2 = key;
    break;
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
dttalpha_show(
  dt_ctl_t *ctl,
  char *server,
  const char *keyboard,
  const char *datadir,
  const char *tmpdir,
  char *id,
  int pagesize,
  int npages,
  int *assert_fail)
{
  DT_F("dttalpha_show");
  dtnote_t note;
  dtalpha_t alpha;
  char s1[51];
  dtmemsource_t memsource;
  dtmemalloc_t *memalloc = NULL;
  int have_memsource = 0;
  int have_dialog = 0;
  int have_alpha = 0;
  dtdialog_t dialog;
  dt_key_t key;
  char *input = NULL;
# define INPUT1 "dttalpha.in1"
# define INPUT2 "dttalpha.d00"
  char output[80];
  int i, n;
  dt_rc_e rc;

  DT_C(dtmemram_init,(ctl,				/* make a memsource for our use	    */
    &memsource, "test", 0,
    pagesize, npages));
  have_memsource = 1;

  DT_C((memsource.alloc),(ctl,			/* allocate with the interface	    */
    &memsource,
    (void **)&memalloc,
    0, 0,								/* should use dtmem_alloc()		    */
    pagesize, npages,
    F, "test"));

  DT_MEMSET(&note, 0, sizeof(note));
  note.mem = memalloc;

  DT_C(dtdialog_init,(ctl, &dialog,		/* make our own dialog			    */
    ctl->dialog, server, "Dttalpha",
    DTDIALOG_FLAG_NONE,
    keyboard));
  have_dialog = 1;

  DT_C(dtalpha_begin,(ctl, &alpha,		/* get ready for alpha			    */
    &dialog, &note));
  have_alpha = 1;

  DT_C(dtcons_add,(ctl,	"APP",			/* register alpha with cons manager	*/
    dttalpha_cons_opfunc, &alpha));

  n = pagesize / sizeof(s1);			/* how many strings in a page	    */
  n *= npages;							/* strings in all the pages		    */

  DT_C(dtos_malloc2,(ctl,				/* space for input filename		    */
    (void **)&input,
    strlen(datadir) + 1 +
    DT_MAX(strlen(INPUT1), 
           strlen(INPUT2)) + 1,
    F, "filename"));

  for (i=0; i<n+20; i++)				/* 20 will over fill				*/
  {
    char status[64];
    if (i%10 == 0)						/* every 10th line				    */
	{
      sprintf(input, "%s/%s",			/* name for input file			  	*/
        datadir, INPUT1);
      DT_C(dtalpha_load,(ctl,			/* load from file				    */
        &alpha, input));
	}
    sprintf(s1, "line %03d .........................................", i);
    DT_C(dtnote_append,(ctl,			/* append new note				    */
      &alpha.note, s1, strlen(s1)));
    DT_C(dtnote_add,(ctl,				/* terminate new note			    */
      &alpha.note, "", 1));
    sprintf(status, "Buffering %d%% /\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/",
      100*(i+1)/(n+20));
    if (ctl->cons)						/* we have a cons manager?		    */
      DT_C(dtcons_update,(ctl, status,	/* update for new note addition	    */
        &key))
    else
	{
      DT_C(dtalpha_status,(ctl, &alpha,	/* update for new status		    */
        status));
      DT_C(dtalpha_update,(ctl, &alpha,	/* update for new note addition	    */
        &key));
	}

    if (i == 4 ||						/* near the start				    */
        i == 41 ||						/* in the middle, before filled		*/
        i == 61)						/* after filled						*/
	{
      long cmp;
	  DT_C(dtstr_printf,(ctl,			/* name for output file			    */
        output, sizeof(output),
        "%s/dttalpha.tmp",
        tmpdir));
      DT_C(dtalpha_save,(ctl, &alpha,	/* output the alpha				    */
        output));
      sprintf(input, "%s/%s",			/* name for correct file		  	*/
        datadir, INPUT2);
      sprintf(input+strlen(input)-2,
        "%02d", i);

      DT_C(dttalpha_cmp,(ctl, input,	/* compare input to output		    */
        output, pagesize * npages,
        &cmp));
      DT_C(dtos_unlink,(ctl, output));	/* get rid of the output file	   	*/

      *assert_fail += DT_RC_GOOD !=
        dt_assert(ctl, 
          cmp == 0, F,
          "\"%s\" same as \"%s\" (cmp %ld)",
          output, input, cmp);
	}
  }

  if (ctl->cons)						/* we have a cons manager?		    */
  {
    dt_say(ctl, F, "here is some output");
    DT_C(dtcons_update,(ctl, 			/* update for final status	    	*/
      "Displaying final alpha", &key))
    DT_C(dtcons_input,(ctl, &key))		/* wait for user input			    */
  }
  else
  {
    DT_C(dtalpha_status,(ctl, &alpha,	/* update for final status		    */
      "Displaying final alpha"))
#ifdef NONO
    DT_C(dtalpha_input,(ctl, &alpha,	/* wait for user input			    */
      &key));
#endif
  }

cleanup:
  if (input)
    DT_I(dtos_free2,(ctl, input,
      F, "filename"));

  DT_I(dtcons_del,(ctl,					/* unregister alpha with cons mgr	*/
    dttalpha_cons_opfunc, &alpha));

  if (have_alpha)
  DT_I(dtalpha_finish,(ctl, &alpha));	/* finish showing the note		    */

  if (have_dialog)
  DT_I(dtdialog_uninit,(ctl, &dialog));	/* finish the dialog			    */

  if (memalloc)
  DT_I((memalloc->free),(ctl,			/* deallocate with the interface	*/
    memalloc,
    F, "test"));

  if (have_memsource)
  DT_I(dtmemsource_uninit,(ctl,			/* get rid of the memsource		    */
    &memsource));

  return rc;
}

/*..........................................................................*/

dt_rc_e 
dttalpha_arg_check(						/* check params for this program	*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttalpha_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttalpha)
{
  DT_F("dttalpha");
  dt_ctl_t *ctl = &param->ctl;
  char server[64] = {0};
  char other1[64] = {0};
  char other2[64] = {0};
  dt_enter_arg_t arg[4];
  dt_rc_e rc = DT_RC_GOOD;
  int assert_fail = 0;					/* failed assertion count		    */

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "wait",
    &param->datadir, NULL, NULL, NULL,
    &param->output, "Tmp dir", NULL, "/tmp",
    NULL));

#define SERVER_DESC "dialog server"
#define SERVER_HELP NULL
#define OTHER1_DESC "other1"
#define OTHER1_HELP NULL
#define OTHER2_DESC "other2"
#define OTHER2_HELP NULL

  DT_GI(dt_enter_arg,(ctl, arg,			/* set up positionals			    */
    SERVER_DESC, SERVER_HELP, 
    server, sizeof(server),
    OTHER1_DESC, OTHER1_HELP,
    other1, sizeof(other1),
    OTHER2_DESC, OTHER2_HELP,
    other2, sizeof(other2),
    NULL));
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttalpha_arg_check, NULL));

  if (strlen(server) == 0)				/* no server argument?				*/
  {
	if (param->dialog_flag &			/* there is a dialog param? 		*/
		(DTPARAM_FLAG_VALUE |			/* on command line? 				*/
		 DTPARAM_FLAG_DEFAULT) &&		/* or just defaulted? 				*/
		strcmp(param->dialog, "none"))	/* and it's not "none" 				*/
	  DT_Q(dtstr_printf,(ctl, server,	/* use the dialog param 			*/
        sizeof(server), "%s", 
		param->dialog))
	else								/* dialog unknown thus far?			*/
	{
	  dtprj_param_defaults_t *dflts =	/* for short 						*/
	    dtprj_param_defaults;
	  int i;
	  for (i=0; dflts[i].abbrev; i++)	/* search project defaults table 	*/
	  {
	    if (!strcmp("-dialog",
      	      dflts[i].abbrev))
		{
		  DT_Q(dtstr_printf,(ctl, server,
            sizeof(server), "%s", 
		    dflts[i].dflt));
		  break;
		}
	  }
	}
  }

  while (rc == DT_RC_GOOD)
  {
    if (DT_ABBREV("all", param->op) ||
        DT_ABBREV("wait", param->op))
    {
      DT_GI(dttalpha_show,(ctl, server,
        param->keyboard,
        param->datadir, 
        param->output,
        "1",
        1024, 4,
        &assert_fail));
    }

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttalpha_arg_check, NULL, rc));
  }

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
