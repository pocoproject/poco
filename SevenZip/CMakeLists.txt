set(LIBNAME "PocoSevenZip")

set(SRCS
    src/Archive.cpp
    src/ArchiveEntry.cpp
)

# TODO: Currently only bundled is supported, in future this should also be possible
# with an unbundled version of 7z
list(APPEND SRCS
    src/7zAlloc.c
    src/7zBuf2.c
    src/7zBuf.c
    src/7zCrc.c
    src/7zCrcOpt.c
    src/7zDec.c
    src/7zFile.c
    src/7zIn.c
    src/7zStream.c
#    src/Alloc.c
    src/Bcj2.c
    src/Bra86.c
    src/Bra.c
#    src/BraIA64.c
    src/CpuArch.c
#    src/Delta.c
#    src/LzFind.c
#    src/LzFindMt.c
    src/Lzma2Dec.c
#    src/Lzma2Enc.c
    src/Lzma86Dec.c
#    src/Lzma86Enc.c
    src/LzmaDec.c
#    src/LzmaEnc.c
#    src/LzmaLib.c
#    src/MtCoder.c
    src/Ppmd7.c
    src/Ppmd7Dec.c
#    src/Ppmd7Enc.c
#    src/Sha256.c
#    src/Threads.c
#    src/Xz.c
#    src/XzCrc64.c
#    src/XzDec.c
#    src/XzEnc.c
#    src/XzIn.c
)

include_directories( "include" )

if (NOT POCO_STATIC)
  add_definitions(-DSevenZip_EXPORTS)
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
  # TODO: Enable CMake for samples
  #add_subdirectory(samples)
  # TODO: Add tests
  #add_subdirectory(testsuite)
endif ()

