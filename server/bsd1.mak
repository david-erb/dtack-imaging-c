
SRC = ./..
OUT = ./..



default: $(MK_DEFAULT)


all: statics shareds others


static: statics
statics: libs exes 

shared: shareds
shareds: shlibs shexes 

allsubs:

exes: \
	  $(OUT)/bin/dtview$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtview$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdtserver.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtserver.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtview$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtview$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtview$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtview$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtview$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtview$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdtserver.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtserver.so$(MK_SHVER)
	$(MK_RM) $(OUT)/server/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtview$(MK_EXE): \
	  $(OUT)/server/dtview.o \
	  $(OUT)/server/view_ipc.o \
	  $(OUT)/server/view1.o \
	  $(OUT)/lib/libdtserver.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtxchgs.$(MK_LIB) \
	  $(OUT)/lib/libdtwins.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtscr.$(MK_LIB) \
	  $(OUT)/lib/libdtcand.$(MK_LIB) \
	  $(OUT)/lib/libdthws.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtview$(MK_EXE) \
	  $(OUT)/server/dtview.o \
	  $(OUT)/server/view_ipc.o \
	  $(OUT)/server/view1.o \
	  -L$(OUT)/lib -ldtserver \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtxchgs \
	  -L$(OUT)/lib -ldtwins \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtscr \
	  -L$(OUT)/lib -ldtcand \
	  -L$(OUT)/lib -ldthws \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_X_LOAD)    $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dtview$(MK_SHEXE): $(OUT)/server/dtview.o \
	  $(OUT)/server/view_ipc.o \
	  $(OUT)/server/view1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtview$(MK_SHEXE) \
	  $(OUT)/server/dtview.o \
	  $(OUT)/server/view_ipc.o \
	  $(OUT)/server/view1.o  \
	  -L$(OUT)/libso -ldtserver \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtxchgs \
	  -L$(OUT)/libso -ldtwins \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtscr \
	  -L$(OUT)/libso -ldtcand \
	  -L$(OUT)/libso -ldthws \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_X_LOAD)    $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdtserver.$(MK_LIB): \
	  $(OUT)/server/view2.o
	$(MK_RM) $(OUT)/lib/libdtserver.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtserver.$(MK_LIB) \
	  $(OUT)/server/view2.o
	$(MK_RANLIB) $(OUT)/lib/libdtserver.$(MK_LIB)
$(OUT)/libso/libdtserver.so$(MK_SHVER): \
	  $(OUT)/server/view2.o
	$(MK_RM) $(OUT)/libso/libdtserver.so$(MK_SHVER) $(OUT)/libso/libdtserver.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtserver.so$(MK_SHVER) \
	  $(OUT)/server/view2.o
	ln -s libdtserver.so$(MK_SHVER) $(OUT)/libso/libdtserver.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/server/view2.o: $(SRC)/server/view2.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/view.h \
	  $(SRC)/include/dtack/view.p
	$(MK_CC) -c -o $(OUT)/server/view2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/server \
	  -I$(SRC)/include \
	  $(SRC)/server/view2.c
$(OUT)/server/dtview.o: $(SRC)/server/dtview.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/view.h \
	  $(SRC)/include/dtack/view.p \
	  $(SRC)/include/dtack/main.h
	$(MK_CC) -c -o $(OUT)/server/dtview.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/server \
	  -I$(SRC)/include \
	  $(SRC)/server/dtview.c
$(OUT)/server/view_ipc.o: $(SRC)/server/view_ipc.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
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
	  $(SRC)/server/dtview.use \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/server/view_ipc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/server \
	  -I$(SRC)/include \
	  $(SRC)/server/view_ipc.c
$(OUT)/server/view1.o: $(SRC)/server/view1.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/heap.h \
	  $(SRC)/include/dtack/heap.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/xchg.h \
	  $(SRC)/include/dtack/xchg.p \
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/view.h \
	  $(SRC)/include/dtack/view.p
	$(MK_CC) -c -o $(OUT)/server/view1.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/server \
	  -I$(SRC)/include \
	  $(SRC)/server/view1.c


