#
# Makefile
#
# $Id: //poco/1.2/Foundation/Makefile#2 $
#
# Makefile for Poco Foundation
#

include $(POCO_BASE)/build/rules/global

objects = ArchiveStrategy ASCIIEncoding AsyncChannel Base64Decoder Base64Encoder \
	BinaryReader BinaryWriter Bugcheck ByteOrder Channel Configurable ConsoleChannel \
	CountingStream DateTime LocalDateTime DateTimeFormat DateTimeFormatter DateTimeParser \
	Debugger DeflatingStream DigestEngine DigestStream DirectoryIterator \
	Environment Event EventArgs ErrorHandler Exception FPEnvironment File Glob \
	FileChannel Formatter FormattingChannel HexBinaryDecoder LineEndingConverter \
	HexBinaryEncoder InflatingStream Latin1Encoding Latin9Encoding LogFile Logger \
	LoggingFactory LoggingRegistry LogStream NamedEvent NamedMutex NullChannel \
	MemoryPool MD2Engine MD4Engine MD5Engine Manifest Message Mutex \
	NestedDiagnosticContext Notification NotificationCenter \
	NotificationQueue NullStream NumberFormatter NumberParser AbstractObserver \
	Path PatternFormatter Process PurgeStrategy RWLock Random RandomStream \
	RegularExpression RefCountedObject Runnable RotateStrategy \
	SHA1Engine Semaphore SharedLibrary SimpleFileChannel \
	SignalHandler SplitterChannel Stopwatch StreamChannel StreamConverter StreamCopier \
	StreamTokenizer String StringTokenizer SynchronizedObject SyslogChannel \
	Task TaskManager TaskNotification TeeStream HashStatistic\
	TemporaryFile TextConverter TextEncoding TextIterator Thread ThreadLocal \
	ThreadPool ActiveDispatcher Timer Timespan Timestamp Timezone Token URI \
	FileStreamFactory URIStreamFactory URIStreamOpener UTF16Encoding Windows1252Encoding \
	UTF8Encoding UnicodeConverter UUID UUIDGenerator Void Format \
	Pipe PipeImpl PipeStream \
	adler32 chartables compress crc32 \
	deflate get gzio infback inffast inflate inftrees maketables pcre study \
	trees zutil

target         = PocoFoundation
target_version = $(LIBVERSION)
target_libs    =

include $(POCO_BASE)/build/rules/lib
