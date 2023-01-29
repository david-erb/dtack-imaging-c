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
	  $(OUT)\static\dtthigh1.exe \
	  $(OUT)\static\dtthigh2.exe \
	  $(OUT)\static\dttarith.exe \
	  $(OUT)\static\dttadj.exe \
	  $(OUT)\static\dttconv.exe \
	  $(OUT)\static\dttmorph.exe \
	  $(OUT)\static\dttgeom.exe \
	  $(OUT)\static\dttcast.exe \
	  $(OUT)\static\dttcmp.exe \
	  $(OUT)\static\dttdb.exe

shexes: \
	  $(OUT)\dynamic\dtthigh1.exe \
	  $(OUT)\dynamic\dtthigh2.exe \
	  $(OUT)\dynamic\dttarith.exe \
	  $(OUT)\dynamic\dttadj.exe \
	  $(OUT)\dynamic\dttconv.exe \
	  $(OUT)\dynamic\dttmorph.exe \
	  $(OUT)\dynamic\dttgeom.exe \
	  $(OUT)\dynamic\dttcast.exe \
	  $(OUT)\dynamic\dttcmp.exe \
	  $(OUT)\dynamic\dttdb.exe

scripts: 

libs: \
	  $(OUT)\lib\dtthix1.lib \
	  $(OUT)\lib\dtthix2.lib

shlibs: \
	  $(OUT)\dynamic\dtthix1.dll \
	  $(OUT)\dynamic\dtthix2.dll

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
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP
	cvpack /STRIP



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\dynamic\dtthigh1.exe
	-del $(OUT)\dynamic\dtthigh2.exe
	-del $(OUT)\dynamic\dttarith.exe
	-del $(OUT)\dynamic\dttadj.exe
	-del $(OUT)\dynamic\dttconv.exe
	-del $(OUT)\dynamic\dttmorph.exe
	-del $(OUT)\dynamic\dttgeom.exe
	-del $(OUT)\dynamic\dttcast.exe
	-del $(OUT)\dynamic\dttcmp.exe
	-del $(OUT)\dynamic\dttdb.exe
	-del $(OUT)\static\dtthigh1.exe
	-del $(OUT)\static\dtthigh2.exe
	-del $(OUT)\static\dttarith.exe
	-del $(OUT)\static\dttadj.exe
	-del $(OUT)\static\dttconv.exe
	-del $(OUT)\static\dttmorph.exe
	-del $(OUT)\static\dttgeom.exe
	-del $(OUT)\static\dttcast.exe
	-del $(OUT)\static\dttcmp.exe
	-del $(OUT)\static\dttdb.exe
	-del $(OUT)\lib\dtthix1.lib
	-del $(OUT)\lib\dtthix2.lib
	-del $(OUT)\dynamic\dtthix1.dll
	-del $(OUT)\dynamic\dtthix2.dll
	-del $(OUT)\test\high\*.obj
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



$(OUT)\static\dtthigh1.exe: \
	  $(OUT)\test\high\high1.obj \
	  $(OUT)\lib\dtthix1.lib \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtthigh1.exe @<<
	  /MAP:$(OUT)\static\dtthigh1.map
	  /PDB:$(OUT)\static\dtthigh1.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\high1.obj
	$(OUT)\lib\dtthix1.lib
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)       
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtthigh1.res
<<
$(OUT)\dynamic\dtthigh1.exe: \
	  $(OUT)\test\high\high1.obj \
	  $(OUT)\libdll\dtthix1.lib \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtthigh1.exe @<<
	  /MAP:$(OUT)\dynamic\dtthigh1.map 
	  /PDB:$(OUT)\dynamic\dtthigh1.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\high1.obj
	  $(OUT)\libdll\dtthix1.lib
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)      
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtthigh1.lib
	del $(OUT)\dynamic\dtthigh1.exp



$(OUT)\static\dtthigh2.exe: \
	  $(OUT)\test\high\high2.obj \
	  $(OUT)\lib\dtthix2.lib \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dtthigh2.exe @<<
	  /MAP:$(OUT)\static\dtthigh2.map
	  /PDB:$(OUT)\static\dtthigh2.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\high2.obj
	$(OUT)\lib\dtthix2.lib
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dtthigh2.res
<<
$(OUT)\dynamic\dtthigh2.exe: \
	  $(OUT)\test\high\high2.obj \
	  $(OUT)\libdll\dtthix2.lib \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dtthigh2.exe @<<
	  /MAP:$(OUT)\dynamic\dtthigh2.map 
	  /PDB:$(OUT)\dynamic\dtthigh2.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\high2.obj
	  $(OUT)\libdll\dtthix2.lib
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dtthigh2.lib
	del $(OUT)\dynamic\dtthigh2.exp



$(OUT)\static\dttarith.exe: \
	  $(OUT)\test\high\dttarith.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttarith.exe @<<
	  /MAP:$(OUT)\static\dttarith.map
	  /PDB:$(OUT)\static\dttarith.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttarith.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttarith.res
<<
$(OUT)\dynamic\dttarith.exe: \
	  $(OUT)\test\high\dttarith.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttarith.exe @<<
	  /MAP:$(OUT)\dynamic\dttarith.map 
	  /PDB:$(OUT)\dynamic\dttarith.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttarith.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttarith.lib
	del $(OUT)\dynamic\dttarith.exp



$(OUT)\static\dttadj.exe: \
	  $(OUT)\test\high\dttadj.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttadj.exe @<<
	  /MAP:$(OUT)\static\dttadj.map
	  /PDB:$(OUT)\static\dttadj.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttadj.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttadj.res
<<
$(OUT)\dynamic\dttadj.exe: \
	  $(OUT)\test\high\dttadj.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttadj.exe @<<
	  /MAP:$(OUT)\dynamic\dttadj.map 
	  /PDB:$(OUT)\dynamic\dttadj.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttadj.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttadj.lib
	del $(OUT)\dynamic\dttadj.exp



$(OUT)\static\dttconv.exe: \
	  $(OUT)\test\high\dttconv.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttconv.exe @<<
	  /MAP:$(OUT)\static\dttconv.map
	  /PDB:$(OUT)\static\dttconv.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttconv.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttconv.res
<<
$(OUT)\dynamic\dttconv.exe: \
	  $(OUT)\test\high\dttconv.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttconv.exe @<<
	  /MAP:$(OUT)\dynamic\dttconv.map 
	  /PDB:$(OUT)\dynamic\dttconv.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttconv.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttconv.lib
	del $(OUT)\dynamic\dttconv.exp



$(OUT)\static\dttmorph.exe: \
	  $(OUT)\test\high\dttmorph.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttmorph.exe @<<
	  /MAP:$(OUT)\static\dttmorph.map
	  /PDB:$(OUT)\static\dttmorph.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttmorph.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttmorph.res
<<
$(OUT)\dynamic\dttmorph.exe: \
	  $(OUT)\test\high\dttmorph.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttmorph.exe @<<
	  /MAP:$(OUT)\dynamic\dttmorph.map 
	  /PDB:$(OUT)\dynamic\dttmorph.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttmorph.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttmorph.lib
	del $(OUT)\dynamic\dttmorph.exp



$(OUT)\static\dttgeom.exe: \
	  $(OUT)\test\high\dttgeom.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttgeom.exe @<<
	  /MAP:$(OUT)\static\dttgeom.map
	  /PDB:$(OUT)\static\dttgeom.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttgeom.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttgeom.res
<<
$(OUT)\dynamic\dttgeom.exe: \
	  $(OUT)\test\high\dttgeom.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttgeom.exe @<<
	  /MAP:$(OUT)\dynamic\dttgeom.map 
	  /PDB:$(OUT)\dynamic\dttgeom.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttgeom.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttgeom.lib
	del $(OUT)\dynamic\dttgeom.exp



$(OUT)\static\dttcast.exe: \
	  $(OUT)\test\high\dttcast.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttcast.exe @<<
	  /MAP:$(OUT)\static\dttcast.map
	  /PDB:$(OUT)\static\dttcast.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttcast.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttcast.res
<<
$(OUT)\dynamic\dttcast.exe: \
	  $(OUT)\test\high\dttcast.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttcast.exe @<<
	  /MAP:$(OUT)\dynamic\dttcast.map 
	  /PDB:$(OUT)\dynamic\dttcast.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttcast.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttcast.lib
	del $(OUT)\dynamic\dttcast.exp



$(OUT)\static\dttcmp.exe: \
	  $(OUT)\test\high\dttcmp.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttcmp.exe @<<
	  /MAP:$(OUT)\static\dttcmp.map
	  /PDB:$(OUT)\static\dttcmp.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttcmp.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttcmp.res
<<
$(OUT)\dynamic\dttcmp.exe: \
	  $(OUT)\test\high\dttcmp.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttcmp.exe @<<
	  /MAP:$(OUT)\dynamic\dttcmp.map 
	  /PDB:$(OUT)\dynamic\dttcmp.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttcmp.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttcmp.lib
	del $(OUT)\dynamic\dttcmp.exp



$(OUT)\static\dttdb.exe: \
	  $(OUT)\test\high\dttdb.obj \
	  $(OUT)\lib\dttaa.lib \
	  $(OUT)\lib\dtxchg.lib \
	  $(OUT)\lib\dtcolor.lib \
	  $(OUT)\lib\dthigh.lib \
	  $(OUT)\lib\dtwinc.lib \
	  $(OUT)\lib\dtbigmem.lib \
	  $(OUT)\lib\dtlow.lib \
	  $(OUT)\lib\dt1d.lib
	link /nologo /OUT:$(OUT)\static\dttdb.exe @<<
	  /MAP:$(OUT)\static\dttdb.map
	  /PDB:$(OUT)\static\dttdb.pdb
 /DEBUG
          /NODEFAULTLIB:msvcrtd
	$(OUT)\test\high\dttdb.obj
	$(OUT)\lib\dttaa.lib
	$(OUT)\lib\dtxchg.lib
	$(OUT)\lib\dtcolor.lib
	$(OUT)\lib\dthigh.lib
	$(OUT)\lib\dtwinc.lib
	$(OUT)\lib\dtbigmem.lib
	$(OUT)\lib\dtlow.lib
	$(OUT)\lib\dt1d.lib
	$(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD) 
	advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
	$(SRC)\config\win32\dttdb.res
<<
$(OUT)\dynamic\dttdb.exe: \
	  $(OUT)\test\high\dttdb.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dtxchg.lib \
	  $(OUT)\libdll\dtcolor.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtwinc.lib \
	  $(OUT)\libdll\dtbigmem.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /OUT:$(OUT)\dynamic\dttdb.exe @<<
	  /MAP:$(OUT)\dynamic\dttdb.map 
	  /PDB:$(OUT)\dynamic\dttdb.pdb 

	  /DEBUG
	  
          /NODEFAULTLIB:msvcrtd
	  $(OUT)\test\high\dttdb.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dtxchg.lib
	  $(OUT)\libdll\dtcolor.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtwinc.lib
	  $(OUT)\libdll\dtbigmem.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib  $(MK_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)          $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	del $(OUT)\dynamic\dttdb.lib
	del $(OUT)\dynamic\dttdb.exp





#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dtthix1.lib: \
	  $(OUT)\test\high\dtthist.obj \
	  $(OUT)\test\high\dttpgm.obj \
	  $(OUT)\test\high\dttplot.obj \
	  $(OUT)\test\high\dttrun.obj \
	  $(OUT)\test\high\dttlog.obj \
	  $(OUT)\test\high\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtthix1.lib @<<
	  $(OUT)\test\high\dtthist.obj
	  $(OUT)\test\high\dttpgm.obj
	  $(OUT)\test\high\dttplot.obj
	  $(OUT)\test\high\dttrun.obj
	  $(OUT)\test\high\dttlog.obj
	  $(OUT)\test\high\libmain.obj
<<
	
$(OUT)\dynamic\dtthix1.dll \
$(OUT)\libdll\dtthix1.lib: \
	  $(OUT)\test\high\dtthist.obj \
	  $(OUT)\test\high\dttpgm.obj \
	  $(OUT)\test\high\dttplot.obj \
	  $(OUT)\test\high\dttrun.obj \
	  $(OUT)\test\high\dttlog.obj \
	  $(OUT)\test\high\libmain.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtthix1.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtthix1.lib
	  $(OUT)\test\high\dtthist.obj
	  $(OUT)\test\high\dttpgm.obj
	  $(OUT)\test\high\dttplot.obj
	  $(OUT)\test\high\dttrun.obj
	  $(OUT)\test\high\dttlog.obj
	  $(OUT)\test\high\libmain.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
$(OUT)\lib\dtthix2.lib: \
	  $(OUT)\test\high\dttresam.obj \
	  $(OUT)\test\high\dttcooc.obj \
	  $(OUT)\test\high\dttjoint.obj \
	  $(OUT)\test\high\dttstat.obj \
	  $(OUT)\test\high\dttmatrix.obj \
	  $(OUT)\test\high\libmain.obj
	lib /nologo /out:$(OUT)\lib\dtthix2.lib @<<
	  $(OUT)\test\high\dttresam.obj
	  $(OUT)\test\high\dttcooc.obj
	  $(OUT)\test\high\dttjoint.obj
	  $(OUT)\test\high\dttstat.obj
	  $(OUT)\test\high\dttmatrix.obj
	  $(OUT)\test\high\libmain.obj
<<
	
$(OUT)\dynamic\dtthix2.dll \
$(OUT)\libdll\dtthix2.lib: \
	  $(OUT)\test\high\dttresam.obj \
	  $(OUT)\test\high\dttcooc.obj \
	  $(OUT)\test\high\dttjoint.obj \
	  $(OUT)\test\high\dttstat.obj \
	  $(OUT)\test\high\dttmatrix.obj \
	  $(OUT)\test\high\libmain.obj \
	  $(OUT)\libdll\dttaa.lib \
	  $(OUT)\libdll\dthigh.lib \
	  $(OUT)\libdll\dtlow.lib \
	  $(OUT)\libdll\dt1d.lib
	link /nologo /out:$(OUT)\dynamic\dtthix2.dll @<<!
	  /dll /implib:$(OUT)\libdll\dtthix2.lib
	  $(OUT)\test\high\dttresam.obj
	  $(OUT)\test\high\dttcooc.obj
	  $(OUT)\test\high\dttjoint.obj
	  $(OUT)\test\high\dttstat.obj
	  $(OUT)\test\high\dttmatrix.obj
	  $(OUT)\test\high\libmain.obj
	  $(OUT)\libdll\dttaa.lib
	  $(OUT)\libdll\dthigh.lib
	  $(OUT)\libdll\dtlow.lib
	  $(OUT)\libdll\dt1d.lib
	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\test\high\dtthist.obj: $(SRC)\test\high\dtthist.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dtthist.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dtthist.c
$(OUT)\test\high\dttpgm.obj: $(SRC)\test\high\dttpgm.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\pnm.h \
	  $(SRC)\include\dtack\pnm.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttpgm.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttpgm.c
$(OUT)\test\high\dttplot.obj: $(SRC)\test\high\dttplot.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttplot.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttplot.c
$(OUT)\test\high\dttrun.obj: $(SRC)\test\high\dttrun.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\runf.h \
	  $(SRC)\include\dtack\runf.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttrun.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttrun.c
$(OUT)\test\high\dttlog.obj: $(SRC)\test\high\dttlog.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttlog.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttlog.c
$(OUT)\test\high\libmain.obj: $(SRC)\test\high\libmain.c \
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
	   /Fo$(OUT)\test\high\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\libmain.c
$(OUT)\test\high\dttresam.obj: $(SRC)\test\high\dttresam.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttresam.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttresam.c
$(OUT)\test\high\dttcooc.obj: $(SRC)\test\high\dttcooc.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttcooc.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttcooc.c
$(OUT)\test\high\dttjoint.obj: $(SRC)\test\high\dttjoint.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttjoint.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttjoint.c
$(OUT)\test\high\dttstat.obj: $(SRC)\test\high\dttstat.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttstat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttstat.c
$(OUT)\test\high\dttmatrix.obj: $(SRC)\test\high\dttmatrix.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\point.h \
	  $(SRC)\include\dtack\point.p \
	  $(SRC)\include\dtack\points.h \
	  $(SRC)\include\dtack\points.p \
	  $(SRC)\include\dtack\poly.h \
	  $(SRC)\include\dtack\poly.p \
	  $(SRC)\include\dtack\matrix.h \
	  $(SRC)\include\dtack\matrix.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttmatrix.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttmatrix.c
$(OUT)\test\high\high1.obj: $(SRC)\test\high\high1.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\high1.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\high1.c
$(OUT)\test\high\high2.obj: $(SRC)\test\high\high2.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\high2.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\high2.c
$(OUT)\test\high\dttarith.obj: $(SRC)\test\high\dttarith.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttarith.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttarith.c
$(OUT)\test\high\dttadj.obj: $(SRC)\test\high\dttadj.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\include\dtack\stack.h \
	  $(SRC)\include\dtack\stack.p \
	  $(SRC)\include\dtack\prj.h \
	  $(SRC)\include\dtack\prj.p \
	  $(SRC)\include\dtack\runf.h \
	  $(SRC)\include\dtack\runf.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttadj.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttadj.c
$(OUT)\test\high\dttconv.obj: $(SRC)\test\high\dttconv.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttconv.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttconv.c
$(OUT)\test\high\dttmorph.obj: $(SRC)\test\high\dttmorph.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttmorph.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttmorph.c
$(OUT)\test\high\dttgeom.obj: $(SRC)\test\high\dttgeom.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttgeom.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttgeom.c
$(OUT)\test\high\dttcast.obj: $(SRC)\test\high\dttcast.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p \
	  $(SRC)\test\high\dttcast.use
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttcast.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttcast.c
$(OUT)\test\high\dttcmp.obj: $(SRC)\test\high\dttcmp.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\mem.h \
	  $(SRC)\include\dtack\mem.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttcmp.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttcmp.c
$(OUT)\test\high\dttdb.obj: $(SRC)\test\high\dttdb.c \
	  $(SRC)\include\dtack\t.h \
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
	  $(SRC)\include\dtack\str.p \
	  $(SRC)\include\dtack\main.h \
	  $(SRC)\include\dtack\yield.h \
	  $(SRC)\include\dtack\t.p \
	  $(SRC)\include\dtack\list.h \
	  $(SRC)\include\dtack\list.p \
	  $(SRC)\include\dtack\db.h \
	  $(SRC)\include\dtack\db.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\test\high\dttdb.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\test\high \
	  -I$(SRC)\include \
	  $(SRC)\test\high\dttdb.c


