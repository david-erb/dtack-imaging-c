/* Example Program dthist1                                                  */
/* Illustrates direct histogram access.                                     */
/* Program takes no command line arguments.                                 */
/* Program prints out 4 histogram bins like:                                */
/*   dthist1: bin   2 count is   40                                         */
/*   dthist1: bin  11 count is   20                                         */
/*   dthist1: bin 120 count is   30                                         */
/*   dthist1: bin 255 count is   10                                         */
/* Error checking left out for clarity.                                     */

#include <dtack/base.h>
#include <dtack/os.h>

int
main(void)
{
# define N 10
  static unsigned char data[N] = {    /* simple data set                    */
    11, 2, 255, 2, 2,
    120, 2, 11, 120, 120};
  dthist_t hist;
  dthist_freq2_t *freq;
  dthist_bin_t i;

  dthist_alloc(NULL, &hist, 256,      /* allocate direct histogram          */
    0, 256);
  dthist_clear(NULL, &hist);          /* clear histogram bins               */

  freq = (dthist_freq2_t *)hist.freq; /* reference the freq array           */

  for (i=0; i<N; i++)                 /* loop for each data value           */
  {
    dthist_bin_t bin;
    DTHIST_VALUE_TO_BIN(DT_N, NULL,   /* determine which bin                */
      &hist, data[i], &bin);
    freq[bin] += 10;                  /* increment bin by 10                */
  }

  for (i=0; i<hist.width; i++)        /* loop for each bin                  */
  {
    if (freq[i] != 0)
      printf("dthist1:"               /* print bin information              */
        " bin %3u count is %3u\n",
      (unsigned int)i,
      (unsigned int)freq[i]);
  }

  dthist_free(NULL, &hist);           /* free histogram                     */

  return 0;
}
