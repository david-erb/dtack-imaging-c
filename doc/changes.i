| CHANGES ./include/dtack/xchg.h          	
| CHANGES ./app/tabrobot/server.c         	
| 20010104 ./source/low/liner.c            	fix error checking for crlf on end of line (purify) 
| 20010104 ./source/high/scan.c            	init p and blob members to keep purify happy 
| 20010104 ./source/dg/line.c              	fix error checking for crlf on end of line (purify) 
| 20001130 ./source/low/msw.c              	use custom wndproc and exit on certain messages 
| 20001130 ./source/low/msw.c              	different logic in yield to call defwndproc better 
| 20001019 ./source/low/serialw.c          	added to the library 
| 20001019 ./source/low/serialu.c          	added to the library 
| 20001019 ./source/low/serial.c           	added to the library 
| 20001019 ./include/dtack/serial.h        	added to the library 
| 20001003 ./source/low/threadu.c          	remove pthread support for Solaris 
| 20000808 ./source/low/eventw.c           	added to the library 
| 20000807 ./source/low/os_delay.c         	experimental MSW32 delay using select() (commented out) 
| 20000807 ./source/low/enter.c            	added win, yield, and mem to exclude list 
| 20000720 ./app/tabrobot/tabrobot.c       	add delay scale factor param->b 
| 20000720 ./app/tabrobot/server.c         	added read poll, check for negative delay 
| 20000720 ./app/tabrobot/server.c         	add delay scale factor param->b 
| 20000717 ./app/tabrobot/tabroam.c        	added to suite 
| 20000717 ./app/tabrobot/preset.c         	added tabrobot_preset_load() function 
| 20000630 ./source/low/rev.c              	changed wrong MAJOR to MINOR 
| 20000630 ./include/dtack/base.h          	01-30 
| 20000615 ./source/low/msw.c              	yield for posted messages also, call DefWindowProc 
| 20000508 ./source/low/semu.c             	removed dummy functions for windows (now in semw.c) 
| 20000508 ./source/dg/dgserver.c          	mark structure initialized 
| 20000508 ./source/dg/dgserver.c          	conditional unregister of ipc 
| 20000507 ./source/low/threadw.c          	added to the library 
| 20000507 ./source/low/threadu.c          	added to the library 
| 20000507 ./source/low/thread.c           	added to the library 
| 20000507 ./source/low/semw.c             	added to the library 
| 20000507 ./include/dtack/threads.h       	added to the library 
| 20000507 ./include/dtack/thread.h        	added to the library 
| 20000506 ./source/screen/scr_main.c      	move list destroy call 
| 20000506 ./source/screen/hwsmsw.c        	added mouse click event, invalidate rect on export_data 
| 20000506 ./source/high/resample.c        	fixed bug when enlarging b08 by 2 
| 20000505 ./source/xchg/xchg_jpg.c        	added luminance flag to decompression 
| 20000505 ./include/dtack/xchg.h          	added luminance flag 
| 20000426 ./source/low/ipcrock.c          	added comment 
| 20000411 ./test/high/dttmorph.c          	test edges with marker value 
| 20000411 ./source/low/rev.c              	added dtrev_base function 
| 20000404 ./test/high/dttmorph.c          	added median 
| 20000404 ./source/high/median.c          	added new ntypes 
| 20000306 ./source/low/os_futil.c         	added support for bare drive identifier on NT 
| 20000305 ./source/dg/header.c            	use SCHEDULE debug level for file times 
| 20000304 ./source/low/fd.c               	change backslashes in filenames 
| 20000228 ./source/low/semu.c             	added dummy functions for windows 
| 20000223 ./source/low/ipc.c              	added dtipc_write_all2 with timeout 
| 20000223 ./source/low/f.c                	different declaration for dtf_open 
| 20000223 ./app/tabrobot/tabrobot.c       	added param->epsilon 
| 20000223 ./app/tabrobot/server.c         	added param->epsilon 
| 20000223 ./app/tabrobot/roam.c           	added timeouts 
| 20000223 ./app/tabrobot/client.c         	added timeouts 
| 20000218 ./source/low/f.c                	wrong argument in dtfd_read call 
| 20000204 ./source/low/fd.c               	added signature 
| 20000204 ./include/dtack/fd.h            	added signature 
| 20000203 ./include/dtack/cipher.h        	created 
| 20000202 ./source/low/f.c                	created 
| 20000202 ./include/dtack/f.h             	created 
| 20000129 ./source/low/os_futil.c         	ifdef struct stat stuff 
| 20000128 ./include/dtack/base.h          	01-26 
| 20000125 ./app/tabrobot/ptzpela.c        	added iris 
| 20000110 ./include/dtack/base.h          	01-23 
| 20000109 ./source/low/liner.c            	treat lines ending in backslash as having continuation 
| 20000106 ./include/dtack/base.h          	01-22 
| 20000104 ./source/low/semu.c             	properly declare union semun for egcs 
| 20000104 ./source/low/param.c            	properly initialize table for egcs 
| 20000104 ./source/low/list.c             	init after malloc and realloc (purify) 
| 20000104 ./source/low/fd.c               	properly initialize dtfd_clear for egcs 
| 20000104 ./source/low/cipher.c           	created 
| 20000104 ./include/dtack/main.h          	proper initialization of structures for egcs 
| 20000104 ./include/dtack/config2.h       	remove gnu/types.h 
| 20000102 ./include/dtack/licens.h        	added extern "C" 
| 20000102 ./include/dtack/1d.h            	added WV5 to dll list 
| 19991231 ./source/low/semw.c             	created 
| 19991226 ./source/dg/dgserver.c          	free body list at uninit 
| 19991219 ./source/low/yield.c            	send dt_yield as caller to dt_say 
| 19991219 ./source/low/str.c              	added more special characters to dtstr_htmlencode() 
| 19991219 ./source/high/fftw.c            	added yields 
| 19991219 ./include/dtack/1d.h            	moved ifdef __cplusplus 
| 19991218 ./source/high/col.c             	changed some long to dtntype_s4_t 
| 19991217 ./test/xchg/dttxchg.c           	data/dttxchg3.tiff to data/dttxchg3.tif 
| 19991217 ./test/high/dttrun.c            	fixed some format statements 
| 19991217 ./test/1d/dtt1d.c               	changed expected cast for "in stride and out stride, changed type" 
| 19991217 ./source/xchg/wmf.c             	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./source/low/hist.c             	changed some example printf casts 
| 19991217 ./source/high/scan.c            	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./source/high/runf.c            	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./source/high/label2.c          	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./source/high/convolve.c        	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./source/high/conv_l.c          	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./include/dtack/wmf.h           	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./include/dtack/dib.h           	changed some long's to 4-byte int's in favor of 64-bit CPU 
| 19991217 ./include/dtack/config2.h       	split out from config.h 
| 19991217 ./include/dtack/config.h        	split out stuff into config2.h 
| 19991217 ./include/dtack/base.h          	introduction of config2.h 
| 19991211 ./source/low/param.c            	remove DTPARAM_FLAG_ALLOC definition 
| 19991211 ./source/low/enter.c            	handle "null" value for errto, dbgto and sayto 
| 19991207 ./source/dg/line.c              	added return argument for number of bytes read 
| 19991207 ./source/dg/dgserver.c          	added ability to read posted data 
| 19991206 ./source/low/dbtable.c          	fix 64-bit compile error 
| 19991124 ./tool/gen.c                    	allow more data types 
| 19991109 ./app/wrangler/procsu.c         	care abount unacknowledged dead child 
| 19991009 ./source/low/prj_dat.c          	enable dos key mappings in console mode 
| 19991009 ./source/low/msw.c              	bug calling yield with NULL ctl 
| 19990826 ./source/low/ipcrock.c          	fixed done function for NT 
| 19990824 ./source/low/ipcrock.c          	spec flags for linger and shutdown 
| 19990819 ./source/low/os_time.c          	add dtos_time_str1() 
| 19990815 ./source/dg/header.c            	release header lines before getting new ones 
| 19990814 ./source/low/os_futil.c         	added new functions 
| 19990814 ./source/dg/request.c           	added custom separator 
| 19990813 ./source/high/blob.c            	check for null image 
| 19990810 ./source/low/list.c             	added sort function 
| 19990810 ./source/low/fd.c               	use strlen if write length is -1 
| 19990810 ./include/dtack/stats.h         	changed some int to dtstats_xy_t 
| 19990810 ./include/dtack/base.h          	01-20 
| 19990809 ./source/1d/1d_sort5.c          	added sort func with arg 
| 19990809 ./include/dtack/base.h          	changed dtxy_t to dtstats_xy_t 
| 19990809 ./include/dtack/1d.h            	added compare func with arg 
| 19990806 ./source/low/str.c              	added dtstr_htmlencode() and cgiencode() 
| 19990804 ./test/high/dttdb.c             	added to the suite of tests 
| 19990803 ./source/low/list.c             	added nth function 
| 19990803 ./source/low/dbtable.c          	added to the library 
| 19990725 ./source/low/ipc.c              	use strlen(data) if length is -1 
| 19990723 ./source/low/os_mem.c           	pre-filter debug 
| 19990723 ./source/low/liner.c            	new functions to keep blank and comment lines 
| 19990721 ./source/low/fd.c               	added buffering scheme to speed up short reads 
| 19990721 ./include/dtack/fd.h            	added internal buffering 
| 19990718 ./source/low/fd.c               	check mask before debugging (for efficiency only) 
| 19990718 ./include/dtack/base.h          	01-19 
| 19990705 ./include/dtack/base.h          	01-18 
| 19990619 ./app/tableau/tableau.c         	say rev when usr1 
| 19990619 ./app/tableau/tableau.c         	explicitly non-console app 
| 19990619 ./app/tableau/main.c            	explicitly non-console app 
| 19990619 ./app/tabchief/tabchief.c       	no delay before reboot 
| 19990619 ./app/tabchief/tabchief.c       	explicitly non-console app 
| 19990619 ./app/tabchief/tabchief.c       	added rev 
| 19990519 ./app/tabrobot/tabptz.c         	poll for command/data 
| 19990517 ./app/tabrobot/tabrobot.c       	explicitly non-console app 
| 19990517 ./app/tabrobot/tabptz.c         	explicitly non-console app 
| 19990517 ./app/tabrobot/preset.c         	added param->precision to presets filename generation 
| 19990501 ./source/low/say.c              	modified support MSW console mode 
| 19990501 ./source/low/prj_dat.c          	support MSW console mode 
| 19990501 ./source/low/err.c              	modified support MSW console mode 
| 19990501 ./source/low/dbg.c              	modified support MSW console mode 
| 19990207 ./include/dtack/base.h          	01-17 
| 19990206 ./include/dtack/msw.h           	defined STRICT 1 
| 19990206 ./include/dtack/main.h          	cleanup console macro 
| 19990201 ./include/dtack/fd.h            	ifdef around declaration of dtfd_clear 
| 19990130 ./test/high/dttmatrix.c         	added to the suite of tests 
| 19990130 ./test/1d/dtt1d.c               	put ifdef before FFT test 
| 19990130 ./source/win/winc_ipc.c         	timeout 5 seconds looking for dtview server 
| 19990130 ./source/low/poly.c             	added to the library 
| 19990130 ./source/low/points.c           	added to the library 
| 19990130 ./source/low/matrix3d.c         	added to the library 
| 19990130 ./source/low/matrix2d.c         	added to the library 
| 19990130 ./source/low/matrix.c           	added to the library 
| 19990130 ./source/high/matrix.c          	added to the library 
| 19990126 ./app/tableau/tableau.c         	change recv to recvt 
| 19990124 ./test/high/dttresam.c          	added some debug 
| 19990124 ./source/low/say.c              	support MSW console mode 
| 19990124 ./source/low/err.c              	support MSW console mode 
| 19990124 ./source/low/dbg.c              	support MSW console mode 
| 19990124 ./source/1d/1d_cmp.c            	workaround for Visual C++ optimizer bug 
| 19990124 ./include/dtack/main.h          	added rwd (remove-when-done) logic 
| 19990124 ./include/dtack/base.h          	01-16 
| 19990124 ./dtutil/dtutil.c               	added wait for file to disappear 
| 19990122 ./app/tabrobot/server.c         	check for client NULL in server_poll 
| 19990122 ./app/tabrobot/log.c            	don't emit EOF write errors 
| 19990114 ./source/low/os_futil.c         	changed debugging messages 
| 19990105 ./test/high/dttconv.c           	test for big endian machine before reading data files 
| 19990102 ./source/low/rev.c              	added dbg calls 
| 19990102 ./source/low/msgu.c             	don't give error on receive truncation 
| 19990102 ./source/low/msgu.c             	added const to post and added flush 
| 19990101 ./source/low/ipcrock.c          	reduce "waiting for read" messages 
| 19981231 ./source/high/thresh.c          	use 1d to do the work and handle all types 
| 19981231 ./source/fb/fbsnap.c            	snappy.h not inside ifdef 
| 19981230 ./source/low/str.c              	obvious bug in dtstr_extract() 
| 19981230 ./app/tabrobot/ptzipc.c         	added connect timeout 
| 19981219 ./source/high/image.c           	changed error message 
| 19981214 ./include/dtack/main.h          	int main 
| 19981213 ./source/low/semu.c             	can't pass NULL for senum 
| 19981212 ./test/high/dttresam.c          	changed cmp type to avoid compilation error 
| 19981212 ./source/high/morph.c           	removed unrechable return statements 
| 19981208 ./test/high/dttresam.c          	added to the suite of tests 
| 19981208 ./test/high/dttmorph.c          	added to the suite of tests 
| 19981208 ./source/high/resample.c        	optimized 
| 19981208 ./source/high/morph.c           	optimized 
| 19981203 ./source/dg/dgclient.c          	added ability to send extra header fields 
| 19981127 ./source/fb/fbvfw.c             	added raw buffer concept 
| 19981123 ./source/low/fd.c               	fixed bug in debug 
| 19981016 ./include/dtack/base.h          	01-15 
| 19981015 ./source/high/fftw.c            	check for segmented output 
| 19981015 ./include/dtack/config.h        	raised MALLOC_MAX to bigger number 
| 19981015 ./include/dtack/base.h          	added DT_CLEANUP 
| 19981014 ./source/screen/scrhws.c        	don't clobber mouse location 
| 19981014 ./source/screen/scr_main.c      	different cursor positions 
| 19981014 ./source/screen/scr_act.c       	handle mouse event mappings different in hws systems 
| 19981014 ./source/screen/hwsx.c          	remove sunpro1 compiler warnings 
| 19981005 ./source/xchg/xchg_jpg.c        	malloc_max redefined 
| 19980924 ./source/high/image.c           	added FLAG_OVERLAY 
| 19980924 ./source/high/hist1.c           	bug not checking *cando 
| 19980924 ./include/dtack/image.h         	added FLAG_OVERLAY 
| 19980923 ./source/xchg/read.c            	added rz to automatically scan for gzipped input files 
| 19980923 ./source/low/fd.c               	added rz to input modes 
| 19980920 ./source/low/ipcrock.c          	yield during write and read 
| 19980905 ./source/low/fd.c               	add explicit member isopen 
| 19980905 ./source/high/gen1.c            	add gen_distance routine 
| 19980905 ./include/dtack/fd.h            	add explicit member isopen 
| 19980818 ./source/low/ipcrock.c          	ifdeffed out KEEPALIVE=0 
| 19980724 ./source/low/list.c             	fixed bug in after() when expanding list 
| 19980718 ./source/low/os_mem.c           	changed free to DT_FREE 
| 19980718 ./include/dtack/main.h          	changed free to DT_FREE 
| 19980716 ./source/xchg/xchg_jpg.c        	remove sunpro1 compiler warnings 
| 19980716 ./source/screen/scr_act.c       	remove sunpro1 compiler warnings 
| 19980716 ./source/screen/hwsx.c          	check for negative time in double-click 
| 19980716 ./source/low/os_time.c          	remove sunpro0 compiler errors 
| 19980716 ./source/low/os_delay.c         	extra delay time for all Sun platforms 
| 19980716 ./source/low/ntype_da.c         	remove sunpro1 compiler warnings 
| 19980716 ./source/low/fd.c               	remove sunpro1 compiler warnings 
| 19980716 ./source/low/enter.c            	remove sunpro1 compiler warnings 
| 19980716 ./source/low/cons.c             	remove sunpro1 compiler warnings 
| 19980716 ./source/high/noise.c           	remove sunpro1 compiler warnings 
| 19980716 ./source/bigmem/font.c          	remove sunpro1 compiler warnings 
| 19980716 ./source/1d/1d_ntype.c          	remove sunpro1 compiler warnings 
| 19980716 ./source/1d/1d_conv.c           	remove sunpro1 compiler warnings 
| 19980716 ./include/dtack/config.h        	corrected wrong IMPLIED for SP1 
| 19980716 ./include/dtack/config.h        	added DTCONFIG_FREE_ARGTYPE 
| 19980716 ./include/dtack/base.h          	added DT_MEMCPY, DT_MEMSET, DT_FREE 
| 19980712 ./source/high/stats.c           	proper computation of standard deviation, based on variance 
| 19980712 ./source/high/fftw.c            	PRJ_FFTW_DEFINE removed in favor of $(MK_FFTW_DEFINE) 
| 19980712 ./source/1d/1d_fftw.c           	PRJ_FFTW_DEFINE removed in favor of $(MK_FFTW_DEFINE) 
| 19980525 ./source/low/list.c             	print elem name as it is freed 
| /*.......................................................................... ./app/tabrobot/server.c         	
|  ./app/tabrobot/server.c         	
