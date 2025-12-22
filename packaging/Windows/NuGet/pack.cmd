echo off
rem echo Usage:
rem echo ------
rem echo pack (shared | static) [vsvers] [release]       // pack shared 150 1.10.1
rem echo ------
setlocal enableextensions enabledelayedexpansion
if "%1"=="" goto usage
if "%2"=="" goto usage
if "%3"=="" goto usage

del *.%1.%3.nupkg *.%1.%3.snupkg

for %%G in (Data/SQLite Data/ODBC Data/MySQL Data/PostgreSQL Foundation Encodings XML JSON Util Net Crypto NetSSL_OpenSSL NetSSL_Win Data  Zip   JWT PDF CppParser MongoDB Redis) do (
	@copy Poco.front foo
	if exist ..\..\..\%%G\dependencies (
		for /f "delims=" %%M in (..\..\..\%%G\dependencies) do (
			echo ^<dependency id='Poco.%%M.vs$vs$.%1' version='$version$' ^/^> >> foo
		)
	)
	@copy /y /b foo+Poco.%1.back  Poco.%1.nuspec
	set module=%%G
	set inc=%%G
	if [%%G] == [NetSSL_OpenSSL] 	set module=NetSSL
	if [%%G] == [NetSSL_Win] 		set module=NetSSLWin
	if [%%G] == [Data/SQLite] 		set module=DataSQLite		& set inc=Data\SQLite
	if [%%G] == [Data/ODBC] 		set module=DataODBC			& set inc=Data\ODBC
	if [%%G] == [Data/MySQL] 		set module=DataMySQL		& set inc=Data\MySQL
	if [%%G] == [Data/PostgreSQL] 	set module=DataPostgreSQL	& set inc=Data\PostgreSQL
	call nuget pack Poco.noarch.nuspec 									-p vs=%2 -p version=%3 -p id=!module! -p inc=!inc!
	call nuget pack Poco.%1.nuspec -symbols -symbolpackageformat snupkg	-p vs=%2 -p version=%3 -p id=!module! -p mode=%1
)

goto exit
:usage
echo Usage:
echo ------
echo "pack (shared | static) [vsvers] [release]"       // pack shared 150 1.10.1
echo ------
:exit
endlocal
rem echo on