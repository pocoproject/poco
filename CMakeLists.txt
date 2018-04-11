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
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake32)
endif()

#################################################################################
# Setup C/C++ compiler options
#################################################################################
option(DISABLE_CPP14 "Disable C++14 if available" OFF)

if (DISABLE_CPP14)
    add_definitions(-DPOCO_DISABLE_CPP14)
else()
  # C++14 compiler flags
  include(CXX1x)
  check_for_cxx14_compiler(CXX14_COMPILER)

  # If a C++14 compiler is available, then set the appropriate flags
  if(CXX14_COMPILER)
    enable_cxx14()
  endif()
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Include some common macros to simpilfy Poco CMake files
include(PocoMacros)

# Allow enabling and disabling components
option(POCO_ENABLE_CPPUNIT "Enable CppUnit" ON)
option(POCO_ENABLE_ENCODINGS "Enable Encodings" ON)
option(POCO_ENABLE_ENCODINGS_COMPILER "Enable Encodings Compiler" OFF)
option(POCO_ENABLE_XML "Enable XML" ON)
option(POCO_ENABLE_JSON "Enable JSON" ON)
option(POCO_ENABLE_MONGODB "Enable MongoDB" ON)
option(POCO_ENABLE_PDF "Enable PDF" OFF)
option(POCO_ENABLE_UTIL "Enable Util" ON)
option(POCO_ENABLE_NET "Enable Net" ON)

option(POCO_ENABLE_WSTRING "Enable std::wstring support" ON)
option(POCO_ENABLE_FPENVIRONMENT "Enable floating-point support" ON)

if(POCO_STATIC)
    message(DEPRECATION "POCO_STATIC is deprecated and will be removed! Use BUILD_SHARED_LIBS instead")
    option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
else()
    option(BUILD_SHARED_LIBS "Build shared libraries" ON)
endif()

# allow disabling of internally built OpenSSL# (see below for details)
# if POCO pre-built OpenSSL directory is found, and POCO_DISABLE_INTERNAL_OPENSSL=OFF,
# the internal OpenSSL build will be used
option(POCO_DISABLE_INTERNAL_OPENSSL "Disable internal OpensSSL binaries use" OFF)

if(MSVC AND (EXISTS "${PROJECT_SOURCE_DIR}/openssl/build/") AND NOT POCO_DISABLE_INTERNAL_OPENSSL)
    set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/openssl/build/")
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(_MSVC_ARCH "64")
    else()
        set(_MSVC_ARCH "32")
    endif()
    if(BUILD_SHARED_LIBS)
	set(_LIB_DIR "bin")
    else()
	set(OPENSSL_MSVC_STATIC_RT TRUE)
	set(_LIB_DIR "lib")
	endif()
	set(INTERNAL_OPENSSL_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/openssl/build/win${_MSVC_ARCH}/${_LIB_DIR}")
	set(INTERNAL_OPENSSL_LIBRARIES "${INTERNAL_OPENSSL_LIBRARY_DIR}/debug/")
	list(APPEND INTERNAL_OPENSSL_LIBRARIES "${INTERNAL_OPENSSL_LIBRARY_DIR}/release/")
	foreach(_OSSL_PATH ${INTERNAL_OPENSSL_LIBRARIES})
		string(REPLACE "/" "\\" _OSSL_PATH ${_OSSL_PATH})
		string(APPEND INTERNAL_OPENSSL_PATH ${_OSSL_PATH} "\;")
	endforeach()
	list(APPEND OPENSSL_ROOT_DIR ${INTERNAL_OPENSSL_LIBRARIES})
endif(MSVC AND (EXISTS "${PROJECT_SOURCE_DIR}/openssl/build/") AND NOT POCO_DISABLE_INTERNAL_OPENSSL)

if(POCO_ENABLE_NETSSL OR POCO_ENABLE_CRYPTO)
    find_package(OpenSSL REQUIRED)
else()
    find_package(OpenSSL QUIET)
endif(POCO_ENABLE_NETSSL OR POCO_ENABLE_CRYPTO)

if(OPENSSL_FOUND)
  message(STATUS "Found OpenSSL")
  message(STATUS "Enable NetSSL and Crypto support")
  option(POCO_ENABLE_NETSSL "Enable NetSSL" ON)
  option(POCO_ENABLE_CRYPTO "Enable Crypto" ON)
else()
  message(STATUS "OpenSSL not found")
  message(STATUS "Disable NetSSL and Crypto support")
  option(POCO_ENABLE_NETSSL "Enable NetSSL" OFF)
  option(POCO_ENABLE_CRYPTO "Enable Crypto" OFF)
endif()

if(WIN32)
    option(POCO_ENABLE_NETSSL_WIN "NetSSL Windows" ON)
else()
    option(POCO_ENABLE_NETSSL_WIN "NetSSL Windows" OFF)
endif()
option(POCO_ENABLE_SQL "Enable SQL" ON)
option(POCO_ENABLE_SQL_SQLITE "Enable SQL SQLite" ON)
option(POCO_ENABLE_SQL_MYSQL "Enable SQL MySQL" ON)
option(POCO_ENABLE_SQL_POSTGRESQL "Enable SQL PosgreSQL" ON)
option(POCO_ENABLE_SQL_ODBC "Enable SQL ODBC" ON)
option(POCO_ENABLE_SEVENZIP "Enable SevenZip" OFF)
option(POCO_ENABLE_ZIP "Enable Zip" ON)
option(POCO_ENABLE_APACHECONNECTOR "Enable ApacheConnector" OFF)
option(POCO_ENABLE_CPPPARSER "Enable C++ parser" OFF)
option(POCO_ENABLE_POCODOC "Enable Poco Documentation Generator" OFF)
option(POCO_ENABLE_PAGECOMPILER "Enable PageCompiler" ON)
option(POCO_ENABLE_PAGECOMPILER_FILE2PAGE "Enable File2Page" ON)
option(POCO_ENABLE_REDIS "Enable Redis" ON)

option(POCO_ENABLE_TESTS
  "Set to OFF|ON (default is OFF) to control build of POCO tests" OFF)

option(POCO_ENABLE_SAMPLES
  "Set to OFF|ON (default is OFF) to control build of POCO samples" OFF)

option(POCO_UNBUNDLED
  "Set to OFF|ON (default is OFF) to control linking dependencies as external" OFF)

if(MSVC)
    option(POCO_MT
      "Set to OFF|ON (default is OFF) to control build of POCO as /MT instead of /MD" OFF)

    option(POCO_ENABLE_MSVC_MP
      "Set to OFF|ON (default is OFF) to control parallel build of POCO with MSVC" OFF)
endif()

# Uncomment from next two lines to force static or dynamic library, default is autodetection
if(BUILD_SHARED_LIBS)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "Building dynamic libraries")
    endif()
else(BUILD_SHARED_LIBS)
    set(LIB_MODE_DEFINITIONS -DPOCO_STATIC)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "Building static libraries")
    endif()
endif(BUILD_SHARED_LIBS)

if (POCO_ENABLE_TESTS)
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

if (POCO_UNBUNDLED)
    if(POCO_VERBOSE_MESSAGES)
        message(STATUS "Build with using external sqlite, libz, pcre, expat ...")
    endif()
elseif (POCO_VERBOSE_MESSAGES)
    message(STATUS "Build with using internal copy of sqlite, libz, pcre, expat, ...")
endif ()

include(DefinePlatformSpecifc)

# Collect the built libraries and include dirs, the will be used to create the PocoConfig.cmake file
set(Poco_COMPONENTS "")

# Pthreads/threads support
find_package(Threads REQUIRED)

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
if(POCO_ENABLE_MONGODB)
    add_subdirectory(MongoDB)
    list(APPEND Poco_COMPONENTS "MongoDB")
endif()
if(POCO_ENABLE_PDF)
    add_subdirectory(PDF)
    list(APPEND Poco_COMPONENTS "PDF")
endif()
if(POCO_ENABLE_UTIL)
    add_subdirectory(Util)
    list(APPEND Poco_COMPONENTS "Util")
endif()
if(POCO_ENABLE_NET)
    add_subdirectory(Net)
    list(APPEND Poco_COMPONENTS "Net")
endif()

#NetSSL


if(WIN32 AND POCO_ENABLE_NETSSL_WIN)
    add_subdirectory(NetSSL_Win)
    list(APPEND Poco_COMPONENTS "NetSSL_Win")
endif(WIN32 AND POCO_ENABLE_NETSSL_WIN)

if(OPENSSL_FOUND)
    if(MSVC)
        list(APPEND OPENSSL_LIBRARIES ws2_32 Crypt32)
    endif()
    include_directories("${OPENSSL_INCLUDE_DIR}")
    if(POCO_ENABLE_NETSSL)
      add_subdirectory(NetSSL_OpenSSL)
      list(APPEND Poco_COMPONENTS "NetSSL_OpenSSL")
    endif()
    if(POCO_ENABLE_CRYPTO)
      add_subdirectory(Crypto)
      list(APPEND Poco_COMPONENTS "Crypto")
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
if(POCO_ENABLE_REDIS)
    add_subdirectory(Redis)
    list(APPEND Poco_COMPONENTS "Redis")
endif()

find_package(APR)
find_package(Apache2)
if(APRUTIL_FOUND AND APACHE_FOUND)
    include_directories( "${APACHE_INCLUDE_DIR}" "${APRUTIL_INCLUDE_DIR}" )
    if(POCO_ENABLE_APACHECONNECTOR)
    add_subdirectory(ApacheConnector)
    list(APPEND Poco_COMPONENTS "ApacheConnector")
    endif()
endif(APRUTIL_FOUND AND APACHE_FOUND)

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

# in tree build settings
#configure_file(PocoBuildTreeSettings.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PocoBuildTreeSettings.cmake @ONLY)

if(POCO_VERBOSE_MESSAGES)
    message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
    message(STATUS "Installation target path: ${CMAKE_INSTALL_PREFIX}")

    message(STATUS "")
    message(STATUS "Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")
    message(STATUS "Setting Poco build type - ${CMAKE_BUILD_TYPE}")
    message(STATUS "")

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
endif()
