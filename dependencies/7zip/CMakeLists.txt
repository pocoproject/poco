

# Sources
#file(GLOB SRCS_G "src/*.c")
#POCO_SOURCES(SRCS 7zip ${SRCS_G})

# TODO: Currently only bundled is supported, in future this should also be possible
# with an unbundled version of 7z
POCO_SOURCES(SRCS 7zip
	src/7zAlloc.c
	src/7zBuf2.c
	src/7zBuf.c
	src/7zCrc.c
	src/7zCrcOpt.c
	src/7zDec.c
	src/7zFile.c
	src/7zIn.c
	src/7zStream.c
	src/Alloc.c
	src/Bcj2.c
	src/Bra86.c
	src/Bra.c
#	 src/BraIA64.c
	src/CpuArch.c
#	 src/Delta.c
#	 src/LzFind.c
#	 src/LzFindMt.c
	src/Lzma2Dec.c
#	 src/Lzma2Enc.c
	src/Lzma86Dec.c
#	 src/Lzma86Enc.c
	src/LzmaDec.c
#	 src/LzmaEnc.c
#	 src/LzmaLib.c
#	 src/MtCoder.c
	src/Ppmd7.c
	src/Ppmd7Dec.c
#	 src/Ppmd7Enc.c
#	 src/Sha256.c
#	 src/Threads.c
#	 src/Xz.c
#	 src/XzCrc64.c
#	 src/XzDec.c
#	 src/XzEnc.c
#	 src/XzIn.c
)

# Headers
file(GLOB_RECURSE HDRS_G "src/*.h")
POCO_HEADERS(SRCS 7zip ${HDRS_G})

add_library(_BUNDLED_7ZIP STATIC ${SRCS})

target_include_directories(_BUNDLED_7ZIP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

add_library(7Zip::7Zip ALIAS _BUNDLED_7ZIP)
