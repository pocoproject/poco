#!/bin/bash
#
# Script to rebuild libraries and dependencies
# (with or without a sanitizer) and run the tests.
#
# The use case of the script is mainly for development purposes -
# to clean and rebuild a single library, with all of its dependencies,
# and run the tests.
#
# Usage: ./runLibTests.sh library	 [address | undefined | thread ]
#
# Example: ./runLibTests.sh Data/SQLite address
# (distcleans, rebuilds and runs tests for Data/SQLite with address sanitizer)
#

# g++ does not like empty quoted arguments, but
# the shellcheck wants them quoted to remain quiet
# shellcheck disable=SC2086

path=$1
if [ -z "${path}" ]; then
	echo "Library not specified"
	echo "Usage: $0 path	 [address | undefined | thread ]"
	exit 1
fi

libraries=
IFS='/' read -r -a libraries <<< "$path"

self="${BASH_SOURCE[0]}"

if [ -d "$self" ] ; then
	basedir="$(cd "$self" || exit; pwd -P)"
else
	basedir="$(cd "$(dirname "$self")" || exit; pwd -P)"
fi

# shellcheck disable=SC1091
. "$basedir"/poco_env.bash

flag=$2
flags=
if [ -n "${flag}" ]; then
	flags=SANITIZEFLAGS+=-fsanitize="$flag"
fi

path="$basedir"/"${libraries[0]}"

make distclean -C "$basedir"/Foundation
make distclean -C "$basedir"/CppUnit

make -s -j4 -C "$basedir"/Foundation $flags
make -s -j4 -C "$basedir"/CppUnit $flags

# Foundation requested, build/run tests and exit
if [[ "$path" == "$basedir"/"Foundation" ]]; then
	cd "$path/testsuite/" || exit
	make -s -j4 -C ./ $flags
	cd "bin/$OSNAME/$OSARCH/" || exit
	./testrunner -all
	./testrunnerd -all
	echo "$path $flags done."
	exit 0
fi

for library in "${libraries[@]}"
do
	cd "$library" || exit
	make distclean
	make -s -j4 -C ./ $flags
	cd testsuite || exit
	make distclean
	make -s -j4 -C ./ $flags
	cd bin/"$OSNAME"/"$OSARCH"/ || exit
	./testrunner -all
	./testrunnerd -all
	echo "$1 $flags done."
	cd ../../../../ || exit
done
