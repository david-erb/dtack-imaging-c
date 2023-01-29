


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
	  $(DOC)/lj4/keybdq.lj4 \
	  $(DOC)/lj4/keympq.lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/keybdq.lj3 \
	  $(DOC)/PRJ_LJ3DIR/keympq.lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/keybdq.ps6 \
	  $(DOC)/PRJ_PS6DIR/keympq.ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/keybdq.ps3 \
	  $(DOC)/PRJ_PS3DIR/keympq.ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/keybdq.dvi \
	  $(DOC)/dvi/keympq.dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/keybdq.dvl \
	  $(DOC)/dvl/keympq.dvl
dvls: dvls-


latexs-: \
	  $(DOC)/source/keybd/keybdq.ltx \
	  $(DOC)/source/keybd/keympq.ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/keybdq.htm \
	  $(DOC)/html/keympq.htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/keybdq/sect000.htm \
	  $(DOC)/html/keympq/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/keybdq.txt \
	  $(DOC)/text/keympq.txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/keybdq.rtf \
	  $(DOC)/PRJ_RTFDIR/keympq.rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s




$(DOC)/lj4/keybdq.lj4: $(DOC)/PRJ_PS6DIR/keybdq.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/keybdq.ps6 $(DOC)/lj4/keybdq.lj4

$(DOC)/PRJ_LJ3DIR/keybdq.lj3: $(DOC)/PRJ_PS3DIR/keybdq.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/keybdq.ps3 $(DOC)/PRJ_LJ3DIR/keybdq.lj3

$(DOC)/PRJ_PS6DIR/keybdq.ps6: $(DOC)/dvl/keybdq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/keybd $(DOC)/dvl/keybdq.dvl $(DOC)/PRJ_PS6DIR/keybdq.ps6 1200

$(DOC)/PRJ_PS3DIR/keybdq.ps3: $(DOC)/dvl/keybdq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/keybd $(DOC)/dvl/keybdq.dvl $(DOC)/PRJ_PS3DIR/keybdq.ps3 300

$(DOC)/dvl/keybdq.dvl: $(DOC)/source/keybd/keybdq.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/keybd; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/keybd/keybdq.ltx \
	   $(DOC)/dvl/keybdq.dvl:utopia.ltx \
	   $(DOC)/dvi/keybdq.dvi:plain.ltx

$(DOC)/dvi/keybdq.dvi: $(DOC)/source/keybd/keybdq.ltx
	cd $(DOC)/source/keybd; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/keybd/keybdq.ltx \
	   $(DOC)/dvi/keybdq.dvi:plain.ltx

$(DOC)/source/keybd/keybdq.ltx: $(DOC)/source/keybd/keybdq.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keybdq.dx \
	  $(DOC)/source/keybd/keybdq.ltx \
	  "keybdq" \
	  "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/keybdq.htm: $(DOC)/source/keybd/keybdq.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keybdq.dx \
	  $(DOC)/html/keybdq.htm \
	  "keybdq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/keybdq/sect000.htm: $(DOC)/source/keybd/keybdq.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keybdq.dx \
	  $(DOC)/html/keybdq \
	  "keybdq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/text/keybdq.txt: $(DOC)/source/keybd/keybdq.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keybdq.dx \
	  $(DOC)/text/keybdq.txt \
	  "keybdq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/PRJ_RTFDIR/keybdq.rtf: $(DOC)/source/keybd/keybdq.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keybdq.dx \
	  $(DOC)/PRJ_RTFDIR/keybdq.rtf \
	  "keybdq" "PRJ_LIB_SERIES_SHORT"


$(DOC)/source/keybd/keybdq.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/keybd/keybd.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/keybd \
	  -Dfilebase=keybdq \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/keybd/keybd.c > $(DOC)/source/keybd/keybdq.dx
	@bash dtpics.sh $(SRC)/source/keybd $(DOC)/source/keybd/keybdq.dx $(DOC)/source/keybd





$(DOC)/lj4/keympq.lj4: $(DOC)/PRJ_PS6DIR/keympq.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/keympq.ps6 $(DOC)/lj4/keympq.lj4

$(DOC)/PRJ_LJ3DIR/keympq.lj3: $(DOC)/PRJ_PS3DIR/keympq.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/keympq.ps3 $(DOC)/PRJ_LJ3DIR/keympq.lj3

$(DOC)/PRJ_PS6DIR/keympq.ps6: $(DOC)/dvl/keympq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/keybd $(DOC)/dvl/keympq.dvl $(DOC)/PRJ_PS6DIR/keympq.ps6 1200

$(DOC)/PRJ_PS3DIR/keympq.ps3: $(DOC)/dvl/keympq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/keybd $(DOC)/dvl/keympq.dvl $(DOC)/PRJ_PS3DIR/keympq.ps3 300

$(DOC)/dvl/keympq.dvl: $(DOC)/source/keybd/keympq.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/keybd; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/keybd/keympq.ltx \
	   $(DOC)/dvl/keympq.dvl:utopia.ltx \
	   $(DOC)/dvi/keympq.dvi:plain.ltx

$(DOC)/dvi/keympq.dvi: $(DOC)/source/keybd/keympq.ltx
	cd $(DOC)/source/keybd; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/keybd/keympq.ltx \
	   $(DOC)/dvi/keympq.dvi:plain.ltx

$(DOC)/source/keybd/keympq.ltx: $(DOC)/source/keybd/keympq.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keympq.dx \
	  $(DOC)/source/keybd/keympq.ltx \
	  "keympq" \
	  "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/keympq.htm: $(DOC)/source/keybd/keympq.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keympq.dx \
	  $(DOC)/html/keympq.htm \
	  "keympq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/keympq/sect000.htm: $(DOC)/source/keybd/keympq.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keympq.dx \
	  $(DOC)/html/keympq \
	  "keympq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/text/keympq.txt: $(DOC)/source/keybd/keympq.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keympq.dx \
	  $(DOC)/text/keympq.txt \
	  "keympq" "PRJ_LIB_SERIES_SHORT"

$(DOC)/PRJ_RTFDIR/keympq.rtf: $(DOC)/source/keybd/keympq.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/keybd/keympq.dx \
	  $(DOC)/PRJ_RTFDIR/keympq.rtf \
	  "keympq" "PRJ_LIB_SERIES_SHORT"


$(DOC)/source/keybd/keympq.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/keybd/keymap.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/keybd \
	  -Dfilebase=keympq \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/keybd/keymap.c > $(DOC)/source/keybd/keympq.dx
	@bash dtpics.sh $(SRC)/source/keybd $(DOC)/source/keybd/keympq.dx $(DOC)/source/keybd



$(SRC)/source/keybd/keybd1.c: $(SRC)/source/keybd/keybd.c
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/keybd/keybd1.c $(SRC)/source/keybd/keybd.c



#----------------------------------------------------------------------------

examples: \
	  $(SRC)/source/keybd/keybd1.c


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
	bash yam.sh $(SRC) source/keybd default > $(OUT)/source/keybd/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd qw0 > $(OUT)/source/keybd/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd qw1 > $(OUT)/source/keybd/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd qw6 > $(OUT)/source/keybd/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd dw0 > $(OUT)/source/keybd/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd ww0 > $(OUT)/source/keybd/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd dw1 > $(OUT)/source/keybd/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd dt0 > $(OUT)/source/keybd/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd db0 > $(OUT)/source/keybd/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd wb0 > $(OUT)/source/keybd/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd wb3 > $(OUT)/source/keybd/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd wv0 > $(OUT)/source/keybd/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd wv4 > $(OUT)/source/keybd/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd wv5 > $(OUT)/source/keybd/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd dv0 > $(OUT)/source/keybd/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/keybd bsd1 > $(OUT)/source/keybd/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source/keybd default -DDTNODEPEND > $(OUT)/source/keybd/makefile
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
	  -f $(SRC)/source/keybd/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/keybd/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/keybd/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source/keybd ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/keybd/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/keybd/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/keybd/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/keybd/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/keybd/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/keybd/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/keybd/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/source/keybd \
	  $(SRC)/source/keybd/dtkeybd1.c \
	  $(SRC)/source/keybd/keymapq.c \
	  $(SRC)/source/keybd/keymapt.c \
	  $(SRC)/source/keybd/keyunix.c \
	  $(SRC)/source/keybd/keyqnx.c \
	  $(SRC)/source/keybd/trmios.c \
	  $(SRC)/source/keybd/keybd.c \
	  $(SRC)/source/keybd/keydos.c \
	  $(SRC)/source/keybd/keymap.c \
	  $(SRC)/source/keybd/keymapc.c \
	  $(SRC)/source/keybd/libmain.c

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
