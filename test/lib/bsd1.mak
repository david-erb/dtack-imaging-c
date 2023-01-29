
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
	  $(OUT)/lib/libdttzz.$(MK_LIB) \
	  $(OUT)/lib/libdttaa.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdttzz.so$(MK_SHVER) \
	  $(OUT)/libso/libdttaa.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/lib/libdttzz.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdttaa.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdttzz.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdttaa.so$(MK_SHVER)
	$(MK_RM) $(OUT)/test/lib/*.o
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



$(OUT)/lib/libdttzz.$(MK_LIB): \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/libmain.o
	$(MK_RM) $(OUT)/lib/libdttzz.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdttzz.$(MK_LIB) \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdttzz.$(MK_LIB)
$(OUT)/libso/libdttzz.so$(MK_SHVER): \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/libmain.o
	$(MK_RM) $(OUT)/libso/libdttzz.so$(MK_SHVER) $(OUT)/libso/libdttzz.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdttzz.so$(MK_SHVER) \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/libmain.o
	ln -s libdttzz.so$(MK_SHVER) $(OUT)/libso/libdttzz.so
$(OUT)/lib/libdttaa.$(MK_LIB): \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/gen256.o \
	  $(OUT)/test/lib/util1.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/raw.o \
	  $(OUT)/test/lib/blob_dat.o \
	  $(OUT)/test/lib/libmain.o
	$(MK_RM) $(OUT)/lib/libdttaa.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/gen256.o \
	  $(OUT)/test/lib/util1.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/raw.o \
	  $(OUT)/test/lib/blob_dat.o \
	  $(OUT)/test/lib/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdttaa.$(MK_LIB)
$(OUT)/libso/libdttaa.so$(MK_SHVER): \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/gen256.o \
	  $(OUT)/test/lib/util1.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/raw.o \
	  $(OUT)/test/lib/blob_dat.o \
	  $(OUT)/test/lib/libmain.o
	$(MK_RM) $(OUT)/libso/libdttaa.so$(MK_SHVER) $(OUT)/libso/libdttaa.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdttaa.so$(MK_SHVER) \
	  $(OUT)/test/lib/check.o \
	  $(OUT)/test/lib/gen256.o \
	  $(OUT)/test/lib/util1.o \
	  $(OUT)/test/lib/time.o \
	  $(OUT)/test/lib/raw.o \
	  $(OUT)/test/lib/blob_dat.o \
	  $(OUT)/test/lib/libmain.o
	ln -s libdttaa.so$(MK_SHVER) $(OUT)/libso/libdttaa.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/test/lib/gen256.o: $(SRC)/test/lib/gen256.c \
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
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/lib/gen256.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/gen256.c
$(OUT)/test/lib/check.o: $(SRC)/test/lib/check.c \
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
	$(MK_CC) -c -o $(OUT)/test/lib/check.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/check.c
$(OUT)/test/lib/time.o: $(SRC)/test/lib/time.c \
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
	$(MK_CC) -c -o $(OUT)/test/lib/time.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/time.c
$(OUT)/test/lib/libmain.o: $(SRC)/test/lib/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/test/lib/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/libmain.c
$(OUT)/test/lib/util1.o: $(SRC)/test/lib/util1.c \
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
	  $(SRC)/include/dtack/stack.h \
	  $(SRC)/include/dtack/stack.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/lib/util1.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/util1.c
$(OUT)/test/lib/raw.o: $(SRC)/test/lib/raw.c \
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
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/lib/raw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/raw.c
$(OUT)/test/lib/blob_dat.o: $(SRC)/test/lib/blob_dat.c \
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
	$(MK_CC) -c -o $(OUT)/test/lib/blob_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/lib \
	  -I$(SRC)/include \
	  $(SRC)/test/lib/blob_dat.c


