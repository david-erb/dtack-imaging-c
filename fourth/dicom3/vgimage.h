/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* vgimage.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _VGImage_HEADER_
#define _VGImage_HEADER_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include "ttypes.h"
#include "vgerror.h"

class VGImage : public VGError {
protected:
	unsigned numrows;
	unsigned numcols;
public:
	VGImage(void)			{}
	VGImage(unsigned width,unsigned height)
		{
			numcols=width;
			numrows=height;
		}
	virtual ~VGImage() {};

	unsigned getWidth(void)		{ return numcols; }
	unsigned getHeight(void)	{ return numrows; }

	virtual unsigned	getDepth(void) = 0;
	virtual Uint16		getPixel(unsigned r,unsigned c) = 0;
	virtual void		setPixel(unsigned r,unsigned c,Uint16 p) = 0;
	virtual void 		setRow(unsigned r,unsigned char *newrow) = 0;

	virtual int		Read(istream *ist,char byteorder=0) = 0;
};

template<class T,Uint16 mask>
class VGImage_template : public VGImage {
protected:
	T	*data;
	T	**row;

	void CreateRowTable(void)
		{
			row = new T * [numrows];
			if (row) {
				T *ptr;
				unsigned i;
				for (i=0,ptr=data; i<numrows;++i,ptr+=numcols)
					row[i]=ptr;
			}
			else {
				error("Can't allocate row table");
			}
		}
#ifdef PARANOIDERRORCHECKING
	int Check(unsigned r,unsigned c)
		{

			if (!row || !data) {
				error("row or data not allocated");
				return 0;
			}
			else if (r>=numrows || c>=numcols) {
				error("co-ordinates out of range");
				return 0;
			}
			else {
				return 1;
			}
		}
#endif /* PARANOIDERRORCHECKING */
public:
	VGImage_template(void)
		: VGImage()		{}

	VGImage_template(unsigned r,unsigned c)
		: VGImage(r,c)
		{
			data = new T [numrows*numcols];
			if (data) {
				CreateRowTable();
			}
			else {
				error("Can't allocate data");
				row=0;
 			}
		}

	VGImage_template(unsigned r,unsigned c,T *d)
		: VGImage(r,c)
		{
			data = d;
			if (data) {
				CreateRowTable();
			}
			else {
				error("No data supplied");
				row=0;
 			}
		}

	virtual ~VGImage_template(void)
		{
			if (row)  delete [] row;
			if (data) delete [] data;
		}

	virtual unsigned getDepth(void)	= 0;

	Uint16 getPixel(unsigned r,unsigned c)
		{
			return
#ifdef PARANOIDERRORCHECKING
				Check(r,c) ?
#endif /* PARANOIDERRORCHECKING */
				(*(row[r]+c))&mask
#ifdef PARANOIDERRORCHECKING
				: 0
#endif /* PARANOIDERRORCHECKING */
			;
		}
	void setPixel(unsigned r,unsigned c,Uint16 p)
		{
#ifdef PARANOIDERRORCHECKING
			if (Check(r,c))
#endif /* PARANOIDERRORCHECKING */
				*(row[r]+c)=p&mask;
		}

	void setRow(unsigned r,unsigned char *newrow)
		{
#ifdef PARANOIDERRORCHECKING
			if (Check(r,c))
#endif /* PARANOIDERRORCHECKING */
				memcpy((char *)row[r],(char *)newrow,
					numcols*sizeof(T));
		}
};

class VGImage_8bit  : public VGImage_template<unsigned char,(Uint16)0xff> {
public:
	VGImage_8bit(void)
		: VGImage_template<unsigned char,(Uint16)0xff>()	{}

	VGImage_8bit(unsigned r,unsigned c)
		: VGImage_template<unsigned char,(Uint16)0xff>(r,c)	{}

	VGImage_8bit(unsigned r,unsigned c,unsigned char *d)
		: VGImage_template<unsigned char,(Uint16)0xff>(r,c,d)	{}

	virtual ~VGImage_8bit() {}

	int Read(istream *ist,char byteorder)
		{
			(void)byteorder;
			unsigned r;
			for (r=0; r<numrows; ++r)
				ist->read((char *)(row[r]),numcols);
			return ist->good();
		}

	unsigned getDepth(void)		{ return 8; }
};

class VGImage_12bit : public VGImage_template<Uint16,(Uint16)0xfff> {
public:
	VGImage_12bit(void)
		: VGImage_template<Uint16,(Uint16)0xfff>()		{}

	VGImage_12bit(unsigned r,unsigned c)
		: VGImage_template<Uint16,(Uint16)0xfff>(r,c)		{}

	VGImage_12bit(unsigned r,unsigned c,Uint16 *d)
		: VGImage_template<Uint16,(Uint16)0xfff>(r,c,d)		{}

	virtual ~VGImage_12bit() {}

	int Read(istream *ist,char byteorder)
		{
			unsigned r;
			for (r=0; r<numrows; ++r) {
				unsigned c;
				for (c=0; c<numcols; ++c) {
					unsigned char c1,c2;
					if (byteorder == 'b') {
						ist->read(&c2,1);
						ist->read(&c1,1);
					}
					else if (byteorder == 'l') {
						ist->read(&c1,1);
						ist->read(&c2,1);
					}
					else {
						error("Bad byteorder");
						return 0;
					}
					*(row[r]+c)=(c2<<8)+c1;
				}
			}
			return ist->good();
		}

	unsigned getDepth(void)		{ return 12; }
};

class VGImage_16bit : public VGImage_template<Uint16,(Uint16)0xffff> {
public:
	VGImage_16bit(void)
		: VGImage_template<Uint16,(Uint16)0xffff>()		{}

	VGImage_16bit(unsigned r,unsigned c)
		: VGImage_template<Uint16,(Uint16)0xffff>(r,c)		{}

	VGImage_16bit(unsigned r,unsigned c,Uint16 *d)
		: VGImage_template<Uint16,(Uint16)0xffff>(r,c,d)	{}

	virtual ~VGImage_16bit() {}

	int Read(istream *ist,char byteorder)
		{
			unsigned r;
			for (r=0; r<numrows; ++r) {
				unsigned c;
				for (c=0; c<numcols; ++c) {
					unsigned char c1,c2;
					if (byteorder == 'b') {
						ist->read(&c2,1);
						ist->read(&c1,1);
					}
					else if (byteorder == 'l') {
						ist->read(&c1,1);
						ist->read(&c2,1);
					}
					else {
						error("Bad byteorder");
						return 0;
					}
					*(row[r]+c)=(c2<<8)+c1;
				}
			}
			return ist->good();
		}

	unsigned getDepth(void)		{ return 16; }
};

#endif /* _VGImage_HEADER_ */






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
