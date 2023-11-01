#!/bin/bash
#
# Script to rebuild a library and dependencies
# (with or without a sanitizer) and run the tests.
# Currently works only for top-level libs
# dependent on Foundation only.
#
# Usage: ./runLibTests.sh library [address | undefined | thread ]
#

library=$1
if [ -z "${library}" ]; then
	echo "Library not specified"
	echo "Usage: $0 library [address | undefined | thread ]"
	exit 1
fi

self="${BASH_SOURCE[0]}"

if [ -d "$self" ] ; then
	basedir="$(cd "$self" || exit; pwd -P)"
else
	basedir="$(cd "$(dirname "$self")" || exit; pwd -P)"
fi

# shellcheck disable=SC1091
. "$basedir"/poco_env.bash

flag=$2

make distclean -C CppUnit
make distclean -C Foundation
if [[ "$library" != "Foundation" ]]; then
	make distclean -C "$library"
fi
make distclean -C "$library"/testsuite

if [ -n "${flag}" ]; then
	make -s -j4 -C "$POCO_BASE"/CppUnit SANITIZEFLAGS+=-fsanitize="$flag"
	make -s -j4 -C "$POCO_BASE"/Foundation SANITIZEFLAGS+=-fsanitize="$flag"
	if [[ "$library" != "Foundation" ]]; then
		make -s -j4 -C "$POCO_BASE"/"$library" SANITIZEFLAGS+=-fsanitize="$flag"
	fi
	make -s -j4 -C "$POCO_BASE"/"$library"/testsuite SANITIZEFLAGS+=-fsanitize="$flag"
else
	make -s -j4 -C "$POCO_BASE"/CppUnit
	make -s -j4 -C "$POCO_BASE"/Foundation
	if [[ "$library" != "Foundation" ]]; then
		make -s -j4 -C "$POCO_BASE"/"$library"
	fi
	make -s -j4 -C "$POCO_BASE"/"$library"/testsuite
fi

cd "$basedir"/"$library"/testsuite/bin/"$OSNAME"/"$OSARCH"/ || exit
testrunner -all
testrunnerd -all
