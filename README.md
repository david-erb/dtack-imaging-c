# dtack-imaging-c

This is a pure C image processing library I developed when writing custom applications for my clients.
It was quite cost efficient to have a set of purpose-built reusable "objects" which I could make use of in each new project.

I wrote this code in the late 1990's, so now, 25+ years later, they look rather dated! 
I used the concepts of "function groups" and strict naming conventions to try to accomplish what object oriented programming does now.  I used the C preprocessor include feature to implement crudely what you get from C++ templates.

The library is compilable by the compilers at the time such as Watcom, Borland, Microsoft, Solaris, Sunos, Silicon Graphics, early Linux GNU, and FreeBSD gnu.  There is a central configuration script which generates makefiles for the different compilers

## Algorithms (alg directory)

There are some algorithms I explored implementing from the printed literature.

name | description
---- | -----------
abu89 | Automatic Thresholding of Gray-Level Pictures Using Two-Dimensional Entropy
esc94 | Error diffusion dithering for laser printers
flo76 | Produces an two-valued output image by error diffusion
kap85 | A New Method for Gray-Level Picture Thresholding Using the Entropy of the Histogram
lev85 | Segmentation performance By-blob or by-label uniformity number
per87 | Iterative thresholding
sez90 | Histogram peak detection and multi-thresholding
wes78 | Segmentation performance by busyness analysis.

 
## Apps (app directory)

Applications I wrote using the library.

name | description
---- | -----------
dtview | server executive for image visual display. Run as server waiting for client IPC requests and for mouse/keyboard events.
reverb | client/server IP address mapper
tabptz | Server to handle specialized pan-tilt-zoom camera commands.
tabroam | Auto-roam ptz. Continuously read presets file and send goto commands to tabrobot server.
tableau | tableau streaming video client.  Stream load and show full-motion video from the Tableau Internet server.  
tabgrab | Grab frames and stuff them into file or shared memory.
tabchief | Stream load and show full-motion video from the Tabchief Internet server.
umplay | MPEG player. Mpeg playback client and server.  Can also do interactive file playback. 
wrangler | video server dispatch. Accept HTTP commands from trusted server to launch and  monitor video push drones. 

## Command line tools (tool directory)

This is a set of tools which are wrapped in a command line dispatcher for chaining together using the command line or shell script.

name | description
---- | -----------
dttool | main program, image subcommand dispatch
box | draw box on image
convolve | convolve image with kernel
copy | copy image
sum | make sum of row or column
blob | segment blobs in image an colorize them
color | color converter
cooc | make co-occurrence matrix image
enclose | draw enclosing boxes around objects
fft | fft transform
dttoolgen | generate image
global | segmentation performance
dttoolhist | make histogram of image
linear | linear operator on row or column
logical | logical operator on row or column
lookup | apply lookup table to image
median | median filter
morph | morphological operator on image with kernel
multithresh | multiple threshold gray level display
nonlinear | operator on image (log, sqrt, cos, sin, abs)
paste | paste one image on another
profile | make profile of row or column
dttoolresample | decimate or expand
run | make run of row or column
slice | slice gray level range
snr | signal to noice ratio
stats | statistics about image
subabs | subtract absolute value
dttooltext | write text on image
thresh | simple constant bilevel threshold

## Library function groups (source directory)

The purpose of this library is to support image processing
algorithm development.
The library itself is not considered a commercial product.

The library is loosely organized by functional units.
Functional units are simply families of routines,
all of which operate on any particular data type.
In object oriented parlance, the functional units are classes.
The functions are the methods of the classes,
and the data structure they operate on.

At the image level, the organization starts to fade from
object oriented into procedure oriented.
This manifests itself in the image class which
is a large class with very diverse methods.
There are not separate classes for different types of images.
If you consider this library to be a DSP library,
then the image is the primary input signal.

The major functional units listed in rough order of
decreasing generality:
- string operations
- error handling
- debug output
- console output
- operating system interface
- file handling
- interprocess communication
- one-dimensional operations: arithmetic, data type conversion, sorting
- major memory
- data compression
- histogram
- lookup tables
- parameter maintenance
- random numbers
- extendable stack
- configuration management
- image operations


The major operations on the image data type are:
- connected components
- data type conversion
- occurrence matrices
- two-dimensional convolution
- joint probabilities
- point operators: log, lookup,
- geometric transformations: resample
- statistical reduction: variance, sums, ranges
- arithmetic
- comparison
- synthetic image generation
- export and import
- annotation

name | description
---- | -----------
1d | fast operations on 1d vectors
bigmem | fast allocation, blt, io and other operators on big memory blocks
color | colorspace conversion routines
dg | http client/server routines
fb | frame grabber hardware support
high | high level 2d image operations such as convolution, morphological and fft
inter | human interactions for console and window dialogs
keybd | keyboard interactions
low | low level non-image routines such as sockets, events, shared memory and command line parsing
mpeg | mpeg encoding
screen | screen management for image display
win | client to window manager
test | tests for function groups
xchg | image file format conversion

## Library details (source directory)

And (if you're still reading!), here are some more details about the library function groups.


### Library dtxchg: image file format conversion
name | description
---- | -----------
bmp.c | Microsoft Windows bitmap files
dib.c | Microsoft Windows device independent bitmap s
dtexim1.c | image import and export example program
export.c | image export
import.c | image import
lut8xchg.c | 8-bit lut import and export
parse.c | external name parsing
read.c | image reading
readtif.c | tif file reading
wmf.c | Microsoft Windows metafile with Adobe placeable header
write.c | image writing
writebmp.c | write a Microsoft Windows Bitmap File
writewmf.c | write a Microsoft Windows metafile
xchg_dat.c | program constants
xchg_dsk.c | import and export from windowing server disk files
xchg_dtm.c | import and export from an NCSA Collage server
xchg_jpg.c | import and export jpeg files
xchg_scr.c | import and export from the screen interface
xchg_win.c | import and export from a windowing server
xchg_wxm.c | import and export Windows metafiles from Visual Basic
xchg_wxx.c | import and export Windows files from Visual Basic

### Library dtwinc: distributed image display 
name | description
---- | -----------
dsk.c | windows stored on disk files
dsk_c.c | Windows stored on disk files, client
svc.c | server support functions
svc_dsk.c | windowing server on disk
svc_ipc.c | windowing server on interprocess communication
winc.c | client umbrella to windowing server
winc_dsk.c | client to disk windowing server
winc_ipc.c | client to interprocess communication windowing server
wins.c | windowing server umbrella
wins_dsk.c | windowing server implemented on disk
wins_ipc.c | windowing server implemented on interprocess communication

### Library dtfb: image capture from framegrabber
name | description
---- | -----------
fb.c | fb API
fbbt.c | BTTV driver for Bt848 on Linux
fbcortex.c | Cortex I board on QNX
fbdir.c | read files from directory
fbgen.c | generate synthetic images
fbmeteor.c | meteor board on linux
fbpsi.c | VMC2
fbsnap.c | Sanppy
fbutil.c | fast utility functions
fbvbmci.c | MCI interface to Video Blaster
fbvfw.c | Video for Windows
fbvidmux.c | vidmux utility functions
fbvmc.c | raw access to VMC2
meteor_ioctl.h | include for meteor
psi.h | include for fbpsi
bttv.h | include for BTTV driver
bt848.h | include for BTTV driver

### Library dthigh: image processing
name | description
---- | -----------
adj.c | connected components
adj_run.c | connected components from runs
arith.c | image arithmetic
assign.c | assign pixel values
blob.c | functions operating on blob lists
cdecl.c | non-DLL functions
cast.c | image type conversion
check.c | image type checking
cmp.c | image comparison
col.c | column extraction/replacement
constant.c | set image to constant
conv_d.c | image convolution for double-precision
conv_l.c | image convolution for unsigned integer
conv_s.c | image convolution for signed integer
convolve | image convolution included template
convolve.c | image convolution interface
cooc.c | coocurrence matrices
cooc2.c | coocurrence matrices, optimized
cumul.c | cumulative probability
dbg.c | debug image values
diff.c | image attention
err.c | handle image operational errors
fft.c | FFT API
fftw.c | FFT API implementation using fftw package
gen1.c | generate synthetic images
gen2.c | generate synthetic images
gen3.c | generate synthetic images
gen_dat.c | data for synthetic image generation
gen_kern.c | generate kernels
hist1.c | image histogram functions
hist2.c | image histogram plotting functions
image.c | image creation and deleting
joint.c | joint probability
joint2.c | joint probability, optimized
label1.c | connected components labeling
label2.c | connected components labeling
log.c | image pixel logarithm
logical.c | image logical operations
lookup.c | image pixel lookup
matrix.c | treat image as matrix
mean.c | compute image mean
median.c | median filtering
median.i | template median filtering
morph.c | morphological operations
morph3x3.i | template morphological operations
noise.c | noise addition
pixel.c | pixel utility
plot1.c | plot into image
plot2.c | plot into image
plot_txt.c | plot text into image
range.c | compute range of image values
resample.c | image interpolated scaling
rotate.c | 90 degree rotation
run.c | image horizontal runs
run_dat.c | constant data for image runs
run_len.c | run length statistics
runf.c | fast image horizontal runs
scan.c | scan for horizontal runs
stats.c | compute statistics on image
sum.c | image sums
sum_cols.c | column sums
sum_rows.c | row sums	
sumsq.c | sum of squares
sumsqd.c | sum of squared image differences
sumsqs.c | sum of squared scalar interface
thresh.c | image thresholding
transpos.c | image transpose

### Library dtcolor: image color space conversion
name | description
---- | -----------
convert.c | color space conversion routines
org.c | color organization object support routines
r_g_b.c | convert from band-interleaved RGB
rgb.c | convert from 16-bit RGB
rgb888.c | convert from byte-interleaved RGB
y_cb_cr.c | convert from band-interleaved YCbCr
ycbycr.c | convert from 16-bit 4:2:2 YCbCr
yuyv.c | convert from YUV

### Library dtscr: image display on console or window
name | description
---- | -----------
candum.c | dummy canvas implementation
canfastd.c | dummy canvas implementation on Fastgraph for DOS
canfastg.c | canvas implementation on Fastgraph for DOS
canhws.c | canvas implementation on hierarchical window systems
canhwsd.c | canvas implementation on dummy hws
canpoll.c | canvas keyboard/mouse poll function
cansvgac.c | canvas implementation for SVGACC for DOS
cansvgad.c | dummy canvas implementation for SVGACC for DOS
canvas.c | canvas screen interface
hws.c | hierarchical windows screen interface
hwsfgw.c | hierarchical windows screen implemented on Fastgraph for Windows
hwsmsw.c | hierarchical windows screen implemented on Windows API
hwsqw4.c | hierarchical windows screen implemented on QNX Windows
hwsx.c | hierarchical windows screen implemented on X Window (xlib)
scr_act.c | screen action handling
scr_cur.c | screen cursor drawing
scr_dbg.c | screen debug
scr_main.c | screen interface functions
scrcan.c | screen on canvas
scrcand.c | dummy screen on canvas
scrdum.c | screen on dummy
scrhws.c | screen on hierarchical windows system
scrhwsd.c | dummy screen on hierarchical windows system
vga.c | VGA utility functions

### Library dtbigmem: segmented memory management
name | description
---- | -----------
font.c | font generation functions
mem.c | big memory umbrella
mem_disk.c | big memory on disk implementation
mem_ems.c | big memory on EMS for DOS implementation
mem_ram.c | big memory on simple RAM implementation
mem_std.c | common big memory functions
memclip.c | rectangular region clipper
memcmp.c | big memory region comparison
memconf.c | big memory configuration management
memcopy.c | big memory region copying
memgetp.c | big memory subregion extraction
memio.c | big memory file reading and writing 
memparse.c | big memory spec parsing
memputp.c | big memory subregion insertion
memset.c | set big memory to constant
memsourc.c | big memory source poll handling
memtxms.c | big memory on XMS for DOS implementation
memxmsif.c | another big memory on XMS for DOS implementation
memxmslb.c | another big memory on XMS for DOS implementation
stack.c | stack implemented on top of big memory
stack_io.c | stack file reading an writing

### Library dtdg: virtual circuit http interprocess communication
name | description
---- | -----------
delay.c | standard delay command
dgclient.c | http client
dgserver.c | http server
exit.c | standard exit command
header.c | header processing
line.c | line input
request.c | handle client request
response.c | response routines
rev.c | library revision

### Library dtlow: low level timing, logging, sockets, etc
name | description
---- | -----------
assert.c | test runtime truth assertions
cdecl.c | non-vararg front end to functions with variable arg lists
cipher.c | cipher routines
conf_dat.c | configuration constants
cons.c | virtual console implementation
cstack.c | C stack
dbg.c | runtime debug routines
dbg2.c | more runtime debug routines
dbg_dat.c | debug routine constants
dbtable.c | database tables
dtenv1.c | environment functions example program
dtenv2.c | environment functions example program
dtenv3.c | environment functions example program
dtenv4.c | environment functions example program
dthist1.c | histogram functions example program
dthist2.c | histogram functions example program
dtparam1.c | parameter functions example program
dtport1.c | I/O port example program
enter.c | program environment creation functions
err.c | error handling functions
err2.c | more error handling functions
eventw.c | event signals
f.c | fopen simulator
fd.c | file interface implementation
ftp.c | interface to Internet FTP protocol
heap2.c | C runtime heap checking functions
heap3.c | C runtime heap checking functions
heapmon.c | C runtime heap monitoring functions
hist.c | histogram functions
hist2.c | histogram functions, optimized
hist_da.c | histogram functions static data
ipc.c | interprocess communication umbrella
ipcdde.c | Microsoft Windows DDE ipc implementation
ipcqbaby.c | qbaby ipc implementation
ipcqnx4.c | QNX ipc implementation
ipcrock.c | socket ipc implementation
leave.c | program environment destruction functions
license.c | license manager interface
liner.c | line input from text files
list.c | keyed list implementation
list_io.c | keyed list input/output functions
lut8.c | 8-bit lookup table functions
lut8_dat.c | data for 8-bit lookup table functions
lut8_mat.c | 8-bit lookup table matching functions
matrix.c | matrix operations
matrix2d.c | matrix operations for 2d coordinate transmissions
matrix3d.c | matrix operations for 3d coordinate transmissions
msgu.c | message queues for unix
msw.c | Microsoft Windows low level interface
ntype.c | data type functions
ntype_da.c | static data for type functions
points.c | point operations
poly.c | polynomial coordinate transformations
os_delay.c | delay functions
os_env.c | environment variable functions
os_futil.c | file utility functions
os_heap.c | heap monitoring functions	
os_heap2.c | more heap monitoring functions
os_mem.c | heap allocation
os_time.c | real time functions
param.c | command line keyword parameter functions
phys.c | physical memory functions
pnm.c | Portable Anymap header functions
portio.c | direct I/O port access
prj.c | project constant access functions
prj_dat.c | project constant data
qnx4.c | QNX low level functions
rev.c | revision reporting
say.c | functions to output to console
serial.c | serial comms
serialu.c | serial comms for Unix
serialw.c | serial comms for Windows
semu.c | semaphores for unix
semw.c | semaphores for Windows
shmu.c | shared memory for Unix
shmw.c | shared memory for Windows
str.c | string handling utilities
threads.c | thread manager
thread.c | thread object common routines
threadu.c | thread object for Unix
threadw.c | thread object for Windows
usage.c | emit program usage
yield.c | runtime yield functions

### Library dt1d: 1 dimensional mathematics
name | description
---- | -----------
1d.c
1d_abs.c
1d_arith.c
1d_cast.c
1d_clip.c
1d_cmp.c
1d_conv.c
1d_cumf.c
1d_cumr.c
1d_dat.c
1d_diff.c
1d_fft.c
1d_fftw.c
1d_logic.c
1d_look.c
1d_noise.c
1d_ntype.c
1d_ran.c
1d_range.c
1d_set.c
1d_sort1.c
1d_sort5.c
1d_stats.c
1d_sum.c
1d_sumsd.c
1d_sumsq.c
1d_sumss.c
1d_thres.c
1d_vari.c
