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
statics: libs 

shared: shareds
shareds: shlibs 

allsubs:

exes: 

shexes: 

scripts: 

libs: \
	  $(OUT)\lib\dt1d.lib

shlibs: \
	  $(OUT)\dynamic\dt1d.dll

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	-del $(OUT)\lib\dt1d.lib
	-del $(OUT)\dynamic\dt1d.dll
	-del $(OUT)\source\1d\*.obj
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




#-------------------------------------------------------------------------
# libs



$(OUT)\lib\dt1d.lib: \
	  $(OUT)\source\1d\1d_fft.obj \
	  $(OUT)\source\1d\1d_fftw.obj \
	  $(OUT)\source\1d\1d_thres.obj \
	  $(OUT)\source\1d\1d.obj \
	  $(OUT)\source\1d\1d_abs.obj \
	  $(OUT)\source\1d\1d_arith.obj \
	  $(OUT)\source\1d\1d_noise.obj \
	  $(OUT)\source\1d\1d_set.obj \
	  $(OUT)\source\1d\1d_sum.obj \
	  $(OUT)\source\1d\1d_sumsq.obj \
	  $(OUT)\source\1d\1d_sumsd.obj \
	  $(OUT)\source\1d\1d_sumss.obj \
	  $(OUT)\source\1d\1d_cumf.obj \
	  $(OUT)\source\1d\1d_cumr.obj \
	  $(OUT)\source\1d\1d_logic.obj \
	  $(OUT)\source\1d\1d_look.obj \
	  $(OUT)\source\1d\1d_cmp.obj \
	  $(OUT)\source\1d\1d_diff.obj \
	  $(OUT)\source\1d\1d_range.obj \
	  $(OUT)\source\1d\1d_ran.obj \
	  $(OUT)\source\1d\1d_sort1.obj \
	  $(OUT)\source\1d\1d_sort5.obj \
	  $(OUT)\source\1d\1d_stats.obj \
	  $(OUT)\source\1d\1d_cast.obj \
	  $(OUT)\source\1d\1d_conv.obj \
	  $(OUT)\source\1d\1d_clip.obj \
	  $(OUT)\source\1d\1d_vari.obj \
	  $(OUT)\source\1d\1d_dat.obj \
	  $(OUT)\source\1d\1d_ntype.obj \
	  $(OUT)\source\1d\libmain.obj
	lib /nologo /out:$(OUT)\lib\dt1d.lib @<<
	  $(OUT)\source\1d\1d_fft.obj
	  $(OUT)\source\1d\1d_fftw.obj
	  $(OUT)\source\1d\1d_thres.obj
	  $(OUT)\source\1d\1d.obj
	  $(OUT)\source\1d\1d_abs.obj
	  $(OUT)\source\1d\1d_arith.obj
	  $(OUT)\source\1d\1d_noise.obj
	  $(OUT)\source\1d\1d_set.obj
	  $(OUT)\source\1d\1d_sum.obj
	  $(OUT)\source\1d\1d_sumsq.obj
	  $(OUT)\source\1d\1d_sumsd.obj
	  $(OUT)\source\1d\1d_sumss.obj
	  $(OUT)\source\1d\1d_cumf.obj
	  $(OUT)\source\1d\1d_cumr.obj
	  $(OUT)\source\1d\1d_logic.obj
	  $(OUT)\source\1d\1d_look.obj
	  $(OUT)\source\1d\1d_cmp.obj
	  $(OUT)\source\1d\1d_diff.obj
	  $(OUT)\source\1d\1d_range.obj
	  $(OUT)\source\1d\1d_ran.obj
	  $(OUT)\source\1d\1d_sort1.obj
	  $(OUT)\source\1d\1d_sort5.obj
	  $(OUT)\source\1d\1d_stats.obj
	  $(OUT)\source\1d\1d_cast.obj
	  $(OUT)\source\1d\1d_conv.obj
	  $(OUT)\source\1d\1d_clip.obj
	  $(OUT)\source\1d\1d_vari.obj
	  $(OUT)\source\1d\1d_dat.obj
	  $(OUT)\source\1d\1d_ntype.obj
	  $(OUT)\source\1d\libmain.obj
<<
	
$(OUT)\dynamic\dt1d.dll \
$(OUT)\libdll\dt1d.lib: \
	  $(OUT)\source\1d\1d_fft.obj \
	  $(OUT)\source\1d\1d_fftw.obj \
	  $(OUT)\source\1d\1d_thres.obj \
	  $(OUT)\source\1d\1d.obj \
	  $(OUT)\source\1d\1d_abs.obj \
	  $(OUT)\source\1d\1d_arith.obj \
	  $(OUT)\source\1d\1d_noise.obj \
	  $(OUT)\source\1d\1d_set.obj \
	  $(OUT)\source\1d\1d_sum.obj \
	  $(OUT)\source\1d\1d_sumsq.obj \
	  $(OUT)\source\1d\1d_sumsd.obj \
	  $(OUT)\source\1d\1d_sumss.obj \
	  $(OUT)\source\1d\1d_cumf.obj \
	  $(OUT)\source\1d\1d_cumr.obj \
	  $(OUT)\source\1d\1d_logic.obj \
	  $(OUT)\source\1d\1d_look.obj \
	  $(OUT)\source\1d\1d_cmp.obj \
	  $(OUT)\source\1d\1d_diff.obj \
	  $(OUT)\source\1d\1d_range.obj \
	  $(OUT)\source\1d\1d_ran.obj \
	  $(OUT)\source\1d\1d_sort1.obj \
	  $(OUT)\source\1d\1d_sort5.obj \
	  $(OUT)\source\1d\1d_stats.obj \
	  $(OUT)\source\1d\1d_cast.obj \
	  $(OUT)\source\1d\1d_conv.obj \
	  $(OUT)\source\1d\1d_clip.obj \
	  $(OUT)\source\1d\1d_vari.obj \
	  $(OUT)\source\1d\1d_dat.obj \
	  $(OUT)\source\1d\1d_ntype.obj \
	  $(OUT)\source\1d\libmain.obj \
	  $(OUT)\source\1d\1d_dat.obj \
	  $(OUT)\source\1d\1d_ntype.obj
	link /nologo /out:$(OUT)\dynamic\dt1d.dll @<<!
	  /dll /implib:$(OUT)\libdll\dt1d.lib
	  $(OUT)\source\1d\1d_fft.obj
	  $(OUT)\source\1d\1d_fftw.obj
	  $(OUT)\source\1d\1d_thres.obj
	  $(OUT)\source\1d\1d.obj
	  $(OUT)\source\1d\1d_abs.obj
	  $(OUT)\source\1d\1d_arith.obj
	  $(OUT)\source\1d\1d_noise.obj
	  $(OUT)\source\1d\1d_set.obj
	  $(OUT)\source\1d\1d_sum.obj
	  $(OUT)\source\1d\1d_sumsq.obj
	  $(OUT)\source\1d\1d_sumsd.obj
	  $(OUT)\source\1d\1d_sumss.obj
	  $(OUT)\source\1d\1d_cumf.obj
	  $(OUT)\source\1d\1d_cumr.obj
	  $(OUT)\source\1d\1d_logic.obj
	  $(OUT)\source\1d\1d_look.obj
	  $(OUT)\source\1d\1d_cmp.obj
	  $(OUT)\source\1d\1d_diff.obj
	  $(OUT)\source\1d\1d_range.obj
	  $(OUT)\source\1d\1d_ran.obj
	  $(OUT)\source\1d\1d_sort1.obj
	  $(OUT)\source\1d\1d_sort5.obj
	  $(OUT)\source\1d\1d_stats.obj
	  $(OUT)\source\1d\1d_cast.obj
	  $(OUT)\source\1d\1d_conv.obj
	  $(OUT)\source\1d\1d_clip.obj
	  $(OUT)\source\1d\1d_vari.obj
	  $(OUT)\source\1d\1d_dat.obj
	  $(OUT)\source\1d\1d_ntype.obj
	  $(OUT)\source\1d\libmain.obj

	  advapi32.lib shell32.lib vfw32.lib winmm.lib comdlg32.lib gdi32.lib kernel32.lib user32.lib
<<
	lib /nologo /out:$(OUT)\libdll\dt1d.lib @<<!
	  $(OUT)\libdll\dt1d.lib
	  $(OUT)\source\1d\1d_dat.obj
	  $(OUT)\source\1d\1d_ntype.obj
<<

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)\source\1d\1d_fftw.obj: $(SRC)\source\1d\1d_fftw.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_fftw.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_FFTW_DEFINE) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  -I$(MK_FFTW_INCLUDE) \
	  $(SRC)\source\1d\1d_fftw.c
$(OUT)\source\1d\1d_fft.obj: $(SRC)\source\1d\1d_fft.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_fft.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_FFTW_DEFINE) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_fft.c
$(OUT)\source\1d\1d_thres.obj: $(SRC)\source\1d\1d_thres.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_thres.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_thres.c
$(OUT)\source\1d\1d_arith.obj: $(SRC)\source\1d\1d_arith.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_arith.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_arith.c
$(OUT)\source\1d\1d_logic.obj: $(SRC)\source\1d\1d_logic.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_logic.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_logic.c
$(OUT)\source\1d\1d_cast.obj: $(SRC)\source\1d\1d_cast.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_cast.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_cast.c
$(OUT)\source\1d\1d_clip.obj: $(SRC)\source\1d\1d_clip.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_clip.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_clip.c
$(OUT)\source\1d\1d_cmp.obj: $(SRC)\source\1d\1d_cmp.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_cmp.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_cmp.c
$(OUT)\source\1d\1d_conv.obj: $(SRC)\source\1d\1d_conv.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_conv.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_conv.c
$(OUT)\source\1d\1d_cumf.obj: $(SRC)\source\1d\1d_cumf.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_cumf.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_cumf.c
$(OUT)\source\1d\1d_cumr.obj: $(SRC)\source\1d\1d_cumr.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_cumr.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_cumr.c
$(OUT)\source\1d\1d_diff.obj: $(SRC)\source\1d\1d_diff.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_diff.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_diff.c
$(OUT)\source\1d\1d_look.obj: $(SRC)\source\1d\1d_look.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_look.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_look.c
$(OUT)\source\1d\1d_noise.obj: $(SRC)\source\1d\1d_noise.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_noise.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_noise.c
$(OUT)\source\1d\1d_ran.obj: $(SRC)\source\1d\1d_ran.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_ran.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_ran.c
$(OUT)\source\1d\1d_range.obj: $(SRC)\source\1d\1d_range.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_range.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_range.c
$(OUT)\source\1d\1d_set.obj: $(SRC)\source\1d\1d_set.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_set.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_set.c
$(OUT)\source\1d\1d_sort1.obj: $(SRC)\source\1d\1d_sort1.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_sort1.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_sort1.c
$(OUT)\source\1d\1d_sort5.obj: $(SRC)\source\1d\1d_sort5.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_sort5.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_sort5.c
$(OUT)\source\1d\1d_sum.obj: $(SRC)\source\1d\1d_sum.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_sum.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_sum.c
$(OUT)\source\1d\1d_sumsq.obj: $(SRC)\source\1d\1d_sumsq.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_sumsq.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_sumsq.c
$(OUT)\source\1d\1d_sumsd.obj: $(SRC)\source\1d\1d_sumsd.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_sumsd.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_sumsd.c
$(OUT)\source\1d\1d_sumss.obj: $(SRC)\source\1d\1d_sumss.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_sumss.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_sumss.c
$(OUT)\source\1d\1d_vari.obj: $(SRC)\source\1d\1d_vari.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_vari.obj $(OPTIMIZE_HIGH) \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_vari.c
$(OUT)\source\1d\1d_dat.obj: $(SRC)\source\1d\1d_dat.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_dat.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_dat.c
$(OUT)\source\1d\1d_ntype.obj: $(SRC)\source\1d\1d_ntype.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_ntype.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_ntype.c
$(OUT)\source\1d\1d.obj: $(SRC)\source\1d\1d.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d.c
$(OUT)\source\1d\1d_abs.obj: $(SRC)\source\1d\1d_abs.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_abs.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_abs.c
$(OUT)\source\1d\1d_stats.obj: $(SRC)\source\1d\1d_stats.c \
	  $(SRC)\include\dtack\config.h \
	  $(SRC)\include\dtack\ntype.h \
	  $(SRC)\include\dtack\stats.h \
	  $(SRC)\include\dtack\1d.h \
	  $(SRC)\include\dtack\1d.p
	cl /nologo /c /Gf /J /GB /MD /Gz \
	   /Fo$(OUT)\source\1d\1d_stats.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\1d_stats.c
$(OUT)\source\1d\libmain.obj: $(SRC)\source\1d\libmain.c \
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
	   /Fo$(OUT)\source\1d\libmain.obj $(Z7) /O1 \
	   /WX /W0 \
	  -DDTCONFIG_WV5 -DDTPRJ_CONSOLE=DT_$(MK_PRJ_CONSOLE) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)\source\1d \
	  -I$(SRC)\include \
	  $(SRC)\source\1d\libmain.c


