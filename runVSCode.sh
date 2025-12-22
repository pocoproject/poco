#!/bin/bash
#
# this script sets the proper build/runtime
# environment before opening poco in vscode

self="${BASH_SOURCE[0]}"

if [ -d "$self" ] ; then
	basedir="$(cd "$self" || exit; pwd -P)"
else
	basedir="$(cd "$(dirname "$self")" || exit; pwd -P)"
fi

# shellcheck disable=SC1091
. "$basedir"/poco_env.bash

code "$basedir"
