
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
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtwins.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtwinc.so$(MK_SHVER) \
	  $(OUT)/libso/libdtwins.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/lib/libdtwinc.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtwins.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtwinc.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtwins.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/win/*.o
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



$(OUT)/lib/libdtwinc.$(MK_LIB): \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/dsk_c.o \
	  $(OUT)/source/win/winc.o \
	  $(OUT)/source/win/winc_ipc.o \
	  $(OUT)/source/win/winc_dsk.o \
	  $(OUT)/source/win/libmain.o
	$(MK_RM) $(OUT)/lib/libdtwinc.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/dsk_c.o \
	  $(OUT)/source/win/winc.o \
	  $(OUT)/source/win/winc_ipc.o \
	  $(OUT)/source/win/winc_dsk.o \
	  $(OUT)/source/win/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtwinc.$(MK_LIB)
$(OUT)/libso/libdtwinc.so$(MK_SHVER): \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/dsk_c.o \
	  $(OUT)/source/win/winc.o \
	  $(OUT)/source/win/winc_ipc.o \
	  $(OUT)/source/win/winc_dsk.o \
	  $(OUT)/source/win/libmain.o
	$(MK_RM) $(OUT)/libso/libdtwinc.so$(MK_SHVER) $(OUT)/libso/libdtwinc.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtwinc.so$(MK_SHVER) \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/dsk_c.o \
	  $(OUT)/source/win/winc.o \
	  $(OUT)/source/win/winc_ipc.o \
	  $(OUT)/source/win/winc_dsk.o \
	  $(OUT)/source/win/libmain.o
	ln -s libdtwinc.so$(MK_SHVER) $(OUT)/libso/libdtwinc.so
$(OUT)/lib/libdtwins.$(MK_LIB): \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/svc.o \
	  $(OUT)/source/win/svc_ipc.o \
	  $(OUT)/source/win/svc_dsk.o \
	  $(OUT)/source/win/wins_ipc.o \
	  $(OUT)/source/win/wins_dsk.o \
	  $(OUT)/source/win/wins.o \
	  $(OUT)/source/win/libmain.o
	$(MK_RM) $(OUT)/lib/libdtwins.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtwins.$(MK_LIB) \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/svc.o \
	  $(OUT)/source/win/svc_ipc.o \
	  $(OUT)/source/win/svc_dsk.o \
	  $(OUT)/source/win/wins_ipc.o \
	  $(OUT)/source/win/wins_dsk.o \
	  $(OUT)/source/win/wins.o \
	  $(OUT)/source/win/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtwins.$(MK_LIB)
$(OUT)/libso/libdtwins.so$(MK_SHVER): \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/svc.o \
	  $(OUT)/source/win/svc_ipc.o \
	  $(OUT)/source/win/svc_dsk.o \
	  $(OUT)/source/win/wins_ipc.o \
	  $(OUT)/source/win/wins_dsk.o \
	  $(OUT)/source/win/wins.o \
	  $(OUT)/source/win/libmain.o
	$(MK_RM) $(OUT)/libso/libdtwins.so$(MK_SHVER) $(OUT)/libso/libdtwins.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtwins.so$(MK_SHVER) \
	  $(OUT)/source/win/dsk.o \
	  $(OUT)/source/win/svc.o \
	  $(OUT)/source/win/svc_ipc.o \
	  $(OUT)/source/win/svc_dsk.o \
	  $(OUT)/source/win/wins_ipc.o \
	  $(OUT)/source/win/wins_dsk.o \
	  $(OUT)/source/win/wins.o \
	  $(OUT)/source/win/libmain.o
	ln -s libdtwins.so$(MK_SHVER) $(OUT)/libso/libdtwins.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/win/dsk.o: $(SRC)/source/win/dsk.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p
	$(MK_CC) -c -o $(OUT)/source/win/dsk.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/dsk.c
$(OUT)/source/win/dsk_c.o: $(SRC)/source/win/dsk_c.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p \
	  $(SRC)/include/dtack/dsk_c.h \
	  $(SRC)/include/dtack/dsk_c.p
	$(MK_CC) -c -o $(OUT)/source/win/dsk_c.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/dsk_c.c
$(OUT)/source/win/winc.o: $(SRC)/source/win/winc.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/winc.h \
	  $(SRC)/include/dtack/winc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/win/winc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/winc.c
$(OUT)/source/win/winc_ipc.o: $(SRC)/source/win/winc_ipc.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/winc.h \
	  $(SRC)/include/dtack/winc.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/win/winc_ipc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/winc_ipc.c
$(OUT)/source/win/winc_dsk.o: $(SRC)/source/win/winc_dsk.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/winc.h \
	  $(SRC)/include/dtack/winc.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/win/winc_dsk.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/winc_dsk.c
$(OUT)/source/win/libmain.o: $(SRC)/source/win/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/win/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/libmain.c
$(OUT)/source/win/svc.o: $(SRC)/source/win/svc.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/win/svc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/svc.c
$(OUT)/source/win/svc_ipc.o: $(SRC)/source/win/svc_ipc.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/win/svc_ipc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/svc_ipc.c
$(OUT)/source/win/svc_dsk.o: $(SRC)/source/win/svc_dsk.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/win/svc_dsk.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/svc_dsk.c
$(OUT)/source/win/wins_ipc.o: $(SRC)/source/win/wins_ipc.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p
	$(MK_CC) -c -o $(OUT)/source/win/wins_ipc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/wins_ipc.c
$(OUT)/source/win/wins_dsk.o: $(SRC)/source/win/wins_dsk.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/dsk.h \
	  $(SRC)/include/dtack/dsk.p \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p
	$(MK_CC) -c -o $(OUT)/source/win/wins_dsk.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/wins_dsk.c
$(OUT)/source/win/wins.o: $(SRC)/source/win/wins.c \
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
	  $(SRC)/include/dtack/win_r.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/wins.h \
	  $(SRC)/include/dtack/wins.p
	$(MK_CC) -c -o $(OUT)/source/win/wins.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/win \
	  -I$(SRC)/include \
	  $(SRC)/source/win/wins.c


