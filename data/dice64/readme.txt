### up the precision with command like:
dttool regray -precision b16 g_b08.pgm g_b16.pgm
dttool regray -precision b16 b_b08.pgm b_b16.pgm

### using default min and max for s1 gives wrong uniformity answers
### probably because data range of input is 25 to 233 and regray into -128 to 127
### leaves empty bins which slightly disturb the variance
dttool regray -precision s1 -max 105 -min -103 g_b08.pgm g_s1.pgm

### using default min and max for f2 gives floating exception
dttool regray -precision f2 -max 2.55e10 -min 0 g_b08.pgm g_f2.pgm
dttool regray -precision f2 -max 2.55e10 -min 0 b_b08.pgm b_f2.pgm

