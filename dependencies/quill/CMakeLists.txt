cmake_minimum_required(VERSION 3.12...4.0)
project(quill)

#-------------------------------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------------------------------

option(QUILL_NO_EXCEPTIONS "Enable this option to build without exception handling support." OFF)

option(QUILL_NO_THREAD_NAME_SUPPORT "Enable this option to disable features that require thread name retrieval, ensuring compatibility with older Windows versions (e.g., Windows Server 2012/2016) and Android." OFF)

option(QUILL_X86ARCH "Enable x86-specific optimizations for cache coherence using _mm_prefetch, _mm_clflush, and _mm_clflushopt instructions. Ensure the target architecture must also be specified with -march=\"...\" when enabling this option." OFF)

option(QUILL_DISABLE_NON_PREFIXED_MACROS "Enable this option to disable non-prefixed `LOG_*` macros, keeping only the `QUILL_LOG_*` macros to avoid conflicts with other logging libraries." OFF)

option(QUILL_DISABLE_FUNCTION_NAME "Disable the use of __FUNCTION__ in `LOG_*` macros when the function name is not needed." OFF)

option(QUILL_DETAILED_FUNCTION_NAME "Use detailed function name (__PRETTY_FUNCTION__ or __FUNCSIG__) instead of __FUNCTION__ in LOG_* macros" OFF)

option(QUILL_DISABLE_FILE_INFO "Disable the use of __FILE__ and __LINE__ in `LOG_*` macros when the file name and the line number are not needed." OFF)

option(QUILL_ENABLE_ASSERTIONS "Enable assertions in release builds. When disabled, assertions are only active in debug builds." OFF)

option(QUILL_BUILD_EXAMPLES "Enable this option to build and install the examples. Set this to ON to include example projects in the build process and have them installed after configuring with CMake." OFF)

option(QUILL_BUILD_TESTS "Enable this option to build the test suite." OFF)

option(QUILL_ENABLE_EXTENSIVE_TESTS "Enable extensive tests that may require more resources and are not suitable for hosted CI runners." OFF)

option(QUILL_BUILD_BENCHMARKS "Enable this option to build the benchmarks." OFF)

option(QUILL_SANITIZE_ADDRESS "Enable AddressSanitizer (ASan) for memory error detection in tests." OFF)

option(QUILL_SANITIZE_THREAD "Enable ThreadSanitizer (TSan) for detecting thread-related issues in tests. Note: Using this option with non-Clang compilers may produce false positives." OFF)

option(QUILL_BUILD_FUZZING "Enable fuzzing with libFuzzer, AddressSanitizer, and UndefinedBehaviorSanitizer. Requires Clang compiler." OFF)

option(QUILL_CODE_COVERAGE "Enable code coverage analysis during the build." OFF)

option(QUILL_USE_VALGRIND "Use Valgrind as the default memory checking tool in CTest. Valgrind must be installed." OFF)

option(QUILL_ENABLE_INSTALL "Enable the CMake install target when Quill is not the master project." OFF)

option(QUILL_DOCS_GEN "Generate documentation during the build process." OFF)

# Internal option
set(QUILL_ENABLE_GCC_HARDENING OFF CACHE INTERNAL "")

#-------------------------------------------------------------------------------------------------------
# Determine if quill is built as a subproject (using add_subdirectory) or if it is the master project.
#-------------------------------------------------------------------------------------------------------
set(QUILL_MASTER_PROJECT FALSE CACHE BOOL "Master Project" FORCE)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(QUILL_MASTER_PROJECT TRUE CACHE BOOL "Master Project" FORCE)
endif ()

#-------------------------------------------------------------------------------------------------------
# Custom cmake functions
#-------------------------------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(QuillUtils)

#-------------------------------------------------------------------------------------------------------
# Resolve version
#-------------------------------------------------------------------------------------------------------
quill_extract_version()
project(quill VERSION ${QUILL_VERSION} LANGUAGES CXX)

#-------------------------------------------------------------------------------------------------------
# Set default build to release
#-------------------------------------------------------------------------------------------------------
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif ()

#---------------------------------------------------------------------------------------
# Compiler config
#---------------------------------------------------------------------------------------
if (NOT CMAKE_CXX_STANDARD)
    # If CMAKE_CXX_STANDARD is not set, configure it to 17 by default
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif (CMAKE_CXX_STANDARD LESS 17)
    # If CMAKE_CXX_STANDARD is set but less than 17, issue a fatal error
    message(FATAL_ERROR "Quill requires at least C++17. Please set CMAKE_CXX_STANDARD to 17 or higher.")
endif ()

#-------------------------------------------------------------------------------------------------------
# Required Packages
#-------------------------------------------------------------------------------------------------------
find_package(Threads REQUIRED)

if (QUILL_BUILD_TESTS)
    enable_testing()

    if (QUILL_USE_VALGRIND)
        # find valgrind
        find_program(MEMORYCHECK_COMMAND NAMES valgrind)
        if (NOT MEMORYCHECK_COMMAND)
            message(WARNING "Valgrind not found")
        endif ()

        # set valgrind params
        set(MEMORYCHECK_COMMAND_OPTIONS "--tool=memcheck --leak-check=full --leak-resolution=med --show-leak-kinds=all --track-origins=yes --vgdb=no --fair-sched=yes")

        # add memcheck test action to ctest
        include(CTest)
    endif ()
endif ()

# Error out if QUILL_ENABLE_EXTENSIVE_TESTS is ON but QUILL_BUILD_TESTS is OFF
if (QUILL_ENABLE_EXTENSIVE_TESTS AND NOT QUILL_BUILD_TESTS)
    message(FATAL_ERROR "QUILL_ENABLE_EXTENSIVE_TESTS requires QUILL_BUILD_TESTS to be enabled. Please enable QUILL_BUILD_TESTS.")
endif ()

#-------------------------------------------------------------------------------------------------------
# Log Info
#-------------------------------------------------------------------------------------------------------
if (QUILL_MASTER_PROJECT)
    option(QUILL_VERBOSE_MAKEFILE "Enable verbose output for makefiles when Quill is the master project. This provides detailed information about the build process." OFF)
    message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
    message(STATUS "QUILL_VERSION: ${QUILL_VERSION}")
endif ()

message(STATUS "QUILL_NO_EXCEPTIONS: " ${QUILL_NO_EXCEPTIONS})
message(STATUS "QUILL_NO_THREAD_NAME_SUPPORT: " ${QUILL_NO_THREAD_NAME_SUPPORT})
message(STATUS "QUILL_X86ARCH: " ${QUILL_X86ARCH})
message(STATUS "QUILL_DISABLE_NON_PREFIXED_MACROS: " ${QUILL_DISABLE_NON_PREFIXED_MACROS})
message(STATUS "QUILL_DISABLE_FUNCTION_NAME: " ${QUILL_DISABLE_FUNCTION_NAME})
message(STATUS "QUILL_DETAILED_FUNCTION_NAME: " ${QUILL_DETAILED_FUNCTION_NAME})
message(STATUS "QUILL_DISABLE_FILE_INFO: " ${QUILL_DISABLE_FILE_INFO})
message(STATUS "QUILL_ENABLE_ASSERTIONS: " ${QUILL_ENABLE_ASSERTIONS})
message(STATUS "QUILL_ENABLE_INSTALL: " ${QUILL_ENABLE_INSTALL})

#---------------------------------------------------------------------------------------
# Verbose make file option
#---------------------------------------------------------------------------------------
if (QUILL_VERBOSE_MAKEFILE)
    set(CMAKE_VERBOSE_MAKEFILE TRUE CACHE BOOL "Verbose output" FORCE)
endif ()

# address sanitizer flags
if (QUILL_SANITIZE_ADDRESS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer -g")
endif ()

# thread sanitizer flags
if (QUILL_SANITIZE_THREAD)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer -g")
endif ()

# Append extra options for coverage
if (QUILL_CODE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer -g -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

if (QUILL_BUILD_EXAMPLES)
    add_subdirectory(examples)
    add_subdirectory(docs/examples)
endif ()

if (QUILL_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif ()

if (QUILL_BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(test)
endif ()

if (QUILL_BUILD_FUZZING)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "QUILL_BUILD_FUZZING requires Clang compiler")
    endif ()
    
    add_subdirectory(fuzz)
endif ()

# library name
set(TARGET_NAME quill)

# header files
set(HEADER_FILES
        include/quill/backend/BackendManager.h
        include/quill/backend/BackendOptions.h
        include/quill/backend/BackendWorker.h
        include/quill/backend/BackendWorkerLock.h
        include/quill/backend/BacktraceStorage.h
        include/quill/backend/ManualBackendWorker.h
        include/quill/backend/PatternFormatter.h
        include/quill/backend/RdtscClock.h
        include/quill/backend/SignalHandler.h
        include/quill/backend/StringFromTime.h
        include/quill/backend/ThreadUtilities.h
        include/quill/backend/TimestampFormatter.h
        include/quill/backend/TransitEvent.h
        include/quill/backend/TransitEventBuffer.h
        include/quill/backend/BackendUtilities.h

        include/quill/bundled/fmt/args.h
        include/quill/bundled/fmt/base.h
        include/quill/bundled/fmt/chrono.h
        include/quill/bundled/fmt/color.h
        include/quill/bundled/fmt/compile.h
        include/quill/bundled/fmt/format.h
        include/quill/bundled/fmt/format-inl.h
        include/quill/bundled/fmt/os.h
        include/quill/bundled/fmt/ostream.h
        include/quill/bundled/fmt/printf.h
        include/quill/bundled/fmt/ranges.h
        include/quill/bundled/fmt/std.h
        include/quill/bundled/fmt/xchar.h

        include/quill/core/Attributes.h
        include/quill/core/BoundedSPSCQueue.h
        include/quill/core/ChronoTimeUtils.h
        include/quill/core/Common.h
        include/quill/core/DynamicFormatArgStore.h
        include/quill/core/Codec.h
        include/quill/core/Filesystem.h
        include/quill/core/FrontendOptions.h
        include/quill/core/InlinedVector.h
        include/quill/core/LoggerBase.h
        include/quill/core/LoggerManager.h
        include/quill/core/LogLevel.h
        include/quill/core/MacroMetadata.h
        include/quill/core/MathUtilities.h
        include/quill/core/PatternFormatterOptions.h
        include/quill/core/QuillError.h
        include/quill/core/Rdtsc.h
        include/quill/core/SinkManager.h
        include/quill/core/SourceLocation.h
        include/quill/core/Spinlock.h
        include/quill/core/ThreadContextManager.h
        include/quill/core/TimeUtilities.h
        include/quill/core/UnboundedSPSCQueue.h
        include/quill/backend/Utf8Conv.h

        include/quill/filters/Filter.h

        include/quill/sinks/AndroidSink.h
        include/quill/sinks/ConsoleSink.h
        include/quill/sinks/FileSink.h
        include/quill/sinks/JsonSink.h
        include/quill/sinks/NullSink.h
        include/quill/sinks/RotatingFileSink.h
        include/quill/sinks/RotatingJsonFileSink.h
        include/quill/sinks/RotatingSink.h
        include/quill/sinks/Sink.h
        include/quill/sinks/StreamSink.h
        include/quill/sinks/SyslogSink.h
        include/quill/sinks/SystemdSink.h

        include/quill/std/Array.h
        include/quill/std/Chrono.h
        include/quill/std/Deque.h
        include/quill/std/FilesystemPath.h
        include/quill/std/ForwardList.h
        include/quill/std/List.h
        include/quill/std/Map.h
        include/quill/std/Optional.h
        include/quill/std/Pair.h
        include/quill/std/Set.h
        include/quill/std/Tuple.h
        include/quill/std/UnorderedMap.h
        include/quill/std/UnorderedSet.h
        include/quill/std/Vector.h
        include/quill/std/WideString.h

        include/quill/Backend.h
        include/quill/BackendTscClock.h
        include/quill/BinaryDataDeferredFormatCodec.h
        include/quill/CsvWriter.h
        include/quill/DeferredFormatCodec.h
        include/quill/DirectFormatCodec.h
        include/quill/Frontend.h
        include/quill/HelperMacros.h
        include/quill/LogFunctions.h
        include/quill/Logger.h
        include/quill/LogMacros.h
        include/quill/SimpleSetup.h
        include/quill/StopWatch.h
        include/quill/StringRef.h
        include/quill/UserClockSource.h
        include/quill/Utility.h
)

# Add as a library
add_library(${TARGET_NAME} INTERFACE)
add_library(${TARGET_NAME}::${TARGET_NAME} ALIAS ${TARGET_NAME})

if (QUILL_NO_EXCEPTIONS)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_NO_EXCEPTIONS)
endif ()

if (QUILL_NO_THREAD_NAME_SUPPORT)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_NO_THREAD_NAME_SUPPORT)
endif ()

if (QUILL_X86ARCH)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_X86ARCH)
endif ()

if (QUILL_DISABLE_NON_PREFIXED_MACROS)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_DISABLE_NON_PREFIXED_MACROS)
endif ()

if (QUILL_DISABLE_FUNCTION_NAME)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_DISABLE_FUNCTION_NAME)
endif ()

if (QUILL_DETAILED_FUNCTION_NAME)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_DETAILED_FUNCTION_NAME)
endif ()

if (QUILL_DISABLE_FILE_INFO)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_DISABLE_FILE_INFO)
endif ()

if (QUILL_ENABLE_ASSERTIONS)
    target_compile_definitions(${TARGET_NAME} PUBLIC INTERFACE -DQUILL_ENABLE_ASSERTIONS)
endif ()

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.19)
    # cmake prior to 3.19 does not support target_sources for interface libraries
    target_sources(${TARGET_NAME} PRIVATE ${HEADER_FILES})
endif ()

# Link dependencies
target_link_libraries(${TARGET_NAME} PUBLIC INTERFACE Threads::Threads)

if (MINGW)
    # strftime requires this when using MinGw to correctly format the time ..
    target_link_libraries(${TARGET_NAME} PUBLIC INTERFACE ucrtbase)
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(${TARGET_NAME} PUBLIC INTERFACE stdc++fs)
endif ()

# Add include directories for this library
target_include_directories(${TARGET_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Compiler options
target_compile_options(${TARGET_NAME} INTERFACE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-gnu-zero-variadic-macro-arguments>)

# Install
if (QUILL_MASTER_PROJECT OR QUILL_ENABLE_INSTALL)
    # ---- Install ---- #
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(version_config ${PROJECT_BINARY_DIR}/quill-config-version.cmake)
    set(project_config ${PROJECT_BINARY_DIR}/quill-config.cmake)
    set(pkgconfig ${PROJECT_BINARY_DIR}/quill.pc)
    set(targets_export_name quill-targets)

    set(QUILL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/quill CACHE STRING
            "Installation directory for cmake files, relative to ${CMAKE_INSTALL_PREFIX}.")

    set(QUILL_LIB_DIR ${CMAKE_INSTALL_LIBDIR} CACHE STRING
            "Installation directory for libraries, relative to ${CMAKE_INSTALL_PREFIX}.")

    set(QUILL_INC_DIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE STRING
            "Installation directory for include files, relative to ${CMAKE_INSTALL_PREFIX}.")

    set(QUILL_PKGCONFIG_DIR ${CMAKE_INSTALL_LIBDIR}/pkgconfig CACHE PATH
            "Installation directory for pkgconfig (.pc) files, relative to ${CMAKE_INSTALL_PREFIX}.")

    if (WIN32)
        set(QUILL_PTHREAD_LIBS "")
    else ()
        set(QUILL_PTHREAD_LIBS "-lpthread")
    endif ()

    # Generate pkgconfig
    configure_file(
            "${CMAKE_CURRENT_LIST_DIR}/cmake/quill.pc.in"
            "${pkgconfig}"
            @ONLY)

    # Copy pkgconfig
    install(FILES "${pkgconfig}" DESTINATION "${QUILL_PKGCONFIG_DIR}")

    # Generate the version, config and target files into the build directory.
    write_basic_package_version_file(
            ${version_config}
            VERSION ${QUILL_VERSION}
            COMPATIBILITY AnyNewerVersion)

    configure_package_config_file(
            ${CMAKE_CURRENT_LIST_DIR}/cmake/quill-config.cmake.in
            ${project_config}
            INSTALL_DESTINATION ${QUILL_CMAKE_DIR})

    # Install version, config files
    install(FILES ${project_config} ${version_config}
            DESTINATION ${QUILL_CMAKE_DIR})

    # Install the headers
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/quill DESTINATION ${QUILL_INC_DIR})

    # Install the library
    install(TARGETS ${TARGET_NAME} EXPORT ${targets_export_name}
            LIBRARY DESTINATION ${QUILL_LIB_DIR}
            ARCHIVE DESTINATION ${QUILL_LIB_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # Export the library
    install(EXPORT ${targets_export_name} DESTINATION ${QUILL_CMAKE_DIR}
            NAMESPACE quill::)

    # ---- Packaging ---- #
    set(CPACK_GENERATOR ZIP)
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}" "${PROJECT_NAME}" ALL .)
    set(CPACK_PROJECT_URL "https://github.com/odygrd/quill")
    set(CPACK_PACKAGE_VENDOR "Odysseas Georgoudis")
    set(CPACK_PACKAGE_CONTACT "Odysseas Odysseas <odygrd@hotmail.com>")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Asynchronous Low Latency C++ Logging Library")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
    set(CPACK_PACKAGE_RELOCATABLE ON)
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "System Environment/Libraries")
    set(CPACK_RPM_PACKAGE_URL ${CPACK_PROJECT_URL})
    set(CPACK_RPM_PACKAGE_DESCRIPTION "Asynchronous Low Latency C++ Logging Library")
    include(CPack)
endif ()

if (QUILL_DOCS_GEN)
    if (NOT QUILL_BUILD_EXAMPLES)
        message(FATAL_ERROR "QUILL_DOCS_GEN requires QUILL_BUILD_EXAMPLES to be ON. Please enable QUILL_BUILD_EXAMPLES.")
    endif ()

    add_subdirectory(docs)
endif ()
