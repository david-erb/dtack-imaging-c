
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
	  $(OUT)/bin/dtkeybd1$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtkeybd1$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdtkeybd.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtkeybd.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtkeybd1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtkeybd1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtkeybd1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtkeybd1$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtkeybd1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtkeybd1$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdtkeybd.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtkeybd.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/keybd/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtkeybd1$(MK_EXE): \
	  $(OUT)/source/keybd/dtkeybd1.o \
	  $(OUT)/lib/libdtkeybd.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtkeybd1$(MK_EXE) \
	  $(OUT)/source/keybd/dtkeybd1.o \
	  -L$(OUT)/lib -ldtkeybd \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dtkeybd1$(MK_SHEXE): $(OUT)/source/keybd/dtkeybd1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtkeybd1$(MK_SHEXE) \
	  $(OUT)/source/keybd/dtkeybd1.o  \
	  -L$(OUT)/libso -ldtkeybd \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdtkeybd.$(MK_LIB): \
	  $(OUT)/source/keybd/keybd.o \
	  $(OUT)/source/keybd/keyunix.o \
	  $(OUT)/source/keybd/keyqnx.o \
	  $(OUT)/source/keybd/keydos.o \
	  $(OUT)/source/keybd/keymap.o \
	  $(OUT)/source/keybd/keymapt.o \
	  $(OUT)/source/keybd/keymapc.o \
	  $(OUT)/source/keybd/keymapq.o \
	  $(OUT)/source/keybd/trmios.o \
	  $(OUT)/source/keybd/libmain.o
	$(MK_RM) $(OUT)/lib/libdtkeybd.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtkeybd.$(MK_LIB) \
	  $(OUT)/source/keybd/keybd.o \
	  $(OUT)/source/keybd/keyunix.o \
	  $(OUT)/source/keybd/keyqnx.o \
	  $(OUT)/source/keybd/keydos.o \
	  $(OUT)/source/keybd/keymap.o \
	  $(OUT)/source/keybd/keymapt.o \
	  $(OUT)/source/keybd/keymapc.o \
	  $(OUT)/source/keybd/keymapq.o \
	  $(OUT)/source/keybd/trmios.o \
	  $(OUT)/source/keybd/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtkeybd.$(MK_LIB)
$(OUT)/libso/libdtkeybd.so$(MK_SHVER): \
	  $(OUT)/source/keybd/keybd.o \
	  $(OUT)/source/keybd/keyunix.o \
	  $(OUT)/source/keybd/keyqnx.o \
	  $(OUT)/source/keybd/keydos.o \
	  $(OUT)/source/keybd/keymap.o \
	  $(OUT)/source/keybd/keymapt.o \
	  $(OUT)/source/keybd/keymapc.o \
	  $(OUT)/source/keybd/keymapq.o \
	  $(OUT)/source/keybd/trmios.o \
	  $(OUT)/source/keybd/libmain.o
	$(MK_RM) $(OUT)/libso/libdtkeybd.so$(MK_SHVER) $(OUT)/libso/libdtkeybd.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtkeybd.so$(MK_SHVER) \
	  $(OUT)/source/keybd/keybd.o \
	  $(OUT)/source/keybd/keyunix.o \
	  $(OUT)/source/keybd/keyqnx.o \
	  $(OUT)/source/keybd/keydos.o \
	  $(OUT)/source/keybd/keymap.o \
	  $(OUT)/source/keybd/keymapt.o \
	  $(OUT)/source/keybd/keymapc.o \
	  $(OUT)/source/keybd/keymapq.o \
	  $(OUT)/source/keybd/trmios.o \
	  $(OUT)/source/keybd/libmain.o
	ln -s libdtkeybd.so$(MK_SHVER) $(OUT)/libso/libdtkeybd.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/keybd/dtkeybd1.o: $(SRC)/source/keybd/dtkeybd1.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/keybd/dtkeybd1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  $(SRC)/source/keybd/dtkeybd1.c
$(OUT)/source/keybd/keymapq.o: $(SRC)/source/keybd/keymapq.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keymapq.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  -IPRJ_QNXTERM_INCLUDE \
	  $(SRC)/source/keybd/keymapq.c
$(OUT)/source/keybd/keymapt.o: $(SRC)/source/keybd/keymapt.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keymapt.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_TERMCAP_DEFINE) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  -I$(MK_TERMCAP_INCLUDE) \
	  $(SRC)/source/keybd/keymapt.c
$(OUT)/source/keybd/keyunix.o: $(SRC)/source/keybd/keyunix.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keyunix.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_TERMIOS_DEFINE) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  -I$(MK_TERMIOS_INCLUDE) \
	  $(SRC)/source/keybd/keyunix.c
$(OUT)/source/keybd/keyqnx.o: $(SRC)/source/keybd/keyqnx.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keyqnx.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  -IPRJ_QNXTERM_INCLUDE \
	  $(SRC)/source/keybd/keyqnx.c
$(OUT)/source/keybd/trmios.o: $(SRC)/source/keybd/trmios.c \
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
	$(MK_CC) -c -o $(OUT)/source/keybd/trmios.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -D$(MK_TERMIOS_DEFINE) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  -I$(MK_TERMIOS_INCLUDE) \
	  $(SRC)/source/keybd/trmios.c
$(OUT)/source/keybd/keybd.o: $(SRC)/source/keybd/keybd.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keybd.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  $(SRC)/source/keybd/keybd.c
$(OUT)/source/keybd/keydos.o: $(SRC)/source/keybd/keydos.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keydos.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  $(SRC)/source/keybd/keydos.c
$(OUT)/source/keybd/keymap.o: $(SRC)/source/keybd/keymap.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keymap.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  $(SRC)/source/keybd/keymap.c
$(OUT)/source/keybd/keymapc.o: $(SRC)/source/keybd/keymapc.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p
	$(MK_CC) -c -o $(OUT)/source/keybd/keymapc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  $(SRC)/source/keybd/keymapc.c
$(OUT)/source/keybd/libmain.o: $(SRC)/source/keybd/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/keybd/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/keybd \
	  -I$(SRC)/include \
	  $(SRC)/source/keybd/libmain.c


