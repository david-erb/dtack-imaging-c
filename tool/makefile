


SRC = ..
OUT = ..

all: exes 

allsubs:

#----------------------------------------------------------------------------
protos: \
	  dttool.p

#----------------------------------------------------------------------------

$(DT_YAM)/data/doclist.m4: $(DT_YAM)/data/doclist.dat
	bash dtdocm4.sh $(DT_YAM)

lj4s-: 
lj4s: lj4s-


lj3s-: 
lj3s: lj3s-


ps6s-: 
ps6s: ps6s-


ps3s-: 
ps3s: ps3s-


dvis-: 
dvis: dvis-


dvls-: 
dvls: dvls-


latexs-: 
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: 
htmls: htmls-


htm2s-: 
htm2s: htm2s-


texts-: 
texts: texts-


rtfs-: 
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s


$(SRC)/tool/dttool.use: \
	  $(SRC)/tool/main.c
	bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DT_YAM)/doc/include \
	  $(SRC)/tool/main.c | \
	bash dtdx.sh $(SRC) -fuse -o/tmp/dttool \
	  "\"-aDavid Erb\"" \
	  "\"-cDtack, Inc.\"" -
	(echo "/*"; cat /tmp/dttool.use; echo "*/") > $(SRC)/tool/dttool.use
	dtutil cstring usage < /tmp/dttool.use >> $(SRC)/tool/dttool.use
	rm -f /tmp/dttool.use

$(SRC)/tool/box.use: \
	  $(SRC)/tool/box.c
	bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DT_YAM)/doc/include \
	  $(SRC)/tool/box.c | \
	bash dtdx.sh $(SRC) -fuse -o/tmp/box \
	  "\"-aDavid Erb\"" \
	  "\"-cDtack, Inc.\"" -
	(echo "/*"; cat /tmp/box.use; echo "*/") > $(SRC)/tool/box.use
	dtutil cstring usage < /tmp/box.use >> $(SRC)/tool/box.use
	rm -f /tmp/box.use

$(SRC)/tool/convolve.use: \
	  $(SRC)/tool/convolve.c
	bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DT_YAM)/doc/include \
	  $(SRC)/tool/convolve.c | \
	bash dtdx.sh $(SRC) -fuse -o/tmp/convolve \
	  "\"-aDavid Erb\"" \
	  "\"-cDtack, Inc.\"" -
	(echo "/*"; cat /tmp/convolve.use; echo "*/") > $(SRC)/tool/convolve.use
	dtutil cstring usage < /tmp/convolve.use >> $(SRC)/tool/convolve.use
	rm -f /tmp/convolve.use

$(SRC)/tool/copy.use: \
	  $(SRC)/tool/copy.c
	bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DT_YAM)/doc/include \
	  $(SRC)/tool/copy.c | \
	bash dtdx.sh $(SRC) -fuse -o/tmp/copy \
	  "\"-aDavid Erb\"" \
	  "\"-cDtack, Inc.\"" -
	(echo "/*"; cat /tmp/copy.use; echo "*/") > $(SRC)/tool/copy.use
	dtutil cstring usage < /tmp/copy.use >> $(SRC)/tool/copy.use
	rm -f /tmp/copy.use





#----------------------------------------------------------------------------

examples: 


usages: \
	  $(SRC)/tool/dttool.use \
	  $(SRC)/tool/box.use \
	  $(SRC)/tool/convolve.use \
	  $(SRC)/tool/copy.use


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
         ../config/prj.yam
	bash yam.sh $(SRC) tool default > $(OUT)/tool/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool qw0 > $(OUT)/tool/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool qw1 > $(OUT)/tool/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool qw6 > $(OUT)/tool/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool dw0 > $(OUT)/tool/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool ww0 > $(OUT)/tool/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool dw1 > $(OUT)/tool/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool dt0 > $(OUT)/tool/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool db0 > $(OUT)/tool/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool wb0 > $(OUT)/tool/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool wb3 > $(OUT)/tool/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool wv0 > $(OUT)/tool/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool wv4 > $(OUT)/tool/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool wv5 > $(OUT)/tool/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool dv0 > $(OUT)/tool/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) tool bsd1 > $(OUT)/tool/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) tool default -DDTNODEPEND > $(OUT)/tool/makefile
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
	  -f $(SRC)/tool/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/tool/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/tool/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/tool ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/tool/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/tool/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/tool/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/tool/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/tool/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/tool/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/tool/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos

dttool.p: \
	  $(SRC)/tool/parse.c
	bash dtdxc.sh \
	  dttool.p -fproto \
	  $(SRC)/tool/parse.c


#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/tool \
	  $(SRC)/tool/box.c \
	  $(SRC)/tool/color.c \
	  $(SRC)/tool/convolve.c \
	  $(SRC)/tool/cooc.c \
	  $(SRC)/tool/copy.c \
	  $(SRC)/tool/enclose.c \
	  $(SRC)/tool/find.c \
	  $(SRC)/tool/fft.c \
	  $(SRC)/tool/gen.c \
	  $(SRC)/tool/hist.c \
	  $(SRC)/tool/linear.c \
	  $(SRC)/tool/logical.c \
	  $(SRC)/tool/lookup.c \
	  $(SRC)/tool/lut.c \
	  $(SRC)/tool/median.c \
	  $(SRC)/tool/morph.c \
	  $(SRC)/tool/mult.c \
	  $(SRC)/tool/nonlin.c \
	  $(SRC)/tool/paste.c \
	  $(SRC)/tool/profile.c \
	  $(SRC)/tool/sum.c \
	  $(SRC)/tool/run.c \
	  $(SRC)/tool/regray.c \
	  $(SRC)/tool/resample.c \
	  $(SRC)/tool/slice.c \
	  $(SRC)/tool/snr.c \
	  $(SRC)/tool/stats.c \
	  $(SRC)/tool/subabs.c \
	  $(SRC)/tool/text.c \
	  $(SRC)/tool/winstop.c \
	  $(SRC)/tool/multithr.c \
	  $(SRC)/tool/thresh.c \
	  $(SRC)/tool/parse.c \
	  $(SRC)/tool/libmain.c \
	  $(SRC)/tool/main.c

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
