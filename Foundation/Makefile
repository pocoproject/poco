#
# Makefile
#
# $Id: //poco/svn/Foundation/Makefile#2 $
#
# Makefile for Poco Foundation
#

include $(POCO_BASE)/build/rules/global

objects = ArchiveStrategy ASCIIEncoding AsyncChannel Base64Decoder Base64Encoder \
	BinaryReader BinaryWriter Bugcheck ByteOrder Channel Checksum Configurable ConsoleChannel \
	CountingStream DateTime LocalDateTime DateTimeFormat DateTimeFormatter DateTimeParser \
	Debugger DeflatingStream DigestEngine DigestStream DirectoryIterator \
	Environment Event EventArgs ErrorHandler Exception FPEnvironment File Glob \
	FileChannel Formatter FormattingChannel HexBinaryDecoder LineEndingConverter \
	HexBinaryEncoder InflatingStream Latin1Encoding Latin2Encoding Latin9Encoding LogFile Logger \
	LoggingFactory LoggingRegistry LogStream NamedEvent NamedMutex NullChannel \
	MemoryPool MD2Engine MD4Engine MD5Engine Manifest Message Mutex \
	NestedDiagnosticContext Notification NotificationCenter \
	TimedNotificationQueue PriorityNotificationQueue \
	NotificationQueue NullStream NumberFormatter NumberParser AbstractObserver \
	Path PatternFormatter Process PurgeStrategy RWLock Random RandomStream \
	RegularExpression RefCountedObject Runnable RotateStrategy Condition \
	SHA1Engine Semaphore SharedLibrary SimpleFileChannel \
	SignalHandler SplitterChannel Stopwatch StreamChannel StreamConverter StreamCopier \
	StreamTokenizer String StringTokenizer SynchronizedObject \
	Task TaskManager TaskNotification TeeStream Hash HashStatistic \
	TemporaryFile TextConverter TextEncoding TextIterator Thread ThreadLocal \
	ThreadPool ThreadTarget ActiveDispatcher Timer Timespan Timestamp Timezone Token URI \
	FileStreamFactory URIStreamFactory URIStreamOpener UTF16Encoding Windows1250Encoding Windows1251Encoding Windows1252Encoding \
	UTF8Encoding UnicodeConverter UUID UUIDGenerator Var VarHolder Void Format \
	Pipe PipeImpl PipeStream SharedMemory FileStream Unicode UTF8String \
	adler32 compress crc32 deflate gclose gzlib gzread gzwrite infback inffast inflate inftrees trees uncompr zutil \
	pcre_chartables pcre_compile pcre_globals pcre_maketables pcre_study \
	pcre_tables pcre_try_flipped pcre_ucd pcre_valid_utf8 \
	pcre_exec pcre_ord2utf8 pcre_newline pcre_fullinfo pcre_xclass

ifeq ($(POCO_CONFIG),MinGW)
	objects += EventLogChannel WindowsConsoleChannel
else
	objects += SyslogChannel
endif

target         = PocoFoundation
target_version = $(LIBVERSION)
target_libs    =

include $(POCO_BASE)/build/rules/lib
