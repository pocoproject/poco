@echo off

rem
rem buildwin.cmd
rem
rem command-line build script for MS Visual Studio
rem
rem Usage:
rem ------
rem buildwin VS_VERSION        LINKMODE          CONFIGURATION    SAMPLES
rem buildwin {71|80|90} [static|shared|both] [release|debug|both] [yes|no]
rem
rem VS_VERSION is required argument. Default is build all.

rem Change OPENSSL_DIR to match your setup
set OPENSSL_DIR=c:\OpenSSL
set OPENSSL_INCLUDE=%OPENSSL_DIR%\include
set OPENSSL_LIB=%OPENSSL_DIR%\lib\VC
set INCLUDE=%INCLUDE%;%OPENSSL_INCLUDE%
set LIB=%LIB%;%OPENSSL_LIB%

set POCOBASE=%CD%

rem VS version {71|80|90}
if "%1"=="" goto usage
set VS_VERSION=vs%1

rem Link mode [static|shared|both]
set LINK_MODE=%2
if not "%LINK_MODE%"=="static" (
if not "%LINK_MODE%"=="shared" (
if not "%LINK_MODE%"=="" (
if not "%LINK_MODE%"=="both"  goto usage)))

rem Configuration [release|debug|both]
set CONFIGURATION=%3
if not "%CONFIGURATION%"=="release" (
if not "%CONFIGURATION%"=="debug" (
if not "%CONFIGURATION%"=="" (
if not "%CONFIGURATION%"=="both" goto usage)))

rem Samples [yes|no]
set SAMPLES=%4
if "%SAMPLES%"=="" (set SAMPLES=yes)

set RELEASE_SHARED=0
set DEBUG_SHARED=0
set DEBUG_STATIC=0
set RELEASE_STATIC=0

if "%LINK_MODE%"=="shared" (
if "%CONFIGURATION%"=="release" (set RELEASE_SHARED=1) else (
if "%CONFIGURATION%"=="both" (set RELEASE_SHARED=1) else (
if "%CONFIGURATION%"=="" (set RELEASE_SHARED=1))))

if "%LINK_MODE%"=="shared" (
if "%CONFIGURATION%"=="debug" (set DEBUG_SHARED=1) else (
if "%CONFIGURATION%"=="both" (set DEBUG_SHARED=1) else (
if "%CONFIGURATION%"=="" (set DEBUG_SHARED=1))))

if "%LINK_MODE%"=="static" (
if "%CONFIGURATION%"=="release" (set RELEASE_STATIC=1) else (
if "%CONFIGURATION%"=="both" (set RELEASE_STATIC=1) else (
if "%CONFIGURATION%"=="" (set RELEASE_STATIC=1))))

if "%LINK_MODE%"=="static" (
if "%CONFIGURATION%"=="debug" (set DEBUG_STATIC=1) else (
if "%CONFIGURATION%"=="both" (set DEBUG_STATIC=1) else (
if "%CONFIGURATION%"=="" (set DEBUG_STATIC=1))))

if "%LINK_MODE%"=="both" (
if "%CONFIGURATION%"=="debug" (
set DEBUG_STATIC=1
set DEBUG_SHARED=1) else (
if "%CONFIGURATION%"=="release" (
set RELEASE_STATIC=1
set RELEASE_SHARED=1) else (
if "%CONFIGURATION%"=="both" (
set DEBUG_STATIC=1
set DEBUG_SHARED=1
set RELEASE_STATIC=1
set RELEASE_SHARED=1) else (
if "%CONFIGURATION%"=="" (
set DEBUG_STATIC=1
set DEBUG_SHARED=1
set RELEASE_STATIC=1
set RELEASE_SHARED=1)))))

if "%LINK_MODE%"=="" (
if "%CONFIGURATION%"=="debug" (
set DEBUG_STATIC=1
set DEBUG_SHARED=1) else (
if "%CONFIGURATION%"=="release" (
set RELEASE_STATIC=1
set RELEASE_SHARED=1) else (
if "%CONFIGURATION%"=="both" (
set DEBUG_STATIC=1
set DEBUG_SHARED=1
set RELEASE_STATIC=1
set RELEASE_SHARED=1) else (
if "%CONFIGURATION%"=="" (
set DEBUG_STATIC=1
set DEBUG_SHARED=1
set RELEASE_STATIC=1
set RELEASE_SHARED=1)))))

cd CppUnit
if %DEBUG_SHARED%==1   (devenv /useenv /rebuild debug_shared CppUnit_%VS_VERSION%.sln)
if %RELEASE_SHARED%==1 (devenv /useenv /rebuild release_shared CppUnit_%VS_VERSION%.sln)
if %DEBUG_STATIC%==1   (devenv /useenv /rebuild debug_static CppUnit_%VS_VERSION%.sln)
if %RELEASE_STATIC%==1 (devenv /useenv /rebuild release_static CppUnit_%VS_VERSION%.sln)
cd %POCOBASE%

cd Foundation
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared Foundation_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared Foundation_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static Foundation_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static Foundation_%VS_VERSION%.sln
cd %POCOBASE%

cd XML
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared XML_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared XML_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static XML_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static XML_%VS_VERSION%.sln
cd %POCOBASE%

cd Util
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared Util_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared Util_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static Util_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static Util_%VS_VERSION%.sln
cd %POCOBASE%

cd Net
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared Net_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared Net_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static Net_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static Net_%VS_VERSION%.sln
cd %POCOBASE%

cd NetSSL_OpenSSL
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared NetSSL_OpenSSL_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared NetSSL_OpenSSL_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static NetSSL_OpenSSL_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static NetSSL_OpenSSL_%VS_VERSION%.sln
cd %POCOBASE%

cd Data
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared Data_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared Data_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static Data_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static Data_%VS_VERSION%.sln
cd %POCOBASE%

cd Data/SQLite
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared SQLite_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared SQLite_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static SQLite_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static SQLite_%VS_VERSION%.sln
cd %POCOBASE%

cd Data/ODBC
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared ODBC_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared ODBC_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static ODBC_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static ODBC_%VS_VERSION%.sln
cd %POCOBASE%

if "%SAMPLES%"=="no" goto :EOF

cd Foundation/samples
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared samples_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared samples_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static samples_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static samples_%VS_VERSION%.sln
cd %POCOBASE%
		
cd XML/samples
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared samples_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared samples_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static samples_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static samples_%VS_VERSION%.sln
cd %POCOBASE%
		
cd Util/samples
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared samples_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared samples_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static samples_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static samples_%VS_VERSION%.sln
cd %POCOBASE%
		
cd Net/samples
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared samples_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared samples_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static samples_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static samples_%VS_VERSION%.sln
cd %POCOBASE%
		
cd NetSSL_OpenSSL/samples
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared samples_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared samples_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static samples_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static samples_%VS_VERSION%.sln
cd %POCOBASE%

cd Data/samples
if %DEBUG_SHARED%==1   devenv /useenv /rebuild debug_shared samples_%VS_VERSION%.sln
if %RELEASE_SHARED%==1 devenv /useenv /rebuild release_shared samples_%VS_VERSION%.sln
if %DEBUG_STATIC%==1   devenv /useenv /rebuild debug_static samples_%VS_VERSION%.sln
if %RELEASE_STATIC%==1 devenv /useenv /rebuild release_static samples_%VS_VERSION%.sln
cd %POCOBASE%

goto :EOF

:usage
echo Usage:
echo ------
echo buildwin  VS_VERSION       [LINKMODE]        [CONFIGURATION]   [SAMPLES]
echo "buildwin {71|80|90} [static|shared|both] [release|debug|both] [yes|no]"