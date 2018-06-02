cmake_minimum_required(VERSION 3.2.0)

project(Poco)

file(STRINGS "${PROJECT_SOURCE_DIR}/libversion" SHARED_LIBRARY_VERSION)
# Read the version information from the VERSION file
file (STRINGS "${PROJECT_SOURCE_DIR}/VERSION" PACKAGE_VERSION )
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION})

set(PROJECT_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(RELEASE_NAME "Unstable-trunk")

message(STATUS "Poco package version: ${PROJECT_VERSION}")

# Put the libaries and binaries that get built into directories at the
# top of the build tree rather than in hard-to-find leaf
# directories. This simplifies manual testing and the use of the build
# tree rather than installed Boost libraries.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# Windows DLLs are "runtime" for CMake. Output them to "bin" like the Visual Studio projects do.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Append our module directory to CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(${CMAKE_VERSION} VERSION_LESS "3.4")
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/V33)
endif()
#################################################################################
# Setup C/C++ compiler options
#################################################################################
option(DISABLE_CPP11 "Disable C++11 if available" OFF)
option(DISABLE_CPP14 "Disable C++14 if available" OFF)

if (DISABLE_CPP11 OR DISABLE_CPP14)
    add_definitions(-DPOCO_DISABLE_CPP11)

    if (DISABLE_CPP14)
        add_definitions(-DPOCO_DISABLE_CPP14)
    endif()
else()
    # C++11/14 compiler flags
    include(CXX1x)
    check_for_cxx11_compiler(CXX11_COMPILER)

    # If a C++11 compiler is available, then set the appropriate flags
    if(CXX11_COMPILER)
        enable_cxx11()
        check_for_cxx14_compiler(CXX14_COMPILER)

        # If a C++14 compiler is available, then set the appropriate flags
        if(CXX14_COMPILER)
            enable_cxx14()
        endif()
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()
message(STATUS "Setting Poco build type - ${CMAKE_BUILD_TYPE}")

# Include some common macros to simpilfy the Poco CMake files
include(PocoMacros)

if(ENABLE_CRYPTO OR ENABLE_NETSSL)
	find_package(OpenSSL REQUIRED)
else()
	find_package(OpenSSL)
endif()

if(OPENSSL_FOUND)
	option(ENABLE_NETSSL "Enable NetSSL" ON)
	option(ENABLE_CRYPTO "Enable Crypto" ON)
else()
	option(ENABLE_NETSSL "Enable NetSSL" OFF)
	option(ENABLE_CRYPTO "Enable Crypto" OFF)
endif()

if(ENABLE_APACHECONNECTOR)
    find_package(APR REQUIRED)
    find_package(Apache2 REQUIRED)
else()
    find_package(APR)
    find_package(Apache2)    
endif()

if(APRUTIL_FOUND AND APACHE_FOUND AND 
    EXISTS ${PROJECT_SOURCE_DIR}/ApacheConnector AND ENABLE_APACHECONNECTOR)
    option(ENABLE_APACHECONNECTOR "Enable ApacheConnector" ON)
else()
    option(ENABLE_APACHECONNECTOR "Enable ApacheConnector" OFF)
endif()

if(ENABLE_DATA_MYSQL)
  find_package(MySQL REQUIRED)
else()
  find_package(MySQL)
endif()

if(MYSQL_FOUND)
  option(ENABLE_DATA "Enable Data" ON)
  option(ENABLE_DATA_MYSQL "Enable Data MySQL" ON)
else()
  option(ENABLE_DATA "Enable Data" OFF)
  option(ENABLE_DATA_MYSQL "Enable Data MySQL" OFF)
endif()

if(ENABLE_DATA_POSTGRESQL)
    find_package(PostgreSQL REQUIRED)
else()
    find_package(PostgreSQL QUIET)
endif()
 
if(POSTGRESQL_FOUND)
    option(ENABLE_DATA "Enable SQL" ON)
    option(ENABLE_DATA_POSTGRESQL "Enable SQL PosgreSQL" ON)
else()
    option(ENABLE_DATA "Enable SQL" OFF)
    option(ENABLE_DATA_POSTGRESQL "Enable SQL PosgreSQL" OFF)
endif()

if(ENABLE_DATA_ODBC)
  find_package(ODBC REQUIRED)
else()
  find_package(ODBC)
endif()

if(ODBC_FOUND)
  option(ENABLE_DATA "Enable Data" ON)
  option(ENABLE_DATA_ODBC "Enable Data ODBC" ON)
else()
  option(ENABLE_DATA "Enable Data" OFF)
  option(ENABLE_DATA_ODBC "Enable Data ODBC" OFF)
endif()

# Allow enabling and disabling components
option(ENABLE_ENCODINGS "Enable Encodings" ON)
option(ENABLE_ENCODINGS_COMPILER "Enable Encodings Compiler" OFF)
option(ENABLE_XML "Enable XML" ON)
option(ENABLE_JSON "Enable JSON" ON)
option(ENABLE_MONGODB "Enable MongoDB" ON)
option(ENABLE_DATA_SQLITE "Enable Data SQlite" ON)
option(ENABLE_REDIS "Enable Redis" ON)
option(ENABLE_PDF "Enable PDF" OFF)
option(ENABLE_UTIL "Enable Util" ON)
option(ENABLE_NET "Enable Net" ON)
option(ENABLE_NETSSL_WIN "Enable NetSSL Windows" OFF)
option(ENABLE_SEVENZIP "Enable SevenZip" OFF)
option(ENABLE_ZIP "Enable Zip" ON)
option(ENABLE_CPPPARSER "Enable C++ parser" OFF)
option(ENABLE_POCODOC "Enable Poco Documentation Generator" OFF)
option(ENABLE_PAGECOMPILER "Enable PageCompiler" ON)
option(ENABLE_PAGECOMPILER_FILE2PAGE "Enable File2Page" ON)

option(FORCE_OPENSSL "Force usage of OpenSSL even under windows" OFF)

option(ENABLE_TESTS
  "Set to OFF|ON (default is OFF) to control build of POCO tests & samples" OFF)

option(POCO_STATIC
  "Set to OFF|ON (default is OFF) to control build of POCO as STATIC library" OFF)
if(POCO_STATIC)
    message(STATUS "Building static libraries")
    option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
else()
    message(STATUS "Building dynamic libraries")
    option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif()

option(POCO_UNBUNDLED
  "Set to OFF|ON (default is OFF) to control linking dependencies as external" OFF)

if(MSVC)
    option(POCO_MT
      "Set to OFF|ON (default is OFF) to control build of POCO as /MT instead of /MD" OFF)

    option(ENABLE_MSVC_MP
      "Set to OFF|ON (default is OFF) to control parallel build of POCO with MSVC" OFF)
endif()


if(ENABLE_TESTS)
  option(ENABLE_LONG_RUNNING_TESTS "Enable long running test" ON)
  include(CTest)
  enable_testing()
  message(STATUS "Building with unittests & samples")
else()
  message(STATUS "Building without tests & samples")
endif()

if(POCO_UNBUNDLED)
  message(STATUS "Using external sqlite, zlib, pcre, expat, ...")
else()
  message(STATUS "Using internal sqlite, zlib, pcre, expat, ...")
endif()

include(DefinePlatformSpecifc)

# Collect the built libraries and include dirs, the will be used to create the PocoConfig.cmake file
set(Poco_COMPONENTS "")

if(ENABLE_TESTS)
   add_subdirectory(CppUnit)
   set(ENABLE_XML ON CACHE BOOL "Enable XML" FORCE )
   set(ENABLE_JSON ON CACHE BOOL "Enable JSON" FORCE)

    if(ENABLE_ZIP)
        set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    endif()
endif()

if(ENABLE_ENCODINGS_COMPILER OR ENABLE_APACHECONNECTOR)
    set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(ENABLE_PAGECOMPILER_FILE2PAGE)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(ENABLE_PAGECOMPILER)
    set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)    
endif()

if(ENABLE_MONGODB OR ENABLE_REDIS)
    set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
endif()

if(ENABLE_DATA_SQLITE OR ENABLE_DATA_MYSQL OR ENABLE_DATA_ODBC OR ENABLE_DATA_POSTGRESQL)
    set(ENABLE_DATA ON CACHE BOOL "Enable Data" FORCE)
    if(ENABLE_TESTS)
        set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
        set(ENABLE_DATA_SQLITE ON CACHE BOOL "Enable Data SQlite" FORCE)
    endif()
endif()

if(ENABLE_DATA AND ENABLE_TESTS)
    set(ENABLE_DATA_SQLITE ON CACHE BOOL "Enable Data SQlite" FORCE)
endif()

if(ENABLE_NETSSL_WIN)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    if(ENABLE_TESTS)
        set(ENABLE_CRYPTO ON CACHE BOOL "Enable Crypto" FORCE)
    endif()
endif()

if(ENABLE_NETSSL)
	set(ENABLE_CRYPTO ON CACHE BOOL "Enable Crypto" FORCE)
    set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(ENABLE_CRYPTO AND ENABLE_TESTS)
    set(ENABLE_NETSSL ON CACHE BOOL "Enable NetSSL" FORCE)
    set(ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(ENABLE_NET AND ENABLE_TESTS)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(ENABLE_PDF)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
endif()

if(ENABLE_POCODOC)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
    set(ENABLE_CPPPARSER ON CACHE BOOL "Enable C++ parser" FORCE)
endif()

if(ENABLE_SEVENZIP OR ENABLE_ZIP)
    set(ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)    
endif()

if(ENABLE_UTIL AND ENABLE_TESTS)
    set(ENABLE_JSON ON CACHE BOOL "Enable JSON" FORCE)
    set(ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
endif()

add_subdirectory(Foundation)
if(ENABLE_ENCODINGS)
    add_subdirectory(Encodings)
    list(APPEND Poco_COMPONENTS "Encodings")
endif()

if(ENABLE_XML)
    add_subdirectory(XML)
    list(APPEND Poco_COMPONENTS "XML")
endif()

if(ENABLE_JSON)
    add_subdirectory(JSON)
    list(APPEND Poco_COMPONENTS "JSON")
endif()

if(ENABLE_UTIL)
    add_subdirectory(Util)
    list(APPEND Poco_COMPONENTS "Util")
endif()

if(ENABLE_NET)
    add_subdirectory(Net)
    list(APPEND Poco_COMPONENTS "Net")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/MongoDB AND ENABLE_MONGODB)
    add_subdirectory(MongoDB)
    list(APPEND Poco_COMPONENTS "MongoDB")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/Redis AND ENABLE_REDIS)
    add_subdirectory(Redis)
    list(APPEND Poco_COMPONENTS "Redis")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/PDF AND ENABLE_PDF)
    add_subdirectory(PDF)
    list(APPEND Poco_COMPONENTS "PDF")
endif()


#NetSSL


if(WIN32 AND EXISTS ${PROJECT_SOURCE_DIR}/NetSSL_Win AND ENABLE_NETSSL_WIN)
    add_subdirectory(NetSSL_Win)
    list(APPEND Poco_COMPONENTS "NetSSL_Win")
endif(WIN32 AND EXISTS ${PROJECT_SOURCE_DIR}/NetSSL_Win AND ENABLE_NETSSL_WIN)

if(OPENSSL_FOUND)
    if(EXISTS ${PROJECT_SOURCE_DIR}/NetSSL_OpenSSL AND ENABLE_NETSSL)
        add_subdirectory(NetSSL_OpenSSL)
        list(APPEND Poco_COMPONENTS "NetSSL_OpenSSL")
    endif()
    if(EXISTS ${PROJECT_SOURCE_DIR}/Crypto AND ENABLE_CRYPTO)
        add_subdirectory(Crypto)
        list(APPEND Poco_COMPONENTS "Crypto")
    endif()
endif(OPENSSL_FOUND)

if(EXISTS ${PROJECT_SOURCE_DIR}/Data AND ENABLE_DATA)
    add_subdirectory(Data)
    list(APPEND Poco_COMPONENTS "Data")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/SevenZip AND ENABLE_SEVENZIP)
    add_subdirectory(SevenZip)
    list(APPEND Poco_COMPONENTS "SevenZip")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/Zip AND ENABLE_ZIP)
    add_subdirectory(Zip)
    list(APPEND Poco_COMPONENTS "Zip")
endif()

if(APRUTIL_FOUND AND APACHE_FOUND AND 
    EXISTS ${PROJECT_SOURCE_DIR}/ApacheConnector AND ENABLE_APACHECONNECTOR)
    include_directories( "${APACHE_INCLUDE_DIR}" "${APRUTIL_INCLUDE_DIR}" ) # TODO(Bjoe) use target_include_directories or target_link_libraries
    add_subdirectory(ApacheConnector)
    list(APPEND Poco_COMPONENTS "ApacheConnector")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/CppParser AND ENABLE_CPPPARSER)
    add_subdirectory(CppParser)
    list(APPEND Poco_COMPONENTS "CppParser")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/PocoDoc AND ENABLE_POCODOC)
    add_subdirectory(PocoDoc)
    list(APPEND Poco_COMPONENTS "PocoDoc")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/PageCompiler AND ENABLE_PAGECOMPILER)
    add_subdirectory(PageCompiler)
    list(APPEND Poco_COMPONENTS "PageCompiler")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/PageCompiler/File2Page AND ENABLE_PAGECOMPILER_FILE2PAGE)
    add_subdirectory(PageCompiler/File2Page)
    list(APPEND Poco_COMPONENTS "File2Page")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/Encodings/Compiler AND ENABLE_ENCODINGS_COMPILER)
    add_subdirectory(Encodings/Compiler)
    list(APPEND Poco_COMPONENTS "EncodingsCompiler")
endif()


#############################################################
# Uninstall stuff see: http://www.vtk.org/Wiki/CMake_FAQ
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#############################################################
# Enable packaging

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Poco Libraries")
set(CPACK_PACKAGE_VENDOR "Applied Informatics Software Engineering GmbH")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr/local")

include(CPack)

#############################################################
# cmake config files

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" @ONLY)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
        "lib${LIB_SUFFIX}/cmake/${PROJECT_NAME}"
    COMPONENT
        Devel
)

# in tree build settings
#configure_file(PocoBuildTreeSettings.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PocoBuildTreeSettings.cmake @ONLY)


message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
message(STATUS "Installation target path: ${CMAKE_INSTALL_PREFIX}")

message(STATUS "C_FLAGS:  =${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_DEBUG:=${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELEASE:=${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_C_FLAGS_MINSIZEREL:=${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO:=${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "")
message(STATUS "")
message(STATUS "CXX_FLAGS:=${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG:=${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE:=${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:=${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

foreach(component ${Poco_COMPONENTS})
    message(STATUS "Building: ${component}")
endforeach()
