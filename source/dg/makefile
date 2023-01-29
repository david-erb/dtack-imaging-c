


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
	  $(DOC)/lj4/dgserver.lj4 \
	  $(DOC)/lj4/dgclient.lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/dgserver.lj3 \
	  $(DOC)/PRJ_LJ3DIR/dgclient.lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/dgserver.ps6 \
	  $(DOC)/PRJ_PS6DIR/dgclient.ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/dgserver.ps3 \
	  $(DOC)/PRJ_PS3DIR/dgclient.ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/dgserver.dvi \
	  $(DOC)/dvi/dgclient.dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/dgserver.dvl \
	  $(DOC)/dvl/dgclient.dvl
dvls: dvls-


latexs-: \
	  $(DOC)/source/dg/dgserver.ltx \
	  $(DOC)/source/dg/dgclient.ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/dgserver.htm \
	  $(DOC)/html/dgclient.htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/dgserver/sect000.htm \
	  $(DOC)/html/dgclient/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/dgserver.txt \
	  $(DOC)/text/dgclient.txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/dgserver.rtf \
	  $(DOC)/PRJ_RTFDIR/dgclient.rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s




$(DOC)/lj4/dgserver.lj4: $(DOC)/PRJ_PS6DIR/dgserver.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/dgserver.ps6 $(DOC)/lj4/dgserver.lj4

$(DOC)/PRJ_LJ3DIR/dgserver.lj3: $(DOC)/PRJ_PS3DIR/dgserver.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/dgserver.ps3 $(DOC)/PRJ_LJ3DIR/dgserver.lj3

$(DOC)/PRJ_PS6DIR/dgserver.ps6: $(DOC)/dvl/dgserver.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/dg $(DOC)/dvl/dgserver.dvl $(DOC)/PRJ_PS6DIR/dgserver.ps6 1200

$(DOC)/PRJ_PS3DIR/dgserver.ps3: $(DOC)/dvl/dgserver.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/dg $(DOC)/dvl/dgserver.dvl $(DOC)/PRJ_PS3DIR/dgserver.ps3 300

$(DOC)/dvl/dgserver.dvl: $(DOC)/source/dg/dgserver.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/dg; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/dg/dgserver.ltx \
	   $(DOC)/dvl/dgserver.dvl:utopia.ltx \
	   $(DOC)/dvi/dgserver.dvi:plain.ltx

$(DOC)/dvi/dgserver.dvi: $(DOC)/source/dg/dgserver.ltx
	cd $(DOC)/source/dg; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/dg/dgserver.ltx \
	   $(DOC)/dvi/dgserver.dvi:plain.ltx

$(DOC)/source/dg/dgserver.ltx: $(DOC)/source/dg/dgserver.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgserver.dx \
	  $(DOC)/source/dg/dgserver.ltx \
	  "dgserver" \
	  "NCI"

$(DOC)/html/dgserver.htm: $(DOC)/source/dg/dgserver.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgserver.dx \
	  $(DOC)/html/dgserver.htm \
	  "dgserver" "NCI"

$(DOC)/html/dgserver/sect000.htm: $(DOC)/source/dg/dgserver.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgserver.dx \
	  $(DOC)/html/dgserver \
	  "dgserver" "NCI"

$(DOC)/text/dgserver.txt: $(DOC)/source/dg/dgserver.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgserver.dx \
	  $(DOC)/text/dgserver.txt \
	  "dgserver" "NCI"

$(DOC)/PRJ_RTFDIR/dgserver.rtf: $(DOC)/source/dg/dgserver.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgserver.dx \
	  $(DOC)/PRJ_RTFDIR/dgserver.rtf \
	  "dgserver" "NCI"


$(DOC)/source/dg/dgserver.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/dg/dgserver.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/dg \
	  -Dfilebase=dgserver \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/dg/dgserver.c > $(DOC)/source/dg/dgserver.dx
	@bash dtpics.sh $(SRC)/source/dg $(DOC)/source/dg/dgserver.dx $(DOC)/source/dg




$(DOC)/lj4/dgclient.lj4: $(DOC)/PRJ_PS6DIR/dgclient.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/dgclient.ps6 $(DOC)/lj4/dgclient.lj4

$(DOC)/PRJ_LJ3DIR/dgclient.lj3: $(DOC)/PRJ_PS3DIR/dgclient.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/dgclient.ps3 $(DOC)/PRJ_LJ3DIR/dgclient.lj3

$(DOC)/PRJ_PS6DIR/dgclient.ps6: $(DOC)/dvl/dgclient.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/dg $(DOC)/dvl/dgclient.dvl $(DOC)/PRJ_PS6DIR/dgclient.ps6 1200

$(DOC)/PRJ_PS3DIR/dgclient.ps3: $(DOC)/dvl/dgclient.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/dg $(DOC)/dvl/dgclient.dvl $(DOC)/PRJ_PS3DIR/dgclient.ps3 300

$(DOC)/dvl/dgclient.dvl: $(DOC)/source/dg/dgclient.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/dg; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/dg/dgclient.ltx \
	   $(DOC)/dvl/dgclient.dvl:utopia.ltx \
	   $(DOC)/dvi/dgclient.dvi:plain.ltx

$(DOC)/dvi/dgclient.dvi: $(DOC)/source/dg/dgclient.ltx
	cd $(DOC)/source/dg; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/dg/dgclient.ltx \
	   $(DOC)/dvi/dgclient.dvi:plain.ltx

$(DOC)/source/dg/dgclient.ltx: $(DOC)/source/dg/dgclient.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgclient.dx \
	  $(DOC)/source/dg/dgclient.ltx \
	  "dgclient" \
	  "NCI"

$(DOC)/html/dgclient.htm: $(DOC)/source/dg/dgclient.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgclient.dx \
	  $(DOC)/html/dgclient.htm \
	  "dgclient" "NCI"

$(DOC)/html/dgclient/sect000.htm: $(DOC)/source/dg/dgclient.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgclient.dx \
	  $(DOC)/html/dgclient \
	  "dgclient" "NCI"

$(DOC)/text/dgclient.txt: $(DOC)/source/dg/dgclient.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgclient.dx \
	  $(DOC)/text/dgclient.txt \
	  "dgclient" "NCI"

$(DOC)/PRJ_RTFDIR/dgclient.rtf: $(DOC)/source/dg/dgclient.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/dg/dgclient.dx \
	  $(DOC)/PRJ_RTFDIR/dgclient.rtf \
	  "dgclient" "NCI"


$(DOC)/source/dg/dgclient.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/dg/dgclient.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/dg \
	  -Dfilebase=dgclient \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/dg/dgclient.c > $(DOC)/source/dg/dgclient.dx
	@bash dtpics.sh $(SRC)/source/dg $(DOC)/source/dg/dgclient.dx $(DOC)/source/dg








#----------------------------------------------------------------------------

examples: 


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
	bash yam.sh $(SRC) source/dg default > $(OUT)/source/dg/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg qw0 > $(OUT)/source/dg/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg qw1 > $(OUT)/source/dg/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg qw6 > $(OUT)/source/dg/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg dw0 > $(OUT)/source/dg/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg ww0 > $(OUT)/source/dg/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg dw1 > $(OUT)/source/dg/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg dt0 > $(OUT)/source/dg/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg db0 > $(OUT)/source/dg/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg wb0 > $(OUT)/source/dg/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg wb3 > $(OUT)/source/dg/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg wv0 > $(OUT)/source/dg/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg wv4 > $(OUT)/source/dg/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg wv5 > $(OUT)/source/dg/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg dv0 > $(OUT)/source/dg/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/dg bsd1 > $(OUT)/source/dg/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source/dg default -DDTNODEPEND > $(OUT)/source/dg/makefile
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
	  -f $(SRC)/source/dg/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/dg/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/dg/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source/dg ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dg/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/dg/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/dg/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/dg/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/dg/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/dg/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/dg/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/source/dg \
	  $(SRC)/source/dg/line.c \
	  $(SRC)/source/dg/header.c \
	  $(SRC)/source/dg/dgserver.c \
	  $(SRC)/source/dg/request.c \
	  $(SRC)/source/dg/response.c \
	  $(SRC)/source/dg/rev.c \
	  $(SRC)/source/dg/exit.c \
	  $(SRC)/source/dg/delay.c \
	  $(SRC)/source/dg/dgclient.c \
	  $(SRC)/source/dg/libmain.c

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
