setlocal EnableExtensions EnableDelayedExpansion
rd /q /s ..\lib64 ..\bin64 & call buildwin  90 ..\rebuild all bot..\h x64   samples tests devenv  > log\9..\0.x64.log 2>&1
rd /q /s ..\lib64 ..\bin64 & call buildwin 100 ..\rebuild all bot..\h x64   samples tests msbuild > log\10..\0.x64.log 2>&1
rd /q /s ..\lib64 ..\bin64 & call buildwin 110 ..\rebuild all bot..\h x64   samples tests msbuild > log\11..\0.x64.log 2>&1
rd /q /s ..\lib64 ..\bin64 & call buildwin 120 ..\rebuild all bot..\h x64   samples tests msbuild > log\12..\0.x64.log 2>&1
rd /q /s ..\lib64 ..\bin64 & call buildwin 140 ..\rebuild all bot..\h x64   samples tests msbuild > log\14..\0.x64.log 2>&1
rd /q /s ..\lib64 ..\bin64 & call buildwin 150 ..\rebuild all bot..\h x64   samples tests msbuild > log\15..\0.x64.log 2>&1
endlocal
