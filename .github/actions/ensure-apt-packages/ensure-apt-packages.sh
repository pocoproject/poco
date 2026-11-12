#!/usr/bin/env bash
#
# Installs the apt packages a CI job needs, but only those that are missing.
# ENSURE_APT_PACKAGES lists apt package names, checked with dpkg-query.
# ENSURE_APT_COMMANDS lists command:package pairs; the package is installed
# when the command is not found in PATH. Both lists are whitespace-separated.
# When everything is present, apt-get is not run and no mirror is contacted.

set -euo pipefail

read -r -d '' -a packages <<< "${ENSURE_APT_PACKAGES:-}" || true
read -r -d '' -a commands <<< "${ENSURE_APT_COMMANDS:-}" || true

if [[ ${#packages[@]} -eq 0 && ${#commands[@]} -eq 0 ]]; then
    echo "::error::ensure-apt-packages: no packages or commands given" >&2
    exit 1
fi

for entry in "${commands[@]}"; do
    if [[ ! "$entry" =~ ^[^:]+:[^:]+$ ]]; then
        echo "::error::ensure-apt-packages: invalid command:package pair '$entry'" >&2
        exit 1
    fi
done

missing=()
for pkg in "${packages[@]}"; do
    status=$(dpkg-query -W -f='${db:Status-Status}' "$pkg" 2>/dev/null || true)
    if [[ "$status" == "installed" ]]; then
        echo "present: $pkg"
    else
        missing+=("$pkg")
    fi
done

for entry in "${commands[@]}"; do
    cmd=${entry%%:*}
    if path=$(type -P "$cmd"); then
        echo "present: $cmd ($path)"
    else
        missing+=("${entry#*:}")
    fi
done

declare -A seen=()
install=()
for pkg in "${missing[@]}"; do
    if [[ -z "${seen[$pkg]:-}" ]]; then
        seen[$pkg]=1
        install+=("$pkg")
    fi
done

if [[ ${#install[@]} -eq 0 ]]; then
    echo "All required packages are present; apt-get is not run."
    exit 0
fi

echo "Installing missing packages: ${install[*]}"
sudo apt-get -y update
sudo apt-get -y install "${install[@]}"
