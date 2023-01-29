Dtack Mjpeg Rev 1-4 readme.txt 14 July 1997


INTRODUCTION
------------
This suite of programs implements a set of ActiveMovie filter
components and Java applet classes which work together to deliver
live streaming video in a World Wide Web environment.

The ActiveMovie filter graph is started as a CGI program.
The MJ package includes a program to act as a mini web server.
A watchdog program provides reliable unattended operation.


INSTALLATION
------------
1----DOWNLOAD AND EXTRACT
1a   download mj1-4.exe (about 1.4MB) from Dtack's web site
     - http://www.dtack.com/secure/xxxxxxxx/mj1-4.exe 
     - get the password from Dtack to use in place of xxxxxxxx

1b  execute this file on your system
     - you may double-click on it or use the taskbar Start->Run
     - in the "Install to" text field, you may choose any permanent 
       directory, but we suggest c:\mj\1-4


2----GET VIDEO CAPTURE STARTED
2a  open a DOS box and type:
      c:
      cd \mj\1-4
      vc -d -1
    - write down the first (unique) part of a capture card's name, such as
      "Winnov" or "Videum" or "Creative"

2b  double-click on the "Edit mjgrab.arg" icon in the mj1-4 Program group
    - on the line beginning with "-input", replace the "Winnov" with
      the first part of your capture card's name as noted above

2c  double-click on the "Edit mjgrab.conf.arg" icon in the mj1-4 Program group
    - make the same change as immediately above

2d  double-click on "Mjgrab start conf" in the mj1-4 Program group
    - in the ensuing dialogs, set your capture card to 24 bit RGB
      (you MUST use this color setting)
    - after finishing with the dialogs, a live Video Monitor
      will appear, updating at least twice per second


3---START THE SERVERS
3a  double-click on "Start servers" in the mj1-4 Program group
    - a black DOS-box window will appear showing program start messages
    - all servers log errors to c:\mj\logs

    Note: if you click on "Start servers", then you do NOT need to click on
      "Filter graph server" or "Mjgrab start".

3b  - test the mj server by using your favorite browser to surf to 
      http://your.computer.com:5788/docs?mjarch1.gif


4---BROWSE THE STREAMS
4a  using your browser, go to the url http://your.computer.com:5788/docs?mj.html


5---INSTALL UNDER REAL WEB SERVER (OPTIONAL)
5a  copy "nph-mj.exe" to "nph-mj.cgi" in a cgi-ready directory on your server
    - set execute permission on this file if necessary

5b  copy the c:\mj\1-4\mj14 folder (java .class files) to the main docs 
      directory of your web server

5c  copy c:\mj\1-4\mj.html to the main docs directory of your web server
    - edit if necessary to reflect your codebase and the location of nph-mj.exe
      and its config file (see comment at start of html)

5d  edit c:\mj\1-4\nph-mj.cgi.arg to specify the paths correctly

5e  using your browser, go to the url http://your.computer.com/mj.html


MANIFEST
--------

watchdog
--------
mjstart.exe           server start/restart watchdog

capture
-------
vc.exe                program to list capture cards available
mjgrab.exe            program to capture video
msvcrtd.dll           run-time library for video capture
mjgrab.arg            normal video capture startup arguments
mjgrab.conf.arg       dialog video capture startup arguments

filters
-------
dtmj_grabit.ax        filter to get image from mjgrab
dtmj_encode.ax        filter to jpeg-encode image
dtmj_cgiout.ax        filter to talk to java
mjdll.bat             batch file to register and unregister filters
regsvr32.exe          utility to self-register filters

server programs
---------------
mjserver.exe          web server daemon program
mjserver.arg          arg file for daemon
nph-mj.exe            filter graph program
nph-mj.arg            arg file for graph program when run by mjserver
nph-mj.cgi.arg        arg file for graph program when run by real web server
nph-mj.grf            the filter graph configuration

java
----
mj.html               sample html which uses mjserver
*.class               pre-compiled java applets



CHANGE HISTORY
--------------
mj1-4 14 July 1997
  - contrast, brightness, hue and saturation Java controls for Winnov Videum
  - Java standalone application now available
  - mjstart provides launch/re-launch capabilities for unattended operation
  - size of distributed com object dll's decreased by 500%
  - client count limit now enforced, gentle message given to browser
  - capture video window now movable and iconizable

mj1-3 28 June 1997
  - filter graph now starts as a CGI program
  - mjserver provides web server capabilities
  - capture hardware settings now passed from Java back to framegrabber

mj1-2 23 June 1997
  - streamlined install process
  - fixed bug in NT shared file access

mj1-1 18 June 1997
  - multiple simultaneous filter graphs now supported
  - persistent filter graph properties allow one-time configuration
  - mjgrab uses shared memory to talk with the filter graphs
  - image size is variable, controlled by mjgrab
  - java applet controls JPEG compression quality on the fly
  - automated install process creates shortcuts

mj1-0 16 June 1997
  - first release


