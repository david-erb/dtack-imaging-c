
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
	  $(OUT)/bin/dtthigh1$(MK_EXE) \
	  $(OUT)/bin/dtthigh2$(MK_EXE) \
	  $(OUT)/bin/dttarith$(MK_EXE) \
	  $(OUT)/bin/dttadj$(MK_EXE) \
	  $(OUT)/bin/dttconv$(MK_EXE) \
	  $(OUT)/bin/dttmorph$(MK_EXE) \
	  $(OUT)/bin/dttgeom$(MK_EXE) \
	  $(OUT)/bin/dttcast$(MK_EXE) \
	  $(OUT)/bin/dttcmp$(MK_EXE) \
	  $(OUT)/bin/dttdb$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtthigh1$(MK_SHEXE) \
	  $(OUT)/bin/dtthigh2$(MK_SHEXE) \
	  $(OUT)/bin/dttarith$(MK_SHEXE) \
	  $(OUT)/bin/dttadj$(MK_SHEXE) \
	  $(OUT)/bin/dttconv$(MK_SHEXE) \
	  $(OUT)/bin/dttmorph$(MK_SHEXE) \
	  $(OUT)/bin/dttgeom$(MK_SHEXE) \
	  $(OUT)/bin/dttcast$(MK_SHEXE) \
	  $(OUT)/bin/dttcmp$(MK_SHEXE) \
	  $(OUT)/bin/dttdb$(MK_SHEXE)

scripts: 

libs: \
	  $(OUT)/lib/libdtthix1.$(MK_LIB) \
	  $(OUT)/lib/libdtthix2.$(MK_LIB)

shlibs: \
	  $(OUT)/libso/libdtthix1.so$(MK_SHVER) \
	  $(OUT)/libso/libdtthix2.so$(MK_SHVER)

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtthigh1$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtthigh1$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtthigh2$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtthigh2$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttarith$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttarith$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttadj$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttadj$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttconv$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttconv$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttmorph$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttmorph$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttgeom$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttgeom$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttcast$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttcast$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttcmp$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttcmp$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dttdb$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttdb$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtthigh1$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtthigh1$(MK_EXE); fi
	if [ -w $(OUT)/bin/dtthigh2$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtthigh2$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttarith$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttarith$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttadj$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttadj$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttconv$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttconv$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttmorph$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttmorph$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttgeom$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttgeom$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttcast$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttcast$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttcmp$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttcmp$(MK_EXE); fi
	if [ -w $(OUT)/bin/dttdb$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dttdb$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtthigh1$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtthigh2$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttarith$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttadj$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttconv$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttmorph$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttgeom$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttcast$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttcmp$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dttdb$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtthigh1$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dtthigh2$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttarith$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttadj$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttconv$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttmorph$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttgeom$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttcast$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttcmp$(MK_EXE)
	$(MK_RM) $(OUT)/bin/dttdb$(MK_EXE)
	$(MK_RM) $(OUT)/lib/libdtthix1.$(MK_LIB)
	$(MK_RM) $(OUT)/lib/libdtthix2.$(MK_LIB)
	$(MK_RM) $(OUT)/libso/libdtthix1.so$(MK_SHVER)
	$(MK_RM) $(OUT)/libso/libdtthix2.so$(MK_SHVER)
	$(MK_RM) $(OUT)/test/high/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtthigh1$(MK_EXE): \
	  $(OUT)/test/high/high1.o \
	  $(OUT)/lib/libdtthix1.$(MK_LIB) \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtthigh1$(MK_EXE) \
	  $(OUT)/test/high/high1.o \
	  -L$(OUT)/lib -ldtthix1 \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtthigh1$(MK_SHEXE): $(OUT)/test/high/high1.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtthigh1$(MK_SHEXE) \
	  $(OUT)/test/high/high1.o  \
	  -L$(OUT)/libso -ldtthix1 \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)
$(OUT)/bin/dtthigh2$(MK_EXE): \
	  $(OUT)/test/high/high2.o \
	  $(OUT)/lib/libdtthix2.$(MK_LIB) \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtthigh2$(MK_EXE) \
	  $(OUT)/test/high/high2.o \
	  -L$(OUT)/lib -ldtthix2 \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dtthigh2$(MK_SHEXE): $(OUT)/test/high/high2.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtthigh2$(MK_SHEXE) \
	  $(OUT)/test/high/high2.o  \
	  -L$(OUT)/libso -ldtthix2 \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttarith$(MK_EXE): \
	  $(OUT)/test/high/dttarith.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttarith$(MK_EXE) \
	  $(OUT)/test/high/dttarith.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttarith$(MK_SHEXE): $(OUT)/test/high/dttarith.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttarith$(MK_SHEXE) \
	  $(OUT)/test/high/dttarith.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttadj$(MK_EXE): \
	  $(OUT)/test/high/dttadj.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttadj$(MK_EXE) \
	  $(OUT)/test/high/dttadj.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttadj$(MK_SHEXE): $(OUT)/test/high/dttadj.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttadj$(MK_SHEXE) \
	  $(OUT)/test/high/dttadj.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttconv$(MK_EXE): \
	  $(OUT)/test/high/dttconv.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttconv$(MK_EXE) \
	  $(OUT)/test/high/dttconv.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttconv$(MK_SHEXE): $(OUT)/test/high/dttconv.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttconv$(MK_SHEXE) \
	  $(OUT)/test/high/dttconv.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttmorph$(MK_EXE): \
	  $(OUT)/test/high/dttmorph.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttmorph$(MK_EXE) \
	  $(OUT)/test/high/dttmorph.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttmorph$(MK_SHEXE): $(OUT)/test/high/dttmorph.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttmorph$(MK_SHEXE) \
	  $(OUT)/test/high/dttmorph.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttgeom$(MK_EXE): \
	  $(OUT)/test/high/dttgeom.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttgeom$(MK_EXE) \
	  $(OUT)/test/high/dttgeom.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttgeom$(MK_SHEXE): $(OUT)/test/high/dttgeom.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttgeom$(MK_SHEXE) \
	  $(OUT)/test/high/dttgeom.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttcast$(MK_EXE): \
	  $(OUT)/test/high/dttcast.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttcast$(MK_EXE) \
	  $(OUT)/test/high/dttcast.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttcast$(MK_SHEXE): $(OUT)/test/high/dttcast.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttcast$(MK_SHEXE) \
	  $(OUT)/test/high/dttcast.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttcmp$(MK_EXE): \
	  $(OUT)/test/high/dttcmp.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttcmp$(MK_EXE) \
	  $(OUT)/test/high/dttcmp.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttcmp$(MK_SHEXE): $(OUT)/test/high/dttcmp.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttcmp$(MK_SHEXE) \
	  $(OUT)/test/high/dttcmp.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttdb$(MK_EXE): \
	  $(OUT)/test/high/dttdb.o \
	  $(OUT)/lib/libdttaa.$(MK_LIB) \
	  $(OUT)/lib/libdtxchg.$(MK_LIB) \
	  $(OUT)/lib/libdtcolor.$(MK_LIB) \
	  $(OUT)/lib/libdthigh.$(MK_LIB) \
	  $(OUT)/lib/libdtwinc.$(MK_LIB) \
	  $(OUT)/lib/libdtbigmem.$(MK_LIB) \
	  $(OUT)/lib/libdtlow.$(MK_LIB) \
	  $(OUT)/lib/libdt1d.$(MK_LIB)
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dttdb$(MK_EXE) \
	  $(OUT)/test/high/dttdb.o \
	  -L$(OUT)/lib -ldttaa \
	  -L$(OUT)/lib -ldtxchg \
	  -L$(OUT)/lib -ldtcolor \
	  -L$(OUT)/lib -ldthigh \
	  -L$(OUT)/lib -ldtwinc \
	  -L$(OUT)/lib -ldtbigmem \
	  -L$(OUT)/lib -ldtlow \
	  -L$(OUT)/lib -ldt1d  \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)
$(OUT)/bin/dttdb$(MK_SHEXE): $(OUT)/test/high/dttdb.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dttdb$(MK_SHEXE) \
	  $(OUT)/test/high/dttdb.o  \
	  -L$(OUT)/libso -ldttaa \
	  -L$(OUT)/libso -ldtxchg \
	  -L$(OUT)/libso -ldtcolor \
	  -L$(OUT)/libso -ldthigh \
	  -L$(OUT)/libso -ldtwinc \
	  -L$(OUT)/libso -ldtbigmem \
	  -L$(OUT)/libso -ldtlow \
	  -L$(OUT)/libso -ldt1d \
	  $(MK_PRJ_SOCKET_LOAD)   $(MK_IJG_LOAD)   $(MK_XV_LOAD)   $(MK_ZLIB_LOAD)   $(MK_FFTW_LOAD)   $(MK_MCRYPT_LOAD)   $(MK_TIFF_LOAD)   $(MK_PRJ_LOADS)  $(MK_PRJ_CURSES_LOAD)  $(MK_TERMCAP_LOAD) $(MK_TERMIOS_LOAD)

#-------------------------------------------------------------------------
# libs



$(OUT)/lib/libdtthix1.$(MK_LIB): \
	  $(OUT)/test/high/dtthist.o \
	  $(OUT)/test/high/dttpgm.o \
	  $(OUT)/test/high/dttplot.o \
	  $(OUT)/test/high/dttrun.o \
	  $(OUT)/test/high/dttlog.o \
	  $(OUT)/test/high/libmain.o
	$(MK_RM) $(OUT)/lib/libdtthix1.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtthix1.$(MK_LIB) \
	  $(OUT)/test/high/dtthist.o \
	  $(OUT)/test/high/dttpgm.o \
	  $(OUT)/test/high/dttplot.o \
	  $(OUT)/test/high/dttrun.o \
	  $(OUT)/test/high/dttlog.o \
	  $(OUT)/test/high/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtthix1.$(MK_LIB)
$(OUT)/libso/libdtthix1.so$(MK_SHVER): \
	  $(OUT)/test/high/dtthist.o \
	  $(OUT)/test/high/dttpgm.o \
	  $(OUT)/test/high/dttplot.o \
	  $(OUT)/test/high/dttrun.o \
	  $(OUT)/test/high/dttlog.o \
	  $(OUT)/test/high/libmain.o
	$(MK_RM) $(OUT)/libso/libdtthix1.so$(MK_SHVER) $(OUT)/libso/libdtthix1.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtthix1.so$(MK_SHVER) \
	  $(OUT)/test/high/dtthist.o \
	  $(OUT)/test/high/dttpgm.o \
	  $(OUT)/test/high/dttplot.o \
	  $(OUT)/test/high/dttrun.o \
	  $(OUT)/test/high/dttlog.o \
	  $(OUT)/test/high/libmain.o
	ln -s libdtthix1.so$(MK_SHVER) $(OUT)/libso/libdtthix1.so
$(OUT)/lib/libdtthix2.$(MK_LIB): \
	  $(OUT)/test/high/dttresam.o \
	  $(OUT)/test/high/dttcooc.o \
	  $(OUT)/test/high/dttjoint.o \
	  $(OUT)/test/high/dttstat.o \
	  $(OUT)/test/high/dttmatrix.o \
	  $(OUT)/test/high/libmain.o
	$(MK_RM) $(OUT)/lib/libdtthix2.$(MK_LIB)
	$(MK_AR) $(OUT)/lib/libdtthix2.$(MK_LIB) \
	  $(OUT)/test/high/dttresam.o \
	  $(OUT)/test/high/dttcooc.o \
	  $(OUT)/test/high/dttjoint.o \
	  $(OUT)/test/high/dttstat.o \
	  $(OUT)/test/high/dttmatrix.o \
	  $(OUT)/test/high/libmain.o
	$(MK_RANLIB) $(OUT)/lib/libdtthix2.$(MK_LIB)
$(OUT)/libso/libdtthix2.so$(MK_SHVER): \
	  $(OUT)/test/high/dttresam.o \
	  $(OUT)/test/high/dttcooc.o \
	  $(OUT)/test/high/dttjoint.o \
	  $(OUT)/test/high/dttstat.o \
	  $(OUT)/test/high/dttmatrix.o \
	  $(OUT)/test/high/libmain.o
	$(MK_RM) $(OUT)/libso/libdtthix2.so$(MK_SHVER) $(OUT)/libso/libdtthix2.so
	$(MK_LDSHAREDLIB) -o $(OUT)/libso/libdtthix2.so$(MK_SHVER) \
	  $(OUT)/test/high/dttresam.o \
	  $(OUT)/test/high/dttcooc.o \
	  $(OUT)/test/high/dttjoint.o \
	  $(OUT)/test/high/dttstat.o \
	  $(OUT)/test/high/dttmatrix.o \
	  $(OUT)/test/high/libmain.o
	ln -s libdtthix2.so$(MK_SHVER) $(OUT)/libso/libdtthix2.so

#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/test/high/dtthist.o: $(SRC)/test/high/dtthist.c \
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
	$(MK_CC) -c -o $(OUT)/test/high/dtthist.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dtthist.c
$(OUT)/test/high/dttpgm.o: $(SRC)/test/high/dttpgm.c \
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
	  $(SRC)/include/dtack/pnm.h \
	  $(SRC)/include/dtack/pnm.p
	$(MK_CC) -c -o $(OUT)/test/high/dttpgm.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttpgm.c
$(OUT)/test/high/dttplot.o: $(SRC)/test/high/dttplot.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttplot.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttplot.c
$(OUT)/test/high/dttrun.o: $(SRC)/test/high/dttrun.c \
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
	  $(SRC)/include/dtack/runf.h \
	  $(SRC)/include/dtack/runf.p \
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttrun.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttrun.c
$(OUT)/test/high/dttlog.o: $(SRC)/test/high/dttlog.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttlog.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttlog.c
$(OUT)/test/high/libmain.o: $(SRC)/test/high/libmain.c \
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
	$(MK_CC) -c -o $(OUT)/test/high/libmain.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/libmain.c
$(OUT)/test/high/dttresam.o: $(SRC)/test/high/dttresam.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttresam.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttresam.c
$(OUT)/test/high/dttcooc.o: $(SRC)/test/high/dttcooc.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttcooc.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttcooc.c
$(OUT)/test/high/dttjoint.o: $(SRC)/test/high/dttjoint.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttjoint.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttjoint.c
$(OUT)/test/high/dttstat.o: $(SRC)/test/high/dttstat.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttstat.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttstat.c
$(OUT)/test/high/dttmatrix.o: $(SRC)/test/high/dttmatrix.c \
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
	  $(SRC)/include/dtack/point.h \
	  $(SRC)/include/dtack/point.p \
	  $(SRC)/include/dtack/points.h \
	  $(SRC)/include/dtack/points.p \
	  $(SRC)/include/dtack/poly.h \
	  $(SRC)/include/dtack/poly.p \
	  $(SRC)/include/dtack/matrix.h \
	  $(SRC)/include/dtack/matrix.p
	$(MK_CC) -c -o $(OUT)/test/high/dttmatrix.o $(MK_CCSHARABLE_ON) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttmatrix.c
$(OUT)/test/high/high1.o: $(SRC)/test/high/high1.c \
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
	$(MK_CC) -c -o $(OUT)/test/high/high1.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/high1.c
$(OUT)/test/high/high2.o: $(SRC)/test/high/high2.c \
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
	$(MK_CC) -c -o $(OUT)/test/high/high2.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/high2.c
$(OUT)/test/high/dttarith.o: $(SRC)/test/high/dttarith.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttarith.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttarith.c
$(OUT)/test/high/dttadj.o: $(SRC)/test/high/dttadj.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/include/dtack/stack.h \
	  $(SRC)/include/dtack/stack.p \
	  $(SRC)/include/dtack/prj.h \
	  $(SRC)/include/dtack/prj.p \
	  $(SRC)/include/dtack/runf.h \
	  $(SRC)/include/dtack/runf.p
	$(MK_CC) -c -o $(OUT)/test/high/dttadj.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttadj.c
$(OUT)/test/high/dttconv.o: $(SRC)/test/high/dttconv.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttconv.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttconv.c
$(OUT)/test/high/dttmorph.o: $(SRC)/test/high/dttmorph.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttmorph.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttmorph.c
$(OUT)/test/high/dttgeom.o: $(SRC)/test/high/dttgeom.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttgeom.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttgeom.c
$(OUT)/test/high/dttcast.o: $(SRC)/test/high/dttcast.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p \
	  $(SRC)/test/high/dttcast.use
	$(MK_CC) -c -o $(OUT)/test/high/dttcast.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttcast.c
$(OUT)/test/high/dttcmp.o: $(SRC)/test/high/dttcmp.c \
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
	  $(SRC)/include/dtack/mem.h \
	  $(SRC)/include/dtack/mem.p
	$(MK_CC) -c -o $(OUT)/test/high/dttcmp.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttcmp.c
$(OUT)/test/high/dttdb.o: $(SRC)/test/high/dttdb.c \
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
	  $(SRC)/include/dtack/list.h \
	  $(SRC)/include/dtack/list.p \
	  $(SRC)/include/dtack/db.h \
	  $(SRC)/include/dtack/db.p
	$(MK_CC) -c -o $(OUT)/test/high/dttdb.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/test/high \
	  -I$(SRC)/include \
	  $(SRC)/test/high/dttdb.c


