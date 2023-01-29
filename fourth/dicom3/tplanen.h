/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* tplanen.h 0.12 95/09/18 Copyright (C) 1993,1994,1995. David A. Clunie. All rights reserved. */

#ifndef _DC3tplanen_HEADER_
#define _DC3tplanen_HEADER_

#include "tplanev.h"

enum ImagePlaneHeadFeet { HeadFirst,FeetFirst };

enum ImagePlanePosition { 
	Supine,Prone,LeftLateralDecubitus,RightLateralDecubitus };

enum ImagePlaneAxis { AnteriorPosterior,HeadFeet,LeftRight };

enum ImagePlanePlane {  Axial,Sagittal,Coronal,
			AxialLP, AxialRP,
			SagittalPF,SagittalIP,
			CoronalLI };

class AbstractImagePlane {
	Point3D center;
	Point3D tlhc;
	Vector3D rowvec;
	Vector3D colvec;

	friend class ImagePlane;	// uses the following methods ...

	Point3D	_get_center	(void)			{ return center; }
	void	_set_center	(Point3D p)		{ center=p; }
	void	_rotate_center	(Rotation3D r)		{ center=center*r; }
	void	_shift_center	(Vector3D shift)	{ center=center+shift; }

	Point3D	_get_tlhc	(void)			{ return tlhc; }
	void	_set_tlhc	(Point3D p)		{ tlhc=p; }
	void	_rotate_tlhc	(Rotation3D r)		{ tlhc=tlhc*r; }
	void	_shift_tlhc	(Vector3D shift)	{ tlhc=tlhc+shift; }

	void	_scale_tlhc	(double factor)	
		{
			Point3D origin(0,0,0);
			Vector3D v = (origin-tlhc)*factor;
			tlhc=origin-v;
		}

	Vector3D	_get_rowvec	(void)		{ return rowvec; }
	void		_set_rowvec	(Vector3D v)	{ rowvec=v; }
	void		_rotate_rowvec	(Rotation3D r)	{ rowvec=rowvec*r; }

	Vector3D	_get_colvec	(void)		{ return colvec; }
	void		_set_colvec	(Vector3D v)	{ colvec=v; }
	void		_rotate_colvec	(Rotation3D r)	{ colvec=colvec*r; }

public:
	AbstractImagePlane(void) {}

	AbstractImagePlane(ImagePlanePlane plane,double hfov,double vfov)
		{
			Point3D  o(0,0,0); center=o;
			double dh=hfov/2;
			double dv=vfov/2;
			switch (plane) {
				case Axial:
				case AxialLP:
					// Conventional - viewed "bottom up"
				{
					Point3D  b(-dh,-dv, 0); tlhc=b;
					Vector3D r( 1,  0,  0); rowvec=r;
					Vector3D c( 0,  1,  0); colvec=c;
					break;
				}
				case AxialRP:
					// Old EMI style - viewed "top down"
				{
					Point3D  b( dh,-dv, 0); tlhc=b;
					Vector3D r(-1,  0,  0);  rowvec=r;
					Vector3D c( 0,  1,  0);  colvec=c;
					break;
				}
				case Sagittal:
				case SagittalPF:
					// Conventional MRI viewed "from left"
				{
					Point3D  b( 0,-dh, dv); tlhc=b;
					Vector3D r( 0,  1,  0);  rowvec=r;
					Vector3D c( 0,  0, -1);  colvec=c;
					break;
				}
				case SagittalIP:
					// Conventional CT Lateral Scout
				{
					Point3D  b( 0,-dh, dv); tlhc=b;
					Vector3D r( 0,  0, -1);  rowvec=r;
					Vector3D c( 0,  1,  0);  colvec=c;
					break;
				}
				case Coronal:
				case CoronalLI:
					// Conventional MRI viewed "from front"
					// Conventional CT AP Scout
				{
					Point3D  b(-dh, 0, dv); tlhc=b;
					Vector3D r( 1,  0,  0); rowvec=r;
					Vector3D c( 0,  0, -1); colvec=c;
					break;
				}
			}
		}
};


class DerivedImagePlane
{
	Rotation3D	rotation;	// this to base
	Rotation3D	unrotation;	// base to this

	char *getOrientation(Vector3D vector)
		{
			char *orientation=new char[4];
			char *optr = orientation;
			*optr='\0';

			char orientationX = vector.getX() < 0 ? 'R' : 'L';
			char orientationY = vector.getY() < 0 ? 'A' : 'P';
			char orientationZ = vector.getZ() < 0 ? 'F' : 'H';

			double absX = fabs(vector.getX());
			double absY = fabs(vector.getY());
			double absZ = fabs(vector.getZ());

			int i;
			for (i=0; i<3; ++i) {
				if (absX>.0001 && absX>absY && absX>absZ) {
					*optr++=orientationX;
					absX=0;
				}
				else if (absY>.0001 && absY>absX && absY>absZ) {
					*optr++=orientationY;
					absY=0;
				}
				else if (absZ>.0001 && absZ>absX && absZ>absY) {
					*optr++=orientationZ;
					absZ=0;
				}
				else break;
				*optr='\0';
			}
			return orientation;
		}

	// derived classes pass access to VirtualImagePlane's methods ...

	virtual Point3D		_get_center	(void)			= 0;
	virtual void		_set_center	(Point3D p)		= 0;
	virtual void		_rotate_center	(Rotation3D r)		= 0;
	virtual void		_shift_center	(Vector3D shift)	= 0;

	virtual Point3D		_get_tlhc	(void)			= 0;
	virtual void		_set_tlhc	(Point3D p)		= 0;
	virtual void		_rotate_tlhc	(Rotation3D r)		= 0;
	virtual void		_shift_tlhc	(Vector3D shift)	= 0;
	virtual void		_scale_tlhc	(double factor)		= 0;

	virtual Vector3D	_get_rowvec	(void)			= 0;
	virtual void		_set_rowvec	(Vector3D v)		= 0;
	virtual void		_rotate_rowvec	(Rotation3D r)		= 0;

	virtual Vector3D	_get_colvec	(void)			= 0;
	virtual void		_set_colvec	(Vector3D v)		= 0;
	virtual void		_rotate_colvec	(Rotation3D r)		= 0;

public:
	DerivedImagePlane(ImagePlaneHeadFeet hfff,ImagePlanePosition posn)
		{
			switch(hfff) {
				case HeadFirst:
					break;
				case FeetFirst:
				{
					Rotation3D r(Y,180);
					rotation*=r;
					Rotation3D unr(Y,-180);
					unrotation*=unr;
					break;
				}
			}

			switch(posn) {
				case Supine:
					break;
				case Prone:
				{
					Rotation3D r(Z,180);
					rotation*=r;
					Rotation3D unr(Z,-180);
					unrotation*=unr;
					break;
				}
				case LeftLateralDecubitus:
				{
					Rotation3D r(Z,90);
					rotation*=r;
					Rotation3D unr(Z,-90);
					unrotation*=unr;
					break;
				}
				case RightLateralDecubitus:
				{
					Rotation3D r(Z,-90);
					rotation*=r;
					Rotation3D unr(Z,90);
					unrotation*=unr;
					break;
				}
			}
		}

	char *getRowOrientation(void)
		{
			return getOrientation(getRowVector());
		}
	char *getColOrientation(void)
		{
			return getOrientation(getColVector());
		}

	Point3D  getTLHC(void)		{ return _get_tlhc()*rotation; }
	Point3D  getCenter(void)	{ return _get_center()*rotation; }
	Vector3D getRowVector(void)	{ return _get_rowvec()*rotation; }
	Vector3D getColVector(void)	{ return _get_colvec()*rotation; }

	void setCorners(Point3D tlhc,Vector3D row,Vector3D col)
		{
			_set_tlhc(tlhc*unrotation);
			_set_rowvec((row*unrotation)/row.magnitude());
			_set_colvec((col*unrotation)/col.magnitude());
			_set_center((tlhc+row/2+col/2)*unrotation);
		}

	void angle(ImagePlaneAxis axis,double angle)
		{
			Index3D plane;
			switch(axis) {
				case LeftRight:		plane=X; break;
				case AnteriorPosterior:	plane=Y; break;
				case HeadFeet:		plane=Z; break;
			}
			Rotation3D r(plane,angle);
			Rotation3D br=r*unrotation;

			_rotate_tlhc(br);
			_rotate_center(br);
			_rotate_rowvec(br);
			_rotate_colvec(br);
		}

	void shift(Vector3D shift)
		{
			_shift_center(shift*unrotation);
			_shift_tlhc(shift*unrotation);
		}

	void scale(double factor)
		{
			Point3D origin(0,0,0);
			Vector3D shift = origin - _get_center();
			_shift_tlhc(shift);
			_scale_tlhc(factor);
			_shift_tlhc(-shift);
		}
};

class PatientPlane : public DerivedImagePlane
{
public:
	PatientPlane(ImagePlaneHeadFeet hfff,ImagePlanePosition posn) :
		DerivedImagePlane(hfff,posn)
		{}
};

class MachinePlane : public DerivedImagePlane
{
public:
	MachinePlane(void) :
		DerivedImagePlane(HeadFirst,Supine)
		{}
};

class ImagePlane :	public virtual AbstractImagePlane,
			public PatientPlane,
			public MachinePlane
{
	// instances of virtual methods of DerivedImagePlane
	// allows DerivedImagePlane methods access to AbstractImagePlane

	Point3D		_get_center	(void)
			{
				return AbstractImagePlane::_get_center();
			}

	void		_set_center	(Point3D p)
			{
				AbstractImagePlane::_set_center(p);
			}

	void		_rotate_center	(Rotation3D r)
			{
				AbstractImagePlane::_rotate_center(r);
			}

	void		_shift_center	(Vector3D shift)
			{
				AbstractImagePlane::_shift_center(shift);
			}


	Point3D		_get_tlhc	(void)
			{
				return AbstractImagePlane::_get_tlhc();
			}

	void		_set_tlhc	(Point3D p)
			{
				AbstractImagePlane::_set_tlhc(p);
			}

	void		_rotate_tlhc	(Rotation3D r)
			{
				AbstractImagePlane::_rotate_tlhc(r);
			}

	void		_shift_tlhc	(Vector3D shift)
			{
				AbstractImagePlane::_shift_tlhc(shift);
			}

	void		_scale_tlhc	(double factor)
			{
				AbstractImagePlane::_scale_tlhc(factor);
			}



	Vector3D	_get_rowvec	(void)
			{
				return AbstractImagePlane::_get_rowvec();
			}

	void		_set_rowvec	(Vector3D v)
			{
				AbstractImagePlane::_set_rowvec(v);
			}

	void		_rotate_rowvec	(Rotation3D r)
			{
				AbstractImagePlane::_rotate_rowvec(r);
			}


	Vector3D	_get_colvec	(void)
			{
				return AbstractImagePlane::_get_colvec();
			}

	void		_set_colvec	(Vector3D v)
			{
				AbstractImagePlane::_set_colvec(v);
			}

	void		_rotate_colvec	(Rotation3D r)
			{
				AbstractImagePlane::_rotate_colvec(r);
			}
public:
	ImagePlane(
		ImagePlanePlane		plane,
		double fov,
		ImagePlaneHeadFeet	hfff,
		ImagePlanePosition	posn
	) :
		AbstractImagePlane(plane,fov,fov),
		PatientPlane(hfff,posn),
		MachinePlane()
		{
		}

	ImagePlane(
		ImagePlanePlane		plane,
		double hfov,
		double vfov,
		ImagePlaneHeadFeet	hfff,
		ImagePlanePosition	posn
	) :
		AbstractImagePlane(plane,hfov,vfov),
		PatientPlane(hfff,posn),
		MachinePlane()
		{
		}

	ImagePlane(
		ImagePlaneHeadFeet	hfff,
		ImagePlanePosition	posn
	) :
		AbstractImagePlane(),
		PatientPlane(hfff,posn),
		MachinePlane()
		{
		}
};

#endif /* _DC3tplanen_HEADER_ */





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
