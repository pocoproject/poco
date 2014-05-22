set( TEST_SRCS
src/ActiveDispatcherTest.cpp
src/ActiveMethodTest.cpp
src/ActivityTest.cpp
src/AnyTest.cpp
src/ArrayTest.cpp
src/AutoPtrTest.cpp
src/AutoReleasePoolTest.cpp
src/Base32Test.cpp
src/Base64Test.cpp
src/BasicEventTest.cpp
src/BinaryReaderWriterTest.cpp
src/ByteOrderTest.cpp
src/CacheTestSuite.cpp
src/ChannelTest.cpp
src/ClassLoaderTest.cpp
src/ClockTest.cpp
src/ConditionTest.cpp
src/CoreTest.cpp
src/CoreTestSuite.cpp
src/CountingStreamTest.cpp
src/CryptTestSuite.cpp
src/DateTimeFormatterTest.cpp
src/DateTimeParserTest.cpp
src/DateTimeTest.cpp
src/DateTimeTestSuite.cpp
src/DigestStreamTest.cpp
src/DirectoryIteratorsTest.cpp
src/DirectoryWatcherTest.cpp
src/Driver.cpp
src/DummyDelegate.cpp
src/DynamicFactoryTest.cpp
src/DynamicTestSuite.cpp
src/EventTestSuite.cpp
src/ExpireCacheTest.cpp
src/ExpireLRUCacheTest.cpp
src/FIFOEventTest.cpp
src/FIFOBufferStreamTest.cpp
src/FPETest.cpp
src/FileChannelTest.cpp
src/FileStreamTest.cpp
src/FileTest.cpp
src/FilesystemTestSuite.cpp
src/FormatTest.cpp
src/FoundationTestSuite.cpp
src/GlobTest.cpp
src/HMACEngineTest.cpp
src/HashMapTest.cpp
src/HashSetTest.cpp
src/HashTableTest.cpp
src/HashingTestSuite.cpp
src/HexBinaryTest.cpp
src/LRUCacheTest.cpp
src/LineEndingConverterTest.cpp
src/LinearHashTableTest.cpp
src/ListMapTest.cpp
src/LocalDateTimeTest.cpp
src/LogStreamTest.cpp
src/LoggerTest.cpp
src/LoggingFactoryTest.cpp
src/LoggingRegistryTest.cpp
src/LoggingTestSuite.cpp
src/MD4EngineTest.cpp
src/MD5EngineTest.cpp
src/ManifestTest.cpp
src/MemoryPoolTest.cpp
src/MemoryStreamTest.cpp
src/NDCTest.cpp
src/NamedEventTest.cpp
src/NamedMutexTest.cpp
src/NamedTuplesTest.cpp
src/NotificationCenterTest.cpp
src/NotificationQueueTest.cpp
src/ObjectPoolTest.cpp
src/PriorityNotificationQueueTest.cpp
src/TimedNotificationQueueTest.cpp
src/NotificationsTestSuite.cpp
src/NullStreamTest.cpp
src/NumberFormatterTest.cpp
src/NumberParserTest.cpp
src/PathTest.cpp
src/PatternFormatterTest.cpp
src/PBKDF2EngineTest.cpp
src/PriorityEventTest.cpp
src/ProcessTest.cpp
src/ProcessesTestSuite.cpp
src/RWLockTest.cpp
src/RandomStreamTest.cpp
src/RandomTest.cpp
src/RegularExpressionTest.cpp
src/SHA1EngineTest.cpp
src/SemaphoreTest.cpp
src/SharedLibraryTest.cpp
src/SharedLibraryTestSuite.cpp
src/SharedMemoryTest.cpp
src/SharedPtrTest.cpp
src/SimpleFileChannelTest.cpp
src/SimpleHashTableTest.cpp
src/StopwatchTest.cpp
src/StreamConverterTest.cpp
src/StreamCopierTest.cpp
src/StreamTokenizerTest.cpp
src/StreamsTestSuite.cpp
src/StringTest.cpp
src/StringTokenizerTest.cpp
src/TaskManagerTest.cpp
src/TaskTest.cpp
src/TaskTestSuite.cpp
src/TeeStreamTest.cpp
src/TestChannel.cpp
src/TestPlugin.cpp
src/TextConverterTest.cpp
src/TextEncodingTest.cpp
src/TextIteratorTest.cpp
src/TextBufferIteratorTest.cpp
src/TextTestSuite.cpp
src/ThreadLocalTest.cpp
src/ThreadPoolTest.cpp
src/ThreadTest.cpp
src/ThreadingTestSuite.cpp
src/TimerTest.cpp
src/TimespanTest.cpp
src/TimestampTest.cpp
src/TimezoneTest.cpp
src/TuplesTest.cpp
src/TypeListTest.cpp
src/URIStreamOpenerTest.cpp
src/URITest.cpp
src/URITestSuite.cpp
src/UTF8StringTest.cpp
src/UUIDGeneratorTest.cpp
src/UUIDTest.cpp
src/UUIDTestSuite.cpp
src/UniqueExpireCacheTest.cpp
src/UnicodeConverterTest.cpp
src/UniqueExpireLRUCacheTest.cpp
src/VarTest.cpp
src/ZLibTest.cpp
)

set( WIN_TEST_SRCS
src/WinDriver.cpp
)

set(TESTUNIT "${LIBNAME}-testrunner")

add_executable( ${TESTUNIT} ${TEST_SRCS} )
#set_target_properties( ${TESTUNIT} PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( ${TESTUNIT} PocoFoundation CppUnit )
if (NOT ANDROID)
  target_link_libraries( ${TESTUNIT} pthread)
endif ()

add_executable( TestApp src/TestApp.cpp )
#set_target_properties( TestApp PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( TestApp PocoFoundation )

add_library( TestLibrary SHARED src/TestApp.cpp )
#set_target_properties( TestLibrary PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS} )
target_link_libraries( TestLibrary PocoFoundation )

