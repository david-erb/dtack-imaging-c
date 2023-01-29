


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
	  $(DOC)/lj4/mem.lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/mem.lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/mem.ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/mem.ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/mem.dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/mem.dvl
dvls: dvls-


latexs-: \
	  $(DOC)/source/bigmem/mem.ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/mem.htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/mem/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/mem.txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/mem.rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s




$(DOC)/lj4/mem.lj4: $(DOC)/PRJ_PS6DIR/mem.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/mem.ps6 $(DOC)/lj4/mem.lj4

$(DOC)/PRJ_LJ3DIR/mem.lj3: $(DOC)/PRJ_PS3DIR/mem.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/mem.ps3 $(DOC)/PRJ_LJ3DIR/mem.lj3

$(DOC)/PRJ_PS6DIR/mem.ps6: $(DOC)/dvl/mem.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/bigmem $(DOC)/dvl/mem.dvl $(DOC)/PRJ_PS6DIR/mem.ps6 1200

$(DOC)/PRJ_PS3DIR/mem.ps3: $(DOC)/dvl/mem.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/bigmem $(DOC)/dvl/mem.dvl $(DOC)/PRJ_PS3DIR/mem.ps3 300

$(DOC)/dvl/mem.dvl: $(DOC)/source/bigmem/mem.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/bigmem; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/bigmem/mem.ltx \
	   $(DOC)/dvl/mem.dvl:utopia.ltx \
	   $(DOC)/dvi/mem.dvi:plain.ltx

$(DOC)/dvi/mem.dvi: $(DOC)/source/bigmem/mem.ltx
	cd $(DOC)/source/bigmem; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/bigmem/mem.ltx \
	   $(DOC)/dvi/mem.dvi:plain.ltx

$(DOC)/source/bigmem/mem.ltx: $(DOC)/source/bigmem/mem.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/bigmem/mem.dx \
	  $(DOC)/source/bigmem/mem.ltx \
	  "mem" \
	  "PRJ_SERIES"

$(DOC)/html/mem.htm: $(DOC)/source/bigmem/mem.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/bigmem/mem.dx \
	  $(DOC)/html/mem.htm \
	  "mem" "PRJ_SERIES"

$(DOC)/html/mem/sect000.htm: $(DOC)/source/bigmem/mem.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/bigmem/mem.dx \
	  $(DOC)/html/mem \
	  "mem" "PRJ_SERIES"

$(DOC)/text/mem.txt: $(DOC)/source/bigmem/mem.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/bigmem/mem.dx \
	  $(DOC)/text/mem.txt \
	  "mem" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/mem.rtf: $(DOC)/source/bigmem/mem.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/bigmem/mem.dx \
	  $(DOC)/PRJ_RTFDIR/mem.rtf \
	  "mem" "PRJ_SERIES"


$(DOC)/source/bigmem/mem.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/bigmem/mem.doc \
	  $(SRC)/source/bigmem/mem.c \
	  $(SRC)/source/bigmem/memconf.c \
	  $(SRC)/source/bigmem/memputp.c \
	  $(SRC)/source/bigmem/memgetp.c \
	  $(SRC)/source/bigmem/memio.c \
	  $(SRC)/source/bigmem/memcopy.c \
	  $(SRC)/source/bigmem/memcmp.c \
	  $(SRC)/source/bigmem/memset.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/bigmem \
	  -Dfilebase=mem \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/bigmem/mem.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/mem.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memconf.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memputp.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memgetp.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memio.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memcopy.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memcmp.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/bigmem/memset.c > $(DOC)/source/bigmem/mem.dx
	@bash dtpics.sh $(SRC)/source/bigmem $(DOC)/source/bigmem/mem.dx $(DOC)/source/bigmem



$(SRC)/source/bigmem/mem1.c: $(SRC)/source/bigmem/mem.doc
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/bigmem/mem1.c $(SRC)/source/bigmem/mem.doc




#----------------------------------------------------------------------------

examples: \
	  $(SRC)/source/bigmem/mem1.c


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
	bash yam.sh $(SRC) source/bigmem default > $(OUT)/source/bigmem/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem qw0 > $(OUT)/source/bigmem/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem qw1 > $(OUT)/source/bigmem/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem qw6 > $(OUT)/source/bigmem/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem dw0 > $(OUT)/source/bigmem/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem ww0 > $(OUT)/source/bigmem/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem dw1 > $(OUT)/source/bigmem/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem dt0 > $(OUT)/source/bigmem/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem db0 > $(OUT)/source/bigmem/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem wb0 > $(OUT)/source/bigmem/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem wb3 > $(OUT)/source/bigmem/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem wv0 > $(OUT)/source/bigmem/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem wv4 > $(OUT)/source/bigmem/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem wv5 > $(OUT)/source/bigmem/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem dv0 > $(OUT)/source/bigmem/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/bigmem bsd1 > $(OUT)/source/bigmem/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source/bigmem default -DDTNODEPEND > $(OUT)/source/bigmem/makefile
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
	  -f $(SRC)/source/bigmem/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/bigmem/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/bigmem/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source/bigmem ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/bigmem/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/bigmem/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/bigmem/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/bigmem/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/bigmem/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/bigmem/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/bigmem/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/source/bigmem \
	  $(SRC)/source/bigmem/dtmem1.c \
	  $(SRC)/source/bigmem/dtmem2.c \
	  $(SRC)/source/bigmem/font.c \
	  $(SRC)/source/bigmem/memgetp.c \
	  $(SRC)/source/bigmem/memset.c \
	  $(SRC)/source/bigmem/memcmp.c \
	  $(SRC)/source/bigmem/memcopy.c \
	  $(SRC)/source/bigmem/mem_ems.c \
	  $(SRC)/source/bigmem/memxmsif.c \
	  $(SRC)/source/bigmem/memxmslb.c \
	  $(SRC)/source/bigmem/memtxms.c \
	  $(SRC)/source/bigmem/memsourc.c \
	  $(SRC)/source/bigmem/mem.c \
	  $(SRC)/source/bigmem/memconf.c \
	  $(SRC)/source/bigmem/memparse.c \
	  $(SRC)/source/bigmem/memio.c \
	  $(SRC)/source/bigmem/memclip.c \
	  $(SRC)/source/bigmem/memputp.c \
	  $(SRC)/source/bigmem/mem_std.c \
	  $(SRC)/source/bigmem/mem_ram.c \
	  $(SRC)/source/bigmem/mem_disk.c \
	  $(SRC)/source/bigmem/stack.c \
	  $(SRC)/source/bigmem/stack_io.c \
	  $(SRC)/source/bigmem/libmain.c

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
