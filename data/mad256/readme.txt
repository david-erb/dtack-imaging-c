this is file: ./data/mad256/readme.txt

This directory contains data files used during testing convolution.

# Madison map image thresholded to 5 levels
t5.bpe		dttool multithresh mad256.pgm 90 120 144 166 t5.bpe

# multithresholded image convolved with averaging kernel
t5ave3.bpe	dttool convolve -precision s2 t5.bpe ave t5ave3.bpe

# multithresholded image convolved with highpass kernel
t5high3.bpe	dttool convolve -precision s2 t5.bpe highpass t5high3.bpe

# multithresholded image convolved with 5x5 averaging kernel
t5ave5.bpe	dttool convolve -precision s2 t5.bpe ave5 t5ave5.bpe

# multithresholded image convolved with 5x5 averaging kernel
t5ave7.bpe	dttool convolve -precision s2 t5.bpe ave7 t5ave7.bpe



