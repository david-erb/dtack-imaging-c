


SRC = ..
OUT = ..

all: exes 

allsubs:

#----------------------------------------------------------------------------
protos: \
	  $(SRC)/include/dtack/view.p

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


$(SRC)/server/dtview.use: \
	  $(SRC)/server/view_ipc.c
	bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DT_YAM)/doc/include \
	  $(SRC)/server/view_ipc.c | \
	bash dtdx.sh $(SRC) -fuse -o/tmp/dtview \
	  "\"-aDavid Erb\"" \
	  "\"-cDtack, Inc.\"" -
	(echo "/*"; cat /tmp/dtview.use; echo "*/") > $(SRC)/server/dtview.use
	dtutil cstring usage < /tmp/dtview.use >> $(SRC)/server/dtview.use
	rm -f /tmp/dtview.use


 



#----------------------------------------------------------------------------

examples: 


usages: \
	  $(SRC)/server/dtview.use


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
	bash yam.sh $(SRC) server default > $(OUT)/server/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server qw0 > $(OUT)/server/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server qw1 > $(OUT)/server/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server qw6 > $(OUT)/server/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server dw0 > $(OUT)/server/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server ww0 > $(OUT)/server/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server dw1 > $(OUT)/server/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server dt0 > $(OUT)/server/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server db0 > $(OUT)/server/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server wb0 > $(OUT)/server/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server wb3 > $(OUT)/server/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server wv0 > $(OUT)/server/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server wv4 > $(OUT)/server/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server wv5 > $(OUT)/server/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server dv0 > $(OUT)/server/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) server bsd1 > $(OUT)/server/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) server default -DDTNODEPEND > $(OUT)/server/makefile
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
	  -f $(SRC)/server/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/server/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/server/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/server ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/server/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/server/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/server/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/server/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/server/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/server/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/server/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos

$(SRC)/include/dtack/view.p: \
	  $(SRC)/server/view_ipc.c \
	  $(SRC)/server/view1.c \
	  $(SRC)/server/view2.c
	bash dtdxc.sh \
	  $(SRC)/include/dtack/view.p -fproto \
	  $(SRC)/server/view_ipc.c \
	  $(SRC)/server/view1.c \
	  $(SRC)/server/view2.c        

# ------------------------------------------------------------------------



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/server \
	  $(SRC)/server/view2.c \
	  $(SRC)/server/dtview.c \
	  $(SRC)/server/view_ipc.c \
	  $(SRC)/server/view1.c \
	  $(SRC)/server/dtviewf.c \
	  $(SRC)/server/dtviews.c

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
