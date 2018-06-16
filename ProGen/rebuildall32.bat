setlocal EnableDelayedExpansion EnableExtensions        
rd /q /s ..\lib ..\bin     & call ..\buildwin  90 rebuild all both Win32 samples tests devenv  > log\90.x32.log 2>&1
rd /q /s ..\lib ..\bin     & call ..\buildwin 100 rebuild all both Win32 samples tests msbuild > log\100.x32.log 2>&1
rd /q /s ..\lib ..\bin     & call ..\buildwin 110 rebuild all both Win32 samples tests msbuild > log\110.x32.log 2>&1
rd /q /s ..\lib ..\bin     & call ..\buildwin 120 rebuild all both Win32 samples tests msbuild > log\120.x32.log 2>&1
rd /q /s ..\lib ..\bin     & call ..\buildwin 140 rebuild all both Win32 samples tests msbuild > log\140.x32.log 2>&1
rd /q /s ..\lib ..\bin     & call ..\buildwin 150 rebuild all both Win32 samples tests msbuild > log\150.x32.log 2>&1
endlocal
