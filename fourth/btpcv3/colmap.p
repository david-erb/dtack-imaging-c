extern "C" {

extern
void
DTCONFIG_API1
rgb_ycc(
  unsigned char ***components,
  int ymax,
  int xmax, 
  int smooth);

extern
void
DTCONFIG_API1
ycc_rgb(
  unsigned char ***components, 
  const int& rows, 
  const int& cols,
  const unsigned char *clip, 
  const char& expand);
}