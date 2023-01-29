this is file: ./data/readme.txt

This directory contains data files used during testing.

# Madison map shrunk to 256x240
mad256.pgm	dttool resample -op bilinear /img/nci/madison.pgm 2 2 mad256.pgm

# dice 256x256
dice.pgm
dice_sez.pgm  	segmented by sez90
dice_080.pgm	thresholded
dice_170.pgm	thresholded

# pool ball 172x144
poolb_g.pgm	pool ball, green band
poolb_s1.pgm	matte, round white balls
poolb_s2.pgm	matte, square white balls

# timer 256x256
timer.pgm	kitchen timer
timer_s1.pgm	dtsez90 -n 31 -dx 3
dt_sez90:   1. peak start   0  maximum   0  end   3  assigned gray   1
dt_sez90:   2. peak start  11  maximum  39  end  45  assigned gray  51
dt_sez90:   3. peak start  57  maximum  64  end  71  assigned gray 102
dt_sez90:   4. peak start  86  maximum 128  end 133  assigned gray 153
dt_sez90:   5. peak start 158  maximum 173  end 185  assigned gray 204
dt_sez90:   6. peak start 198  maximum 203  end 255  assigned gray 255
