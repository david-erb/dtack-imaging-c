


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
	  $(DOC)/lj4/imageq.lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/imageq.lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/imageq.ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/imageq.ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/imageq.dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/imageq.dvl
dvls: dvls-


latexs-: \
	  $(DOC)/source/high/imageq.ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/imageq.htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/imageq/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/imageq.txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/imageq.rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s




$(DOC)/lj4/imageq.lj4: $(DOC)/PRJ_PS6DIR/imageq.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/imageq.ps6 $(DOC)/lj4/imageq.lj4

$(DOC)/PRJ_LJ3DIR/imageq.lj3: $(DOC)/PRJ_PS3DIR/imageq.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/imageq.ps3 $(DOC)/PRJ_LJ3DIR/imageq.lj3

$(DOC)/PRJ_PS6DIR/imageq.ps6: $(DOC)/dvl/imageq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/high $(DOC)/dvl/imageq.dvl $(DOC)/PRJ_PS6DIR/imageq.ps6 1200

$(DOC)/PRJ_PS3DIR/imageq.ps3: $(DOC)/dvl/imageq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/high $(DOC)/dvl/imageq.dvl $(DOC)/PRJ_PS3DIR/imageq.ps3 300

$(DOC)/dvl/imageq.dvl: $(DOC)/source/high/imageq.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/high; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/high/imageq.ltx \
	   $(DOC)/dvl/imageq.dvl:utopia.ltx \
	   $(DOC)/dvi/imageq.dvi:plain.ltx

$(DOC)/dvi/imageq.dvi: $(DOC)/source/high/imageq.ltx
	cd $(DOC)/source/high; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/high/imageq.ltx \
	   $(DOC)/dvi/imageq.dvi:plain.ltx

$(DOC)/source/high/imageq.ltx: $(DOC)/source/high/imageq.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/high/imageq.dx \
	  $(DOC)/source/high/imageq.ltx \
	  "imageq" \
	  "PRJ_SERIES"

$(DOC)/html/imageq.htm: $(DOC)/source/high/imageq.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/high/imageq.dx \
	  $(DOC)/html/imageq.htm \
	  "imageq" "PRJ_SERIES"

$(DOC)/html/imageq/sect000.htm: $(DOC)/source/high/imageq.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/high/imageq.dx \
	  $(DOC)/html/imageq \
	  "imageq" "PRJ_SERIES"

$(DOC)/text/imageq.txt: $(DOC)/source/high/imageq.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/high/imageq.dx \
	  $(DOC)/text/imageq.txt \
	  "imageq" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/imageq.rtf: $(DOC)/source/high/imageq.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/high/imageq.dx \
	  $(DOC)/PRJ_RTFDIR/imageq.rtf \
	  "imageq" "PRJ_SERIES"


$(DOC)/source/high/imageq.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/high/image.doc \
	  $(SRC)/source/high/image1.doc \
	  $(SRC)/source/high/image.c \
	  $(SRC)/source/high/pixel.c \
	  $(SRC)/source/high/image2.doc \
	  $(SRC)/source/high/arith.c \
	  $(SRC)/source/high/logical.c \
	  $(SRC)/source/high/cumul.c \
	  $(SRC)/source/high/cast.c \
	  $(SRC)/source/high/cmp.c \
	  $(SRC)/source/high/diff.c \
	  $(SRC)/source/high/log.c \
	  $(SRC)/source/high/adj.c \
	  $(SRC)/source/high/adj_run.c \
	  $(SRC)/source/high/scan.c \
	  $(SRC)/source/high/label1.c \
	  $(SRC)/source/high/blob.c \
	  $(SRC)/source/high/check.c \
	  $(SRC)/source/high/dbg.c \
	  $(SRC)/source/high/err.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/high \
	  -Dfilebase=imageq \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/high/image.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/image1.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/image.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/pixel.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/image2.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/arith.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/logical.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/cumul.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/cast.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/cmp.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/diff.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/log.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/adj.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/adj_run.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/scan.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/label1.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/blob.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/check.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/dbg.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/high/err.c > $(DOC)/source/high/imageq.dx
	@bash dtpics.sh $(SRC)/source/high $(DOC)/source/high/imageq.dx $(DOC)/source/high








































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
	bash yam.sh $(SRC) source/high default > $(OUT)/source/high/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high qw0 > $(OUT)/source/high/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high qw1 > $(OUT)/source/high/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high qw6 > $(OUT)/source/high/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high dw0 > $(OUT)/source/high/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high ww0 > $(OUT)/source/high/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high dw1 > $(OUT)/source/high/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high dt0 > $(OUT)/source/high/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high db0 > $(OUT)/source/high/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high wb0 > $(OUT)/source/high/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high wb3 > $(OUT)/source/high/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high wv0 > $(OUT)/source/high/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high wv4 > $(OUT)/source/high/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high wv5 > $(OUT)/source/high/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high dv0 > $(OUT)/source/high/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/high bsd1 > $(OUT)/source/high/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source/high default -DDTNODEPEND > $(OUT)/source/high/makefile
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
	  -f $(SRC)/source/high/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/high/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/high/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source/high ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/high/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/high/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/high/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/high/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/high/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/high/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/high/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/source/high \
	  $(SRC)/source/high/fftw.c \
	  $(SRC)/source/high/fft.c \
	  $(SRC)/source/high/adj_run.c \
	  $(SRC)/source/high/blob.c \
	  $(SRC)/source/high/cast.c \
	  $(SRC)/source/high/col.c \
	  $(SRC)/source/high/cooc2.c \
	  $(SRC)/source/high/conv_d.c \
	  $(SRC)/source/high/conv_l.c \
	  $(SRC)/source/high/conv_s.c \
	  $(SRC)/source/high/joint2.c \
	  $(SRC)/source/high/label1.c \
	  $(SRC)/source/high/label2.c \
	  $(SRC)/source/high/log.c \
	  $(SRC)/source/high/lookup.c \
	  $(SRC)/source/high/morph.c \
	  $(SRC)/source/high/median.c \
	  $(SRC)/source/high/plot1.c \
	  $(SRC)/source/high/resample.c \
	  $(SRC)/source/high/rotate.c \
	  $(SRC)/source/high/run_len.c \
	  $(SRC)/source/high/run.c \
	  $(SRC)/source/high/scan.c \
	  $(SRC)/source/high/sum_rows.c \
	  $(SRC)/source/high/sum_cols.c \
	  $(SRC)/source/high/transpos.c \
	  $(SRC)/source/high/matrix.c \
	  $(SRC)/source/high/assign.c \
	  $(SRC)/source/high/adj.c \
	  $(SRC)/source/high/arith.c \
	  $(SRC)/source/high/check.c \
	  $(SRC)/source/high/cmp.c \
	  $(SRC)/source/high/constant.c \
	  $(SRC)/source/high/convolve.c \
	  $(SRC)/source/high/cooc.c \
	  $(SRC)/source/high/cumul.c \
	  $(SRC)/source/high/dbg.c \
	  $(SRC)/source/high/diff.c \
	  $(SRC)/source/high/err.c \
	  $(SRC)/source/high/gen1.c \
	  $(SRC)/source/high/gen2.c \
	  $(SRC)/source/high/gen3.c \
	  $(SRC)/source/high/gen_kern.c \
	  $(SRC)/source/high/gen_dat.c \
	  $(SRC)/source/high/hist1.c \
	  $(SRC)/source/high/hist2.c \
	  $(SRC)/source/high/image.c \
	  $(SRC)/source/high/joint.c \
	  $(SRC)/source/high/logical.c \
	  $(SRC)/source/high/mean.c \
	  $(SRC)/source/high/noise.c \
	  $(SRC)/source/high/pixel.c \
	  $(SRC)/source/high/plot2.c \
	  $(SRC)/source/high/plot_txt.c \
	  $(SRC)/source/high/range.c \
	  $(SRC)/source/high/run_dat.c \
	  $(SRC)/source/high/runf.c \
	  $(SRC)/source/high/stats.c \
	  $(SRC)/source/high/sum.c \
	  $(SRC)/source/high/sumsq.c \
	  $(SRC)/source/high/sumsqd.c \
	  $(SRC)/source/high/sumsqs.c \
	  $(SRC)/source/high/thresh.c \
	  $(SRC)/source/high/libmain.c

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
