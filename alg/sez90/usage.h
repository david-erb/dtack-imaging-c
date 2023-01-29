/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*
Algorithm: dt_sez90
David Erb
Dtack Imaging Software

This help file contains the following sections:
   1. Usage
   2. Index

---------------------------------------------
1. Usage
Command line usage is:
dt_sez90 [params] input_image output_image

where params may be zero or more of:

-k           algorithm constant k (default 3.0) 
-lambda      algorithm constant lambda (default 100.0) 
-op          operation to perform (default is "default") 
-variant     algorithm variant to use (default "fast") 
-foreground  color of "lighted" output pixels (default 255) 
-background  color of "darkened" output pixels (default 0) 
-x           output x position of image data 
-y           output y position of image data 


The following standard params are also accepted:

-D      debug options separated by plus signs (+) 
-mem    memory sources available for large allocations 
-errto  output file for error messages 


The -op parameter may take one of the following values:

default  perform algorithm on input to produce output 
test     perform implementation self-tests and display pass/fail result 
time     perform implementation benchmark and display execution times 


The -variant parameter may take one of the following values:

fast  use fastest alternative 
easy  use most understandable alternative 


The test and time ops may be exercised under varying memory
conditions.
The minimum conditions are:

-op test  maxalloc 16384, pagesize 128, npages 1 
-op time  maxalloc 262144, pagesize 512, npages 1 


The -D parameter may take any of the following options
separated by plus signs (+).
Any option may be abbreviated to the shortest form
which is not ambiguous with other options.

any       all debug masks 
all       any debug mask 
stats     statistics 
img       image operations 
time      execution times 
complex   execution complexity indicators 
ipc       interprocess communication 
arg       arguments 
assert    assertions 
memtrace  ny memory allocation/freeing 
async     asynchronous events 
bigio     ig blocks of I/O only 
page      memory paging 
win       windowing operations 
alimits   print limits 
object    object values 
intermd   intermediate values 
pixel     pixel values 
io        all I/O 
memstats  memory summary 
alt       program-chosen alternative 
trace     execution trace 
usr1      user's use 
usr2      user's use 

-------------------- END OF FILE -------------------
*/
'A' ,'l' ,'g' ,'o' ,'r' ,'i' ,'t' ,'h' ,'m' ,':' ,' ' ,'d' ,
't' ,'_' ,'s' ,'e' ,'z' ,'9' ,'0' ,'\n','D' ,'a' ,'v' ,'i' ,
'd' ,' ' ,'E' ,'r' ,'b' ,'\n','D' ,'t' ,'a' ,'c' ,'k' ,' ' ,
'I' ,'m' ,'a' ,'g' ,'i' ,'n' ,'g' ,' ' ,'S' ,'o' ,'f' ,'t' ,
'w' ,'a' ,'r' ,'e' ,'\n','\n','T' ,'h' ,'i' ,'s' ,' ' ,'h' ,
'e' ,'l' ,'p' ,' ' ,'f' ,'i' ,'l' ,'e' ,' ' ,'c' ,'o' ,'n' ,
't' ,'a' ,'i' ,'n' ,'s' ,' ' ,'t' ,'h' ,'e' ,' ' ,'f' ,'o' ,
'l' ,'l' ,'o' ,'w' ,'i' ,'n' ,'g' ,' ' ,'s' ,'e' ,'c' ,'t' ,
'i' ,'o' ,'n' ,'s' ,':' ,'\n',' ' ,' ' ,' ' ,'1' ,'.' ,' ' ,
'U' ,'s' ,'a' ,'g' ,'e' ,'\n',' ' ,' ' ,' ' ,'2' ,'.' ,' ' ,
'I' ,'n' ,'d' ,'e' ,'x' ,'\n','\n','-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'\n','1' ,'.' ,' ' ,'U' ,'s' ,'a' ,'g' ,
'e' ,'\n','C' ,'o' ,'m' ,'m' ,'a' ,'n' ,'d' ,' ' ,'l' ,'i' ,
'n' ,'e' ,' ' ,'u' ,'s' ,'a' ,'g' ,'e' ,' ' ,'i' ,'s' ,':' ,
'\n','d' ,'t' ,'_' ,'s' ,'e' ,'z' ,'9' ,'0' ,' ' ,'[' ,'p' ,
'a' ,'r' ,'a' ,'m' ,'s' ,']' ,' ' ,'i' ,'n' ,'p' ,'u' ,'t' ,
'_' ,'i' ,'m' ,'a' ,'g' ,'e' ,' ' ,'o' ,'u' ,'t' ,'p' ,'u' ,
't' ,'_' ,'i' ,'m' ,'a' ,'g' ,'e' ,'\n','\n','w' ,'h' ,'e' ,
'r' ,'e' ,' ' ,'p' ,'a' ,'r' ,'a' ,'m' ,'s' ,' ' ,'m' ,'a' ,
'y' ,' ' ,'b' ,'e' ,' ' ,'z' ,'e' ,'r' ,'o' ,' ' ,'o' ,'r' ,
' ' ,'m' ,'o' ,'r' ,'e' ,' ' ,'o' ,'f' ,':' ,'\n','\n','-' ,
'k' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,
'a' ,'l' ,'g' ,'o' ,'r' ,'i' ,'t' ,'h' ,'m' ,' ' ,'c' ,'o' ,
'n' ,'s' ,'t' ,'a' ,'n' ,'t' ,' ' ,'k' ,' ' ,'(' ,'d' ,'e' ,
'f' ,'a' ,'u' ,'l' ,'t' ,' ' ,'3' ,'.' ,'0' ,')' ,' ' ,'\n',
'-' ,'l' ,'a' ,'m' ,'b' ,'d' ,'a' ,' ' ,' ' ,' ' ,' ' ,' ' ,
' ' ,'a' ,'l' ,'g' ,'o' ,'r' ,'i' ,'t' ,'h' ,'m' ,' ' ,'c' ,
'o' ,'n' ,'s' ,'t' ,'a' ,'n' ,'t' ,' ' ,'l' ,'a' ,'m' ,'b' ,
'd' ,'a' ,' ' ,'(' ,'d' ,'e' ,'f' ,'a' ,'u' ,'l' ,'t' ,' ' ,
'1' ,'0' ,'0' ,'.' ,'0' ,')' ,' ' ,'\n','-' ,'o' ,'p' ,' ' ,
' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'o' ,'p' ,'e' ,
'r' ,'a' ,'t' ,'i' ,'o' ,'n' ,' ' ,'t' ,'o' ,' ' ,'p' ,'e' ,
'r' ,'f' ,'o' ,'r' ,'m' ,' ' ,'(' ,'d' ,'e' ,'f' ,'a' ,'u' ,
'l' ,'t' ,' ' ,'i' ,'s' ,' ' ,'"' ,'d' ,'e' ,'f' ,'a' ,'u' ,
'l' ,'t' ,'"' ,')' ,' ' ,'\n','-' ,'v' ,'a' ,'r' ,'i' ,'a' ,
'n' ,'t' ,' ' ,' ' ,' ' ,' ' ,' ' ,'a' ,'l' ,'g' ,'o' ,'r' ,
'i' ,'t' ,'h' ,'m' ,' ' ,'v' ,'a' ,'r' ,'i' ,'a' ,'n' ,'t' ,
' ' ,'t' ,'o' ,' ' ,'u' ,'s' ,'e' ,' ' ,'(' ,'d' ,'e' ,'f' ,
'a' ,'u' ,'l' ,'t' ,' ' ,'"' ,'f' ,'a' ,'s' ,'t' ,'"' ,')' ,
' ' ,'\n','-' ,'f' ,'o' ,'r' ,'e' ,'g' ,'r' ,'o' ,'u' ,'n' ,
'd' ,' ' ,' ' ,'c' ,'o' ,'l' ,'o' ,'r' ,' ' ,'o' ,'f' ,' ' ,
'"' ,'l' ,'i' ,'g' ,'h' ,'t' ,'e' ,'d' ,'"' ,' ' ,'o' ,'u' ,
't' ,'p' ,'u' ,'t' ,' ' ,'p' ,'i' ,'x' ,'e' ,'l' ,'s' ,' ' ,
'(' ,'d' ,'e' ,'f' ,'a' ,'u' ,'l' ,'t' ,' ' ,'2' ,'5' ,'5' ,
')' ,' ' ,'\n','-' ,'b' ,'a' ,'c' ,'k' ,'g' ,'r' ,'o' ,'u' ,
'n' ,'d' ,' ' ,' ' ,'c' ,'o' ,'l' ,'o' ,'r' ,' ' ,'o' ,'f' ,
' ' ,'"' ,'d' ,'a' ,'r' ,'k' ,'e' ,'n' ,'e' ,'d' ,'"' ,' ' ,
'o' ,'u' ,'t' ,'p' ,'u' ,'t' ,' ' ,'p' ,'i' ,'x' ,'e' ,'l' ,
's' ,' ' ,'(' ,'d' ,'e' ,'f' ,'a' ,'u' ,'l' ,'t' ,' ' ,'0' ,
')' ,' ' ,'\n','-' ,'x' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,
' ' ,' ' ,' ' ,' ' ,'o' ,'u' ,'t' ,'p' ,'u' ,'t' ,' ' ,'x' ,
' ' ,'p' ,'o' ,'s' ,'i' ,'t' ,'i' ,'o' ,'n' ,' ' ,'o' ,'f' ,
' ' ,'i' ,'m' ,'a' ,'g' ,'e' ,' ' ,'d' ,'a' ,'t' ,'a' ,' ' ,
'\n','-' ,'y' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,
' ' ,' ' ,'o' ,'u' ,'t' ,'p' ,'u' ,'t' ,' ' ,'y' ,' ' ,'p' ,
'o' ,'s' ,'i' ,'t' ,'i' ,'o' ,'n' ,' ' ,'o' ,'f' ,' ' ,'i' ,
'm' ,'a' ,'g' ,'e' ,' ' ,'d' ,'a' ,'t' ,'a' ,' ' ,'\n','\n',
'\n','T' ,'h' ,'e' ,' ' ,'f' ,'o' ,'l' ,'l' ,'o' ,'w' ,'i' ,
'n' ,'g' ,' ' ,'s' ,'t' ,'a' ,'n' ,'d' ,'a' ,'r' ,'d' ,' ' ,
'p' ,'a' ,'r' ,'a' ,'m' ,'s' ,' ' ,'a' ,'r' ,'e' ,' ' ,'a' ,
'l' ,'s' ,'o' ,' ' ,'a' ,'c' ,'c' ,'e' ,'p' ,'t' ,'e' ,'d' ,
':' ,'\n','\n','-' ,'D' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'d' ,
'e' ,'b' ,'u' ,'g' ,' ' ,'o' ,'p' ,'t' ,'i' ,'o' ,'n' ,'s' ,
' ' ,'s' ,'e' ,'p' ,'a' ,'r' ,'a' ,'t' ,'e' ,'d' ,' ' ,'b' ,
'y' ,' ' ,'p' ,'l' ,'u' ,'s' ,' ' ,'s' ,'i' ,'g' ,'n' ,'s' ,
' ' ,'(' ,'+' ,')' ,' ' ,'\n','-' ,'m' ,'e' ,'m' ,' ' ,' ' ,
' ' ,' ' ,'m' ,'e' ,'m' ,'o' ,'r' ,'y' ,' ' ,'s' ,'o' ,'u' ,
'r' ,'c' ,'e' ,'s' ,' ' ,'a' ,'v' ,'a' ,'i' ,'l' ,'a' ,'b' ,
'l' ,'e' ,' ' ,'f' ,'o' ,'r' ,' ' ,'l' ,'a' ,'r' ,'g' ,'e' ,
' ' ,'a' ,'l' ,'l' ,'o' ,'c' ,'a' ,'t' ,'i' ,'o' ,'n' ,'s' ,
' ' ,'\n','-' ,'e' ,'r' ,'r' ,'t' ,'o' ,' ' ,' ' ,'o' ,'u' ,
't' ,'p' ,'u' ,'t' ,' ' ,'f' ,'i' ,'l' ,'e' ,' ' ,'f' ,'o' ,
'r' ,' ' ,'e' ,'r' ,'r' ,'o' ,'r' ,' ' ,'m' ,'e' ,'s' ,'s' ,
'a' ,'g' ,'e' ,'s' ,' ' ,'\n','\n','\n','T' ,'h' ,'e' ,' ' ,
'-' ,'o' ,'p' ,' ' ,'p' ,'a' ,'r' ,'a' ,'m' ,'e' ,'t' ,'e' ,
'r' ,' ' ,'m' ,'a' ,'y' ,' ' ,'t' ,'a' ,'k' ,'e' ,' ' ,'o' ,
'n' ,'e' ,' ' ,'o' ,'f' ,' ' ,'t' ,'h' ,'e' ,' ' ,'f' ,'o' ,
'l' ,'l' ,'o' ,'w' ,'i' ,'n' ,'g' ,' ' ,'v' ,'a' ,'l' ,'u' ,
'e' ,'s' ,':' ,'\n','\n','d' ,'e' ,'f' ,'a' ,'u' ,'l' ,'t' ,
' ' ,' ' ,'p' ,'e' ,'r' ,'f' ,'o' ,'r' ,'m' ,' ' ,'a' ,'l' ,
'g' ,'o' ,'r' ,'i' ,'t' ,'h' ,'m' ,' ' ,'o' ,'n' ,' ' ,'i' ,
'n' ,'p' ,'u' ,'t' ,' ' ,'t' ,'o' ,' ' ,'p' ,'r' ,'o' ,'d' ,
'u' ,'c' ,'e' ,' ' ,'o' ,'u' ,'t' ,'p' ,'u' ,'t' ,' ' ,'\n',
't' ,'e' ,'s' ,'t' ,' ' ,' ' ,' ' ,' ' ,' ' ,'p' ,'e' ,'r' ,
'f' ,'o' ,'r' ,'m' ,' ' ,'i' ,'m' ,'p' ,'l' ,'e' ,'m' ,'e' ,
'n' ,'t' ,'a' ,'t' ,'i' ,'o' ,'n' ,' ' ,'s' ,'e' ,'l' ,'f' ,
'-' ,'t' ,'e' ,'s' ,'t' ,'s' ,' ' ,'a' ,'n' ,'d' ,' ' ,'d' ,
'i' ,'s' ,'p' ,'l' ,'a' ,'y' ,' ' ,'p' ,'a' ,'s' ,'s' ,'/' ,
'f' ,'a' ,'i' ,'l' ,' ' ,'r' ,'e' ,'s' ,'u' ,'l' ,'t' ,' ' ,
'\n','t' ,'i' ,'m' ,'e' ,' ' ,' ' ,' ' ,' ' ,' ' ,'p' ,'e' ,
'r' ,'f' ,'o' ,'r' ,'m' ,' ' ,'i' ,'m' ,'p' ,'l' ,'e' ,'m' ,
'e' ,'n' ,'t' ,'a' ,'t' ,'i' ,'o' ,'n' ,' ' ,'b' ,'e' ,'n' ,
'c' ,'h' ,'m' ,'a' ,'r' ,'k' ,' ' ,'a' ,'n' ,'d' ,' ' ,'d' ,
'i' ,'s' ,'p' ,'l' ,'a' ,'y' ,' ' ,'e' ,'x' ,'e' ,'c' ,'u' ,
't' ,'i' ,'o' ,'n' ,' ' ,'t' ,'i' ,'m' ,'e' ,'s' ,' ' ,'\n',
'\n','\n','T' ,'h' ,'e' ,' ' ,'-' ,'v' ,'a' ,'r' ,'i' ,'a' ,
'n' ,'t' ,' ' ,'p' ,'a' ,'r' ,'a' ,'m' ,'e' ,'t' ,'e' ,'r' ,
' ' ,'m' ,'a' ,'y' ,' ' ,'t' ,'a' ,'k' ,'e' ,' ' ,'o' ,'n' ,
'e' ,' ' ,'o' ,'f' ,' ' ,'t' ,'h' ,'e' ,' ' ,'f' ,'o' ,'l' ,
'l' ,'o' ,'w' ,'i' ,'n' ,'g' ,' ' ,'v' ,'a' ,'l' ,'u' ,'e' ,
's' ,':' ,'\n','\n','f' ,'a' ,'s' ,'t' ,' ' ,' ' ,'u' ,'s' ,
'e' ,' ' ,'f' ,'a' ,'s' ,'t' ,'e' ,'s' ,'t' ,' ' ,'a' ,'l' ,
't' ,'e' ,'r' ,'n' ,'a' ,'t' ,'i' ,'v' ,'e' ,' ' ,'\n','e' ,
'a' ,'s' ,'y' ,' ' ,' ' ,'u' ,'s' ,'e' ,' ' ,'m' ,'o' ,'s' ,
't' ,' ' ,'u' ,'n' ,'d' ,'e' ,'r' ,'s' ,'t' ,'a' ,'n' ,'d' ,
'a' ,'b' ,'l' ,'e' ,' ' ,'a' ,'l' ,'t' ,'e' ,'r' ,'n' ,'a' ,
't' ,'i' ,'v' ,'e' ,' ' ,'\n','\n','\n','T' ,'h' ,'e' ,' ' ,
't' ,'e' ,'s' ,'t' ,' ' ,'a' ,'n' ,'d' ,' ' ,'t' ,'i' ,'m' ,
'e' ,' ' ,'o' ,'p' ,'s' ,' ' ,'m' ,'a' ,'y' ,' ' ,'b' ,'e' ,
' ' ,'e' ,'x' ,'e' ,'r' ,'c' ,'i' ,'s' ,'e' ,'d' ,' ' ,'u' ,
'n' ,'d' ,'e' ,'r' ,' ' ,'v' ,'a' ,'r' ,'y' ,'i' ,'n' ,'g' ,
' ' ,'m' ,'e' ,'m' ,'o' ,'r' ,'y' ,'\n','c' ,'o' ,'n' ,'d' ,
'i' ,'t' ,'i' ,'o' ,'n' ,'s' ,'.' ,'\n','T' ,'h' ,'e' ,' ' ,
'm' ,'i' ,'n' ,'i' ,'m' ,'u' ,'m' ,' ' ,'c' ,'o' ,'n' ,'d' ,
'i' ,'t' ,'i' ,'o' ,'n' ,'s' ,' ' ,'a' ,'r' ,'e' ,':' ,'\n',
'\n','-' ,'o' ,'p' ,' ' ,'t' ,'e' ,'s' ,'t' ,' ' ,' ' ,'m' ,
'a' ,'x' ,'a' ,'l' ,'l' ,'o' ,'c' ,' ' ,'1' ,'6' ,'3' ,'8' ,
'4' ,',' ,' ' ,'p' ,'a' ,'g' ,'e' ,'s' ,'i' ,'z' ,'e' ,' ' ,
'1' ,'2' ,'8' ,',' ,' ' ,'n' ,'p' ,'a' ,'g' ,'e' ,'s' ,' ' ,
'1' ,' ' ,'\n','-' ,'o' ,'p' ,' ' ,'t' ,'i' ,'m' ,'e' ,' ' ,
' ' ,'m' ,'a' ,'x' ,'a' ,'l' ,'l' ,'o' ,'c' ,' ' ,'2' ,'6' ,
'2' ,'1' ,'4' ,'4' ,',' ,' ' ,'p' ,'a' ,'g' ,'e' ,'s' ,'i' ,
'z' ,'e' ,' ' ,'5' ,'1' ,'2' ,',' ,' ' ,'n' ,'p' ,'a' ,'g' ,
'e' ,'s' ,' ' ,'1' ,' ' ,'\n','\n','\n','T' ,'h' ,'e' ,' ' ,
'-' ,'D' ,' ' ,'p' ,'a' ,'r' ,'a' ,'m' ,'e' ,'t' ,'e' ,'r' ,
' ' ,'m' ,'a' ,'y' ,' ' ,'t' ,'a' ,'k' ,'e' ,' ' ,'a' ,'n' ,
'y' ,' ' ,'o' ,'f' ,' ' ,'t' ,'h' ,'e' ,' ' ,'f' ,'o' ,'l' ,
'l' ,'o' ,'w' ,'i' ,'n' ,'g' ,' ' ,'o' ,'p' ,'t' ,'i' ,'o' ,
'n' ,'s' ,'\n','s' ,'e' ,'p' ,'a' ,'r' ,'a' ,'t' ,'e' ,'d' ,
' ' ,'b' ,'y' ,' ' ,'p' ,'l' ,'u' ,'s' ,' ' ,'s' ,'i' ,'g' ,
'n' ,'s' ,' ' ,'(' ,'+' ,')' ,'.' ,'\n','A' ,'n' ,'y' ,' ' ,
'o' ,'p' ,'t' ,'i' ,'o' ,'n' ,' ' ,'m' ,'a' ,'y' ,' ' ,'b' ,
'e' ,' ' ,'a' ,'b' ,'b' ,'r' ,'e' ,'v' ,'i' ,'a' ,'t' ,'e' ,
'd' ,' ' ,'t' ,'o' ,' ' ,'t' ,'h' ,'e' ,' ' ,'s' ,'h' ,'o' ,
'r' ,'t' ,'e' ,'s' ,'t' ,' ' ,'f' ,'o' ,'r' ,'m' ,'\n','w' ,
'h' ,'i' ,'c' ,'h' ,' ' ,'i' ,'s' ,' ' ,'n' ,'o' ,'t' ,' ' ,
'a' ,'m' ,'b' ,'i' ,'g' ,'u' ,'o' ,'u' ,'s' ,' ' ,'w' ,'i' ,
't' ,'h' ,' ' ,'o' ,'t' ,'h' ,'e' ,'r' ,' ' ,'o' ,'p' ,'t' ,
'i' ,'o' ,'n' ,'s' ,'.' ,'\n','\n','a' ,'n' ,'y' ,' ' ,' ' ,
' ' ,' ' ,' ' ,' ' ,' ' ,'a' ,'l' ,'l' ,' ' ,'d' ,'e' ,'b' ,
'u' ,'g' ,' ' ,'m' ,'a' ,'s' ,'k' ,'s' ,' ' ,'\n','a' ,'l' ,
'l' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'a' ,'n' ,'y' ,' ' ,
'd' ,'e' ,'b' ,'u' ,'g' ,' ' ,'m' ,'a' ,'s' ,'k' ,' ' ,'\n',
's' ,'t' ,'a' ,'t' ,'s' ,' ' ,' ' ,' ' ,' ' ,' ' ,'s' ,'t' ,
'a' ,'t' ,'i' ,'s' ,'t' ,'i' ,'c' ,'s' ,' ' ,'\n','i' ,'m' ,
'g' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'i' ,'m' ,'a' ,'g' ,
'e' ,' ' ,'o' ,'p' ,'e' ,'r' ,'a' ,'t' ,'i' ,'o' ,'n' ,'s' ,
' ' ,'\n','t' ,'i' ,'m' ,'e' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,
'e' ,'x' ,'e' ,'c' ,'u' ,'t' ,'i' ,'o' ,'n' ,' ' ,'t' ,'i' ,
'm' ,'e' ,'s' ,' ' ,'\n','c' ,'o' ,'m' ,'p' ,'l' ,'e' ,'x' ,
' ' ,' ' ,' ' ,'e' ,'x' ,'e' ,'c' ,'u' ,'t' ,'i' ,'o' ,'n' ,
' ' ,'c' ,'o' ,'m' ,'p' ,'l' ,'e' ,'x' ,'i' ,'t' ,'y' ,' ' ,
'i' ,'n' ,'d' ,'i' ,'c' ,'a' ,'t' ,'o' ,'r' ,'s' ,' ' ,'\n',
'i' ,'p' ,'c' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'i' ,'n' ,
't' ,'e' ,'r' ,'p' ,'r' ,'o' ,'c' ,'e' ,'s' ,'s' ,' ' ,'c' ,
'o' ,'m' ,'m' ,'u' ,'n' ,'i' ,'c' ,'a' ,'t' ,'i' ,'o' ,'n' ,
' ' ,'\n','a' ,'r' ,'g' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,
'a' ,'r' ,'g' ,'u' ,'m' ,'e' ,'n' ,'t' ,'s' ,' ' ,'\n','a' ,
's' ,'s' ,'e' ,'r' ,'t' ,' ' ,' ' ,' ' ,' ' ,'a' ,'s' ,'s' ,
'e' ,'r' ,'t' ,'i' ,'o' ,'n' ,'s' ,' ' ,'\n','m' ,'e' ,'m' ,
't' ,'r' ,'a' ,'c' ,'e' ,' ' ,' ' ,'n' ,'y' ,' ' ,'m' ,'e' ,
'm' ,'o' ,'r' ,'y' ,' ' ,'a' ,'l' ,'l' ,'o' ,'c' ,'a' ,'t' ,
'i' ,'o' ,'n' ,'/' ,'f' ,'r' ,'e' ,'e' ,'i' ,'n' ,'g' ,' ' ,
'\n','a' ,'s' ,'y' ,'n' ,'c' ,' ' ,' ' ,' ' ,' ' ,' ' ,'a' ,
's' ,'y' ,'n' ,'c' ,'h' ,'r' ,'o' ,'n' ,'o' ,'u' ,'s' ,' ' ,
'e' ,'v' ,'e' ,'n' ,'t' ,'s' ,' ' ,'\n','b' ,'i' ,'g' ,'i' ,
'o' ,' ' ,' ' ,' ' ,' ' ,' ' ,'i' ,'g' ,' ' ,'b' ,'l' ,'o' ,
'c' ,'k' ,'s' ,' ' ,'o' ,'f' ,' ' ,'I' ,'/' ,'O' ,' ' ,'o' ,
'n' ,'l' ,'y' ,' ' ,'\n','p' ,'a' ,'g' ,'e' ,' ' ,' ' ,' ' ,
' ' ,' ' ,' ' ,'m' ,'e' ,'m' ,'o' ,'r' ,'y' ,' ' ,'p' ,'a' ,
'g' ,'i' ,'n' ,'g' ,' ' ,'\n','w' ,'i' ,'n' ,' ' ,' ' ,' ' ,
' ' ,' ' ,' ' ,' ' ,'w' ,'i' ,'n' ,'d' ,'o' ,'w' ,'i' ,'n' ,
'g' ,' ' ,'o' ,'p' ,'e' ,'r' ,'a' ,'t' ,'i' ,'o' ,'n' ,'s' ,
' ' ,'\n','a' ,'l' ,'i' ,'m' ,'i' ,'t' ,'s' ,' ' ,' ' ,' ' ,
'p' ,'r' ,'i' ,'n' ,'t' ,' ' ,'l' ,'i' ,'m' ,'i' ,'t' ,'s' ,
' ' ,'\n','o' ,'b' ,'j' ,'e' ,'c' ,'t' ,' ' ,' ' ,' ' ,' ' ,
'o' ,'b' ,'j' ,'e' ,'c' ,'t' ,' ' ,'v' ,'a' ,'l' ,'u' ,'e' ,
's' ,' ' ,'\n','i' ,'n' ,'t' ,'e' ,'r' ,'m' ,'d' ,' ' ,' ' ,
' ' ,'i' ,'n' ,'t' ,'e' ,'r' ,'m' ,'e' ,'d' ,'i' ,'a' ,'t' ,
'e' ,' ' ,'v' ,'a' ,'l' ,'u' ,'e' ,'s' ,' ' ,'\n','p' ,'i' ,
'x' ,'e' ,'l' ,' ' ,' ' ,' ' ,' ' ,' ' ,'p' ,'i' ,'x' ,'e' ,
'l' ,' ' ,'v' ,'a' ,'l' ,'u' ,'e' ,'s' ,' ' ,'\n','i' ,'o' ,
' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'a' ,'l' ,'l' ,' ' ,
'I' ,'/' ,'O' ,' ' ,'\n','m' ,'e' ,'m' ,'s' ,'t' ,'a' ,'t' ,
's' ,' ' ,' ' ,'m' ,'e' ,'m' ,'o' ,'r' ,'y' ,' ' ,'s' ,'u' ,
'm' ,'m' ,'a' ,'r' ,'y' ,' ' ,'\n','a' ,'l' ,'t' ,' ' ,' ' ,
' ' ,' ' ,' ' ,' ' ,' ' ,'p' ,'r' ,'o' ,'g' ,'r' ,'a' ,'m' ,
'-' ,'c' ,'h' ,'o' ,'s' ,'e' ,'n' ,' ' ,'a' ,'l' ,'t' ,'e' ,
'r' ,'n' ,'a' ,'t' ,'i' ,'v' ,'e' ,' ' ,'\n','t' ,'r' ,'a' ,
'c' ,'e' ,' ' ,' ' ,' ' ,' ' ,' ' ,'e' ,'x' ,'e' ,'c' ,'u' ,
't' ,'i' ,'o' ,'n' ,' ' ,'t' ,'r' ,'a' ,'c' ,'e' ,' ' ,'\n',
'u' ,'s' ,'r' ,'1' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'u' ,'s' ,
'e' ,'r' ,'\'','s' ,' ' ,'u' ,'s' ,'e' ,' ' ,'\n','u' ,'s' ,
'r' ,'2' ,' ' ,' ' ,' ' ,' ' ,' ' ,' ' ,'u' ,'s' ,'e' ,'r' ,
'\'','s' ,' ' ,'u' ,'s' ,'e' ,' ' ,'\n','\n','-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'-' ,' ' ,'E' ,'N' ,'D' ,' ' ,'O' ,'F' ,
' ' ,'F' ,'I' ,'L' ,'E' ,' ' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,'-' ,
'-' ,'\n',





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
