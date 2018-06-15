setlocal EnableExtensions EnableDelayedExpansion
rem rd /q /s lib64 bin64 & buildwin  90 rebuild all both x64   samples tests devenv  > log\90.x64.log 2>&1
rd /q /s lib64 bin64 & buildwin 100 rebuild all both x64   samples tests msbuild > log\100.x64.log 2>&1
rd /q /s lib64 bin64 & buildwin 110 rebuild all both x64   samples tests msbuild > log\110.x64.log 2>&1
rd /q /s lib64 bin64 & buildwin 120 rebuild all both x64   samples tests msbuild > log\120.x64.log 2>&1
rd /q /s lib64 bin64 & buildwin 140 rebuild all both x64   samples tests msbuild > log\140.x64.log 2>&1
rd /q /s lib64 bin64 & buildwin 150 rebuild all both x64   samples tests msbuild > log\150.x64.log 2>&1
endlocal
