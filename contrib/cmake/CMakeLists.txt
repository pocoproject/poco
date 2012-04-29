PROJECT (FTPTest)

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin CACHE PATH
   "Single output directory for building all executables.")

SET( TARGET_BASE_NAME "${PROJECT_NAME}" )
SET( LIB_NAME "${TARGET_BASE_NAME}Lib" )

#-----------------------------------------------------------------------------
# Let's use the highest warning level.
#-----------------------------------------------------------------------------
IF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
  # Use the highest warning level for visual studio.
  IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
  IF(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  ELSE(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  ENDIF(CMAKE_C_FLAGS MATCHES "/W[0-4]")
  # Disable deprecation warnings for standard C functions in VS2005 and later
  IF(CMAKE_COMPILER_2005)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
  ENDIF(CMAKE_COMPILER_2005)
ENDIF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
IF(CMAKE_BUILD_TOOL MATCHES "make")
  IF(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  ENDIF(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
  IF(NOT CMAKE_C_FLAGS MATCHES "-Wall")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  ENDIF(NOT CMAKE_C_FLAGS MATCHES "-Wall")
ENDIF(CMAKE_BUILD_TOOL MATCHES "make")

SET(Poco_DIR ${PROJECT_SOURCE_DIR})
FIND_PACKAGE(Poco REQUIRED)
IF(${Poco_FOUND})
  INCLUDE_DIRECTORIES(${Poco_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Poco_LIBRARY_DIRS})
ENDIF(WIN32)
ENDIF(${Poco_FOUND})

#-----------------------------------------------------------------------------
SET( EXE_NAME "${TARGET_BASE_NAME}" )
# Add any source files here.
SET( EXE_SRCS
 "My File.cpp"
)
# Add any include files here.
SET( EXE_INCS
  "My File.h"
)

#-----------------------------------------------------------------------------
# The executable.
ADD_EXECUTABLE(${EXE_NAME} ${EXE_SRCS} ${EXE_INCS})

TARGET_LINK_LIBRARIES ( ${EXE_NAME}
  "optimized" CppUnit
  "debug" CppUnitd
  "optimized" PocoFoundation
  "debug" PocoFoundationd
  "optimized" PocoNet
  "debug" PocoNetd
  "optimized" PocoUtil
  "debug" PocoUtild
  "optimized" PocoXML
  "debug" PocoXMLd
)

# Copy the DLLS to where the executable is.
# Only do this if you haven't set your path to the Poco DLLs.
IF(WIN32)
 SET ( DLL_FILES_DEBUG
   ${Poco_BINARY_DIRS}/CppUnitd.dll
   ${Poco_BINARY_DIRS}/PocoFoundationd.dll
   ${Poco_BINARY_DIRS}/PocoNetd.dll
   ${Poco_BINARY_DIRS}/PocoUtild.dll
   ${Poco_BINARY_DIRS}/PocoXMLd.dll
  )

  SET ( DLL_FILES_RELEASE
   ${Poco_BINARY_DIRS}/CppUnit.dll
   ${Poco_BINARY_DIRS}/PocoFoundation.dll
   ${Poco_BINARY_DIRS}/PocoNet.dll
   ${Poco_BINARY_DIRS}/PocoUtil.dll
   ${Poco_BINARY_DIRS}/PocoXML.dll
  )

  # Copy these files to the build tree.
  ADD_CUSTOM_TARGET(CopyDll ALL echo "Copying dlls ...")

  FOREACH(file ${DLL_FILES_DEBUG})
    GET_FILENAME_COMPONENT(fn ${file} NAME)
    SET(tgt ${EXECUTABLE_OUTPUT_PATH}/Debug/${fn})
    SET(src ${file})
    ADD_CUSTOM_COMMAND(
      TARGET CopyDll
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy_if_different ${src} ${tgt}
      COMMENT "Source copy of dlls"
    )
  ENDFOREACH(file)

  FOREACH(file ${DLL_FILES_RELEASE})
    GET_FILENAME_COMPONENT(fn ${file} NAME)
    SET(tgt ${EXECUTABLE_OUTPUT_PATH}/Release/${fn})
    SET(src ${file})
    ADD_CUSTOM_COMMAND(
      TARGET CopyDll
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy_if_different ${src} ${tgt}
      COMMENT "Source copy of dlls"
    )
  ENDFOREACH(file)

ENDIF(WIN32)

