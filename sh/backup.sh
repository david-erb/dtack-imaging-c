#! /bin/bash

set -x


cd /usr/erb/prj/dt
rm -f *.zip

cd /usr/erb/prj
mytidy dt

date=`date +%Y%m%d.%H%M%S`

tarball=/usr/erb/backup/dt.$date.tgz 
tar czf $tarball dt
ls -lt $tarball

