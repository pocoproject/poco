rem @echo off
rem Copyright (c) 2012-2016, EnterpriseDB Corporation.  All rights reserved

rem PostgreSQL server psql runner script for Windows

set server=localhost
set database=postgres
set port=5432
set username=postgres
set command="ALTER USER postgres WITH ENCRYPTED PASSWORD 'postgres';"

for /f "delims=" %%a in ('chcp ^|find /c "932"') do @ set CLIENTENCODING_JP=%%a
if "%CLIENTENCODING_JP%"=="1" set PGCLIENTENCODING=SJIS
if "%CLIENTENCODING_JP%"=="1" set /P PGCLIENTENCODING="Client Encoding [%PGCLIENTENCODING%]: "

rem Run psql
"C:\Program Files\PostgreSQL\9.5\bin\psql.exe" --no-password -h %server% -U %username% -d %database% -p %port%  --command=%command%;


rem pause

rem PGPASSFILE 
rem chmod 0600 ~/.pgpass
rem %APPDATA%\postgresql\pgpass.conf