#!/usr/bin/env bash
# The privileged part of the nightly. provision.sh installs this file as
# /usr/local/sbin/poco-nightly-root, owned by root, and allows the runner account to run
# it, and nothing else, through sudo (poco-nightly.sudoers). Its actions take no
# arguments, so a change in the repository cannot change what runs as root until the
# host is provisioned again.
#
#   services-start          start the proxy and the database containers and wait for them
#   services-stop           remove the database containers and stop the proxy
#   setcap-net-testrunner   allow raw sockets for the calling user's Net test runner
set -euo pipefail
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
names=(mysql postgres redis mongo sqlserver oracle)

services_stop() {
    local containers
    containers="$(docker container ls --all --format '{{.Names}}')"
    for name in "${names[@]}"; do
        if grep -Fxq "poco-nightly-$name" <<< "$containers"; then
            docker rm -fv "poco-nightly-$name"
        fi
    done
    systemctl stop squid
}

wait_service() {
    local service="$1"
    shift
    for attempt in $(seq 1 180); do
        if "$@" >/dev/null 2>&1; then return; fi
        echo "Waiting for $service ($attempt/180)"
        sleep 5
    done
    echo "Service did not become ready: $service" >&2
    docker logs "poco-nightly-$service"
    return 1
}

oracle_ready() {
    docker logs poco-nightly-oracle 2>&1 | grep -q "DATABASE IS READY TO USE!"
}

services_start() {
    services_stop
    systemctl start squid
    docker run -d --name poco-nightly-mysql --memory=1g -p 127.0.0.1:3306:3306 -e MYSQL_ALLOW_EMPTY_PASSWORD=yes -e MYSQL_USER=pocotest -e MYSQL_PASSWORD=pocotest -e MYSQL_DATABASE=pocotest mysql:8.1.0
    docker run -d --name poco-nightly-postgres --memory=512m -p 127.0.0.1:5432:5432 -p '[::1]:5432:5432' -e POSTGRES_PASSWORD=postgres postgres:16.0
    docker run -d --name poco-nightly-redis --memory=256m -p 127.0.0.1:6379:6379 -p '[::1]:6379:6379' redis:7
    docker run -d --name poco-nightly-mongo --memory=1g -p 127.0.0.1:27017:27017 mongo:7 --wiredTigerCacheSizeGB 0.25
    docker run -d --name poco-nightly-sqlserver --memory=3g -p 127.0.0.1:1433:1433 -e ACCEPT_EULA=Y -e MSSQL_PID=Express -e MSSQL_SA_PASSWORD=Pocopoco1 -e MSSQL_MEMORY_LIMIT_MB=2048 mcr.microsoft.com/mssql/server:2022-latest
    docker run -d --name poco-nightly-oracle --memory=3g -p 127.0.0.1:1521:1521 -e ORACLE_PWD=poco container-registry.oracle.com/database/free:23.5.0.0-lite
    wait_service mysql docker exec poco-nightly-mysql mysql --protocol=TCP -h127.0.0.1 -upocotest -ppocotest pocotest -e 'SELECT 1'
    docker exec poco-nightly-mysql mysql -uroot -e "GRANT SYSTEM_VARIABLES_ADMIN ON *.* TO 'pocotest'@'%';"
    wait_service postgres docker exec poco-nightly-postgres pg_isready
    wait_service redis docker exec poco-nightly-redis redis-cli ping
    wait_service mongo docker exec poco-nightly-mongo mongosh --quiet --eval 'quit(db.runCommand({ping:1}).ok ? 0 : 1)'
    wait_service sqlserver docker exec poco-nightly-sqlserver /opt/mssql-tools18/bin/sqlcmd -S localhost -U sa -P Pocopoco1 -C -b -Q 'SELECT 1'
    wait_service oracle oracle_ready
    docker exec -i poco-nightly-oracle sqlplus -L -s 'sys/poco@localhost:1521/FREEPDB1 as sysdba' <<'SQL'
WHENEVER SQLERROR EXIT FAILURE
CREATE USER poco IDENTIFIED BY poco;
GRANT CONNECT, RESOURCE, DBA TO poco;
GRANT UNLIMITED TABLESPACE TO poco;
EXIT;
SQL
}

setcap_net_testrunner() {
    # only the calling user's own build of the Net test runner, not a link to another file
    local user="${SUDO_USER:?run through sudo}" home runner
    home="$(getent passwd "$user" | cut -d: -f6)"
    runner="$home/poco-nightly/build/bin/Net-testrunner"
    if [[ -L "$runner" || ! -f "$runner" || "$(stat -c %U "$runner")" != "$user" ]]; then
        echo "Not a Net test runner of $user: $runner" >&2
        exit 1
    fi
    setcap cap_net_raw+ep "$runner"
}

if [[ $# -ne 1 ]]; then
    echo "usage: $0 services-start|services-stop|setcap-net-testrunner" >&2
    exit 2
fi
case "$1" in
    services-start) services_start ;;
    services-stop) services_stop ;;
    setcap-net-testrunner) setcap_net_testrunner ;;
    *) echo "unknown action: $1" >&2; exit 2 ;;
esac
