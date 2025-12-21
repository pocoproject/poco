#
# Makefile
#
# Makefile for Poco Benchmark application
#
# Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
# and Contributors.
#
# SPDX-License-Identifier:	BSL-1.0
#

include $(POCO_BASE)/build/rules/global

# Find Google Benchmark installation
# Try pkg-config first
BENCHMARK_CFLAGS := $(shell pkg-config --cflags benchmark 2>/dev/null)
BENCHMARK_LIBS := $(shell pkg-config --libs benchmark 2>/dev/null)

# If pkg-config failed, try common locations
ifeq ($(BENCHMARK_LIBS),)
    # macOS Homebrew (Apple Silicon)
    ifneq ($(wildcard /opt/homebrew/opt/google-benchmark/include/benchmark/benchmark.h),)
        BENCHMARK_CFLAGS := -I/opt/homebrew/opt/google-benchmark/include
        BENCHMARK_LIBS := -L/opt/homebrew/opt/google-benchmark/lib -lbenchmark -lpthread
    # macOS Homebrew (Intel)
    else ifneq ($(wildcard /usr/local/opt/google-benchmark/include/benchmark/benchmark.h),)
        BENCHMARK_CFLAGS := -I/usr/local/opt/google-benchmark/include
        BENCHMARK_LIBS := -L/usr/local/opt/google-benchmark/lib -lbenchmark -lpthread
    # Linux standard locations
    else ifneq ($(wildcard /usr/include/benchmark/benchmark.h),)
        BENCHMARK_CFLAGS :=
        BENCHMARK_LIBS := -lbenchmark -lpthread
    else ifneq ($(wildcard /usr/local/include/benchmark/benchmark.h),)
        BENCHMARK_CFLAGS := -I/usr/local/include
        BENCHMARK_LIBS := -L/usr/local/lib -lbenchmark -lpthread
    endif
endif

# Check if we found it
ifneq ($(BENCHMARK_LIBS),)

objects = BenchmarkApp PatternFormatterBench LoggerBench NotificationQueueBench

target         = benchmark
target_version = 1
target_libs    = PocoUtil PocoFoundation

SYSLIBS += $(BENCHMARK_LIBS)
INCLUDE += -I$(POCO_BASE)/Benchmark/include $(BENCHMARK_CFLAGS)

include $(POCO_BASE)/build/rules/exec

else

.PHONY: all clean distclean

all:
	@echo "Google Benchmark library not found - skipping Benchmark build"
	@echo "Install with: brew install google-benchmark (macOS) or apt install libbenchmark-dev (Linux)"

clean:
	@true

distclean:
	@true

endif
