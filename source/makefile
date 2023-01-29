


SRC = ..
OUT = ..

all: allsubs 

allsubs:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) all
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) all
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) all
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) all
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) all
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) all
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) all
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) all
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) all
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) all
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) all
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) all
#----------------------------------------------------------------------------
protos: 
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) protos
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) protos
#----------------------------------------------------------------------------

$(DT_YAM)/data/doclist.m4: $(DT_YAM)/data/doclist.dat
	bash dtdocm4.sh $(DT_YAM)

lj4s-: \
	  $(DOC)/lj4/libix.lj4 \
	  $(DOC)/lj4/imagedt.lj4
lj4s: lj4s-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) lj4s
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) lj4s

lj3s-: \
	  $(DOC)/PRJ_LJ3DIR/libix.lj3 \
	  $(DOC)/PRJ_LJ3DIR/imagedt.lj3
lj3s: lj3s-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) lj3s
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) lj3s

ps6s-: \
	  $(DOC)/PRJ_PS6DIR/libix.ps6 \
	  $(DOC)/PRJ_PS6DIR/imagedt.ps6
ps6s: ps6s-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) ps6s
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) ps6s

ps3s-: \
	  $(DOC)/PRJ_PS3DIR/libix.ps3 \
	  $(DOC)/PRJ_PS3DIR/imagedt.ps3
ps3s: ps3s-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) ps3s
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) ps3s

dvis-: \
	  $(DOC)/dvi/libix.dvi \
	  $(DOC)/dvi/imagedt.dvi
dvis: dvis-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) dvis
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) dvis

dvls-: \
	  $(DOC)/dvl/libix.dvl \
	  $(DOC)/dvl/imagedt.dvl
dvls: dvls-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) dvls
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) dvls

latexs-: \
	  $(DOC)/source/libix.ltx \
	  $(DOC)/source/imagedt.ltx
latexs: latexs-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) latexs
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) latexs

mans-: 
mans: mans-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) mans
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) mans

infos-: 
infos: infos-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) infos
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) infos

htmls-: \
	  $(DOC)/html/libix.htm \
	  $(DOC)/html/imagedt.htm
htmls: htmls-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) htmls
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) htmls

htm2s-: \
	  $(DOC)/html/libix/sect000.htm \
	  $(DOC)/html/imagedt/sect000.htm
htm2s: htm2s-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) htm2s
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) htm2s

texts-: \
	  $(DOC)/text/libix.txt \
	  $(DOC)/text/imagedt.txt
texts: texts-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) texts
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) texts

rtfs-: \
	  $(DOC)/PRJ_RTFDIR/libix.rtf \
	  $(DOC)/PRJ_RTFDIR/imagedt.rtf
rtfs: rtfs-
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) rtfs
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) rtfs


#-------------------------------------------------------------------------
# docs


docs: latexs dvls ps6s ps3s lj4s mans infos htmls htm2s
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) docs
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) docs



$(DOC)/lj4/libix.lj4: $(DOC)/PRJ_PS6DIR/libix.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/libix.ps6 $(DOC)/lj4/libix.lj4

$(DOC)/PRJ_LJ3DIR/libix.lj3: $(DOC)/PRJ_PS3DIR/libix.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/libix.ps3 $(DOC)/PRJ_LJ3DIR/libix.lj3

$(DOC)/PRJ_PS6DIR/libix.ps6: $(DOC)/dvl/libix.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source $(DOC)/dvl/libix.dvl $(DOC)/PRJ_PS6DIR/libix.ps6 1200

$(DOC)/PRJ_PS3DIR/libix.ps3: $(DOC)/dvl/libix.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source $(DOC)/dvl/libix.dvl $(DOC)/PRJ_PS3DIR/libix.ps3 300

$(DOC)/dvl/libix.dvl: $(DOC)/source/libix.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/libix.ltx \
	   $(DOC)/dvl/libix.dvl:utopia.ltx \
	   $(DOC)/dvi/libix.dvi:plain.ltx

$(DOC)/dvi/libix.dvi: $(DOC)/source/libix.ltx
	cd $(DOC)/source; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/libix.ltx \
	   $(DOC)/dvi/libix.dvi:plain.ltx

$(DOC)/source/libix.ltx: $(DOC)/source/libix.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/libix.dx \
	  $(DOC)/source/libix.ltx \
	  "libix" \
	  "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/libix.htm: $(DOC)/source/libix.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/libix.dx \
	  $(DOC)/html/libix.htm \
	  "libix" "PRJ_LIB_SERIES_SHORT"

$(DOC)/html/libix/sect000.htm: $(DOC)/source/libix.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/libix.dx \
	  $(DOC)/html/libix \
	  "libix" "PRJ_LIB_SERIES_SHORT"

$(DOC)/text/libix.txt: $(DOC)/source/libix.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/libix.dx \
	  $(DOC)/text/libix.txt \
	  "libix" "PRJ_LIB_SERIES_SHORT"

$(DOC)/PRJ_RTFDIR/libix.rtf: $(DOC)/source/libix.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/libix.dx \
	  $(DOC)/PRJ_RTFDIR/libix.rtf \
	  "libix" "PRJ_LIB_SERIES_SHORT"


$(DOC)/source/libix.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/libix.doc
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source \
	  -Dfilebase=libix \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/libix.doc > $(DOC)/source/libix.dx
	@bash dtpics.sh $(SRC)/source $(DOC)/source/libix.dx $(DOC)/source





$(DOC)/lj4/imagedt.lj4: $(DOC)/PRJ_PS6DIR/imagedt.ps6
	bash $(SRC)/sh/dtps2lj4.sh $(DOC)/PRJ_PS6DIR/imagedt.ps6 $(DOC)/lj4/imagedt.lj4

$(DOC)/PRJ_LJ3DIR/imagedt.lj3: $(DOC)/PRJ_PS3DIR/imagedt.ps3
	bash $(SRC)/sh/dtps2lj3.sh $(DOC)/PRJ_PS3DIR/imagedt.ps3 $(DOC)/PRJ_LJ3DIR/imagedt.lj3

$(DOC)/PRJ_PS6DIR/imagedt.ps6: $(DOC)/dvl/imagedt.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source $(DOC)/dvl/imagedt.dvl $(DOC)/PRJ_PS6DIR/imagedt.ps6 1200

$(DOC)/PRJ_PS3DIR/imagedt.ps3: $(DOC)/dvl/imagedt.dvl
	bash $(SRC)/sh/dtdvl2ps.sh $(DOC)/source $(DOC)/dvl/imagedt.dvl $(DOC)/PRJ_PS3DIR/imagedt.ps3 300

$(DOC)/dvl/imagedt.dvl: $(DOC)/source/imagedt.ltx $(DT_YAM)/data/utopia.ltx
	cd $(DOC)/source; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/imagedt.ltx \
	   $(DOC)/dvl/imagedt.dvl:utopia.ltx \
	   $(DOC)/dvi/imagedt.dvi:plain.ltx

$(DOC)/dvi/imagedt.dvi: $(DOC)/source/imagedt.ltx
	cd $(DOC)/source; \
	bash dtlatex.sh $(DT_YAM) $(SRC) $(DOC)/source/imagedt.ltx \
	   $(DOC)/dvi/imagedt.dvi:plain.ltx

$(DOC)/source/imagedt.ltx: $(DOC)/source/imagedt.dx
	bash dtdx2ltx.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/imagedt.dx \
	  $(DOC)/source/imagedt.ltx \
	  "imagedt" \
	  "PRJ_PRODUCT_SERIES"

$(DOC)/html/imagedt.htm: $(DOC)/source/imagedt.dx
	@bash dtdx2htm.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/imagedt.dx \
	  $(DOC)/html/imagedt.htm \
	  "imagedt" "PRJ_PRODUCT_SERIES"

$(DOC)/html/imagedt/sect000.htm: $(DOC)/source/imagedt.dx
	bash dtdx2ht2.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/imagedt.dx \
	  $(DOC)/html/imagedt \
	  "imagedt" "PRJ_PRODUCT_SERIES"

$(DOC)/text/imagedt.txt: $(DOC)/source/imagedt.dx
	bash dtdx2txt.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/imagedt.dx \
	  $(DOC)/text/imagedt.txt \
	  "imagedt" "PRJ_PRODUCT_SERIES"

$(DOC)/PRJ_RTFDIR/imagedt.rtf: $(DOC)/source/imagedt.dx
	bash dtdx2rtf.sh guide \
	  $(DT_YAM) \
	  $(SRC) \
	  $(DOC)/source/imagedt.dx \
	  $(DOC)/PRJ_RTFDIR/imagedt.rtf \
	  "imagedt" "PRJ_PRODUCT_SERIES"


$(DOC)/source/imagedt.dx: $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/imagedt.doc
	@bash dtinclude.sh \
	  -I$(SRC)/doc/include \
	  -I$(DOC)/source \
	  -Dfilebase=imagedt \
	  $(DT_YAM)/data/doclist.m4 \
	  $(SRC)/source/imagedt.doc > $(DOC)/source/imagedt.dx
	@bash dtpics.sh $(SRC)/source $(DOC)/source/imagedt.dx $(DOC)/source






#----------------------------------------------------------------------------

examples: 
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) examples
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) examples

usages: 
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) usages
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) usages

#-------------------------------------------------------------------------
cis:
	find . -type d ! -name . -prune -o \
	  \( -name "*.[chx]" -o -name "dir.yam" \) \
	  -exec ci -l {} </dev/null \;
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) cis
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) cis
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
	bash yam.sh $(SRC) source default > $(OUT)/source/makefile
makefiles:
	bash yam.sh $(SRC) source default > $(OUT)/source/makefile
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) makefiles_sub
makefiles_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) makefiles
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) makefiles

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source qw0 > $(OUT)/source/qw0.mak
qw0.maks:
	bash yam.sh $(SRC) source qw0 > $(OUT)/source/qw0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks_sub
qw0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) qw0.maks
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source qw1 > $(OUT)/source/qw1.mak
qw1.maks:
	bash yam.sh $(SRC) source qw1 > $(OUT)/source/qw1.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks_sub
qw1.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) qw1.maks
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source qw6 > $(OUT)/source/qw6.mak
qw6.maks:
	bash yam.sh $(SRC) source qw6 > $(OUT)/source/qw6.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks_sub
qw6.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) qw6.maks
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source dw0 > $(OUT)/source/dw0.mak
dw0.maks:
	bash yam.sh $(SRC) source dw0 > $(OUT)/source/dw0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks_sub
dw0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) dw0.maks
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source ww0 > $(OUT)/source/ww0.mak
ww0.maks:
	bash yam.sh $(SRC) source ww0 > $(OUT)/source/ww0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks_sub
ww0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) ww0.maks
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source dw1 > $(OUT)/source/dw1.mak
dw1.maks:
	bash yam.sh $(SRC) source dw1 > $(OUT)/source/dw1.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks_sub
dw1.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) dw1.maks
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source dt0 > $(OUT)/source/dt0.mak
dt0.maks:
	bash yam.sh $(SRC) source dt0 > $(OUT)/source/dt0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks_sub
dt0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) dt0.maks
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source db0 > $(OUT)/source/db0.mak
db0.maks:
	bash yam.sh $(SRC) source db0 > $(OUT)/source/db0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) db0.maks_sub
db0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) db0.maks
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source wb0 > $(OUT)/source/wb0.mak
wb0.maks:
	bash yam.sh $(SRC) source wb0 > $(OUT)/source/wb0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks_sub
wb0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) wb0.maks
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source wb3 > $(OUT)/source/wb3.mak
wb3.maks:
	bash yam.sh $(SRC) source wb3 > $(OUT)/source/wb3.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks_sub
wb3.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) wb3.maks
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source wv0 > $(OUT)/source/wv0.mak
wv0.maks:
	bash yam.sh $(SRC) source wv0 > $(OUT)/source/wv0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks_sub
wv0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) wv0.maks
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source wv4 > $(OUT)/source/wv4.mak
wv4.maks:
	bash yam.sh $(SRC) source wv4 > $(OUT)/source/wv4.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks_sub
wv4.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) wv4.maks
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source wv5 > $(OUT)/source/wv5.mak
wv5.maks:
	bash yam.sh $(SRC) source wv5 > $(OUT)/source/wv5.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks_sub
wv5.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) wv5.maks
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source dv0 > $(OUT)/source/dv0.mak
dv0.maks:
	bash yam.sh $(SRC) source dv0 > $(OUT)/source/dv0.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks_sub
dv0.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) dv0.maks
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../config/prj.yam
	bash yam.sh $(SRC) source bsd1 > $(OUT)/source/bsd1.mak
bsd1.maks:
	bash yam.sh $(SRC) source bsd1 > $(OUT)/source/bsd1.mak
	$(MAKE) -f $(SRC)/source/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks_sub
bsd1.maks_sub:
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) bsd1.maks

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) source default -DDTNODEPEND > $(OUT)/source/makefile
	make yamsubs

yamsubs:
	bash yam.sh $(SRC) source/1d default -DDTNODEPEND > $(OUT)/source/1d/makefile
	bash yam.sh $(SRC) source/low default -DDTNODEPEND > $(OUT)/source/low/makefile
	bash yam.sh $(SRC) source/dg default -DDTNODEPEND > $(OUT)/source/dg/makefile
	bash yam.sh $(SRC) source/bigmem default -DDTNODEPEND > $(OUT)/source/bigmem/makefile
	bash yam.sh $(SRC) source/keybd default -DDTNODEPEND > $(OUT)/source/keybd/makefile
	bash yam.sh $(SRC) source/inter default -DDTNODEPEND > $(OUT)/source/inter/makefile
	bash yam.sh $(SRC) source/screen default -DDTNODEPEND > $(OUT)/source/screen/makefile
	bash yam.sh $(SRC) source/win default -DDTNODEPEND > $(OUT)/source/win/makefile
	bash yam.sh $(SRC) source/color default -DDTNODEPEND > $(OUT)/source/color/makefile
	bash yam.sh $(SRC) source/high default -DDTNODEPEND > $(OUT)/source/high/makefile
	bash yam.sh $(SRC) source/xchg default -DDTNODEPEND > $(OUT)/source/xchg/makefile
	bash yam.sh $(SRC) source/fb default -DDTNODEPEND > $(OUT)/source/fb/makefile
	cd 1d; make yamsubs
	cd low; make yamsubs
	cd dg; make yamsubs
	cd bigmem; make yamsubs
	cd keybd; make yamsubs
	cd inter; make yamsubs
	cd screen; make yamsubs
	cd win; make yamsubs
	cd color; make yamsubs
	cd high; make yamsubs
	cd xchg; make yamsubs
	cd fb; make yamsubs

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
	  -f $(SRC)/source/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/source/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/source/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/source ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/source/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/source/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/source/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/source/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/source/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/source/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/source/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos



#----------------------------------------------------------------------------
deps: 
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) deps
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) deps
#----------------------------------------------------------------------------
doctidy:
	rm -f *.rtf *.hh *.hpj *.ref *.dx
	rm -f *.idx *.ind *.log *.dvi *.dvl *.aux *.blg *.bbl *.ltx
docclean: doctidy
	rm -f *.htm *.txi *.txt *.inf
	rm -f *.lj4 *.lj4.gz *.lj3 *.lj3.gz *.ps6 *.ps6.gz*.ps3 *.ps3.gz
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) docclean
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) docclean
#----------------------------------------------------------------------------
veryclean: docclean 
	cd 1d; $(MAKE) -f $(SRC)/source/1d/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd low; $(MAKE) -f $(SRC)/source/low/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd dg; $(MAKE) -f $(SRC)/source/dg/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd bigmem; $(MAKE) -f $(SRC)/source/bigmem/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd keybd; $(MAKE) -f $(SRC)/source/keybd/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd inter; $(MAKE) -f $(SRC)/source/inter/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd screen; $(MAKE) -f $(SRC)/source/screen/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd win; $(MAKE) -f $(SRC)/source/win/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd color; $(MAKE) -f $(SRC)/source/color/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd high; $(MAKE) -f $(SRC)/source/high/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd xchg; $(MAKE) -f $(SRC)/source/xchg/makefile SRC=$(SRC) OUT=$(OUT) veryclean
	cd fb; $(MAKE) -f $(SRC)/source/fb/makefile SRC=$(SRC) OUT=$(OUT) veryclean# end of default finish
