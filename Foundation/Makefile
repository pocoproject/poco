#
# Makefile
#
# Makefile for Poco Foundation
#

include $(POCO_BASE)/build/rules/global

ifdef POCO_ENABLE_TRACE
INCLUDE += -I $(POCO_BASE)/Trace/include/Poco/Trace
endif

objects = ArchiveStrategy Ascii ASCIIEncoding AsyncChannel AsyncNotificationCenter ActiveThreadPool\
	Base32Decoder Base32Encoder Base64Decoder Base64Encoder \
	BinaryReader BinaryWriter Bugcheck ByteOrder Channel Checksum Clock Configurable ConsoleChannel \
	Condition CountingStream DateTime LocalDateTime DateTimeFormat DateTimeFormatter DateTimeParser \
	Debugger DeflatingStream DigestEngine DigestStream DirectoryIterator DirectoryWatcher \
	Environment Event EventChannel Error EventArgs ErrorHandler Exception FIFOBufferStream FPEnvironment File \
	FileChannel Formatter FormattingChannel Glob HexBinaryDecoder LineEndingConverter \
	HexBinaryEncoder InflatingStream IOLock JSONString Latin1Encoding Latin2Encoding Latin9Encoding LogFile \
	Logger LoggingFactory LoggingRegistry LogStream NamedEvent NamedMutex NullChannel \
	MemoryPool MD4Engine MD5Engine Manifest Message Mutex \
	NestedDiagnosticContext Notification NotificationCenter \
	NotificationQueue PriorityNotificationQueue TimedNotificationQueue \
	NullStream NumberFormatter NumberParser NumericString AbstractObserver \
	Path PatternFormatter JSONFormatter PIDFile Process ProcessRunner PurgeStrategy RWLock Random RandomStream \
	DirectoryIteratorStrategy RegularExpression RefCountedObject Runnable RotateStrategy \
	SHA1Engine SHA2Engine Semaphore SharedLibrary SimpleFileChannel \
	SignalHandler SplitterChannel SortedDirectoryIterator Stopwatch StreamChannel \
	StreamConverter StreamCopier StreamTokenizer String StringTokenizer SynchronizedObject \
	Task TaskManager TaskNotification TeeStream Hash HashStatistic \
	TemporaryFile TextConverter TextEncoding TextIterator TextBufferIterator Thread ThreadLocal \
	ThreadPool ThreadTarget ActiveDispatcher Timer Timespan Timestamp Timezone Token URI \
	FileStreamFactory URIStreamFactory URIStreamOpener UTF32Encoding UTF16Encoding UTF8Encoding UTF8String \
	Unicode UnicodeConverter Windows1250Encoding Windows1251Encoding Windows1252Encoding \
	UUID UUIDGenerator ULID ULIDGenerator Void Var VarHolder VarIterator VarVisitor Format Pipe PipeImpl PipeStream SharedMemory \
	MemoryStream FileStream AtomicCounter DataURIStream DataURIStreamFactory FileStreamRWLock \
	BufferedBidirectionalStreamBuf BufferedStreamBuf UnbufferedStreamBuf

zlib_objects = adler32 compress crc32 deflate \
	infback inffast inflate inftrees trees zutil

pcre_objects = pcre2_auto_possess pcre2_chartables pcre2_chkdint pcre2_compile \
	pcre2_compile_cgroup pcre2_compile_class pcre2_config \
	pcre2_context pcre2_convert pcre2_dfa_match pcre2_error pcre2_extuni \
	pcre2_find_bracket pcre2_jit_compile pcre2_maketables pcre2_match \
	pcre2_match_data pcre2_match_next pcre2_newline pcre2_ord2utf pcre2_pattern_info \
	pcre2_script_run pcre2_serialize pcre2_string_utils pcre2_study pcre2_substitute \
	pcre2_substring pcre2_tables pcre2_ucd pcre2_valid_utf pcre2_xclass

pcre_utf8_objects = pcre2_ucd pcre2_tables

utf8proc_objects = utf8proc

ifdef POCO_UNBUNDLED
	SYSLIBS += -lpcre2-8 -lutf8proc -lz
	objects += $(pcre_utf8_objects)
else
	COMMONFLAGS += -DUTF8PROC_STATIC -DHAVE_CONFIG_H
	objects += $(zlib_objects) $(pcre_objects) $(pcre_utf8_objects) $(utf8proc_objects)
endif

# FastLogger support (requires Quill library) - enabled by default
# Set POCO_NO_FASTLOGGER=1 to disable
ifndef POCO_NO_FASTLOGGER
	POCO_ENABLE_FASTLOGGER = 1
endif
ifdef POCO_ENABLE_FASTLOGGER
	objects += FastLogger
	COMMONFLAGS += -DPOCO_ENABLE_FASTLOGGER
	ifdef POCO_UNBUNDLED
		SYSLIBS += -lquill
	else
		INCLUDE += -I$(POCO_BASE)/dependencies/quill/include
	endif
endif

ifeq ($(findstring MinGW, $(POCO_CONFIG)), MinGW)
	objects += EventLogChannel WindowsConsoleChannel
else
	objects += SyslogChannel
endif

target         = PocoFoundation
target_version = $(LIBVERSION)
target_libs    =
ifdef POCO_ENABLE_TRACE
target_libs   += PocoTrace
endif

ifeq ($(findstring MinGW, $(POCO_CONFIG)), MinGW)
    $(shell cd src; $(WINDMC) pocomsg.mc)
endif

# make build system looks for sources in src/
ifndef POCO_UNBUNDLED
	prebuild = $(shell \
		ln -sf $(POCO_BASE)/dependencies/zlib/src/*.c        src/ && \
		ln -sf $(POCO_BASE)/dependencies/zlib/src/*.h        src/ && \
		ln -sf $(POCO_BASE)/dependencies/pcre2/src/*.c       src/ && \
		ln -sf $(POCO_BASE)/dependencies/pcre2/src/*.h       src/ && \
		ln -sf $(POCO_BASE)/dependencies/utf8proc/src/*.c    src/ && \
		ln -sf $(POCO_BASE)/dependencies/utf8proc/src/*.h    src/ \
	)

	INCLUDE += -I$(POCO_BASE)/dependencies/zlib/src
	INCLUDE += -I$(POCO_BASE)/dependencies/pcre2/src
	INCLUDE += -I$(POCO_BASE)/dependencies/utf8proc/src
	INCLUDE += -I$(POCO_BASE)/dependencies/v8_double_conversion/src
endif

include $(POCO_BASE)/build/rules/lib
