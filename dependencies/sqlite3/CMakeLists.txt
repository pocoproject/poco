

if(POCO_SQLITE_UNBUNDLED)
	if (ENABLE_DATA_SQLITE)
		find_package(SQLite3 REQUIRED)
	else()
		find_package(SQLite3)
	endif()
	if (SQLite3_FOUND)
		set_target_properties(SQLite::SQLite3 PROPERTIES IMPORTED_GLOBAL TRUE)
		message(WARNING "Unbundled SQLite3: Assuming that it was compiled with SQLITE_THREADSAFE=1")
		target_compile_definitions(SQLite::SQLite3 INTERFACE SQLITE_THREADSAFE=1)
	endif()
else()

	# Sources
	file(GLOB SRCS_G "src/*.c")
	POCO_SOURCES(SRCS sqlite3 ${SRCS_G})

	# Headers
	file(GLOB_RECURSE HDRS_G "src/*.h")
	POCO_HEADERS(SRCS sqlite3 ${HDRS_G})

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_SQLITE3 OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_SQLITE3 PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_compile_definitions(
		_BUNDLED_SQLITE3
		PUBLIC
			SQLITE_THREADSAFE=1

		PRIVATE
			SQLITE_DISABLE_LFS
			SQLITE_OMIT_UTF16
			SQLITE_OMIT_PROGRESS_CALLBACK
			SQLITE_OMIT_COMPLETE
			SQLITE_OMIT_TCL_VARIABLE
			SQLITE_OMIT_DEPRECATED
	)
	option(ENABLE_DATA_SQLITE_FTS5 "Enable SQLite FTS5 Extension" OFF)
	if(ENABLE_DATA_SQLITE_FTS5)
		target_compile_definitions(_BUNDLED_SQLITE3 PUBLIC SQLITE_ENABLE_FTS5)
	endif()

	target_include_directories(_BUNDLED_SQLITE3
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	add_library(SQLite::SQLite3 ALIAS _BUNDLED_SQLITE3)
endif()

