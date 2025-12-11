#
# CMakeLists.txt
#
# CMake build configuration for Poco Benchmark application
#
# Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
# and Contributors.
#
# SPDX-License-Identifier:	BSL-1.0
#

# Sources
set(SRCS
	src/BenchmarkApp.cpp
	src/PatternFormatterBench.cpp
	src/LoggerBench.cpp
)

# Headers
file(GLOB_RECURSE HDRS_G "include/*.h")

add_executable(Benchmark ${SRCS} ${HDRS_G})

target_link_libraries(Benchmark
	PUBLIC
		Poco::Foundation
		Poco::Util
		benchmark::benchmark
)

target_include_directories(Benchmark
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)
