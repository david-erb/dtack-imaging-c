
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
	  $(OUT)/bin/dttview$(MK_EXE)

shexes: \
	  $(OUT)/bin/dttview$(MK_SHEXE)

scripts: 

libs: 

shlibs: 

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dttview$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttview$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttview$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttview$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dttview$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttview$(MK_EXE)
	$(MK_RM) $(OUT)/test/view/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dttview$(MK_EXE): \
	  $(OUT)/test/view/dttview.o \
	  $(OUT)/test/view/dttviewx.o \
	  $(OUT)/lib/libdtserver.$(MK_LIB) \
	  $(OUT)/lib/libdtwins.$(MK_LIB) \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtxchgs.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtscr.$(MK_LIB) \
	  $(OUT)/lib/libdtcand.$(MK_LIB) \
	  $(OUT)/lib/libdthws.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttview$(MK_EXE) \
	  $(OUT)/test/view/dttview.o \
	  $(OUT)/test/view/dttviewx.o \
	  -L$(OUT)/lib -ldtserver \
	  -L$(OUT)/lib -ldtwins \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtxchgs \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtscr \
	  -L$(OUT)/lib -ldtcand \
	  -L$(OUT)/lib -ldthws \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_X_LOAD)    $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttview$(MK_SHEXE): $(OUT)/test/view/dttview.o \
	  $(OUT)/test/view/dttviewx.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttview$(MK_SHEXE) \
	  $(OUT)/test/view/dttview.o \
	  $(OUT)/test/view/dttviewx.o  \
	  -L$(OUT)/libso -ldtserver \
	  -L$(OUT)/libso -ldtwins \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtxchgs \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtscr \
	  -L$(OUT)/libso -ldtcand \
	  -L$(OUT)/libso -ldthws \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_X_LOAD)    $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)

#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/test/view/dttview.o: $(SRC)/test/view/dttview.c \
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
	$(MK_CC) -c -o $(OUT)/test/view/dttview.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/view \
	  -I$(SRC)/include \
	  $(SRC)/test/view/dttview.c
$(OUT)/test/view/dttviewx.o: $(SRC)/test/view/dttviewx.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p \
	  $(SRC)/include/dtack/view.h \
	  $(SRC)/include/dtack/view.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/test/view/dttview.use
	$(MK_CC) -c -o $(OUT)/test/view/dttviewx.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/view \
	  -I$(SRC)/include \
	  $(SRC)/test/view/dttviewx.c


