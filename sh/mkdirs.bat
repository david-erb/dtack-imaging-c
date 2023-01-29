if "%1"=="" goto badarg

mkdir %1

mkdir %1\dtutil

mkdir %1\fourth
mkdir %1\fourth\btpcv3
mkdir %1\fourth\mpegssg
mkdir %1\fourth\xv310a
mkdir %1\fourth\zlib
mkdir %1\fourth\jpeg6a
mkdir %1\fourth\emsif
mkdir %1\fourth\xmsif
mkdir %1\fourth\svgacc
mkdir %1\fourth\fastg

mkdir %1\source
mkdir %1\source\1d
mkdir %1\source\low
mkdir %1\source\dg
mkdir %1\source\bigmem
mkdir %1\source\keybd
mkdir %1\source\inter
mkdir %1\source\screen
mkdir %1\source\fb
mkdir %1\source\mpeg
mkdir %1\source\color
mkdir %1\source\win
mkdir %1\source\high
mkdir %1\source\xchg

mkdir %1\alg
mkdir %1\alg\esc94
mkdir %1\alg\lev85
mkdir %1\alg\sez90

mkdir %1\include

mkdir %1\test
mkdir %1\test\lib
mkdir %1\test\libdll
mkdir %1\test\w31
mkdir %1\test\high
mkdir %1\test\inter
mkdir %1\test\bigmem
mkdir %1\test\low
mkdir %1\test\1d
mkdir %1\test\ftp
mkdir %1\test\xchg
mkdir %1\test\view
mkdir %1\test\win
mkdir %1\test\fb
mkdir %1\test\screen
mkdir %1\test\xms

mkdir %1\tool
mkdir %1\server

mkdir %1\app
mkdir %1\app\uclient
mkdir %1\app\reverb
mkdir %1\app\tabchief
mkdir %1\app\tableau
mkdir %1\app\tabgrab
mkdir %1\app\tabrobot
mkdir %1\app\tabdrone
mkdir %1\app\wrangler
mkdir %1\app\mpeg

mkdir %1\lib
mkdir %1\libdll
mkdir %1\static
mkdir %1\dynamic
mkdir %1\bat
mkdir %1\tmp
mkdir %1\data
mkdir %1\config
goto exit
:badarg
echo dtmkdirs.bat: not enough arguments
goto exit
:exit
