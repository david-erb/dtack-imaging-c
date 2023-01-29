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
  20000223 DE different declaration for dtf_open
  20000218 DE wrong argument in dtfd_read call
  20000202 DE created
 *..........................................................................*/


/*..........................................................................
| TITLE dtf()
|dxinclude(liblink1.dx)
| 
|!SUMMARY
| Functions that act like fopen(), fread(), fclose(), but auto-decrypt.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: f.c,v $ $Revision: 1.2 $");
#include <dtack/sem.h>
#include <dtack/list.h>
#include <dtack/f.h>
#include <dtack/os.h>
#include <dtack/cipher.h>

static dtf_t *global_f = NULL;

/* ------------------------------------------------------------------------	*/
static
void
___log_va(
  const char *f,
  const char *format,
  va_list ap)
{
  FILE *file = stderr; 					/* fopen("/tmp/tmp.out", "a");		*/
  if (file)
  {
    char s[64];
    time_t clock = time(NULL);			/* current time 					*/
    struct tm *t = localtime(&clock);	/* format into fields 				*/
    strftime(s, sizeof(s),				/* express as string 				*/
      "%Y%m%d.%H%M.%S", t);

    fprintf(file, "%s [%5d] ", s, getpid());
    fprintf(file, "%s: ", f);
    vfprintf(file, format, ap);
    fprintf(file, "\n");
    if (file != stderr &&
		file != stdout)
	  fclose(file);
  }
}

/* ------------------------------------------------------------------------	*/
static
void
___log(
  const char *f,
  const char *format,
  ...)
{
  va_list ap;
  va_start(ap, format);
  /*  ___log_va(f, format, ap); */
  va_end(ap);
}
/* ------------------------------------------------------------------------	*/

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtf_init(								/* initialize structure				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtf_t *f)
{
  DT_F("dtf_init");
  const char *env = getenv("DT_DBG");
  long mask;
  dt_rc_e rc;

  if (env && *env)
  {
    DT_C(dt_dbg_parse,(ctl, env, &mask, NULL, 0));
    ctl->dbg_mask = mask;
  }

___log(F, "called ctl=0x%08lx env=%s mask=0x%08lx", 
    ctl, env? env: "NULL", mask);

  memset(f, 0, sizeof(f));

  DT_C(dtlist_create,(ctl,
    &f->param_list, "f param list"));

  DT_C(dtlist_create,(ctl,
    &f->file_list, "f file list"));

  DT_C(dtsem_create,(ctl,
    &f->lock, 0));

  DT_C(dtsem_post,(ctl, &f->lock, 1));	/* let them have at it 				*/

  f->ctl = ctl;							/* use given ctl 					*/
  
  f->initialized = 1;					/* mark us initialized 				*/

  global_f = f;							/* remember f structure in global 	*/

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (f->lock.initialized)
	  DT_I(dtsem_destroy,(ctl,
        &f->lock));

	if (f->param_list.m)
	  DT_I(dtlist_destroy,(ctl,
        &f->param_list));

	if (f->file_list.m)
	  DT_I(dtlist_destroy,(ctl,
        &f->file_list));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtf_uninit(								/* uninit structure					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtf_t *f)
{
  DT_F("dtf_uninit");
  dt_rc_e rc = DT_RC_GOOD;

___log(F, "called f->initialized=%d ctl=0x%08lx", f->initialized, ctl);

  if (!f->initialized)					/* f structure not initialized? 	*/
	return rc;

  if (f->lock.initialized)
  {
	DT_I(dtsem_destroy,(ctl,
      &f->lock));
  }

  if (f->file_list.m)
  {
	DT_I(dtlist_free,(ctl,
      &f->file_list));
	DT_I(dtlist_destroy,(ctl,
      &f->file_list));
  }

  if (f->param_list.m)
  {
	DT_I(dtlist_free,(ctl,
      &f->param_list));
	DT_I(dtlist_destroy,(ctl,
      &f->param_list));
  }

  memset(f, 0, sizeof(f));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtf_set(								/* set param to value				*/
  const char *keyword,
  const char *value)
{
  DT_F("dtf_uninit");
  dtf_t *f = global_f;
  dt_ctl_t *ctl = f->ctl;
  dtsem_t *lock = &f->lock;
  dtlist_t *list = &f->param_list;
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  if (!f->initialized)					/* f structure not initialized? 	*/
	return dt_err(ctl, F,
      "f 0x%08lx not initialized", f);
  
  DT_C(dtsem_wait,(ctl, lock, 1));		/* snag the lock 					*/

  DT_C(dtlist_search,(ctl, list,
    keyword, &elem));
  if (elem)								/* already in the list? 			*/
  {
	DT_C(dtos_free2,(ctl,
      elem->user, F, elem->user));
	DT_C(dtos_strdup,(ctl, value,
      (char **)&elem->user));
  }
  else									/* not in the list yet? 			*/
  {
	char *name, *user;
	DT_C(dtos_strdup,(ctl, keyword,
      &name));
	DT_C(dtos_strdup,(ctl, value,
      &user));
	DT_C(dtlist_after,(ctl, list,
      DTLIST_ELEM_TAIL, name, user, 
      NULL));
  }
  
cleanup:
  DT_I(dtsem_post,(ctl, lock, 1));		/* release the lock 				*/

  return rc;
}

/*..........................................................................
 * return NULL and no error if not found
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtf_get(								/* get param's value				*/
  const char *keyword,
  const char **value)
{
  DT_F("dtf_get");
  dtf_t *f = global_f;
  dt_ctl_t *ctl = f->ctl;
  dtsem_t *lock = &f->lock;
  dtlist_t *list = &f->param_list;
  dtlist_elem_t *elem;
  dt_rc_e rc = DT_RC_GOOD;

  if (!f->initialized)					/* f structure not initialized? 	*/
	return dt_err(ctl, F,
      "f 0x%08lx not initialized", f);
  
  DT_C(dtsem_wait,(ctl, lock, 1));		/* snag the lock 					*/

  DT_C(dtlist_search,(ctl, list,
    keyword, &elem));
  if (elem)								/* found in the list? 				*/
	*value = elem->user;
  else									/* not found in the list? 			*/
	*value = NULL;
  
cleanup:
  DT_I(dtsem_post,(ctl, lock, 1));		/* release the lock 				*/
  return rc;
}


/*..........................................................................
 *..........................................................................*/

FILE
DTCONFIG_API1
*dtf_open(								/* fake fopen						*/
  const char *filename,
  const char *mode)
{
  DT_F("dtf_open");
  dtf_t *f = global_f;
  dt_ctl_t *ctl = f->ctl;
  const char *password1;
  const char *password2;
  const char *use_fopen;
  int readable;
  dtfd_t *fd = NULL;
  dtfd_buffer_t b;						/* buffer for decrypted data 		*/
  dtcipher_t cipher;					/* encryption cipher 				*/
  dtcipher_header_t header;
  int h = sizeof(header);
  dt_rc_e rc;

  b.b = NULL;
  cipher.initialized = 0;

___log(F, "called f->initialized=%d ctl=0x%08lx filename=%s mode=%s", f->initialized, ctl, filename, mode);

  use_fopen = "f not initialized";
  if (!f->initialized ||				/* f structure not initialized yet? */
	  strcmp(mode, "r"))				/* not reading? 					*/
	goto cleanup;

  use_fopen = "file not readable";
  DT_C(dtos_isreadable,(ctl,
    filename, &readable));
  if (!readable)						/* file not readable? 				*/
	goto cleanup;

  DT_C(dtf_get,(DTF_IDENT,
    &password1));
  DT_C(dtf_get,(DTF_NAME,
    &password2));

  use_fopen = "decrypt keys not set";
  if (!password1 || !password2)			/* not expecting encrypted file?	*/
	goto cleanup;

  DT_C(dtos_malloc2,(ctl,				/* space for the fd structure 		*/
    (void **)&fd, sizeof(*fd),
    F, "fd"));
  DTFD_CLEAR(ctl, fd);

  DT_C(dtfd_open,(ctl, fd, 
    filename, mode));

  use_fopen = "file too short";
  DT_C(dtfd_size,(ctl, fd, &b.m));		/* file too short to have a header? */
  if (b.m < h)
    goto cleanup;

  DT_C(dtfd_read_all,(ctl, fd,			/* suck in the whole header			*/
    &header, h));

  use_fopen = "file has no cipher sig";
  if (strcmp(header.sig, DTCIPHER_SIG))	/* not a cipher header? 			*/
	goto cleanup;

  use_fopen = NULL;						/* we will take care of it 			*/
  
  b.m -= h;								/* skip the header size				*/
  DT_C(dtos_malloc2,(ctl,				/* space for entire rest of  data	*/
    (void **)&b.b, b.m,
    F, "buffer"));
  DT_C(dtfd_read_all,(ctl, fd,			/* suck in the whole file 			*/
    b.b, b.m));

  DT_C(dtcipher_init,(ctl, &cipher,		/* get the cipher ready 			*/
    password1, password2));
  DT_C(dtcipher_decrypt,(ctl,			/* decrypt the data 				*/
    &cipher, 
    b.b, b.m, 
    b.b, b.m));

  if (fd->b.b)							/* file has a buffer? 				*/
	DT_C(dtos_free2,(ctl,				/* release the buffer 				*/
      fd->b.b, F, "old buffer"));
	
  b.n = b.m - header.pad;				/* subtract off the pad 			*/
  b.p = b.b;							/* at start of buffer 				*/
  fd->pos = 0;
  fd->b = b;							/* replace fd's buffer 				*/

cleanup:
	
  if (rc != DT_RC_GOOD && b.b)
	DT_I(dtos_free2,(ctl, b.b, 
      F, "buffer"));
	
  if (cipher.initialized)				/* we decrypted? 					*/
	DT_I(dtcipher_uninit,(ctl, &cipher));

  if (use_fopen)
  {
___log(F, "%s fd=0x%08lx", use_fopen, fd);
	if (fd && DTFD_ISOPEN(ctl, fd))
	  DT_I(dtfd_close,(ctl, fd));
	if (fd)
	  DT_I(dtos_free2,(ctl, fd, F, "fd"));
	fd = (dtfd_t *)fopen(filename, mode);
  }
  else
  {
    if (rc != DT_RC_GOOD &&
	    DTFD_ISOPEN(ctl, fd))
	  DT_I(dtfd_close,(ctl, fd));

	if (rc != DT_RC_GOOD && fd)
    {
	  DT_I(dtos_free2,(ctl, fd, F, "fd"));
	  fd = NULL;
    }
  }
  return (FILE *)fd;
}


/*..........................................................................
 *..........................................................................*/

int
DTCONFIG_API1
dtf_read(								/* fake fread						*/
  void *buf,
  unsigned int size,
  unsigned int n,
  FILE *file)
{
  DT_F("dtf_fread");
  dtf_t *f = global_f;
  dt_ctl_t *ctl = f->ctl;
  dtfd_t *fd = (dtfd_t *)file;
  long ngot;
  dt_rc_e rc = DT_RC_GOOD;

___log(F, "called f->initialized=%d ctl=0x%08lx fd->sig=0x%08lx", f->initialized, ctl, fd->sig);

  if (!f->initialized ||				/* f structure not initialized? 	*/
	  fd->sig != DTFD_SIG)				/* not one of our fd structures? 	*/
	return fread(buf, size, n, file);

  DT_C(dtfd_read,(ctl, fd,
    buf, size*n, &ngot));
cleanup:
  if (rc != DT_RC_GOOD)
	ngot = -1;
  return ngot;
}


/*..........................................................................
 *..........................................................................*/

int
DTCONFIG_API1
dtf_close(								/* fake fclose						*/
  FILE *file)
{
  DT_F("dtf_close");
  dtf_t *f = global_f;
  dt_ctl_t *ctl = f->ctl;
  dtfd_t *fd = (dtfd_t *)file;
  dt_rc_e rc = DT_RC_GOOD;

___log(F, "called f->initialized=%d ctl=0x%08lx fd->sig=0x%08lx", f->initialized, ctl, fd->sig);

  if (!f->initialized ||				/* f structure not initialized? 	*/
	  fd->sig != DTFD_SIG)				/* not one of our fd structures? 	*/
	return fclose(file);

  if (fd)
  {
	if (DTFD_ISOPEN(ctl, fd))
	  DT_I(dtfd_close,(ctl, fd));
	DT_I(dtos_free2,(ctl, fd, F, "fd"));
  }
  
cleanup:
  return -(rc != DT_RC_GOOD);
}

/*..........................................................................
 *..........................................................................*/

int
DTCONFIG_API1
dtf_error(								/* fake ferror						*/
  FILE *file)
{
  DT_F("dtf_error");
  dtf_t *f = global_f;
  dt_ctl_t *ctl = f->ctl;
  dtfd_t *fd = (dtfd_t *)file;
  dt_rc_e rc = DT_RC_GOOD;

  if (!f->initialized ||				/* f structure not initialized? 	*/
	  fd->sig != DTFD_SIG)				/* not one of our fd structures? 	*/
	return ferror(file);

  return 0;								/* we don't track error status 		*/
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtf_rev(								/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtf lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 1;
  if (rev_date != NULL)
    *rev_date = 20000203;
  return DT_RC_GOOD;
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
