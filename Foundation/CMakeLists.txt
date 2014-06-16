set(LIBNAME "PocoFoundation")

if (WIN32)
  # cmake has CMAKE_RC_COMPILER, but no message compiler
  if ("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
    # this path is only present for 2008+, but we currently require PATH to
    # be set up anyway
    get_filename_component(sdk_dir "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows;CurrentInstallFolder]" REALPATH)
    get_filename_component(kit_dir "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot]" REALPATH)
    if (X64)
      set(sdk_bindir "${sdk_dir}/bin/x64")
      set(kit_bindir "${kit_dir}/bin/x64")
    else (X64)
      set(sdk_bindir "${sdk_dir}/bin")
      set(kit_bindir "${kit_dir}/bin/x86")
    endif (X64)
  endif ()
  find_program(CMAKE_MC_COMPILER mc.exe HINTS "${sdk_bindir}" "${kit_bindir}"
    DOC "path to message compiler")
  if (NOT CMAKE_MC_COMPILER)
    message(FATAL_ERROR "message compiler not found: required to build")
  endif (NOT CMAKE_MC_COMPILER)
  message(STATUS "Found message compiler: ${CMAKE_MC_COMPILER}")
  mark_as_advanced(CMAKE_MC_COMPILER)
endif(WIN32)
  
set( BASE_SRCS 
  src/ASCIIEncoding.cpp
  src/Ascii.cpp
  src/AtomicCounter.cpp
  src/AbstractObserver.cpp
  src/ActiveDispatcher.cpp
  src/ArchiveStrategy.cpp
  src/AsyncChannel.cpp
  src/Base64Decoder.cpp
  src/Base64Encoder.cpp
  src/Base32Decoder.cpp
  src/Base32Encoder.cpp
  src/BinaryReader.cpp
  src/BinaryWriter.cpp
  src/Bugcheck.cpp
  src/ByteOrder.cpp
  src/Channel.cpp
  src/Checksum.cpp
  src/Clock.cpp
  src/Condition.cpp
  src/Configurable.cpp
  src/ConsoleChannel.cpp
  src/CountingStream.cpp
  src/DateTime.cpp
  src/DateTimeFormat.cpp
  src/DateTimeFormatter.cpp
  src/DateTimeParser.cpp
  src/Debugger.cpp
  src/DeflatingStream.cpp
  src/DigestEngine.cpp
  src/DigestStream.cpp
  src/DirectoryIterator.cpp
  src/DirectoryIteratorStrategy.cpp
  src/DirectoryWatcher.cpp
  src/Environment.cpp
  src/Error.cpp
  src/ErrorHandler.cpp
  src/Event.cpp
  src/EventArgs.cpp
  src/Exception.cpp
  src/FPEnvironment.cpp
  src/File.cpp
  src/FIFOBufferStream.cpp
  src/FileChannel.cpp
  src/FileStream.cpp
  src/FileStreamFactory.cpp
  src/Format.cpp
  src/Formatter.cpp
  src/FormattingChannel.cpp
  src/Glob.cpp
  src/Hash.cpp
  src/HashStatistic.cpp
  src/HexBinaryDecoder.cpp
  src/HexBinaryEncoder.cpp
  src/InflatingStream.cpp
  src/Latin1Encoding.cpp
  src/Latin2Encoding.cpp
  src/Latin9Encoding.cpp
  src/LineEndingConverter.cpp
  src/LocalDateTime.cpp
  src/LogFile.cpp
  src/LogStream.cpp
  src/Logger.cpp
  src/LoggingFactory.cpp
  src/LoggingRegistry.cpp
  src/MD4Engine.cpp
  src/MD5Engine.cpp
  src/Manifest.cpp
  src/MemoryPool.cpp
  src/MemoryStream.cpp
  src/Message.cpp
  src/Mutex.cpp
  src/NamedEvent.cpp
  src/NamedMutex.cpp
  src/NestedDiagnosticContext.cpp
  src/Notification.cpp
  src/NotificationCenter.cpp
  src/NotificationQueue.cpp
  src/TimedNotificationQueue.cpp
  src/PriorityNotificationQueue.cpp
  src/NullChannel.cpp
  src/NullStream.cpp
  src/NumberFormatter.cpp
  src/NumberParser.cpp
#  src/OpcomChannel.cpp  
  src/Path.cpp
  src/PatternFormatter.cpp
  src/Pipe.cpp
  src/PipeImpl.cpp
  src/PipeStream.cpp
  src/Process.cpp
  src/PurgeStrategy.cpp
  src/RWLock.cpp
  src/Random.cpp
  src/RandomStream.cpp
  src/RefCountedObject.cpp
  src/RegularExpression.cpp
  src/RotateStrategy.cpp
  src/Runnable.cpp
  src/SHA1Engine.cpp
  src/Semaphore.cpp
  src/SharedLibrary.cpp
  src/SharedMemory.cpp
  src/SignalHandler.cpp
  src/SimpleFileChannel.cpp
  src/SortedDirectoryIterator.cpp
  src/SplitterChannel.cpp
  src/Stopwatch.cpp
  src/StreamChannel.cpp
  src/StreamConverter.cpp
  src/StreamCopier.cpp
  src/StreamTokenizer.cpp
  src/String.cpp
  src/NumericString.cpp
  src/StringTokenizer.cpp
  src/SynchronizedObject.cpp
  src/Task.cpp
  src/TaskManager.cpp
  src/TaskNotification.cpp
  src/TeeStream.cpp
  src/TemporaryFile.cpp
  src/TextConverter.cpp
  src/TextEncoding.cpp
  src/TextIterator.cpp
  src/TextBufferIterator.cpp
  src/Thread.cpp
  src/ThreadTarget.cpp
  src/ThreadLocal.cpp
  src/ThreadPool.cpp
  src/Timer.cpp
  src/Timespan.cpp
  src/Timestamp.cpp
  src/Timezone.cpp
  src/Token.cpp
  src/URI.cpp
  src/URIStreamFactory.cpp
  src/URIStreamOpener.cpp
  src/UTF16Encoding.cpp
  src/UTF32Encoding.cpp
  src/UTF8Encoding.cpp
  src/UTF8String.cpp
  src/UUID.cpp
  src/UUIDGenerator.cpp
  src/Unicode.cpp
  src/UnicodeConverter.cpp
  src/Var.cpp
  src/VarHolder.cpp
  src/VarIterator.cpp
  src/Void.cpp
  src/Windows1250Encoding.cpp
  src/Windows1251Encoding.cpp
  src/Windows1252Encoding.cpp
  src/adler32.c
  src/compress.c
  src/crc32.c
  src/deflate.c
  src/infback.c
  src/inffast.c
  src/inflate.c
  src/inftrees.c
  src/pcre_chartables.c
  src/pcre_compile.c
  src/pcre_exec.c
  src/pcre_fullinfo.c
  src/pcre_globals.c
  src/pcre_maketables.c
  src/pcre_newline.c
  src/pcre_ord2utf8.c
  src/pcre_study.c
  src/pcre_tables.c
  src/pcre_try_flipped.c
  src/pcre_ucd.c
  src/pcre_valid_utf8.c
  src/pcre_xclass.c
  src/pocomsg.mc
  src/trees.c
  src/zutil.c
)

set(WIN_SRCS
  src/EventLogChannel.cpp
  src/WindowsConsoleChannel.cpp
)

if (WIN32)
  set(RES_SRCS ${CMAKE_SOURCE_DIR}/Foundation/src/pocomsg.h)
  set_source_files_properties(${RES_SRCS} PROPERTIES GENERATED true)
  add_custom_command(
    OUTPUT ${RES_SRCS}
    DEPENDS ${CMAKE_SOURCE_DIR}/Foundation/src/pocomsg.mc
    COMMAND ${CMAKE_MC_COMPILER}
    ARGS 
         -h ${CMAKE_SOURCE_DIR}/Foundation/src
         -r ${CMAKE_SOURCE_DIR}/Foundation/src
         ${CMAKE_SOURCE_DIR}/Foundation/src/pocomsg.mc
    VERBATIM # recommended: p260
  )
  list(APPEND WIN_SRCS ${RES_SRCS})
endif (WIN32)

set(LIN_SRCS
  src/SyslogChannel.cpp
)

if(CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${WIN_SRCS})
 add_definitions( -DPCRE_STATIC -DFoundation_EXPORTS)
 set(SYSLIBS ${SYSLIBS} iphlpapi)
else (CMAKE_SYSTEM MATCHES "Windows")
 set(SRCS ${BASE_SRCS} ${LIN_SRCS})
 add_definitions( -DPCRE_STATIC)
endif(CMAKE_SYSTEM MATCHES "Windows")

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} PROPERTIES VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
if (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
 set_target_properties( ${LIBNAME} PROPERTIES LINK_FLAGS "-library=stlport4")
endif (${CMAKE_CXX_COMPILER_ID} MATCHES "SunPro")
target_link_libraries( ${LIBNAME} ${SYSLIBS})

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
    add_subdirectory( samples )
    add_subdirectory( testsuite )
endif ()

