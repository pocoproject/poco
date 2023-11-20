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

POCO_LIB_PATH=$POCO_BASE/lib/$OSNAME/$OSARCH
POCO_BIN_PATH=$POCO_BASE/Foundation/testsuite/bin/$OSNAME/$OSARCH
POCO_PATHS=$POCO_BASE:$POCO_LIB_PATH:$POCO_BIN_PATH
if [[ "$PATH" != *"$POCO_PATHS"* ]]; then
	PATH=$POCO_PATHS:$PATH
	export PATH
fi

explibpath=
expbinpath=
exppocopath=
function setPath {
# adds POCO_PATHS to system PATH only if they are not there already
	IFS=':' read -ra PATHDIR <<< "$1"
	for i in "${PATHDIR[@]}"; do
		if [[ "$POCO_LIB_PATH" == "$i" ]]; then
			explibpath+="$i"
		elif [[ "$POCO_BIN_PATH" == "$i" ]]; then
			expbinpath+="$i"
		elif [[ "$POCO_BASE" == "$i" ]]; then
			exppocopath+="$i"
		fi
	done
}

setPath "$PATH"

if [[ -z "$exppocopath" ]]; then
	export PATH="$POCO_BASE:$PATH"
fi

if [[ -z "$explibpath" ]]; then
	export PATH="$POCO_LIB_PATH:$PATH"
fi

if [[ -z "$expbinpath" ]]; then
	export PATH="$POCO_BIN_PATH:$PATH"
fi

explibpath=
function setLibPath {
# adds POCO_LIB_PATH to (DY)LD_LIBRARY_PATH only if it is not there already
	IFS=':' read -ra LIBDIR <<< "$1"
	for i in "${LIBDIR[@]}"; do
		if [[ "$POCO_LIB_PATH" == "$i" ]]; then
			explibpath="$1"
			break
		fi
	done
}

case "${OSNAME}" in
	Linux*)
		setLibPath "$LD_LIBRARY_PATH"
		if [[ -z "$explibpath" ]]; then
			export LD_LIBRARY_PATH="$POCO_LIB_PATH:$DYLD_LIBRARY_PATH"
		fi
		;;
	Darwin*)
		setLibPath "$DYLD_LIBRARY_PATH"
		if [[ -z "$explibpath" ]]; then
			export DYLD_LIBRARY_PATH="$POCO_LIB_PATH:$DYLD_LIBRARY_PATH"
		fi
		;;
esac

# uncomment for sanitizer builds
#LSAN_OPTIONS=verbosity=1:log_threads=1
#export LSAN_OPTIONS
#TSAN_OPTIONS="suppressions=$POCO_BASE/tsan.suppress,second_deadlock_stack=1"
#export TSAN_OPTIONS

echo "\$OSNAME    = $OSNAME"
echo "\$OSARCH    = $OSARCH"
echo "\$POCO_BASE = $POCO_BASE"
echo "\$PATH      = $PATH"
case "${OSNAME}" in
	Linux*)  echo "\$LD_LIBRARY_PATH   = $LD_LIBRARY_PATH";;
	Darwin*) echo "\$DYLD_LIBRARY_PATH = $DYLD_LIBRARY_PATH";;
esac
