/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* vgdisplay.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _VGDisplayImage_HEADER_
#define _VGDisplayImage_HEADER_

#include "vgimage.h"
#include "vgerror.h"

class VGDisplay : public VGError {
	friend class VGWindow;
	friend class VGColormap;

	char *		name;
	Display *	display;
	int 		screen;
	Window 		rootwindow;
	unsigned long 	foreground;
	unsigned long 	background;
	Visual *	visual;
	unsigned 	depth;
	int 		bitmappad;

	char * 		getName(void)		{ return name; }
	Display *	getDisplay(void)	{ return display; }
	int 		getScreen(void)		{ return screen; }
	Window 		getRootwindow(void)	{ return rootwindow; }
	unsigned long 	getForeground(void)	{ return foreground; }
	unsigned long 	getBackground(void)	{ return background; }
	Visual *	getVisual(void)		{ return visual; }
	unsigned 	getDepth(void)		{ return depth; }
	int 		getBitmappad(void)	{ return bitmappad; }
public:
	VGDisplay(void)
		{
			name = NULL;
			if ((display=XOpenDisplay(name)) == NULL) {
				error("Can't open display");
			}
			else {
				screen     = DefaultScreen(display);
				rootwindow = RootWindow(display,screen);
				foreground = BlackPixel(display,screen);
				background = WhitePixel(display,screen);
				visual     = DefaultVisual(display,screen);
				depth      = DefaultDepth(display,screen);
				bitmappad  = XBitmapPad(display);
			}
		}

	XEvent NextEvent(void)
		{
			XEvent report;
			XNextEvent(display,&report);
			return report;
		}
};

class VGColormap : public VGError {
	friend class VGWindow;

	VGDisplay *	display;
	Colormap 	colormap;
	char *		map;
	unsigned long	maplength;
	unsigned long *	pixels;
	unsigned long	pixelslength;

	Colormap getColormap(void)	{ return colormap; }
public:
	VGColormap(VGDisplay *d)
		{
			display=d;
			map=0;
			maplength=0;
			pixels=0;
			pixelslength=0;
#ifdef CRAP
			colormap = XCreateColormap(
				display->getDisplay(),
				display->getRootwindow(),
				display->getVisual(),
				AllocNone
			);
#else /* CRAP */
			colormap = XCopyColormapAndFree(
				display->getDisplay(),
				DefaultColormap(
					display->getDisplay(),
					display->getScreen()
				)
			);
#endif /* CRAP */
			if (colormap == (Colormap) None) {
				error("Can't create colormap");
			}
		}

	~VGColormap(void)
		{
			XFreeColormap(
				display->getDisplay(),
				colormap
			);
			if (map) delete[] map;
			if (pixels) delete[] pixels;
		}

	void setLinearGray(const unsigned long length = 256,
		const unsigned num = 240)
		// length - range of possible "real" pixel values to map from
		// num    - the number of colors to allocate
		{
			pixels = new unsigned long [num];
			map = new char [length];
			if (pixels && map) {
				int status = XAllocColorCells(
					display->getDisplay(),
					colormap,
					False,		// contiguous planes
					(unsigned long *) 0,	// plane masks
					0,			// # of planes
					pixels,
					num
				);
				if (status) {
					unsigned long i;
					XColor c;
					c.flags=DoRed | DoGreen | DoBlue;
					for (i=0; i<num; ++i) {
						unsigned short k = 
							(unsigned short)
								((65535*i)/num);
						c.pixel	= pixels[i];
					 	c.red	= k;
					 	c.green	= k;
						c.blue	= k;
						XStoreColor(
							display->getDisplay(),
							colormap,
							&c
						);
					}
					for (i=0; i<length; ++i) {
						map[i]=(char)
							pixels[(i*num)/length];
					}
					maplength=length;
					pixelslength=num;
				}
				else {
					error("couldn't allocate color cells");
				}
			}
			else {
				error("couldn't allocate space for colormap");
			}
		}

	void setMapLevelWidth(unsigned level,unsigned width)
		{
			unsigned long i;
			long center=level;
			long bottom=(long)center-width/2;
			long top=(long)center+width/2;
			if (bottom < 0) bottom=0;
			if (top > (maplength-1)) top=maplength-1;

			unsigned long bottompixel = 
				pixels[(bottom*pixelslength)/maplength];
			unsigned long toppixel = 
				pixels[(top*pixelslength)/maplength];

			XColor c;
			c.flags=DoRed | DoGreen | DoBlue;
			for (i=0; i<bottompixel; ++i) {
				unsigned short k = 0;
				c.pixel	= pixels[i];
				c.red	= k;
				c.green	= k;
				c.blue	= k;
				XStoreColor(
					display->getDisplay(),
					colormap,
					&c
				);
			}
			for (i=bottompixel; i<toppixel; ++i) {
				unsigned short k = (unsigned short)
					(((i-bottompixel)*65535)
						/(toppixel-bottompixel));
				c.pixel	= pixels[i];
				c.red	= k;
				c.green	= k;
				c.blue	= k;
				XStoreColor(
					display->getDisplay(),
					colormap,
					&c
				);
			}
			for (i=toppixel; i<pixelslength; ++i) {
				unsigned short k = 65535;
				c.pixel	= pixels[i];
				c.red	= k;
				c.green	= k;
				c.blue	= k;
				XStoreColor(
					display->getDisplay(),
					colormap,
					&c
				);
			}
		}

	void setImageLevelWidth(unsigned level,unsigned width)
		{
			unsigned long i;
			long center=level;
			long bottom=(long)center-width/2;
			long top=(long)center+width/2;
			if (bottom < 0) bottom=0;
			if (top > (maplength-1)) top=maplength-1;

			for (i=0; i<bottom; ++i) {
				map[i]=(char)pixels[0];
			}
			for (i=bottom; i<top; ++i) {
				map[i]=(char)
					pixels[((i-bottom)*pixelslength)
						/(top-bottom)];
			}
			for (i=top; i<maplength; ++i) {
				map[i]=(char)pixels[pixelslength-1];
			}
		}

	char *		getMap(void)		{ return map; }
	unsigned long	getMaplength(void)	{ return maplength; }
};

class VGWindow : public VGError {
	friend class VGWindowImage;

	VGDisplay *	vgdisplay;

	Window 		window;
	int 		x;
	int 		y;
	unsigned 	width;
	unsigned	height;
	unsigned 	border;

	Window 		getWindow(void)		{ return window; }
	int 		getX(void)		{ return x; }
	int 		getY(void)		{ return y; }
	unsigned 	getWidth(void)		{ return width; }
	unsigned	getHeight(void)		{ return height; }
	unsigned 	getBorder(void)		{ return border; }

	char * 		getName(void)
				{ return vgdisplay->getName(); }
	Display *	getDisplay(void)
				{ return vgdisplay->getDisplay(); }
	int 		getScreen(void)
				{ return vgdisplay->getScreen(); }
	Window 		getRootwindow(void)
				{ return vgdisplay->getRootwindow(); }
	unsigned long 	getForeground(void)
				{ return vgdisplay->getForeground(); }
	unsigned long 	getBackground(void)
				{ return vgdisplay->getBackground(); }
	Visual *	getVisual(void)
				{ return vgdisplay->getVisual(); }
	unsigned 	getDepth(void)
				{ return vgdisplay->getDepth(); }
	int 		getBitmappad(void)
				{ return vgdisplay->getBitmappad(); }
public:
	VGWindow(VGDisplay *d,unsigned w,unsigned h)
		{
			vgdisplay=d;

			x=0;
			y=0;
			width=w;
			height=h;
			border=4;

			window = XCreateSimpleWindow(
				getDisplay(),
				getRootwindow(),
				x,y,width,height,
				border,
				getForeground(),
				getBackground()
			);
		}

	~VGWindow(void)
		{
			XDestroyWindow(
				getDisplay(),
				window
			);
		}

	void SelectInput(void)
		{
			XSelectInput(getDisplay(),window,
				ExposureMask|ButtonPressMask|ButtonReleaseMask);
		}

	void Map(void)
		{
			XMapWindow(getDisplay(),window);
		}

	void Clear(void)
		{
			XClearWindow(getDisplay(),window);
		}

	void setColormap(VGColormap *colormap)
		{
			XSetWindowColormap(
				getDisplay(),
				getWindow(),
				colormap->getColormap()
			);
		}
};

class VGWindowImage : public VGError {
	VGWindow *	window;
	XImage *	ximage;
	GC		gc;
public:
	VGWindowImage(VGWindow *w)
		{
			window=w;

			// Null parameters are filled in by XCreateImage ...

			ximage = XCreateImage(window->getDisplay(),
					window->getVisual(),
					window->getDepth(),
					ZPixmap,
					0,		// offset
					(char *) 0,	// image data
					window->getWidth(),
					window->getHeight(),
					window->getBitmappad(),
					0		// bytes per line
			);

			if (ximage) {
				ximage->data = new 
					char [ximage->bytes_per_line*
						window->getHeight()];
				if (!ximage->data) {
					error("Couldn't allocate ximage data");
					XDestroyImage(ximage);
					ximage=0;
				}
				else {
					gc = DefaultGC(
						window->getDisplay(),
						window->getScreen()
					);
				}
			}
			else {
				error("Couldn't allocate ximage");
			}
		}

	void Put(void)
		{
			XPutImage(
				window->getDisplay(),
				window->getWindow(),
				gc,ximage,
				0,0,0,0,
				window->getWidth(),
				window->getHeight()
			);
		}

	void Clear(void)
		{
			XClearWindow(
				window->getDisplay(),
				window->getWindow()
			);
		}

	void Fill(void)
		{
			unsigned i;
			for (i=0; i<ximage->height; ++i) {
				unsigned j;
				char *p = ximage->data
					+ i*ximage->bytes_per_line;
				for (j=0; j<ximage->width; ++j) *p++=i;
			}
		}

	void Load(VGImage_8bit *image)
		{
			if (image->getHeight() != ximage->height
			 || image->getWidth()  != ximage->width) {
				error("dimensions don't match");
			}
			else {
				unsigned i;
				for (i=0; i<ximage->height; ++i) {
					unsigned j;
					char *p = ximage->data
						+ i*ximage->bytes_per_line;
					for (j=0; j<ximage->width; ++j)
						*p++=image->getPixel(i,j);
				}
			}
		}

	void Load(VGImage *image,VGColormap *vgmap)
		{
			char *map=vgmap->getMap();
			if (image->getHeight() != ximage->height
			 || image->getWidth()  != ximage->width) {
				error("dimensions don't match");
			}
			else if (vgmap->getMaplength()
					!= 1<<image->getDepth()) {
				error("map length wrong");
			}
			else {
				unsigned i;
				for (i=0; i<ximage->height; ++i) {
					unsigned j;
					char *p = ximage->data
						+ i*ximage->bytes_per_line;
					for (j=0; j<ximage->width; ++j)
						*p++=map[image->getPixel(i,j)];
				}
			}
		}

	void Load(VGImage *image,VGColormap *vgmap,unsigned x,unsigned y)
		{
			char *map=vgmap->getMap();
			if (y+image->getHeight() > ximage->height
			 || x+image->getWidth()  > ximage->width) {
				error("image too big");
			}
			else if (vgmap->getMaplength()
					!= 1<<image->getDepth()) {
				error("map length wrong");
			}
			else {
				unsigned i;
				for (i=0; i<image->getHeight(); ++i) {
					unsigned j;
					char *p = ximage->data
						+ (y+i)*ximage->bytes_per_line
						+ x;
					for (j=0; j<image->getWidth(); ++j)
						*p++=map[image->getPixel(i,j)];
				}
			}
		}

	unsigned getHeight(void)	{ return ximage->height; }
	unsigned getWidth(void)		{ return ximage->width; }
};

#endif /* _VGDisplayImage_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
