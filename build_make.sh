#!/usr/bin/env bash
#
# Generic Poco component build / test runner.
#
# Usage:
#   ./build_make.sh [release|debug|all] [asan|ubsan|tsan] [test] [rebuild] [<component-path>]
#
# Defaults:
#   - mode      = all (release + debug, shared)
#   - sanitizer = none
#   - test      = off (don't build or run testsuite)
#   - rebuild   = off (incremental)
#   - component = none -> build EVERY discovered component, in dep order
#
# When a component path is given, only that component plus its transitive deps
# (derived from each Makefile's target_libs) are built. When 'test' is passed,
# the corresponding testsuite directory is also walked (which pulls CppUnit
# into the graph through the testsuite's own target_libs) and its testrunner
# is executed at the end.
#
# Examples:
#   ./build_make.sh                            # build every component
#   ./build_make.sh test                       # build every component + run every testsuite
#   ./build_make.sh release Data/SQLite test   # release-only Data/SQLite + deps + run its tests
#   ./build_make.sh Foundation                 # just Foundation
#   ./build_make.sh debug asan Util test rebuild
#
# Configures the make build for $POCO_CONFIG (default Linux-clang-c++20) on
# the first run. Sources poco_env.bash for POCO_BASE / OSNAME / OSARCH /
# LD_LIBRARY_PATH. Pass 'rebuild' to make clean before building (use after
# switching sanitizers).

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# poco_env.bash references LD_LIBRARY_PATH/DYLD_LIBRARY_PATH, which may be unset;
# source it before enabling 'set -u' so the unset reference doesn't abort.
# shellcheck disable=SC1091
. "$SCRIPT_DIR/poco_env.bash" >/dev/null

set -euo pipefail

POCO_CONFIG="${POCO_CONFIG:-Linux-clang-c++20}"
OMIT_DEFAULT=Data/ODBC,Data/MySQL,Data/PostgreSQL,MongoDB,Redis,PDF,DNSSD,DNSSD/Avahi,DNSSD/Bonjour,CppParser

MODE=all
SANITIZER=
RUN_TESTS=0
REBUILD=0
COMPONENT=

usage()
{
	sed -n '2,/^$/{s/^# \{0,1\}//;p}' "$0"
}

for arg in "$@"; do
	case "$arg" in
		release|debug|all) MODE=$arg ;;
		asan|ubsan|tsan)   SANITIZER=$arg ;;
		test)              RUN_TESTS=1 ;;
		rebuild)           REBUILD=1 ;;
		-h|--help)         usage; exit 0 ;;
		*)
			if [[ -d "$POCO_BASE/$arg" && -f "$POCO_BASE/$arg/Makefile" ]]; then
				COMPONENT=$arg
			else
				echo "error: '$arg' is neither a known flag nor a component dir" >&2
				echo "" >&2
				usage >&2
				exit 1
			fi
			;;
	esac
done

SAN_CFLAGS=
SAN_LDFLAGS=
case "$SANITIZER" in
	asan)  SAN_CFLAGS="-fsanitize=address -fno-omit-frame-pointer";   SAN_LDFLAGS="-fsanitize=address"   ;;
	ubsan) SAN_CFLAGS="-fsanitize=undefined -fno-omit-frame-pointer"; SAN_LDFLAGS="-fsanitize=undefined" ;;
	tsan)  SAN_CFLAGS="-fsanitize=thread -fno-omit-frame-pointer";    SAN_LDFLAGS="-fsanitize=thread"    ;;
esac

case "$MODE" in
	all)     MAKE_TARGETS=()                ;;
	release) MAKE_TARGETS=(shared_release)  ;;
	debug)   MAKE_TARGETS=(shared_debug)    ;;
esac

cd "$POCO_BASE"

if [[ ! -f config.make ]] || ! grep -qE "^POCO_CONFIG[[:space:]]*=[[:space:]]*${POCO_CONFIG}[[:space:]]*$" config.make; then
	echo "==> configure --config=$POCO_CONFIG"
	./configure --config="$POCO_CONFIG" --omit="$OMIT_DEFAULT"
fi

JOBS=$(nproc 2>/dev/null || echo 4)
MAKE_OPTS=(-s -j"$JOBS")
MAKE_ENV=(POCO_BASE="$POCO_BASE" POCO_BUILD="$POCO_BASE" POCO_CONFIG="$POCO_CONFIG")
if [[ -n "$SAN_CFLAGS" ]]; then
	MAKE_ENV+=(POCO_FLAGS="$SAN_CFLAGS" POCO_LDFLAGS="$SAN_LDFLAGS")
fi

# Cheap textual extractor for Makefile assignments. Spawning a fresh `make`
# per component to evaluate $(target) / $(target_libs) is correct but adds
# seconds to startup; POCO's Makefiles are simple enough that grep+sed gets
# the same answer in milliseconds. Handles `var = ...` and `var += ...`,
# both top-level and inside conditional blocks (conditionals are treated as
# always-active, which matches the ifndef-default for POCO's flags and at
# worst over-includes a dep - never miscalculates the build order).
parse_target()
{
	# `|| true` so set -e + pipefail don't abort on a Makefile that has no
	# `target =` line (grep returns 1 on no match).
	{ grep -E "^[[:space:]]*target[[:space:]]*=" "$1" 2>/dev/null || true; } \
		| head -1 \
		| sed 's/^[^=]*=[[:space:]]*//' \
		| awk '{print $1}'
}

parse_target_libs()
{
	{ grep -E "^[[:space:]]*target_libs[[:space:]]*[+]?=" "$1" 2>/dev/null || true; } \
		| sed 's/^[^=]*=[[:space:]]*//; s/\\$//' \
		| tr '\n' ' '
}

# 1. Component discovery: find every component Makefile and build the
# library-name -> directory index, caching target_libs at the same time.
declare -A LIB_TO_DIR
declare -A DEPS_OF
declare -a ALL_DIRS=()

while IFS= read -r mk; do
	rel="${mk#"$POCO_BASE"/}"
	dir="${rel%/Makefile}"
	case "$dir" in
		*/testsuite|*/testsuite/*|*/samples|*/samples/*|build|build/*|dependencies|dependencies/*) continue ;;
	esac
	target=$(parse_target "$mk")
	[[ -z "$target" ]] && continue
	LIB_TO_DIR[$target]=$dir
	DEPS_OF[$dir]=$(parse_target_libs "$mk")
	ALL_DIRS+=("$dir")
done < <(find "$POCO_BASE" -maxdepth 3 -name Makefile -type f 2>/dev/null)

# 2. Honor OMIT from config.make (entries are top-level dirs).
declare -A OMITTED
if [[ -f config.make ]]; then
	omit_line=$({ grep -E "^OMIT[[:space:]]*=" config.make 2>/dev/null || true; } \
		| head -1 | sed 's/^OMIT[[:space:]]*=[[:space:]]*//')
	for e in $omit_line; do OMITTED[$e]=1; done
fi

# 3. Topological walk via DFS.
declare -A VISITED
declare -a ORDER=()
declare -a TEST_DIRS=()  # component dirs whose testsuite was added

walk()
{
	local dir=$1
	[[ -n "${VISITED[$dir]:-}" ]] && return
	[[ -n "${OMITTED[$dir]:-}" ]] && return
	[[ ! -f "$POCO_BASE/$dir/Makefile" ]] && return
	VISITED[$dir]=1

	# Cache miss happens for testsuite dirs (skipped by discovery scan).
	if [[ -z "${DEPS_OF[$dir]+x}" ]]; then
		DEPS_OF[$dir]=$(parse_target_libs "$POCO_BASE/$dir/Makefile")
	fi
	for lib in ${DEPS_OF[$dir]}; do
		local depdir="${LIB_TO_DIR[$lib]:-}"
		[[ -n "$depdir" ]] && walk "$depdir"
	done
	ORDER+=("$dir")
}

if [[ -n "$COMPONENT" ]]; then
	walk "$COMPONENT"
else
	for d in "${ALL_DIRS[@]}"; do walk "$d"; done
fi

# 4. Testsuites. Every testsuite needs CppUnit, so pull it in unconditionally
# before walking any testsuite - some testsuite Makefiles (e.g.
# Foundation/testsuite) delegate to sub-makefiles and don't list CppUnit in
# their own target_libs, so the walker would otherwise place them in ORDER
# ahead of CppUnit and the link step would fail.
#
# Test scope follows the build scope: when a specific component was requested,
# only that component's testsuite is built and run; deps are built as
# libraries only. When no component was specified (build-everything mode),
# every component's testsuite is built and run.
if (( RUN_TESTS )); then
	cppunit_dir="${LIB_TO_DIR[CppUnit]:-}"
	if [[ -z "$cppunit_dir" ]]; then
		echo "error: 'test' was requested but CppUnit component was not discovered" >&2
		exit 1
	fi
	walk "$cppunit_dir"

	if [[ -n "$COMPONENT" ]]; then
		ts="$COMPONENT/testsuite"
		if [[ -f "$POCO_BASE/$ts/Makefile" ]]; then
			walk "$ts"
			TEST_DIRS+=("$COMPONENT")
		else
			echo "==> note: $COMPONENT has no testsuite, nothing to test"
		fi
	else
		component_order=("${ORDER[@]}")
		for d in "${component_order[@]}"; do
			[[ "$d" == "$cppunit_dir" ]] && continue
			ts="$d/testsuite"
			if [[ -f "$POCO_BASE/$ts/Makefile" ]]; then
				walk "$ts"
				TEST_DIRS+=("$d")
			fi
		done
	fi
fi

if (( ${#ORDER[@]} == 0 )); then
	echo "error: nothing to build (no components resolved)" >&2
	exit 1
fi

# 5. Optional clean.
if (( REBUILD )); then
	echo "==> cleaning"
	for d in "${ORDER[@]}"; do
		make "${MAKE_OPTS[@]}" -C "$POCO_BASE/$d" "${MAKE_ENV[@]}" clean
	done
fi

# 6. Build.
for d in "${ORDER[@]}"; do
	label="$d ($MODE${SANITIZER:+ + $SANITIZER})"
	echo "==> $label"
	make "${MAKE_OPTS[@]}" -C "$POCO_BASE/$d" "${MAKE_ENV[@]}" "${MAKE_TARGETS[@]}"
done

# 7. Run testsuites.
if (( RUN_TESTS )); then
	if [[ "$MODE" == release ]]; then
		RUNNER_NAME=testrunner
	else
		RUNNER_NAME=testrunnerd
	fi

	case "$SANITIZER" in
		asan)
			export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:print_stacktrace=1:abort_on_error=0:halt_on_error=0}"
			echo "==> ASAN_OPTIONS=$ASAN_OPTIONS"
			;;
		ubsan)
			export UBSAN_OPTIONS="${UBSAN_OPTIONS:-print_stacktrace=1:halt_on_error=1}"
			echo "==> UBSAN_OPTIONS=$UBSAN_OPTIONS"
			;;
		tsan)
			export TSAN_OPTIONS="${TSAN_OPTIONS:-halt_on_error=1:second_deadlock_stack=1}"
			echo "==> TSAN_OPTIONS=$TSAN_OPTIONS"
			;;
	esac

	failed=()
	skipped=()
	for d in "${TEST_DIRS[@]}"; do
		runner="$POCO_BASE/$d/testsuite/bin/$OSNAME/$OSARCH/$RUNNER_NAME"
		if [[ ! -x "$runner" ]]; then
			runner=$(find "$POCO_BASE/$d/testsuite/bin" -type f -name "$RUNNER_NAME" 2>/dev/null | head -1)
		fi
		if [[ -z "$runner" || ! -x "$runner" ]]; then
			echo "==> skipping $d (no $RUNNER_NAME under $d/testsuite/bin)"
			skipped+=("$d")
			continue
		fi
		echo "==> testing $d"
		if ! "$runner" -all; then
			failed+=("$d")
		fi
	done

	if (( ${#failed[@]} > 0 )); then
		echo "==> FAILED testsuites: ${failed[*]}" >&2
		exit 1
	fi
	if (( ${#skipped[@]} > 0 )); then
		echo "==> skipped testsuites (no runner found): ${skipped[*]}"
	fi
fi

echo "==> done"
