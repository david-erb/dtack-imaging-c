/* Example Program dtenv3                                                   */
/* Illustrates yielding                                                     */
/* Program takes one optional argument: the yield string                    */
/* argument           output                                                */
/* ""                 no output for five seconds                            */
/* "message"          11 messages like "dtenv3: 10% done"                   */
/* "twirl"            twirling baton for five seconds                       */
/* "dot"              11 dots                                               */

#include <dtack/base.h>
#include <dtack/os.h>

int
main(
  int argc,
  char *argv[])
{
  dt_ctl_t ctl;
  char message[80] = {0};
  int i;

  memset(&ctl, 0, sizeof(ctl));       /* clear ctl structure                */
  ctl.progname = "dtenv3";            /* set program name for messages      */

  if (argc > 1)                       /* command line has something?        */
    dt_yield_parse(&ctl, argv[1],     /* presume it's the yield hint        */
      &ctl.yield_flags,
      message, sizeof(message));

  if (strlen(message))                /* yield hint parsing failed?         */
    dt_err(&ctl, "dtenv3", "%s",
      message);
  else                                /* yield hint ok?                     */
    for (i=0; i<=100; i+=10)
    {
      dt_yield(&ctl, "%d%% done", i); /* yield timeslice                    */
      if (i < 100)                    /* not at last loop yet?              */
        dtos_delay(&ctl, 500);        /* delay a bit                        */
    }

  return 0;
}
