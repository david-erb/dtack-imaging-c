
dt_rc_e
dt_sez90(								/* calculate pds and mark peaks		*/
  dtparam_t *param,
  const double * const input,			/* input array to find peaks in	    */
  const int n,							/* length of input and output	    */
  char * const mark);					/* output array to put marks in	   	*/

dt_rc_e
dt_sez90_hist(							/* calculate pds and mark peaks		*/
  dtparam_t *param,
  dthist_t *hist,						/* histogram to find peaks in	    */
  char * const mark);					/* output array to put marks in	   	*/

dt_rc_e
dt_sez90_ends(							/* mark peaks at ends of given pds	*/
  dtparam_t *param,					  
  const double * const input,			/* input histogram				    */
  const int n,					/* length of histogram and mark	    */
  char * const mark);					/* array where peaks are marked	    */

dt_rc_e
dt_sez90_mark(							/* mark peaks given pds				*/
  dtparam_t *param,
  const double * const input,
  const int n,
  const double * const pds,
  char * const mark);

void
dt_sez90_mark_dbg(						/* debug mark array				    */
  dtparam_t *param,
  const double * const input,
  const int n,
  const char * const mark,				/* peaks previously marked		    */
  char *caller,
  long mask);

dt_rc_e
dt_sez90_pds(
  dtparam_t *param,
  const double *input,
  int n,
  double *pds);

dt_rc_e
dt_sez90_peak_next(
  dtparam_t *param,
  const char * const mark,
  const int n,
  int i,
  int * const start,
  int * const maximum,
  int * const end);

dt_rc_e
dt_sez90_peak_prev(
  dtparam_t *param,
  const char * const mark,
  const int n,
  int i,
  int * const start,
  int * const maximum,
  int * const end);

dt_rc_e
dt_sez90_plot_boxes(					/* plot boxes around peaks			*/
  dtparam_t *param,
  const char * const mark,				/* peaks previously marked		    */
  const int n,
  dtimage_t *plot,						/* image to plot in				    */
  double color);

dt_rc_e
dt_sez90_plot_maxes(					/* plot peak maximums				*/
  dtparam_t *param,
  const char * const mark,				/* peaks previously marked			*/
  const int n,
  dtimage_t *plot,						/* image to plot in				    */
  double color);
