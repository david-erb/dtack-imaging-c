/* Example Program dtport1                                                  */
/* Illustrates simple I/O port usage.                                       */
/* Two required numeric arguments are port and value.                       */
/* Most error checking left out for clarity.                                */

#include <dtack/base.h>
#include <dtack/portio.h>

static void barf(
  const char *message)
{
  printf("dtport1: %s\n", message);
  exit(0);
}

int
main(
  int argc,
  char *argv[])
{
  char *e;
  int port, value;
  dtportio_t portio;
  dt_rc_e rc;
  int oldval, newval;

  if (argc != 3) barf("wrong argument count");

  port = (int)strtol(argv[1], &e, 0);
  if (*e) barf("port not a number");
  if (port < 0 || port > 0xfff) barf("port value out of range 0-4095");

  value = (int)strtol(argv[2], &e, 0);
  if (*e) barf("value not a number");
  if (value < 0 || value > 0xff) barf("value out of range 0-255");

  rc = dtportio_init(NULL, &portio, NULL);
  if (rc != DT_RC_GOOD) exit(0);

  rc = dtportio_inb(NULL, &portio, port, &oldval);
  if (rc != DT_RC_GOOD) exit(0);

  rc = dtportio_outb(NULL, &portio, port, value);
  if (rc != DT_RC_GOOD) exit(0);

  rc = dtportio_inb(NULL, &portio, port, &newval);
  if (rc != DT_RC_GOOD) exit(0);

  rc = dtportio_uninit(NULL, &portio);
  if (rc != DT_RC_GOOD) exit(0);

  printf("dtport1: I/O port 0x%03x old=0x%02x value=0x%02x new=0x%02x\n",
    (unsigned int)port, oldval, (int)value, newval);

  return 0;
}
