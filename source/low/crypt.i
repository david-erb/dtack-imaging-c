
static int _crypt_sbox[256];
static int _crypt_key[256];
static int _crypt_i;
static int _crypt_j;
static int _crypt_first = 0;
static char _crypt_password[4096];
static char _crypt_reminder[4096];
static char *_crypt_revision = "encrypt standard 00.00 category 01.00 revision 07.43";
static
void
_rc4_initialize(const char *password)
{
  int l;
  int a, b;

  l = strlen(password);
  
  for (a=0; a<256; a++)
  {
	_crypt_key[a] = password[a % l];
	_crypt_sbox[a] = a;
  }

  b = 0;
  for (a=0; a<256; a++)
  {
	int tempSwap;

	b = (b + _crypt_sbox[a] + _crypt_key[a]) % 256;
	tempSwap = _crypt_sbox[a];
	_crypt_sbox[a] = _crypt_sbox[b];
	_crypt_sbox[b] = tempSwap;
  }

  _crypt_i = 0;
  _crypt_j = 0;
}
   
// ----------------------------------------------------------------------------
//
static
void
_rc4_crypt(const char *plaintxt, int length, char *cipher)
{
  int a;

  for (a=0; a<length; a++)
  {
	int temp;
	int k;

    _crypt_i = (_crypt_i + 1) % 256;
    _crypt_j = (_crypt_j + _crypt_sbox[_crypt_i]) % 256;
    temp = _crypt_sbox[_crypt_i];
    _crypt_sbox[_crypt_i] = _crypt_sbox[_crypt_j];
    _crypt_sbox[_crypt_j] = temp;
   
    k = _crypt_sbox[(_crypt_sbox[_crypt_i] + _crypt_sbox[_crypt_j]) % 256];

    *cipher++ = plaintxt[a] ^ k;
  }
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dt_dbg_cryptout(		
  int fd,
  const char *string,
  int length)
{
  if (_crypt_password[0])
  {
	char *cipher;
										/* supposed to prefix file header? */
	if (_crypt_first)
	{
	  int l;
	  l = strlen(_crypt_revision);
	  WRITE(fd, &l, 4);
	  WRITE(fd, _crypt_revision, l);
	  l = strlen(_crypt_reminder);
	  WRITE(fd, &l, 4);
	  WRITE(fd, _crypt_reminder, l);
										/* don't prefix file header at next write */
	  _crypt_first = 0;
	}
	
	cipher = (char *)malloc(length);
	if (cipher)
	{
	  _rc4_crypt(string, length, cipher);

	  WRITE(fd, &length, 4);
	  WRITE(fd, cipher, length);

	  free(cipher);
	}
  }
  else								/* not encrypting at all? */
  {
	WRITE(fd, string, length);
  }
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dt_dbg_cryptset(						/* set encryption password         */
  dt_ctl_t *ctl,
  const char *password,
  const char *reminder)
{
  DT_F("dt_dbg_cryptset");
  strcpy(_crypt_password, password);
  strcpy(_crypt_reminder, reminder);
  _rc4_initialize(_crypt_password);
										/* note to prefix file header at next write */
  _crypt_first = 1;
}
