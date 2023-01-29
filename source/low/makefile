


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
	  $(DOC)/lj4/utilq.lj4 \
	  $(DOC)/lj4/util.lj4 \
	  $(DOC)/lj4/hist.lj4 \
	  $(DOC)/lj4/env.lj4 \
	  $(DOC)/lj4/param.lj4
lj4s: lj4s-


lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/utilq.lj3 \
	  $(DOC)/PRJ_LJ3DIR/util.lj3 \
	  $(DOC)/PRJ_LJ3DIR/hist.lj3 \
	  $(DOC)/PRJ_LJ3DIR/env.lj3 \
	  $(DOC)/PRJ_LJ3DIR/param.lj3
lj3s: lj3s-


ps6s-: \
	  $(DOC)/PRJ_PS6DIR/utilq.ps6 \
	  $(DOC)/PRJ_PS6DIR/util.ps6 \
	  $(DOC)/PRJ_PS6DIR/hist.ps6 \
	  $(DOC)/PRJ_PS6DIR/env.ps6 \
	  $(DOC)/PRJ_PS6DIR/param.ps6
ps6s: ps6s-


ps3s-: \
	  $(DOC)/PRJ_PS3DIR/utilq.ps3 \
	  $(DOC)/PRJ_PS3DIR/util.ps3 \
	  $(DOC)/PRJ_PS3DIR/hist.ps3 \
	  $(DOC)/PRJ_PS3DIR/env.ps3 \
	  $(DOC)/PRJ_PS3DIR/param.ps3
ps3s: ps3s-


dvis-: \
	  $(DOC)/dvi/utilq.dvi \
	  $(DOC)/dvi/util.dvi \
	  $(DOC)/dvi/hist.dvi \
	  $(DOC)/dvi/env.dvi \
	  $(DOC)/dvi/param.dvi
dvis: dvis-


dvls-: \
	  $(DOC)/dvl/utilq.dvl \
	  $(DOC)/dvl/util.dvl \
	  $(DOC)/dvl/hist.dvl \
	  $(DOC)/dvl/env.dvl \
	  $(DOC)/dvl/param.dvl
dvls: dvls-


latexs-: \
	  $(DOC)/source/low/utilq.ltx \
	  $(DOC)/source/low/util.ltx \
	  $(DOC)/source/low/hist.ltx \
	  $(DOC)/source/low/env.ltx \
	  $(DOC)/source/low/param.ltx
latexs: latexs-


mans-: 
mans: mans-


infos-: 
infos: infos-


htmls-: \
	  $(DOC)/html/utilq.htm \
	  $(DOC)/html/util.htm \
	  $(DOC)/html/hist.htm \
	  $(DOC)/html/env.htm \
	  $(DOC)/html/param.htm
htmls: htmls-


htm2s-: \
	  $(DOC)/html/utilq/sect000.htm \
	  $(DOC)/html/util/sect000.htm \
	  $(DOC)/html/hist/sect000.htm \
	  $(DOC)/html/env/sect000.htm \
	  $(DOC)/html/param/sect000.htm
htm2s: htm2s-


texts-: \
	  $(DOC)/text/utilq.txt \
	  $(DOC)/text/util.txt \
	  $(DOC)/text/hist.txt \
	  $(DOC)/text/env.txt \
	  $(DOC)/text/param.txt
texts: texts-


rtfs-: \
	  $(DOC)/PRJ_RTFDIR/utilq.rtf \
	  $(DOC)/PRJ_RTFDIR/util.rtf \
	  $(DOC)/PRJ_RTFDIR/hist.rtf \
	  $(DOC)/PRJ_RTFDIR/env.rtf \
	  $(DOC)/PRJ_RTFDIR/param.rtf
rtfs: rtfs-



#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s




$(DOC)/lj4/utilq.lj4: $(DOC)/PRJ_PS6DIR/utilq.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/utilq.ps6 $(DOC)/lj4/utilq.lj4

$(DOC)/PRJ_LJ3DIR/utilq.lj3: $(DOC)/PRJ_PS3DIR/utilq.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/utilq.ps3 $(DOC)/PRJ_LJ3DIR/utilq.lj3

$(DOC)/PRJ_PS6DIR/utilq.ps6: $(DOC)/dvl/utilq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/utilq.dvl $(DOC)/PRJ_PS6DIR/utilq.ps6 1200

$(DOC)/PRJ_PS3DIR/utilq.ps3: $(DOC)/dvl/utilq.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/utilq.dvl $(DOC)/PRJ_PS3DIR/utilq.ps3 300

$(DOC)/dvl/utilq.dvl: $(DOC)/source/low/utilq.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/utilq.ltx \
	   $(DOC)/dvl/utilq.dvl:utopia.ltx \
	   $(DOC)/dvi/utilq.dvi:plain.ltx

$(DOC)/dvi/utilq.dvi: $(DOC)/source/low/utilq.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/utilq.ltx \
	   $(DOC)/dvi/utilq.dvi:plain.ltx

$(DOC)/source/low/utilq.ltx: $(DOC)/source/low/utilq.dx
	bash dtdx2ltx.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/utilq.dx \
	  $(DOC)/source/low/utilq.ltx \
	  "utilq" \
	  "PRJ_SERIES"

$(DOC)/html/utilq.htm: $(DOC)/source/low/utilq.dx
	@bash dtdx2htm.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/utilq.dx \
	  $(DOC)/html/utilq.htm \
	  "utilq" "PRJ_SERIES"

$(DOC)/html/utilq/sect000.htm: $(DOC)/source/low/utilq.dx
	bash dtdx2ht2.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/utilq.dx \
	  $(DOC)/html/utilq \
	  "utilq" "PRJ_SERIES"

$(DOC)/text/utilq.txt: $(DOC)/source/low/utilq.dx
	bash dtdx2txt.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/utilq.dx \
	  $(DOC)/text/utilq.txt \
	  "utilq" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/utilq.rtf: $(DOC)/source/low/utilq.dx
	bash dtdx2rtf.sh summary \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/utilq.dx \
	  $(DOC)/PRJ_RTFDIR/utilq.rtf \
	  "utilq" "PRJ_SERIES"


$(DOC)/source/low/utilq.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/util.doc \
	  $(SRC)/source/low/fd.c \
	  $(SRC)/source/low/list.c \
	  $(SRC)/source/low/str.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/low \
	  -Dfilebase=utilq \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/util.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/fd.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/list.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/str.c > $(DOC)/source/low/utilq.dx
	@bash dtpics.sh $(SRC)/source/low $(DOC)/source/low/utilq.dx $(DOC)/source/low




$(DOC)/lj4/util.lj4: $(DOC)/PRJ_PS6DIR/util.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/util.ps6 $(DOC)/lj4/util.lj4

$(DOC)/PRJ_LJ3DIR/util.lj3: $(DOC)/PRJ_PS3DIR/util.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/util.ps3 $(DOC)/PRJ_LJ3DIR/util.lj3

$(DOC)/PRJ_PS6DIR/util.ps6: $(DOC)/dvl/util.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/util.dvl $(DOC)/PRJ_PS6DIR/util.ps6 1200

$(DOC)/PRJ_PS3DIR/util.ps3: $(DOC)/dvl/util.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/util.dvl $(DOC)/PRJ_PS3DIR/util.ps3 300

$(DOC)/dvl/util.dvl: $(DOC)/source/low/util.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/util.ltx \
	   $(DOC)/dvl/util.dvl:utopia.ltx \
	   $(DOC)/dvi/util.dvi:plain.ltx

$(DOC)/dvi/util.dvi: $(DOC)/source/low/util.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/util.ltx \
	   $(DOC)/dvi/util.dvi:plain.ltx

$(DOC)/source/low/util.ltx: $(DOC)/source/low/util.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/util.dx \
	  $(DOC)/source/low/util.ltx \
	  "util" \
	  "PRJ_SERIES"

$(DOC)/html/util.htm: $(DOC)/source/low/util.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/util.dx \
	  $(DOC)/html/util.htm \
	  "util" "PRJ_SERIES"

$(DOC)/html/util/sect000.htm: $(DOC)/source/low/util.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/util.dx \
	  $(DOC)/html/util \
	  "util" "PRJ_SERIES"

$(DOC)/text/util.txt: $(DOC)/source/low/util.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/util.dx \
	  $(DOC)/text/util.txt \
	  "util" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/util.rtf: $(DOC)/source/low/util.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/util.dx \
	  $(DOC)/PRJ_RTFDIR/util.rtf \
	  "util" "PRJ_SERIES"


$(DOC)/source/low/util.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/util.doc \
	  $(SRC)/source/low/fd.c \
	  $(SRC)/source/low/list.c \
	  $(SRC)/source/low/str.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/low \
	  -Dfilebase=util \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/util.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/fd.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/list.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/str.c > $(DOC)/source/low/util.dx
	@bash dtpics.sh $(SRC)/source/low $(DOC)/source/low/util.dx $(DOC)/source/low





$(DOC)/lj4/hist.lj4: $(DOC)/PRJ_PS6DIR/hist.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/hist.ps6 $(DOC)/lj4/hist.lj4

$(DOC)/PRJ_LJ3DIR/hist.lj3: $(DOC)/PRJ_PS3DIR/hist.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/hist.ps3 $(DOC)/PRJ_LJ3DIR/hist.lj3

$(DOC)/PRJ_PS6DIR/hist.ps6: $(DOC)/dvl/hist.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/hist.dvl $(DOC)/PRJ_PS6DIR/hist.ps6 1200

$(DOC)/PRJ_PS3DIR/hist.ps3: $(DOC)/dvl/hist.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/hist.dvl $(DOC)/PRJ_PS3DIR/hist.ps3 300

$(DOC)/dvl/hist.dvl: $(DOC)/source/low/hist.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/hist.ltx \
	   $(DOC)/dvl/hist.dvl:utopia.ltx \
	   $(DOC)/dvi/hist.dvi:plain.ltx

$(DOC)/dvi/hist.dvi: $(DOC)/source/low/hist.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/hist.ltx \
	   $(DOC)/dvi/hist.dvi:plain.ltx

$(DOC)/source/low/hist.ltx: $(DOC)/source/low/hist.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/hist.dx \
	  $(DOC)/source/low/hist.ltx \
	  "hist" \
	  "PRJ_SERIES"

$(DOC)/html/hist.htm: $(DOC)/source/low/hist.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/hist.dx \
	  $(DOC)/html/hist.htm \
	  "hist" "PRJ_SERIES"

$(DOC)/html/hist/sect000.htm: $(DOC)/source/low/hist.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/hist.dx \
	  $(DOC)/html/hist \
	  "hist" "PRJ_SERIES"

$(DOC)/text/hist.txt: $(DOC)/source/low/hist.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/hist.dx \
	  $(DOC)/text/hist.txt \
	  "hist" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/hist.rtf: $(DOC)/source/low/hist.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/hist.dx \
	  $(DOC)/PRJ_RTFDIR/hist.rtf \
	  "hist" "PRJ_SERIES"


$(DOC)/source/low/hist.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/hist.doc \
	  $(SRC)/source/low/hist.c \
	  $(SRC)/source/low/hist2.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/low \
	  -Dfilebase=hist \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/hist.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/hist.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/hist2.c > $(DOC)/source/low/hist.dx
	@bash dtpics.sh $(SRC)/source/low $(DOC)/source/low/hist.dx $(DOC)/source/low





$(DOC)/lj4/env.lj4: $(DOC)/PRJ_PS6DIR/env.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/env.ps6 $(DOC)/lj4/env.lj4

$(DOC)/PRJ_LJ3DIR/env.lj3: $(DOC)/PRJ_PS3DIR/env.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/env.ps3 $(DOC)/PRJ_LJ3DIR/env.lj3

$(DOC)/PRJ_PS6DIR/env.ps6: $(DOC)/dvl/env.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/env.dvl $(DOC)/PRJ_PS6DIR/env.ps6 1200

$(DOC)/PRJ_PS3DIR/env.ps3: $(DOC)/dvl/env.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/env.dvl $(DOC)/PRJ_PS3DIR/env.ps3 300

$(DOC)/dvl/env.dvl: $(DOC)/source/low/env.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/env.ltx \
	   $(DOC)/dvl/env.dvl:utopia.ltx \
	   $(DOC)/dvi/env.dvi:plain.ltx

$(DOC)/dvi/env.dvi: $(DOC)/source/low/env.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/env.ltx \
	   $(DOC)/dvi/env.dvi:plain.ltx

$(DOC)/source/low/env.ltx: $(DOC)/source/low/env.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/env.dx \
	  $(DOC)/source/low/env.ltx \
	  "env" \
	  "PRJ_SERIES"

$(DOC)/html/env.htm: $(DOC)/source/low/env.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/env.dx \
	  $(DOC)/html/env.htm \
	  "env" "PRJ_SERIES"

$(DOC)/html/env/sect000.htm: $(DOC)/source/low/env.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/env.dx \
	  $(DOC)/html/env \
	  "env" "PRJ_SERIES"

$(DOC)/text/env.txt: $(DOC)/source/low/env.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/env.dx \
	  $(DOC)/text/env.txt \
	  "env" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/env.rtf: $(DOC)/source/low/env.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/env.dx \
	  $(DOC)/PRJ_RTFDIR/env.rtf \
	  "env" "PRJ_SERIES"


$(DOC)/source/low/env.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/env.doc \
	  $(SRC)/source/low/err.c \
	  $(SRC)/source/low/say.c \
	  $(SRC)/source/low/dbg.c \
	  $(SRC)/source/low/dbg2.c \
	  $(SRC)/source/low/heapmon.c \
	  $(SRC)/source/low/leave.c \
	  $(SRC)/source/low/enter.c \
	  $(SRC)/source/low/assert.c \
	  $(SRC)/source/low/yield.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/low \
	  -Dfilebase=env \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/env.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/err.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/say.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/dbg.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/dbg2.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/heapmon.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/leave.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/enter.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/assert.c $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/yield.c > $(DOC)/source/low/env.dx
	@bash dtpics.sh $(SRC)/source/low $(DOC)/source/low/env.dx $(DOC)/source/low





$(DOC)/lj4/param.lj4: $(DOC)/PRJ_PS6DIR/param.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/param.ps6 $(DOC)/lj4/param.lj4

$(DOC)/PRJ_LJ3DIR/param.lj3: $(DOC)/PRJ_PS3DIR/param.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/param.ps3 $(DOC)/PRJ_LJ3DIR/param.lj3

$(DOC)/PRJ_PS6DIR/param.ps6: $(DOC)/dvl/param.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/param.dvl $(DOC)/PRJ_PS6DIR/param.ps6 1200

$(DOC)/PRJ_PS3DIR/param.ps3: $(DOC)/dvl/param.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source/low $(DOC)/dvl/param.dvl $(DOC)/PRJ_PS3DIR/param.ps3 300

$(DOC)/dvl/param.dvl: $(DOC)/source/low/param.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/param.ltx \
	   $(DOC)/dvl/param.dvl:utopia.ltx \
	   $(DOC)/dvi/param.dvi:plain.ltx

$(DOC)/dvi/param.dvi: $(DOC)/source/low/param.ltx
	cd $(DOC)/source/low; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/low/param.ltx \
	   $(DOC)/dvi/param.dvi:plain.ltx

$(DOC)/source/low/param.ltx: $(DOC)/source/low/param.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/param.dx \
	  $(DOC)/source/low/param.ltx \
	  "param" \
	  "PRJ_SERIES"

$(DOC)/html/param.htm: $(DOC)/source/low/param.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/param.dx \
	  $(DOC)/html/param.htm \
	  "param" "PRJ_SERIES"

$(DOC)/html/param/sect000.htm: $(DOC)/source/low/param.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/param.dx \
	  $(DOC)/html/param \
	  "param" "PRJ_SERIES"

$(DOC)/text/param.txt: $(DOC)/source/low/param.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/param.dx \
	  $(DOC)/text/param.txt \
	  "param" "PRJ_SERIES"

$(DOC)/PRJ_RTFDIR/param.rtf: $(DOC)/source/low/param.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/low/param.dx \
	  $(DOC)/PRJ_RTFDIR/param.rtf \
	  "param" "PRJ_SERIES"


$(DOC)/source/low/param.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/param.doc \
	  $(SRC)/source/low/param.c
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source/low \
	  -Dfilebase=param \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/low/param.doc $(SRC)/doc/include/srcbreak.dx  \
	  $(SRC)/source/low/param.c > $(DOC)/source/low/param.dx
	@bash dtpics.sh $(SRC)/source/low $(DOC)/source/low/param.dx $(DOC)/source/low



$(SRC)/source/low/env1.c: $(SRC)/source/low/env.doc
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/low/env1.c $(SRC)/source/low/env.doc
$(SRC)/source/low/hist1.c: $(SRC)/source/low/hist.doc
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/low/hist1.c $(SRC)/source/low/hist.doc
$(SRC)/source/low/param1.c: $(SRC)/source/low/param.doc
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/low/param1.c $(SRC)/source/low/param.doc
$(SRC)/source/low/port1.c: $(SRC)/source/low/portio.c
	bash dtdx.sh $(SRC) -fexample -o$(SRC)/source/low/port1.c $(SRC)/source/low/portio.c




#----------------------------------------------------------------------------

examples: \
	  $(SRC)/source/low/env1.c \
	  $(SRC)/source/low/hist1.c \
	  $(SRC)/source/low/param1.c \
	  $(SRC)/source/low/port1.c


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
	bash yam.sh $(SRC) source/low default > $(OUT)/source/low/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low qw0 > $(OUT)/source/low/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low qw1 > $(OUT)/source/low/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low qw6 > $(OUT)/source/low/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low dw0 > $(OUT)/source/low/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low ww0 > $(OUT)/source/low/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low dw1 > $(OUT)/source/low/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low dt0 > $(OUT)/source/low/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low db0 > $(OUT)/source/low/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low wb0 > $(OUT)/source/low/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low wb3 > $(OUT)/source/low/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low wv0 > $(OUT)/source/low/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low wv4 > $(OUT)/source/low/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low wv5 > $(OUT)/source/low/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low dv0 > $(OUT)/source/low/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) source/low bsd1 > $(OUT)/source/low/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source/low default -DDTNODEPEND > $(OUT)/source/low/makefile
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
	  -f $(SRC)/source/low/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/low/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/low/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source/low ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/low/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/low/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/low/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/low/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/low/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/low/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/low/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	bash dtdep.sh $(SRC)/source/low \
	  $(SRC)/source/low/dtenv1.c \
	  $(SRC)/source/low/dtenv2.c \
	  $(SRC)/source/low/dtenv3.c \
	  $(SRC)/source/low/dtenv4.c \
	  $(SRC)/source/low/dthist1.c \
	  $(SRC)/source/low/dthist2.c \
	  $(SRC)/source/low/dtparam1.c \
	  $(SRC)/source/low/dtport1.c \
	  $(SRC)/source/low/hist2.c \
	  $(SRC)/source/low/enter.c \
	  $(SRC)/source/low/ipcqnx4.c \
	  $(SRC)/source/low/ipcqbaby.c \
	  $(SRC)/source/low/ipcrock.c \
	  $(SRC)/source/low/license.c \
	  $(SRC)/source/low/fd.c \
	  $(SRC)/source/low/f.c \
	  $(SRC)/source/low/cipher.c \
	  $(SRC)/source/low/prj_dat.c \
	  $(SRC)/source/low/dbtable.c \
	  $(SRC)/source/low/points.c \
	  $(SRC)/source/low/poly.c \
	  $(SRC)/source/low/matrix3d.c \
	  $(SRC)/source/low/matrix2d.c \
	  $(SRC)/source/low/matrix.c \
	  $(SRC)/source/low/assert.c \
	  $(SRC)/source/low/cons.c \
	  $(SRC)/source/low/cstack.c \
	  $(SRC)/source/low/dbg.c \
	  $(SRC)/source/low/dbg2.c \
	  $(SRC)/source/low/err.c \
	  $(SRC)/source/low/err2.c \
	  $(SRC)/source/low/eventw.c \
	  $(SRC)/source/low/ftp.c \
	  $(SRC)/source/low/heapmon.c \
	  $(SRC)/source/low/heap2.c \
	  $(SRC)/source/low/hist.c \
	  $(SRC)/source/low/hist_da.c \
	  $(SRC)/source/low/ipc.c \
	  $(SRC)/source/low/ipcdde.c \
	  $(SRC)/source/low/leave.c \
	  $(SRC)/source/low/liner.c \
	  $(SRC)/source/low/list.c \
	  $(SRC)/source/low/list_io.c \
	  $(SRC)/source/low/lut8.c \
	  $(SRC)/source/low/lut8_mat.c \
	  $(SRC)/source/low/msgu.c \
	  $(SRC)/source/low/msw.c \
	  $(SRC)/source/low/os_mem.c \
	  $(SRC)/source/low/os_delay.c \
	  $(SRC)/source/low/os_futil.c \
	  $(SRC)/source/low/os_heap.c \
	  $(SRC)/source/low/os_heap2.c \
	  $(SRC)/source/low/os_time.c \
	  $(SRC)/source/low/os_env.c \
	  $(SRC)/source/low/param.c \
	  $(SRC)/source/low/phys.c \
	  $(SRC)/source/low/pnm.c \
	  $(SRC)/source/low/portio.c \
	  $(SRC)/source/low/prj.c \
	  $(SRC)/source/low/qnx4.c \
	  $(SRC)/source/low/rev.c \
	  $(SRC)/source/low/say.c \
	  $(SRC)/source/low/semu.c \
	  $(SRC)/source/low/semw.c \
	  $(SRC)/source/low/serial.c \
	  $(SRC)/source/low/serialu.c \
	  $(SRC)/source/low/serialw.c \
	  $(SRC)/source/low/shmu.c \
	  $(SRC)/source/low/shmw.c \
	  $(SRC)/source/low/str.c \
	  $(SRC)/source/low/threads.c \
	  $(SRC)/source/low/thread.c \
	  $(SRC)/source/low/threadu.c \
	  $(SRC)/source/low/threadw.c \
	  $(SRC)/source/low/usage.c \
	  $(SRC)/source/low/yield.c \
	  $(SRC)/source/low/libmain.c \
	  $(SRC)/source/low/dbg_dat.c \
	  $(SRC)/source/low/conf_dat.c \
	  $(SRC)/source/low/lut8_dat.c \
	  $(SRC)/source/low/ntype.c \
	  $(SRC)/source/low/ntype_da.c

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
