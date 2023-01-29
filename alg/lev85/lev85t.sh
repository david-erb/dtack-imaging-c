echo dtlev85t.sh: ========== help ==============
sh sh/dtrun.sh $1 bin/dtlev85 -help
echo dtlev85t.sh: ========== HELP ==============
sh sh/dtrun.sh $1 bin/dtlev85 -HELP
echo dtlev85t.sh: ========== dice64 ==============
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_b08.pgm 0  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_b08.pgm 255

sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b16.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b32.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_s1.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_s2.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_s4.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_f1.pgm data/dice64/b_b08.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_f2.pgm data/dice64/b_b08.pgm 1  

sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_b16.pgm 65535  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_b32.pgm 4294967295
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_s1.pgm 127  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_s2.pgm 32767  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_s4.pgm 2147483647
sh sh/dtrun.sh $1 bin/dtlev85a data/dice64/g_b08.pgm data/dice64/b_f2.pgm 2.55e10 

sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 0   data/dice64/g_b08.pgm data/dice64/b_b08.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 1   data/dice64/g_b08.pgm data/dice64/b_b08.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 255 data/dice64/g_b08.pgm data/dice64/b_b08.pgm

sh sh/dtrun.sh $1 bin/dtlev85b data/dice64/g_b08.pgm data/dice64/b_b08.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op label data/dice64/g_b08.pgm data/dice64/b_b08.pgm

sh sh/dtrun.sh $1 bin/dtlev85c data/dice64/g_b08.pgm

sh sh/dtrun.sh $1 bin/dtlev85 -op blob data/dice64/g_b08.pgm data/dice64/b_b08.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op blob -min 11 data/dice64/g_b08.pgm data/dice64/b_b08.pgm

sh sh/dtrun.sh $1 bin/dtlev85e data/dice64/g_b08.pgm data/dice64/b_b08.pgm 0  
sh sh/dtrun.sh $1 bin/dtlev85e data/dice64/g_b08.pgm data/dice64/b_b08.pgm 1 
sh sh/dtrun.sh $1 bin/dtlev85e data/dice64/g_b08.pgm data/dice64/b_b08.pgm 255  

sh sh/dtrun.sh $1 bin/dtlev85d data/dice64/g_b08.pgm data/dice64/b_b08.pgm

echo dtlev85t.sh: ========== pool ball ==============
sh sh/dtrun.sh $1 bin/dtlev85a data/poolb_g.pgm data/poolb_s1.pgm 0  
sh sh/dtrun.sh $1 bin/dtlev85a data/poolb_g.pgm data/poolb_s1.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/poolb_g.pgm data/poolb_s1.pgm 255

sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 0   data/poolb_g.pgm data/poolb_s1.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 1   data/poolb_g.pgm data/poolb_s1.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 255 data/poolb_g.pgm data/poolb_s1.pgm

sh sh/dtrun.sh $1 bin/dtlev85b data/poolb_g.pgm data/poolb_s1.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op label data/poolb_g.pgm data/poolb_s1.pgm

sh sh/dtrun.sh $1 bin/dtlev85c data/poolb_g.pgm

sh sh/dtrun.sh $1 bin/dtlev85 -op blob data/poolb_g.pgm data/poolb_s1.pgm

sh sh/dtrun.sh $1 bin/dtlev85e data/poolb_g.pgm data/poolb_s1.pgm 0  
sh sh/dtrun.sh $1 bin/dtlev85e data/poolb_g.pgm data/poolb_s1.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85e data/poolb_g.pgm data/poolb_s1.pgm 255

sh sh/dtrun.sh $1 bin/dtlev85d data/poolb_g.pgm data/poolb_s1.pgm

echo dtlev85t.sh: ========== dice 256x256 ==============
sh sh/dtrun.sh $1 bin/dtlev85a data/dice.pgm data/dice_sez.pgm 0  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice.pgm data/dice_sez.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85a data/dice.pgm data/dice_sez.pgm 255

sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 0   data/dice.pgm data/dice_sez.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 1   data/dice.pgm data/dice_sez.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op total -b 255 data/dice.pgm data/dice_sez.pgm

sh sh/dtrun.sh $1 bin/dtlev85b data/dice.pgm data/dice_sez.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op label data/dice.pgm data/dice_sez.pgm

sh sh/dtrun.sh $1 bin/dtlev85c data/dice.pgm

sh sh/dtrun.sh $1 bin/dtlev85 -op blob data/dice.pgm data/dice_sez.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op blob -min 11 data/dice.pgm data/dice_sez.pgm

sh sh/dtrun.sh $1 bin/dtlev85e data/dice.pgm data/dice_sez.pgm 0  
sh sh/dtrun.sh $1 bin/dtlev85e data/dice.pgm data/dice_sez.pgm 1  
sh sh/dtrun.sh $1 bin/dtlev85e data/dice.pgm data/dice_sez.pgm 255

sh sh/dtrun.sh $1 bin/dtlev85d data/dice.pgm data/dice_sez.pgm

echo dtlev85t.sh: ========== timer256 ==============
### -max eliminates big white
sh sh/dtrun.sh $1 bin/dtlev85 -op blo -min 100 -max 40000 data/timer.pgm data/timer_s1.pgm

sh sh/dtrun.sh $1 bin/dtlev85b data/timer.pgm data/timer_s1.pgm
sh sh/dtrun.sh $1 bin/dtlev85 -op label data/timer.pgm data/timer_s1.pgm

sh sh/dtrun.sh $1 bin/dtlev85c data/timer.pgm

sh sh/dtrun.sh $1 bin/dtlev85e data/timer.pgm data/timer_s1.pgm 1  

sh sh/dtrun.sh $1 bin/dtlev85d data/timer.pgm data/timer_s1.pgm

echo dtlev85t.sh: ========== deliberate errors =====

sh sh/dtrun.sh $1 bin/dtlev85 -op qtime

echo dtlev85t.sh: ========== deliberate errors =====
echo GREP OFF error:
sh sh/dtrun.sh $1 bin/dtlev85
sh sh/dtrun.sh $1 bin/dtlev85 -op xxx
sh sh/dtrun.sh $1 bin/dtlev85 -sigma 1
sh sh/dtrun.sh $1 bin/dtlev85 xxx
sh sh/dtrun.sh $1 bin/dtlev85 xxx yyy
sh sh/dtrun.sh $1 bin/dtlev85 data/timer.pgm yyy

echo GREP ON error:
