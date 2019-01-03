cmake_minimum_required(VERSION 3.2.0)

project(Poco)

option(POCO_VERBOSE_MESSAGES "Enable informational messages during configure" ON)

file(STRINGS "${PROJECT_SOURCE_DIR}/libversion" SHARED_LIBRARY_VERSION)
# Read the version information from the VERSION file
file (STRINGS "${PROJECT_SOURCE_DIR}/VERSION" PACKAGE_VERSION )
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION})
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION})

set(PROJECT_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(RELEASE_NAME "Unstable-trunk")

if(POCO_VERBOSE_MESSAGES)
    message(STATUS "Poco package version: ${PROJECT_VERSION}")
endif()

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
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option(POCO_DISABLE_CPP14 "Disable C++14 if not available" OFF)

if(NOT POCO_DISABLE_CPP14)
  # C++14 compiler flags
  include(CXX1x)
  check_for_cxx14_compiler(CXX14_COMPILER)

  # If a C++14 compiler is available, then set the appropriate flags
  if(CXX14_COMPILER)
    set(CMAKE_CXX_STANDARD 14)
  else()
    set(POCO_DISABLE_CPP14 ON CACHE BOOL "Disable C++14 if not available" FORCE)
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Include some common macros to simpilfy the Poco CMake files
include(PocoMacros)

if(POCO_STATIC)
	message(DEPRECATION "POCO_STATIC is deprecated and will be removed! Use BUILD_SHARED_LIBS instead")
	if(POCO_VERBOSE_MESSAGES)
		message(STATUS "Building static libraries")
	endif()
	option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
else()
	if(POCO_VERBOSE_MESSAGES)
		message(STATUS "Building dynamic libraries")
	endif()
	option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif()

if(MSVC)
    option(POCO_MT
      "Set to OFF|ON (default is OFF) to control build of POCO as /MT instead of /MD" OFF)

    option(POCO_ENABLE_MSVC_MP
      "Set to OFF|ON (default is OFF) to control parallel build of POCO with MSVC" OFF)


    # allow disabling of internally built OpenSSL# (see below for details)
    # if POCO pre-built OpenSSL directory is found, and POCO_DISABLE_INTERNAL_OPENSSL=OFF,
    # the internal OpenSSL build will be used
    option(POCO_DISABLE_INTERNAL_OPENSSL "Disable internal OpensSSL binaries use" OFF)

    
    if((NOT POCO_DISABLE_INTERNAL_OPENSSL) AND (POCO_ENABLE_NETSSL OR POCO_ENABLE_CRYPTO OR (POCO_ENABLE_SQL_MYSQL AND MINGW)))
      include(UseEmbeddedOpenSSL)
    endif()
endif()

if(WIN32)
    option(POCO_ENABLE_NETSSL_WIN "NetSSL Windows" ON)
endif()

if(POCO_ENABLE_NETSSL OR POCO_ENABLE_CRYPTO OR (POCO_ENABLE_SQL_MYSQL AND MINGW))
	find_package(OpenSSL REQUIRED)
else()
	find_package(OpenSSL)
endif()

if(OPENSSL_FOUND)
	option(POCO_ENABLE_NETSSL "Enable NetSSL" ON)
	option(POCO_ENABLE_CRYPTO "Enable Crypto" ON)
else()
	option(POCO_ENABLE_NETSSL "Enable NetSSL" OFF)
	option(POCO_ENABLE_CRYPTO "Enable Crypto" OFF)
endif()

if(POCO_ENABLE_APACHECONNECTOR)
    find_package(APR REQUIRED)
    find_package(Apache2 REQUIRED)
else()
    find_package(APR)
    find_package(Apache2)
endif()

if(APRUTIL_FOUND AND APACHE_FOUND)
    option(POCO_ENABLE_APACHECONNECTOR "Enable ApacheConnector" ON)
else()
    option(POCO_ENABLE_APACHECONNECTOR "Enable ApacheConnector" OFF)
endif()

if(POCO_ENABLE_SQL_MYSQL)
  find_package(MySQL REQUIRED)
else()
  find_package(MySQL)
endif()

if(MYSQL_FOUND)
  option(POCO_ENABLE_SQL "Enable SQL" ON)
  option(POCO_ENABLE_SQL_MYSQL "Enable SQL MySQL" ON)
else()
  option(POCO_ENABLE_SQL "Enable SQL" OFF)
  option(POCO_ENABLE_SQL_MYSQL "Enable SQL MySQL" OFF)
endif()

if(POCO_ENABLE_SQL_POSTGRESQL)
    find_package(PostgreSQL REQUIRED)
else()
    find_package(PostgreSQL)
endif()

if(POSTGRESQL_FOUND)
    option(POCO_ENABLE_SQL "Enable SQL" ON)
    option(POCO_ENABLE_SQL_POSTGRESQL "Enable SQL PosgreSQL" ON)
else()
    option(POCO_ENABLE_SQL "Enable SQL" OFF)
    option(POCO_ENABLE_SQL_POSTGRESQL "Enable SQL PosgreSQL" OFF)
endif()

if(POCO_ENABLE_SQL_ODBC)
  find_package(ODBC REQUIRED)
else()
  find_package(ODBC)
endif()

if(ODBC_FOUND)
  option(POCO_ENABLE_SQL "Enable SQL" ON)
  option(POCO_ENABLE_SQL_ODBC "Enable SQL ODBC" ON)
else()
  option(POCO_ENABLE_SQL "Enable SQL" OFF)
  option(POCO_ENABLE_SQL_ODBC "Enable SQL ODBC" OFF)
endif()

# Allow enabling and disabling components
option(POCO_ENABLE_ENCODINGS "Enable Encodings" ON)
option(POCO_ENABLE_ENCODINGS_COMPILER "Enable Encodings Compiler" OFF)
option(POCO_ENABLE_XML "Enable XML" ON)
option(POCO_ENABLE_JSON "Enable JSON" ON)
option(POCO_ENABLE_MONGODB "Enable MongoDB" ON)
option(POCO_ENABLE_SQL_SQLITE "Enable SQL SQLite" OFF)
option(POCO_ENABLE_REDIS "Enable Redis" ON)
option(POCO_ENABLE_PDF "Enable PDF" OFF)
option(POCO_ENABLE_UTIL "Enable Util" ON)
option(POCO_ENABLE_NET "Enable Net" ON)

option(POCO_ENABLE_SEVENZIP "Enable SevenZip" OFF)
option(POCO_ENABLE_ZIP "Enable Zip" ON)
option(POCO_ENABLE_CPPPARSER "Enable C++ parser" OFF)
option(POCO_ENABLE_POCODOC "Enable Poco Documentation Generator" OFF)
option(POCO_ENABLE_PAGECOMPILER "Enable PageCompiler" ON)
option(POCO_ENABLE_PAGECOMPILER_FILE2PAGE "Enable File2Page" ON)
option(POCO_ENABLE_WSTRING "Enable std::wstring support" ON)
option(POCO_ENABLE_FPENVIRONMENT "Enable floating-point support" ON)
option(POCO_ENABLE_CPPUNIT "Enable CppUnit" ON)

option(POCO_ENABLE_TESTS
  "Set to OFF|ON (default is OFF) to control build of POCO tests" OFF)

option(POCO_ENABLE_SAMPLES
  "Set to OFF|ON (default is OFF) to control build of POCO samples" OFF)
option(POCO_UNBUNDLED
  "Set to OFF|ON (default is OFF) to control linking dependencies as external" OFF)

if(POCO_ENABLE_TESTS)
  option(POCO_ENABLE_LONG_RUNNING_TESTS "Enable long running test" ON)
  include(CTest)
  enable_testing()
  if(POCO_VERBOSE_MESSAGES)
      message(STATUS "Building with tests")
  endif()
elseif (POCO_VERBOSE_MESSAGES)
  message(STATUS "Building without tests")
endif ()

if (POCO_ENABLE_SAMPLES)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "Building with samples")
    endif()
elseif (POCO_VERBOSE_MESSAGES)
    message(STATUS "Building without samples")
endif ()

if(POCO_UNBUNDLED)
  if(POCO_VERBOSE_MESSAGES)
      message(STATUS "Build with using external sqlite, libz, pcre, expat ...")
  endif()
elseif(POCO_VERBOSE_MESSAGES)
  message(STATUS "Build with using internal copy of sqlite, libz, pcre, expat, ...")
endif()

include(DefinePlatformSpecifc)

# Collect the built libraries and include dirs, the will be used to create the PocoConfig.cmake file
set(Poco_COMPONENTS "")
set(POCO_PKG_CONFIG_FILES "")
# Pthreads/threads support
find_package(Threads REQUIRED)

if(POCO_ENABLE_TESTS)
   set(POCO_ENABLE_XML ON CACHE BOOL "Enable XML" FORCE )
   set(POCO_ENABLE_JSON ON CACHE BOOL "Enable JSON" FORCE)

    if(POCO_ENABLE_ZIP)
	set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    endif()
endif()

if(POCO_ENABLE_ENCODINGS_COMPILER OR POCO_ENABLE_APACHECONNECTOR)
    set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(POCO_ENABLE_PAGECOMPILER_FILE2PAGE)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(POCO_ENABLE_PAGECOMPILER)
    set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(POCO_ENABLE_MONGODB OR POCO_ENABLE_REDIS)
    set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
endif()

if(POCO_ENABLE_SQL_SQLITE OR POCO_ENABLE_SQL_MYSQL OR POCO_ENABLE_SQL_ODBC OR POCO_ENABLE_SQL_POSTGRESQL)
    set(POCO_ENABLE_SQL ON CACHE BOOL "Enable Data" FORCE)
    if(POCO_ENABLE_TESTS)
        set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
        set(POCO_ENABLE_SQL_SQLITE ON CACHE BOOL "Enable Data SQlite" FORCE)
    endif()
endif()

if(POCO_ENABLE_SQL AND POCO_ENABLE_SAMPLES)
    set(POCO_ENABLE_SQL_SQLITE ON CACHE BOOL "Enable Data SQlite" FORCE)
endif()

if(POCO_ENABLE_NETSSL_WIN)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(POCO_ENABLE_NETSSL)
    set(POCO_ENABLE_CRYPTO ON CACHE BOOL "Enable Crypto" FORCE)
    set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(POCO_ENABLE_CRYPTO AND POCO_ENABLE_TESTS)
	set(POCO_ENABLE_NETSSL ON CACHE BOOL "Enable NetSSL" FORCE)
	set(POCO_ENABLE_NET ON CACHE BOOL "Enable Net" FORCE)
endif()

if(POCO_ENABLE_CRYPTO AND POCO_ENABLE_SAMPLES)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(POCO_ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
endif()

if(POCO_ENABLE_NET AND POCO_ENABLE_TESTS)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
endif()

if(POCO_ENABLE_NET AND POCO_ENABLE_SAMPLES)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(POCO_ENABLE_JSON ON CACHE BOOL "Enable JSON" FORCE)
endif()

if(POCO_ENABLE_PDF)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(POCO_ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
endif()

if(POCO_ENABLE_POCODOC)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(POCO_ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
    set(POCO_ENABLE_CPPPARSER ON CACHE BOOL "Enable C++ parser" FORCE)
endif()

if(POCO_ENABLE_SEVENZIP OR POCO_ENABLE_ZIP)
    set(POCO_ENABLE_UTIL ON CACHE BOOL "Enable Util" FORCE)
    set(POCO_ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
endif()

if(POCO_ENABLE_UTIL AND POCO_ENABLE_SAMPLES)
    set(POCO_ENABLE_JSON ON CACHE BOOL "Enable JSON" FORCE)
    set(POCO_ENABLE_XML ON CACHE BOOL "Enable XML" FORCE)
endif()

if (POCO_ENABLE_TESTS OR POCO_ENABLE_CPPUNIT)
    add_subdirectory(CppUnit)
    list(APPEND Poco_COMPONENTS "CppUnit")
endif ()

add_subdirectory(Foundation)
if(POCO_ENABLE_ENCODINGS)
    add_subdirectory(Encodings)
    list(APPEND Poco_COMPONENTS "Encodings")
endif()

if(POCO_ENABLE_XML)
    add_subdirectory(XML)
    list(APPEND Poco_COMPONENTS "XML")
endif()

if(POCO_ENABLE_JSON)
    add_subdirectory(JSON)
    list(APPEND Poco_COMPONENTS "JSON")
endif()

if(POCO_ENABLE_UTIL)
    add_subdirectory(Util)
    list(APPEND Poco_COMPONENTS "Util")
endif()

if(POCO_ENABLE_NET)
    add_subdirectory(Net)
    list(APPEND Poco_COMPONENTS "Net")
endif()

if(POCO_ENABLE_MONGODB)
    add_subdirectory(MongoDB)
    list(APPEND Poco_COMPONENTS "MongoDB")
endif()

if(POCO_ENABLE_REDIS)
    add_subdirectory(Redis)
    list(APPEND Poco_COMPONENTS "Redis")
endif()

if(POCO_ENABLE_PDF)
    add_subdirectory(PDF)
    list(APPEND Poco_COMPONENTS "PDF")
endif()


#NetSSL


if(WIN32 AND POCO_ENABLE_NETSSL_WIN)
    add_subdirectory(NetSSL_Win)
    list(APPEND Poco_COMPONENTS "NetSSL_Win")
endif(WIN32 AND POCO_ENABLE_NETSSL_WIN)

if(OPENSSL_FOUND)
    if(POCO_ENABLE_CRYPTO)
	 add_subdirectory(Crypto)
	 list(APPEND Poco_COMPONENTS "Crypto")
    endif()
    if(POCO_ENABLE_NETSSL)
	add_subdirectory(NetSSL_OpenSSL)
	list(APPEND Poco_COMPONENTS "NetSSL_OpenSSL")
    endif()
endif(OPENSSL_FOUND)

if(POCO_ENABLE_SQL)
    add_subdirectory(SQL)
    list(APPEND Poco_COMPONENTS "SQL")
endif()

if(POCO_ENABLE_SEVENZIP)
    add_subdirectory(SevenZip)
    list(APPEND Poco_COMPONENTS "SevenZip")
endif()

if(POCO_ENABLE_ZIP)
    add_subdirectory(Zip)
    list(APPEND Poco_COMPONENTS "Zip")
endif()

if(APRUTIL_FOUND AND APACHE_FOUND AND POCO_ENABLE_APACHECONNECTOR)
    add_subdirectory(ApacheConnector)
    list(APPEND Poco_COMPONENTS "ApacheConnector")
endif()

if(POCO_ENABLE_CPPPARSER)
    add_subdirectory(CppParser)
    list(APPEND Poco_COMPONENTS "CppParser")
endif()

if(POCO_ENABLE_POCODOC)
    add_subdirectory(PocoDoc)
    list(APPEND Poco_COMPONENTS "PocoDoc")
endif()

if(POCO_ENABLE_PAGECOMPILER)
    add_subdirectory(PageCompiler)
    list(APPEND Poco_COMPONENTS "PageCompiler")
endif()

if(POCO_ENABLE_PAGECOMPILER_FILE2PAGE)
    add_subdirectory(PageCompiler/File2Page)
    list(APPEND Poco_COMPONENTS "File2Page")
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/Encodings/Compiler AND POCO_ENABLE_ENCODINGS_COMPILER)
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

# Set config script install location in a location that find_package() will
# look for, which is different on MS Windows than for UNIX
# Note: also set in POCO_GENERATE_PACKAGE macro in cmake/PocoMacros.cmake
if (WIN32)
  set(PocoConfigPackageLocation "cmake")
else()
  set(PocoConfigPackageLocation "lib/cmake/${PROJECT_NAME}")
endif()

configure_file(cmake/${PROJECT_NAME}Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake" @ONLY)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
        "${PocoConfigPackageLocation}"
    COMPONENT
        Devel
)

# Install pkg-config files
install(FILES ${POCO_PKG_CONFIG_FILES}
        DESTINATION lib${LIB_SUFFIX}/pkgconfig)

if(POCO_VERBOSE_MESSAGES)
    message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
    message(STATUS "Installation target path: ${CMAKE_INSTALL_PREFIX}")
    message(STATUS "Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")

    string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
    message(STATUS "[cmake] build type:             ${CMAKE_BUILD_TYPE}")
    message(STATUS "[cmake] build with cxx flags:   ${CMAKE_CXX_FLAGS_${BUILD_TYPE}} ${CMAKE_CXX_FLAGS}")
    message(STATUS "[cmake] build with c flags:     ${CMAKE_C_FLAGS_${BUILD_TYPE}} ${CMAKE_C_FLAGS}")

    foreach(component ${Poco_COMPONENTS})
        message(STATUS "Building: ${component}")
    endforeach()
endif()
