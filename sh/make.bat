@rem call c:\1\1devel\zlib\sh\dtmake.bat wv5 . libs
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 source libs
call c:\1\1devel\dt\sh\dtmake.bat wv5 source\low libs
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 source\fb libs
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 test static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 tool static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/uclient static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/tabrobot static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/tableau static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/tabchief static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/tableau static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/tabrobot static
@call c:\1\1devel\dt\sh\dtmake.bat wv5 test/low static
call c:\1\1devel\dt\sh\dtmake.bat wv5 app/tabrobot static
@rem call c:\1\1devel\dt\sh\dtmake.bat wv5 app/uclient static
@rem copy/y dt.wv5\static\uclient.exe c:\golftec\02-09
:exit
