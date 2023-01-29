/* Example Program dtenv2                                                   */
/* Illustrates a common program template                                    */
/* Program takes one optional parameter and one optional argument.          */
/* command line        result                                               */
/* ""                  dtenv2: color "red", arg ""                          */
/* "-color blue"       dtenv2: color "blue", arg ""                         */
/* "-color blue yyy"   dtenv2: color "blue", arg "yyy"                      */
/* "-color blue yyy"   dtenv2: color "blue", arg "yyy"                      */
/* "xxx"               dtenv2: color "red", arg "xxx"                       */
/* Error checking left out for clarity.                                     */

#include <dtack/base.h>

int main(
  int argc,
  char *argv[])
{
  dtparam_t param;                    /* space for param structure          */
  dt_ctl_t *ctl = &param.ctl;         /* for short                          */
  dt_enter_arg_t arg[1+1];            /* space for one arg plus sentinel    */
  char myarg[80];                     /* space for positional argument      */

  memset(&param, 0, sizeof(param));   /* clear param structure              */
  dt_heapmon_init(ctl);               /* commence heap monitoring           */

  dtparam_init(&param);               /* initialize param structure         */

  dtparam_set(&param, 0,              /* set default for params we want     */
    &param.color, NULL, NULL, "red",  /* 4-tuple for "color"                */
    NULL);                            /* sentinel                           */

  dt_enter_arg(ctl, arg,              /* set up positional argument         */
    "myarg description", NULL,
    myarg, sizeof(myarg),
    NULL);

  dt_enter_parse(argc, argv,          /* parse command line                 */
    "dtenv3", NULL, &param, arg,
    NULL, NULL);                      /* no custom checking                 */

  printf("color \"%s\","              /* print what we got                  */
    " arg \"%s\"\n",
    param.color, arg[0].val);

  dt_leave(&param);                   /* free resources                     */

  dt_heapmon_uninit(ctl, NULL);       /* quit heap monitoring               */

  return 0;
}
