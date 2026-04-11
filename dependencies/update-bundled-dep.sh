#!/usr/bin/env bash
#
# update-bundled-dep.sh - Update a bundled third-party dependency
#
# Strategy: download the upstream release, then for each file currently
# bundled in dependencies/<lib>/{src,include}/ find its counterpart in the
# upstream tarball (matched first by relative path, then by basename) and
# overwrite it. This preserves the curated subset of files that POCO
# bundles without listing them explicitly.
#
# Usage: update-bundled-dep.sh <library> <new_version>
#
# Adding a new library: add a case under "Per-library config" below.
#
# Notes:
# - SQLite: URL is derived from the version automatically. For versions
#   that aren't on the current sqlite.org/download.html page, set
#   SQLITE_URL to override.
# - 7zip uses a custom URL scheme. Set SEVENZIP_URL.
# - After running, manually inspect: git diff dependencies/<lib>
#   then build and test before committing.

set -euo pipefail

POCO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEPS="$POCO_ROOT/dependencies"

LIB=${1:-}
V=${2:-}

usage() {
	cat <<EOF
Usage: $0 <library> <new_version>

Updates a bundled third-party dependency in dependencies/<library>/.

Supported libraries:
  zlib pcre2 utf8proc expat sqlite3 png hpdf
  v8_double_conversion cpptrace quill wepoll 7zip

Examples:
  $0 expat 2.7.5
  $0 png 1.6.56
  SQLITE_URL=https://sqlite.org/2026/sqlite-amalgamation-3510300.zip \\
    $0 sqlite3 3.51.3

After running, review with: git diff dependencies/<lib>
Then build and test before committing.
EOF
	exit 1
}

[ -z "$LIB" ] && usage
[ -z "$V" ] && usage
[ -d "$DEPS/$LIB" ] || { echo "ERROR: $DEPS/$LIB does not exist" >&2; exit 1; }

# Compute the sqlite amalgamation URL for a given version.
#
# SQLite encodes versions in the filename as a 7-digit packed integer:
# major*1000000 + minor*10000 + patch*100, e.g. 3.53.0 -> 3530000. The
# filename is hosted under sqlite.org/<year>/, and <year> changes per
# release. We resolve <year> by scraping sqlite.org/download.html for
# a link whose filename matches the packed integer; if the requested
# version is no longer current (no match on the download page), we fall
# back to trying the current and previous two years.
sqlite_amalgamation_url() {
	local ver=$1
	local maj min pat
	IFS=. read -r maj min pat <<EOF
$ver
EOF
	[ -z "${maj:-}" ] || [ -z "${min:-}" ] || [ -z "${pat:-}" ] && return 1
	local n
	n=$(printf '%d%02d%02d00' "$maj" "$min" "$pat")

	local resp path
	resp=$(curl -sSL --max-time 15 https://sqlite.org/download.html 2>/dev/null || true)
	if [ -n "$resp" ]; then
		path=$(printf '%s' "$resp" \
			| grep -oE "[0-9]{4}/sqlite-amalgamation-$n\\.zip" \
			| head -n 1)
		if [ -n "$path" ]; then
			printf 'https://sqlite.org/%s' "$path"
			return 0
		fi
	fi

	# Fall back: try the current year and a few recent years.
	local year
	year=$(date +%Y)
	local y
	for y in "$year" "$((year - 1))" "$((year - 2))"; do
		local candidate="https://sqlite.org/$y/sqlite-amalgamation-$n.zip"
		if curl -sS -o /dev/null --max-time 10 -w '%{http_code}' \
			"$candidate" 2>/dev/null | grep -q '^200$'; then
			printf '%s' "$candidate"
			return 0
		fi
	done
	return 1
}

# Per-library config: $url is the source archive URL, $bundle_root is the
# subdirectory under dependencies/<lib>/ that contains the bundled files.
case "$LIB" in
	zlib)
		url="https://github.com/madler/zlib/releases/download/v$V/zlib-$V.tar.gz"
		bundle_root=src ;;
	pcre2)
		url="https://github.com/PCRE2Project/pcre2/releases/download/pcre2-$V/pcre2-$V.tar.gz"
		bundle_root=src ;;
	utf8proc)
		url="https://github.com/JuliaStrings/utf8proc/releases/download/v$V/utf8proc-$V.tar.gz"
		bundle_root=src ;;
	expat)
		tag="R_${V//./_}"
		url="https://github.com/libexpat/libexpat/releases/download/$tag/expat-$V.tar.gz"
		bundle_root=src ;;
	sqlite3)
		url=${SQLITE_URL:-$(sqlite_amalgamation_url "$V")} || {
			echo "ERROR: could not determine sqlite download URL for $V" >&2
			echo "Override with SQLITE_URL=<full URL>" >&2
			exit 1
		}
		bundle_root=src ;;
	png)
		url="https://github.com/pnggroup/libpng/archive/refs/tags/v$V.tar.gz"
		bundle_root=src ;;
	hpdf)
		url="https://github.com/libharu/libharu/archive/refs/tags/v$V.tar.gz"
		bundle_root=src ;;
	v8_double_conversion)
		url="https://github.com/google/double-conversion/archive/refs/tags/v$V.tar.gz"
		bundle_root=src ;;
	cpptrace)
		url="https://github.com/jeremy-rifkin/cpptrace/archive/refs/tags/v$V.tar.gz"
		bundle_root=src ;;
	quill)
		url="https://github.com/odygrd/quill/archive/refs/tags/v$V.tar.gz"
		bundle_root=include ;;
	wepoll)
		url="https://github.com/piscisaureus/wepoll/archive/refs/tags/v$V.tar.gz"
		bundle_root=src ;;
	7zip)
		if [ -z "${SEVENZIP_URL:-}" ]; then
			echo "ERROR: 7zip releases use custom URLs." >&2
			echo "Set SEVENZIP_URL (see https://www.7-zip.org/download.html)." >&2
			exit 1
		fi
		url="$SEVENZIP_URL"
		bundle_root=src ;;
	*)
		echo "ERROR: unknown library '$LIB'" >&2
		usage ;;
esac

bundle_dir="$DEPS/$LIB/$bundle_root"
[ -d "$bundle_dir" ] || { echo "ERROR: $bundle_dir does not exist" >&2; exit 1; }

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

echo "==> Downloading $url"
archive="$tmp/source"
curl -fSL "$url" -o "$archive"

echo "==> Extracting"
extracted="$tmp/extracted"
mkdir -p "$extracted"
ftype=$(file -b "$archive")
case "$ftype" in
	*Zip*|*"ZIP archive"*) unzip -q "$archive" -d "$extracted" ;;
	*gzip*) tar -xzf "$archive" -C "$extracted" ;;
	*XZ*|*"XZ compressed"*) tar -xJf "$archive" -C "$extracted" ;;
	*bzip2*) tar -xjf "$archive" -C "$extracted" ;;
	*)
		# Fallback: let tar autodetect
		tar -xf "$archive" -C "$extracted" 2>/dev/null \
			|| { echo "ERROR: cannot determine archive type ($ftype)" >&2; exit 1; } ;;
esac

echo "==> Refreshing bundled files in $bundle_dir"
# Collect bundled files into an array (relative paths) so the loop runs in
# the parent shell and counters work.
bundled=()
while IFS= read -r f; do
	bundled+=("${f#./}")
done < <(cd "$bundle_dir" && find . -type f | sort)

updated=0
missing=()
for rel in "${bundled[@]}"; do
	# First try matching the full relative path (handles nested layouts
	# like quill/include/quill/core/Foo.h).
	found=$(find "$extracted" -type f -path "*/$rel" 2>/dev/null | head -n 1 || true)
	if [ -z "$found" ]; then
		# Fall back to matching by basename only.
		base=$(basename "$rel")
		found=$(find "$extracted" -type f -name "$base" 2>/dev/null | head -n 1 || true)
	fi
	if [ -n "$found" ]; then
		cp "$found" "$bundle_dir/$rel"
		updated=$((updated + 1))
	else
		missing+=("$rel")
	fi
done

echo "  $updated file(s) updated"
if [ ${#missing[@]} -gt 0 ]; then
	echo "  WARNING: ${#missing[@]} file(s) not found in upstream:"
	for m in "${missing[@]}"; do echo "    $m"; done
fi

echo "==> Updating version markers"

cml="$DEPS/$LIB/CMakeLists.txt"
if [ -f "$cml" ]; then
	if grep -q "^# Version:" "$cml"; then
		sed -i.bak "s|^# Version:.*|# Version: $V|" "$cml" && rm "$cml.bak"
		echo "  $cml (# Version)"
	fi
	# Also bump the version in the "# Source:" URL if it embeds one.
	# Matches patterns:
	#   /vX.Y.Z              (most GitHub tags)
	#   /R_X_Y_Z             (libexpat)
	#   /pcre2-X.Y           (pcre2)
	#   /sqlite-amalgamation-NNNNNNN.zip  (sqlite encoded packed integer)
	# Uses awk to avoid sed regex portability issues across BSD and GNU.
	if grep -q "^# Source:" "$cml"; then
		awk -v lib="$LIB" -v new="$V" '
			function sqlite_packed(v,    parts, n) {
				n = split(v, parts, ".")
				if (n < 3) return ""
				return sprintf("%d%02d%02d00", parts[1], parts[2], parts[3])
			}
			/^# Source:/ {
				r_new = "R_" new; gsub(/\./, "_", r_new)
				gsub(/\/v[0-9][0-9A-Za-z_.-]*/, "/v" new)
				gsub(/\/R_[0-9][0-9_]*/, "/" r_new)
				gsub(/\/pcre2-[0-9][0-9.]*/, "/pcre2-" new)
				if (lib == "sqlite3") {
					packed = sqlite_packed(new)
					if (packed != "") {
						gsub(/sqlite-amalgamation-[0-9]+\.zip/,
							"sqlite-amalgamation-" packed ".zip")
					}
				}
			}
			{ print }
		' "$cml" > "$cml.new" && mv "$cml.new" "$cml"
		echo "  $cml (# Source)"
	fi
fi

readme="$DEPS/README.md"
if [ -f "$readme" ]; then
	# Update the version cell in the Markdown table row for this library.
	# Use awk for portability: BSD and GNU sed disagree on extended regex
	# grouping with alternation, and character classes with "|" as the
	# negation target can trigger "empty (sub)expression" on BSD sed.
	awk -v lib="$LIB" -v ver="$V" '
		BEGIN { pat = "^\\| \\[" lib "\\]\\(" lib "/\\)" }
		$0 ~ pat {
			# Split row on "|" to locate and rewrite the version cell.
			n = split($0, cells, "|")
			# Cells are: "", " [lib](lib/) ... ", " OLD_VERSION ", ...
			if (n >= 3) {
				cells[3] = " " ver " "
				out = cells[1]
				for (i = 2; i <= n; i++) out = out "|" cells[i]
				print out
				next
			}
		}
		{ print }
	' "$readme" > "$readme.new" && mv "$readme.new" "$readme"
	echo "  $readme"
fi

echo
echo "Done. Review with: git diff dependencies/$LIB dependencies/README.md"
echo "Build and test before committing."
