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
  20000104 DE created
 *..........................................................................*/


/*..........................................................................
| TITLE dtcipher()
|dxinclude(liblink1.dx)
| 
|!SUMMARY
| Encrypt/decrypt buffers.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: cipher.c,v $ $Revision: 1.2 $");
#include <dtack/cipher.h>
#include <dtack/os.h>
#include <dtack/errno.h>


#if DTPRJ_MCRYPT == DT_1

#ifndef DTDEPEND
#include <mcrypt.h>
#endif

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtcipher_init(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  const char *password1,
  const char *password2)
{
  DT_F("dtcipher_init");
#if DTPRJ_MCRYPT == DT_1
  int lpassword1 = strlen(password1);
  int lpassword2 = strlen(password2);
  char *name = NULL;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  if (lpassword1 < 1 ||
	  lpassword1 > DTCIPHER_PASSWORD_MAX-1)
	DT_CLEANUP((ctl, F,
      "password1 length %d not in [1..%d]",
      lpassword1, DTCIPHER_PASSWORD_MAX-1))

  if (lpassword2 < 1 ||
	  lpassword2 > DTCIPHER_PASSWORD_MAX-1)
	DT_CLEANUP((ctl, F,
      "password2 length %d not in [1..%d]",
      lpassword2, DTCIPHER_PASSWORD_MAX-1))

  DT_MEMSET(cipher, 0, sizeof(*cipher));

  strcpy(cipher->password1, password1);
  strcpy(cipher->password2, password2);
  cipher->mode = MCRYPT_CBC;
  cipher->algorithm = MCRYPT_BLOWFISH_448;
  
  name = mcrypt_get_algorithms_name(cipher->algorithm);
  if (!name)
	DT_CLEANUP((ctl, F,
      "invalid algorithm %d", cipher->algorithm));
  DT_C(dtos_strdup,(ctl, name, &cipher->name));

  cipher->blocksize = mcrypt_get_block_size(cipher->algorithm);

  if (cipher->blocksize < 1 ||
	  cipher->blocksize > DTCIPHER_BLOCKSIZE_MAX)
	DT_CLEANUP((ctl, F,
        "%s blocksize %d not in [1..%d]",
        cipher->name, cipher->blocksize, 
        DTCIPHER_BLOCKSIZE_MAX))

  cipher->initialized = 1;
  
  cipher->keysize = mcrypt_get_key_size(cipher->algorithm);

  if (cipher->keysize < 1 ||
	  cipher->keysize > DTCIPHER_KEYSIZE_MAX)
	DT_CLEANUP((ctl, F,
      "%s keysize %d not in [1..%d]",
      cipher->name, cipher->keysize, 
      DTCIPHER_KEYSIZE_MAX));

  gen_key_sha1(cipher->key, NULL, 0, cipher->keysize, cipher->password1, lpassword1);

  gen_key_sha1(cipher->iv, NULL, 0, cipher->keysize, cipher->password2, lpassword2);

cleanup:
  if (rc != DT_RC_GOOD)
	if (cipher->name)
	  DT_I(dtos_free2,(ctl, cipher->name, F, cipher->name));
  if (name)
    free(name);
  return rc;

#else
  return dt_err_notlinked(ctl, F, "mcrypt library");
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtcipher_uninit(
  dt_ctl_t *ctl,
  dtcipher_t *cipher)
{
  DT_F("dtcipher_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (cipher->initialized)
  {
	if (cipher->name)
	  DT_I(dtos_free2,(ctl, cipher->name, F, cipher->name));
  }
  DT_MEMSET(cipher, 0, sizeof(*cipher));
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtcipher_header(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  dtcipher_header_t *header)
{
  DT_F("dtcipher_header");
  dt_rc_e rc = DT_RC_GOOD;

  
  DT_MEMSET(header, 0, sizeof(*header));
  strcpy(header->sig, DTCIPHER_SIG);

  header->algorithm = cipher->algorithm;
  header->mode = cipher->mode;
  
cleanup:
  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtcipher_encrypt(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  char *in,
  int insize,
  char *out,
  int outsize)
{
  DT_F("dtcipher_encrypt");
#if DTPRJ_MCRYPT == DT_1
  int td;
  int mrc;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  if (!cipher->initialized)
	DT_CLEANUP((ctl, F,
      "cipher structure not initialized"));

  if (insize % cipher->blocksize)
	  DT_CLEANUP((ctl, F,
        "input size %d not a multiple of %s blocksize %d",
        insize, cipher->name, cipher->blocksize));
  if (outsize % cipher->blocksize)
	  DT_CLEANUP((ctl, F,
        "output size %d not a multiple of %s blocksize %d",
        outsize, cipher->name, cipher->blocksize));

  td = mcrypt_generic_init(
    cipher->mode, cipher->algorithm, 
    cipher->key, strlen(cipher->key), 
    cipher->iv);
  if (td < 0)
	DT_CLEANUP((ctl, F,
      "mcrypt_generic_init failed"));

  for (i=0; i<insize; i+=cipher->blocksize)
  {
	memmove(out+i, in+i, cipher->blocksize);
    mrc = mcrypt_generic(td, out+i, cipher->blocksize);
	if (mrc < 0)
	  DT_CLEANUP((ctl, F,
        "mcrypt_generic_encrypt failed at byte %d", i));
  }

  mrc = mcrypt_generic_end(td);
  if (mrc < 0)
	DT_CLEANUP((ctl, F,
      "mcrypt_generic_end failed"));

cleanup:
  return rc;

#else
  return dt_err_notlinked(ctl, F, "mcrypt library");
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtcipher_decrypt(
  dt_ctl_t *ctl,
  dtcipher_t *cipher,
  char *in,
  int insize,
  char *out,
  int outsize)
{
  DT_F("dtcipher_decrypt");
#if DTPRJ_MCRYPT == DT_1
  int td;
  int mrc;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  if (!cipher->initialized)
	DT_CLEANUP((ctl, F,
      "cipher structure not initialized"));

  if (insize % cipher->blocksize)
	  DT_CLEANUP((ctl, F,
        "input size %d not a multiple of %s blocksize %d",
        insize, cipher->name, cipher->blocksize));
  if (outsize % cipher->blocksize)
	  DT_CLEANUP((ctl, F,
        "output size %d not a multiple of %s blocksize %d",
        outsize, cipher->name, cipher->blocksize));

  td = mcrypt_generic_init(
    cipher->mode, cipher->algorithm, 
    cipher->key, strlen(cipher->key), 
    cipher->iv);
  if (td < 0)
	DT_CLEANUP((ctl, F,
      "mcrypt_generic_init failed"));

  for (i=0; i<insize; i+=cipher->blocksize)
  {
	memmove(out+i, in+i, cipher->blocksize);
    mrc = mdecrypt_generic(td, out+i, cipher->blocksize);
	if (mrc < 0)
	  DT_CLEANUP((ctl, F,
        "mcrypt_generic_decrypt failed at byte %d", i));
  }

  mrc = mcrypt_generic_end(td);
  if (mrc < 0)
	DT_CLEANUP((ctl, F,
      "mcrypt_generic_end failed"));

cleanup:
  return rc;

#else
  return dt_err_notlinked(ctl, F, "mcrypt library");
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcipher_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtcipher lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 0;
  if (rev_date != NULL)
    *rev_date = 20000203;
  return DT_RC_GOOD;
}

