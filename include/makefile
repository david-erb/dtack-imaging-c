


SRC = ..
OUT = ..

all: allsubs 

allsubs:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) all
#----------------------------------------------------------------------------
protos: 
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) protos
#----------------------------------------------------------------------------

$(DT_YAM)/data/doclist.m4: $(DT_YAM)/data/doclist.dat
	bash dtdocm4.sh $(DT_YAM)

lj4s-: 
lj4s: lj4s-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) lj4s

lj3s-: 
lj3s: lj3s-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) lj3s

ps6s-: 
ps6s: ps6s-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) ps6s

ps3s-: 
ps3s: ps3s-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) ps3s

dvis-: 
dvis: dvis-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) dvis

dvls-: 
dvls: dvls-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) dvls

latexs-: 
latexs: latexs-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) latexs

mans-: 
mans: mans-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) mans

infos-: 
infos: infos-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) infos

htmls-: 
htmls: htmls-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) htmls

htm2s-: 
htm2s: htm2s-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) htm2s

texts-: 
texts: texts-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) texts

rtfs-: 
rtfs: rtfs-
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) rtfs


#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) docs




#----------------------------------------------------------------------------

examples: 
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) examples

usages: 
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) usages

#-------------------------------------------------------------------------
cis:
	find . -type d ! -name . -prune -o \
	  \( -name "*.[chx]" -o -name "dir.yam" \) \
	  -exec ci -l {} </dev/null \;
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) cis
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
	bash yam.sh $(SRC) include default > $(OUT)/include/makefile
makefiles:
	bash yam.sh $(SRC) include default > $(OUT)/include/makefile
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) makefiles_sub
makefiles_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) makefiles

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include qw0 > $(OUT)/include/qw0.mak
qw0.maks:
	bash yam.sh $(SRC) include qw0 > $(OUT)/include/qw0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks_sub
qw0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include qw1 > $(OUT)/include/qw1.mak
qw1.maks:
	bash yam.sh $(SRC) include qw1 > $(OUT)/include/qw1.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks_sub
qw1.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include qw6 > $(OUT)/include/qw6.mak
qw6.maks:
	bash yam.sh $(SRC) include qw6 > $(OUT)/include/qw6.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks_sub
qw6.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include dw0 > $(OUT)/include/dw0.mak
dw0.maks:
	bash yam.sh $(SRC) include dw0 > $(OUT)/include/dw0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks_sub
dw0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include ww0 > $(OUT)/include/ww0.mak
ww0.maks:
	bash yam.sh $(SRC) include ww0 > $(OUT)/include/ww0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks_sub
ww0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include dw1 > $(OUT)/include/dw1.mak
dw1.maks:
	bash yam.sh $(SRC) include dw1 > $(OUT)/include/dw1.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks_sub
dw1.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include dt0 > $(OUT)/include/dt0.mak
dt0.maks:
	bash yam.sh $(SRC) include dt0 > $(OUT)/include/dt0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks_sub
dt0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include db0 > $(OUT)/include/db0.mak
db0.maks:
	bash yam.sh $(SRC) include db0 > $(OUT)/include/db0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) db0.maks_sub
db0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include wb0 > $(OUT)/include/wb0.mak
wb0.maks:
	bash yam.sh $(SRC) include wb0 > $(OUT)/include/wb0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks_sub
wb0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include wb3 > $(OUT)/include/wb3.mak
wb3.maks:
	bash yam.sh $(SRC) include wb3 > $(OUT)/include/wb3.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks_sub
wb3.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include wv0 > $(OUT)/include/wv0.mak
wv0.maks:
	bash yam.sh $(SRC) include wv0 > $(OUT)/include/wv0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks_sub
wv0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include wv4 > $(OUT)/include/wv4.mak
wv4.maks:
	bash yam.sh $(SRC) include wv4 > $(OUT)/include/wv4.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks_sub
wv4.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include wv5 > $(OUT)/include/wv5.mak
wv5.maks:
	bash yam.sh $(SRC) include wv5 > $(OUT)/include/wv5.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks_sub
wv5.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include dv0 > $(OUT)/include/dv0.mak
dv0.maks:
	bash yam.sh $(SRC) include dv0 > $(OUT)/include/dv0.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks_sub
dv0.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) include bsd1 > $(OUT)/include/bsd1.mak
bsd1.maks:
	bash yam.sh $(SRC) include bsd1 > $(OUT)/include/bsd1.mak
	$(MAKE) -f $(SRC)/include/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks_sub
bsd1.maks_sub:
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) include default -DDTNODEPEND > $(OUT)/include/makefile
	make yamsubs

yamsubs:
	bash yam.sh $(SRC) include/dtack default -DDTNODEPEND > $(OUT)/include/dtack/makefile
	cd dtack; make yamsubs

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
	  -f $(SRC)/include/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/include/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/include/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/include ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/include/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/include/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/include/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/include/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/include/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/include/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) deps
#----------------------------------------------------------------------------
doctidy:
	rm -f *.rtf *.hh *.hpj *.ref *.dx
	rm -f *.idx *.ind *.log *.dvi *.dvl *.aux *.blg *.bbl *.ltx
docclean: doctidy
	rm -f *.htm *.txi *.txt *.inf
	rm -f *.lj4 *.lj4.gz *.lj3 *.lj3.gz *.ps6 *.ps6.gz*.ps3 *.ps3.gz
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) docclean
#----------------------------------------------------------------------------
veryclean: docclean 
	cd dtack; $(MAKE) -f $(SRC)/include/dtack/makefile SRC=$(SRC) OUT=$(OUT) veryclean# end of default finish
