
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
	  $(OUT)/bin/dtenv1$(MK_EXE) \
	  $(OUT)/bin/dtenv2$(MK_EXE) \
	  $(OUT)/bin/dtenv3$(MK_EXE) \
	  $(OUT)/bin/dtenv4$(MK_EXE) \
	  $(OUT)/bin/dthist1$(MK_EXE) \
	  $(OUT)/bin/dthist2$(MK_EXE) \
	  $(OUT)/bin/dtparam1$(MK_EXE) \
	  $(OUT)/bin/dtport1$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtenv1$(MK_SHEXE) \
	  $(OUT)/bin/dtenv2$(MK_SHEXE) \
	  $(OUT)/bin/dtenv3$(MK_SHEXE) \
	  $(OUT)/bin/dtenv4$(MK_SHEXE) \
	  $(OUT)/bin/dthist1$(MK_SHEXE) \
	  $(OUT)/bin/dthist2$(MK_SHEXE) \
	  $(OUT)/bin/dtparam1$(MK_SHEXE) \
	  $(OUT)/bin/dtport1$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdtlow.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtlow.so$(MK_SHVER) \
	  $(OUT)/libso/libdtlow.sa$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtenv1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtenv2$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv2$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtenv3$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv3$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtenv4$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv4$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dthist1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dthist1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dthist2$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dthist2$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtparam1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtparam1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtport1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtport1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtenv1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv1$(MK_EXE); fi
	if [ -w $(OUT)/bin/dtenv2$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv2$(MK_EXE); fi
	if [ -w $(OUT)/bin/dtenv3$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv3$(MK_EXE); fi
	if [ -w $(OUT)/bin/dtenv4$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtenv4$(MK_EXE); fi
	if [ -w $(OUT)/bin/dthist1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dthist1$(MK_EXE); fi
	if [ -w $(OUT)/bin/dthist2$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dthist2$(MK_EXE); fi
	if [ -w $(OUT)/bin/dtparam1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtparam1$(MK_EXE); fi
	if [ -w $(OUT)/bin/dtport1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtport1$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtenv1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtenv2$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtenv3$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtenv4$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dthist1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dthist2$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtparam1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtport1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtenv1$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dtenv2$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dtenv3$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dtenv4$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dthist1$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dthist2$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dtparam1$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dtport1$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdtlow.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtlow.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtlow.sa$(MK_SHVER)
	$(MK_RM) $(OUT)/source/low/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtenv1$(MK_EXE): \
	  $(OUT)/source/low/dtenv1.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtenv1$(MK_EXE) \
	  $(OUT)/source/low/dtenv1.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv1$(MK_SHEXE): $(OUT)/source/low/dtenv1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtenv1$(MK_SHEXE) \
	  $(OUT)/source/low/dtenv1.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv2$(MK_EXE): \
	  $(OUT)/source/low/dtenv2.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtenv2$(MK_EXE) \
	  $(OUT)/source/low/dtenv2.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv2$(MK_SHEXE): $(OUT)/source/low/dtenv2.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtenv2$(MK_SHEXE) \
	  $(OUT)/source/low/dtenv2.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv3$(MK_EXE): \
	  $(OUT)/source/low/dtenv3.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtenv3$(MK_EXE) \
	  $(OUT)/source/low/dtenv3.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv3$(MK_SHEXE): $(OUT)/source/low/dtenv3.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtenv3$(MK_SHEXE) \
	  $(OUT)/source/low/dtenv3.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv4$(MK_EXE): \
	  $(OUT)/source/low/dtenv4.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtenv4$(MK_EXE) \
	  $(OUT)/source/low/dtenv4.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtenv4$(MK_SHEXE): $(OUT)/source/low/dtenv4.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtenv4$(MK_SHEXE) \
	  $(OUT)/source/low/dtenv4.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dthist1$(MK_EXE): \
	  $(OUT)/source/low/dthist1.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dthist1$(MK_EXE) \
	  $(OUT)/source/low/dthist1.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dthist1$(MK_SHEXE): $(OUT)/source/low/dthist1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dthist1$(MK_SHEXE) \
	  $(OUT)/source/low/dthist1.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dthist2$(MK_EXE): \
	  $(OUT)/source/low/dthist2.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dthist2$(MK_EXE) \
	  $(OUT)/source/low/dthist2.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dthist2$(MK_SHEXE): $(OUT)/source/low/dthist2.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dthist2$(MK_SHEXE) \
	  $(OUT)/source/low/dthist2.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtparam1$(MK_EXE): \
	  $(OUT)/source/low/dtparam1.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtparam1$(MK_EXE) \
	  $(OUT)/source/low/dtparam1.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtparam1$(MK_SHEXE): $(OUT)/source/low/dtparam1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtparam1$(MK_SHEXE) \
	  $(OUT)/source/low/dtparam1.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtport1$(MK_EXE): \
	  $(OUT)/source/low/dtport1.o \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtport1$(MK_EXE) \
	  $(OUT)/source/low/dtport1.o \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtport1$(MK_SHEXE): $(OUT)/source/low/dtport1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtport1$(MK_SHEXE) \
	  $(OUT)/source/low/dtport1.o  \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdtlow.$(MK_LIB): \
	  $(OUT)/source/low/dbtable.o \
	  $(OUT)/source/low/points.o \
	  $(OUT)/source/low/poly.o \
	  $(OUT)/source/low/matrix3d.o \
	  $(OUT)/source/low/matrix2d.o \
	  $(OUT)/source/low/matrix.o \
	  $(OUT)/source/low/assert.o \
	  $(OUT)/source/low/cipher.o \
	  $(OUT)/source/low/cons.o \
	  $(OUT)/source/low/cstack.o \
	  $(OUT)/source/low/dbg.o \
	  $(OUT)/source/low/dbg2.o \
	  $(OUT)/source/low/err.o \
	  $(OUT)/source/low/err2.o \
	  $(OUT)/source/low/enter.o \
	  $(OUT)/source/low/eventw.o \
	  $(OUT)/source/low/f.o \
	  $(OUT)/source/low/fd.o \
	  $(OUT)/source/low/ftp.o \
	  $(OUT)/source/low/heapmon.o \
	  $(OUT)/source/low/heap2.o \
	  $(OUT)/source/low/hist.o \
	  $(OUT)/source/low/hist2.o \
	  $(OUT)/source/low/hist_da.o \
	  $(OUT)/source/low/ipc.o \
	  $(OUT)/source/low/ipcqnx4.o \
	  $(OUT)/source/low/ipcqbaby.o \
	  $(OUT)/source/low/ipcrock.o \
	  $(OUT)/source/low/ipcdde.o \
	  $(OUT)/source/low/leave.o \
	  $(OUT)/source/low/license.o \
	  $(OUT)/source/low/liner.o \
	  $(OUT)/source/low/list.o \
	  $(OUT)/source/low/list_io.o \
	  $(OUT)/source/low/lut8.o \
	  $(OUT)/source/low/lut8_mat.o \
	  $(OUT)/source/low/msgu.o \
	  $(OUT)/source/low/msw.o \
	  $(OUT)/source/low/os_mem.o \
	  $(OUT)/source/low/os_delay.o \
	  $(OUT)/source/low/os_futil.o \
	  $(OUT)/source/low/os_heap.o \
	  $(OUT)/source/low/os_heap2.o \
	  $(OUT)/source/low/os_time.o \
	  $(OUT)/source/low/os_env.o \
	  $(OUT)/source/low/param.o \
	  $(OUT)/source/low/phys.o \
	  $(OUT)/source/low/pnm.o \
	  $(OUT)/source/low/portio.o \
	  $(OUT)/source/low/prj.o \
	  $(OUT)/source/low/qnx4.o \
	  $(OUT)/source/low/rev.o \
	  $(OUT)/source/low/say.o \
	  $(OUT)/source/low/semu.o \
	  $(OUT)/source/low/semw.o \
	  $(OUT)/source/low/serial.o \
	  $(OUT)/source/low/serialu.o \
	  $(OUT)/source/low/serialw.o \
	  $(OUT)/source/low/shmu.o \
	  $(OUT)/source/low/shmw.o \
	  $(OUT)/source/low/str.o \
	  $(OUT)/source/low/threads.o \
	  $(OUT)/source/low/thread.o \
	  $(OUT)/source/low/threadu.o \
	  $(OUT)/source/low/threadw.o \
	  $(OUT)/source/low/usage.o \
	  $(OUT)/source/low/yield.o \
	  $(OUT)/source/low/libmain.o \
	  $(OUT)/source/low/dbg_dat.o \
	  $(OUT)/source/low/conf_dat.o \
	  $(OUT)/source/low/lut8_dat.o \
	  $(OUT)/source/low/ntype.o \
	  $(OUT)/source/low/ntype_da.o \
	  $(OUT)/source/low/prj_dat.o
	$(MK_RM) $(OUT)/lib/libdtlow.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/source/low/dbtable.o \
	  $(OUT)/source/low/points.o \
	  $(OUT)/source/low/poly.o \
	  $(OUT)/source/low/matrix3d.o \
	  $(OUT)/source/low/matrix2d.o \
	  $(OUT)/source/low/matrix.o \
	  $(OUT)/source/low/assert.o \
	  $(OUT)/source/low/cipher.o \
	  $(OUT)/source/low/cons.o \
	  $(OUT)/source/low/cstack.o \
	  $(OUT)/source/low/dbg.o \
	  $(OUT)/source/low/dbg2.o \
	  $(OUT)/source/low/err.o \
	  $(OUT)/source/low/err2.o \
	  $(OUT)/source/low/enter.o \
	  $(OUT)/source/low/eventw.o \
	  $(OUT)/source/low/f.o \
	  $(OUT)/source/low/fd.o \
	  $(OUT)/source/low/ftp.o \
	  $(OUT)/source/low/heapmon.o \
	  $(OUT)/source/low/heap2.o \
	  $(OUT)/source/low/hist.o \
	  $(OUT)/source/low/hist2.o \
	  $(OUT)/source/low/hist_da.o \
	  $(OUT)/source/low/ipc.o \
	  $(OUT)/source/low/ipcqnx4.o \
	  $(OUT)/source/low/ipcqbaby.o \
	  $(OUT)/source/low/ipcrock.o \
	  $(OUT)/source/low/ipcdde.o \
	  $(OUT)/source/low/leave.o \
	  $(OUT)/source/low/license.o \
	  $(OUT)/source/low/liner.o \
	  $(OUT)/source/low/list.o \
	  $(OUT)/source/low/list_io.o \
	  $(OUT)/source/low/lut8.o \
	  $(OUT)/source/low/lut8_mat.o \
	  $(OUT)/source/low/msgu.o \
	  $(OUT)/source/low/msw.o \
	  $(OUT)/source/low/os_mem.o \
	  $(OUT)/source/low/os_delay.o \
	  $(OUT)/source/low/os_futil.o \
	  $(OUT)/source/low/os_heap.o \
	  $(OUT)/source/low/os_heap2.o \
	  $(OUT)/source/low/os_time.o \
	  $(OUT)/source/low/os_env.o \
	  $(OUT)/source/low/param.o \
	  $(OUT)/source/low/phys.o \
	  $(OUT)/source/low/pnm.o \
	  $(OUT)/source/low/portio.o \
	  $(OUT)/source/low/prj.o \
	  $(OUT)/source/low/qnx4.o \
	  $(OUT)/source/low/rev.o \
	  $(OUT)/source/low/say.o \
	  $(OUT)/source/low/semu.o \
	  $(OUT)/source/low/semw.o \
	  $(OUT)/source/low/serial.o \
	  $(OUT)/source/low/serialu.o \
	  $(OUT)/source/low/serialw.o \
	  $(OUT)/source/low/shmu.o \
	  $(OUT)/source/low/shmw.o \
	  $(OUT)/source/low/str.o \
	  $(OUT)/source/low/threads.o \
	  $(OUT)/source/low/thread.o \
	  $(OUT)/source/low/threadu.o \
	  $(OUT)/source/low/threadw.o \
	  $(OUT)/source/low/usage.o \
	  $(OUT)/source/low/yield.o \
	  $(OUT)/source/low/libmain.o \
	  $(OUT)/source/low/dbg_dat.o \
	  $(OUT)/source/low/conf_dat.o \
	  $(OUT)/source/low/lut8_dat.o \
	  $(OUT)/source/low/ntype.o \
	  $(OUT)/source/low/ntype_da.o \
	  $(OUT)/source/low/prj_dat.o
	$(MK_RANLIB) $(OUT)/lib/libdtlow.$(MK_LIB)
$(OUT)/libso/libdtlow.so$(MK_SHVER): \
	  $(OUT)/source/low/dbtable.o \
	  $(OUT)/source/low/points.o \
	  $(OUT)/source/low/poly.o \
	  $(OUT)/source/low/matrix3d.o \
	  $(OUT)/source/low/matrix2d.o \
	  $(OUT)/source/low/matrix.o \
	  $(OUT)/source/low/assert.o \
	  $(OUT)/source/low/cipher.o \
	  $(OUT)/source/low/cons.o \
	  $(OUT)/source/low/cstack.o \
	  $(OUT)/source/low/dbg.o \
	  $(OUT)/source/low/dbg2.o \
	  $(OUT)/source/low/err.o \
	  $(OUT)/source/low/err2.o \
	  $(OUT)/source/low/enter.o \
	  $(OUT)/source/low/eventw.o \
	  $(OUT)/source/low/f.o \
	  $(OUT)/source/low/fd.o \
	  $(OUT)/source/low/ftp.o \
	  $(OUT)/source/low/heapmon.o \
	  $(OUT)/source/low/heap2.o \
	  $(OUT)/source/low/hist.o \
	  $(OUT)/source/low/hist2.o \
	  $(OUT)/source/low/hist_da.o \
	  $(OUT)/source/low/ipc.o \
	  $(OUT)/source/low/ipcqnx4.o \
	  $(OUT)/source/low/ipcqbaby.o \
	  $(OUT)/source/low/ipcrock.o \
	  $(OUT)/source/low/ipcdde.o \
	  $(OUT)/source/low/leave.o \
	  $(OUT)/source/low/license.o \
	  $(OUT)/source/low/liner.o \
	  $(OUT)/source/low/list.o \
	  $(OUT)/source/low/list_io.o \
	  $(OUT)/source/low/lut8.o \
	  $(OUT)/source/low/lut8_mat.o \
	  $(OUT)/source/low/msgu.o \
	  $(OUT)/source/low/msw.o \
	  $(OUT)/source/low/os_mem.o \
	  $(OUT)/source/low/os_delay.o \
	  $(OUT)/source/low/os_futil.o \
	  $(OUT)/source/low/os_heap.o \
	  $(OUT)/source/low/os_heap2.o \
	  $(OUT)/source/low/os_time.o \
	  $(OUT)/source/low/os_env.o \
	  $(OUT)/source/low/param.o \
	  $(OUT)/source/low/phys.o \
	  $(OUT)/source/low/pnm.o \
	  $(OUT)/source/low/portio.o \
	  $(OUT)/source/low/prj.o \
	  $(OUT)/source/low/qnx4.o \
	  $(OUT)/source/low/rev.o \
	  $(OUT)/source/low/say.o \
	  $(OUT)/source/low/semu.o \
	  $(OUT)/source/low/semw.o \
	  $(OUT)/source/low/serial.o \
	  $(OUT)/source/low/serialu.o \
	  $(OUT)/source/low/serialw.o \
	  $(OUT)/source/low/shmu.o \
	  $(OUT)/source/low/shmw.o \
	  $(OUT)/source/low/str.o \
	  $(OUT)/source/low/threads.o \
	  $(OUT)/source/low/thread.o \
	  $(OUT)/source/low/threadu.o \
	  $(OUT)/source/low/threadw.o \
	  $(OUT)/source/low/usage.o \
	  $(OUT)/source/low/yield.o \
	  $(OUT)/source/low/libmain.o \
	  $(OUT)/source/low/dbg_dat.o \
	  $(OUT)/source/low/conf_dat.o \
	  $(OUT)/source/low/lut8_dat.o \
	  $(OUT)/source/low/ntype.o \
	  $(OUT)/source/low/ntype_da.o \
	  $(OUT)/source/low/prj_dat.o
	$(MK_RM) $(OUT)/libso/libdtlow.so$(MK_SHVER) $(OUT)/libso/libdtlow.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtlow.so$(MK_SHVER) \
	  $(OUT)/source/low/dbtable.o \
	  $(OUT)/source/low/points.o \
	  $(OUT)/source/low/poly.o \
	  $(OUT)/source/low/matrix3d.o \
	  $(OUT)/source/low/matrix2d.o \
	  $(OUT)/source/low/matrix.o \
	  $(OUT)/source/low/assert.o \
	  $(OUT)/source/low/cipher.o \
	  $(OUT)/source/low/cons.o \
	  $(OUT)/source/low/cstack.o \
	  $(OUT)/source/low/dbg.o \
	  $(OUT)/source/low/dbg2.o \
	  $(OUT)/source/low/err.o \
	  $(OUT)/source/low/err2.o \
	  $(OUT)/source/low/enter.o \
	  $(OUT)/source/low/eventw.o \
	  $(OUT)/source/low/f.o \
	  $(OUT)/source/low/fd.o \
	  $(OUT)/source/low/ftp.o \
	  $(OUT)/source/low/heapmon.o \
	  $(OUT)/source/low/heap2.o \
	  $(OUT)/source/low/hist.o \
	  $(OUT)/source/low/hist2.o \
	  $(OUT)/source/low/hist_da.o \
	  $(OUT)/source/low/ipc.o \
	  $(OUT)/source/low/ipcqnx4.o \
	  $(OUT)/source/low/ipcqbaby.o \
	  $(OUT)/source/low/ipcrock.o \
	  $(OUT)/source/low/ipcdde.o \
	  $(OUT)/source/low/leave.o \
	  $(OUT)/source/low/license.o \
	  $(OUT)/source/low/liner.o \
	  $(OUT)/source/low/list.o \
	  $(OUT)/source/low/list_io.o \
	  $(OUT)/source/low/lut8.o \
	  $(OUT)/source/low/lut8_mat.o \
	  $(OUT)/source/low/msgu.o \
	  $(OUT)/source/low/msw.o \
	  $(OUT)/source/low/os_mem.o \
	  $(OUT)/source/low/os_delay.o \
	  $(OUT)/source/low/os_futil.o \
	  $(OUT)/source/low/os_heap.o \
	  $(OUT)/source/low/os_heap2.o \
	  $(OUT)/source/low/os_time.o \
	  $(OUT)/source/low/os_env.o \
	  $(OUT)/source/low/param.o \
	  $(OUT)/source/low/phys.o \
	  $(OUT)/source/low/pnm.o \
	  $(OUT)/source/low/portio.o \
	  $(OUT)/source/low/prj.o \
	  $(OUT)/source/low/qnx4.o \
	  $(OUT)/source/low/rev.o \
	  $(OUT)/source/low/say.o \
	  $(OUT)/source/low/semu.o \
	  $(OUT)/source/low/semw.o \
	  $(OUT)/source/low/serial.o \
	  $(OUT)/source/low/serialu.o \
	  $(OUT)/source/low/serialw.o \
	  $(OUT)/source/low/shmu.o \
	  $(OUT)/source/low/shmw.o \
	  $(OUT)/source/low/str.o \
	  $(OUT)/source/low/threads.o \
	  $(OUT)/source/low/thread.o \
	  $(OUT)/source/low/threadu.o \
	  $(OUT)/source/low/threadw.o \
	  $(OUT)/source/low/usage.o \
	  $(OUT)/source/low/yield.o \
	  $(OUT)/source/low/libmain.o \
	  $(OUT)/source/low/dbg_dat.o \
	  $(OUT)/source/low/conf_dat.o \
	  $(OUT)/source/low/lut8_dat.o \
	  $(OUT)/source/low/ntype.o \
	  $(OUT)/source/low/ntype_da.o \
	  $(OUT)/source/low/prj_dat.o
	ln -s libdtlow.so$(MK_SHVER) $(OUT)/libso/libdtlow.so
$(OUT)/libso/libdtlow.sa$(MK_SHVER): \
	  $(OUT)/source/low/dbg_dat.o \
	  $(OUT)/source/low/conf_dat.o \
	  $(OUT)/source/low/lut8_dat.o \
	  $(OUT)/source/low/ntype.o \
	  $(OUT)/source/low/ntype_da.o \
	  $(OUT)/source/low/prj_dat.o
	$(MK_RM) $(OUT)/libso/libdtlow.sa$(MK_SHVER) $(OUT)/libso/libdtlow.sa
	$(MK_AR) $(OUT)/libso/libdtlow.sa$(MK_SHVER) \
	  $(OUT)/source/low/dbg_dat.o \
	  $(OUT)/source/low/conf_dat.o \
	  $(OUT)/source/low/lut8_dat.o \
	  $(OUT)/source/low/ntype.o \
	  $(OUT)/source/low/ntype_da.o \
	  $(OUT)/source/low/prj_dat.o
	$(MK_RANLIB) $(OUT)/libso/libdtlow.sa$(MK_SHVER)
	ln -s libdtlow.sa$(MK_SHVER) $(OUT)/libso/libdtlow.sa

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/source/low/dtenv1.o: $(SRC)/source/low/dtenv1.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dtenv1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dtenv1.c
$(OUT)/source/low/dtenv2.o: $(SRC)/source/low/dtenv2.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dtenv2.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dtenv2.c
$(OUT)/source/low/dtenv3.o: $(SRC)/source/low/dtenv3.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dtenv3.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dtenv3.c
$(OUT)/source/low/dtenv4.o: $(SRC)/source/low/dtenv4.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dtenv4.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dtenv4.c
$(OUT)/source/low/dthist1.o: $(SRC)/source/low/dthist1.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dthist1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dthist1.c
$(OUT)/source/low/dthist2.o: $(SRC)/source/low/dthist2.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dthist2.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dthist2.c
$(OUT)/source/low/dtparam1.o: $(SRC)/source/low/dtparam1.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dtparam1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dtparam1.c
$(OUT)/source/low/dtport1.o: $(SRC)/source/low/dtport1.c \
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
	  $(SRC)/include/dtack/portio.h \
	  $(SRC)/include/dtack/portio.p
	$(MK_CC) -c -o $(OUT)/source/low/dtport1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dtport1.c
$(OUT)/source/low/hist2.o: $(SRC)/source/low/hist2.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/hist2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_OFF) $(MK_CCOPTIMIZE_HIGH) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/hist2.c
$(OUT)/source/low/enter.o: $(SRC)/source/low/enter.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/liner.h \
	  $(SRC)/include/dtack/liner.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/enter.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_NOINTER=DT_1 \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/enter.c
$(OUT)/source/low/ipcqnx4.o: $(SRC)/source/low/ipcqnx4.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/ipcqnx4.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ipcqnx4.c
$(OUT)/source/low/ipcqbaby.o: $(SRC)/source/low/ipcqbaby.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/label.h
	$(MK_CC) -c -o $(OUT)/source/low/ipcqbaby.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ipcqbaby.c
$(OUT)/source/low/ipcrock.o: $(SRC)/source/low/ipcrock.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/label.h \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/ipcrock.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_SOCKET=DT_$(MK_PRJ_SOCKET_DEFINE) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  -I$(MK_PRJ_SOCKET_INCLUDE) \
	  $(SRC)/source/low/ipcrock.c
$(OUT)/source/low/license.o: $(SRC)/source/low/license.c \
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
	  $(SRC)/include/dtack/licens.h \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/license.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DPRJ_FLEXLM_DEFINE \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  -IPRJ_FLEXLM_INCLUDE \
	  $(SRC)/source/low/license.c
$(OUT)/source/low/fd.o: $(SRC)/source/low/fd.c \
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
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/fd.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_ZLIB=DT_$(MK_ZLIB_DEFINE) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  -I$(MK_ZLIB_INCLUDE) \
	  $(SRC)/source/low/fd.c
$(OUT)/source/low/f.o: $(SRC)/source/low/f.c \
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
	  $(SRC)/include/dtack/sem.h \
	  $(SRC)/include/dtack/sem.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/f.h \
	  $(SRC)/include/dtack/f.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/cipher.h \
	  $(SRC)/include/dtack/cipher.p
	$(MK_CC) -c -o $(OUT)/source/low/f.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_ZLIB=DT_$(MK_ZLIB_DEFINE) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  -I$(MK_ZLIB_INCLUDE) \
	  $(SRC)/source/low/f.c
$(OUT)/source/low/cipher.o: $(SRC)/source/low/cipher.c \
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
	  $(SRC)/include/dtack/cipher.h \
	  $(SRC)/include/dtack/cipher.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/cipher.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_ZLIB=DT_$(MK_ZLIB_DEFINE) -D$(MK_MCRYPT_DEFINE) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  -I$(MK_ZLIB_INCLUDE) \
	  -I$(MK_MCRYPT_INCLUDE) \
	  $(SRC)/source/low/cipher.c
$(OUT)/source/low/prj_dat.o: $(SRC)/source/low/prj_dat.c \
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
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/source/low/prj_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) -DDTPRJ_X=DT_$(MK_PRJ_X_DEFINE) -DDTPRJ_SOCKET=DT_$(MK_PRJ_SOCKET_DEFINE) -DDTPRJ_TIFF=DT_$(MK_TIFF_DEFINE) -DDTPRJ_CURSES=DT_$(MK_PRJ_CURSES_DEFINE) -DDTPRJ_TERMCAP=DT_$(MK_PRJ_TERMCAP_DEFINE) -DDTPRJ_TERMIOS=DT_$(MK_PRJ_TERMIOS_DEFINE) -DPRJ_VMC_DEFINE -DDTPRJ_NOINTER=DT_1 \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/prj_dat.c
$(OUT)/source/low/dbtable.o: $(SRC)/source/low/dbtable.c \
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
	  $(SRC)/include/dtack/db.h \
	  $(SRC)/include/dtack/db.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/dbtable.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dbtable.c
$(OUT)/source/low/points.o: $(SRC)/source/low/points.c \
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
	  $(SRC)/include/dtack/point.h \
	  $(SRC)/include/dtack/point.p \
	  $(SRC)/include/dtack/points.h \
	  $(SRC)/include/dtack/points.p
	$(MK_CC) -c -o $(OUT)/source/low/points.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/points.c
$(OUT)/source/low/poly.o: $(SRC)/source/low/poly.c \
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
	  $(SRC)/include/dtack/point.h \
	  $(SRC)/include/dtack/point.p \
	  $(SRC)/include/dtack/poly.h \
	  $(SRC)/include/dtack/poly.p \
	  $(SRC)/include/dtack/matrix.h \
	  $(SRC)/include/dtack/matrix.p
	$(MK_CC) -c -o $(OUT)/source/low/poly.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/poly.c
$(OUT)/source/low/matrix3d.o: $(SRC)/source/low/matrix3d.c \
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
	  $(SRC)/include/dtack/point.h \
	  $(SRC)/include/dtack/point.p \
	  $(SRC)/include/dtack/poly.h \
	  $(SRC)/include/dtack/poly.p \
	  $(SRC)/include/dtack/matrix.h \
	  $(SRC)/include/dtack/matrix.p
	$(MK_CC) -c -o $(OUT)/source/low/matrix3d.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/matrix3d.c
$(OUT)/source/low/matrix2d.o: $(SRC)/source/low/matrix2d.c \
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
	  $(SRC)/include/dtack/point.h \
	  $(SRC)/include/dtack/point.p \
	  $(SRC)/include/dtack/poly.h \
	  $(SRC)/include/dtack/poly.p \
	  $(SRC)/include/dtack/matrix.h \
	  $(SRC)/include/dtack/matrix.p
	$(MK_CC) -c -o $(OUT)/source/low/matrix2d.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/matrix2d.c
$(OUT)/source/low/matrix.o: $(SRC)/source/low/matrix.c \
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
	  $(SRC)/include/dtack/point.h \
	  $(SRC)/include/dtack/point.p \
	  $(SRC)/include/dtack/poly.h \
	  $(SRC)/include/dtack/poly.p \
	  $(SRC)/include/dtack/matrix.h \
	  $(SRC)/include/dtack/matrix.p
	$(MK_CC) -c -o $(OUT)/source/low/matrix.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/matrix.c
$(OUT)/source/low/assert.o: $(SRC)/source/low/assert.c \
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
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/assert.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/assert.c
$(OUT)/source/low/cons.o: $(SRC)/source/low/cons.c \
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
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/cons.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/cons.c
$(OUT)/source/low/cstack.o: $(SRC)/source/low/cstack.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/cstack.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/cstack.c
$(OUT)/source/low/dbg.o: $(SRC)/source/low/dbg.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/dbg.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dbg.c
$(OUT)/source/low/dbg2.o: $(SRC)/source/low/dbg2.c \
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
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/dbg2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dbg2.c
$(OUT)/source/low/err.o: $(SRC)/source/low/err.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/err.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/err.c
$(OUT)/source/low/err2.o: $(SRC)/source/low/err2.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/err2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/err2.c
$(OUT)/source/low/eventw.o: $(SRC)/source/low/eventw.c \
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
	  $(SRC)/include/dtack/event.h \
	  $(SRC)/include/dtack/event.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/eventw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/eventw.c
$(OUT)/source/low/ftp.o: $(SRC)/source/low/ftp.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/ftp.h \
	  $(SRC)/include/dtack/ftp.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/ftp.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ftp.c
$(OUT)/source/low/heapmon.o: $(SRC)/source/low/heapmon.c \
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
	  $(SRC)/include/dtack/heap.h \
	  $(SRC)/include/dtack/heap.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/source/low/heapmon.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/heapmon.c
$(OUT)/source/low/heap2.o: $(SRC)/source/low/heap2.c \
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
	  $(SRC)/include/dtack/heap.h \
	  $(SRC)/include/dtack/heap.p
	$(MK_CC) -c -o $(OUT)/source/low/heap2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/heap2.c
$(OUT)/source/low/hist.o: $(SRC)/source/low/hist.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/hist.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/hist.c
$(OUT)/source/low/hist_da.o: $(SRC)/source/low/hist_da.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/hist_da.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/hist_da.c
$(OUT)/source/low/ipc.o: $(SRC)/source/low/ipc.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/label.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/ipc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ipc.c
$(OUT)/source/low/ipcdde.o: $(SRC)/source/low/ipcdde.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/ipcdde.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ipcdde.c
$(OUT)/source/low/leave.o: $(SRC)/source/low/leave.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/leave.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/leave.c
$(OUT)/source/low/liner.o: $(SRC)/source/low/liner.c \
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
	  $(SRC)/include/dtack/liner.h \
	  $(SRC)/include/dtack/liner.p
	$(MK_CC) -c -o $(OUT)/source/low/liner.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/liner.c
$(OUT)/source/low/list.o: $(SRC)/source/low/list.c \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/list.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/list.c
$(OUT)/source/low/list_io.o: $(SRC)/source/low/list_io.c \
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
	  $(SRC)/include/dtack/list.p
	$(MK_CC) -c -o $(OUT)/source/low/list_io.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/list_io.c
$(OUT)/source/low/lut8.o: $(SRC)/source/low/lut8.c \
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
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/lut8.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/lut8.c
$(OUT)/source/low/lut8_mat.o: $(SRC)/source/low/lut8_mat.c \
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
	  $(SRC)/include/dtack/lut8.p
	$(MK_CC) -c -o $(OUT)/source/low/lut8_mat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/lut8_mat.c
$(OUT)/source/low/msgu.o: $(SRC)/source/low/msgu.c \
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
	  $(SRC)/include/dtack/msg.h \
	  $(SRC)/include/dtack/msg.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/msgu.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/msgu.c
$(OUT)/source/low/msw.o: $(SRC)/source/low/msw.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/msw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/msw.c
$(OUT)/source/low/os_mem.o: $(SRC)/source/low/os_mem.c \
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
	  $(SRC)/include/dtack/heap.h \
	  $(SRC)/include/dtack/heap.p
	$(MK_CC) -c -o $(OUT)/source/low/os_mem.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_mem.c
$(OUT)/source/low/os_delay.o: $(SRC)/source/low/os_delay.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/os_delay.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_delay.c
$(OUT)/source/low/os_futil.o: $(SRC)/source/low/os_futil.c \
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
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/os_futil.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_futil.c
$(OUT)/source/low/os_heap.o: $(SRC)/source/low/os_heap.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/os_heap.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_heap.c
$(OUT)/source/low/os_heap2.o: $(SRC)/source/low/os_heap2.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/os_heap2.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_heap2.c
$(OUT)/source/low/os_time.o: $(SRC)/source/low/os_time.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/os_time.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_time.c
$(OUT)/source/low/os_env.o: $(SRC)/source/low/os_env.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/os_env.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/os_env.c
$(OUT)/source/low/param.o: $(SRC)/source/low/param.c \
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
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/param.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/param.c
$(OUT)/source/low/phys.o: $(SRC)/source/low/phys.c \
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
	  $(SRC)/include/dtack/phys.h \
	  $(SRC)/include/dtack/phys.p
	$(MK_CC) -c -o $(OUT)/source/low/phys.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/phys.c
$(OUT)/source/low/pnm.o: $(SRC)/source/low/pnm.c \
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
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/pnm.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/pnm.c
$(OUT)/source/low/portio.o: $(SRC)/source/low/portio.c \
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
	  $(SRC)/include/dtack/msw.p \
	  $(SRC)/include/dtack/portio.h \
	  $(SRC)/include/dtack/portio.p \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p
	$(MK_CC) -c -o $(OUT)/source/low/portio.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/portio.c
$(OUT)/source/low/prj.o: $(SRC)/source/low/prj.c \
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
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p
	$(MK_CC) -c -o $(OUT)/source/low/prj.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/prj.c
$(OUT)/source/low/qnx4.o: $(SRC)/source/low/qnx4.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/qnx4.h \
	  $(SRC)/include/dtack/qnx4.p
	$(MK_CC) -c -o $(OUT)/source/low/qnx4.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/qnx4.c
$(OUT)/source/low/rev.o: $(SRC)/source/low/rev.c \
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
	  $(SRC)/include/dtack/rev.h \
	  $(SRC)/include/dtack/rev.p
	$(MK_CC) -c -o $(OUT)/source/low/rev.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/rev.c
$(OUT)/source/low/say.o: $(SRC)/source/low/say.c \
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
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/say.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/say.c
$(OUT)/source/low/semu.o: $(SRC)/source/low/semu.c \
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
	  $(SRC)/include/dtack/sem.h \
	  $(SRC)/include/dtack/sem.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/semu.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/semu.c
$(OUT)/source/low/semw.o: $(SRC)/source/low/semw.c \
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
	  $(SRC)/include/dtack/sem.h \
	  $(SRC)/include/dtack/sem.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/semw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/semw.c
$(OUT)/source/low/serial.o: $(SRC)/source/low/serial.c \
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
	  $(SRC)/include/dtack/serial.h \
	  $(SRC)/include/dtack/serial.p \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/serial.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/serial.c
$(OUT)/source/low/serialu.o: $(SRC)/source/low/serialu.c \
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
	  $(SRC)/include/dtack/serial.h \
	  $(SRC)/include/dtack/serial.p \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/serialu.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/serialu.c
$(OUT)/source/low/serialw.o: $(SRC)/source/low/serialw.c \
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
	  $(SRC)/include/dtack/serial.h \
	  $(SRC)/include/dtack/serial.p \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/serialw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/serialw.c
$(OUT)/source/low/shmu.o: $(SRC)/source/low/shmu.c \
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
	  $(SRC)/include/dtack/shm.h \
	  $(SRC)/include/dtack/shm.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/shmu.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/shmu.c
$(OUT)/source/low/shmw.o: $(SRC)/source/low/shmw.c \
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
	  $(SRC)/include/dtack/msw.p \
	  $(SRC)/include/dtack/shm.h \
	  $(SRC)/include/dtack/shm.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/shmw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/shmw.c
$(OUT)/source/low/str.o: $(SRC)/source/low/str.c \
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
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/str.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/str.c
$(OUT)/source/low/threads.o: $(SRC)/source/low/threads.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/thread.h \
	  $(SRC)/include/dtack/thread.p \
	  $(SRC)/include/dtack/threads.h \
	  $(SRC)/include/dtack/threads.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/threads.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/threads.c
$(OUT)/source/low/thread.o: $(SRC)/source/low/thread.c \
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
	  $(SRC)/include/dtack/thread.h \
	  $(SRC)/include/dtack/thread.p
	$(MK_CC) -c -o $(OUT)/source/low/thread.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/thread.c
$(OUT)/source/low/threadu.o: $(SRC)/source/low/threadu.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/thread.h \
	  $(SRC)/include/dtack/thread.p \
	  $(SRC)/include/dtack/errno.h
	$(MK_CC) -c -o $(OUT)/source/low/threadu.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/threadu.c
$(OUT)/source/low/threadw.o: $(SRC)/source/low/threadw.c \
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
	  $(SRC)/include/dtack/ipc.h \
	  $(SRC)/include/dtack/ipc.p \
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/str.h \
	  $(SRC)/include/dtack/str.p \
	  $(SRC)/include/dtack/os.h \
	  $(SRC)/include/dtack/os.p \
	  $(SRC)/include/dtack/thread.h \
	  $(SRC)/include/dtack/thread.p \
	  $(SRC)/include/dtack/errno.h \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/threadw.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/threadw.c
$(OUT)/source/low/usage.o: $(SRC)/source/low/usage.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/usage.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/usage.c
$(OUT)/source/low/yield.o: $(SRC)/source/low/yield.c \
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
	  $(SRC)/include/dtack/cons.h \
	  $(SRC)/include/dtack/cons.p \
	  $(SRC)/include/dtack/msw.h \
	  $(SRC)/include/dtack/msw.p
	$(MK_CC) -c -o $(OUT)/source/low/yield.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/yield.c
$(OUT)/source/low/libmain.o: $(SRC)/source/low/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/libmain.c
$(OUT)/source/low/dbg_dat.o: $(SRC)/source/low/dbg_dat.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/dbg_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/dbg_dat.c
$(OUT)/source/low/conf_dat.o: $(SRC)/source/low/conf_dat.c \
	  $(SRC)/include/dtack/config.h
	$(MK_CC) -c -o $(OUT)/source/low/conf_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/conf_dat.c
$(OUT)/source/low/lut8_dat.o: $(SRC)/source/low/lut8_dat.c \
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
	  $(SRC)/include/dtack/lut8.p
	$(MK_CC) -c -o $(OUT)/source/low/lut8_dat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/lut8_dat.c
$(OUT)/source/low/ntype.o: $(SRC)/source/low/ntype.c \
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
	  $(SRC)/include/dtack/str.p
	$(MK_CC) -c -o $(OUT)/source/low/ntype.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ntype.c
$(OUT)/source/low/ntype_da.o: $(SRC)/source/low/ntype_da.c \
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
	$(MK_CC) -c -o $(OUT)/source/low/ntype_da.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/source/low \
	  -I$(SRC)/include \
	  $(SRC)/source/low/ntype_da.c


