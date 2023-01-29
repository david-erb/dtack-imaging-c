/* Example Program dtenv4                                                   */
/* Illustrates error handling                                               */
/* Program takes no parameters or arguments.                                */
/* Outputs a line like:                                                     */
/*   dtenv4: this message output through error mechanism, i=99              */

#include <dtack/base.h>

dt_rc_e
myerr(                                /* custom error handler               */
  dt_ctl_t *ctl,
  const char *caller,
  const char *format,
  va_list arg)
{
  printf("%s: ", caller);             /* print caller ID                    */
  vprintf(format, arg);               /* just print message                 */
  printf("\n");                       /* there is no newline in message     */
  return DT_RC_BAD;                   /* by convention                      */
}
                                      /* ..............................     */
int main()
{
  dt_ctl_t ctl;
  memset(&ctl, 0, sizeof(ctl));       /* clear ctl structure                */

  ctl.err = myerr;                    /* install custom function            */

  dt_err(&ctl, "dtenv4",              /* generate an original error         */
    "this message output through"
    " error mechanism, i=%d", 99);

  return 0;
}
