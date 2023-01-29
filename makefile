


SRC = .
OUT = .

all: allsubs 

allsubs:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) all
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) all
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) all
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) all
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) all
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) all
#----------------------------------------------------------------------------
protos: 
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) protos
#----------------------------------------------------------------------------

$(DT_YAM)/data/doclist.m4: $(DT_YAM)/data/doclist.dat
	bash dtdocm4.sh $(DT_YAM)

lj4s-: \
	  $(DOC)/lj4/readme.lj4 \
	  $(DOC)/lj4/revision.lj4
lj4s: lj4s-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) lj4s

lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/readme.lj3 \
	  $(DOC)/PRJ_LJ3DIR/revision.lj3
lj3s: lj3s-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) lj3s

ps6s-: \
	  $(DOC)/PRJ_PS6DIR/readme.ps6 \
	  $(DOC)/PRJ_PS6DIR/revision.ps6
ps6s: ps6s-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) ps6s

ps3s-: \
	  $(DOC)/PRJ_PS3DIR/readme.ps3 \
	  $(DOC)/PRJ_PS3DIR/revision.ps3
ps3s: ps3s-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) ps3s

dvis-: \
	  $(DOC)/dvi/readme.dvi \
	  $(DOC)/dvi/revision.dvi
dvis: dvis-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) dvis

dvls-: \
	  $(DOC)/dvl/readme.dvl \
	  $(DOC)/dvl/revision.dvl
dvls: dvls-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) dvls

latexs-: \
	  $(DOC)/./readme.ltx \
	  $(DOC)/./revision.ltx
latexs: latexs-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) latexs

mans-: 
mans: mans-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) mans

infos-: 
infos: infos-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) infos

htmls-: \
	  $(DOC)/html/readme.htm \
	  $(DOC)/html/revision.htm
htmls: htmls-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) htmls

htm2s-: \
	  $(DOC)/html/readme/sect000.htm \
	  $(DOC)/html/revision/sect000.htm
htm2s: htm2s-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) htm2s

texts-: \
	  $(DOC)/text/readme.txt \
	  $(DOC)/text/revision.txt
texts: texts-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) texts

rtfs-: \
	  $(DOC)/PRJ_RTFDIR/readme.rtf \
	  $(DOC)/PRJ_RTFDIR/revision.rtf
rtfs: rtfs-
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) rtfs


#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) docs



$(DOC)/lj4/readme.lj4: $(DOC)/PRJ_PS6DIR/readme.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/readme.ps6 $(DOC)/lj4/readme.lj4

$(DOC)/PRJ_LJ3DIR/readme.lj3: $(DOC)/PRJ_PS3DIR/readme.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/readme.ps3 $(DOC)/PRJ_LJ3DIR/readme.lj3

$(DOC)/PRJ_PS6DIR/readme.ps6: $(DOC)/dvl/readme.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/. $(DOC)/dvl/readme.dvl $(DOC)/PRJ_PS6DIR/readme.ps6 1200

$(DOC)/PRJ_PS3DIR/readme.ps3: $(DOC)/dvl/readme.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/. $(DOC)/dvl/readme.dvl $(DOC)/PRJ_PS3DIR/readme.ps3 300

$(DOC)/dvl/readme.dvl: $(DOC)/./readme.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/.; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/./readme.ltx \
	   $(DOC)/dvl/readme.dvl:utopia.ltx \
	   $(DOC)/dvi/readme.dvi:plain.ltx

$(DOC)/dvi/readme.dvi: $(DOC)/./readme.ltx
	cd $(DOC)/.; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/./readme.ltx \
	   $(DOC)/dvi/readme.dvi:plain.ltx

$(DOC)/./readme.ltx: $(DOC)/./readme.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./readme.dx \
	  $(DOC)/./readme.ltx \
	  "readme" \
	  "PRJ_GEN_SERIES"

$(DOC)/html/readme.htm: $(DOC)/./readme.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./readme.dx \
	  $(DOC)/html/readme.htm \
	  "readme" "PRJ_GEN_SERIES"

$(DOC)/html/readme/sect000.htm: $(DOC)/./readme.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./readme.dx \
	  $(DOC)/html/readme \
	  "readme" "PRJ_GEN_SERIES"

$(DOC)/text/readme.txt: $(DOC)/./readme.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./readme.dx \
	  $(DOC)/text/readme.txt \
	  "readme" "PRJ_GEN_SERIES"

$(DOC)/PRJ_RTFDIR/readme.rtf: $(DOC)/./readme.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./readme.dx \
	  $(DOC)/PRJ_RTFDIR/readme.rtf \
	  "readme" "PRJ_GEN_SERIES"


$(DOC)/./readme.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/./doc/readme.doc
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/. \
	  -Dfilebase=readme \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/./doc/readme.doc > $(DOC)/./readme.dx
	@bash dtpics.sh $(SRC)/. $(DOC)/./readme.dx $(DOC)/.





$(DOC)/lj4/revision.lj4: $(DOC)/PRJ_PS6DIR/revision.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/revision.ps6 $(DOC)/lj4/revision.lj4

$(DOC)/PRJ_LJ3DIR/revision.lj3: $(DOC)/PRJ_PS3DIR/revision.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/revision.ps3 $(DOC)/PRJ_LJ3DIR/revision.lj3

$(DOC)/PRJ_PS6DIR/revision.ps6: $(DOC)/dvl/revision.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/. $(DOC)/dvl/revision.dvl $(DOC)/PRJ_PS6DIR/revision.ps6 1200

$(DOC)/PRJ_PS3DIR/revision.ps3: $(DOC)/dvl/revision.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/. $(DOC)/dvl/revision.dvl $(DOC)/PRJ_PS3DIR/revision.ps3 300

$(DOC)/dvl/revision.dvl: $(DOC)/./revision.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/.; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/./revision.ltx \
	   $(DOC)/dvl/revision.dvl:utopia.ltx \
	   $(DOC)/dvi/revision.dvi:plain.ltx

$(DOC)/dvi/revision.dvi: $(DOC)/./revision.ltx
	cd $(DOC)/.; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/./revision.ltx \
	   $(DOC)/dvi/revision.dvi:plain.ltx

$(DOC)/./revision.ltx: $(DOC)/./revision.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./revision.dx \
	  $(DOC)/./revision.ltx \
	  "revision" \
	  "PRJ_GEN_SERIES"

$(DOC)/html/revision.htm: $(DOC)/./revision.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./revision.dx \
	  $(DOC)/html/revision.htm \
	  "revision" "PRJ_GEN_SERIES"

$(DOC)/html/revision/sect000.htm: $(DOC)/./revision.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./revision.dx \
	  $(DOC)/html/revision \
	  "revision" "PRJ_GEN_SERIES"

$(DOC)/text/revision.txt: $(DOC)/./revision.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./revision.dx \
	  $(DOC)/text/revision.txt \
	  "revision" "PRJ_GEN_SERIES"

$(DOC)/PRJ_RTFDIR/revision.rtf: $(DOC)/./revision.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/./revision.dx \
	  $(DOC)/PRJ_RTFDIR/revision.rtf \
	  "revision" "PRJ_GEN_SERIES"


$(DOC)/./revision.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/./doc/revision.doc
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/. \
	  -Dfilebase=revision \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/./doc/revision.doc > $(DOC)/./revision.dx
	@bash dtpics.sh $(SRC)/. $(DOC)/./revision.dx $(DOC)/.







#----------------------------------------------------------------------------

examples: 
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) examples

usages: 
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) usages

#-------------------------------------------------------------------------
cis:
	find . -type d ! -name . -prune -o \
	  \( -name "*.[chx]" -o -name "dir.yam" \) \
	  -exec ci -l {} </dev/null \;
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) cis
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
         ./config/prj.yam
	bash yam.sh $(SRC) . default > $(OUT)/./makefile
makefiles:
	bash yam.sh $(SRC) . default > $(OUT)/./makefile
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) makefiles_sub
makefiles_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) makefiles

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . qw0 > $(OUT)/./qw0.mak
qw0.maks:
	bash yam.sh $(SRC) . qw0 > $(OUT)/./qw0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) qw0.maks_sub
qw0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . qw1 > $(OUT)/./qw1.mak
qw1.maks:
	bash yam.sh $(SRC) . qw1 > $(OUT)/./qw1.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) qw1.maks_sub
qw1.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . qw6 > $(OUT)/./qw6.mak
qw6.maks:
	bash yam.sh $(SRC) . qw6 > $(OUT)/./qw6.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) qw6.maks_sub
qw6.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . dw0 > $(OUT)/./dw0.mak
dw0.maks:
	bash yam.sh $(SRC) . dw0 > $(OUT)/./dw0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) dw0.maks_sub
dw0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . ww0 > $(OUT)/./ww0.mak
ww0.maks:
	bash yam.sh $(SRC) . ww0 > $(OUT)/./ww0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) ww0.maks_sub
ww0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . dw1 > $(OUT)/./dw1.mak
dw1.maks:
	bash yam.sh $(SRC) . dw1 > $(OUT)/./dw1.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) dw1.maks_sub
dw1.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . dt0 > $(OUT)/./dt0.mak
dt0.maks:
	bash yam.sh $(SRC) . dt0 > $(OUT)/./dt0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) dt0.maks_sub
dt0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . db0 > $(OUT)/./db0.mak
db0.maks:
	bash yam.sh $(SRC) . db0 > $(OUT)/./db0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) db0.maks_sub
db0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . wb0 > $(OUT)/./wb0.mak
wb0.maks:
	bash yam.sh $(SRC) . wb0 > $(OUT)/./wb0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) wb0.maks_sub
wb0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . wb3 > $(OUT)/./wb3.mak
wb3.maks:
	bash yam.sh $(SRC) . wb3 > $(OUT)/./wb3.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) wb3.maks_sub
wb3.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . wv0 > $(OUT)/./wv0.mak
wv0.maks:
	bash yam.sh $(SRC) . wv0 > $(OUT)/./wv0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) wv0.maks_sub
wv0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . wv4 > $(OUT)/./wv4.mak
wv4.maks:
	bash yam.sh $(SRC) . wv4 > $(OUT)/./wv4.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) wv4.maks_sub
wv4.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . wv5 > $(OUT)/./wv5.mak
wv5.maks:
	bash yam.sh $(SRC) . wv5 > $(OUT)/./wv5.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) wv5.maks_sub
wv5.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . dv0 > $(OUT)/./dv0.mak
dv0.maks:
	bash yam.sh $(SRC) . dv0 > $(OUT)/./dv0.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) dv0.maks_sub
dv0.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ./config/prj.yam
	bash yam.sh $(SRC) . bsd1 > $(OUT)/./bsd1.mak
bsd1.maks:
	bash yam.sh $(SRC) . bsd1 > $(OUT)/./bsd1.mak
	$(MAKE) -f $(SRC)/./makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks_sub
bsd1.maks_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) . default -DDTNODEPEND > $(OUT)/./makefile
	make yamsubs

yamsubs:
	bash yam.sh $(SRC) ./dtutil default -DDTNODEPEND > $(OUT)/./dtutil/makefile
	bash yam.sh $(SRC) ./include default -DDTNODEPEND > $(OUT)/./include/makefile
	bash yam.sh $(SRC) ./source default -DDTNODEPEND > $(OUT)/./source/makefile
	bash yam.sh $(SRC) ./server default -DDTNODEPEND > $(OUT)/./server/makefile
	bash yam.sh $(SRC) ./test default -DDTNODEPEND > $(OUT)/./test/makefile
	bash yam.sh $(SRC) ./tool default -DDTNODEPEND > $(OUT)/./tool/makefile
	cd dtutil; make yamsubs
	cd include; make yamsubs
	cd source; make yamsubs
	cd server; make yamsubs
	cd test; make yamsubs
	cd tool; make yamsubs

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
	  -f $(SRC)/./qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/./qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/./qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/. ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/./wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/./dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/./bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/./sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/./sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/./fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/./lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) deps
#----------------------------------------------------------------------------
doctidy:
	rm -f *.rtf *.hh *.hpj *.ref *.dx
	rm -f *.idx *.ind *.log *.dvi *.dvl *.aux *.blg *.bbl *.ltx
docclean: doctidy
	rm -f *.htm *.txi *.txt *.inf
	rm -f *.lj4 *.lj4.gz *.lj3 *.lj3.gz *.ps6 *.ps6.gz*.ps3 *.ps3.gz
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) docclean
#----------------------------------------------------------------------------
veryclean: docclean 
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd include; $(MAKE) -f $(SRC)/./include/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd source; $(MAKE) -f $(SRC)/./source/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd server; $(MAKE) -f $(SRC)/./server/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd test; $(MAKE) -f $(SRC)/./test/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd tool; $(MAKE) -f $(SRC)/./tool/makefile SRC=$(SRC) OUT=$(OUT) veryclean# end of default finish


dtutil_sub:
	cd dtutil; $(MAKE) -f $(SRC)/./dtutil/makefile SRC=$(SRC) OUT=$(OUT) $(MK_TARGET)



# --------------------------------------------------------------------------
figs:
	-@mkdir $(DOC)/doc/html/dtdoc
	cp $(SRC)/doctree1.xbm $(DOC)/doc/html/dtdoc

# --------------------------------------------------------------------------

tarall:
	make makefiles
	make sungnu.maks
	make dt.taz
tardvi:
	make dvis
	make dtdvi.taz
tarimg:
	make dtimg.taz
zipall:
	rm -f dt.list
	make maks
	make dt.zip

# --------------------------------------------------------------------------

dt.list: dir.yam
	find . \( -name "*.[cihpx]" -o \
                  -name "*.sh" -o \
                  -name "*.dx" -o \
                  -name "*.bat" -o \
                  -name "*.cfg" -o \
                  -name "*.awk" -o \
                  -name "*.doc" -o \
                  -name "*.use" -o \
                  -name "*.fig" -o \
                  -name "*.set" -o \
                  -name "*.rtf" -o \
                  -name "*.hh" -o \
                  -name "*.hpj" -o \
                  -name "*.hlp" -o \
                  -name "*.exe" -o \
                  -name "*.def" -o \
                  -name "*.rc" -o \
                  -name "*.ico" -o \
                  -name "*.bmp" -o \
                  -name "*.tim" -o \
                  -name "*.yam" -o \
                  -name "*.java" -o \
                  -name "makefile" -o \
                  -name "todo" -o \
                  -name "todo.dat" -o \
                  -name "log" -o \
                  -name "log.dat" -o \
                  -name "biblio" -o \
                  -name "biblio.dat" -o \
                  -name "doclist" -o \
                  -name "doclist.dat" -o \
                  -name "*.mak" \) \
	  -print >dt.list
dtsrc.list: dt.list
	egrep -v "[./]mak" dt.list > $@
dtmak.list: dt.list
	egrep    "[./]mak" dt.list > $@
dtdat.list: dir.yam
	find data -type f -print >$@

dtsrc.tar: dtsrc.list
	tar cf dtsrc.tar -I dtsrc.list
dtsrc.taz: dtsrc.tar
	rm -f $@
	gzip -9 -v -c dtsrc.tar >$@
	rm dtsrc.tar
	ls -l $@
dtsrc.pax: dtsrc.list
	cat dtsrc.list | pax -w >$@

dtsrc.paz: dtsrc.list
	rm -f $@
	cat dtsrc.list | pax -w | gzip -9 -v -c >$@
dtmak.paz: dtmak.list
	rm -f $@
	cat dtmak.list | pax -w | gzip -9 -v -c >$@
dtdat.paz: dtdat.list
	rm -f $@
	cat dtdat.list | pax -w | gzip -9 -v -c >$@

# --------------------------------------------------------------------------

# --------------------------------------------------------------------------

dtdvi.tar:
	find . -name "*.dvi" -print >dtdvi.list
	tar cf dtdvi.tar -I dtdvi.list
dtdvi.taz: dtdvi.tar
	rm -f $@
	gzip -9 -v -c dtdvi.tar > $@
	rm dtdvi.tar
	ls -l $@

# --------------------------------------------------------------------------

dtimg.tar:
	find doc/img -name "*.ps" -print >dtimg.list
	tar cf dtimg.tar -I dtimg.list
dtimg.taz: dtimg.tar
	rm -f $@
	gzip -9 -v -c dtimg.tar >$@
	rm dtimg.tar
	ls -l $@

# --------------------------------------------------------------------------

dt.zip: dt.list
	rm -f dt.zip
	zip -qr dt.zip - < dt.list
	ls -l dt.zip
dt.ziz: dt.list
	rm -f dt2.zip
	NOZIP=":";export NOZIP; zip -qrn dt2.zip - < dt.list
	ls -l dt2.zip
	rm -f dt2.zip.gz dt.ziz
	gzip dt2.zip
	mv dt2.zip.gz dt.ziz
	ls -l dt.ziz

# --------------------------------------------------------------------------
uucp: 
	make zipall
	rm -f dt.ziz.pgp
	pgp -c dt.ziz
	uucp dt.ziz.pgp fullfeed!nci!/tmp/xxx

# --------------------------------------------------------------------------
ymodem: 
	make zipall
	myymodem dt.ziz
# --------------------------------------------------------------------------
kermit: 
	make zipall
	kermit
# --------------------------------------------------------------------------
ftp: 
	make zipall
	cp dt.ziz /usr/pcftp
# --------------------------------------------------------------------------
DTDISK = /usr/erb/prj/dtdisk
disk: 
	rm -f dt.list
	make dtsrc.paz
	make dtmak.paz
	make dtdat.paz
	rm -f $(DTDISK)/*~
	cp dtsrc.paz $(DTDISK)/dtsrc.taz
	cp dtmak.paz $(DTDISK)/dtmak.taz
	cp dtdat.paz $(DTDISK)/dtdat.taz
	ls -lt $(DTDISK)/
dtgo:
	rcp $(DTDISK)/*.sh $(DTDISK)/*.bat $(DTDISK)/dt.zip iack:/$(DTDISK)
	touch dtgo
	rcp dtgo iack:/tmp/dtgo
	rm dtgo
# --------------------------------------------------------------------------
itemlist: price_html links_html down_html doc_html sales_html base_html

DTWEB = /usr/erb/prj/dtdoc/doc/html
DTWEB = /usr/dtack/root/web/docs

price_html:
	sh /usr/erb/prj/dt/sh/dtpage.sh price.htm > /usr/erb/prj/dtdoc/price.dx
	makedoc /usr/erb/prj/dtdoc/doc/html/dtprice/all.htm
	grep -v "</*t[ard]" /usr/erb/prj/dtdoc/doc/html/dtprice/all.htm > \
          $(DTWEB)/dtprice/all.htm
links_html:
	sh /usr/erb/prj/dt/sh/dtpage.sh links.htm > /usr/erb/prj/dtdoc/links.dx
	makedoc /usr/erb/prj/dtdoc/doc/html/dtlinks/all.htm
	grep -v "</*t[ard]" /usr/erb/prj/dtdoc/doc/html/dtlinks/all.htm > \
          $(DTWEB)/dtlinks/all.htm
down_html:
	sh /usr/erb/prj/dt/sh/dtdown.sh html > /usr/erb/prj/dtdoc/down.dx
	makedoc /usr/erb/prj/dtdoc/doc/html/dtdown/all.htm
	grep -v "</*t[ard]" /usr/erb/prj/dtdoc/doc/html/dtdown/all.htm > \
          $(DTWEB)/dtdown/all.htm
doc_html:
	sh /usr/erb/prj/dt/sh/dtpage.sh doc.htm > /usr/erb/prj/dtdoc/doc.dx
	makedoc /usr/erb/prj/dtdoc/doc/html/dtdoc/all.htm
	grep -v "</*t[ard]" /usr/erb/prj/dtdoc/doc/html/dtdoc/all.htm > \
          $(DTWEB)/dtdoc/all.htm
sales_html:
	sh /usr/erb/prj/dt/sh/dtpage.sh sales.htm > /usr/erb/prj/dtdoc/sales.dx
	makedoc /usr/erb/prj/dtdoc/doc/html/dtsales/all.htm
	grep -v "</*t[ard]" /usr/erb/prj/dtdoc/doc/html/dtsales/all.htm > \
          $(DTWEB)/dtsales/all.htm
base_html:
	sh /usr/erb/prj/dt/sh/dtpage.sh doc.htm bases > /usr/erb/prj/dtdoc/base.dx
	makedoc /usr/erb/prj/dtdoc/doc/html/dtbase/all.htm
	grep -v "</*t[ard]" /usr/erb/prj/dtdoc/doc/html/dtbase/all.htm > \
          $(DTWEB)/dtbase/all.htm

