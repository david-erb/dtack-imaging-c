@echo off
set CPU=i386
set AXSDK=d:\amovie
k:
cd com\mj1-4
nmake -f %1
cd ..\..
d:
