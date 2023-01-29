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

DT_RCSID("test $RCSfile: dttcipher.c,v $ $Revision: 1.1 $");
#include <dtack/sem.h>
#include <dtack/list.h>
#include <dtack/cipher.h>
#include <dtack/f.h>

#define USAGE "[-Dnnnn]"

static const char *ops[] = {"all",
  "fast",
  "encrypt",
  "decrypt",
  "fdecrypt",
  NULL};

/*..........................................................................*/

static
dt_rc_e
dttcipher_test(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttcipher_test");
  dtcipher_t cipher;
  const char *password1 = "the secret password1";
  const char *password2 = "the second secret password";
  const char *plaintext = "the_plain_text_a_little_longer";
#define L (256)
  char buffer[L];
  char encrypted[L];
  char decrypted[L];
  char printable[L*4];
  dt_rc_e rc;

  cipher.initialized = 0;
  DT_C(dtcipher_init,(ctl, &cipher, password1, password2));

  DT_MEMSET(buffer, 0, L);
  strcpy(buffer, plaintext);

  DT_MEMSET(encrypted, 0, L);
  DT_MEMSET(decrypted, 0, L);
  
  DT_C(dtcipher_encrypt,(ctl, &cipher,
    buffer, L,
    encrypted, L));
  
  DT_C(dtcipher_decrypt,(ctl, &cipher,
    encrypted, L,
    decrypted, L));

  DT_C(dtstr_printable,(ctl,
    buffer, -1,
    printable, sizeof(printable)))
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "buffer:    %s", printable);

  DT_C(dtstr_printable,(ctl,
    encrypted, -1,
    printable, sizeof(printable)))
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "encrypted: %s", printable);

  DT_C(dtstr_printable,(ctl,
    decrypted, -1,
    printable, sizeof(printable)))
  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "decrypted: %s", printable);

  assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      !memcmp(buffer, decrypted, L),
      F, "buffer == decrypted");
	
cleanup:
  if (cipher.initialized)
	DT_I(dtcipher_uninit,(ctl, &cipher));

  return rc;
}


/*..........................................................................*/

static
dt_rc_e
dttcipher_fd(
  dtparam_t *param)
{
  DT_F("dttcipher_fd");
  dt_ctl_t *ctl = &param->ctl;
  dtcipher_t cipher;
  const char *password1 = param->ident;
  const char *password2 = param->name;
  dtfd_t _infd, *in = &_infd;
  dtfd_t _outfd, *out = &_outfd;
  char *b = NULL;
  char *d = NULL;
  long insize;
  long pad;
  long i;
  dtos_time_t beg, end;
  double elapsed;
  dt_rc_e  rc = DT_RC_GOOD;

  cipher.initialized = 0;
  DTFD_CLEAR(ctl, in);
  DTFD_CLEAR(ctl, out);

  DT_C(dtcipher_init,(ctl, &cipher,		/* get the cipher ready 			*/
    password1, password2));


										/* ................................ */
  DT_C(dtfd_open,(ctl, in,				/* open file for reading 			*/
    param->input, "r"));
  DT_C(dtfd_size,(ctl, in, &insize));	/* size of bytes in file 			*/

  pad = insize % cipher.blocksize;
  if (pad)
	pad = cipher.blocksize - pad;

  DT_C(dtos_malloc2,(ctl,				/* space for entire data 			*/
    (void **)&b, insize+pad,
    F, "b buffer"));
  DT_C(dtos_malloc2,(ctl,				/* space for entire data 			*/
    (void **)&d, insize+pad,
    F, "d buffer"));
  DT_C(dtfd_read_all,(ctl, in,			/* suck in the whole file 			*/
    b, insize));
  DT_C(dtfd_close,(ctl, in));			/* close the input file 			*/
										/* ................................ */

  
  DT_C(dtfd_open,(ctl, out,
    param->output, "w"));


  if (!strcmp(param->op, "encrypt"))	/* supposed to encrypt? 			*/
  {
	dtcipher_header_t header;
    DT_C(dtcipher_header,(ctl, &cipher,	/* form cipher file header 			*/
      &header));
	header.pad = pad;					/* add known pad 					*/
	memmove(b+insize, b, pad);			/* fill pad bytes 					*/

	DT_C(dtos_time,(ctl, &beg));
	for (i=0; i<param->n; i++)
    DT_C(dtcipher_encrypt,(ctl,			/* encrypt the data 				*/
      &cipher, 
      b, insize+pad, 
      d, insize+pad));
	DT_C(dtos_time,(ctl, &end));

	DT_C(dtfd_write_all,(ctl, out,		/* write out the header				*/
      &header, sizeof(header)));
	DT_C(dtfd_write_all,(ctl, out,		/* write out the data				*/
      d, insize+pad));
  }
  
  if (!strcmp(param->op, "decrypt"))	/* supposed to decrypt? 			*/
  {
	dtcipher_header_t *header =
	  (dtcipher_header_t *)b;
	int n = insize - sizeof(*header);
	if (strcmp(header->sig,
           DTCIPHER_SIG))
	  DT_CLEANUP((ctl, F,
	    "invalid sig"));

	DT_C(dtos_time,(ctl, &beg));
	for (i=0; i<param->n; i++)
	DT_C(dtcipher_decrypt,(ctl,			/* decrypt the data 				*/
      &cipher, 
      b+sizeof(*header), n,
      d+sizeof(*header), n));
	DT_C(dtos_time,(ctl, &end));

	DT_C(dtfd_write_all,(ctl, out,		/* write out the data				*/
      d+sizeof(*header), 
      n-header->pad));
  }
  
  elapsed = end.wall - beg.wall;
  dt_dbg(ctl, F, DT_DBG_MASK_TIME,
    "%ld %s ops in %0.3f seconds"
    " is %0.3f seconds/op",
    param->n, param->op, elapsed,
    elapsed/param->n);

cleanup:
  if (DTFD_ISOPEN(ctl, out))
	DT_I(dtfd_close,(ctl, out));

  if (d)
	DT_I(dtos_free2,(ctl, d, 
      F, "d buffer"));

  if (b)
	DT_I(dtos_free2,(ctl, b, 
      F, "b buffer"));

  if (cipher.initialized)
	DT_I(dtcipher_uninit,(ctl, 
      &cipher));
  
  if (DTFD_ISOPEN(ctl, in))
	DT_I(dtfd_close,(ctl, in));

  return rc;
}

/*..........................................................................
 * decrypt with the "dtf" routines to test them
 *..........................................................................*/

static
dt_rc_e
dttcipher_fdecrypt(
  dtparam_t *param)
{
  DT_F("dttcipher_fdecrypt");
  dt_ctl_t *ctl = &param->ctl;
  const char *password1 = param->ident;
  const char *password2 = param->name;
  dtf_t f;
  FILE *file = NULL;
  dtfd_t _outfd, *out = &_outfd;
  char b[193];
  dt_rc_e rc = DT_RC_GOOD;

  f.initialized = 0;
  DTFD_CLEAR(ctl, out);

  DT_C(dtf_init,(ctl, &f));
  DT_C(dtf_set,(DTF_IDENT, password1));
  DT_C(dtf_set,(DTF_NAME, password2));

  file = dtf_open(param->input, "r");
  if (!file)
	DT_CLEANUP((ctl, F, "dtf_open(\"%s\") failed", param->input));
  
  DT_C(dtfd_open,(ctl, out,
    param->output, "w"));

  while(1)
  {
	long ngot = dtf_read(b, sizeof(b), 1, file);
	if (ngot < 0)
	  DT_CLEANUP((ctl, F, "dtf_read() failed"));
	if (ngot == 0)
	  break;
	DT_C(dtfd_write_all,(ctl, out,		/* write out the whole buffer		*/
      b, ngot));
  }

cleanup:
  if (DTFD_ISOPEN(ctl, out))
	DT_I(dtfd_close,(ctl, out));

  if (file)
	dtf_close(file);

  if (f.initialized)
	DT_I(dtf_uninit,(ctl, &f));

  return rc;
}


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttcipher_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttcipher_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTMAIN1(dttcipher)
{
  DT_F("dttcipher");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "fast",
    &param->input, NULL, NULL, "input.dat",
    &param->output, NULL, NULL, "output.dat",
    &param->ident, NULL, NULL, "password1",
    &param->name, NULL, NULL, "password2",
    &param->n, NULL, NULL, "1",			/* timing loops 					*/
    &param->reps, NULL, NULL, "1",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_enter_parse,(				/* parse interactively				*/
    argc, argv, F, USAGE, param,
    arg, dttcipher_arg_check, NULL));
                                        /* ................................ */
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {
	  DTT_YIELD_REPS(DT_G, ctl,			/* yield at start of repetition 	*/
        param, reps);
	  
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "fast"))
      {
		DT_GI(dttcipher_test,(ctl, &assert_fail));
      }
	  else
      if (!strcmp(param->op, "encrypt") ||
          !strcmp(param->op, "decrypt"))
      {
		DT_GI(dttcipher_fd,(param));
      }
	  else
      if (!strcmp(param->op, "fdecrypt"))
      {
		DT_GI(dttcipher_fdecrypt,(param));
      }
	  else
		DT_CLEANUP((ctl, F,
          "unknown op \"%s\"", param->op));
	}
  }

                                        /* ................................ */
cleanup:
  DT_I(dt_leave,(param));				/* clean up before leaving			*/

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
