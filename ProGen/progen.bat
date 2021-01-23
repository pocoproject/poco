@echo off
setlocal EnableDelayedExpansion EnableExtensions        
if "%POCO_BASE%"=="" (
	echo POCO_BASE not set
	goto :exit
)
pushd %POCO_BASE%
ProGen\bin\static_md\progen.exe  /platform=%1 CppUnit\CppUnit.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Foundation\Foundation.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Encodings\Encodings.progen
ProGen\bin\static_md\progen.exe  /platform=%1 XML\XML.progen
ProGen\bin\static_md\progen.exe  /platform=%1 JSON\JSON.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Util\Util.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Net\Net.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Crypto\Crypto.progen
ProGen\bin\static_md\progen.exe  /platform=%1 NetSSL_OpenSSL\NetSSL_OpenSSL.progen
ProGen\bin\static_md\progen.exe  /platform=%1 NetSSL_Win\NetSSL_Win.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\Data.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\SQLite\SQLite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\ODBC\ODBC.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\MySQL\MySQL.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\PostgreSQL\PostgreSQL.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Zip\Zip.progen
ProGen\bin\static_md\progen.exe  /platform=%1 PageCompiler\PageCompiler.progen
ProGen\bin\static_md\progen.exe  /platform=%1 PageCompiler\File2Page\File2Page.progen
ProGen\bin\static_md\progen.exe  /platform=%1 PDF\PDF.progen
ProGen\bin\static_md\progen.exe  /platform=%1 CppParser\CppParser.progen
ProGen\bin\static_md\progen.exe  /platform=%1 MongoDB\MongoDB.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Redis\Redis.progen
ProGen\bin\static_md\progen.exe  /platform=%1 ProGen\ProGen.progen


ProGen\bin\static_md\progen.exe  /platform=%1 Foundation\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Encodings\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 XML\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 JSON\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Util\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Net\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Crypto\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 NetSSL_OpenSSL\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 NetSSL_Win\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\SQLite\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\ODBC\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\MySQL\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Data\PostgreSQL\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Zip\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 PageCompiler\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 PDF\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 CppParser\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 MongoDB\testsuite\TestSuite.progen
ProGen\bin\static_md\progen.exe  /platform=%1 Redis\testsuite\TestSuite.progen
popd
:exit
endlocal
