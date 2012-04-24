#
# Makefile
#
# $Id: //poco/1.4/Foundation/Makefile#4 $
#
# Makefile for Poco Foundation
#

include $(POCO_BASE)/build/rules/global

objects = ArchiveStrategy Ascii ASCIIEncoding AsyncChannel Base64Decoder Base64Encoder \
	BinaryReader BinaryWriter Bugcheck ByteOrder Channel Checksum Configurable ConsoleChannel \
	CountingStream DateTime LocalDateTime DateTimeFormat DateTimeFormatter DateTimeParser \
	Debugger DeflatingStream DigestEngine DigestStream DirectoryIterator \
	Environment Event EventArgs ErrorHandler Exception FPEnvironment File Glob \
	FileChannel Formatter FormattingChannel HexBinaryDecoder LineEndingConverter \
	HexBinaryEncoder InflatingStream Latin1Encoding Latin2Encoding Latin9Encoding LogFile \
	Logger LoggingFactory LoggingRegistry LogStream NamedEvent NamedMutex NullChannel \
	MemoryPool MD4Engine MD5Engine Manifest Message Mutex \
	NestedDiagnosticContext Notification NotificationCenter \
	NotificationQueue PriorityNotificationQueue TimedNotificationQueue \
	NullStream NumberFormatter NumberParser AbstractObserver \
	Path PatternFormatter Process PurgeStrategy RWLock Random RandomStream \
	RegularExpression RefCountedObject Runnable RotateStrategy Condition \
	SHA1Engine Semaphore SharedLibrary SimpleFileChannel \
	SignalHandler SplitterChannel Stopwatch StreamChannel StreamConverter StreamCopier \
	StreamTokenizer String StringTokenizer SynchronizedObject \
	Task TaskManager TaskNotification TeeStream Hash HashStatistic \
	TemporaryFile TextConverter TextEncoding TextIterator TextBufferIterator Thread ThreadLocal \
	ThreadPool ThreadTarget ActiveDispatcher Timer Timespan Timestamp Timezone Token URI \
	FileStreamFactory URIStreamFactory URIStreamOpener UTF16Encoding UTF8Encoding UTF8String \
	Unicode UnicodeConverter Windows1250Encoding Windows1251Encoding Windows1252Encoding \
	UUID UUIDGenerator Void Var VarHolder Format Pipe PipeImpl PipeStream SharedMemory \
	MemoryStream FileStream AtomicCounter 

zlib_objects = adler32 compress crc32 deflate \
	infback inffast inflate inftrees trees zutil

pcre_objects = pcre_chartables pcre_compile pcre_globals pcre_maketables \
	pcre_study pcre_try_flipped pcre_valid_utf8 \
	pcre_exec pcre_ord2utf8 pcre_newline pcre_fullinfo pcre_xclass

pcre_utf8_objects = pcre_ucd pcre_tables

ifdef POCO_UNBUNDLED
	objects += $(pcre_utf8_objects) 
	SYSLIBS += -lpcre -lz
else
	objects += $(zlib_objects) $(pcre_objects) $(pcre_utf8_objects)
endif

ifeq ($(findstring MinGW, $(POCO_CONFIG)), MinGW)
	objects += EventLogChannel WindowsConsoleChannel
else
	objects += SyslogChannel
endif

target         = PocoFoundation
target_version = $(LIBVERSION)
target_libs    =

include $(POCO_BASE)/build/rules/lib
