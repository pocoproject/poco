# warning: this must be defined before target, otherwise
# the link_directories() directive is ignored
if(MSVC AND USING_INTERNAL_OPENSSL)
  message(STATUS "${LIBNAME}: using internal OpenSSL from ${OPENSSL_ROOT_DIR}")

  if(POCO_STATIC)
    set(POCO_WIN_LIB_DIR "lib")
  else(POCO_STATIC)
    set(POCO_WIN_LIB_DIR "bin")
  endif(POCO_STATIC)

  set(WIN_LIB_OUTPUT_DIR "")
  STRING(REGEX REPLACE "/" "\\\\" WIN_LIB_OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  STRING(REGEX REPLACE "/" "\\\\" WIN_BIN_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  link_directories(${WIN_LIB_OUTPUT_DIR}\\$(Configuration)\\)

  if(CMAKE_CL_64)
    set(POCO_WIN_PLATFORM_BITS "64")
    set(OPENSSL_WIN_PLATFORM_BITS "64")
  else(CMAKE_CL_64)
    set(POCO_WIN_PLATFORM_BITS "")
    set(OPENSSL_WIN_PLATFORM_BITS "32")
  endif(CMAKE_CL_64)

endif(MSVC AND USING_INTERNAL_OPENSSL)
