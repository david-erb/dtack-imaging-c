
SRC = ./../..
OUT = ./../..



default: $(MK_DEFAULT)


all: statics shareds others


static: statics
statics: libs exes 

shared: shareds
shareds: shlibs shexes 

allsubs:

exes: \
	  $(OUT)/bin/dtexim1$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtexim1$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtxchgs.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtxchg.so$(MK_SHVER) \
	  $(OUT)/libso/libdtxchgs.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtexim1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtexim1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtexim1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtexim1$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtexim1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtexim1$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdtxchg.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtxchgs.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtxchg.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtxchgs.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/xchg/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtexim1$(MK_EXE): \
	  $(OUT)/source/xchg/dtexim1.o \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtexim1$(MK_EXE) \
	  $(OUT)/source/xchg/dtexim1.o \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtexim1$(MK_SHEXE): $(OUT)/source/xchg/dtexim1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtexim1$(MK_SHEXE) \
	  $(OUT)/source/xchg/dtexim1.o  \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdtxchg.$(MK_LIB): \
	  $(OUT)/source/xchg/dib.o \
	  $(OUT)/source/xchg/export.o \
	  $(OUT)/source/xchg/import.o \
	  $(OUT)/source/xchg/read.o \
	  $(OUT)/source/xchg/readtif.o \
	  $(OUT)/source/xchg/bmp.o \
	  $(OUT)/source/xchg/lut8xchg.o \
	  $(OUT)/source/xchg/wmf.o \
	  $(OUT)/source/xchg/write.o \
	  $(OUT)/source/xchg/writebmp.o \
	  $(OUT)/source/xchg/writewmf.o \
	  $(OUT)/source/xchg/parse.o \
	  $(OUT)/source/xchg/xchg_dtm.o \
	  $(OUT)/source/xchg/xchg_win.o \
	  $(OUT)/source/xchg/xchg_jpg.o \
	  $(OUT)/source/xchg/xchg_wxx.o \
	  $(OUT)/source/xchg/xchg_dat.o \
	  $(OUT)/source/xchg/libmain.o
	$(MK_RM) $(OUT)/lib/libdtxchg.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/source/xchg/dib.o \
	  $(OUT)/source/xchg/export.o \
	  $(OUT)/source/xchg/import.o \
	  $(OUT)/source/xchg/read.o \
	  $(OUT)/source/xchg/readtif.o \
	  $(OUT)/source/xchg/bmp.o \
	  $(OUT)/source/xchg/lut8xchg.o \
	  $(OUT)/source/xchg/wmf.o \
	  $(OUT)/source/xchg/write.o \
	  $(OUT)/source/xchg/writebmp.o \
	  $(OUT)/source/xchg/writewmf.o \
	  $(OUT)/source/xchg/parse.o \
	  $(OUT)/source/xchg/xchg_dtm.o \
	  $(OUT)/source/xchg/xchg_win.o \
	  $(OUT)/source/xchg/xchg_jpg.o \
	  $(OUT)/source/xchg/xchg_wxx.o \
	  $(OUT)/source/xchg/xchg_dat.o \
	  $(OUT)/source/xchg/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtxchg.$(MK_LIB)
$(OUT)/libso/libdtxchg.so$(MK_SHVER): \
	  $(OUT)/source/xchg/dib.o \
	  $(OUT)/source/xchg/export.o \
	  $(OUT)/source/xchg/import.o \
	  $(OUT)/source/xchg/read.o \
	  $(OUT)/source/xchg/readtif.o \
	  $(OUT)/source/xchg/bmp.o \
	  $(OUT)/source/xchg/lut8xchg.o \
	  $(OUT)/source/xchg/wmf.o \
	  $(OUT)/source/xchg/write.o \
	  $(OUT)/source/xchg/writebmp.o \
	  $(OUT)/source/xchg/writewmf.o \
	  $(OUT)/source/xchg/parse.o \
	  $(OUT)/source/xchg/xchg_dtm.o \
	  $(OUT)/source/xchg/xchg_win.o \
	  $(OUT)/source/xchg/xchg_jpg.o \
	  $(OUT)/source/xchg/xchg_wxx.o \
	  $(OUT)/source/xchg/xchg_dat.o \
	  $(OUT)/source/xchg/libmain.o
	$(MK_RM) $(OUT)/libso/libdtxchg.so$(MK_SHVER) $(OUT)/libso/libdtxchg.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtxchg.so$(MK_SHVER) \
	  $(OUT)/source/xchg/dib.o \
	  $(OUT)/source/xchg/export.o \
	  $(OUT)/source/xchg/import.o \
	  $(OUT)/source/xchg/read.o \
	  $(OUT)/source/xchg/readtif.o \
	  $(OUT)/source/xchg/bmp.o \
	  $(OUT)/source/xchg/lut8xchg.o \
	  $(OUT)/source/xchg/wmf.o \
	  $(OUT)/source/xchg/write.o \
	  $(OUT)/source/xchg/writebmp.o \
	  $(OUT)/source/xchg/writewmf.o \
	  $(OUT)/source/xchg/parse.o \
	  $(OUT)/source/xchg/xchg_dtm.o \
	  $(OUT)/source/xchg/xchg_win.o \
	  $(OUT)/source/xchg/xchg_jpg.o \
	  $(OUT)/source/xchg/xchg_wxx.o \
	  $(OUT)/source/xchg/xchg_dat.o \
	  $(OUT)/source/xchg/libmain.o
	ln -s libdtxchg.so$(MK_SHVER) $(OUT)/libso/libdtxchg.so
$(OUT)/lib/libdtxchgs.$(MK_LIB): \
	  $(OUT)/source/xchg/xchg_scr.o \
	  $(OUT)/source/xchg/libmain.o
	$(MK_RM) $(OUT)/lib/libdtxchgs.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtxchgs.$(MK_LIB) \
	  $(OUT)/source/xchg/xchg_scr.o \
	  $(OUT)/source/xchg/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtxchgs.$(MK_LIB)
$(OUT)/libso/libdtxchgs.so$(MK_SHVER): \
	  $(OUT)/source/xchg/xchg_scr.o \
	  $(OUT)/source/xchg/libmain.o
	$(MK_RM) $(OUT)/libso/libdtxchgs.so$(MK_SHVER) $(OUT)/libso/libdtxchgs.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtxchgs.so$(MK_SHVER) \
	  $(OUT)/source/xchg/xchg_scr.o \
	  $(OUT)/source/xchg/libmain.o
	ln -s libdtxchgs.so$(MK_SHVER) $(OUT)/libso/libdtxchgs.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/xchg/dtexim1.o: $(SRC)/source/xchg/dtexim1.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/xchg/dtexim1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/dtexim1.c
$(OUT)/source/xchg/readtif.o: $(SRC)/source/xchg/readtif.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/readtif.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_TIFF=DT_$(MK_TIFF_DEFINE) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  -I$(MK_TIFF_INCLUDE) \
	  $(SRC)/source/xchg/readtif.c
$(OUT)/source/xchg/xchg_jpg.o: $(SRC)/source/xchg/xchg_jpg.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_jpg.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_IJG=DT_$(MK_IJG_DEFINE) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  -I$(MK_IJG_INCLUDE) \
	  $(SRC)/source/xchg/xchg_jpg.c
$(OUT)/source/xchg/xchg_dtm.o: $(SRC)/source/xchg/xchg_dtm.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_dtm.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/xchg_dtm.c
$(OUT)/source/xchg/xchg_scr.o: $(SRC)/source/xchg/xchg_scr.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_scr.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/xchg_scr.c
$(OUT)/source/xchg/xchg_can.o: $(SRC)/source/xchg/xchg_can.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_can.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/xchg_can.c
$(OUT)/source/xchg/dib.o: $(SRC)/source/xchg/dib.c \
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/dib.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/dib.c
$(OUT)/source/xchg/export.o: $(SRC)/source/xchg/export.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p
	$(MK_CC) -c -o $(OUT)/source/xchg/export.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/export.c
$(OUT)/source/xchg/import.o: $(SRC)/source/xchg/import.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/import.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/import.c
$(OUT)/source/xchg/read.o: $(SRC)/source/xchg/read.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p \
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/bmp.h \
	  $(SRC)/include/dtack/bmp.p \
	  $(SRC)/include/dtack/wmf.h \
	  $(SRC)/include/dtack/wmf.p
	$(MK_CC) -c -o $(OUT)/source/xchg/read.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/read.c
$(OUT)/source/xchg/bmp.o: $(SRC)/source/xchg/bmp.c \
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
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/bmp.h \
	  $(SRC)/include/dtack/bmp.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/bmp.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/bmp.c
$(OUT)/source/xchg/lut8xchg.o: $(SRC)/source/xchg/lut8xchg.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p
	$(MK_CC) -c -o $(OUT)/source/xchg/lut8xchg.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/lut8xchg.c
$(OUT)/source/xchg/wmf.o: $(SRC)/source/xchg/wmf.c \
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
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/wmf.h \
	  $(SRC)/include/dtack/wmf.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/xchg/wmf.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/wmf.c
$(OUT)/source/xchg/write.o: $(SRC)/source/xchg/write.c \
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
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p \
	  $(SRC)/include/dtack/stack.h \
	  $(SRC)/include/dtack/stack.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/xchg/write.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/write.c
$(OUT)/source/xchg/writebmp.o: $(SRC)/source/xchg/writebmp.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/bmp.h \
	  $(SRC)/include/dtack/bmp.p
	$(MK_CC) -c -o $(OUT)/source/xchg/writebmp.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/writebmp.c
$(OUT)/source/xchg/writewmf.o: $(SRC)/source/xchg/writewmf.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/wmf.h \
	  $(SRC)/include/dtack/wmf.p
	$(MK_CC) -c -o $(OUT)/source/xchg/writewmf.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/writewmf.c
$(OUT)/source/xchg/parse.o: $(SRC)/source/xchg/parse.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/xchg/parse.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/parse.c
$(OUT)/source/xchg/xchg_win.o: $(SRC)/source/xchg/xchg_win.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/winc.h \
	  $(SRC)/include/dtack/winc.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_win.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/xchg_win.c
$(OUT)/source/xchg/xchg_wxx.o: $(SRC)/source/xchg/xchg_wxx.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/dib.h \
	  $(SRC)/include/dtack/dib.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_wxx.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/xchg_wxx.c
$(OUT)/source/xchg/xchg_dat.o: $(SRC)/source/xchg/xchg_dat.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p
	$(MK_CC) -c -o $(OUT)/source/xchg/xchg_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/xchg_dat.c
$(OUT)/source/xchg/libmain.o: $(SRC)/source/xchg/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/xchg/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/xchg \
	  -I$(SRC)/include \
	  $(SRC)/source/xchg/libmain.c


