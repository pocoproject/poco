set(LIBNAME "PocoZip")

set(SRCS
    src/Add.cpp
    src/AutoDetectStream.cpp
    src/Compress.cpp
    src/Decompress.cpp
    src/Delete.cpp
    src/Keep.cpp
    src/ParseCallback.cpp
    src/PartialStream.cpp
    src/Rename.cpp
    src/Replace.cpp
    src/SkipCallback.cpp
    src/ZipArchive.cpp
    src/ZipArchiveInfo.cpp
    src/ZipCommon.cpp
    src/ZipDataInfo.cpp
    src/ZipException.cpp
    src/ZipFileInfo.cpp
    src/ZipLocalFileHeader.cpp
    src/ZipManipulator.cpp
    src/ZipOperation.cpp
    src/ZipStream.cpp
    src/ZipUtil.cpp
)

if (NOT POCO_STATIC)
  add_definitions(-DZip_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoUtil PocoXML PocoFoundation)

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

