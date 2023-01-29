
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
	  $(OUT)/lib/libdtscr.$(MK_LIB) \
	  $(OUT)/lib/libdtcan.$(MK_LIB) \
	  $(OUT)/lib/libdtcand.$(MK_LIB) \
	  $(OUT)/lib/libdtfastg.$(MK_LIB) \
	  $(OUT)/lib/libdtfastgd.$(MK_LIB) \
	  $(OUT)/lib/libdtsvgac.$(MK_LIB) \
	  $(OUT)/lib/libdtsvgacd.$(MK_LIB) \
	  $(OUT)/lib/libdthws.$(MK_LIB) \
	  $(OUT)/lib/libdthwsd.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtscr.so$(MK_SHVER) \
	  $(OUT)/libso/libdtcan.so$(MK_SHVER) \
	  $(OUT)/libso/libdtcand.so$(MK_SHVER) \
	  $(OUT)/libso/libdtfastg.so$(MK_SHVER) \
	  $(OUT)/libso/libdtfastgd.so$(MK_SHVER) \
	  $(OUT)/libso/libdtsvgac.so$(MK_SHVER) \
	  $(OUT)/libso/libdtsvgacd.so$(MK_SHVER) \
	  $(OUT)/libso/libdthws.so$(MK_SHVER) \
	  $(OUT)/libso/libdthwsd.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/lib/libdtscr.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtcan.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtcand.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtfastg.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtfastgd.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtsvgac.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtsvgacd.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdthws.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdthwsd.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtscr.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtcan.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtcand.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtfastg.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtfastgd.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtsvgac.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtsvgacd.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdthws.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdthwsd.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/screen/*.o
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



$(OUT)/lib/libdtscr.$(MK_LIB): \
	  $(OUT)/source/screen/scr_main.o \
	  $(OUT)/source/screen/scr_cur.o \
	  $(OUT)/source/screen/scr_act.o \
	  $(OUT)/source/screen/scr_dbg.o \
	  $(OUT)/source/screen/scrdum.o \
	  $(OUT)/source/screen/vga.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtscr.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtscr.$(MK_LIB) \
	  $(OUT)/source/screen/scr_main.o \
	  $(OUT)/source/screen/scr_cur.o \
	  $(OUT)/source/screen/scr_act.o \
	  $(OUT)/source/screen/scr_dbg.o \
	  $(OUT)/source/screen/scrdum.o \
	  $(OUT)/source/screen/vga.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtscr.$(MK_LIB)
$(OUT)/libso/libdtscr.so$(MK_SHVER): \
	  $(OUT)/source/screen/scr_main.o \
	  $(OUT)/source/screen/scr_cur.o \
	  $(OUT)/source/screen/scr_act.o \
	  $(OUT)/source/screen/scr_dbg.o \
	  $(OUT)/source/screen/scrdum.o \
	  $(OUT)/source/screen/vga.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtscr.so$(MK_SHVER) $(OUT)/libso/libdtscr.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtscr.so$(MK_SHVER) \
	  $(OUT)/source/screen/scr_main.o \
	  $(OUT)/source/screen/scr_cur.o \
	  $(OUT)/source/screen/scr_act.o \
	  $(OUT)/source/screen/scr_dbg.o \
	  $(OUT)/source/screen/scrdum.o \
	  $(OUT)/source/screen/vga.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtscr.so$(MK_SHVER) $(OUT)/libso/libdtscr.so
$(OUT)/lib/libdtcan.$(MK_LIB): \
	  $(OUT)/source/screen/scrcan.o \
	  $(OUT)/source/screen/canvas.o \
	  $(OUT)/source/screen/canpoll.o \
	  $(OUT)/source/screen/candum.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtcan.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtcan.$(MK_LIB) \
	  $(OUT)/source/screen/scrcan.o \
	  $(OUT)/source/screen/canvas.o \
	  $(OUT)/source/screen/canpoll.o \
	  $(OUT)/source/screen/candum.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtcan.$(MK_LIB)
$(OUT)/libso/libdtcan.so$(MK_SHVER): \
	  $(OUT)/source/screen/scrcan.o \
	  $(OUT)/source/screen/canvas.o \
	  $(OUT)/source/screen/canpoll.o \
	  $(OUT)/source/screen/candum.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtcan.so$(MK_SHVER) $(OUT)/libso/libdtcan.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtcan.so$(MK_SHVER) \
	  $(OUT)/source/screen/scrcan.o \
	  $(OUT)/source/screen/canvas.o \
	  $(OUT)/source/screen/canpoll.o \
	  $(OUT)/source/screen/candum.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtcan.so$(MK_SHVER) $(OUT)/libso/libdtcan.so
$(OUT)/lib/libdtcand.$(MK_LIB): \
	  $(OUT)/source/screen/scrcand.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtcand.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtcand.$(MK_LIB) \
	  $(OUT)/source/screen/scrcand.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtcand.$(MK_LIB)
$(OUT)/libso/libdtcand.so$(MK_SHVER): \
	  $(OUT)/source/screen/scrcand.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtcand.so$(MK_SHVER) $(OUT)/libso/libdtcand.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtcand.so$(MK_SHVER) \
	  $(OUT)/source/screen/scrcand.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtcand.so$(MK_SHVER) $(OUT)/libso/libdtcand.so
$(OUT)/lib/libdtfastg.$(MK_LIB): \
	  $(OUT)/source/screen/canfastg.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtfastg.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtfastg.$(MK_LIB) \
	  $(OUT)/source/screen/canfastg.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtfastg.$(MK_LIB)
$(OUT)/libso/libdtfastg.so$(MK_SHVER): \
	  $(OUT)/source/screen/canfastg.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtfastg.so$(MK_SHVER) $(OUT)/libso/libdtfastg.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtfastg.so$(MK_SHVER) \
	  $(OUT)/source/screen/canfastg.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtfastg.so$(MK_SHVER) $(OUT)/libso/libdtfastg.so
$(OUT)/lib/libdtfastgd.$(MK_LIB): \
	  $(OUT)/source/screen/canfastd.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtfastgd.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtfastgd.$(MK_LIB) \
	  $(OUT)/source/screen/canfastd.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtfastgd.$(MK_LIB)
$(OUT)/libso/libdtfastgd.so$(MK_SHVER): \
	  $(OUT)/source/screen/canfastd.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtfastgd.so$(MK_SHVER) $(OUT)/libso/libdtfastgd.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtfastgd.so$(MK_SHVER) \
	  $(OUT)/source/screen/canfastd.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtfastgd.so$(MK_SHVER) $(OUT)/libso/libdtfastgd.so
$(OUT)/lib/libdtsvgac.$(MK_LIB): \
	  $(OUT)/source/screen/cansvgac.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtsvgac.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtsvgac.$(MK_LIB) \
	  $(OUT)/source/screen/cansvgac.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtsvgac.$(MK_LIB)
$(OUT)/libso/libdtsvgac.so$(MK_SHVER): \
	  $(OUT)/source/screen/cansvgac.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtsvgac.so$(MK_SHVER) $(OUT)/libso/libdtsvgac.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtsvgac.so$(MK_SHVER) \
	  $(OUT)/source/screen/cansvgac.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtsvgac.so$(MK_SHVER) $(OUT)/libso/libdtsvgac.so
$(OUT)/lib/libdtsvgacd.$(MK_LIB): \
	  $(OUT)/source/screen/cansvgad.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdtsvgacd.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtsvgacd.$(MK_LIB) \
	  $(OUT)/source/screen/cansvgad.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtsvgacd.$(MK_LIB)
$(OUT)/libso/libdtsvgacd.so$(MK_SHVER): \
	  $(OUT)/source/screen/cansvgad.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdtsvgacd.so$(MK_SHVER) $(OUT)/libso/libdtsvgacd.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtsvgacd.so$(MK_SHVER) \
	  $(OUT)/source/screen/cansvgad.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdtsvgacd.so$(MK_SHVER) $(OUT)/libso/libdtsvgacd.so
$(OUT)/lib/libdthws.$(MK_LIB): \
	  $(OUT)/source/screen/scrhws.o \
	  $(OUT)/source/screen/canhws.o \
	  $(OUT)/source/screen/hws.o \
	  $(OUT)/source/screen/hwsx.o \
	  $(OUT)/source/screen/hwsqw4.o \
	  $(OUT)/source/screen/hwsfgw.o \
	  $(OUT)/source/screen/hwsmsw.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdthws.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdthws.$(MK_LIB) \
	  $(OUT)/source/screen/scrhws.o \
	  $(OUT)/source/screen/canhws.o \
	  $(OUT)/source/screen/hws.o \
	  $(OUT)/source/screen/hwsx.o \
	  $(OUT)/source/screen/hwsqw4.o \
	  $(OUT)/source/screen/hwsfgw.o \
	  $(OUT)/source/screen/hwsmsw.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdthws.$(MK_LIB)
$(OUT)/libso/libdthws.so$(MK_SHVER): \
	  $(OUT)/source/screen/scrhws.o \
	  $(OUT)/source/screen/canhws.o \
	  $(OUT)/source/screen/hws.o \
	  $(OUT)/source/screen/hwsx.o \
	  $(OUT)/source/screen/hwsqw4.o \
	  $(OUT)/source/screen/hwsfgw.o \
	  $(OUT)/source/screen/hwsmsw.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdthws.so$(MK_SHVER) $(OUT)/libso/libdthws.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdthws.so$(MK_SHVER) \
	  $(OUT)/source/screen/scrhws.o \
	  $(OUT)/source/screen/canhws.o \
	  $(OUT)/source/screen/hws.o \
	  $(OUT)/source/screen/hwsx.o \
	  $(OUT)/source/screen/hwsqw4.o \
	  $(OUT)/source/screen/hwsfgw.o \
	  $(OUT)/source/screen/hwsmsw.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdthws.so$(MK_SHVER) $(OUT)/libso/libdthws.so
$(OUT)/lib/libdthwsd.$(MK_LIB): \
	  $(OUT)/source/screen/scrhwsd.o \
	  $(OUT)/source/screen/canhwsd.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/lib/libdthwsd.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdthwsd.$(MK_LIB) \
	  $(OUT)/source/screen/scrhwsd.o \
	  $(OUT)/source/screen/canhwsd.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdthwsd.$(MK_LIB)
$(OUT)/libso/libdthwsd.so$(MK_SHVER): \
	  $(OUT)/source/screen/scrhwsd.o \
	  $(OUT)/source/screen/canhwsd.o \
	  $(OUT)/source/screen/libmain.o
	$(MK_RM) $(OUT)/libso/libdthwsd.so$(MK_SHVER) $(OUT)/libso/libdthwsd.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdthwsd.so$(MK_SHVER) \
	  $(OUT)/source/screen/scrhwsd.o \
	  $(OUT)/source/screen/canhwsd.o \
	  $(OUT)/source/screen/libmain.o
	ln -s libdthwsd.so$(MK_SHVER) $(OUT)/libso/libdthwsd.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/screen/canfastg.o: $(SRC)/source/screen/canfastg.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p \
	  $(SRC)/include/dtack/vga.h \
	  $(SRC)/include/dtack/vga.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/nofar.h \
	  $(SRC)/include/dtack/fastg.h
	$(MK_CC) -c -o $(OUT)/source/screen/canfastg.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/canfastg.c
$(OUT)/source/screen/cansvgac.o: $(SRC)/source/screen/cansvgac.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p \
	  $(SRC)/include/dtack/vga.h \
	  $(SRC)/include/dtack/vga.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/nofar.h \
	  $(SRC)/include/dtack/svgacc.h
	$(MK_CC) -c -o $(OUT)/source/screen/cansvgac.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/cansvgac.c
$(OUT)/source/screen/hwsx.o: $(SRC)/source/screen/hwsx.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/screen/hwsx.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_X=DT_$(MK_PRJ_X_DEFINE) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  -I$(MK_PRJ_X_INCLUDE) \
	  $(SRC)/source/screen/hwsx.c
$(OUT)/source/screen/hwsqw4.o: $(SRC)/source/screen/hwsqw4.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/screen/hwsqw4.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/hwsqw4.c
$(OUT)/source/screen/hwsfgw.o: $(SRC)/source/screen/hwsfgw.c \
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
	  $(SRC)/include/dtack/label.h \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/screen/hwsfgw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  -I. \
	  $(SRC)/source/screen/hwsfgw.c
$(OUT)/source/screen/scr_main.o: $(SRC)/source/screen/scr_main.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/font.h \
	  $(SRC)/include/dtack/font.p
	$(MK_CC) -c -o $(OUT)/source/screen/scr_main.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scr_main.c
$(OUT)/source/screen/scr_cur.o: $(SRC)/source/screen/scr_cur.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/screen/scr_cur.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scr_cur.c
$(OUT)/source/screen/scr_act.o: $(SRC)/source/screen/scr_act.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/screen/scr_act.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scr_act.c
$(OUT)/source/screen/scr_dbg.o: $(SRC)/source/screen/scr_dbg.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p
	$(MK_CC) -c -o $(OUT)/source/screen/scr_dbg.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scr_dbg.c
$(OUT)/source/screen/scrdum.o: $(SRC)/source/screen/scrdum.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/screen/scrdum.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scrdum.c
$(OUT)/source/screen/vga.o: $(SRC)/source/screen/vga.c \
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
	  $(SRC)/include/dtack/vga.h \
	  $(SRC)/include/dtack/vga.p
	$(MK_CC) -c -o $(OUT)/source/screen/vga.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/vga.c
$(OUT)/source/screen/libmain.o: $(SRC)/source/screen/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/screen/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/libmain.c
$(OUT)/source/screen/scrcan.o: $(SRC)/source/screen/scrcan.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/screen/scrcan.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scrcan.c
$(OUT)/source/screen/canvas.o: $(SRC)/source/screen/canvas.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p
	$(MK_CC) -c -o $(OUT)/source/screen/canvas.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/canvas.c
$(OUT)/source/screen/canpoll.o: $(SRC)/source/screen/canpoll.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p
	$(MK_CC) -c -o $(OUT)/source/screen/canpoll.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/canpoll.c
$(OUT)/source/screen/candum.o: $(SRC)/source/screen/candum.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p
	$(MK_CC) -c -o $(OUT)/source/screen/candum.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/candum.c
$(OUT)/source/screen/scrcand.o: $(SRC)/source/screen/scrcand.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p
	$(MK_CC) -c -o $(OUT)/source/screen/scrcand.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scrcand.c
$(OUT)/source/screen/canfastd.o: $(SRC)/source/screen/canfastd.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p
	$(MK_CC) -c -o $(OUT)/source/screen/canfastd.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/canfastd.c
$(OUT)/source/screen/cansvgad.o: $(SRC)/source/screen/cansvgad.c \
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
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p
	$(MK_CC) -c -o $(OUT)/source/screen/cansvgad.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/cansvgad.c
$(OUT)/source/screen/scrhws.o: $(SRC)/source/screen/scrhws.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/screen/scrhws.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scrhws.c
$(OUT)/source/screen/canhws.o: $(SRC)/source/screen/canhws.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/screen/canhws.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/canhws.c
$(OUT)/source/screen/hws.o: $(SRC)/source/screen/hws.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/screen/hws.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/hws.c
$(OUT)/source/screen/hwsmsw.o: $(SRC)/source/screen/hwsmsw.c \
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
	  $(SRC)/include/dtack/label.h \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/hws.h \
	  $(SRC)/include/dtack/hws.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/screen/hwsmsw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/hwsmsw.c
$(OUT)/source/screen/scrhwsd.o: $(SRC)/source/screen/scrhwsd.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/scr.h \
	  $(SRC)/include/dtack/scr.p
	$(MK_CC) -c -o $(OUT)/source/screen/scrhwsd.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/scrhwsd.c
$(OUT)/source/screen/canhwsd.o: $(SRC)/source/screen/canhwsd.c \
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
	  $(SRC)/include/dtack/lut8.h \
	  $(SRC)/include/dtack/lut8.p \
	  $(SRC)/include/dtack/keymap.h \
	  $(SRC)/include/dtack/keymap.p \
	  $(SRC)/include/dtack/keybd.h \
	  $(SRC)/include/dtack/keybd.p \
	  $(SRC)/include/dtack/svc.h \
	  $(SRC)/include/dtack/svc.p \
	  $(SRC)/include/dtack/canvas.h \
	  $(SRC)/include/dtack/canvas.p
	$(MK_CC) -c -o $(OUT)/source/screen/canhwsd.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/screen \
	  -I$(SRC)/include \
	  $(SRC)/source/screen/canhwsd.c


