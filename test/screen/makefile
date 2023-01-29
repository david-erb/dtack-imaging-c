


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
	  $(DOC)/lj4/prj_test_guide(dttscr).lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/prj_test_guide(dttscr).lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/prj_test_guide(dttscr).ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/prj_test_guide(dttscr).ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/prj_test_guide(dttscr).dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/prj_test_guide(dttscr).dvl
dvls: dvls-


latexs-: \
	  $(DOC)/test/screen/prj_test_guide(dttscr).ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/prj_test_guide(dttscr).htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/prj_test_guide(dttscr)/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/prj_test_guide(dttscr).txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/prj_test_guide(dttscr).rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s


$(SRC)/test/screen/dttscr.use: \
	  $(SRC)/test/screen/dttscr.c
	bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DT_YAM)/doc/include \
	  $(SRC)/test/screen/dttscr.c | \
	bash dtdx.sh $(SRC) -fuse -o/tmp/dttscr \
	  "\"-aDavid Erb\"" \
	  "\"-cDtack, Inc.\"" -
	(echo "/*"; cat /tmp/dttscr.use; echo "*/") > $(SRC)/test/screen/dttscr.use
	dtutil cstring usage < /tmp/dttscr.use >> $(SRC)/test/screen/dttscr.use
	rm -f /tmp/dttscr.use



$(DOC)/lj4/prj_test_guide(dttscr).lj4: $(DOC)/PRJ_PS6DIR/prj_test_guide(dttscr).ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/prj_test_guide(dttscr).ps6 $(DOC)/lj4/prj_test_guide(dttscr).lj4

$(DOC)/PRJ_LJ3DIR/prj_test_guide(dttscr).lj3: $(DOC)/PRJ_PS3DIR/prj_test_guide(dttscr).ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/prj_test_guide(dttscr).ps3 $(DOC)/PRJ_LJ3DIR/prj_test_guide(dttscr).lj3

$(DOC)/PRJ_PS6DIR/prj_test_guide(dttscr).ps6: $(DOC)/dvl/prj_test_guide(dttscr).dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/test/screen $(DOC)/dvl/prj_test_guide(dttscr).dvl $(DOC)/PRJ_PS6DIR/prj_test_guide(dttscr).ps6 1200

$(DOC)/PRJ_PS3DIR/prj_test_guide(dttscr).ps3: $(DOC)/dvl/prj_test_guide(dttscr).dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/test/screen $(DOC)/dvl/prj_test_guide(dttscr).dvl $(DOC)/PRJ_PS3DIR/prj_test_guide(dttscr).ps3 300

$(DOC)/dvl/prj_test_guide(dttscr).dvl: $(DOC)/test/screen/prj_test_guide(dttscr).ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/test/screen; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/test/screen/prj_test_guide(dttscr).ltx \
	   $(DOC)/dvl/prj_test_guide(dttscr).dvl:utopia.ltx \
	   $(DOC)/dvi/prj_test_guide(dttscr).dvi:plain.ltx

$(DOC)/dvi/prj_test_guide(dttscr).dvi: $(DOC)/test/screen/prj_test_guide(dttscr).ltx
	cd $(DOC)/test/screen; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/test/screen/prj_test_guide(dttscr).ltx \
	   $(DOC)/dvi/prj_test_guide(dttscr).dvi:plain.ltx

$(DOC)/test/screen/prj_test_guide(dttscr).ltx: $(DOC)/test/screen/prj_test_guide(dttscr).dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/test/screen/prj_test_guide(dttscr).dx \
	  $(DOC)/test/screen/prj_test_guide(dttscr).ltx \
	  "prj_test_guide(dttscr)" \
	  "PRJ_SERIES"

$(DOC)/html/prj_test_guide(dttscr).htm: $(DOC)/test/screen/prj_test_guide(dttscr).dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/test/screen/prj_test_guide(dttscr).dx \
	  $(DOC)/html/prj_test_guide(dttscr).htm \
	  "prj_test_guide(dttscr)" "PRJ_SERIES"

$(DOC)/html/prj_test_guide(dttscr)/sect000.htm: $(DOC)/test/screen/prj_test_guide(dttscr).dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/test/screen/prj_test_guide(dttscr).dx \
	  $(DOC)/html/prj_test_guide(dttscr) \
	  "prj_test_guide(dttscr)" "PRJ_SERIES"

$(DOC)/text/prj_test_guide(dttscr).txt: $(DOC)/test/screen/prj_test_guide(dttscr).dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/test/screen/prj_test_guide(dttscr).dx \
	  $(DOC)/text/prj_test_guide(dttscr).txt \
	  "prj_test_guide(dttscr)" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/prj_test_guide(dttscr).rtf: $(DOC)/test/screen/prj_test_guide(dttscr).dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/test/screen/prj_test_guide(dttscr).dx \
	  $(DOC)/PRJ_RTFDIR/prj_test_guide(dttscr).rtf \
	  "prj_test_guide(dttscr)" "PRJ_SERIES"


$(DOC)/test/screen/prj_test_guide(dttscr).dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/test/screen/dttscr.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/test/screen \
	  -Dfilebase=prj_test_guide(dttscr) \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/test/screen/dttscr.c > $(DOC)/test/screen/prj_test_guide(dttscr).dx
	@bash dtpics.sh $(SRC)/test/screen $(DOC)/test/screen/prj_test_guide(dttscr).dx $(DOC)/test/screen







#----------------------------------------------------------------------------

examples: 


usages: \
	  $(SRC)/test/screen/dttscr.use


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
	bash yam.sh $(SRC) test/screen default > $(OUT)/test/screen/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen qw0 > $(OUT)/test/screen/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen qw1 > $(OUT)/test/screen/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen qw6 > $(OUT)/test/screen/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen dw0 > $(OUT)/test/screen/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen ww0 > $(OUT)/test/screen/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen dw1 > $(OUT)/test/screen/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen dt0 > $(OUT)/test/screen/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen db0 > $(OUT)/test/screen/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen wb0 > $(OUT)/test/screen/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen wb3 > $(OUT)/test/screen/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen wv0 > $(OUT)/test/screen/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen wv4 > $(OUT)/test/screen/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen wv5 > $(OUT)/test/screen/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen dv0 > $(OUT)/test/screen/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) test/screen bsd1 > $(OUT)/test/screen/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) test/screen default -DDTNODEPEND > $(OUT)/test/screen/makefile
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
	  -f $(SRC)/test/screen/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/test/screen/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/test/screen/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/test/screen ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/test/screen/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/test/screen/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/test/screen/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/test/screen/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/test/screen/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/test/screen/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/test/screen/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/test/screen \
	  $(SRC)/test/screen/dttscr.c \
	  $(SRC)/test/screen/dttscrx.c \
	  $(SRC)/test/screen/dttscrf.c \
	  $(SRC)/test/screen/dttscrs.c \
	  $(SRC)/test/screen/dttfastg.c \
	  $(SRC)/test/screen/dttsvgac.c

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
