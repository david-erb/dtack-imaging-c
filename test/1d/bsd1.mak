
SRC = ./../..
OUT = ./../..



default: $(MK_DEFAULT)


all: statics shareds others


static: statics
statics: exes 

shared: shareds
shareds: shexes 

allsubs:

exes: \
	  $(OUT)/bin/dtt1d$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtt1d$(MK_SHEXE)

scripts: 

libs: 

shlibs: 

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtt1d$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtt1d$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtt1d$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtt1d$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtt1d$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtt1d$(MK_EXE)
	$(MK_RM) $(OUT)/test/1d/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtt1d$(MK_EXE): \
	  $(OUT)/test/1d/dtt1d.o \
	  $(OUT)/lib/libdttzz.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtt1d$(MK_EXE) \
	  $(OUT)/test/1d/dtt1d.o \
	  -L$(OUT)/lib -ldttzz \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dtt1d$(MK_SHEXE): $(OUT)/test/1d/dtt1d.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtt1d$(MK_SHEXE) \
	  $(OUT)/test/1d/dtt1d.o  \
	  -L$(OUT)/libso -ldttzz \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)

#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/test/1d/dtt1d.o: $(SRC)/test/1d/dtt1d.c \
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
	$(MK_CC) -c -o $(OUT)/test/1d/dtt1d.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_FFTW_DEFINE) \
	  -I$(SRC)/test/1d \
	  -I$(SRC)/include \
	  $(SRC)/test/1d/dtt1d.c


