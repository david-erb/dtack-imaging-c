
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
	  $(OUT)/lib/libdtinter.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtinter.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/lib/libdtinter.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtinter.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/inter/*.o
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



$(OUT)/lib/libdtinter.$(MK_LIB): \
	  $(OUT)/source/inter/alpha.o \
	  $(OUT)/source/inter/dialog.o \
	  $(OUT)/source/inter/diatext.o \
	  $(OUT)/source/inter/diavba.o \
	  $(OUT)/source/inter/iact.o \
	  $(OUT)/source/inter/interact.o \
	  $(OUT)/source/inter/interdia.o \
	  $(OUT)/source/inter/interhlp.o \
	  $(OUT)/source/inter/note.o \
	  $(OUT)/source/inter/text.o \
	  $(OUT)/source/inter/textcur1.o \
	  $(OUT)/source/inter/texttcap.o \
	  $(OUT)/source/inter/textdos1.o \
	  $(OUT)/source/inter/textqnx1.o \
	  $(OUT)/source/inter/libmain.o
	$(MK_RM) $(OUT)/lib/libdtinter.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtinter.$(MK_LIB) \
	  $(OUT)/source/inter/alpha.o \
	  $(OUT)/source/inter/dialog.o \
	  $(OUT)/source/inter/diatext.o \
	  $(OUT)/source/inter/diavba.o \
	  $(OUT)/source/inter/iact.o \
	  $(OUT)/source/inter/interact.o \
	  $(OUT)/source/inter/interdia.o \
	  $(OUT)/source/inter/interhlp.o \
	  $(OUT)/source/inter/note.o \
	  $(OUT)/source/inter/text.o \
	  $(OUT)/source/inter/textcur1.o \
	  $(OUT)/source/inter/texttcap.o \
	  $(OUT)/source/inter/textdos1.o \
	  $(OUT)/source/inter/textqnx1.o \
	  $(OUT)/source/inter/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtinter.$(MK_LIB)
$(OUT)/libso/libdtinter.so$(MK_SHVER): \
	  $(OUT)/source/inter/alpha.o \
	  $(OUT)/source/inter/dialog.o \
	  $(OUT)/source/inter/diatext.o \
	  $(OUT)/source/inter/diavba.o \
	  $(OUT)/source/inter/iact.o \
	  $(OUT)/source/inter/interact.o \
	  $(OUT)/source/inter/interdia.o \
	  $(OUT)/source/inter/interhlp.o \
	  $(OUT)/source/inter/note.o \
	  $(OUT)/source/inter/text.o \
	  $(OUT)/source/inter/textcur1.o \
	  $(OUT)/source/inter/texttcap.o \
	  $(OUT)/source/inter/textdos1.o \
	  $(OUT)/source/inter/textqnx1.o \
	  $(OUT)/source/inter/libmain.o
	$(MK_RM) $(OUT)/libso/libdtinter.so$(MK_SHVER) $(OUT)/libso/libdtinter.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtinter.so$(MK_SHVER) \
	  $(OUT)/source/inter/alpha.o \
	  $(OUT)/source/inter/dialog.o \
	  $(OUT)/source/inter/diatext.o \
	  $(OUT)/source/inter/diavba.o \
	  $(OUT)/source/inter/iact.o \
	  $(OUT)/source/inter/interact.o \
	  $(OUT)/source/inter/interdia.o \
	  $(OUT)/source/inter/interhlp.o \
	  $(OUT)/source/inter/note.o \
	  $(OUT)/source/inter/text.o \
	  $(OUT)/source/inter/textcur1.o \
	  $(OUT)/source/inter/texttcap.o \
	  $(OUT)/source/inter/textdos1.o \
	  $(OUT)/source/inter/textqnx1.o \
	  $(OUT)/source/inter/libmain.o
	ln -s libdtinter.so$(MK_SHVER) $(OUT)/libso/libdtinter.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/inter/textcur1.o: $(SRC)/source/inter/textcur1.c \
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
	  $(SRC)/include/dtack/text.h \
	  $(SRC)/include/dtack/text.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/inter/textcur1.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_CURSES=DT_$(MK_PRJ_CURSES_DEFINE) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  -I$(MK_PRJ_CURSES_INCLUDE) \
	  $(SRC)/source/inter/textcur1.c
$(OUT)/source/inter/textdos1.o: $(SRC)/source/inter/textdos1.c \
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
	  $(SRC)/include/dtack/text.h \
	  $(SRC)/include/dtack/text.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/inter/textdos1.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/textdos1.c
$(OUT)/source/inter/textqnx1.o: $(SRC)/source/inter/textqnx1.c \
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
	  $(SRC)/include/dtack/text.h \
	  $(SRC)/include/dtack/text.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/qnx4.h \
	  $(SRC)/include/dtack/qnx4.p
	$(MK_CC) -c -o $(OUT)/source/inter/textqnx1.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/textqnx1.c
$(OUT)/source/inter/texttcap.o: $(SRC)/source/inter/texttcap.c \
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
	  $(SRC)/include/dtack/text.h \
	  $(SRC)/include/dtack/text.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/inter/texttcap.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_TERMCAP=DT_$(MK_PRJ_TERMCAP_DEFINE) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  -I$(MK_PRJ_TERMCAP_INCLUDE) \
	  $(SRC)/source/inter/texttcap.c
$(OUT)/source/inter/alpha.o: $(SRC)/source/inter/alpha.c \
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
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/alpha.h \
	  $(SRC)/include/dtack/alpha.p
	$(MK_CC) -c -o $(OUT)/source/inter/alpha.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/alpha.c
$(OUT)/source/inter/dialog.o: $(SRC)/source/inter/dialog.c \
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
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p
	$(MK_CC) -c -o $(OUT)/source/inter/dialog.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/dialog.c
$(OUT)/source/inter/diatext.o: $(SRC)/source/inter/diatext.c \
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
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/text.h \
	  $(SRC)/include/dtack/text.p
	$(MK_CC) -c -o $(OUT)/source/inter/diatext.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/diatext.c
$(OUT)/source/inter/diavba.o: $(SRC)/source/inter/diavba.c \
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
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p
	$(MK_CC) -c -o $(OUT)/source/inter/diavba.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/diavba.c
$(OUT)/source/inter/iact.o: $(SRC)/source/inter/iact.c \
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
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/alpha.h \
	  $(SRC)/include/dtack/alpha.p \
	  $(SRC)/include/dtack/iact.h \
	  $(SRC)/include/dtack/iact.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/inter/iact.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/iact.c
$(OUT)/source/inter/interact.o: $(SRC)/source/inter/interact.c \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/alpha.h \
	  $(SRC)/include/dtack/alpha.p \
	  $(SRC)/include/dtack/iact.h \
	  $(SRC)/include/dtack/iact.p
	$(MK_CC) -c -o $(OUT)/source/inter/interact.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/interact.c
$(OUT)/source/inter/interdia.o: $(SRC)/source/inter/interdia.c \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/alpha.h \
	  $(SRC)/include/dtack/alpha.p \
	  $(SRC)/include/dtack/iact.h \
	  $(SRC)/include/dtack/iact.p
	$(MK_CC) -c -o $(OUT)/source/inter/interdia.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/interdia.c
$(OUT)/source/inter/interhlp.o: $(SRC)/source/inter/interhlp.c \
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
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/dialog.h \
	  $(SRC)/include/dtack/dialog.p \
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/alpha.h \
	  $(SRC)/include/dtack/alpha.p \
	  $(SRC)/include/dtack/iact.h \
	  $(SRC)/include/dtack/iact.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p
	$(MK_CC) -c -o $(OUT)/source/inter/interhlp.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/interhlp.c
$(OUT)/source/inter/note.o: $(SRC)/source/inter/note.c \
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
	  $(SRC)/include/dtack/note.h \
	  $(SRC)/include/dtack/note.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/inter/note.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/note.c
$(OUT)/source/inter/text.o: $(SRC)/source/inter/text.c \
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
	  $(SRC)/include/dtack/text.h \
	  $(SRC)/include/dtack/text.p
	$(MK_CC) -c -o $(OUT)/source/inter/text.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/text.c
$(OUT)/source/inter/libmain.o: $(SRC)/source/inter/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/inter/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/inter \
	  -I$(SRC)/include \
	  $(SRC)/source/inter/libmain.c


