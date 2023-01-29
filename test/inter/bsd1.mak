
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
	  $(OUT)/bin/dttinter$(MK_EXE)

shexes: \
	  $(OUT)/bin/dttinter$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdttinter.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdttinter.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dttinter$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttinter$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttinter$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttinter$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dttinter$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttinter$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdttinter.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdttinter.so$(MK_SHVER)
	$(MK_RM) $(OUT)/test/inter/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dttinter$(MK_EXE): \
	  $(OUT)/test/inter/inter.o \
	  $(OUT)/lib/libdttinter.$(MK_LIB) \
	  $(OUT)/lib/libdttzz.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttinter$(MK_EXE) \
	  $(OUT)/test/inter/inter.o \
	  -L$(OUT)/lib -ldttinter \
	  -L$(OUT)/lib -ldttzz \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttinter$(MK_SHEXE): $(OUT)/test/inter/inter.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttinter$(MK_SHEXE) \
	  $(OUT)/test/inter/inter.o  \
	  -L$(OUT)/libso -ldttinter \
	  -L$(OUT)/libso -ldttzz \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdttinter.$(MK_LIB): \
	  $(OUT)/test/inter/dttmap.o \
	  $(OUT)/test/inter/dttkey.o \
	  $(OUT)/test/inter/dttdia.o \
	  $(OUT)/test/inter/dttalpha.o \
	  $(OUT)/test/inter/dttnote.o
	$(MK_RM) $(OUT)/lib/libdttinter.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdttinter.$(MK_LIB) \
	  $(OUT)/test/inter/dttmap.o \
	  $(OUT)/test/inter/dttkey.o \
	  $(OUT)/test/inter/dttdia.o \
	  $(OUT)/test/inter/dttalpha.o \
	  $(OUT)/test/inter/dttnote.o
	$(MK_RANLIB) $(OUT)/lib/libdttinter.$(MK_LIB)
$(OUT)/libso/libdttinter.so$(MK_SHVER): \
	  $(OUT)/test/inter/dttmap.o \
	  $(OUT)/test/inter/dttkey.o \
	  $(OUT)/test/inter/dttdia.o \
	  $(OUT)/test/inter/dttalpha.o \
	  $(OUT)/test/inter/dttnote.o
	$(MK_RM) $(OUT)/libso/libdttinter.so$(MK_SHVER) $(OUT)/libso/libdttinter.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdttinter.so$(MK_SHVER) \
	  $(OUT)/test/inter/dttmap.o \
	  $(OUT)/test/inter/dttkey.o \
	  $(OUT)/test/inter/dttdia.o \
	  $(OUT)/test/inter/dttalpha.o \
	  $(OUT)/test/inter/dttnote.o
	ln -s libdttinter.so$(MK_SHVER) $(OUT)/libso/libdttinter.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/test/inter/dttmap.o: $(SRC)/test/inter/dttmap.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p
	$(MK_CC) -c -o $(OUT)/test/inter/dttmap.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/inter \
	  -I$(SRC)/include \
	  $(SRC)/test/inter/dttmap.c
$(OUT)/test/inter/dttkey.o: $(SRC)/test/inter/dttkey.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/test/inter/dttkey.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/inter \
	  -I$(SRC)/include \
	  $(SRC)/test/inter/dttkey.c
$(OUT)/test/inter/dttdia.o: $(SRC)/test/inter/dttdia.c \
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
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/test/inter/dttdia.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/inter \
	  -I$(SRC)/include \
	  $(SRC)/test/inter/dttdia.c
$(OUT)/test/inter/dttalpha.o: $(SRC)/test/inter/dttalpha.c \
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
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/alpha.h \
	  $(SRC)/include/dtack/alpha.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/test/inter/dttalpha.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/inter \
	  -I$(SRC)/include \
	  $(SRC)/test/inter/dttalpha.c
$(OUT)/test/inter/dttnote.o: $(SRC)/test/inter/dttnote.c \
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
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/inter/dttnote.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/inter \
	  -I$(SRC)/include \
	  $(SRC)/test/inter/dttnote.c
$(OUT)/test/inter/inter.o: $(SRC)/test/inter/inter.c \
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
	$(MK_CC) -c -o $(OUT)/test/inter/inter.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/inter \
	  -I$(SRC)/include \
	  $(SRC)/test/inter/inter.c


