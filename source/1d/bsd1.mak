
SRC = ./../..
OUT = ./../..



default: $(MK_DEFAULT)


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
	  $(OUT)/lib/libdt1d.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdt1d.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdt1d.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/1d/*.o
	$(MK_RM) $(MK_JUNK)

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



$(OUT)/lib/libdt1d.$(MK_LIB): \
	  $(OUT)/source/1d/1d_thres.o \
	  $(OUT)/source/1d/1d.o \
	  $(OUT)/source/1d/1d_abs.o \
	  $(OUT)/source/1d/1d_arith.o \
	  $(OUT)/source/1d/1d_noise.o \
	  $(OUT)/source/1d/1d_set.o \
	  $(OUT)/source/1d/1d_sum.o \
	  $(OUT)/source/1d/1d_sumsq.o \
	  $(OUT)/source/1d/1d_sumsd.o \
	  $(OUT)/source/1d/1d_sumss.o \
	  $(OUT)/source/1d/1d_cumf.o \
	  $(OUT)/source/1d/1d_cumr.o \
	  $(OUT)/source/1d/1d_logic.o \
	  $(OUT)/source/1d/1d_look.o \
	  $(OUT)/source/1d/1d_cmp.o \
	  $(OUT)/source/1d/1d_diff.o \
	  $(OUT)/source/1d/1d_fft.o \
	  $(OUT)/source/1d/1d_fftw.o \
	  $(OUT)/source/1d/1d_range.o \
	  $(OUT)/source/1d/1d_ran.o \
	  $(OUT)/source/1d/1d_sort1.o \
	  $(OUT)/source/1d/1d_sort5.o \
	  $(OUT)/source/1d/1d_stats.o \
	  $(OUT)/source/1d/1d_cast.o \
	  $(OUT)/source/1d/1d_conv.o \
	  $(OUT)/source/1d/1d_clip.o \
	  $(OUT)/source/1d/1d_vari.o \
	  $(OUT)/source/1d/1d_dat.o \
	  $(OUT)/source/1d/1d_ntype.o \
	  $(OUT)/source/1d/libmain.o
	$(MK_RM) $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdt1d.$(MK_LIB) \
	  $(OUT)/source/1d/1d_thres.o \
	  $(OUT)/source/1d/1d.o \
	  $(OUT)/source/1d/1d_abs.o \
	  $(OUT)/source/1d/1d_arith.o \
	  $(OUT)/source/1d/1d_noise.o \
	  $(OUT)/source/1d/1d_set.o \
	  $(OUT)/source/1d/1d_sum.o \
	  $(OUT)/source/1d/1d_sumsq.o \
	  $(OUT)/source/1d/1d_sumsd.o \
	  $(OUT)/source/1d/1d_sumss.o \
	  $(OUT)/source/1d/1d_cumf.o \
	  $(OUT)/source/1d/1d_cumr.o \
	  $(OUT)/source/1d/1d_logic.o \
	  $(OUT)/source/1d/1d_look.o \
	  $(OUT)/source/1d/1d_cmp.o \
	  $(OUT)/source/1d/1d_diff.o \
	  $(OUT)/source/1d/1d_fft.o \
	  $(OUT)/source/1d/1d_fftw.o \
	  $(OUT)/source/1d/1d_range.o \
	  $(OUT)/source/1d/1d_ran.o \
	  $(OUT)/source/1d/1d_sort1.o \
	  $(OUT)/source/1d/1d_sort5.o \
	  $(OUT)/source/1d/1d_stats.o \
	  $(OUT)/source/1d/1d_cast.o \
	  $(OUT)/source/1d/1d_conv.o \
	  $(OUT)/source/1d/1d_clip.o \
	  $(OUT)/source/1d/1d_vari.o \
	  $(OUT)/source/1d/1d_dat.o \
	  $(OUT)/source/1d/1d_ntype.o \
	  $(OUT)/source/1d/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdt1d.$(MK_LIB)
$(OUT)/libso/libdt1d.so$(MK_SHVER): \
	  $(OUT)/source/1d/1d_thres.o \
	  $(OUT)/source/1d/1d.o \
	  $(OUT)/source/1d/1d_abs.o \
	  $(OUT)/source/1d/1d_arith.o \
	  $(OUT)/source/1d/1d_noise.o \
	  $(OUT)/source/1d/1d_set.o \
	  $(OUT)/source/1d/1d_sum.o \
	  $(OUT)/source/1d/1d_sumsq.o \
	  $(OUT)/source/1d/1d_sumsd.o \
	  $(OUT)/source/1d/1d_sumss.o \
	  $(OUT)/source/1d/1d_cumf.o \
	  $(OUT)/source/1d/1d_cumr.o \
	  $(OUT)/source/1d/1d_logic.o \
	  $(OUT)/source/1d/1d_look.o \
	  $(OUT)/source/1d/1d_cmp.o \
	  $(OUT)/source/1d/1d_diff.o \
	  $(OUT)/source/1d/1d_fft.o \
	  $(OUT)/source/1d/1d_fftw.o \
	  $(OUT)/source/1d/1d_range.o \
	  $(OUT)/source/1d/1d_ran.o \
	  $(OUT)/source/1d/1d_sort1.o \
	  $(OUT)/source/1d/1d_sort5.o \
	  $(OUT)/source/1d/1d_stats.o \
	  $(OUT)/source/1d/1d_cast.o \
	  $(OUT)/source/1d/1d_conv.o \
	  $(OUT)/source/1d/1d_clip.o \
	  $(OUT)/source/1d/1d_vari.o \
	  $(OUT)/source/1d/1d_dat.o \
	  $(OUT)/source/1d/1d_ntype.o \
	  $(OUT)/source/1d/libmain.o
	$(MK_RM) $(OUT)/libso/libdt1d.so$(MK_SHVER) $(OUT)/libso/libdt1d.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdt1d.so$(MK_SHVER) \
	  $(OUT)/source/1d/1d_thres.o \
	  $(OUT)/source/1d/1d.o \
	  $(OUT)/source/1d/1d_abs.o \
	  $(OUT)/source/1d/1d_arith.o \
	  $(OUT)/source/1d/1d_noise.o \
	  $(OUT)/source/1d/1d_set.o \
	  $(OUT)/source/1d/1d_sum.o \
	  $(OUT)/source/1d/1d_sumsq.o \
	  $(OUT)/source/1d/1d_sumsd.o \
	  $(OUT)/source/1d/1d_sumss.o \
	  $(OUT)/source/1d/1d_cumf.o \
	  $(OUT)/source/1d/1d_cumr.o \
	  $(OUT)/source/1d/1d_logic.o \
	  $(OUT)/source/1d/1d_look.o \
	  $(OUT)/source/1d/1d_cmp.o \
	  $(OUT)/source/1d/1d_diff.o \
	  $(OUT)/source/1d/1d_fft.o \
	  $(OUT)/source/1d/1d_fftw.o \
	  $(OUT)/source/1d/1d_range.o \
	  $(OUT)/source/1d/1d_ran.o \
	  $(OUT)/source/1d/1d_sort1.o \
	  $(OUT)/source/1d/1d_sort5.o \
	  $(OUT)/source/1d/1d_stats.o \
	  $(OUT)/source/1d/1d_cast.o \
	  $(OUT)/source/1d/1d_conv.o \
	  $(OUT)/source/1d/1d_clip.o \
	  $(OUT)/source/1d/1d_vari.o \
	  $(OUT)/source/1d/1d_dat.o \
	  $(OUT)/source/1d/1d_ntype.o \
	  $(OUT)/source/1d/libmain.o
	ln -s libdt1d.so$(MK_SHVER) $(OUT)/libso/libdt1d.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/1d/1d_fftw.o: $(SRC)/source/1d/1d_fftw.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_fftw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_FFTW_DEFINE) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  -I$(MK_FFTW_INCLUDE) \
	  $(SRC)/source/1d/1d_fftw.c
$(OUT)/source/1d/1d_fft.o: $(SRC)/source/1d/1d_fft.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_fft.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_FFTW_DEFINE) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_fft.c
$(OUT)/source/1d/1d_thres.o: $(SRC)/source/1d/1d_thres.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_thres.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_thres.c
$(OUT)/source/1d/1d_arith.o: $(SRC)/source/1d/1d_arith.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_arith.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_arith.c
$(OUT)/source/1d/1d_logic.o: $(SRC)/source/1d/1d_logic.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_logic.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_logic.c
$(OUT)/source/1d/1d_cast.o: $(SRC)/source/1d/1d_cast.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_cast.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_cast.c
$(OUT)/source/1d/1d_clip.o: $(SRC)/source/1d/1d_clip.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_clip.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_clip.c
$(OUT)/source/1d/1d_cmp.o: $(SRC)/source/1d/1d_cmp.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_cmp.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_cmp.c
$(OUT)/source/1d/1d_conv.o: $(SRC)/source/1d/1d_conv.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_conv.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_conv.c
$(OUT)/source/1d/1d_cumf.o: $(SRC)/source/1d/1d_cumf.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_cumf.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_cumf.c
$(OUT)/source/1d/1d_cumr.o: $(SRC)/source/1d/1d_cumr.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_cumr.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_cumr.c
$(OUT)/source/1d/1d_diff.o: $(SRC)/source/1d/1d_diff.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_diff.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_diff.c
$(OUT)/source/1d/1d_look.o: $(SRC)/source/1d/1d_look.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_look.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_look.c
$(OUT)/source/1d/1d_noise.o: $(SRC)/source/1d/1d_noise.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_noise.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_noise.c
$(OUT)/source/1d/1d_ran.o: $(SRC)/source/1d/1d_ran.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_ran.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_ran.c
$(OUT)/source/1d/1d_range.o: $(SRC)/source/1d/1d_range.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_range.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_range.c
$(OUT)/source/1d/1d_set.o: $(SRC)/source/1d/1d_set.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_set.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_set.c
$(OUT)/source/1d/1d_sort1.o: $(SRC)/source/1d/1d_sort1.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_sort1.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_sort1.c
$(OUT)/source/1d/1d_sort5.o: $(SRC)/source/1d/1d_sort5.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_sort5.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_sort5.c
$(OUT)/source/1d/1d_sum.o: $(SRC)/source/1d/1d_sum.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_sum.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_sum.c
$(OUT)/source/1d/1d_sumsq.o: $(SRC)/source/1d/1d_sumsq.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_sumsq.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_sumsq.c
$(OUT)/source/1d/1d_sumsd.o: $(SRC)/source/1d/1d_sumsd.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_sumsd.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_sumsd.c
$(OUT)/source/1d/1d_sumss.o: $(SRC)/source/1d/1d_sumss.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_sumss.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_sumss.c
$(OUT)/source/1d/1d_vari.o: $(SRC)/source/1d/1d_vari.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_vari.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_vari.c
$(OUT)/source/1d/1d.o: $(SRC)/source/1d/1d.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d.c
$(OUT)/source/1d/1d_abs.o: $(SRC)/source/1d/1d_abs.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_abs.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_abs.c
$(OUT)/source/1d/1d_stats.o: $(SRC)/source/1d/1d_stats.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_stats.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_stats.c
$(OUT)/source/1d/1d_dat.o: $(SRC)/source/1d/1d_dat.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_dat.c
$(OUT)/source/1d/1d_ntype.o: $(SRC)/source/1d/1d_ntype.c \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p
	$(MK_CC) -c -o $(OUT)/source/1d/1d_ntype.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/1d_ntype.c
$(OUT)/source/1d/libmain.o: $(SRC)/source/1d/libmain.c \
	  $(SRC)/include/dtack/base.h \
	  $(SRC)/include/dtack/config.h \
	  $(SRC)/include/dtack/ntype.h \
	  $(SRC)/include/dtack/stats.h \
	  $(SRC)/include/dtack/1d.h \
	  $(SRC)/include/dtack/1d.p \
	  $(SRC)/include/dtack/config2.h \
	  $(SRC)/include/dtack/param.h \
	  $(SRC)/include/dtack/param.p \
	  $(SRC)/include/dtack/cstack.h \
	  $(SRC)/include/dtack/cstack.p \
	  $(SRC)/include/dtack/hist.h \
	  $(SRC)/include/dtack/hist.p \
	  $(SRC)/include/dtack/fd.h \
	  $(SRC)/include/dtack/fd.p \
	  $(SRC)/include/dtack/run.h \
	  $(SRC)/include/dtack/run.p \
	  $(SRC)/include/dtack/image.h \
	  $(SRC)/include/dtack/image.p \
	  $(SRC)/include/dtack/base.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/1d/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/1d \
	  -I$(SRC)/include \
	  $(SRC)/source/1d/libmain.c


