@echo off
rem
rem $Id$
rem
rem A script for running the POCO testsuites.
rem
rem usage: runtests2 [64] [-d ] [component [test]]
rem
rem component   : the component under test
rem test        : the test as part of the component

rem If the environment variable EXCLUDE_TESTS is set, containing
rem a space-separated list of project names (as found in the
rem components file), these tests will be skipped.
rem

setlocal EnableDelayedExpansion

set TESTRUNNERARGS=-all
set TESTCOMPONENTS='findstr /R "." components'
set BINDIR=bin

if "%1"=="64" (
  set BINDIR=bin64
  shift
)
if "%1" =="-d" (
  set DEBUG=d
  shift
)

if not "%1" == "" (
  set TESTCOMPONENTS="%1"
  if not "%2" == "" (
    set TESTRUNNERARGS=%2
  )
)

set TESTRUNNER=TestSuite%DEBUG%.exe

set runs=0
set failures=0
set failedTests=
set status=0


for /f %%C in ( %TESTCOMPONENTS% ) do (
  set excluded=0
  for %%X in (%EXCLUDE_TESTS%) do (
    if "%%X"=="%%C" (
      set excluded=1
    )
  )
  if !excluded!==0 (
    if exist %%C (
      if exist %%C\testsuite (
        if exist %%C\testsuite\%BINDIR%\%TESTRUNNER% (
          echo.
          echo.
          echo ****************************************
          echo *** %%C                               
          echo **************************************** 
          echo.

		  set /a runs=!runs! + 1
		  set dir=%CD%
		  cd %%C\testsuite
		  %BINDIR%\%TESTRUNNER% %TESTRUNNERARGS%
		  if !ERRORLEVEL! neq 0 (
		    set /a failures=!failures! + 1
		    set failedTests=!failedTests! %%C
		    set status=1
		  )
		  cd !dir!
        )
      )
    )
  )
)

echo.
echo.
echo !runs! runs, !failures! failed.
echo.
for %%F in (!failedTests!) do (
	echo Failed: %%F
)
echo.

exit /b !status!
