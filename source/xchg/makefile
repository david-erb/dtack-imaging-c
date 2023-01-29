


SRC = ../..
OUT = ../..

all: 

allsubs:

#----------------------------------------------------------------------------
protos: 

#----------------------------------------------------------------------------

$(DT_YAM)/data/doclist.m4: $(DT_YAM)/data/doclist.dat
	bash dtdocm4.sh $(DT_YAM)

lj4s-: \
	  $(DOC)/lj4/eximq.lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/eximq.lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/eximq.ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/eximq.ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/eximq.dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/eximq.dvl
dvls: dvls-


latexs-: \
	  $(DOC)/source/xchg/eximq.ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/eximq.htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/eximq/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/eximq.txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/eximq.rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s




$(DOC)/lj4/eximq.lj4: $(DOC)/PRJ_PS6DIR/eximq.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/eximq.ps6 $(DOC)/lj4/eximq.lj4

$(DOC)/PRJ_LJ3DIR/eximq.lj3: $(DOC)/PRJ_PS3DIR/eximq.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/eximq.ps3 $(DOC)/PRJ_LJ3DIR/eximq.lj3

$(DOC)/PRJ_PS6DIR/eximq.ps6: $(DOC)/dvl/eximq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/xchg $(DOC)/dvl/eximq.dvl $(DOC)/PRJ_PS6DIR/eximq.ps6 1200

$(DOC)/PRJ_PS3DIR/eximq.ps3: $(DOC)/dvl/eximq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/xchg $(DOC)/dvl/eximq.dvl $(DOC)/PRJ_PS3DIR/eximq.ps3 300

$(DOC)/dvl/eximq.dvl: $(DOC)/source/xchg/eximq.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/xchg; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/xchg/eximq.ltx \
	   $(DOC)/dvl/eximq.dvl:utopia.ltx \
	   $(DOC)/dvi/eximq.dvi:plain.ltx

$(DOC)/dvi/eximq.dvi: $(DOC)/source/xchg/eximq.ltx
	cd $(DOC)/source/xchg; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/xchg/eximq.ltx \
	   $(DOC)/dvi/eximq.dvi:plain.ltx

$(DOC)/source/xchg/eximq.ltx: $(DOC)/source/xchg/eximq.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/xchg/eximq.dx \
	  $(DOC)/source/xchg/eximq.ltx \
	  "eximq" \
	  "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/eximq.htm: $(DOC)/source/xchg/eximq.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/xchg/eximq.dx \
	  $(DOC)/html/eximq.htm \
	  "eximq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/eximq/sect000.htm: $(DOC)/source/xchg/eximq.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/xchg/eximq.dx \
	  $(DOC)/html/eximq \
	  "eximq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/text/eximq.txt: $(DOC)/source/xchg/eximq.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/xchg/eximq.dx \
	  $(DOC)/text/eximq.txt \
	  "eximq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/PRJ_RTFDIR/eximq.rtf: $(DOC)/source/xchg/eximq.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/xchg/eximq.dx \
	  $(DOC)/PRJ_RTFDIR/eximq.rtf \
	  "eximq" "PRJ_LIB_SERIES_SHORT"


$(DOC)/source/xchg/eximq.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/xchg/exim.doc \
	  $(SRC)/source/xchg/export.c \
	  $(SRC)/source/xchg/import.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/xchg \
	  -Dfilebase=eximq \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/xchg/exim.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/xchg/export.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/xchg/import.c > $(DOC)/source/xchg/eximq.dx
	@bash dtpics.sh $(SRC)/source/xchg $(DOC)/source/xchg/eximq.dx $(DOC)/source/xchg



$(SRC)/source/xchg/dtexim1.c: $(SRC)/source/xchg/exim.doc
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/xchg/dtexim1.c $(SRC)/source/xchg/exim.doc




#----------------------------------------------------------------------------

examples: \
	  $(SRC)/source/xchg/dtexim1.c


usages: 


#-------------------------------------------------------------------------
cis:
	find . -type d ! -name . -prune -o \
	  \( -name "*.[chx]" -o -name "dir.yam" \) \
	  -exec ci -l {} </dev/null \;

#-------------------------------------------------------------------------
maks: \
	qw0.maks \
	qw1.maks \
	qw6.maks \
	dw0.maks \
	ww0.maks \
	dw1.maks \
	dt0.maks \
	db0.maks \
	wb0.maks \
	wb3.maks \
	wv0.maks \
	wv4.maks \
	wv5.maks \
	dv0.maks \
	bsd1.maks

#-------------------------------------------------------------------------

makefile: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg default > $(OUT)/source/xchg/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg qw0 > $(OUT)/source/xchg/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg qw1 > $(OUT)/source/xchg/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg qw6 > $(OUT)/source/xchg/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg dw0 > $(OUT)/source/xchg/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg ww0 > $(OUT)/source/xchg/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg dw1 > $(OUT)/source/xchg/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg dt0 > $(OUT)/source/xchg/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg db0 > $(OUT)/source/xchg/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg wb0 > $(OUT)/source/xchg/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg wb3 > $(OUT)/source/xchg/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg wv0 > $(OUT)/source/xchg/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg wv4 > $(OUT)/source/xchg/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg wv5 > $(OUT)/source/xchg/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg dv0 > $(OUT)/source/xchg/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/xchg bsd1 > $(OUT)/source/xchg/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source/xchg default -DDTNODEPEND > $(OUT)/source/xchg/makefile
	make yamsubs

yamsubs:



#--------------------------------------------------------------------------
scratch:
	yam default_host -DDTNODEPEND
	make yam
	make veryclean
	make protos
	make others
	make makefiles
	make
#-------------------------------------------------------------------------

qw0:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/xchg/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/xchg/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/xchg/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source/xchg ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/xchg/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/xchg/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/xchg/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/xchg/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/xchg/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/xchg/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/xchg/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/source/xchg \
	  $(SRC)/source/xchg/dtexim1.c \
	  $(SRC)/source/xchg/readtif.c \
	  $(SRC)/source/xchg/xchg_jpg.c \
	  $(SRC)/source/xchg/xchg_dtm.c \
	  $(SRC)/source/xchg/xchg_scr.c \
	  $(SRC)/source/xchg/xchg_can.c \
	  $(SRC)/source/xchg/dib.c \
	  $(SRC)/source/xchg/export.c \
	  $(SRC)/source/xchg/import.c \
	  $(SRC)/source/xchg/read.c \
	  $(SRC)/source/xchg/bmp.c \
	  $(SRC)/source/xchg/lut8xchg.c \
	  $(SRC)/source/xchg/wmf.c \
	  $(SRC)/source/xchg/write.c \
	  $(SRC)/source/xchg/writebmp.c \
	  $(SRC)/source/xchg/writewmf.c \
	  $(SRC)/source/xchg/parse.c \
	  $(SRC)/source/xchg/xchg_win.c \
	  $(SRC)/source/xchg/xchg_wxx.c \
	  $(SRC)/source/xchg/xchg_dat.c \
	  $(SRC)/source/xchg/libmain.c

#----------------------------------------------------------------------------
doctidy:
	rm -f *.rtf *.hh *.hpj *.ref *.dx
	rm -f *.idx *.ind *.log *.dvi *.dvl *.aux *.blg *.bbl *.ltx
docclean: doctidy
	rm -f *.htm *.txi *.txt *.inf
	rm -f *.lj4 *.lj4.gz *.lj3 *.lj3.gz *.ps6 *.ps6.gz*.ps3 *.ps3.gz

#----------------------------------------------------------------------------
veryclean: docclean 
# end of default finish
