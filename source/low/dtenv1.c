/* Example Program dtenv1                                                   */
/* Illustrates debug output                                                 */
/* Program takes no command line arguments.                                 */
/* Prints out two simple messages like:                                     */
/*   dtenv1: main: int 3, long 0x12345678, howdy                            */
/*   dtenv1: main: at dtenv1.c@23 int 99, dbl 1.234, howdy again            */
/*   dtenv1: main: this message does appear                                 */

#include <dtack/base.h>

int main(void)
{
  dt_ctl_t ctl;
  memset(&ctl, 0, sizeof(ctl));       /* clear ctl structure                */
  ctl.progname = "dtenv1";            /* set program name for messages      */

  dt_dbg(&ctl, "main",                /* output debug message from main     */
    DT_DBG_MASK_ALWAYS,               /* don't ever filter this one out     */
    "int %d, long 0x%08lx, %s",       /* the format string                  */
    3, (void *)0x12345678L, "howdy");

                                      /* ..............................     */
  ctl.dbg_mask = DT_DBG_MASK_TRACE;   /* enable TRACE class                 */

  dt_dbg_trace(&ctl, "main",          /* output trace debug message         */
    __FILE__, __LINE__,               /* from where in main                 */
    "int %d, dbl %g, %s",             /* the format string                  */
    99, 1.234, "howdy again");

                                      /* ..............................     */
  dt_dbg(&ctl, "main",                /* output filtered message            */
    DT_DBG_MASK_PAGE,                 /* this class is not enabled          */
    "this message does not appear");

  dt_dbg_parse(&ctl, "page",          /* parse the "page" string            */
    &ctl.dbg_mask, NULL, 0);          /* put straight into ctl              */

  dt_dbg(&ctl, "main",                /* try it now                         */
    DT_DBG_MASK_PAGE,                 /* this class is now enabled          */
    "this message does appear");

  return 0;
}
