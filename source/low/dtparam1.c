/* Example Program dtparam1                                                 */
/* Illustrates keyword parsing                                              */
/* command line        result                                               */
/* ""                  dtparam1: color defaults to "red"                    */
/* "-color blue"       dtparam1: color parsed as "blue"                     */
/* "-x 0"              dtparam1: Sorry, "-x" is not understood (try -help). */
/* Most error checking left out for clarity.                                */

#include <dtack/base.h>

int main(
  int argc,
  char *argv[])
{
  dtparam_t param;                    /* space for param structure          */
  dt_ctl_t *ctl = &param.ctl;         /* for short                          */
  int nused;
  char message[80];

  memset(&param, 0, sizeof(param));   /* clear param structure              */
  ctl->progname = "dtparam1";         /* set program name for messages      */
  dtparam_init(&param);               /* initialize param structure         */

  dtparam_set(&param, 0,              /* set default for params we want     */
    &param.color, NULL, NULL, "red",  /* 4-tuple for "color"                */
    NULL);                            /* sentinel                           */

  dtparam_parse(&param,               /* parse command line                 */
    argc-1, argv+1,
    &nused,
    message, sizeof(message));

  if (strlen(message) > 0)            /* error parsing?                     */
    printf("dtparam1: %s\n",          /* print the error message            */
      message);
  else                                /* no error parsing?                  */
    printf("dtparam1:"
      " color %s \"%s\"\n",           /* print what we got                  */
      param.color_flag &              /* check the color flag               */
      DTPARAM_FLAG_VALUE?
        "parsed as": "defaults to",
      param.color);
  dtparam_free(&param);               /* free resources                     */

  return 0;
}
