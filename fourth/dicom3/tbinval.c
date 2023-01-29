/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




// Automatically generated from template - EDITS WILL BE LOST

// Generated by binval.awk with options method

#include <strstream.h>

#include "ttypes.h"
#include "tbinval.h"

char *
DC3BinaryValueDescription_PregnancyStatus(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0001:
			ost << "Not Pregnant" << '\0';
			return ost.str();
		case 0x0002:
			ost << "Possibly Pregnant" << '\0';
			return ost.str();
		case 0x0003:
			ost << "Definitely Pregnant" << '\0';
			return ost.str();
		case 0x0004:
			ost << "Unknown" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_RegionSpatialFormat(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "None" << '\0';
			return ost.str();
		case 0x0001:
			ost << "2D (Tissue or Flow)" << '\0';
			return ost.str();
		case 0x0002:
			ost << "M-Mode (Tissue or Flow)" << '\0';
			return ost.str();
		case 0x0003:
			ost << "Spectral (Tissue or Flow)" << '\0';
			return ost.str();
		case 0x0004:
			ost << "Wave Form" << '\0';
			return ost.str();
		case 0x0005:
			ost << "Graphics" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_RegionDataType(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "None" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Tissue" << '\0';
			return ost.str();
		case 0x0002:
			ost << "Color Flow" << '\0';
			return ost.str();
		case 0x0003:
			ost << "PW Spectral Doppler" << '\0';
			return ost.str();
		case 0x0004:
			ost << "CW Spectral Doppler" << '\0';
			return ost.str();
		case 0x0005:
			ost << "Doppler Mean Trace" << '\0';
			return ost.str();
		case 0x0006:
			ost << "Doppler Mode Trace" << '\0';
			return ost.str();
		case 0x0007:
			ost << "Doppler Max Trace" << '\0';
			return ost.str();
		case 0x0008:
			ost << "Volume Trace" << '\0';
			return ost.str();
		case 0x0009:
			ost << "d(volume)/dt Trace" << '\0';
			return ost.str();
		case 0x000a:
			ost << "ECG Trace" << '\0';
			return ost.str();
		case 0x000b:
			ost << "Pulse Trace" << '\0';
			return ost.str();
		case 0x000c:
			ost << "Phonocardiogram Trace" << '\0';
			return ost.str();
		case 0x000d:
			ost << "Gray Bar" << '\0';
			return ost.str();
		case 0x000e:
			ost << "Color Bar" << '\0';
			return ost.str();
		case 0x000f:
			ost << "Integrated Backscatter" << '\0';
			return ost.str();
		case 0x0010:
			ost << "Area Trace" << '\0';
			return ost.str();
		case 0x0011:
			ost << "d(area)/dt" << '\0';
			return ost.str();
		case 0x0012:
			ost << "Other Physiological Amplitude vs. Time Input" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryBitMapDescription_RegionFlags(Uint16 value)
{
	Uint16 validmask=0;
	ostrstream ost;
	{
		validmask|=(1<<0);
		Uint16 bitvalue=value&(1<<0);
		ost << "Transparency(" << (bitvalue ? "Transparent" : "Opaque") << ") " << '\0';
	}
	{
		validmask|=(1<<1);
		Uint16 bitvalue=value&(1<<1);
		ost << "Scaling Protection(" << (bitvalue ? "Protected" : "Not Protected") << ") " << '\0';
	}
	{
		validmask|=(1<<2);
		Uint16 bitvalue=value&(1<<2);
		ost << "Doppler Scale Type(" << (bitvalue ? "Frequency" : "Velocity") << ") " << '\0';
	}
	if (value&~validmask)
		return 0;
	else
		return ost.str();
}

char *
DC3BinaryValueDescription_PixelComponentOrganization(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "Bit Aligned Positions" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Ranges" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_PixelComponentPhysicalUnits(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "None" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Percent" << '\0';
			return ost.str();
		case 0x0002:
			ost << "dB" << '\0';
			return ost.str();
		case 0x0003:
			ost << "cm" << '\0';
			return ost.str();
		case 0x0004:
			ost << "seconds" << '\0';
			return ost.str();
		case 0x0005:
			ost << "hertz(seconds-1)" << '\0';
			return ost.str();
		case 0x0006:
			ost << "dB/seconds" << '\0';
			return ost.str();
		case 0x0007:
			ost << "cm/sec" << '\0';
			return ost.str();
		case 0x0008:
			ost << "cm2" << '\0';
			return ost.str();
		case 0x0009:
			ost << "cm2/sec" << '\0';
			return ost.str();
		case 0x000a:
			ost << "cm3" << '\0';
			return ost.str();
		case 0x000b:
			ost << "cm3/sec" << '\0';
			return ost.str();
		case 0x000c:
			ost << "degrees" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_PixelComponentDataType(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "None" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Tissue" << '\0';
			return ost.str();
		case 0x0002:
			ost << "Spectral Doppler" << '\0';
			return ost.str();
		case 0x0003:
			ost << "Color Flow Velocity" << '\0';
			return ost.str();
		case 0x0004:
			ost << "Color Flow Variance" << '\0';
			return ost.str();
		case 0x0005:
			ost << "Color Flow Intensity" << '\0';
			return ost.str();
		case 0x0006:
			ost << "Gray Bar" << '\0';
			return ost.str();
		case 0x0007:
			ost << "Color Bar" << '\0';
			return ost.str();
		case 0x0008:
			ost << "Integrated Backscatter" << '\0';
			return ost.str();
		case 0x0009:
			ost << "Computed Border" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryBitMapDescription_USImageType4(Uint16 value)
{
	Uint16 validmask=0;
	ostrstream ost;
	{
		validmask|=(1<<0);
		Uint16 bitvalue=value&(1<<0);
		ost << "2D Imaging(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	{
		validmask|=(1<<1);
		Uint16 bitvalue=value&(1<<1);
		ost << "M-Mode(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	{
		validmask|=(1<<2);
		Uint16 bitvalue=value&(1<<2);
		ost << "CW Doppler(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	{
		validmask|=(1<<3);
		Uint16 bitvalue=value&(1<<3);
		ost << "PW Doppler(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	{
		validmask|=(1<<4);
		Uint16 bitvalue=value&(1<<4);
		ost << "Color Doppler(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	{
		validmask|=(1<<5);
		Uint16 bitvalue=value&(1<<5);
		ost << "Color M-Mode(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	{
		validmask|=(1<<6);
		Uint16 bitvalue=value&(1<<6);
		ost << "3D Rendering(" << (bitvalue ? "yes" : "no") << ") " << '\0';
	}
	if (value&~validmask)
		return 0;
	else
		return ost.str();
}

char *
DC3BinaryValueDescription_PixelRepresentation(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "unsigned" << '\0';
			return ost.str();
		case 0x0001:
			ost << "signed" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_PixelRepresentationUnsigned(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "unsigned" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_PlanarConfiguration(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "sequential planes" << '\0';
			return ost.str();
		case 0x0001:
			ost << "contiguous planes" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_SamplesPerPixelIsOne(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0001:
			ost << "0x0001" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_SamplesPerPixelIsThree(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0003:
			ost << "0x0003" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_PlanarConfigurationIsFrameInterleave(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0001:
			ost << "0x0001" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre16(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0010:
			ost << "0x0010" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre8(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0008:
			ost << "0x0008" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre12Or16(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x000c:
			ost << "0x000c" << '\0';
			return ost.str();
		case 0x0010:
			ost << "0x0010" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre11Or15(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x000b:
			ost << "0x000b" << '\0';
			return ost.str();
		case 0x000f:
			ost << "0x000f" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre8Or16(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0008:
			ost << "0x0008" << '\0';
			return ost.str();
		case 0x0010:
			ost << "0x0010" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre7Or15(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0007:
			ost << "0x0007" << '\0';
			return ost.str();
		case 0x000f:
			ost << "0x000f" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_BitsAre7(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0007:
			ost << "0x0007" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_CurveDataValueRepresentation(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "Unsigned Short(US)" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Signed Short(SS)" << '\0';
			return ost.str();
		case 0x0002:
			ost << "Floating Point Single(FL)" << '\0';
			return ost.str();
		case 0x0003:
			ost << "Floating Point Double(FD)" << '\0';
			return ost.str();
		case 0x0004:
			ost << "Signed Long(SL)" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_CurveDataDescriptor(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "Interval Spacing" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Values" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_AudioType(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "None" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Doppler Audio" << '\0';
			return ost.str();
		case 0x0002:
			ost << "Voice Audio" << '\0';
			return ost.str();
		case 0x0003:
			ost << "Phono Audio" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_AudioSampleFormat(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "16 Bit Signed LSB First Interleaved Per Channel" << '\0';
			return ost.str();
		case 0x0001:
			ost << "8 bit Signed Interleaved Per Channel" << '\0';
			return ost.str();
		default:
			return 0;
	}
}

char *
DC3BinaryValueDescription_NumberOfChannels(Uint16 value)
{
	ostrstream ost;
	switch (value) {
		case 0x0000:
			ost << "Mono" << '\0';
			return ost.str();
		case 0x0001:
			ost << "Stereo" << '\0';
			return ost.str();
		default:
			return 0;
	}
}






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */