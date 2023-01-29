/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*
**	VMC-2 Device Driver interface.
**	Copyright(C) 1992, 1994, Precision Solutions, Inc.
**	all rights reserved.
**
**	The use of the following, in any form, subjects you 
**	to a user licensing structure as outlined in the License 
**	Agreement.
**	
**	Version 1.8, September 1994
**
**	See User documentation for variable ranges.
*/

#ifndef VMC2_DINCLUDE

#define		VMC2_DINCLUDE
#define 	VMC2_READ_VALUE		0x7fff
#define		VMC2_DRIVER_NAME	"/psi/VMC2"

/*
**	Structure used as part of the VMC2_MSG
*/
typedef struct
{
	short	nY,
			nX,
			nHeight,
			nWidth;
} VIDEO_AREA;

#ifndef VMC2_INCLUDE
typedef struct 
{
	short 	iY;
	short 	iU;
	short 	iV;
} YUV;
#endif

typedef struct
{
	unsigned char	nRed,
					nGreen,
					nBlue;
} RGB;

/*
**	The following is the message sent to the VMC-2
**	device driver. 
*/
typedef struct 
{
	char		nResultCode;	/* 0 - Error, 1 - Okay		*/
	char		nMsgType;		/* See MsgType enum			*/
	char		nFormat;		/* See format enum			*/
	short		nDataValue;		/* A variable for control	*/
	VIDEO_AREA	stArea;			/* Box Coords for new Pos.	*/

	char		szPath[144];	/* Full path to File		*/
	char		nYQuality;		/* Lumin Quality for JPEG	*/
	char		nUVQuality;		/* Chrom Quality for JPEG	*/
	YUV			stYuv;			/* Contains color of Pixel	*/
	RGB			stRgb;			/* Contains RGB color		*/
	long		lCompareRatio;	/* Contains compare info.	*/
	char		_spare[58];
} VMC2_MSG;

typedef enum
{
	/*
	**	Message types for the VMC-2 Driver
	**
	**	_MTCaptureFrame, captures a Video frame to the harddisk.
	**	A full frame requires approx. 2 megabytes. 
	**	_MTLoadFrame, load a previously save Frame back into
	**	the VMC-2 VRAMS.
	**
	**	Use stArea to program area or row to capture.
	*/
	_MTCaptureFrame, _MTLoadFrame, _MTCaptureRow, 
	/* 	
	**	For _MTClearFrame: Set stYuv members to the color desired
	**	i.e., (nYComp = 0xFF, nUComp = 0xFF, nVComp = 0xFF) = WHITE
	**	i.e., (nYComp = 0x00, nUComp = 0x00, nVComp = 0x00) = BLACK
	**
	**	For _MTPixel: stYuv to contain the YUV components on
	**	read and write operations.  stArea to contain the X,Y location.
	**	Set nDataValue = VMC2_READ_VALUE to read the Pixel.
	**/
	_MTClearFrame, _MTPixel, _MTYuvToRgb, _MTRgbtoYuv,
	/*
	**	Use nDataValue to program the following:
	**	Set nDataValue = VMC2_READ_VALUE to
	**	read the current value from the driver.
	*/
	_MTPanOff, _MTShiftXOff, _MTShiftYOff, _MTPalSkew,
	_MTChannel, _MTHue, _MTSaturation, _MTContrast,
	_MTBrightness, _MTRedGain, _MTGreenGain, _MTBlueGain, _MTColorKey,
	/*
	**	Use stArea structure to program the following:
	**	Set nDataValue = VMC2_READ_VALUE to read current
	**	position of video window.  The position will be 
	**	returned in stArea. (PIXELS)
	*/
	_MTMoveVideo,
	/*
	**	Audio, see VMC2_AUDIO_MODE enum for _MTAudioMode
	**	Use nDataValue for all except _MTAudioMode
	**	_MTMute - nDataValue = 0 Audio On, 1 Mute
	*/
	_MTMute, _MTVolume, _MTBalance, _MTBass, _MTTreble, 
	_MTAudioMode,
	/*
	**	System Level	
	*/
	_MTVMC2On, _MTVMC2Off, _MTAudioOn, _MTAudioOff, _MTLoadConfig,
	_MTSaveConfig, _MTLiveVideo, _MTFreezeVideo, _MTHideVideo, _MTShowVideo,
	_MTQuit
} MSG_TYPES;

typedef enum
{
	/*
	**	Insert these into the nFormat member of VMC2_MSG structure
	**	Note: as of Sept 1994, YUV and JPEG are the only formats supported.
	**   ".yuv"  ".bmp"  ".tif"   ".pcx"  ".jpg"
	*/
		_FTYUV, _FTBMP, _FTTIFF, _FTPCX, _FTJPEG, _FTBMP24
} FORMAT_TYPES;

typedef enum
{
	_AMMono, _AMStereo, _AMPseudoStereo, _AMSpatialStereo
} VMC2_AUDIO_MODE;

#endif




/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
