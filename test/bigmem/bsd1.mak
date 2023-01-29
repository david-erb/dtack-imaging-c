
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
	  $(OUT)/bin/dttmem$(MK_EXE)

shexes: \
	  $(OUT)/bin/dttmem$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdttmex.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdttmex.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dttmem$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttmem$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttmem$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttmem$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dttmem$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttmem$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdttmex.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdttmex.so$(MK_SHVER)
	$(MK_RM) $(OUT)/test/bigmem/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dttmem$(MK_EXE): \
	  $(OUT)/test/bigmem/mem.o \
	  $(OUT)/lib/libdttmex.$(MK_LIB) \
	  $(OUT)/lib/libdttzz.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttmem$(MK_EXE) \
	  $(OUT)/test/bigmem/mem.o \
	  -L$(OUT)/lib -ldttmex \
	  -L$(OUT)/lib -ldttzz \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dttmem$(MK_SHEXE): $(OUT)/test/bigmem/mem.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttmem$(MK_SHEXE) \
	  $(OUT)/test/bigmem/mem.o  \
	  -L$(OUT)/libso -ldttmex \
	  -L$(OUT)/libso -ldttzz \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdttmex.$(MK_LIB): \
	  $(OUT)/test/bigmem/dttmem2.o \
	  $(OUT)/test/bigmem/dttmem.o \
	  $(OUT)/test/bigmem/dttstack.o \
	  $(OUT)/test/bigmem/libmain.o
	$(MK_RM) $(OUT)/lib/libdttmex.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdttmex.$(MK_LIB) \
	  $(OUT)/test/bigmem/dttmem2.o \
	  $(OUT)/test/bigmem/dttmem.o \
	  $(OUT)/test/bigmem/dttstack.o \
	  $(OUT)/test/bigmem/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdttmex.$(MK_LIB)
$(OUT)/libso/libdttmex.so$(MK_SHVER): \
	  $(OUT)/test/bigmem/dttmem2.o \
	  $(OUT)/test/bigmem/dttmem.o \
	  $(OUT)/test/bigmem/dttstack.o \
	  $(OUT)/test/bigmem/libmain.o
	$(MK_RM) $(OUT)/libso/libdttmex.so$(MK_SHVER) $(OUT)/libso/libdttmex.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdttmex.so$(MK_SHVER) \
	  $(OUT)/test/bigmem/dttmem2.o \
	  $(OUT)/test/bigmem/dttmem.o \
	  $(OUT)/test/bigmem/dttstack.o \
	  $(OUT)/test/bigmem/libmain.o
	ln -s libdttmex.so$(MK_SHVER) $(OUT)/libso/libdttmex.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/test/bigmem/dttmem2.o: $(SRC)/test/bigmem/dttmem2.c \
	  $(SRC)/include/dtack/t.h \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/main.h \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/t.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/heap.h \
	  $(SRC)/include/dtack/heap.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/test/bigmem/dttmem2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/bigmem \
	  -I$(SRC)/include \
	  $(SRC)/test/bigmem/dttmem2.c
$(OUT)/test/bigmem/dttmem.o: $(SRC)/test/bigmem/dttmem.c \
	  $(SRC)/include/dtack/t.h \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/main.h \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/t.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/bigmem/dttmem.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/bigmem \
	  -I$(SRC)/include \
	  $(SRC)/test/bigmem/dttmem.c
$(OUT)/test/bigmem/dttstack.o: $(SRC)/test/bigmem/dttstack.c \
	  $(SRC)/include/dtack/t.h \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/main.h \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/t.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/stack.h \
	  $(SRC)/include/dtack/stack.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/test/bigmem/dttstack.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/bigmem \
	  -I$(SRC)/include \
	  $(SRC)/test/bigmem/dttstack.c
$(OUT)/test/bigmem/libmain.o: $(SRC)/test/bigmem/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/test/bigmem/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/bigmem \
	  -I$(SRC)/include \
	  $(SRC)/test/bigmem/libmain.c
$(OUT)/test/bigmem/mem.o: $(SRC)/test/bigmem/mem.c \
	  $(SRC)/include/dtack/t.h \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/main.h \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/t.p
	$(MK_CC) -c -o $(OUT)/test/bigmem/mem.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/bigmem \
	  -I$(SRC)/include \
	  $(SRC)/test/bigmem/mem.c


