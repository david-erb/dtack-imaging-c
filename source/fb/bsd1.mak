
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
	  $(OUT)/lib/libdtfb.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtfb.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/lib/libdtfb.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtfb.so$(MK_SHVER)
	$(MK_RM) $(OUT)/source/fb/*.o
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



$(OUT)/lib/libdtfb.$(MK_LIB): \
	  $(OUT)/source/fb/fb.o \
	  $(OUT)/source/fb/fbvidmux.o \
	  $(OUT)/source/fb/fbcortex.o \
	  $(OUT)/source/fb/fbdir.o \
	  $(OUT)/source/fb/fbgen.o \
	  $(OUT)/source/fb/fbmeteor.o \
	  $(OUT)/source/fb/fbbt.o \
	  $(OUT)/source/fb/fbvbmci.o \
	  $(OUT)/source/fb/fbvfw.o \
	  $(OUT)/source/fb/fbsnap.o \
	  $(OUT)/source/fb/fbutil.o \
	  $(OUT)/source/fb/libmain.o
	$(MK_RM) $(OUT)/lib/libdtfb.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtfb.$(MK_LIB) \
	  $(OUT)/source/fb/fb.o \
	  $(OUT)/source/fb/fbvidmux.o \
	  $(OUT)/source/fb/fbcortex.o \
	  $(OUT)/source/fb/fbdir.o \
	  $(OUT)/source/fb/fbgen.o \
	  $(OUT)/source/fb/fbmeteor.o \
	  $(OUT)/source/fb/fbbt.o \
	  $(OUT)/source/fb/fbvbmci.o \
	  $(OUT)/source/fb/fbvfw.o \
	  $(OUT)/source/fb/fbsnap.o \
	  $(OUT)/source/fb/fbutil.o \
	  $(OUT)/source/fb/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtfb.$(MK_LIB)
$(OUT)/libso/libdtfb.so$(MK_SHVER): \
	  $(OUT)/source/fb/fb.o \
	  $(OUT)/source/fb/fbvidmux.o \
	  $(OUT)/source/fb/fbcortex.o \
	  $(OUT)/source/fb/fbdir.o \
	  $(OUT)/source/fb/fbgen.o \
	  $(OUT)/source/fb/fbmeteor.o \
	  $(OUT)/source/fb/fbbt.o \
	  $(OUT)/source/fb/fbvbmci.o \
	  $(OUT)/source/fb/fbvfw.o \
	  $(OUT)/source/fb/fbsnap.o \
	  $(OUT)/source/fb/fbutil.o \
	  $(OUT)/source/fb/libmain.o
	$(MK_RM) $(OUT)/libso/libdtfb.so$(MK_SHVER) $(OUT)/libso/libdtfb.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtfb.so$(MK_SHVER) \
	  $(OUT)/source/fb/fb.o \
	  $(OUT)/source/fb/fbvidmux.o \
	  $(OUT)/source/fb/fbcortex.o \
	  $(OUT)/source/fb/fbdir.o \
	  $(OUT)/source/fb/fbgen.o \
	  $(OUT)/source/fb/fbmeteor.o \
	  $(OUT)/source/fb/fbbt.o \
	  $(OUT)/source/fb/fbvbmci.o \
	  $(OUT)/source/fb/fbvfw.o \
	  $(OUT)/source/fb/fbsnap.o \
	  $(OUT)/source/fb/fbutil.o \
	  $(OUT)/source/fb/libmain.o
	ln -s libdtfb.so$(MK_SHVER) $(OUT)/libso/libdtfb.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/fb/fbcortex.o: $(SRC)/source/fb/fbcortex.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/phys.h \
	  $(SRC)/include/dtack/phys.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbcortex.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbcortex.c
$(OUT)/source/fb/fbmeteor.o: $(SRC)/source/fb/fbmeteor.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/fb/fbmeteor.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_METEOR=DT_$(MK_PRJ_METEOR_DEFINE) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbmeteor.c
$(OUT)/source/fb/fbbt.o: $(SRC)/source/fb/fbbt.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/portio.h \
	  $(SRC)/include/dtack/portio.p \
	  $(SRC)/include/dtack/vidmux.h \
	  $(SRC)/include/dtack/vidmux.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/fb/fbbt.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_BTTV=DT_$(MK_PRJ_BTTV_DEFINE) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbbt.c
$(OUT)/source/fb/fbvbmci.o: $(SRC)/source/fb/fbvbmci.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbvbmci.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_VBMCI_DEFINE \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbvbmci.c
$(OUT)/source/fb/fbutil.o: $(SRC)/source/fb/fbutil.c \
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
	  $(SRC)/include/dtack/base.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbutil.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbutil.c
$(OUT)/source/fb/fbvfw.o: $(SRC)/source/fb/fbvfw.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/portio.h \
	  $(SRC)/include/dtack/portio.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbvfw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_VFW_DEFINE \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbvfw.c
$(OUT)/source/fb/fbsnap.o: $(SRC)/source/fb/fbsnap.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/yield.h \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/portio.h \
	  $(SRC)/include/dtack/portio.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbsnap.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_SNAP_DEFINE \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbsnap.c
$(OUT)/source/fb/fb.o: $(SRC)/source/fb/fb.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p
	$(MK_CC) -c -o $(OUT)/source/fb/fb.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fb.c
$(OUT)/source/fb/fbvidmux.o: $(SRC)/source/fb/fbvidmux.c \
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
	  $(SRC)/include/dtack/portio.h \
	  $(SRC)/include/dtack/portio.p \
	  $(SRC)/include/dtack/vidmux.h \
	  $(SRC)/include/dtack/vidmux.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbvidmux.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbvidmux.c
$(OUT)/source/fb/fbdir.o: $(SRC)/source/fb/fbdir.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/phys.h \
	  $(SRC)/include/dtack/phys.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbdir.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbdir.c
$(OUT)/source/fb/fbgen.o: $(SRC)/source/fb/fbgen.c \
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
	  $(SRC)/include/dtack/color.h \
	  $(SRC)/include/dtack/color.p \
	  $(SRC)/include/dtack/fb.h \
	  $(SRC)/include/dtack/fb.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/source/fb/fbgen.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/fbgen.c
$(OUT)/source/fb/libmain.o: $(SRC)/source/fb/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/fb/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/fb \
	  -I$(SRC)/include \
	  $(SRC)/source/fb/libmain.c


