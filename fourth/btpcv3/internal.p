extern "C" {

/* used by encode.c */

extern
int 
DTCONFIG_API1
makescratch(
  const int& xmax, 
  const int& ymax);

extern
int 
DTCONFIG_API1
deletescratch(
  const int& ymax);

extern
void 
DTCONFIG_API1
btpc_enc_plane(
  unsigned char **in, 
  const int& xmax, 
  const int& ymax,
  const int& fromlevel, 
  const int& tolevel, 
  const int *spacing, 
  class Compactr *codeout, 
  const int& clipval);


/* used by decode.c */

extern
unsigned char *
DTCONFIG_API1
initclip(
  const int& maxval);

extern
void
DTCONFIG_API1
btpc_dec_plane(
  unsigned char **out,
  const int& XMAX, 
  const int& YMAX,
  const int& fromlevel, 
  const int& tolevel, 
  const int *spacing,
  class Compactr *codein, 
  const int& clipval);

}
