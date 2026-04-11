#!/usr/bin/env bash
#
# check-upstream-versions.sh - Compare bundled dependency versions to upstream.
#
# For each bundled third-party library, reads the current version from
# dependencies/<lib>/CMakeLists.txt (the "# Version: X.Y.Z" comment) and
# queries the upstream project to find the latest release. Prints a table
# showing which libraries are up to date and which have updates available.
#
# Usage: check-upstream-versions.sh [library ...]
#
# With no arguments, checks all libraries. Otherwise only the named ones.
# Requires: curl. jq is optional (used when present for JSON parsing).
#
# Exit status: 0 if all up to date, 1 if any updates available, 2 on error.
#
# Note: queries to api.github.com are rate-limited (60 req/hour unauthenticated).
# Export GITHUB_TOKEN to raise the limit to 5000/hour.

set -uo pipefail

DEPS="$(cd "$(dirname "$0")" && pwd)"

# Libraries whose upstream is queryable by this script.
# Format: "<name> <kind> <arg>"
#   kind=gh:    GitHub releases, arg=<owner>/<repo>
#   kind=sqlite arg=ignored (scrapes sqlite.org/download.html)
#   kind=png    arg=ignored (scrapes libpng.org)
#   kind=7zip   arg=ignored (scrapes 7-zip.org/history.txt)
#
# IMPORTANT: Before applying an update, read dependencies/README.md for any
# per-library maintenance notes. Notable ones:
#  - tessil: has a local modification (external linkage on
#    numeric_cast/deserialize_value). Re-apply after refreshing sources.
#  - quill:  only supports specific OS/CPU combinations; the
#    ENABLE_FASTLOGGER platform whitelist in the top-level CMakeLists.txt
#    must be re-assessed against the new version.
LIBS=(
	"zlib                 gh     madler/zlib"
	"pcre2                gh     PCRE2Project/pcre2"
	"utf8proc             gh     JuliaStrings/utf8proc"
	"expat                gh     libexpat/libexpat"
	"sqlite3              sqlite -"
	"png                  png    -"
	"v8_double_conversion gh     google/double-conversion"
	"hpdf                 gh     libharu/libharu"
	"7zip                 7zip   -"
	"cpptrace             gh     jeremy-rifkin/cpptrace"
	"quill                gh     odygrd/quill"
	"wepoll               gh     piscisaureus/wepoll"
	"tessil               gh     Tessil/ordered-map"
)

have_jq=0
command -v jq >/dev/null 2>&1 && have_jq=1

curl_opts=(-sSL --max-time 15 --retry 2)
auth_hdr=()
[ -n "${GITHUB_TOKEN:-}" ] && auth_hdr=(-H "Authorization: Bearer $GITHUB_TOKEN")

# Normalize an upstream tag to a plain version string.
# Drops leading "v", "R_", "pcre2-", "release-"; converts "R_2_7_5" -> "2.7.5".
normalize_tag() {
	local t=$1
	t=${t#v}
	t=${t#release-}
	t=${t#pcre2-}
	if [[ $t == R_* ]]; then
		t=${t#R_}
		t=${t//_/.}
	fi
	printf '%s' "$t"
}

# Get the bundled version from dependencies/<lib>/CMakeLists.txt,
# falling back to dependencies/README.md's table when the CMakeLists file
# has no "# Version:" comment (e.g. tessil).
bundled_version() {
	local lib=$1 cml="$DEPS/$lib/CMakeLists.txt"
	local v=""
	if [ -f "$cml" ]; then
		v=$(grep -E "^#[[:space:]]+Version:" "$cml" | head -1 \
			| sed -E 's/^#[[:space:]]+Version:[[:space:]]*//; s/[[:space:]]*$//')
	fi
	if [ -z "$v" ] && [ -f "$DEPS/README.md" ]; then
		# Extract the version cell from "| [<lib>](<lib>/) ... | VERSION |"
		v=$(grep -E "^\|[[:space:]]+\[$lib\]\($lib/\)" "$DEPS/README.md" \
			| head -1 \
			| sed -E 's/^\|[[:space:]]+\['"$lib"'\]\('"$lib"'\/\)[^|]*\|[[:space:]]*([^|[:space:]]*)[[:space:]]*\|.*/\1/')
	fi
	printf '%s' "${v:--}"
}

# Query the latest GitHub release tag for owner/repo.
latest_gh() {
	local repo=$1
	local resp
	resp=$(curl "${curl_opts[@]}" ${auth_hdr[@]+"${auth_hdr[@]}"} \
		"https://api.github.com/repos/$repo/releases/latest" 2>/dev/null) || {
		printf -- '-'; return
	}
	local tag
	if [ "$have_jq" -eq 1 ]; then
		tag=$(printf '%s' "$resp" | jq -r '.tag_name // empty' 2>/dev/null)
	else
		tag=$(printf '%s' "$resp" \
			| grep -m1 '"tag_name"' \
			| sed -E 's/.*"tag_name":[[:space:]]*"([^"]+)".*/\1/')
	fi
	if [ -z "$tag" ]; then
		# Some projects don't cut releases, only tags. Fall back to /tags.
		resp=$(curl "${curl_opts[@]}" ${auth_hdr[@]+"${auth_hdr[@]}"} \
			"https://api.github.com/repos/$repo/tags" 2>/dev/null) || {
			printf -- '-'; return
		}
		if [ "$have_jq" -eq 1 ]; then
			tag=$(printf '%s' "$resp" | jq -r '.[0].name // empty' 2>/dev/null)
		else
			tag=$(printf '%s' "$resp" \
				| grep -m1 '"name"' \
				| sed -E 's/.*"name":[[:space:]]*"([^"]+)".*/\1/')
		fi
	fi
	printf '%s' "${tag:--}"
}

# Scrape latest SQLite version from the download page.
latest_sqlite() {
	local resp
	resp=$(curl "${curl_opts[@]}" "https://sqlite.org/download.html" 2>/dev/null) \
		|| { printf -- '-'; return; }
	# Look for sqlite-amalgamation-NNNNNNN.zip and decode to X.Y.Z.
	local n
	n=$(printf '%s' "$resp" \
		| grep -oE "sqlite-amalgamation-[0-9]{7}" \
		| head -1 \
		| sed -E 's/sqlite-amalgamation-//')
	if [ -z "$n" ]; then printf -- '-'; return; fi
	# NNNNNNN -> A.BB.CC (e.g. 3530000 -> 3.53.0)
	local maj=$((10#${n:0:1}))
	local min=$((10#${n:1:2}))
	local pat=$((10#${n:3:2}))
	printf '%d.%d.%d' "$maj" "$min" "$pat"
}

# Scrape latest libpng 1.6.x version from libpng.org.
latest_png() {
	local resp
	resp=$(curl "${curl_opts[@]}" "http://www.libpng.org/pub/png/libpng.html" 2>/dev/null) \
		|| { printf -- '-'; return; }
	printf '%s' "$resp" | grep -oE "libpng[[:space:]]+1\\.6\\.[0-9]+" \
		| head -1 | sed -E 's/libpng[[:space:]]+//'
}

# Scrape latest 7-Zip version from the history file.
latest_7zip() {
	local resp
	resp=$(curl "${curl_opts[@]}" "https://www.7-zip.org/history.txt" 2>/dev/null) \
		|| { printf -- '-'; return; }
	# First version-looking token in the file header is the current release.
	printf '%s' "$resp" | grep -oE '[0-9]+\.[0-9]+' | head -1
}

# Compare two dotted versions; prints "older"/"same"/"newer" based on a vs b.
# Missing/empty/non-numeric versions are treated as "unknown". Any operand
# without a digit (e.g. "-", or the "no version" cell from README.md) is
# considered unknown -- we never special-case literal non-ASCII characters.
version_cmp() {
	local a=$1 b=$2
	if [ -z "$a" ] || [ -z "$b" ] \
		|| ! [[ $a =~ [0-9] ]] || ! [[ $b =~ [0-9] ]]; then
		printf 'unknown'; return
	fi
	if [ "$a" = "$b" ]; then printf 'same'; return; fi
	# Use sort -V when available (portable enough for macOS coreutils + Linux).
	local first
	first=$(printf '%s\n%s\n' "$a" "$b" | sort -V | head -1)
	if [ "$first" = "$a" ]; then printf 'older'; else printf 'newer'; fi
}

# Select the libraries to check (CLI args or all).
if [ $# -gt 0 ]; then
	selected=("$@")
else
	selected=()
	for entry in "${LIBS[@]}"; do
		# shellcheck disable=SC2206
		parts=($entry)
		selected+=("${parts[0]}")
	done
fi

printf '%-22s %-12s %-12s %s\n' "Library" "Bundled" "Upstream" "Status"
printf '%-22s %-12s %-12s %s\n' "-------" "-------" "--------" "------"

exit_code=0

for lib in "${selected[@]}"; do
	entry=""
	for e in "${LIBS[@]}"; do
		# shellcheck disable=SC2206
		parts=($e)
		if [ "${parts[0]}" = "$lib" ]; then
			entry="$e"
			break
		fi
	done
	if [ -z "$entry" ]; then
		printf '%-22s %-12s %-12s %s\n' "$lib" "?" "?" "unsupported"
		continue
	fi

	# shellcheck disable=SC2206
	parts=($entry)
	kind=${parts[1]}
	arg=${parts[2]}

	bundled=$(bundled_version "$lib")

	case "$kind" in
		gh)     raw=$(latest_gh "$arg") ;;
		sqlite) raw=$(latest_sqlite) ;;
		png)    raw=$(latest_png) ;;
		7zip)   raw=$(latest_7zip) ;;
		*)      raw="-" ;;
	esac
	latest=$(normalize_tag "$raw")

	cmp=$(version_cmp "$bundled" "$latest")
	case "$cmp" in
		same)    status="up to date" ;;
		older)   status="UPDATE AVAILABLE"; exit_code=1 ;;
		newer)   status="ahead of upstream" ;;
		unknown) status="(cannot compare)" ;;
	esac

	printf '%-22s %-12s %-12s %s\n' "$lib" "$bundled" "$latest" "$status"
done

exit "$exit_code"
