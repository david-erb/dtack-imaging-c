/* Example Program dthist2                                                  */
/* Illustrates histogram binning.                                           */
/* Program takes no command line arguments.                                 */
/* Program prints out 10 histogram bins and a cheap bar chart like:         */
/*   dthist2: bin 0 range [-1.0:-0.8] scaled count   6 ======               */
/*   dthist2: bin 1 range [-0.8:-0.6] scaled count   5 =====                */
/* and so on.                                                               */
/* Error checking left out for clarity.                                     */

#include <dtack/base.h>
#include <dtack/os.h>

int
main(void)
{
# define N 1000                       /* how many data values to generate   */
  double *data;
  dt1d_ran_t ran;
  dthist_t hist;
  dthist_freq2_t *freq;
  char bar[20];
  dthist_bin_t i;

  dtos_malloc(NULL,                   /* get space for data                 */
    (void **)&data,
    N * sizeof(*data));

  dt1d_ran_gauss_init(&ran, 0);       /* init random numbers                */
  dt1d_random_gauss_double(&ran,      /* compute random numbers             */
    data, N);
  dt1d_mult(                          /* tighten the curve                  */
    DTNTYPE_F2,
    data, N, 0.5,
    data, N, 0.0, 0.0,
    data, N);

  dthist_alloc(NULL, &hist, 10,       /* allocate 10-bin histogram          */
    -1.0, 1.0);                       /* histogram data range               */
  dthist_clear(NULL, &hist);          /* clear histogram bins               */
  dthist_add_values(NULL, &hist,      /* add data to histogram              */
    data, 0, N, DT_NTYPE_F2);

  dthist_scale(NULL, &hist,           /* scale freq counts to bar size      */
    sizeof(bar)-1);

  freq = (dthist_freq2_t *)hist.freq; /* reference the freq array           */

  for (i=0; i<hist.width; i++)        /* loop for each bin                  */
  {
    double floor, ceiling;
    dthist_bin_to_value(NULL,         /* get bin edges                      */
      &hist, i, &floor, &ceiling);

    memset(bar, '=', sizeof(bar));    /* fill the bar                       */
    bar[(int)freq[i]] = '\0';         /* chop bar off at its height         */

    printf("dthist2:"                 /* print bin information              */
      " bin %u range [%4.1f:%4.1f]"
      " scaled count %3u %s\n",
      (unsigned int)i, floor, ceiling,
      (unsigned int)freq[i], bar);
  }

  dthist_free(NULL, &hist);           /* free histogram                     */

  dtos_free(NULL, data);              /* free data                          */

  return 0;
}
