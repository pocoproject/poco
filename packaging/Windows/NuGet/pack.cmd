echo off
rem echo Usage:
rem echo ------
rem echo pack [vsvers] [release]       // pack 150 1.10.1 
rem echo ------

setlocal
if "%2"=="" goto usage
if "%1"=="" goto usage
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=CppParser
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Crypto
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Data
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataSQLite
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataMySQL
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataODBC
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataPostgreSQL
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Encodings
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Foundation
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=JSON
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=JWT
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=MongoDB
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Net
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=NetSSL
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=NetSSLWin
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=PDF
nuget pack Pocoproject.Poco.shared.nuspec 										-p vs=vs%1 -p version=%2 -p id=PageCompiler
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Redis
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Util
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=XML
nuget pack Pocoproject.Poco.shared.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Zip
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=CppParser 			-p inc=CppParser
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Crypto 			-p inc=Crypto
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Data 				-p inc=Data
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=DataSQLite 		-p inc="Data\SQLite"
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=DataMySQL 			-p inc="Data\MySQL"
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=DataODBC 			-p inc="Data\ODBC"
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=DataPostgreSQL		-p inc="Data\PostgreSQL"
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Encodings 			-p inc=Encodings
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Foundation 		-p inc=Foundation
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=JSON 				-p inc=JSON
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=JWT 				-p inc=JWT
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=MongoDB 			-p inc=MongoDB
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Net 				-p inc=Net
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=NetSSL 			-p inc=NetSSL_OpenSSL
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=NetSSLWin 			-p inc=NetSSL_Win
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=PDF 				-p inc=PDF
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Redis 				-p inc=Redis
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Util 				-p inc=Util
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=XML 				-p inc=XML
nuget pack Pocoproject.Poco.noarch.nuspec -p vs=vs%1 -p version=%2 -p id=Zip 				-p inc=Zip
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=CppParser
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Crypto
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Data
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataSQLite
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataMySQL
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataODBC
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=DataPostgreSQL
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Encodings
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Foundation
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=JSON
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=JWT
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=MongoDB
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Net
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=NetSSL
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=NetSSLWin
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=PDF
nuget pack Pocoproject.Poco.static.nuspec 										-p vs=vs%1 -p version=%2 -p id=PageCompiler
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Redis
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Util
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=XML
nuget pack Pocoproject.Poco.static.nuspec -symbols -symbolpackageformat snupkg	-p vs=vs%1 -p version=%2 -p id=Zip
goto exit
:usage
echo Usage:
echo ------
echo pack [vsvers] [release]       // pack 150 1.10.1 
echo ------
:exit
endlocal
echo on
