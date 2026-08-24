#
# Makefile
#
# Makefile for Poco SQLite
#

include $(POCO_BASE)/build/rules/global

COMMONFLAGS += -I$(POCO_BASE)/Data/SQLite/src

SYSFLAGS += -DSQLITE_THREADSAFE=1 -DSQLITE_DISABLE_LFS \
	-DSQLITE_OMIT_UTF16 -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_COMPLETE \
	-DSQLITE_OMIT_TCL_VARIABLE -DSQLITE_OMIT_DEPRECATED

objects = Binder Extractor Notifier SessionImpl Connector \
	SQLiteException SQLiteStatementImpl Utility

ifdef POCO_ENABLE_SQLITE_FTS5
		SYSFLAGS += -DSQLITE_ENABLE_FTS5
endif

ifdef POCO_SQLITE_UNBUNDLED
	SYSLIBS += -lsqlite3
else
	objects += sqlite3
	prebuild = $(shell \
		ln -sf $(POCO_BASE)/dependencies/sqlite3/src/*.c src/ && \
		ln -sf $(POCO_BASE)/dependencies/sqlite3/src/*.h src/ \
	)
endif

ifdef POCO_ENABLE_SQLITE_VEC
	# Always compiled from the bundled amalgamation, also with unbundled
	# SQLite. SQLITE_CORE is harmless globally (sqlite3.c defines it itself).
	SYSFLAGS += -DPOCO_ENABLE_SQLITE_VEC -DSQLITE_CORE -DSQLITE_VEC_STATIC
	objects += sqlite-vec
	prebuild += $(shell \
		ln -sf $(POCO_BASE)/dependencies/sqlite-vec/src/*.c src/ && \
		ln -sf $(POCO_BASE)/dependencies/sqlite-vec/src/*.h src/ \
	)
	# NEON kernels compile only with clang (GCC rejects the upstream
	# int8x16_t/uint8x16_t intrinsic conversions)
	ifneq (,$(filter aarch64 arm64,$(OSARCH)))
		ifneq (,$(findstring clang,$(CC)))
			SYSFLAGS += -DSQLITE_VEC_ENABLE_NEON
		endif
	endif
	ifdef POCO_ENABLE_SQLITE_VEC_AVX
		ifneq (,$(filter x86_64 amd64,$(OSARCH)))
			# Applies to every TU of PocoDataSQLite (make cannot scope flags
			# per file), so the whole library requires an AVX2-capable CPU.
			SYSFLAGS += -mavx -mavx2 -DSQLITE_VEC_ENABLE_AVX
		else
$(warning POCO_ENABLE_SQLITE_VEC_AVX ignored: target architecture $(OSARCH) is not x86_64)
		endif
	endif
	SYSLIBS += -lm
endif

ifndef POCO_DATA_NO_SQL_PARSER
	target_includes += $(POCO_BASE)/Data/SQLParser $(POCO_BASE)/Data/SQLParser/src
	objects += MemoryDB MemoryDBInspector
endif

target         = PocoDataSQLite
target_version = $(LIBVERSION)
target_libs    = PocoData PocoFoundation

include $(POCO_BASE)/build/rules/lib
