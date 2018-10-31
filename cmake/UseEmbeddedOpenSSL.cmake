
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/openssl/build")
  message(STATUS "Use internal OpenSSL binaries but ${PROJECT_SOURCE_DIR}/openssl/build doesn't exists. Try to checkout ...")
  find_package(Git REQUIRED)
  execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote openssl 
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} 
      RESULT_VARIABLE EXITCODE)
  if(NOT "${EXITCODE}" STREQUAL "0")
    message(FATAL_ERROR "Checkout ${PROJECT_SOURCE_DIR}/openssl FAILED!")
  endif()
endif()

if(POCO_VERBOSE_MESSAGES)
  message(STATUS "Use internal OpenSSL binaries from ${PROJECT_SOURCE_DIR}/openssl/build")
endif()

if( "${CMAKE_SIZEOF_VOID_P}" STREQUAL "8" )
    set(_OPENSSL_MSVC_ARCH_SUFFIX "64")
else()
    set(_OPENSSL_MSVC_ARCH_SUFFIX "32")
endif()

set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/openssl/build/")
if(OPENSSL_USE_STATIC_LIBS)
  list(APPEND CMAKE_LIBRARY_PATH 
          "${OPENSSL_ROOT_DIR}/win${_OPENSSL_MSVC_ARCH_SUFFIX}/lib/debug"
          "${OPENSSL_ROOT_DIR}/win${_OPENSSL_MSVC_ARCH_SUFFIX}/lib/release")
else()
  list(APPEND CMAKE_LIBRARY_PATH 
          "${OPENSSL_ROOT_DIR}/win${_OPENSSL_MSVC_ARCH_SUFFIX}/bin/debug"
          "${OPENSSL_ROOT_DIR}/win${_OPENSSL_MSVC_ARCH_SUFFIX}/bin/release")
endif()
