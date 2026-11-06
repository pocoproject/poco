#!/usr/bin/env bash
set -euo pipefail
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
sudo -n apt-get update
sudo -n env DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build pkg-config python3 git curl ca-certificates bison flex valgrind \
    libssl-dev libssh-dev libltdl-dev apache2-dev libapr1-dev libaprutil1-dev \
    libavahi-client-dev libsqlite3-dev libxml2-dev unixodbc-dev \
    libmariadb-dev-compat libpq-dev libbenchmark-dev docker.io docker-cli \
    odbc-postgresql odbc-mariadb libsqliteodbc odbcinst libcap2-bin libaio1t64 unzip squid
sudo -n env DEBIAN_FRONTEND=noninteractive apt-get install -y -t trixie-backports --no-install-recommends clang-21 clang-tools-21
sudo -n systemctl stop squid
sudo -n install -m 644 "$script_dir/poco-squid.conf" /etc/squid/squid.conf
sudo -n systemctl disable squid
sudo -n odbcinst -i -d -f "$script_dir/poco-odbc.ini"
mkdir -p "$HOME/poco-nightly/provision"
cd "$HOME/poco-nightly/provision"
curl -fsSLo packages-microsoft-prod.deb https://packages.microsoft.com/config/debian/13/packages-microsoft-prod.deb
sudo -n dpkg -i packages-microsoft-prod.deb
sudo -n apt-get update
sudo -n env ACCEPT_EULA=Y DEBIAN_FRONTEND=noninteractive apt-get install -y msodbcsql18
for package in basic odbc; do
    archive="instantclient-$package-linux.x64-23.26.1.0.0.zip"
    curl -fsSLO "https://download.oracle.com/otn_software/linux/instantclient/2326100/$archive"
    sudo -n mkdir -p /opt/oracle
    sudo -n unzip -oq "$archive" -d /opt/oracle
done
if [[ ! -e /usr/lib/x86_64-linux-gnu/libaio.so.1 ]]; then
    sudo -n ln -s libaio.so.1t64 /usr/lib/x86_64-linux-gnu/libaio.so.1
fi
cd /opt/oracle/instantclient_23_26
sudo -n ./odbc_update_ini.sh /
mkdir -p "$HOME/.local/lib/nightly-job-lock"
install -m 755 "$script_dir/job-lock.sh" "$HOME/.local/lib/nightly-job-lock/job-lock.sh"
install -m 644 "$script_dir/job-lock.py" "$HOME/.local/lib/nightly-job-lock/job-lock.py"
for image in mysql:8.1.0 postgres:16.0 redis:7 mongo:7 mcr.microsoft.com/mssql/server:2022-latest container-registry.oracle.com/database/free:23.5.0.0-lite; do
    sudo -n docker pull "$image"
done
