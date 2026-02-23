#
# Makefile
#
# Makefile for bundled cpptrace library
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I$(POCO_BASE)/dependencies/cpptrace/src
INCLUDE += -I$(POCO_BASE)/dependencies/cpptrace/include

SYSFLAGS += -DCPPTRACE_STATIC_DEFINE
SYSFLAGS += -DCPPTRACE_DEMANGLE_WITH_CXXABI -DCPPTRACE_UNWIND_WITH_UNWIND

ifeq ($(OSNAME), Linux)
SYSFLAGS += -DCPPTRACE_GET_SYMBOLS_WITH_LIBBACKTRACE
SYSLIBS += -lbacktrace
else
ifeq ($(OSNAME), Darwin)
SYSFLAGS += -DCPPTRACE_GET_SYMBOLS_WITH_LIBDL
endif
endif

# Symlink .cpp files from subdirectories into src/ for flat compilation.
# utils/utils.cpp conflicts with src/utils.cpp so it is symlinked as utils_detail.cpp.
prebuild = $(shell \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/binary/*.cpp     src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/demangle/*.cpp   src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/jit/*.cpp        src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/platform/*.cpp   src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/snippets/*.cpp   src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/symbols/*.cpp    src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/symbols/dwarf/*.cpp src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/unwind/*.cpp     src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/error.cpp        src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/microfmt.cpp     src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/replace_all.cpp  src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/string_view.cpp  src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/utils.cpp        src/utils_detail.cpp && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/io/file.cpp            src/ && \
	ln -sf $(POCO_BASE)/dependencies/cpptrace/src/utils/io/memory_file_view.cpp src/ \
)

objects = cpptrace ctrace exceptions formatting from_current \
	logging options prune_symbol utils \
	elf mach-o module_base object pe safe_dl \
	demangle_with_cxxabi demangle_with_nothing demangle_with_winapi \
	jit_objects \
	dbghelp_utils memory_mapping \
	snippet \
	symbols_core symbols_with_addr2line symbols_with_dbghelp \
	symbols_with_dl symbols_with_libbacktrace symbols_with_libdwarf \
	symbols_with_nothing debug_map_resolver dwarf_options dwarf_resolver \
	unwind_with_dbghelp unwind_with_execinfo unwind_with_libunwind \
	unwind_with_nothing unwind_with_unwind unwind_with_winapi \
	error microfmt replace_all string_view utils_detail \
	file memory_file_view

target         = cpptrace
target_version = $(LIBVERSION)

include $(POCO_BASE)/build/rules/lib
