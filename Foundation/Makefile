#
# Makefile
#
# Makefile for Poco Foundation
#

include $(POCO_BASE)/build/rules/global

objects = ArchiveStrategy Ascii ASCIIEncoding AsyncChannel AsyncNotificationCenter ActiveThreadPool\
	Base32Decoder Base32Encoder Base64Decoder Base64Encoder \
	BinaryReader BinaryWriter Bugcheck ByteOrder Channel Checksum Clock Configurable ConsoleChannel \
	Condition CountingStream DateTime LocalDateTime DateTimeFormat DateTimeFormatter DateTimeParser \
	Debugger DeflatingStream DigestEngine DigestStream DirectoryIterator DirectoryWatcher \
	Environment Event EventChannel Error EventArgs ErrorHandler Exception FIFOBufferStream FPEnvironment File \
	FileChannel Formatter FormattingChannel Glob HexBinaryDecoder LineEndingConverter \
	HexBinaryEncoder InflatingStream JSONString Latin1Encoding Latin2Encoding Latin9Encoding LogFile \
	Logger LoggingFactory LoggingRegistry LogStream NamedEvent NamedMutex NullChannel \
	MemoryPool MD4Engine MD5Engine Manifest Message Mutex \
	NestedDiagnosticContext Notification NotificationCenter \
	NotificationQueue PriorityNotificationQueue TimedNotificationQueue \
	NullStream NumberFormatter NumberParser NumericString AbstractObserver \
	Path PatternFormatter PIDFile Process ProcessRunner PurgeStrategy RWLock Random RandomStream \
	DirectoryIteratorStrategy RegularExpression RefCountedObject Runnable RotateStrategy \
	SHA1Engine SHA2Engine Semaphore SharedLibrary SimpleFileChannel \
	SignalHandler SplitterChannel SortedDirectoryIterator Stopwatch StreamChannel \
	StreamConverter StreamCopier StreamTokenizer String StringTokenizer SynchronizedObject \
	Task TaskManager TaskNotification TeeStream Hash HashStatistic \
	TemporaryFile TextConverter TextEncoding TextIterator TextBufferIterator Thread ThreadLocal \
	ThreadPool ThreadTarget ActiveDispatcher Timer Timespan Timestamp Timezone Token URI \
	FileStreamFactory URIStreamFactory URIStreamOpener UTF32Encoding UTF16Encoding UTF8Encoding UTF8String \
	Unicode UnicodeConverter Windows1250Encoding Windows1251Encoding Windows1252Encoding \
	UUID UUIDGenerator Void Var VarHolder VarIterator VarVisitor Format Pipe PipeImpl PipeStream SharedMemory \
	MemoryStream FileStream AtomicCounter DataURIStream DataURIStreamFactory

zlib_objects = adler32 compress crc32 deflate \
	infback inffast inflate inftrees trees zutil

pcre_objects = pcre2_auto_possess pcre2_chartables pcre2_compile pcre2_config \
	pcre2_context pcre2_convert pcre2_dfa_match pcre2_error pcre2_extuni \
	pcre2_find_bracket pcre2_jit_compile pcre2_maketables pcre2_match \
	pcre2_match_data pcre2_newline pcre2_ord2utf pcre2_pattern_info \
	pcre2_script_run pcre2_serialize pcre2_string_utils pcre2_study pcre2_substitute \
	pcre2_substring pcre2_tables pcre2_ucd pcre2_valid_utf pcre2_xclass

pcre_utf8_objects = pcre2_ucd pcre2_tables

ifdef POCO_UNBUNDLED
	SYSLIBS += -lpcre2-8 -lz
	objects += $(pcre_utf8_objects)
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

ifeq ($(findstring MinGW, $(POCO_CONFIG)), MinGW)
    $(shell cd src; $(WINDMC) pocomsg.mc)
endif

include $(POCO_BASE)/build/rules/lib
