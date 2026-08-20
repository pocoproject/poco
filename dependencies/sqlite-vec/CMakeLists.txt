# sqlite-vec vector search extension for SQLite
# Version: 0.1.9
# Source: https://github.com/asg017/sqlite-vec/releases/download/v0.1.9/sqlite-vec-0.1.9-amalgamation.tar.gz
#
# Always compiled from this bundled amalgamation (also when SQLite itself is
# unbundled) with SQLITE_CORE + SQLITE_VEC_STATIC; registered at runtime via
# sqlite3_auto_extension() in Data/SQLite. LICENSE-MIT / LICENSE-APACHE come
# from the upstream repository tag (the amalgamation archive lacks them).

# Declared here following the ENABLE_DATA_SQLITE_FTS5 pattern in dependencies/sqlite3.
option(ENABLE_DATA_SQLITE_VEC "Enable sqlite-vec (vector search) extension for Data/SQLite" OFF)

if(ENABLE_DATA_SQLITE_VEC AND ENABLE_DATA_SQLITE)
	if(POCO_SQLITE_UNBUNDLED)
		# SQLite3_VERSION is scoped to dependencies/sqlite3; re-resolve (cached)
		find_package(SQLite3 REQUIRED)
		# sqlite-vec uses interfaces up to 3.44 (xIntegrity); MemoryDB needs
		# PRAGMA table_list (3.37)
		if(SQLite3_VERSION VERSION_LESS 3.37.0)
			message(FATAL_ERROR
				"ENABLE_DATA_SQLITE_VEC requires SQLite >= 3.37.0, found ${SQLite3_VERSION}")
		elseif(SQLite3_VERSION VERSION_LESS 3.44.0)
			message(WARNING
				"Unbundled SQLite ${SQLite3_VERSION} is older than 3.44.0; "
				"sqlite-vec builds with reduced functionality (no xIntegrity, "
				"and no rowid IN (...) optimization below 3.38)")
		endif()
	endif()

	POCO_SOURCES(SRCS sqlite-vec src/sqlite-vec.c)
	POCO_HEADERS(SRCS sqlite-vec src/sqlite-vec.h)

	# NOTE: We use object library to be able to link it with static or shared libraries
	add_library(_BUNDLED_SQLITE_VEC OBJECT EXCLUDE_FROM_ALL ${SRCS})

	set_property(TARGET _BUNDLED_SQLITE_VEC PROPERTY POSITION_INDEPENDENT_CODE ON)

	target_compile_definitions(_BUNDLED_SQLITE_VEC
		PRIVATE
			SQLITE_CORE
		PUBLIC
			SQLITE_VEC_STATIC
	)

	# sqlite-vec has no runtime CPU dispatch; SIMD is fixed at compile time.
	# The NEON kernels compile only with Clang (GCC rejects their implicit
	# int8x16_t/uint8x16_t conversions). AVX is enabled only when the
	# toolchain already targets AVX2, or when forced, since the binary then
	# requires an AVX2-capable CPU.
	option(ENABLE_DATA_SQLITE_VEC_AVX
		"Force AVX2 SIMD in sqlite-vec (adds -mavx -mavx2; binary requires an AVX2 CPU)" OFF)
	set(_sqlite_vec_simd "generic")
	if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
			if(CMAKE_C_COMPILER_ID MATCHES "Clang")
				target_compile_definitions(_BUNDLED_SQLITE_VEC PRIVATE SQLITE_VEC_ENABLE_NEON)
				set(_sqlite_vec_simd "neon")
			endif()
		elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|amd64|AMD64)$")
			if(ENABLE_DATA_SQLITE_VEC_AVX)
				target_compile_options(_BUNDLED_SQLITE_VEC PRIVATE -mavx -mavx2)
				target_compile_definitions(_BUNDLED_SQLITE_VEC PRIVATE SQLITE_VEC_ENABLE_AVX)
				set(_sqlite_vec_simd "avx (forced)")
			else()
				include(CheckCSourceCompiles)
				check_c_source_compiles("
					#ifndef __AVX2__
					#error no avx2
					#endif
					int main(void) { return 0; }" POCO_SQLITE_VEC_HAVE_AVX2)
				if(POCO_SQLITE_VEC_HAVE_AVX2)
					target_compile_definitions(_BUNDLED_SQLITE_VEC PRIVATE SQLITE_VEC_ENABLE_AVX)
					set(_sqlite_vec_simd "avx")
				endif()
			endif()
		endif()
	endif()
	message(STATUS "sqlite-vec SIMD: ${_sqlite_vec_simd}")

	target_include_directories(_BUNDLED_SQLITE_VEC
		PUBLIC
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	)

	# sqlite3.h include path and SQLITE_THREADSAFE from the active SQLite
	target_link_libraries(_BUNDLED_SQLITE_VEC PRIVATE SQLite::SQLite3)

	if(UNIX)
		target_link_libraries(_BUNDLED_SQLITE_VEC INTERFACE m)
	endif()

	add_library(SQLiteVec::SQLiteVec ALIAS _BUNDLED_SQLITE_VEC)
endif()
