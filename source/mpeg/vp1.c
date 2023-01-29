/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




Here is a digest of the Optibase "rtd" calls we make:

  rtdSetParams(SYS_PARAM,				/* set PTS mode 					*/
    USE_PTS, 0);

  sr = rtdInitSystem(0,					/* background mode, 0=polling mode 	*/
    NULL, NULL);

  sr = rtdInitBoard(priv->board,		/* p 2-12 							*/
    NON_DMA_MODE,						/* non-DMA mode 					*/
    NTSC_M,								/* or PAL_M or NTSC_443				*/
    SYSTEM_FILE,						/* or VIDEO or AUDIO or TRANSPORT 	*/
    STAND_ALONE);						/* or genlock 						*/

  rtdSetParams(priv->board,
    SEAMLESS_BACK2BACK, 0);

  sr = rtdPlay(priv->board,				/* start play 						*/
    FIRST_FRAME,						/* ignore hardware buffer contents	*/
    LAST_FRAME,
    NORMAL_SPEED);


  in a loop:
  {
    read data from somewhere

    do {
      sr = rtdPlayBuf(priv->board,		/* give data to videoplex fifo		*/
        buffer, got);
	  if (sr == 3)
	    DT_C(dtos_delay,(ctl, 10));		/* sit around awhile 				*/
    } while (sr == 3);					/* didn't all fit? 					*/
  }


  rtdSetParams(priv->board,				/* go into fast forward 			*/
    PLAY_SPEED, FAST_FWD);

  in a loop:
    stuff in some more data like above


  rtdSetParams(priv->board,				/* go back to normal speed 			*/
    PLAY_SPEED, NORMAL_SPEED);

  in a loop:
    stuff in some more data like above - NOW AUDIO IS OUT OF SYNC!


  rtdFreeBoard(priv->board);

  rtdFreeSystem();






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
