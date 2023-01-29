/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




///    Copyright (c) 1994,1995, 1996
///    NeuroConcepts Inc.
///    All Rights Reserved
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <process.h>
#include <errno.h>
#include <fcntl.h>
#include <share.h>
#include <sys/types.h>
#include <sys/stat.h>

#include  "vplay2.h"
#include  "pcmotion.h"  
                                        
#define PCMOTIONDOS "pcmotion.dos" 
#define LOGFILE_BASE "decrdos"

#define ERR_INIT_DONE       1
#define ERR_VOPEN_DONE      (1<<1)
#define ERR_HALLOC_DONE     (1<<2)
#define ERR_FOPEN_DONE      (1<<3) 
#define ERR_COMFIFO_DONE	(1<<4)   

// Defines and structures that absolutely must agree with the QNX side.
#define FIFO_MAXSIZE			5120	// PIPE_BUF on qnx side
enum DECCNTL {DECCMD_INITPARAM=0, DECCMD_PLAY, DECCMD_PAUSE, 
	DECCMD_STEPFRAME, DECCMD_STEPGOP, DECCMD_STOP, 
	DECCMD_NOP, DECCMD_GETFRAME};
enum VIDEOMODE {NTSC_VIDEO, PAL_VIDEO};
typedef struct deccmd {
	short	cmd;
	union {
		struct {
			short playSpeed;
			short videoMode;
		} init;
		short	playSpeed;	// (-inf, -8] slowest; [-8, -2] slow mo; [-1, 1] normal; [2, inf) fast 
		short	numStep;	// number of frames of GOP to step
	};
} DECCMD;
enum DECREPTYPE {DECREP_FRAME};
typedef struct decrep {
	short	reptype;
	union {
		long	framenum;
		char	pad[1024-2]; // see qnx side comment
	};
} DECREP;

char combuf[FIFO_MAXSIZE];

int OptibasePlaySpeed(int decPlaySpeed); 
int OptibaseVideomode(int decVideomode); 
int SetAudioVolume(int audvol);
int get_param(char *, PARAM *);
char* m2_error(int);
void CleanUp(int);
void EndMovie(int *decfifoFd, int *comfifoFd);
static void log_msg(const char *filebase, const char *text);

unsigned int    bufSize=READBUFSZ;
unsigned        fifofd, fifoComfd;
uchar  huge     *UWorkBuff;

char huge *mp_GetVideoBuffer(unsigned int *lpnSize,unsigned int BuffSize);
char huge *mp_GetSystemBuffer(unsigned int *lpnSize,unsigned int BuffSize);

main(int argc, char *argv[])
{
    //unsigned	fifoIdlefd, idleBytesRead;  
    int         i, retval, playSpeed, forceEnd, videoMode;
    char        video_name[80], errbuf[200];
    int 		cleanUpFlag, initCleanUpFlag;
    unsigned	comBytesRead, repBytesWritten;
    DECCMD		oldCmd, newCmd; 
    struct _stat	statbuf;
    static DECREP		decReply;  // just in case it's big

    if(argc < 2)
    {
		fprintf(stderr, "USAGE: %s filename\n", argv[0]);
		exit(-1);
    }
    strcpy(video_name, argv[1]);
    cleanUpFlag = 0;
     
    sprintf(errbuf, "START DECODER %s", video_name);
    log_msg(LOGFILE_BASE, errbuf);

    // Set the param structure
    p.num_buf = 6;  // default is 6
    if(get_param(PCMOTIONDOS, &p))
    {
	    sprintf(errbuf, "Can't find pcmotion.dos");
	    log_msg(LOGFILE_BASE, errbuf);
		CleanUp(cleanUpFlag);
		exit(-1);
    }
	sprintf(errbuf, "out_mode %d init delay %4.2f aud %d genlock %d addr %x path %s num_buf %d",
		p.video_out_mode, p.audio_initial_delay, p.is_audio, p.genlock, p.board_adr,
		p.opt_path, p.num_buf);
    log_msg(LOGFILE_BASE, errbuf);

    // Allocate buffer for the callback.
    if ((UWorkBuff=((uchar*)halloc(bufSize, sizeof(char))))==NULL)
    {
		sprintf(errbuf, "halloc(): %s", m2_error(ALLOC_ERR4));
   		log_msg(LOGFILE_BASE, errbuf);
		CleanUp(cleanUpFlag);
		exit(-1);
    }
    cleanUpFlag |= ERR_HALLOC_DONE;

    initCleanUpFlag=cleanUpFlag;  

    while(1)
    {
		cleanUpFlag = initCleanUpFlag; 
		
		// Do this first so the screen gets cleared before blocking on the fifo.
		cleanUpFlag |= ERR_INIT_DONE;
		if((retval=m2_init()) != 0)
		{
		    sprintf(errbuf, "m2_init():  %s", m2_error(retval));
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}
	
		// Open data file.  For a fifo this blocks until another process
		// opens the fifo to write.   Using _open cause m2_random_access() to fail!  
		if(_dos_open(video_name, _O_RDONLY | _SH_DENYNO, &fifofd) != 0)
		{
		    sprintf(errbuf, "open failed %s  error %d", video_name, errno);
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}
		cleanUpFlag |= ERR_FOPEN_DONE;
		
		// Open command fifo.  Block until another process opens to write.  This is opened
		// second because mpeg_decadmin first opens the data fifo and then spawns mpeg_deccom
		// which opens the command fifo. 
 		if(_dos_open("e:comfifo", _O_RDONLY | _SH_DENYNO, &fifoComfd) != 0)
		{
		    sprintf(errbuf, "failed to open comfifo %d", errno);
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}   
		cleanUpFlag |= ERR_COMFIFO_DONE;

		
		// Read the initialization block.
		comBytesRead = 1234;
		oldCmd.cmd = -1;
		if((retval=_dos_read(fifoComfd, &oldCmd, sizeof(DECCMD), &comBytesRead)) != 0 || 
		 	comBytesRead != sizeof(DECCMD) || oldCmd.cmd != DECCMD_INITPARAM)
		{
			sprintf(errbuf, "Can't read initialization block from com fifo %d %d %d", 
				retval, comBytesRead, oldCmd.cmd);
			log_msg(LOGFILE_BASE, errbuf);
		 	exit(-1);
		} 
		sprintf(errbuf, "Received init command: videomode %d speed %d", 
			oldCmd.init.videoMode, oldCmd.init.playSpeed);
		log_msg(LOGFILE_BASE, errbuf);
		playSpeed = OptibasePlaySpeed(oldCmd.init.playSpeed);
		videoMode = OptibaseVideomode(oldCmd.init.videoMode);
		if(videoMode == -1)
		{
			sprintf(errbuf, "Bad video mode %d", oldCmd.init.videoMode);
			log_msg(LOGFILE_BASE, errbuf);
			exit(-1);
		}
		else if(videoMode != p.video_out_mode)
		{
			// Need to restart decoder with the correct video mode.
			sprintf(errbuf, "Changing video mode from %d to %d", p.video_out_mode, videoMode);
			log_msg(LOGFILE_BASE, errbuf);
			m2_end();
			p.video_out_mode = videoMode;
			m2_init();
		}      

		// Set video file format.	Since reading from a FIFO the format
		// must be assumed.
		if((retval=UserParam(SYSTEM_FILE)) != 0)	 // .mpg files
		{
		    sprintf(errbuf, "UserParam:  %s", m2_error(retval));
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}
		   	
		// Open decoder without input file name.
		// If a callback function will be used to fetch
		// the MPEG bitstream, the m2_Open parameters will
		// be disregarded.
		cleanUpFlag |= ERR_VOPEN_DONE;
		if((retval=m2_open (NULL, NULL, NULL)) != 0){
		    sprintf(errbuf, "m2_open():  %s",m2_error(retval)); 
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}
	
		// Set in back2back mode.
		// If ACURATE_MODE=on and adjacent pts has a gap, m2_random_access() is automatically
		// called to find the next gop.  In addition, if a system header is detected, an
		// automatic m2_random_access() is done to find the next GOP.
		// Without ACCURATE_MODE the pts is only corrected. 
		if((retval=m2_openMPS_fast(NULL)) != 0){
		    sprintf(errbuf, "m2_openMPS_fast():  %s",m2_error(retval));
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		} 
		
		// Set audio volume. 
		if((retval=SetAudioVolume(59)) == -1)
		{
		    sprintf(errbuf, "set audio failed %s", m2_error(retval)); 
		    log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}

		// Seek to begining of file.  Must do this before playing.
		if((retval=m2_random_access(CUR_FRAME)) != 0)
		{                                
			sprintf(errbuf, "m2_random_access(): %s", m2_error(retval));
			log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}

		// Play file.
		if((retval=m2_play(CUR_FRAME, LAST_FRAME, playSpeed)) != 0)
		{   
			sprintf(errbuf, "m2_play() failed %d  %s", playSpeed, m2_error(retval)); 
			log_msg(LOGFILE_BASE, errbuf);
		    CleanUp(cleanUpFlag);
		    exit(-1);
		}
        
        forceEnd=0;  // stop command sets true to force the end of the movie
        
		while(1)
		{
	    	// If more than one command, get the rest next time through the loop.  When other
	    	// side closes, _dos_read unblocks, returns a 0 and comBytesRead is zero.  The command
	    	// and data fifo can be closed in any order.
	    	if(_dos_read(fifoComfd, &newCmd, sizeof(DECCMD), &comBytesRead) != 0 ||
	    		comBytesRead != sizeof(DECCMD))
	    	{
	     		sprintf(errbuf, "Read failed on comfifo %d", comBytesRead);
	     		log_msg(LOGFILE_BASE, errbuf);
	     		// The fifo will be drained and then
	     		// _dos_read will block until the other side closes the fifo.
	     		while(1)
	     		{  
		     		if(_dos_read(fifofd, UWorkBuff, 1024, &comBytesRead) != 0 ||		     			
		    			comBytesRead != 1024)
		    		{
			     		sprintf(errbuf, "Closed data fifo %d", comBytesRead);
			     		log_msg(LOGFILE_BASE, errbuf);
		    			break;
		    		}	
	     		}
	     		forceEnd=1;
			}  
				
			// Interperate the command
			switch(newCmd.cmd){
			case DECCMD_PLAY:
				if(oldCmd.cmd != DECCMD_PLAY || oldCmd.playSpeed != newCmd.playSpeed)
				{  
					playSpeed = OptibasePlaySpeed(newCmd.playSpeed);

					if((retval=m2_pause()) != 0)
					{
						sprintf(errbuf, "m2_pause() failed for speed change %d", oldCmd.cmd);
						log_msg(LOGFILE_BASE, errbuf);
	    				sprintf(errbuf,"%s",m2_error(retval));  
	    				log_msg(LOGFILE_BASE, errbuf);
	    				forceEnd=1;
					}
   
                    if((retval=m2_play(CUR_FRAME, LAST_FRAME, playSpeed)) != 0)
                    { 
               			sprintf(errbuf, "m2_play() failed %d %d %d", oldCmd.cmd, oldCmd.playSpeed, newCmd.playSpeed);
               			log_msg(LOGFILE_BASE, errbuf);
	    				sprintf(errbuf,"%s",m2_error(retval));  
	    				log_msg(LOGFILE_BASE, errbuf);
	    				forceEnd=1;
                    }
					memcpy(&oldCmd, &newCmd, sizeof(DECCMD));
				}
				break;
			case DECCMD_PAUSE:    
				if(oldCmd.cmd != DECCMD_PAUSE)
				{
					if((retval=m2_pause()) != 0)
					{
						sprintf(errbuf, "m2_pause() failed %d", oldCmd.cmd); 
						log_msg(LOGFILE_BASE, errbuf);
	    				sprintf(errbuf,"%s",m2_error(retval));  
	    				log_msg(LOGFILE_BASE, errbuf);
	    				forceEnd=1;
					}
                	memcpy(&oldCmd, &newCmd, sizeof(DECCMD)); 
                } 
                break;
            case DECCMD_STEPFRAME:
                if(oldCmd.cmd != DECCMD_PAUSE && 
                   oldCmd.cmd != DECCMD_STEPFRAME && 
                   oldCmd.cmd != DECCMD_STEPGOP)
                { 
                	// Must be paused before single stepping.
					if((retval=m2_pause()) != 0)
					{
						sprintf(errbuf, "m2_pause() for step frame command failed %d", oldCmd.cmd);
						log_msg(LOGFILE_BASE, errbuf);
	    				sprintf(errbuf,"%s",m2_error(retval));  
	    				log_msg(LOGFILE_BASE, errbuf);
	    				forceEnd=1;
					}
				}
					
				for(i=0; i < newCmd.numStep; i++)
				{
					if((retval=m2_FrameForward()) != 0)
					{
						sprintf(errbuf, "m2_FrameForward() failed %d", oldCmd.cmd);  
						log_msg(LOGFILE_BASE, errbuf);
	    				sprintf(errbuf,"%s",m2_error(retval));   
	    				log_msg(LOGFILE_BASE, errbuf);
	    				forceEnd=1;
					} 
				} 
						
						
				memcpy(&oldCmd, &newCmd, sizeof(DECCMD));
				break;  
			case DECCMD_STOP:
				forceEnd=1;
				break;
			case DECCMD_NOP:
				// Don't copy newCmd to oldCmd.
				break;
			default:
				sprintf(errbuf, "Unrecognized command %d %ld %d %d", 
					newCmd.cmd, statbuf.st_size, comBytesRead, sizeof(DECCMD));
				log_msg(LOGFILE_BASE, errbuf);
				forceEnd=1;
				break;
			} 
			//} // end if stat

            
            // Check if stop done or SDK detects the end of movie.
    		if(forceEnd || m2_GetStatus())
    		{
    			if (p.RunTimeError)
					p.RunTimeError = 0;

				EndMovie(&fifofd, &fifoComfd);
				break; // end of movie
			}
    		else
    		{  
				m2_continue();
			}
		}
    }

     CleanUp(cleanUpFlag);
     return(0);
}

// Called to end the movie
void
EndMovie(int *decfifoFd, int *comfifoFd)
{
	_dos_close(fifofd);
	fifofd = 0;
	_dos_close(fifoComfd);
	fifoComfd = 0;
				
	m2_close();
	m2_end();
}  

// Called to convert decPlaySpeed into a number recognized by the Optibase API. 
// decPlaySpeed > 1 for search mode.  {-1, 0, 1} normal speed.  [-8, -2] slow motion.
int
OptibasePlaySpeed(int decPlaySpeed) 
{ 
	int playSpeed;
	
	if(decPlaySpeed > 1)
		playSpeed=FAST_SPEED;  
	else if(decPlaySpeed >= -1)
		playSpeed=1;		 	
	else if(decPlaySpeed >= -8)
		playSpeed = -decPlaySpeed;  // slow motion
	else  // < -8
		playSpeed = 8;  // slowest possible 
		
	return(playSpeed);
}  

// Convert decVideomode into a number recognized by the Optibase API.
int
OptibaseVideomode(int decVideomode)
{  
	int videomode;
	
	if(decVideomode == NTSC_VIDEO)
		videomode=NTSC_358;
	else if(decVideomode == PAL_VIDEO)
		videomode = PAL;
	else 
		videomode=-1;
		
	return(videomode);
} 

// Called to set the audio volume.
int
SetAudioVolume(int audvol)
{    
    AUDIO		audioParam;
	int			retval=0;
	
	if((retval=m2_get_audio(&audioParam)) == 0)
	{
		audioParam.volume = audvol;  // range is [0,63].  55 is the default and < 43 is quite. 
		retval=m2_set_audio(&audioParam);
	}
	
	return(retval);
}

//********************************************************************
//
//  CleanUp()
//
//  Description:
//      Used to recover from errors in main() before exiting.
//
//  Returns:
//
//********************************************************************
void
CleanUp(int cleanup)
{
    if(cleanup & ERR_VOPEN_DONE)
		m2_close();
    if(cleanup & ERR_INIT_DONE)
		m2_end();
    if(cleanup & ERR_HALLOC_DONE)
		_hfree(UWorkBuff);
    if(cleanup & ERR_FOPEN_DONE)
    {
		_dos_close(fifofd);
		fifofd = 0;
	}
	if(cleanup & ERR_COMFIFO_DONE)
	{
		_dos_close(fifoComfd); 
		fifoComfd = 0;
	}
}

/// M2_ERROR *******************************************************************
char* m2_error(int error_num)
{
   static char error[128];

   switch (error_num) {
   case   NO_ERR:   sprintf(error,"No error");
	     break;
   case FILE_ERR1:  sprintf(error,"Unable to open audio file");
	     break;
   case FILE_ERR2:  sprintf(error,"Unable to open video file");
	     break;
   case FILE_ERR3:  sprintf(error,"Unable to open reference file");
	     break;
   case FILE_ERR4:  sprintf(error,"Video file empty");
	     break;
   case FILE_ERR5:  sprintf(error,"Video header not found");
	     break;
   case FILE_ERR11: sprintf(error,"Seek error in audio file");
	     break;
   case FILE_ERR12: sprintf(error,"Seek error in video file");
	     break;
   case FILE_ERR13: sprintf(error,"Unable to read reference file");
	     break;
   case FILE_ERR14:  sprintf(error,"Video file format error");
	     break;
   case FILE_ERR15:  sprintf(error,"Unable to open system file");
	     break;
   case FILE_ERR16:  sprintf(error,"System file error");
	     break;
   case ALLOC_ERR1: sprintf(error,"Unable to allocate audio buffer");
	     break;
   case ALLOC_ERR2: sprintf(error,"Unable to allocate video buffer");
	     break;
   case ALLOC_ERR3: sprintf(error,"Unable to allocate reference buffer");
	     break;
   case ALLOC_ERR4: sprintf(error,"Unable to allocate system buffer");
	     break;
   case WAV_ERR:    sprintf(error,"WAV file error");
	     break;
   case AUD_ERR:    sprintf(error,"Invalid audio parameter");
	     break;
   case HW_ERR:     sprintf(error,"Hardware failure");
	     break;
   case HW_ERR1:    sprintf(error,"Board initialization failure");
	     break;
   case HW_ERR2:    sprintf(error,"Unable to load one of the *.opt files");
	     break;
   case HW_ERR3:    sprintf(error,"Parameter initialization failure");
	     break;
   case USER_DATA_ERR: sprintf(error,"User data error");
	     break;
   case TEMP_FILE_ERR:  sprintf(error,"Unable to open temporary file");
	     break;
   case XMS_ERROR: sprintf(error,"XMS error");
	     break;
   case PARAM_OUT_OF_RANGE:  sprintf(error,"parameter out of range");
	     break;
   default:         sprintf(error,"Undefined error");
   }

   return(error);
}

/**********************************************************************
  Function   :  mp_GetSystemBuffer
  Description:  Callback function to return MPEG(System) bitstream data  
  Parameters :  lpnSize -  Pointer to # of bytes returned by the 
		callback function
		BuffSize -  Optimal number of bytes required from user (<= READBUFSZ)
**********************************************************************/
char huge *mp_GetSystemBuffer(uint *lpnSize, uint BuffSize)
{
    if(_dos_read(fifofd, UWorkBuff, BuffSize, lpnSize) !=0)
    {
    	*lpnSize=0;		// The SDK looks at this value first.
		return(NULL);
	}
		
    return((*lpnSize == 0) ? NULL : UWorkBuff); 
}

/**********************************************************************
  Function   :  mp_GetVideoBuffer
  Description:  Callback function to return MPEG bitstream data  
  Parameters :  lpnSize -  Pointer to # of bytes returned by the 
		callback function
		BuffSize -  Optimal number of bytes required from user
  Notes      :    
**********************************************************************/
char huge *mp_GetVideoBuffer(unsigned int *lpnSize,unsigned int BuffSize)
{
    fprintf(stderr, "NOT SUPPOSE TO BE IN HERE!!!!!!!\n");
    log_msg(LOGFILE_BASE, "NOT SUPPOSE TO BE IN HERE !!!!!");
    return(NULL);
}

// This call mimics the same call in Uniquant. 
#define LOGFILE_MAX 100
static void
log_msg(const char *filebase, const char *text)
{  
	char		filename[LOGFILE_MAX], backupfile[LOGFILE_MAX], timestr[20], cr[]="\n"; 
	time_t		long_time;
	struct tm	*newtime;
	int			fd;
	unsigned	byteswritten;
	
	// Create filename.
	strcpy(filename, "./config/");
	strcat(filename, filebase);
	
	// If filename doesn't exist put log in local directory. 
	strncpy(backupfile, filebase, LOGFILE_MAX);
	backupfile[LOGFILE_MAX-1]='\0';
	
	// Generate date.  Only use %d instead of %y%m%d because of dos filename limitation.
    time( &long_time );                     /* Get time as long integer. */
    newtime = localtime( &long_time );      /* Convert to local time. */ 
    strftime(timestr, 20, ".%d", newtime);
    
    // Append date to filename.
    strcat(filename, timestr);
    strcat(backupfile, timestr);
    
    // Generate current time.  Used dos_open() because stream calls can't
    // be mixed with other dos_opens() in this function.  If dos_open is changed
    // to open() etc. everywhere, m2_random_access() fails.
    strftime(timestr, 20, "%I:%M:%S%p:", newtime);
    
    // Try to open a file.
    if(_dos_creatnew(filename, _A_NORMAL, &fd) != 0)  // first choice
    {
		if(_dos_open(filename, _O_RDWR | _SH_DENYNO, &fd) != 0) // maybe first choice already exists
		{
    		if(_dos_creatnew(backupfile, _A_NORMAL, &fd) != 0)  // second choice
    		{
		    	if(_dos_open(backupfile, _O_RDWR | _SH_DENYNO, &fd) != 0)  // maybe second choice already exists
		    	{
					fprintf(stderr, "%s  %s\n", timestr, text);  // nothing opened 
					return;
				}
				else
					_dos_seek(fd, 0L, 2);  // append if file already exists	
			}
		}
		else
			_dos_seek(fd, 0L, 2);  // append if file already exists			
	}

	 
	if(_dos_write(fd, timestr, strlen(timestr), &byteswritten) != 0 || byteswritten != strlen(timestr)) 
	{
		fprintf(stderr, "%s  %s\n", timestr, text);  
	}
	else if(_dos_write(fd, "  ", 2, &byteswritten) != 0 || byteswritten != 2) 
	{
		fprintf(stderr, "%s  %s\n", timestr, text);  
	}
	else if(_dos_write(fd, text, strlen(text), &byteswritten) != 0 || byteswritten != strlen(text)) 
	{
		fprintf(stderr, "%s  %s\n", timestr, text);  
	}
	else if(_dos_write(fd, cr, strlen(cr), &byteswritten) != 0 || byteswritten != strlen(cr)) 
	{
		fprintf(stderr, "%s  %s\n", timestr, text);  
	}
	_dos_close(fd);
	
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
