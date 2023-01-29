# This makefile is input to nmake, the Microsoft make utility.
# This makefile assumes the following:
# 1. that the INCLUDE environment variable points to the standard Microsoft
#    include directories, for example:
#      set INCLUDE=c:\msvc\include
# 2. that the PATH environment variable has the compiler in it, for example:
#      PATH=c:\mvsc\bin;c:\dos

!include "$(SET)\wv5.set"

#                                   optimize for speed
OPTIMIZE_HIGH = /O2

#                                   good optimization but still able to debug
OPTIMIZE_LOW = 




default: shared


all: statics shareds others


static: statics
statics: libs exes 

shared: shareds
shareds: shlibs shexes 

allsubs:

exes: \
	  $(OUT)\static\dtenv1.exe \
	  $(OUT)\static\dtenv2.exe \
	  $(OUT)\static\dtenv3.exe \
	  $(OUT)\static\dtenv4.exe \
	  $(OUT)\static\dthist1.exe \
	  $(OUT)\static\dthist2.exe \
	  $(OUT)\static\dtparam1.exe \
	  $(OUT)\static\dtport1.exe

shexes: \
	  $(OUT)\dynamic\dtenv1.exe \
	  $(OUT)\dynamic\dtenv2.exe \
	  $(OUT)\dynamic\dtenv3.exe \
	  $(OUT)\dynamic\dtenv4.exe \
	  $(OUT)\dynamic\dthist1.exe \
	  $(OUT)\dynamic\dthist2.exe \
	  $(OUT)\dynamic\dtparam1.exe \
	  $(OUT)\dynamic\dtport1.exe

scripts: 

libs: \
	  $(OUT)\lib\dtlow.lib

shlibs: \
	  $(OUT)\dynamic\dtlow.dll

others: 


#----------------------------------------------------------------------------
strip:
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\dtenv1.exe
	-del $(OUT)\dynamic\dtenv2.exe
	-del $(OUT)\dynamic\dtenv3.exe
	-del $(OUT)\dynamic\dtenv4.exe
	-del $(OUT)\dynamic\dthist1.exe
	-del $(OUT)\dynamic\dthist2.exe
	-del $(OUT)\dynamic\dtparam1.exe
	-del $(OUT)\dynamic\dtport1.exe
	-del $(OUT)\static\dtenv1.exe
	-del $(OUT)\static\dtenv2.exe
	-del $(OUT)\static\dtenv3.exe
	-del $(OUT)\static\dtenv4.exe
	-del $(OUT)\static\dthist1.exe
	-del $(OUT)\static\dthist2.exe
	-del $(OUT)\static\dtparam1.exe
	-del $(OUT)\static\dtport1.exe
	-del $(OUT)\lib\dtlow.lib
	-del $(OUT)\dynamic\dtlow.dll
	-del $(OUT)\source\low\*.obj
	-del *.out
	-del tmp
	-del *.map

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)\static\dtenv1.exe: \
	  $(OUT)\source\low\dtenv1.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtenv1.exe @<<
	  /MAP:$(OUT)\static\dtenv1.map
	  /PDB:$(OUT)\static\dtenv1.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dtenv1.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtenv1.res
<<
$(OUT)\dynamic\dtenv1.exe: \
	  $(OUT)\source\low\dtenv1.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtenv1.exe @<<
	  /MAP:$(OUT)\dynamic\dtenv1.map 
	  /PDB:$(OUT)\dynamic\dtenv1.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dtenv1.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtenv1.lib
	del $(OUT)\dynamic\dtenv1.exp


$(OUT)\static\dtenv2.exe: \
	  $(OUT)\source\low\dtenv2.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtenv2.exe @<<
	  /MAP:$(OUT)\static\dtenv2.map
	  /PDB:$(OUT)\static\dtenv2.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dtenv2.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtenv2.res
<<
$(OUT)\dynamic\dtenv2.exe: \
	  $(OUT)\source\low\dtenv2.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtenv2.exe @<<
	  /MAP:$(OUT)\dynamic\dtenv2.map 
	  /PDB:$(OUT)\dynamic\dtenv2.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dtenv2.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtenv2.lib
	del $(OUT)\dynamic\dtenv2.exp


$(OUT)\static\dtenv3.exe: \
	  $(OUT)\source\low\dtenv3.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtenv3.exe @<<
	  /MAP:$(OUT)\static\dtenv3.map
	  /PDB:$(OUT)\static\dtenv3.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dtenv3.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtenv3.res
<<
$(OUT)\dynamic\dtenv3.exe: \
	  $(OUT)\source\low\dtenv3.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtenv3.exe @<<
	  /MAP:$(OUT)\dynamic\dtenv3.map 
	  /PDB:$(OUT)\dynamic\dtenv3.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dtenv3.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtenv3.lib
	del $(OUT)\dynamic\dtenv3.exp


$(OUT)\static\dtenv4.exe: \
	  $(OUT)\source\low\dtenv4.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtenv4.exe @<<
	  /MAP:$(OUT)\static\dtenv4.map
	  /PDB:$(OUT)\static\dtenv4.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dtenv4.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtenv4.res
<<
$(OUT)\dynamic\dtenv4.exe: \
	  $(OUT)\source\low\dtenv4.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtenv4.exe @<<
	  /MAP:$(OUT)\dynamic\dtenv4.map 
	  /PDB:$(OUT)\dynamic\dtenv4.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dtenv4.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtenv4.lib
	del $(OUT)\dynamic\dtenv4.exp


$(OUT)\static\dthist1.exe: \
	  $(OUT)\source\low\dthist1.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dthist1.exe @<<
	  /MAP:$(OUT)\static\dthist1.map
	  /PDB:$(OUT)\static\dthist1.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dthist1.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dthist1.res
<<
$(OUT)\dynamic\dthist1.exe: \
	  $(OUT)\source\low\dthist1.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dthist1.exe @<<
	  /MAP:$(OUT)\dynamic\dthist1.map 
	  /PDB:$(OUT)\dynamic\dthist1.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dthist1.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dthist1.lib
	del $(OUT)\dynamic\dthist1.exp


$(OUT)\static\dthist2.exe: \
	  $(OUT)\source\low\dthist2.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dthist2.exe @<<
	  /MAP:$(OUT)\static\dthist2.map
	  /PDB:$(OUT)\static\dthist2.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dthist2.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dthist2.res
<<
$(OUT)\dynamic\dthist2.exe: \
	  $(OUT)\source\low\dthist2.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dthist2.exe @<<
	  /MAP:$(OUT)\dynamic\dthist2.map 
	  /PDB:$(OUT)\dynamic\dthist2.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dthist2.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dthist2.lib
	del $(OUT)\dynamic\dthist2.exp


$(OUT)\static\dtparam1.exe: \
	  $(OUT)\source\low\dtparam1.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtparam1.exe @<<
	  /MAP:$(OUT)\static\dtparam1.map
	  /PDB:$(OUT)\static\dtparam1.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dtparam1.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtparam1.res
<<
$(OUT)\dynamic\dtparam1.exe: \
	  $(OUT)\source\low\dtparam1.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtparam1.exe @<<
	  /MAP:$(OUT)\dynamic\dtparam1.map 
	  /PDB:$(OUT)\dynamic\dtparam1.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dtparam1.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtparam1.lib
	del $(OUT)\dynamic\dtparam1.exp


$(OUT)\static\dtport1.exe: \
	  $(OUT)\source\low\dtport1.obj \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtport1.exe @<<
	  /MAP:$(OUT)\static\dtport1.map
	  /PDB:$(OUT)\static\dtport1.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\source\low\dtport1.obj
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtport1.res
<<
$(OUT)\dynamic\dtport1.exe: \
	  $(OUT)\source\low\dtport1.obj \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtport1.exe @<<
	  /MAP:$(OUT)\dynamic\dtport1.map 
	  /PDB:$(OUT)\dynamic\dtport1.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\source\low\dtport1.obj
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtport1.lib
	del $(OUT)\dynamic\dtport1.exp







#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dtlow.lib: \
	  $(OUT)\source\low\dbtable.obj \
	  $(OUT)\source\low\points.obj \
	  $(OUT)\source\low\poly.obj \
	  $(OUT)\source\low\matrix3d.obj \
	  $(OUT)\source\low\matrix2d.obj \
	  $(OUT)\source\low\matrix.obj \
	  $(OUT)\source\low\assert.obj \
	  $(OUT)\source\low\cipher.obj \
	  $(OUT)\source\low\cons.obj \
	  $(OUT)\source\low\cstack.obj \
	  $(OUT)\source\low\dbg.obj \
	  $(OUT)\source\low\dbg2.obj \
	  $(OUT)\source\low\err.obj \
	  $(OUT)\source\low\err2.obj \
	  $(OUT)\source\low\enter.obj \
	  $(OUT)\source\low\eventw.obj \
	  $(OUT)\source\low\f.obj \
	  $(OUT)\source\low\fd.obj \
	  $(OUT)\source\low\ftp.obj \
	  $(OUT)\source\low\heapmon.obj \
	  $(OUT)\source\low\heap2.obj \
	  $(OUT)\source\low\hist.obj \
	  $(OUT)\source\low\hist2.obj \
	  $(OUT)\source\low\hist_da.obj \
	  $(OUT)\source\low\ipc.obj \
	  $(OUT)\source\low\ipcqnx4.obj \
	  $(OUT)\source\low\ipcqbaby.obj \
	  $(OUT)\source\low\ipcrock.obj \
	  $(OUT)\source\low\ipcdde.obj \
	  $(OUT)\source\low\leave.obj \
	  $(OUT)\source\low\license.obj \
	  $(OUT)\source\low\liner.obj \
	  $(OUT)\source\low\list.obj \
	  $(OUT)\source\low\list_io.obj \
	  $(OUT)\source\low\lut8.obj \
	  $(OUT)\source\low\lut8_mat.obj \
	  $(OUT)\source\low\msgu.obj \
	  $(OUT)\source\low\msw.obj \
	  $(OUT)\source\low\os_mem.obj \
	  $(OUT)\source\low\os_delay.obj \
	  $(OUT)\source\low\os_futil.obj \
	  $(OUT)\source\low\os_heap.obj \
	  $(OUT)\source\low\os_heap2.obj \
	  $(OUT)\source\low\os_time.obj \
	  $(OUT)\source\low\os_env.obj \
	  $(OUT)\source\low\param.obj \
	  $(OUT)\source\low\phys.obj \
	  $(OUT)\source\low\pnm.obj \
	  $(OUT)\source\low\portio.obj \
	  $(OUT)\source\low\prj.obj \
	  $(OUT)\source\low\qnx4.obj \
	  $(OUT)\source\low\rev.obj \
	  $(OUT)\source\low\say.obj \
	  $(OUT)\source\low\semu.obj \
	  $(OUT)\source\low\semw.obj \
	  $(OUT)\source\low\serial.obj \
	  $(OUT)\source\low\serialu.obj \
	  $(OUT)\source\low\serialw.obj \
	  $(OUT)\source\low\shmu.obj \
	  $(OUT)\source\low\shmw.obj \
	  $(OUT)\source\low\str.obj \
	  $(OUT)\source\low\threads.obj \
	  $(OUT)\source\low\thread.obj \
	  $(OUT)\source\low\threadu.obj \
	  $(OUT)\source\low\threadw.obj \
	  $(OUT)\source\low\usage.obj \
	  $(OUT)\source\low\yield.obj \
	  $(OUT)\source\low\libmain.obj \
	  $(OUT)\source\low\dbg_dat.obj \
	  $(OUT)\source\low\conf_dat.obj \
	  $(OUT)\source\low\lut8_dat.obj \
	  $(OUT)\source\low\ntype.obj \
	  $(OUT)\source\low\ntype_da.obj \
	  $(OUT)\source\low\prj_dat.obj
	lib /nologo /out:$(OUT)\lib\dtlow.lib @<<
	  $(OUT)\source\low\dbtable.obj
	  $(OUT)\source\low\points.obj
	  $(OUT)\source\low\poly.obj
	  $(OUT)\source\low\matrix3d.obj
	  $(OUT)\source\low\matrix2d.obj
	  $(OUT)\source\low\matrix.obj
	  $(OUT)\source\low\assert.obj
	  $(OUT)\source\low\cipher.obj
	  $(OUT)\source\low\cons.obj
	  $(OUT)\source\low\cstack.obj
	  $(OUT)\source\low\dbg.obj
	  $(OUT)\source\low\dbg2.obj
	  $(OUT)\source\low\err.obj
	  $(OUT)\source\low\err2.obj
	  $(OUT)\source\low\enter.obj
	  $(OUT)\source\low\eventw.obj
	  $(OUT)\source\low\f.obj
	  $(OUT)\source\low\fd.obj
	  $(OUT)\source\low\ftp.obj
	  $(OUT)\source\low\heapmon.obj
	  $(OUT)\source\low\heap2.obj
	  $(OUT)\source\low\hist.obj
	  $(OUT)\source\low\hist2.obj
	  $(OUT)\source\low\hist_da.obj
	  $(OUT)\source\low\ipc.obj
	  $(OUT)\source\low\ipcqnx4.obj
	  $(OUT)\source\low\ipcqbaby.obj
	  $(OUT)\source\low\ipcrock.obj
	  $(OUT)\source\low\ipcdde.obj
	  $(OUT)\source\low\leave.obj
	  $(OUT)\source\low\license.obj
	  $(OUT)\source\low\liner.obj
	  $(OUT)\source\low\list.obj
	  $(OUT)\source\low\list_io.obj
	  $(OUT)\source\low\lut8.obj
	  $(OUT)\source\low\lut8_mat.obj
	  $(OUT)\source\low\msgu.obj
	  $(OUT)\source\low\msw.obj
	  $(OUT)\source\low\os_mem.obj
	  $(OUT)\source\low\os_delay.obj
	  $(OUT)\source\low\os_futil.obj
	  $(OUT)\source\low\os_heap.obj
	  $(OUT)\source\low\os_heap2.obj
	  $(OUT)\source\low\os_time.obj
	  $(OUT)\source\low\os_env.obj
	  $(OUT)\source\low\param.obj
	  $(OUT)\source\low\phys.obj
	  $(OUT)\source\low\pnm.obj
	  $(OUT)\source\low\portio.obj
	  $(OUT)\source\low\prj.obj
	  $(OUT)\source\low\qnx4.obj
	  $(OUT)\source\low\rev.obj
	  $(OUT)\source\low\say.obj
	  $(OUT)\source\low\semu.obj
	  $(OUT)\source\low\semw.obj
	  $(OUT)\source\low\serial.obj
	  $(OUT)\source\low\serialu.obj
	  $(OUT)\source\low\serialw.obj
	  $(OUT)\source\low\shmu.obj
	  $(OUT)\source\low\shmw.obj
	  $(OUT)\source\low\str.obj
	  $(OUT)\source\low\threads.obj
	  $(OUT)\source\low\thread.obj
	  $(OUT)\source\low\threadu.obj
	  $(OUT)\source\low\threadw.obj
	  $(OUT)\source\low\usage.obj
	  $(OUT)\source\low\yield.obj
	  $(OUT)\source\low\libmain.obj
	  $(OUT)\source\low\dbg_dat.obj
	  $(OUT)\source\low\conf_dat.obj
	  $(OUT)\source\low\lut8_dat.obj
	  $(OUT)\source\low\ntype.obj
	  $(OUT)\source\low\ntype_da.obj
	  $(OUT)\source\low\prj_dat.obj
<<
	
$(OUT)\dynamic\dtlow.dll \
$(OUT)\libdll\dtlow.lib: \
	  $(OUT)\source\low\dbtable.obj \
	  $(OUT)\source\low\points.obj \
	  $(OUT)\source\low\poly.obj \
	  $(OUT)\source\low\matrix3d.obj \
	  $(OUT)\source\low\matrix2d.obj \
	  $(OUT)\source\low\matrix.obj \
	  $(OUT)\source\low\assert.obj \
	  $(OUT)\source\low\cipher.obj \
	  $(OUT)\source\low\cons.obj \
	  $(OUT)\source\low\cstack.obj \
	  $(OUT)\source\low\dbg.obj \
	  $(OUT)\source\low\dbg2.obj \
	  $(OUT)\source\low\err.obj \
	  $(OUT)\source\low\err2.obj \
	  $(OUT)\source\low\enter.obj \
	  $(OUT)\source\low\eventw.obj \
	  $(OUT)\source\low\f.obj \
	  $(OUT)\source\low\fd.obj \
	  $(OUT)\source\low\ftp.obj \
	  $(OUT)\source\low\heapmon.obj \
	  $(OUT)\source\low\heap2.obj \
	  $(OUT)\source\low\hist.obj \
	  $(OUT)\source\low\hist2.obj \
	  $(OUT)\source\low\hist_da.obj \
	  $(OUT)\source\low\ipc.obj \
	  $(OUT)\source\low\ipcqnx4.obj \
	  $(OUT)\source\low\ipcqbaby.obj \
	  $(OUT)\source\low\ipcrock.obj \
	  $(OUT)\source\low\ipcdde.obj \
	  $(OUT)\source\low\leave.obj \
	  $(OUT)\source\low\license.obj \
	  $(OUT)\source\low\liner.obj \
	  $(OUT)\source\low\list.obj \
	  $(OUT)\source\low\list_io.obj \
	  $(OUT)\source\low\lut8.obj \
	  $(OUT)\source\low\lut8_mat.obj \
	  $(OUT)\source\low\msgu.obj \
	  $(OUT)\source\low\msw.obj \
	  $(OUT)\source\low\os_mem.obj \
	  $(OUT)\source\low\os_delay.obj \
	  $(OUT)\source\low\os_futil.obj \
	  $(OUT)\source\low\os_heap.obj \
	  $(OUT)\source\low\os_heap2.obj \
	  $(OUT)\source\low\os_time.obj \
	  $(OUT)\source\low\os_env.obj \
	  $(OUT)\source\low\param.obj \
	  $(OUT)\source\low\phys.obj \
	  $(OUT)\source\low\pnm.obj \
	  $(OUT)\source\low\portio.obj \
	  $(OUT)\source\low\prj.obj \
	  $(OUT)\source\low\qnx4.obj \
	  $(OUT)\source\low\rev.obj \
	  $(OUT)\source\low\say.obj \
	  $(OUT)\source\low\semu.obj \
	  $(OUT)\source\low\semw.obj \
	  $(OUT)\source\low\serial.obj \
	  $(OUT)\source\low\serialu.obj \
	  $(OUT)\source\low\serialw.obj \
	  $(OUT)\source\low\shmu.obj \
	  $(OUT)\source\low\shmw.obj \
	  $(OUT)\source\low\str.obj \
	  $(OUT)\source\low\threads.obj \
	  $(OUT)\source\low\thread.obj \
	  $(OUT)\source\low\threadu.obj \
	  $(OUT)\source\low\threadw.obj \
	  $(OUT)\source\low\usage.obj \
	  $(OUT)\source\low\yield.obj \
	  $(OUT)\source\low\libmain.obj \
	  $(OUT)\source\low\dbg_dat.obj \
	  $(OUT)\source\low\conf_dat.obj \
	  $(OUT)\source\low\lut8_dat.obj \
	  $(OUT)\source\low\ntype.obj \
	  $(OUT)\source\low\ntype_da.obj \
	  $(OUT)\source\low\prj_dat.obj \
	  $(OUT)\source\low\cdecl.obj \
	  $(OUT)\source\low\dbg_dat.obj \
	  $(OUT)\source\low\conf_dat.obj \
	  $(OUT)\source\low\lut8_dat.obj \
	  $(OUT)\source\low\ntype_da.obj \
	  $(OUT)\source\low\prj_dat.obj \
	  $(OUT)\source\low\hist_da.obj \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtlow.dll  \
	  /dll /implib:$(OUT)\libdll\dtlow.lib  \
	  $(OUT)\source\low\dbtable.obj  \
	  $(OUT)\source\low\points.obj  \
	  $(OUT)\source\low\poly.obj  \
	  $(OUT)\source\low\matrix3d.obj  \
	  $(OUT)\source\low\matrix2d.obj  \
	  $(OUT)\source\low\matrix.obj  \
	  $(OUT)\source\low\assert.obj  \
	  $(OUT)\source\low\cipher.obj  \
	  $(OUT)\source\low\cons.obj  \
	  $(OUT)\source\low\cstack.obj  \
	  $(OUT)\source\low\dbg.obj  \
	  $(OUT)\source\low\dbg2.obj  \
	  $(OUT)\source\low\err.obj  \
	  $(OUT)\source\low\err2.obj  \
	  $(OUT)\source\low\enter.obj  \
	  $(OUT)\source\low\eventw.obj  \
	  $(OUT)\source\low\f.obj  \
	  $(OUT)\source\low\fd.obj  \
	  $(OUT)\source\low\ftp.obj  \
	  $(OUT)\source\low\heapmon.obj  \
	  $(OUT)\source\low\heap2.obj  \
	  $(OUT)\source\low\hist.obj  \
	  $(OUT)\source\low\hist2.obj  \
	  $(OUT)\source\low\hist_da.obj  \
	  $(OUT)\source\low\ipc.obj  \
	  $(OUT)\source\low\ipcqnx4.obj  \
	  $(OUT)\source\low\ipcqbaby.obj  \
	  $(OUT)\source\low\ipcrock.obj  \
	  $(OUT)\source\low\ipcdde.obj  \
	  $(OUT)\source\low\leave.obj  \
	  $(OUT)\source\low\license.obj  \
	  $(OUT)\source\low\liner.obj  \
	  $(OUT)\source\low\list.obj  \
	  $(OUT)\source\low\list_io.obj  \
	  $(OUT)\source\low\lut8.obj  \
	  $(OUT)\source\low\lut8_mat.obj  \
	  $(OUT)\source\low\msgu.obj  \
	  $(OUT)\source\low\msw.obj  \
	  $(OUT)\source\low\os_mem.obj  \
	  $(OUT)\source\low\os_delay.obj  \
	  $(OUT)\source\low\os_futil.obj  \
	  $(OUT)\source\low\os_heap.obj  \
	  $(OUT)\source\low\os_heap2.obj  \
	  $(OUT)\source\low\os_time.obj  \
	  $(OUT)\source\low\os_env.obj  \
	  $(OUT)\source\low\param.obj  \
	  $(OUT)\source\low\phys.obj  \
	  $(OUT)\source\low\pnm.obj  \
	  $(OUT)\source\low\portio.obj  \
	  $(OUT)\source\low\prj.obj  \
	  $(OUT)\source\low\qnx4.obj  \
	  $(OUT)\source\low\rev.obj  \
	  $(OUT)\source\low\say.obj  \
	  $(OUT)\source\low\semu.obj  \
	  $(OUT)\source\low\semw.obj  \
	  $(OUT)\source\low\serial.obj  \
	  $(OUT)\source\low\serialu.obj  \
	  $(OUT)\source\low\serialw.obj  \
	  $(OUT)\source\low\shmu.obj  \
	  $(OUT)\source\low\shmw.obj  \
	  $(OUT)\source\low\str.obj  \
	  $(OUT)\source\low\threads.obj  \
	  $(OUT)\source\low\thread.obj  \
	  $(OUT)\source\low\threadu.obj  \
	  $(OUT)\source\low\threadw.obj  \
	  $(OUT)\source\low\usage.obj  \
	  $(OUT)\source\low\yield.obj  \
	  $(OUT)\source\low\libmain.obj  \
	  $(OUT)\source\low\dbg_dat.obj  \
	  $(OUT)\source\low\conf_dat.obj  \
	  $(OUT)\source\low\lut8_dat.obj  \
	  $(OUT)\source\low\ntype.obj  \
	  $(OUT)\source\low\ntype_da.obj  \
	  $(OUT)\source\low\prj_dat.obj  \
	  $(OUT)\libdll\dt1d.lib  \
	  $(MK_FLEXLM_LOAD)  \
     $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      \
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib

	lib /nologo /out:$(OUT)\libdll\dtlow.lib @<<!
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\source\low\cdecl.obj
	  $(OUT)\source\low\dbg_dat.obj
	  $(OUT)\source\low\conf_dat.obj
	  $(OUT)\source\low\lut8_dat.obj
	  $(OUT)\source\low\ntype_da.obj
	  $(OUT)\source\low\prj_dat.obj
	  $(OUT)\source\low\hist_da.obj
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\low\dtenv1.obj: $(SRC)\source\low\dtenv1.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dtenv1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dtenv1.c
$(OUT)\source\low\dtenv2.obj: $(SRC)\source\low\dtenv2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dtenv2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dtenv2.c
$(OUT)\source\low\dtenv3.obj: $(SRC)\source\low\dtenv3.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dtenv3.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dtenv3.c
$(OUT)\source\low\dtenv4.obj: $(SRC)\source\low\dtenv4.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dtenv4.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dtenv4.c
$(OUT)\source\low\dthist1.obj: $(SRC)\source\low\dthist1.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dthist1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dthist1.c
$(OUT)\source\low\dthist2.obj: $(SRC)\source\low\dthist2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dthist2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dthist2.c
$(OUT)\source\low\dtparam1.obj: $(SRC)\source\low\dtparam1.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dtparam1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dtparam1.c
$(OUT)\source\low\dtport1.obj: $(SRC)\source\low\dtport1.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\portio.h \
	  $(SRC)\include\dtack\portio.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dtport1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dtport1.c
$(OUT)\source\low\hist2.obj: $(SRC)\source\low\hist2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\hist2.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\hist2.c
$(OUT)\source\low\enter.obj: $(SRC)\source\low\enter.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\enter.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_NOINTER=DT_1 \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\enter.c
$(OUT)\source\low\ipcqnx4.obj: $(SRC)\source\low\ipcqnx4.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ipcqnx4.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ipcqnx4.c
$(OUT)\source\low\ipcqbaby.obj: $(SRC)\source\low\ipcqbaby.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\label.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ipcqbaby.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ipcqbaby.c
$(OUT)\source\low\ipcrock.obj: $(SRC)\source\low\ipcrock.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\label.h \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ipcrock.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_SOCKET=DT_$(MK_SOCKET_DEFINE) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  -I$(MK_SOCKET_INCLUDE) \
	  $(SRC)\source\low\ipcrock.c
$(OUT)\source\low\license.obj: $(SRC)\source\low\license.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\licens.h \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\license.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_FLEXLM=DT_$(MK_FLEXLM_DEFINE) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  -I$(MK_FLEXLM_INCLUDE) \
	  $(SRC)\source\low\license.c
$(OUT)\source\low\fd.obj: $(SRC)\source\low\fd.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\fd.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_ZLIB=DT_$(MK_ZLIB_DEFINE) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  -I$(MK_ZLIB_INCLUDE) \
	  $(SRC)\source\low\fd.c
$(OUT)\source\low\f.obj: $(SRC)\source\low\f.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\sem.h \
	  $(SRC)\include\dtack\sem.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\f.h \
	  $(SRC)\include\dtack\f.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\cipher.h \
	  $(SRC)\include\dtack\cipher.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\f.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_ZLIB=DT_$(MK_ZLIB_DEFINE) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  -I$(MK_ZLIB_INCLUDE) \
	  $(SRC)\source\low\f.c
$(OUT)\source\low\cipher.obj: $(SRC)\source\low\cipher.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\cipher.h \
	  $(SRC)\include\dtack\cipher.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\cipher.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_ZLIB=DT_$(MK_ZLIB_DEFINE) -D$(MK_MCRYPT_DEFINE) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  -I$(MK_ZLIB_INCLUDE) \
	  -I$(MK_MCRYPT_INCLUDE) \
	  $(SRC)\source\low\cipher.c
$(OUT)\source\low\prj_dat.obj: $(SRC)\source\low\prj_dat.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\prj_dat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_SOCKET=DT_$(MK_SOCKET_DEFINE) -DDTPRJ_TIFF=DT_0 -DPRJ_TERMCAP_DEFINE -DPRJ_TERMIOS_DEFINE -DPRJ_VMC_DEFINE -DDTPRJ_NOINTER=DT_1 \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\prj_dat.c
$(OUT)\source\low\cdecl.obj: $(SRC)\source\low\cdecl.c
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\cdecl.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\cdecl.c
$(OUT)\source\low\dbg_dat.obj: $(SRC)\source\low\dbg_dat.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dbg_dat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dbg_dat.c
$(OUT)\source\low\conf_dat.obj: $(SRC)\source\low\conf_dat.c \
	  $(SRC)\include\dtack\config.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\conf_dat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\conf_dat.c
$(OUT)\source\low\lut8_dat.obj: $(SRC)\source\low\lut8_dat.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\lut8_dat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\lut8_dat.c
$(OUT)\source\low\ntype_da.obj: $(SRC)\source\low\ntype_da.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ntype_da.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ntype_da.c
$(OUT)\source\low\hist_da.obj: $(SRC)\source\low\hist_da.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\hist_da.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\hist_da.c
$(OUT)\source\low\dbtable.obj: $(SRC)\source\low\dbtable.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\db.h \
	  $(SRC)\include\dtack\db.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dbtable.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dbtable.c
$(OUT)\source\low\points.obj: $(SRC)\source\low\points.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\point.h \
	  $(SRC)\include\dtack\point.p \
	  $(SRC)\include\dtack\points.h \
	  $(SRC)\include\dtack\points.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\points.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\points.c
$(OUT)\source\low\poly.obj: $(SRC)\source\low\poly.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\point.h \
	  $(SRC)\include\dtack\point.p \
	  $(SRC)\include\dtack\poly.h \
	  $(SRC)\include\dtack\poly.p \
	  $(SRC)\include\dtack\matrix.h \
	  $(SRC)\include\dtack\matrix.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\poly.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\poly.c
$(OUT)\source\low\matrix3d.obj: $(SRC)\source\low\matrix3d.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\point.h \
	  $(SRC)\include\dtack\point.p \
	  $(SRC)\include\dtack\poly.h \
	  $(SRC)\include\dtack\poly.p \
	  $(SRC)\include\dtack\matrix.h \
	  $(SRC)\include\dtack\matrix.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\matrix3d.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\matrix3d.c
$(OUT)\source\low\matrix2d.obj: $(SRC)\source\low\matrix2d.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\point.h \
	  $(SRC)\include\dtack\point.p \
	  $(SRC)\include\dtack\poly.h \
	  $(SRC)\include\dtack\poly.p \
	  $(SRC)\include\dtack\matrix.h \
	  $(SRC)\include\dtack\matrix.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\matrix2d.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\matrix2d.c
$(OUT)\source\low\matrix.obj: $(SRC)\source\low\matrix.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\point.h \
	  $(SRC)\include\dtack\point.p \
	  $(SRC)\include\dtack\poly.h \
	  $(SRC)\include\dtack\poly.p \
	  $(SRC)\include\dtack\matrix.h \
	  $(SRC)\include\dtack\matrix.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\matrix.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\matrix.c
$(OUT)\source\low\assert.obj: $(SRC)\source\low\assert.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\assert.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\assert.c
$(OUT)\source\low\cons.obj: $(SRC)\source\low\cons.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\cons.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\cons.c
$(OUT)\source\low\cstack.obj: $(SRC)\source\low\cstack.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\cstack.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\cstack.c
$(OUT)\source\low\dbg.obj: $(SRC)\source\low\dbg.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dbg.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dbg.c
$(OUT)\source\low\dbg2.obj: $(SRC)\source\low\dbg2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\dbg2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\dbg2.c
$(OUT)\source\low\err.obj: $(SRC)\source\low\err.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\err.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\err.c
$(OUT)\source\low\err2.obj: $(SRC)\source\low\err2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\err2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\err2.c
$(OUT)\source\low\eventw.obj: $(SRC)\source\low\eventw.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\event.h \
	  $(SRC)\include\dtack\event.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\eventw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\eventw.c
$(OUT)\source\low\ftp.obj: $(SRC)\source\low\ftp.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\ftp.h \
	  $(SRC)\include\dtack\ftp.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ftp.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ftp.c
$(OUT)\source\low\heapmon.obj: $(SRC)\source\low\heapmon.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\heap.h \
	  $(SRC)\include\dtack\heap.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\heapmon.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\heapmon.c
$(OUT)\source\low\heap2.obj: $(SRC)\source\low\heap2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\heap.h \
	  $(SRC)\include\dtack\heap.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\heap2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\heap2.c
$(OUT)\source\low\hist.obj: $(SRC)\source\low\hist.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\hist.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\hist.c
$(OUT)\source\low\ipc.obj: $(SRC)\source\low\ipc.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\label.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ipc.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ipc.c
$(OUT)\source\low\ipcdde.obj: $(SRC)\source\low\ipcdde.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ipcdde.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ipcdde.c
$(OUT)\source\low\leave.obj: $(SRC)\source\low\leave.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\leave.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\leave.c
$(OUT)\source\low\liner.obj: $(SRC)\source\low\liner.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\liner.h \
	  $(SRC)\include\dtack\liner.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\liner.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\liner.c
$(OUT)\source\low\list.obj: $(SRC)\source\low\list.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\list.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\list.c
$(OUT)\source\low\list_io.obj: $(SRC)\source\low\list_io.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\list_io.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\list_io.c
$(OUT)\source\low\lut8.obj: $(SRC)\source\low\lut8.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\lut8.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\lut8.c
$(OUT)\source\low\lut8_mat.obj: $(SRC)\source\low\lut8_mat.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\lut8.h \
	  $(SRC)\include\dtack\lut8.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\lut8_mat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\lut8_mat.c
$(OUT)\source\low\msgu.obj: $(SRC)\source\low\msgu.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\msg.h \
	  $(SRC)\include\dtack\msg.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\msgu.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\msgu.c
$(OUT)\source\low\msw.obj: $(SRC)\source\low\msw.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\msw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\msw.c
$(OUT)\source\low\os_mem.obj: $(SRC)\source\low\os_mem.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\heap.h \
	  $(SRC)\include\dtack\heap.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_mem.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_mem.c
$(OUT)\source\low\os_delay.obj: $(SRC)\source\low\os_delay.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_delay.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_delay.c
$(OUT)\source\low\os_futil.obj: $(SRC)\source\low\os_futil.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_futil.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_futil.c
$(OUT)\source\low\os_heap.obj: $(SRC)\source\low\os_heap.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_heap.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_heap.c
$(OUT)\source\low\os_heap2.obj: $(SRC)\source\low\os_heap2.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_heap2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_heap2.c
$(OUT)\source\low\os_time.obj: $(SRC)\source\low\os_time.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_time.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_time.c
$(OUT)\source\low\os_env.obj: $(SRC)\source\low\os_env.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\os_env.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\os_env.c
$(OUT)\source\low\param.obj: $(SRC)\source\low\param.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\param.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\param.c
$(OUT)\source\low\phys.obj: $(SRC)\source\low\phys.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\phys.h \
	  $(SRC)\include\dtack\phys.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\phys.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\phys.c
$(OUT)\source\low\pnm.obj: $(SRC)\source\low\pnm.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\pnm.h \
	  $(SRC)\include\dtack\pnm.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\pnm.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\pnm.c
$(OUT)\source\low\portio.obj: $(SRC)\source\low\portio.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p \
	  $(SRC)\include\dtack\portio.h \
	  $(SRC)\include\dtack\portio.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\portio.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\portio.c
$(OUT)\source\low\prj.obj: $(SRC)\source\low\prj.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\prj.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\prj.c
$(OUT)\source\low\qnx4.obj: $(SRC)\source\low\qnx4.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\qnx4.h \
	  $(SRC)\include\dtack\qnx4.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\qnx4.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\qnx4.c
$(OUT)\source\low\rev.obj: $(SRC)\source\low\rev.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\rev.h \
	  $(SRC)\include\dtack\rev.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\rev.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\rev.c
$(OUT)\source\low\say.obj: $(SRC)\source\low\say.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\say.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\say.c
$(OUT)\source\low\semu.obj: $(SRC)\source\low\semu.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\sem.h \
	  $(SRC)\include\dtack\sem.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\semu.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\semu.c
$(OUT)\source\low\semw.obj: $(SRC)\source\low\semw.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\sem.h \
	  $(SRC)\include\dtack\sem.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\semw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\semw.c
$(OUT)\source\low\serial.obj: $(SRC)\source\low\serial.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\serial.h \
	  $(SRC)\include\dtack\serial.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\serial.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\serial.c
$(OUT)\source\low\serialu.obj: $(SRC)\source\low\serialu.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\serial.h \
	  $(SRC)\include\dtack\serial.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\serialu.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\serialu.c
$(OUT)\source\low\serialw.obj: $(SRC)\source\low\serialw.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\serial.h \
	  $(SRC)\include\dtack\serial.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\serialw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\serialw.c
$(OUT)\source\low\shmu.obj: $(SRC)\source\low\shmu.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\shm.h \
	  $(SRC)\include\dtack\shm.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\shmu.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\shmu.c
$(OUT)\source\low\shmw.obj: $(SRC)\source\low\shmw.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p \
	  $(SRC)\include\dtack\shm.h \
	  $(SRC)\include\dtack\shm.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\shmw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\shmw.c
$(OUT)\source\low\str.obj: $(SRC)\source\low\str.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\str.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\str.c
$(OUT)\source\low\threads.obj: $(SRC)\source\low\threads.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\thread.h \
	  $(SRC)\include\dtack\thread.p \
	  $(SRC)\include\dtack\threads.h \
	  $(SRC)\include\dtack\threads.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\threads.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\threads.c
$(OUT)\source\low\thread.obj: $(SRC)\source\low\thread.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\thread.h \
	  $(SRC)\include\dtack\thread.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\thread.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\thread.c
$(OUT)\source\low\threadu.obj: $(SRC)\source\low\threadu.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\thread.h \
	  $(SRC)\include\dtack\thread.p \
	  $(SRC)\include\dtack\errno.h
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\threadu.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\threadu.c
$(OUT)\source\low\threadw.obj: $(SRC)\source\low\threadw.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\ipc.h \
	  $(SRC)\include\dtack\ipc.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\os.h \
	  $(SRC)\include\dtack\os.p \
	  $(SRC)\include\dtack\thread.h \
	  $(SRC)\include\dtack\thread.p \
	  $(SRC)\include\dtack\errno.h \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\threadw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\threadw.c
$(OUT)\source\low\usage.obj: $(SRC)\source\low\usage.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\usage.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\usage.c
$(OUT)\source\low\yield.obj: $(SRC)\source\low\yield.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\cons.h \
	  $(SRC)\include\dtack\cons.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\yield.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\yield.c
$(OUT)\source\low\libmain.obj: $(SRC)\source\low\libmain.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\msw.h \
	  $(SRC)\include\dtack\msw.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\libmain.c
$(OUT)\source\low\ntype.obj: $(SRC)\source\low\ntype.c \
	  $(SRC)\include\dtack\base.h \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p \
	  $(SRC)\include\dtack\config2.h \
	  $(SRC)\include\dtack\param.h \
	  $(SRC)\include\dtack\param.p \
	  $(SRC)\include\dtack\cstack.h \
	  $(SRC)\include\dtack\cstack.p \
	  $(SRC)\include\dtack\hist.h \
	  $(SRC)\include\dtack\hist.p \
	  $(SRC)\include\dtack\fd.h \
	  $(SRC)\include\dtack\fd.p \
	  $(SRC)\include\dtack\run.h \
	  $(SRC)\include\dtack\run.p \
	  $(SRC)\include\dtack\image.h \
	  $(SRC)\include\dtack\image.p \
	  $(SRC)\include\dtack\base.p \
	  $(SRC)\include\dtack\str.h \
	  $(SRC)\include\dtack\str.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\low\ntype.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\low \
	  -I$(SRC)\include \
	  $(SRC)\source\low\ntype.c


