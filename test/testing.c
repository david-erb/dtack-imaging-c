/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
| TITLE
| Test Programs
|
|!Overview
| The General Purpose Library is exercised and verified
| by a set of test programs.
| 
| The collective purpose of the set of test programs is to
| establish a baseline confidence level in the software implementation.
| In other words,
| if all the test programs execute successfully on your computer, 
| then you can be sure that the software is operating at least
| as correctly as it does in the Dtack Imaging Software
| Quality Assurance Lab.
| 
| The tests are arbitrarily assigned to categories as follows:
|
| #Low Level Tests#
| :dttlow str:			string manipulation
| :dttlow heap:			heap monitoring
| :dttlow param:		command-line parameters
| :dttlow list:			small linked lists
| :dttlow os:			miscellaneous operating system interface functions
| :dttmem:				big memory
| :dtt1d:				one-dimensional data processing
| :dttlow stack:		sequential auto-growing big memory
| :dttlow bpe:			lossless compression
| :dttlow ipc:			interprocess communication
|
| #Alphanumeric Display Tests#
| :dttlow key:			keyboard interface
| :dttlow note~~~~~~:	message output queue
| :dttlow dia:			full-screen text entry dialogs
| :dttlow alpha:		full-screen scrolling text display
| 
| #Image Statistics#
| :dtthigh2 stat~~~~:	basic assistance
| :dtthigh1 hist:		histograms
| 
| #Image Miscellaneous#
| :dtthigh1 plot~~~~:	drawing lines and text in images
|
| #Image Point Processing#
| :dttcast~~~~~~~~~~:	image data type conversion
| :dtthigh1 log     :	logarithmic point processing
| 
| #Image Neighborhood Processing#
| :dtthigh2 cooc~~~~:	co-occurrence matrices
| :dtthigh2 joint:		joint probability
| :dtthigh2 resample:	image enlarging and shrinking
| :dttconv:				two-dimensional convolution
| 
| #Image Structure#
| :dtthigh1 run~~~~~:	run extraction
| :dttadj:				connected components
| 
| #Image External Representation#
| :dtthigh1 pgm~~~~:	portable graymap format image output
| :dttxchg:				image output formats
| 
| #Image Display Tests#
| :dttscr~~~~~~~~~~:	graphical display of images
| :dttwin:				client/server windowing
| :dttview:				image viewer interaction
|
| Each test is independent of all the other tests.
| They do not need to be executed in any particular order.
| 
| The test programs are designed to run automatically.
| They do not need user intervention to finish properly.
| (The exception to this rule is :dttlow key: which tests
| the keyboard interface.)
| Several tests, however, can be optionally told to wait for and
| to check user input.
| 
| The test programs check their own results and
| only emit messages on the console when a problem is detected.
| (The exceptions to this rule are
| :dttlow dia: and :dttlow alpha: which test the full-screen
| alphanumeric capabilities.)
| All tests have options which cause them to produce diagnostic messages
| when desired.
|
| In particular, all tests accept the :-D assert: parameter.
| The presence of this parameter causes a test program to print
| a message for each assertion which is successful.
|
| All tests also accept the :-help: and :-HELP: parameters,
| printing out short and long help messages respectively.
| 
|.Test Output
| When not given any options, each program is silent when successful.
| Error messages are printed to standard out and/or standard error,
| depending on the operating system.
| Error messages may also be directed to a file by using the
| :-errto: and/or :-dbgto: parameters 
| and giving a filename immediately following.
| Using these options ensures that standard out and standard error
| are not used for messages.
| 
| There are three different types of failure messages:
| 1.	operating system messages upon program abnormal termination
| 1.	errors
| 2.	failed assertions
| If the operating system gives an abnormal termination message,
| such as "Segmentation Violation" or "General Protection Fault",
| you know something is seriously wrong with the implementation.
| Usually, however, the programs will detect an error condition
| and exit gracefully after printing one or more error messages.
| The presence of an error message indicates that the test program
| found something it didn't expect, so it decided to stop.
| A failed assertion, on the other hand,
| indicates that the test program simply got the wrong answer.
| The test programs do not stop when they find failed assertions.
| 
|.Running the Tests
| Each test is either a standalone program or is grouped
| together with other tests in one program.
| The programs which contain multiple tests are:
| :dttlow:		contains :str:, :param:, :note:, :list:, :os:, :stack:, 
|				:bpe:, :ipc:, :dia:, :alpha:
| :dtthigh1:	contains :hist:, :plot:, :log:, :pgm:, :run:
| :dtthigh2:	contains :stat:, :cooc:, :joint:, :resample:
| A test is accessed by giving its name as the first argument
| of its containing program, for example:
| 
|:dttlow ipc:
|:dtthigh1 plot -D assert:
|
| The complete set of available tests is executed by the Bourne shell
| script in :DTACK/sh/dttest.sh:
| and the DOS bash file in :DTACK/bat/dttest.bat:.
| This script and batch file collect all output into a single file
| called :test.out:.
| This output file can be scanned for error messages and assertion failures.
| Exactly this is done by the installation script :dtinst.sh:
| and batch file :dtinst.bat:.
| 
|!Image Display Tests
| This category consists of the following tests:
| :dttscr:	Graphical Display of Images.
|			Tests display of images in windows.
| :dttwin:	Client/Server Windowing. 
|			Tests communications between client and server.
| :dttview:	Image Viewer Interaction.
|			Tests routines used in :dtview:, the Dtack Viewer program.
| 
| These programs overlap somewhat.
|_Aside The tests overlap coverage.
| 
| All these tests require the :dtscr: Function Family.
|_aside The test programs use :-screen: parameter.
| All of them examine the :-screen: parameter on the command line.
| When this parameter is not given, the environment variable
| :DT_SCREEN: is examined.
| If the environment variable is not present,
| the default value for this parameter is defined at compile time.
| It will generally specify the native windowing system, such as
| X Window or Microsoft Windows.
| When no windowing system is available,
| one is simulated by using :-screen dummy:.
| All three tests work with this value.
| 
|dtinclude(screen1.dx)
| 
| :dttscr: displays images, but does not expect either user
|_index dttscr, :dttscr:, Test Program
|_aside This is the simplest test.
| interaction or client interaction.
| It is the simplest program.
| You should run it to make sure that images can be displayed on your computer.
| 
| :dttwin: also displays images,
|_index dttwin, :dttwin:, Test Program
|_Aside This test covers client/server.
| but also acts as a server which expects several predefined messages
| to be sent by a client.
| The program has a client part which sends the required messages.
| :dttwin: does not handle user interaction.
| When :-screen: indicates a real windowing system (not :dummy:),
| you should expect to see three test windows pop up on the
| upper left part of your screen.
| They disappear quickly.
| They will appear once for each different type of client/server
| communications, including :dsk:.
| 
| Finally, :dttview: complements :dttwin:.
|_index dttview, :dttview:, Test Program
|_Aside This test covers user interaction.
| It also runs as a server,
| but instead of expecting client messages,
| user input is simulated.
| When :-screen: indicates a real windowing system (not :dummy:),
| you should expect to see the following sequence displayed on your screen:
| 1.	three test images appear on the upper left, then disappear
| 2.	three test images appear again, the middle one disappears,
|		the other two dance around, then disappear
| 3.	15 regularly-spaced images pop up, then disappear
| Each step is followed by an "Info" window with a message about saving windows.
| 
| :dttwin: and :dttview: together test all the routines used by
|_Aside If these tests work, :dtview: will work.
| :dtview:, the Dtack Viewer program.
| This program is your principal tool for image display
| during development,
| unless you already have a favorite viewer program
| which can read files produced by your programs.
| The Dtack Viewer program is very rudimentary,
| but has one important advantage if run in X Windows or Microsoft Windows:
| your client programs can send images to it.
| This means that the images stay on the screen
| even when the client programs terminate.
| 
| END
 *..........................................................................*/






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
