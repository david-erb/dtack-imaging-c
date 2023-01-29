
SRC = ./..
OUT = ./..



default: $(MK_DEFAULT)


all: statics shareds others


static: statics
statics: exes 

shared: shareds
shareds: shexes 

allsubs:

exes: \
	  $(OUT)/bin/dtutil$(MK_EXE)

shexes: \
	  $(OUT)/bin/dtutil$(MK_SHEXE)

scripts: 

libs: 

shlibs: 

others: 


#----------------------------------------------------------------------------
strip:
	if [ -w $(OUT)/bin/dtutil$(MK_SHEXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtutil$(MK_SHEXE); fi
	if [ -w $(OUT)/bin/dtutil$(MK_EXE) ]; \
	  then $(MK_STRIP) $(OUT)/bin/dtutil$(MK_EXE); fi



#----------------------------------------------------------------------------
clean:
	$(MK_RM) $(OUT)/bin/dtutil$(MK_SHEXE)
	$(MK_RM) $(OUT)/bin/dtutil$(MK_EXE)
	$(MK_RM) $(OUT)/dtutil/*.o
	$(MK_RM) $(MK_JUNK)

#----------------------------------------------------------------------------
veryclean: clean rpcclean
#----------------------------------------------------------------------------
rpcclean:



#-------------------------------------------------------------------------
# subs




#-------------------------------------------------------------------------
# exes



$(OUT)/bin/dtutil$(MK_EXE): \
	  $(OUT)/dtutil/dtutil.o \
	  
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_OFF) -o $(OUT)/bin/dtutil$(MK_EXE) \
	  $(OUT)/dtutil/dtutil.o \
	    
$(OUT)/bin/dtutil$(MK_SHEXE): $(OUT)/dtutil/dtutil.o
	$(MK_LD) $(MK_LDDEBUGGER_ON) $(MK_LDSTACK_SMALL) $(MK_LDSHARABLE_ON) -o $(OUT)/bin/dtutil$(MK_SHEXE) \
	  $(OUT)/dtutil/dtutil.o   

#-------------------------------------------------------------------------
# libs




#-------------------------------------------------------------------------
# others



#-------------------------------------------------------------------------
# srcs



$(OUT)/dtutil/dtutil.o: $(SRC)/dtutil/dtutil.c
	$(MK_CC) -c -o $(OUT)/dtutil/dtutil.o $(MK_CCSHARABLE_OFF) $(MK_CCEXCEPTION_OFF) $(MK_CCDEBUGGER_ON) $(MK_CCOPTIMIZE_NONE) \
	   $(MK_CCWARNINGS_HIGH) \
	  -D$(MK_DTCONFIG_TARGET) -DDTPRJ_NOPAGING=DT_$(MK_PRJ_NOPAGING) \
	  -I$(SRC)/dtutil \
	  -I$(SRC)/include \
	  $(SRC)/dtutil/dtutil.c



# -------------------------------
# to make this a console mode program
# 1. remove /Gz from compile line
# 2. change subsystem:windows to subsystem:console in link block
# -------------------------------

subdir_dtutil: default
