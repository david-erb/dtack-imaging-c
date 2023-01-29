call dtset
del dt.ziz
del dt.zip
dir a:
rexec 192.9.200.1 pcftp pcftp1 "/usr/erb/sh/waitfor dt.ziz"
ftp -f c:\ftp\getdtziz
rexec 192.9.200.1 pcftp pcftp1 "rm dt.ziz"
copy dt.ziz a:
gzip -c -d <dt.ziz >dt.zip

if "%1"=="" goto done

call dtinst %1

:done
