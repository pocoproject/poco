#!/usr/bin/env bash
# Starts or stops the proxy and the database containers of the nightly through the
# privileged helper that provision.sh installs (poco-nightly-root.sh).
set -euo pipefail
action="${1:-start}"
case "$action" in
    start|stop) exec sudo -n /usr/local/sbin/poco-nightly-root "services-$action" ;;
    *) echo "usage: $0 start|stop" >&2; exit 2 ;;
esac
