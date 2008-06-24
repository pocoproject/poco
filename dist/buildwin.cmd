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

rem Action [build|rebuild|clean]
set ACTION=%2
if not "%ACTION%"=="build" (
if not "%ACTION%"=="rebuild" (
if not "%ACTION%"=="clean"  goto usage))

rem Link mode [static|shared|both]
set LINK_MODE=%3
if not "%LINK_MODE%"=="static" (
if not "%LINK_MODE%"=="shared" (
if not "%LINK_MODE%"=="" (
if not "%LINK_MODE%"=="both"  goto usage)))

rem Configuration [release|debug|both]
set CONFIGURATION=%4
if not "%CONFIGURATION%"=="release" (
if not "%CONFIGURATION%"=="debug" (
if not "%CONFIGURATION%"=="" (
if not "%CONFIGURATION%"=="both" goto usage)))

rem Samples [yes|no]
set SAMPLES=%5
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

rem root level components
for /f %%G in ('findstr /R /V "./." components') do (
 if exist %%G (
  cd %%G
  echo.
  echo ========== Building %%G ==========
  if %DEBUG_SHARED%==1   (devenv /useenv /%ACTION% debug_shared %%G_%VS_VERSION%.sln)
  if %RELEASE_SHARED%==1 (devenv /useenv /%ACTION% release_shared %%G_%VS_VERSION%.sln)
  if %DEBUG_STATIC%==1   (devenv /useenv /%ACTION% debug_static %%G_%VS_VERSION%.sln)
  if %RELEASE_STATIC%==1 (devenv /useenv /%ACTION% release_static %%G_%VS_VERSION%.sln)
  cd %POCOBASE%
 )
)

rem lower level components
for /f %%G in ('findstr /R "./." components') do (
 if exist %%G (
  cd %%G
   for /f "tokens=1,2 delims=/" %%Q in ("%%G") do (
    echo.
    echo ========== Building %%R ==========
    if %DEBUG_SHARED%==1   (devenv /useenv /%ACTION% debug_shared %%R_%VS_VERSION%.sln)
    if %RELEASE_SHARED%==1 (devenv /useenv /%ACTION% release_shared %%R_%VS_VERSION%.sln)
    if %DEBUG_STATIC%==1   (devenv /useenv /%ACTION% debug_static %%R_%VS_VERSION%.sln)
    if %RELEASE_STATIC%==1 (devenv /useenv /%ACTION% release_static %%R_%VS_VERSION%.sln)
  )
  cd %POCOBASE%
 )
)

if "%SAMPLES%"=="no" goto :EOF

rem root level component samples
for /f %%G in ('findstr /R /V "./." components') do (
 if exist %%G/samples (
  cd %%G/samples
  echo.
  echo ========== Building %%G/samples ==========
  if %DEBUG_SHARED%==1   devenv /useenv /%ACTION% debug_shared samples_%VS_VERSION%.sln
  if %RELEASE_SHARED%==1 devenv /useenv /%ACTION% release_shared samples_%VS_VERSION%.sln
  if %DEBUG_STATIC%==1   devenv /useenv /%ACTION% debug_static samples_%VS_VERSION%.sln
  if %RELEASE_STATIC%==1 devenv /useenv /%ACTION% release_static samples_%VS_VERSION%.sln
  cd %POCOBASE%
 )
)

rem lower level component samples
for /f %%G in ('findstr /R "./." components') do (
 if exist %%G/samples (
  cd %%G/samples
  echo.
  echo ========== Building %%G/samples ==========
  if %DEBUG_SHARED%==1   devenv /useenv /%ACTION% debug_shared samples_%VS_VERSION%.sln
  if %RELEASE_SHARED%==1 devenv /useenv /%ACTION% release_shared samples_%VS_VERSION%.sln
  if %DEBUG_STATIC%==1   devenv /useenv /%ACTION% debug_static samples_%VS_VERSION%.sln
  if %RELEASE_STATIC%==1 devenv /useenv /%ACTION% release_static samples_%VS_VERSION%.sln
  cd %POCOBASE%
 )
)

goto :EOF

:usage
echo Usage:
echo ------
echo buildwin  VS_VERSION         ACTION             [LINKMODE]        [CONFIGURATION]   [SAMPLES]
echo "buildwin {71|80|90} [build|rebuild|clean] [static|shared|both] [release|debug|both] [yes|no]"