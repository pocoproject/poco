# usage:
# . env.bash
# or
# source env.bash

self="${BASH_SOURCE[0]}"

if [ "$self" == "$0" ] ; then
	echo "This file must be sourced from bash, not run."
	echo "Usage: . $0"
	exit 1
fi

if [ -d "$self" ] ; then
	basedir="$(cd "$self" || exit; pwd -P)"
else
	basedir="$(cd "$(dirname "$self")" || exit; pwd -P)"
fi

OSNAME="${OSNAME:=$(uname -s)}"
export OSNAME
OSARCH="${OSARCH:=$(uname -m)}"
export OSARCH
POCO_BASE="$basedir"
export POCO_BASE
PATH=$POCO_BASE/lib/$OSNAME/$OSARCH:$POCO_BASE:$PATH
export PATH

# uncomment for sanitizer builds
#LSAN_OPTIONS=verbosity=1:log_threads=1
#export LSAN_OPTIONS
#TSAN_OPTIONS="suppressions=$POCO_BASE/tsan.suppress,second_deadlock_stack=1"
#export TSAN_OPTIONS

echo "\$OSNAME       = $OSNAME"
echo "\$OSARCH       = $OSARCH"
echo "\$POCO_BASE    = $POCO_BASE"
echo "\$LSAN_OPTIONS = $LSAN_OPTIONS"
echo "\$TSAN_OPTIONS = $TSAN_OPTIONS"
