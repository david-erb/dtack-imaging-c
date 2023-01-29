Dtack Mjpeg Rev 1-3 readme.txt 6/28/97


INTRODUCTION
------------
This suite of programs implements a set of ActiveMovie filter
components and Java applet classes which work together to deliver
live streaming video in a World Wide Web environment.

The ActiveMovie filter graph is started as a CGI program.
The MJ package includes a program to act as a mini web server.


INSTALLATION
------------
1----DOWNLOAD AND EXTRACT
1a   download mj1-3.exe (about 3.2MB) from Dtack's web site
     - http://www.dtack.com/secure/xxxxxxxx/mj1-3.exe 
     - get the password from Dtack to use in place of xxxxxxxx

1b  execute this file on your system
     - you may double-click on it or use the taskbar Start->Run
     - in the "Install to" text field, you may choose any permanent 
       directory, but we suggest c:\mj\1-3


2----GET VIDEO CAPTURE STARTED
2a  open a DOS box and type:
      c:
      cd \mj\1-3
      vc -d -1
    - write them the first (unique) part of a capture card's name, such as
      "Winnov" or "Videum" or "Creative"

2b  double-click on the "Edit tabgrab.arg" icon in the mj1-3 Program group
    - on the line beginning with "-input", replace the "Winnov" with
      the first part of your capture card's name as noted above

2c  double-click on the "Edit tabgrab.conf.arg" icon in the mj1-3 Program group
    - make the same change as immediately above

2d  double-click on "Tabgrab start conf" in the mj1-3 Program group
    - in the ensuing dialogs, set your capture card to 24 bit RGB
      (you MUST use this color setting)
    - after finishing with the dialogs, a live Video Monitor
      will appear, updating at least once per second


3---START FILTER GRAPH SERVER(S)
3a  double-click on "Filter graph server" in the mj1-3 Program group
    - no window will appear
    - the server will log its errors in \mj\logs
    - only one server instance is necessary for any number of client browsers
    - you may skip this step if you will be running a standard web server
    - mjserver.exe is recommended instead of a standard web server

3b  - test the mj server by using your favorite browser to surf to 
      http://your.computer.com:5788/docs?mjarch1.gif


4---BROWSE THE STREAMS
4a  using your browser, go to the url http://your.computer.com:5788/docs?mj.html


5---INSTALL UNDER REAL WEB SERVER (OPTIONAL)
5a  copy "nph-mj.exe" to "nph-mj.cgi" in a cgi-ready directory on your server
    - set execute permission on this file if necessary

5b  copy the \mj\1-3\mj13 folder (java .class files) to the main docs 
      directory of your web server

5c  copy \mj\1-3\mj_alt.html to the main docs directory of your web server
    - edit if necessary to reflect your codebase and the location of nph-mj.exe

5d  edit \mj\1-3\nph-mj.cgi.arg to specify the paths correctly

5e  using your browser, go to the url http://your.computer.com/mj_alt.html


MANIFEST
--------

capture
-------
vc.exe                     program to list capture cards available
tabgrab.exe                program to capture video
msvcrtd.dll                run-time library for video capture
tabgrab.arg                normal video capture startup arguments
tabgrab.conf.arg           dialog video capture startup arguments

filters
-------
dtmj_grabit.ax             filter to get image from tabgrab
dtmj_encode.ax             filter to jpeg-encode image
dtmj_cgiout.ax             filter to talk to java
mjdll.bat                  batch file to register and unregister filters
regsvr32.exe               utility to self-register filters

filter graph
------------
mjserver.exe               web server daemon program
mjserver.arg               arg file for daemon
nph-mj.exe                 filter graph program
nph-mj.arg                 arg file for graph program when run by mjserver
nph-mj.cgi.arg             arg file for graph program when run by real web server
nph-mj.grf                 the filter graph configuration

java
----
mj.html                    sample html which uses mjserver
mj_alt.html                sample html which uses a real web server
*.class                    pre-compiled java applets



IMPROVEMENTS TO BE DONE
-----------------------
- watchdog for unattended tabgrab and mjserver operation
- tabgrab window movable



CHANGE HISTORY
--------------
mj1-4 6 Jul 97
  - combined control applets to single applet
  - added java application

mj1-3 28 Jun 97
  - filter graph now starts as a CGI program
  - mjserver provides web server capabilities
  - capture hardware settings now passed from Java back to framegrabber

mj1-2 23 Jun 97
  - streamlined install process
  - fixed bug in NT shared file access

mj1-1 18 Jun 97
  - multiple simultaneous filter graphs now supported
  - persistent filter graph properties allow one-time configuration
  - tabgrab uses shared memory to talk with the filter graphs
  - image size is variable, controlled by tabgrab
  - java applet controls JPEG compression quality on the fly
  - automated install process creates shortcuts

mj1-0 16 Jun 97
  - first release


