


SRC = ../..
OUT = ../..

all: exes 

allsubs:

#----------------------------------------------------------------------------
protos: \
	  t.p \
	  base.p \
	  db.p \
	  threads.p \
	  thread.p \
	  serial.p \
	  points.p \
	  poly.p \
	  matrix.p \
	  sem.p \
	  event.p \
	  shm.p \
	  msg.p \
	  cstack.p \
	  heap.p \
	  liner.p \
	  prj.p \
	  phys.p \
	  portio.p \
	  stack.p \
	  param.p \
	  hist.p \
	  rev.p \
	  font.p \
	  umdec.p \
	  ustate.p \
	  reverb.p \
	  vidmux.p \
	  fb.p \
	  color.p \
	  mem.p \
	  1d.p \
	  ftp.p \
	  ipc.p \
	  dg.p \
	  cons.p \
	  os.p \
	  str.p \
	  fd.p \
	  f.p \
	  pnm.p \
	  lut8.p \
	  qnx4.p \
	  cipher.p \
	  list.p \
	  dsk.p \
	  dsk_c.p \
	  xchg.p \
	  wmf.p \
	  bmp.p \
	  dib.p \
	  run.p \
	  runf.p \
	  image.p \
	  canvas.p \
	  vga.p \
	  hws.p \
	  scr.p \
	  trmios.p \
	  keybd.p \
	  keymap.p \
	  iact.p \
	  note.p \
	  alpha.p \
	  dialog.p \
	  msw.p \
	  text.p \
	  winc.p \
	  svc.p \
	  wins.p

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
	bash yam.sh $(SRC) include/dtack default > $(OUT)/include/dtack/makefile
makefiles: makefile

qw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack qw0 > $(OUT)/include/dtack/qw0.mak
qw0.maks: qw0.mak
	
qw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack qw1 > $(OUT)/include/dtack/qw1.mak
qw1.maks: qw1.mak
	
qw6.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack qw6 > $(OUT)/include/dtack/qw6.mak
qw6.maks: qw6.mak
	
dw0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack dw0 > $(OUT)/include/dtack/dw0.mak
dw0.maks: dw0.mak
	
ww0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack ww0 > $(OUT)/include/dtack/ww0.mak
ww0.maks: ww0.mak
	
dw1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack dw1 > $(OUT)/include/dtack/dw1.mak
dw1.maks: dw1.mak
	
dt0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack dt0 > $(OUT)/include/dtack/dt0.mak
dt0.maks: dt0.mak
	
db0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack db0 > $(OUT)/include/dtack/db0.mak
db0.maks: db0.mak
	
wb0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack wb0 > $(OUT)/include/dtack/wb0.mak
wb0.maks: wb0.mak
	
wb3.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack wb3 > $(OUT)/include/dtack/wb3.mak
wb3.maks: wb3.mak
	
wv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack wv0 > $(OUT)/include/dtack/wv0.mak
wv0.maks: wv0.mak
	
wv4.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack wv4 > $(OUT)/include/dtack/wv4.mak
wv4.maks: wv4.mak
	
wv5.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack wv5 > $(OUT)/include/dtack/wv5.mak
wv5.maks: wv5.mak
	
dv0.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack dv0 > $(OUT)/include/dtack/dv0.mak
dv0.maks: dv0.mak
	
bsd1.mak: dir.yam \
         $(DT_YAM)/config/def.yam \
         ../../config/prj.yam
	bash yam.sh $(SRC) include/dtack bsd1 > $(OUT)/include/dtack/bsd1.mak
bsd1.maks: bsd1.mak

#-------------------------------------------------------------------------

yam:
	bash yam.sh $(SRC) include/dtack default -DDTNODEPEND > $(OUT)/include/dtack/makefile
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
	  -f $(SRC)/include/dtack/qw0.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw0
qw1:
	$(MAKE) $(MAKEFLAGS) \
	  -f $(SRC)/include/dtack/qw1.mak  \
	  $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=qw1
qw6:
	$(MAKE) -f $(SRC)/include/dtack/qw6.mak SRC=$(SRC) OUT=$(OUT) 
dw0:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/dw0.mak 
ww0:
	wmake $(__MAKEOPTS__) -h -f $(SRC)/include/dtack ww0.mak
dw1:
	wmake $(__MAKEOPTS__) -h \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/dw1.mak 
dt0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/dt0.mak 
db0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/db0.mak 
wb0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/wb0.mak 
wb3:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/wb3.mak 
wv0:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/wv0.mak 
wv4:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/wv4.mak 
wv5:
	$(MAKE) -$(MAKEFLAGS) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  -f $(SRC)/include/dtack/wv5.mak 
dv0:
	$(MAKE) --$(MAKEFLAGS) --x- \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  --f $(SRC)/include/dtack/dv0.mak 
bsd1:
	$(MAKE) \
	  -f $(DT_SET)/bsd1.set \
	  -f $(DT_SET)/$(MK_TARGET).set \
	  -f $(SRC)/include/dtack/bsd1.mak $(ACT) \
	  SRC=$(SRC) \
	  OUT=$(OUT) \
	  MK_TARGET=$(MK_TARGET)

sg1:
	$(MAKE) -f $(SRC)/include/dtack/sg1.mak SRC=$(SRC) OUT=$(OUT) 
sg1x5:
	$(MAKE) -f $(SRC)/include/dtack/sg1x5.mak SRC=$(SRC) OUT=$(OUT) 
fg1:
	$(MAKE) -f $(SRC)/include/dtack/fg1.mak SRC=$(SRC) OUT=$(OUT) 
lg1:
	$(MAKE) -f $(SRC)/include/dtack/lg1.mak SRC=$(SRC) OUT=$(OUT) 

#-------------------------------------------------------------------------
# protos

t.p: \
	  $(SRC)/include/dtack/../../test/lib/check.c \
	  $(SRC)/include/dtack/../../test/lib/gen256.c \
	  $(SRC)/include/dtack/../../test/lib/util1.c \
	  $(SRC)/include/dtack/../../test/lib/time.c \
	  $(SRC)/include/dtack/../../test/lib/raw.c
	bash dtdxc.sh \
	  t.p -fproto \
	  $(SRC)/include/dtack/../../test/lib/check.c \
	  $(SRC)/include/dtack/../../test/lib/gen256.c \
	  $(SRC)/include/dtack/../../test/lib/util1.c \
	  $(SRC)/include/dtack/../../test/lib/time.c \
	  $(SRC)/include/dtack/../../test/lib/raw.c

base.p: \
	  $(SRC)/include/dtack/../../source/inter/interact.c \
	  $(SRC)/include/dtack/../../source/inter/interdia.c \
	  $(SRC)/include/dtack/../../source/inter/interhlp.c \
	  $(SRC)/include/dtack/../../source/low/assert.c \
	  $(SRC)/include/dtack/../../source/low/dbg.c \
	  $(SRC)/include/dtack/../../source/low/dbg2.c \
	  $(SRC)/include/dtack/../../source/low/err.c \
	  $(SRC)/include/dtack/../../source/low/err2.c \
	  $(SRC)/include/dtack/../../source/low/enter.c \
	  $(SRC)/include/dtack/../../source/low/leave.c \
	  $(SRC)/include/dtack/../../source/low/usage.c \
	  $(SRC)/include/dtack/../../source/low/heapmon.c \
	  $(SRC)/include/dtack/../../source/low/ntype.c \
	  $(SRC)/include/dtack/../../source/low/say.c \
	  $(SRC)/include/dtack/../../source/low/yield.c
	bash dtdxc.sh \
	  base.p -fproto \
	  $(SRC)/include/dtack/../../source/inter/interact.c \
	  $(SRC)/include/dtack/../../source/inter/interdia.c \
	  $(SRC)/include/dtack/../../source/inter/interhlp.c \
	  $(SRC)/include/dtack/../../source/low/assert.c \
	  $(SRC)/include/dtack/../../source/low/dbg.c \
	  $(SRC)/include/dtack/../../source/low/dbg2.c \
	  $(SRC)/include/dtack/../../source/low/err.c \
	  $(SRC)/include/dtack/../../source/low/err2.c \
	  $(SRC)/include/dtack/../../source/low/enter.c \
	  $(SRC)/include/dtack/../../source/low/leave.c \
	  $(SRC)/include/dtack/../../source/low/usage.c \
	  $(SRC)/include/dtack/../../source/low/heapmon.c \
	  $(SRC)/include/dtack/../../source/low/ntype.c \
	  $(SRC)/include/dtack/../../source/low/say.c \
	  $(SRC)/include/dtack/../../source/low/yield.c

# ----------------------------------------------

db.p: \
	  $(SRC)/include/dtack/../../source/low/dbtable.c
	bash dtdxc.sh \
	  db.p -fproto \
	  $(SRC)/include/dtack/../../source/low/dbtable.c

threads.p: \
	  $(SRC)/include/dtack/../../source/low/threads.c
	bash dtdxc.sh \
	  threads.p -fproto \
	  $(SRC)/include/dtack/../../source/low/threads.c
thread.p: \
	  $(SRC)/include/dtack/../../source/low/thread.c \
	  $(SRC)/include/dtack/../../source/low/threadu.c
	bash dtdxc.sh \
	  thread.p -fproto \
	  $(SRC)/include/dtack/../../source/low/thread.c \
	  $(SRC)/include/dtack/../../source/low/threadu.c

serial.p: \
	  $(SRC)/include/dtack/../../source/low/serialw.c \
	  $(SRC)/include/dtack/../../source/low/serial.c
	bash dtdxc.sh \
	  serial.p -fproto \
	  $(SRC)/include/dtack/../../source/low/serialw.c \
	  $(SRC)/include/dtack/../../source/low/serial.c

points.p: \
	  $(SRC)/include/dtack/../../source/low/points.c
	bash dtdxc.sh \
	  points.p -fproto \
	  $(SRC)/include/dtack/../../source/low/points.c

poly.p: \
	  $(SRC)/include/dtack/../../source/low/poly.c
	bash dtdxc.sh \
	  poly.p -fproto \
	  $(SRC)/include/dtack/../../source/low/poly.c

matrix.p: \
	  $(SRC)/include/dtack/../../source/low/matrix3d.c \
	  $(SRC)/include/dtack/../../source/low/matrix2d.c \
	  $(SRC)/include/dtack/../../source/low/matrix.c
	bash dtdxc.sh \
	  matrix.p -fproto \
	  $(SRC)/include/dtack/../../source/low/matrix3d.c \
	  $(SRC)/include/dtack/../../source/low/matrix2d.c \
	  $(SRC)/include/dtack/../../source/low/matrix.c

sem.p: \
	  $(SRC)/include/dtack/../../source/low/semu.c
	bash dtdxc.sh \
	  sem.p -fproto \
	  $(SRC)/include/dtack/../../source/low/semu.c

event.p: \
	  $(SRC)/include/dtack/../../source/low/eventw.c
	bash dtdxc.sh \
	  event.p -fproto \
	  $(SRC)/include/dtack/../../source/low/eventw.c

shm.p: \
	  $(SRC)/include/dtack/../../source/low/shmu.c
	bash dtdxc.sh \
	  shm.p -fproto \
	  $(SRC)/include/dtack/../../source/low/shmu.c

msg.p: \
	  $(SRC)/include/dtack/../../source/low/msgu.c
	bash dtdxc.sh \
	  msg.p -fproto \
	  $(SRC)/include/dtack/../../source/low/msgu.c

cstack.p: \
	  $(SRC)/include/dtack/../../source/low/cstack.c
	bash dtdxc.sh \
	  cstack.p -fproto \
	  $(SRC)/include/dtack/../../source/low/cstack.c

heap.p: \
	  $(SRC)/include/dtack/../../source/low/heap2.c
	bash dtdxc.sh \
	  heap.p -fproto \
	  $(SRC)/include/dtack/../../source/low/heap2.c

liner.p: \
	  $(SRC)/include/dtack/../../source/low/liner.c
	bash dtdxc.sh \
	  liner.p -fproto \
	  $(SRC)/include/dtack/../../source/low/liner.c

prj.p: \
	  $(SRC)/include/dtack/../../source/low/prj.c
	bash dtdxc.sh \
	  prj.p -fproto \
	  $(SRC)/include/dtack/../../source/low/prj.c

phys.p: \
	  $(SRC)/include/dtack/../../source/low/phys.c
	bash dtdxc.sh \
	  phys.p -fproto \
	  $(SRC)/include/dtack/../../source/low/phys.c

portio.p: \
	  $(SRC)/include/dtack/../../source/low/portio.c
	bash dtdxc.sh \
	  portio.p -fproto \
	  $(SRC)/include/dtack/../../source/low/portio.c

stack.p: \
	  $(SRC)/include/dtack/../../source/bigmem/stack.c \
	  $(SRC)/include/dtack/../../source/bigmem/stack_io.c
	bash dtdxc.sh \
	  stack.p -fproto \
	  $(SRC)/include/dtack/../../source/bigmem/stack.c \
	  $(SRC)/include/dtack/../../source/bigmem/stack_io.c

param.p: \
	  $(SRC)/include/dtack/../../source/low/param.c
	bash dtdxc.sh \
	  param.p -fproto \
	  $(SRC)/include/dtack/../../source/low/param.c

hist.p: \
	  $(SRC)/include/dtack/../../source/low/hist.c \
	  $(SRC)/include/dtack/../../source/low/hist2.c
	bash dtdxc.sh \
	  hist.p -fproto \
	  $(SRC)/include/dtack/../../source/low/hist.c \
	  $(SRC)/include/dtack/../../source/low/hist2.c

rev.p: \
	  $(SRC)/include/dtack/../../source/low/rev.c
	bash dtdxc.sh \
	  rev.p -fproto \
	  $(SRC)/include/dtack/../../source/low/rev.c

font.p: \
	  $(SRC)/include/dtack/../../source/bigmem/font.c
	bash dtdxc.sh \
	  font.p -fproto \
	  $(SRC)/include/dtack/../../source/bigmem/font.c

umdec.p: \
	  $(SRC)/include/dtack/../../source/mpeg/umdeclib.c \
	  $(SRC)/include/dtack/../../source/mpeg/umdecssg.c \
	  $(SRC)/include/dtack/../../source/mpeg/umdecvp.c
	bash dtdxc.sh \
	  umdec.p -fproto \
	  $(SRC)/include/dtack/../../source/mpeg/umdeclib.c \
	  $(SRC)/include/dtack/../../source/mpeg/umdecssg.c \
	  $(SRC)/include/dtack/../../source/mpeg/umdecvp.c

ustate.p: \
	  $(SRC)/include/dtack/../../app/ustate/ustatelib.c \
	  $(SRC)/include/dtack/../../app/ustate/rev.c
	bash dtdxc.sh \
	  ustate.p -fproto \
	  $(SRC)/include/dtack/../../app/ustate/ustatelib.c \
	  $(SRC)/include/dtack/../../app/ustate/rev.c

reverb.p: \
	  $(SRC)/include/dtack/../../app/reverb/client.c \
	  $(SRC)/include/dtack/../../app/reverb/rev.c
	bash dtdxc.sh \
	  reverb.p -fproto \
	  $(SRC)/include/dtack/../../app/reverb/client.c \
	  $(SRC)/include/dtack/../../app/reverb/rev.c

vidmux.p: \
	  $(SRC)/include/dtack/../../source/fb/fbvidmux.c
	bash dtdxc.sh \
	  vidmux.p -fproto \
	  $(SRC)/include/dtack/../../source/fb/fbvidmux.c

fb.p: \
	  $(SRC)/include/dtack/../../source/fb/fb.c \
	  $(SRC)/include/dtack/../../source/fb/fbdir.c \
	  $(SRC)/include/dtack/../../source/fb/fbgen.c \
	  $(SRC)/include/dtack/../../source/fb/fbvbmci.c \
	  $(SRC)/include/dtack/../../source/fb/fbvmc.c \
	  $(SRC)/include/dtack/../../source/fb/fbvfw.c \
	  $(SRC)/include/dtack/../../source/fb/fbsnap.c \
	  $(SRC)/include/dtack/../../source/fb/fbpsi.c \
	  $(SRC)/include/dtack/../../source/fb/fbcortex.c \
	  $(SRC)/include/dtack/../../source/fb/fbmeteor.c \
	  $(SRC)/include/dtack/../../source/fb/fbbt.c \
	  $(SRC)/include/dtack/../../source/fb/fbutil.c
	bash dtdxc.sh \
	  fb.p -fproto \
	  $(SRC)/include/dtack/../../source/fb/fb.c \
	  $(SRC)/include/dtack/../../source/fb/fbdir.c \
	  $(SRC)/include/dtack/../../source/fb/fbgen.c \
	  $(SRC)/include/dtack/../../source/fb/fbvbmci.c \
	  $(SRC)/include/dtack/../../source/fb/fbvmc.c \
	  $(SRC)/include/dtack/../../source/fb/fbvfw.c \
	  $(SRC)/include/dtack/../../source/fb/fbsnap.c \
	  $(SRC)/include/dtack/../../source/fb/fbpsi.c \
	  $(SRC)/include/dtack/../../source/fb/fbcortex.c \
	  $(SRC)/include/dtack/../../source/fb/fbmeteor.c \
	  $(SRC)/include/dtack/../../source/fb/fbbt.c \
	  $(SRC)/include/dtack/../../source/fb/fbutil.c

color.p: \
	  $(SRC)/include/dtack/../../source/color/convert.c \
	  $(SRC)/include/dtack/../../source/color/org.c \
	  $(SRC)/include/dtack/../../source/color/rgb.c \
	  $(SRC)/include/dtack/../../source/color/rgb888.c \
	  $(SRC)/include/dtack/../../source/color/r_g_b.c \
	  $(SRC)/include/dtack/../../source/color/ycbycr.c \
	  $(SRC)/include/dtack/../../source/color/y_cb_cr.c \
	  $(SRC)/include/dtack/../../source/color/yuyv.c
	bash dtdxc.sh \
	  color.p -fproto \
	  $(SRC)/include/dtack/../../source/color/convert.c \
	  $(SRC)/include/dtack/../../source/color/org.c \
	  $(SRC)/include/dtack/../../source/color/rgb.c \
	  $(SRC)/include/dtack/../../source/color/rgb888.c \
	  $(SRC)/include/dtack/../../source/color/r_g_b.c \
	  $(SRC)/include/dtack/../../source/color/ycbycr.c \
	  $(SRC)/include/dtack/../../source/color/y_cb_cr.c \
	  $(SRC)/include/dtack/../../source/color/yuyv.c

mem.p: \
	  $(SRC)/include/dtack/../../source/bigmem/memsourc.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem.c \
	  $(SRC)/include/dtack/../../source/bigmem/memclip.c \
	  $(SRC)/include/dtack/../../source/bigmem/memputp.c \
	  $(SRC)/include/dtack/../../source/bigmem/memgetp.c \
	  $(SRC)/include/dtack/../../source/bigmem/memconf.c \
	  $(SRC)/include/dtack/../../source/bigmem/memparse.c \
	  $(SRC)/include/dtack/../../source/bigmem/memcopy.c \
	  $(SRC)/include/dtack/../../source/bigmem/memio.c \
	  $(SRC)/include/dtack/../../source/bigmem/memcmp.c \
	  $(SRC)/include/dtack/../../source/bigmem/memset.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_std.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_ram.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_disk.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_ems.c \
	  $(SRC)/include/dtack/../../source/bigmem/memxmsif.c \
	  $(SRC)/include/dtack/../../source/bigmem/memxmslb.c \
	  $(SRC)/include/dtack/../../source/bigmem/memtxms.c
	bash dtdxc.sh \
	  mem.p -fproto \
	  $(SRC)/include/dtack/../../source/bigmem/memsourc.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem.c \
	  $(SRC)/include/dtack/../../source/bigmem/memclip.c \
	  $(SRC)/include/dtack/../../source/bigmem/memputp.c \
	  $(SRC)/include/dtack/../../source/bigmem/memgetp.c \
	  $(SRC)/include/dtack/../../source/bigmem/memconf.c \
	  $(SRC)/include/dtack/../../source/bigmem/memparse.c \
	  $(SRC)/include/dtack/../../source/bigmem/memcopy.c \
	  $(SRC)/include/dtack/../../source/bigmem/memio.c \
	  $(SRC)/include/dtack/../../source/bigmem/memcmp.c \
	  $(SRC)/include/dtack/../../source/bigmem/memset.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_std.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_ram.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_disk.c \
	  $(SRC)/include/dtack/../../source/bigmem/mem_ems.c \
	  $(SRC)/include/dtack/../../source/bigmem/memxmsif.c \
	  $(SRC)/include/dtack/../../source/bigmem/memxmslb.c \
	  $(SRC)/include/dtack/../../source/bigmem/memtxms.c

1d.p: \
	  $(SRC)/include/dtack/../../source/1d/1d.c \
	  $(SRC)/include/dtack/../../source/1d/1d_abs.c \
	  $(SRC)/include/dtack/../../source/1d/1d_arith.c \
	  $(SRC)/include/dtack/../../source/1d/1d_ran.c \
	  $(SRC)/include/dtack/../../source/1d/1d_range.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cast.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cmp.c \
	  $(SRC)/include/dtack/../../source/1d/1d_diff.c \
	  $(SRC)/include/dtack/../../source/1d/1d_fft.c \
	  $(SRC)/include/dtack/../../source/1d/1d_fftw.c \
	  $(SRC)/include/dtack/../../source/1d/1d_set.c \
	  $(SRC)/include/dtack/../../source/1d/1d_stats.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sum.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sumsq.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sumsd.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sumss.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cumf.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cumr.c \
	  $(SRC)/include/dtack/../../source/1d/1d_look.c \
	  $(SRC)/include/dtack/../../source/1d/1d_logic.c \
	  $(SRC)/include/dtack/../../source/1d/1d_noise.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sort1.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sort5.c \
	  $(SRC)/include/dtack/../../source/1d/1d_conv.c \
	  $(SRC)/include/dtack/../../source/1d/1d_clip.c \
	  $(SRC)/include/dtack/../../source/1d/1d_thres.c \
	  $(SRC)/include/dtack/../../source/1d/1d_vari.c
	bash dtdxc.sh \
	  1d.p -fproto \
	  $(SRC)/include/dtack/../../source/1d/1d.c \
	  $(SRC)/include/dtack/../../source/1d/1d_abs.c \
	  $(SRC)/include/dtack/../../source/1d/1d_arith.c \
	  $(SRC)/include/dtack/../../source/1d/1d_ran.c \
	  $(SRC)/include/dtack/../../source/1d/1d_range.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cast.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cmp.c \
	  $(SRC)/include/dtack/../../source/1d/1d_diff.c \
	  $(SRC)/include/dtack/../../source/1d/1d_fft.c \
	  $(SRC)/include/dtack/../../source/1d/1d_fftw.c \
	  $(SRC)/include/dtack/../../source/1d/1d_set.c \
	  $(SRC)/include/dtack/../../source/1d/1d_stats.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sum.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sumsq.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sumsd.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sumss.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cumf.c \
	  $(SRC)/include/dtack/../../source/1d/1d_cumr.c \
	  $(SRC)/include/dtack/../../source/1d/1d_look.c \
	  $(SRC)/include/dtack/../../source/1d/1d_logic.c \
	  $(SRC)/include/dtack/../../source/1d/1d_noise.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sort1.c \
	  $(SRC)/include/dtack/../../source/1d/1d_sort5.c \
	  $(SRC)/include/dtack/../../source/1d/1d_conv.c \
	  $(SRC)/include/dtack/../../source/1d/1d_clip.c \
	  $(SRC)/include/dtack/../../source/1d/1d_thres.c \
	  $(SRC)/include/dtack/../../source/1d/1d_vari.c

ftp.p: \
	  $(SRC)/include/dtack/../../source/low/ftp.c
	bash dtdxc.sh \
	  ftp.p -fproto \
	  $(SRC)/include/dtack/../../source/low/ftp.c

ipc.p: \
	  $(SRC)/include/dtack/../../source/low/ipc.c \
	  $(SRC)/include/dtack/../../source/low/ipcqnx4.c \
	  $(SRC)/include/dtack/../../source/low/ipcqbaby.c \
	  $(SRC)/include/dtack/../../source/low/ipcrock.c \
	  $(SRC)/include/dtack/../../source/low/ipcdde.c
	bash dtdxc.sh \
	  ipc.p -fproto \
	  $(SRC)/include/dtack/../../source/low/ipc.c \
	  $(SRC)/include/dtack/../../source/low/ipcqnx4.c \
	  $(SRC)/include/dtack/../../source/low/ipcqbaby.c \
	  $(SRC)/include/dtack/../../source/low/ipcrock.c \
	  $(SRC)/include/dtack/../../source/low/ipcdde.c

dg.p: \
	  $(SRC)/include/dtack/../../source/dg/line.c \
	  $(SRC)/include/dtack/../../source/dg/header.c \
	  $(SRC)/include/dtack/../../source/dg/dgserver.c \
	  $(SRC)/include/dtack/../../source/dg/request.c \
	  $(SRC)/include/dtack/../../source/dg/response.c \
	  $(SRC)/include/dtack/../../source/dg/rev.c \
	  $(SRC)/include/dtack/../../source/dg/exit.c \
	  $(SRC)/include/dtack/../../source/dg/delay.c \
	  $(SRC)/include/dtack/../../source/dg/dgclient.c
	bash dtdxc.sh \
	  dg.p -fproto \
	  $(SRC)/include/dtack/../../source/dg/line.c \
	  $(SRC)/include/dtack/../../source/dg/header.c \
	  $(SRC)/include/dtack/../../source/dg/dgserver.c \
	  $(SRC)/include/dtack/../../source/dg/request.c \
	  $(SRC)/include/dtack/../../source/dg/response.c \
	  $(SRC)/include/dtack/../../source/dg/rev.c \
	  $(SRC)/include/dtack/../../source/dg/exit.c \
	  $(SRC)/include/dtack/../../source/dg/delay.c \
	  $(SRC)/include/dtack/../../source/dg/dgclient.c

cons.p: \
	  $(SRC)/include/dtack/../../source/low/cons.c
	bash dtdxc.sh \
	  cons.p -fproto \
	  $(SRC)/include/dtack/../../source/low/cons.c

os.p: \
	  $(SRC)/include/dtack/../../source/low/os_delay.c \
	  $(SRC)/include/dtack/../../source/low/os_futil.c \
	  $(SRC)/include/dtack/../../source/low/os_heap.c \
	  $(SRC)/include/dtack/../../source/low/os_heap2.c \
	  $(SRC)/include/dtack/../../source/low/os_mem.c \
	  $(SRC)/include/dtack/../../source/low/os_time.c \
	  $(SRC)/include/dtack/../../source/low/os_env.c
	bash dtdxc.sh \
	  os.p -fproto \
	  $(SRC)/include/dtack/../../source/low/os_delay.c \
	  $(SRC)/include/dtack/../../source/low/os_futil.c \
	  $(SRC)/include/dtack/../../source/low/os_heap.c \
	  $(SRC)/include/dtack/../../source/low/os_heap2.c \
	  $(SRC)/include/dtack/../../source/low/os_mem.c \
	  $(SRC)/include/dtack/../../source/low/os_time.c \
	  $(SRC)/include/dtack/../../source/low/os_env.c
str.p: \
	  $(SRC)/include/dtack/../../source/low/str.c
	bash dtdxc.sh \
	  str.p -fproto \
	  $(SRC)/include/dtack/../../source/low/str.c
fd.p: \
	  $(SRC)/include/dtack/../../source/low/fd.c
	bash dtdxc.sh \
	  fd.p -fproto \
	  $(SRC)/include/dtack/../../source/low/fd.c
f.p: \
	  $(SRC)/include/dtack/../../source/low/f.c
	bash dtdxc.sh \
	  f.p -fproto \
	  $(SRC)/include/dtack/../../source/low/f.c
pnm.p: \
	  $(SRC)/include/dtack/../../source/low/pnm.c
	bash dtdxc.sh \
	  pnm.p -fproto \
	  $(SRC)/include/dtack/../../source/low/pnm.c
lut8.p: \
	  $(SRC)/include/dtack/../../source/xchg/lut8xchg.c \
	  $(SRC)/include/dtack/../../source/low/lut8.c \
	  $(SRC)/include/dtack/../../source/low/lut8_mat.c
	bash dtdxc.sh \
	  lut8.p -fproto \
	  $(SRC)/include/dtack/../../source/xchg/lut8xchg.c \
	  $(SRC)/include/dtack/../../source/low/lut8.c \
	  $(SRC)/include/dtack/../../source/low/lut8_mat.c

qnx4.p: \
	  $(SRC)/include/dtack/../../source/low/qnx4.c
	bash dtdxc.sh \
	  qnx4.p -fproto \
	  $(SRC)/include/dtack/../../source/low/qnx4.c

cipher.p: \
	  $(SRC)/include/dtack/../../source/low/cipher.c
	bash dtdxc.sh \
	  cipher.p -fproto \
	  $(SRC)/include/dtack/../../source/low/cipher.c

list.p: \
	  $(SRC)/include/dtack/../../source/low/list.c \
	  $(SRC)/include/dtack/../../source/low/list_io.c
	bash dtdxc.sh \
	  list.p -fproto \
	  $(SRC)/include/dtack/../../source/low/list.c \
	  $(SRC)/include/dtack/../../source/low/list_io.c

dsk.p: \
	  $(SRC)/include/dtack/../../source/win/dsk.c
	bash dtdxc.sh \
	  dsk.p -fproto \
	  $(SRC)/include/dtack/../../source/win/dsk.c
dsk_c.p: \
	  $(SRC)/include/dtack/../../source/win/dsk_c.c
	bash dtdxc.sh \
	  dsk_c.p -fproto \
	  $(SRC)/include/dtack/../../source/win/dsk_c.c

# ----------------------------------------------

xchg.p: \
	  $(SRC)/include/dtack/../../source/xchg/parse.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_dtm.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_win.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_wxx.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_jpg.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_scr.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_can.c
	bash dtdxc.sh \
	  xchg.p -fproto \
	  $(SRC)/include/dtack/../../source/xchg/parse.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_dtm.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_win.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_wxx.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_jpg.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_scr.c \
	  $(SRC)/include/dtack/../../source/xchg/xchg_can.c

wmf.p: \
	  $(SRC)/include/dtack/../../source/xchg/wmf.c
	bash dtdxc.sh \
	  wmf.p -fproto \
	  $(SRC)/include/dtack/../../source/xchg/wmf.c

bmp.p: \
	  $(SRC)/include/dtack/../../source/xchg/bmp.c
	bash dtdxc.sh \
	  bmp.p -fproto \
	  $(SRC)/include/dtack/../../source/xchg/bmp.c

dib.p: \
	  $(SRC)/include/dtack/../../source/xchg/dib.c
	bash dtdxc.sh \
	  dib.p -fproto \
	  $(SRC)/include/dtack/../../source/xchg/dib.c

run.p: \
	  $(SRC)/include/dtack/../../source/high/run.c
	bash dtdxc.sh \
	  run.p -fproto \
	  $(SRC)/include/dtack/../../source/high/run.c

runf.p: \
	  $(SRC)/include/dtack/../../source/high/runf.c
	bash dtdxc.sh \
	  runf.p -fproto \
	  $(SRC)/include/dtack/../../source/high/runf.c

image.p: \
	  $(SRC)/include/dtack/../../source/high/assign.c \
	  $(SRC)/include/dtack/../../source/high/image.c \
	  $(SRC)/include/dtack/../../source/xchg/import.c \
	  $(SRC)/include/dtack/../../source/xchg/export.c \
	  $(SRC)/include/dtack/../../source/xchg/read.c \
	  $(SRC)/include/dtack/../../source/xchg/readtif.c \
	  $(SRC)/include/dtack/../../source/xchg/write.c \
	  $(SRC)/include/dtack/../../source/xchg/writebmp.c \
	  $(SRC)/include/dtack/../../source/xchg/writewmf.c \
	  $(SRC)/include/dtack/../../source/high/thresh.c \
	  $(SRC)/include/dtack/../../source/high/logical.c \
	  $(SRC)/include/dtack/../../source/high/arith.c \
	  $(SRC)/include/dtack/../../source/high/check.c \
	  $(SRC)/include/dtack/../../source/high/dbg.c \
	  $(SRC)/include/dtack/../../source/high/err.c \
	  $(SRC)/include/dtack/../../source/high/fft.c \
	  $(SRC)/include/dtack/../../source/high/fftw.c \
	  $(SRC)/include/dtack/../../source/high/log.c \
	  $(SRC)/include/dtack/../../source/high/noise.c \
	  $(SRC)/include/dtack/../../source/high/cast.c \
	  $(SRC)/include/dtack/../../source/high/col.c \
	  $(SRC)/include/dtack/../../source/high/convolve.c \
	  $(SRC)/include/dtack/../../source/high/cooc.c \
	  $(SRC)/include/dtack/../../source/high/cooc2.c \
	  $(SRC)/include/dtack/../../source/high/constant.c \
	  $(SRC)/include/dtack/../../source/high/conv_l.c \
	  $(SRC)/include/dtack/../../source/high/conv_d.c \
	  $(SRC)/include/dtack/../../source/high/conv_s.c \
	  $(SRC)/include/dtack/../../source/high/matrix.c \
	  $(SRC)/include/dtack/../../source/high/joint.c \
	  $(SRC)/include/dtack/../../source/high/joint2.c \
	  $(SRC)/include/dtack/../../source/high/label1.c \
	  $(SRC)/include/dtack/../../source/high/lookup.c \
	  $(SRC)/include/dtack/../../source/high/sum.c \
	  $(SRC)/include/dtack/../../source/high/sum_rows.c \
	  $(SRC)/include/dtack/../../source/high/sum_cols.c \
	  $(SRC)/include/dtack/../../source/high/cmp.c \
	  $(SRC)/include/dtack/../../source/high/gen1.c \
	  $(SRC)/include/dtack/../../source/high/gen2.c \
	  $(SRC)/include/dtack/../../source/high/gen3.c \
	  $(SRC)/include/dtack/../../source/high/gen_kern.c \
	  $(SRC)/include/dtack/../../source/high/median.c \
	  $(SRC)/include/dtack/../../source/high/morph.c \
	  $(SRC)/include/dtack/../../source/high/stats.c \
	  $(SRC)/include/dtack/../../source/high/range.c \
	  $(SRC)/include/dtack/../../source/high/scan.c \
	  $(SRC)/include/dtack/../../source/high/blob.c \
	  $(SRC)/include/dtack/../../source/high/run_len.c \
	  $(SRC)/include/dtack/../../source/high/mean.c \
	  $(SRC)/include/dtack/../../source/high/cumul.c \
	  $(SRC)/include/dtack/../../source/high/sumsq.c \
	  $(SRC)/include/dtack/../../source/high/sumsqd.c \
	  $(SRC)/include/dtack/../../source/high/sumsqs.c \
	  $(SRC)/include/dtack/../../source/high/resample.c \
	  $(SRC)/include/dtack/../../source/high/rotate.c \
	  $(SRC)/include/dtack/../../source/high/transpos.c \
	  $(SRC)/include/dtack/../../source/high/pixel.c \
	  $(SRC)/include/dtack/../../source/high/hist1.c \
	  $(SRC)/include/dtack/../../source/high/hist2.c \
	  $(SRC)/include/dtack/../../source/high/plot1.c \
	  $(SRC)/include/dtack/../../source/high/plot2.c \
	  $(SRC)/include/dtack/../../source/high/plot_txt.c \
	  $(SRC)/include/dtack/../../source/high/adj.c \
	  $(SRC)/include/dtack/../../source/high/adj_run.c
	bash dtdxc.sh \
	  image.p -fproto \
	  $(SRC)/include/dtack/../../source/high/assign.c \
	  $(SRC)/include/dtack/../../source/high/image.c \
	  $(SRC)/include/dtack/../../source/xchg/import.c \
	  $(SRC)/include/dtack/../../source/xchg/export.c \
	  $(SRC)/include/dtack/../../source/xchg/read.c \
	  $(SRC)/include/dtack/../../source/xchg/readtif.c \
	  $(SRC)/include/dtack/../../source/xchg/write.c \
	  $(SRC)/include/dtack/../../source/xchg/writebmp.c \
	  $(SRC)/include/dtack/../../source/xchg/writewmf.c \
	  $(SRC)/include/dtack/../../source/high/thresh.c \
	  $(SRC)/include/dtack/../../source/high/logical.c \
	  $(SRC)/include/dtack/../../source/high/arith.c \
	  $(SRC)/include/dtack/../../source/high/check.c \
	  $(SRC)/include/dtack/../../source/high/dbg.c \
	  $(SRC)/include/dtack/../../source/high/err.c \
	  $(SRC)/include/dtack/../../source/high/fft.c \
	  $(SRC)/include/dtack/../../source/high/fftw.c \
	  $(SRC)/include/dtack/../../source/high/log.c \
	  $(SRC)/include/dtack/../../source/high/noise.c \
	  $(SRC)/include/dtack/../../source/high/cast.c \
	  $(SRC)/include/dtack/../../source/high/col.c \
	  $(SRC)/include/dtack/../../source/high/convolve.c \
	  $(SRC)/include/dtack/../../source/high/cooc.c \
	  $(SRC)/include/dtack/../../source/high/cooc2.c \
	  $(SRC)/include/dtack/../../source/high/constant.c \
	  $(SRC)/include/dtack/../../source/high/conv_l.c \
	  $(SRC)/include/dtack/../../source/high/conv_d.c \
	  $(SRC)/include/dtack/../../source/high/conv_s.c \
	  $(SRC)/include/dtack/../../source/high/matrix.c \
	  $(SRC)/include/dtack/../../source/high/joint.c \
	  $(SRC)/include/dtack/../../source/high/joint2.c \
	  $(SRC)/include/dtack/../../source/high/label1.c \
	  $(SRC)/include/dtack/../../source/high/lookup.c \
	  $(SRC)/include/dtack/../../source/high/sum.c \
	  $(SRC)/include/dtack/../../source/high/sum_rows.c \
	  $(SRC)/include/dtack/../../source/high/sum_cols.c \
	  $(SRC)/include/dtack/../../source/high/cmp.c \
	  $(SRC)/include/dtack/../../source/high/gen1.c \
	  $(SRC)/include/dtack/../../source/high/gen2.c \
	  $(SRC)/include/dtack/../../source/high/gen3.c \
	  $(SRC)/include/dtack/../../source/high/gen_kern.c \
	  $(SRC)/include/dtack/../../source/high/median.c \
	  $(SRC)/include/dtack/../../source/high/morph.c \
	  $(SRC)/include/dtack/../../source/high/stats.c \
	  $(SRC)/include/dtack/../../source/high/range.c \
	  $(SRC)/include/dtack/../../source/high/scan.c \
	  $(SRC)/include/dtack/../../source/high/blob.c \
	  $(SRC)/include/dtack/../../source/high/run_len.c \
	  $(SRC)/include/dtack/../../source/high/mean.c \
	  $(SRC)/include/dtack/../../source/high/cumul.c \
	  $(SRC)/include/dtack/../../source/high/sumsq.c \
	  $(SRC)/include/dtack/../../source/high/sumsqd.c \
	  $(SRC)/include/dtack/../../source/high/sumsqs.c \
	  $(SRC)/include/dtack/../../source/high/resample.c \
	  $(SRC)/include/dtack/../../source/high/rotate.c \
	  $(SRC)/include/dtack/../../source/high/transpos.c \
	  $(SRC)/include/dtack/../../source/high/pixel.c \
	  $(SRC)/include/dtack/../../source/high/hist1.c \
	  $(SRC)/include/dtack/../../source/high/hist2.c \
	  $(SRC)/include/dtack/../../source/high/plot1.c \
	  $(SRC)/include/dtack/../../source/high/plot2.c \
	  $(SRC)/include/dtack/../../source/high/plot_txt.c \
	  $(SRC)/include/dtack/../../source/high/adj.c \
	  $(SRC)/include/dtack/../../source/high/adj_run.c

# ----------------------------------------------

canvas.p: \
	  $(SRC)/include/dtack/../../source/screen/canvas.c \
	  $(SRC)/include/dtack/../../source/screen/canpoll.c \
	  $(SRC)/include/dtack/../../source/screen/candum.c \
	  $(SRC)/include/dtack/../../source/screen/canfastg.c \
	  $(SRC)/include/dtack/../../source/screen/cansvgac.c \
	  $(SRC)/include/dtack/../../source/screen/canhws.c
	bash dtdxc.sh \
	  canvas.p -fproto \
	  $(SRC)/include/dtack/../../source/screen/canvas.c \
	  $(SRC)/include/dtack/../../source/screen/canpoll.c \
	  $(SRC)/include/dtack/../../source/screen/candum.c \
	  $(SRC)/include/dtack/../../source/screen/canfastg.c \
	  $(SRC)/include/dtack/../../source/screen/cansvgac.c \
	  $(SRC)/include/dtack/../../source/screen/canhws.c

vga.p: \
	  $(SRC)/include/dtack/../../source/screen/vga.c
	bash dtdxc.sh \
	  vga.p -fproto \
	  $(SRC)/include/dtack/../../source/screen/vga.c

hws.p: \
	  $(SRC)/include/dtack/../../source/screen/hws.c \
	  $(SRC)/include/dtack/../../source/screen/hwsx.c \
	  $(SRC)/include/dtack/../../source/screen/hwsqw4.c \
	  $(SRC)/include/dtack/../../source/screen/hwsfgw.c \
	  $(SRC)/include/dtack/../../source/screen/hwsmsw.c
	bash dtdxc.sh \
	  hws.p -fproto \
	  $(SRC)/include/dtack/../../source/screen/hws.c \
	  $(SRC)/include/dtack/../../source/screen/hwsx.c \
	  $(SRC)/include/dtack/../../source/screen/hwsqw4.c \
	  $(SRC)/include/dtack/../../source/screen/hwsfgw.c \
	  $(SRC)/include/dtack/../../source/screen/hwsmsw.c

scr.p: \
	  $(SRC)/include/dtack/../../source/screen/scr_main.c \
	  $(SRC)/include/dtack/../../source/screen/scr_cur.c \
	  $(SRC)/include/dtack/../../source/screen/scr_act.c \
	  $(SRC)/include/dtack/../../source/screen/scr_dbg.c \
	  $(SRC)/include/dtack/../../source/screen/scrdum.c \
	  $(SRC)/include/dtack/../../source/screen/scrcan.c \
	  $(SRC)/include/dtack/../../source/screen/scrhws.c
	bash dtdxc.sh \
	  scr.p -fproto \
	  $(SRC)/include/dtack/../../source/screen/scr_main.c \
	  $(SRC)/include/dtack/../../source/screen/scr_cur.c \
	  $(SRC)/include/dtack/../../source/screen/scr_act.c \
	  $(SRC)/include/dtack/../../source/screen/scr_dbg.c \
	  $(SRC)/include/dtack/../../source/screen/scrdum.c \
	  $(SRC)/include/dtack/../../source/screen/scrcan.c \
	  $(SRC)/include/dtack/../../source/screen/scrhws.c

# ----------------------------------------------

trmios.p: \
	  $(SRC)/include/dtack/../../source/keybd/trmios.c
	bash dtdxc.sh \
	  trmios.p -fproto \
	  $(SRC)/include/dtack/../../source/keybd/trmios.c

keybd.p: \
	  $(SRC)/include/dtack/../../source/keybd/keybd.c \
	  $(SRC)/include/dtack/../../source/keybd/keyqnx.c \
	  $(SRC)/include/dtack/../../source/keybd/keyunix.c \
	  $(SRC)/include/dtack/../../source/keybd/keydos.c
	bash dtdxc.sh \
	  keybd.p -fproto \
	  $(SRC)/include/dtack/../../source/keybd/keybd.c \
	  $(SRC)/include/dtack/../../source/keybd/keyqnx.c \
	  $(SRC)/include/dtack/../../source/keybd/keyunix.c \
	  $(SRC)/include/dtack/../../source/keybd/keydos.c
keymap.p: \
	  $(SRC)/include/dtack/../../source/keybd/keymap.c \
	  $(SRC)/include/dtack/../../source/keybd/keymapt.c \
	  $(SRC)/include/dtack/../../source/keybd/keymapq.c \
	  $(SRC)/include/dtack/../../source/keybd/keymapc.c
	bash dtdxc.sh \
	  keymap.p -fproto \
	  $(SRC)/include/dtack/../../source/keybd/keymap.c \
	  $(SRC)/include/dtack/../../source/keybd/keymapt.c \
	  $(SRC)/include/dtack/../../source/keybd/keymapq.c \
	  $(SRC)/include/dtack/../../source/keybd/keymapc.c

iact.p: \
	  $(SRC)/include/dtack/../../source/inter/iact.c
	bash dtdxc.sh \
	  iact.p -fproto \
	  $(SRC)/include/dtack/../../source/inter/iact.c

note.p: \
	  $(SRC)/include/dtack/../../source/inter/note.c
	bash dtdxc.sh \
	  note.p -fproto \
	  $(SRC)/include/dtack/../../source/inter/note.c

alpha.p: \
	  $(SRC)/include/dtack/../../source/inter/alpha.c
	bash dtdxc.sh \
	  alpha.p -fproto \
	  $(SRC)/include/dtack/../../source/inter/alpha.c

dialog.p: \
	  $(SRC)/include/dtack/../../source/inter/dialog.c \
	  $(SRC)/include/dtack/../../source/inter/diatext.c \
	  $(SRC)/include/dtack/../../source/inter/diavba.c
	bash dtdxc.sh \
	  dialog.p -fproto \
	  $(SRC)/include/dtack/../../source/inter/dialog.c \
	  $(SRC)/include/dtack/../../source/inter/diatext.c \
	  $(SRC)/include/dtack/../../source/inter/diavba.c

msw.p: \
	  $(SRC)/include/dtack/../../source/low/msw.c
	bash dtdxc.sh \
	  msw.p -fproto \
	  $(SRC)/include/dtack/../../source/low/msw.c

text.p: \
	  $(SRC)/include/dtack/../../source/inter/text.c \
	  $(SRC)/include/dtack/../../source/inter/textcur1.c \
	  $(SRC)/include/dtack/../../source/inter/texttcap.c \
	  $(SRC)/include/dtack/../../source/inter/textdos1.c \
	  $(SRC)/include/dtack/../../source/inter/textqnx1.c
	bash dtdxc.sh \
	  text.p -fproto \
	  $(SRC)/include/dtack/../../source/inter/text.c \
	  $(SRC)/include/dtack/../../source/inter/textcur1.c \
	  $(SRC)/include/dtack/../../source/inter/texttcap.c \
	  $(SRC)/include/dtack/../../source/inter/textdos1.c \
	  $(SRC)/include/dtack/../../source/inter/textqnx1.c

# ----------------------------------------------


winc.p: \
	  $(SRC)/include/dtack/../../source/win/winc.c \
	  $(SRC)/include/dtack/../../source/win/winc_ipc.c \
	  $(SRC)/include/dtack/../../source/win/winc_dsk.c
	bash dtdxc.sh \
	  winc.p -fproto \
	  $(SRC)/include/dtack/../../source/win/winc.c \
	  $(SRC)/include/dtack/../../source/win/winc_ipc.c \
	  $(SRC)/include/dtack/../../source/win/winc_dsk.c
svc.p: \
	  $(SRC)/include/dtack/../../source/win/svc.c \
	  $(SRC)/include/dtack/../../source/win/svc_ipc.c \
	  $(SRC)/include/dtack/../../source/win/svc_dsk.c
	bash dtdxc.sh \
	  svc.p -fproto \
	  $(SRC)/include/dtack/../../source/win/svc.c \
	  $(SRC)/include/dtack/../../source/win/svc_ipc.c \
	  $(SRC)/include/dtack/../../source/win/svc_dsk.c
wins.p: \
	  $(SRC)/include/dtack/../../source/win/wins.c \
	  $(SRC)/include/dtack/../../source/win/wins_ipc.c \
	  $(SRC)/include/dtack/../../source/win/wins_dsk.c
	bash dtdxc.sh \
	  wins.p -fproto \
	  $(SRC)/include/dtack/../../source/win/wins.c \
	  $(SRC)/include/dtack/../../source/win/wins_ipc.c \
	  $(SRC)/include/dtack/../../source/win/wins_dsk.c



#----------------------------------------------------------------------------
deps: 

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
