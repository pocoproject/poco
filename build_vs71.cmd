@echo off

rem
rem build.cmd
rem
rem $Id: //poco/1.2/dist/build_vs71.cmd#1 $
rem
rem command-line build script for VS 7.1
rem

cd CppUnit
devenv /useenv /rebuild debug_shared CppUnit_vs71.sln
devenv /useenv /rebuild release_shared CppUnit_vs71.sln
cd ..

cd Foundation
devenv /useenv /rebuild debug_shared Foundation_vs71.sln
devenv /useenv /rebuild release_shared Foundation_vs71.sln
cd samples
devenv /useenv /rebuild debug_shared samples_vs71.sln
devenv /useenv /rebuild release_shared samples_vs71.sln
cd ..
cd ..

cd XML
devenv /useenv /rebuild debug_shared XML_vs71.sln
devenv /useenv /rebuild release_shared XML_vs71.sln
cd samples
devenv /useenv /rebuild debug_shared samples_vs71.sln
devenv /useenv /rebuild release_shared samples_vs71.sln
cd ..
cd ..

cd Util
devenv /useenv /rebuild debug_shared Util_vs71.sln
devenv /useenv /rebuild release_shared Util_vs71.sln
cd samples
devenv /useenv /rebuild debug_shared samples_vs71.sln
devenv /useenv /rebuild release_shared samples_vs71.sln
cd ..
cd ..

cd Net
devenv /useenv /rebuild debug_shared Net_vs71.sln
devenv /useenv /rebuild release_shared Net_vs71.sln
cd samples
devenv /useenv /rebuild debug_shared samples_vs71.sln
devenv /useenv /rebuild release_shared samples_vs71.sln
cd ..
cd ..
