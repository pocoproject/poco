#!/usr/bin/env bash
set -euo pipefail
action="${1:-start}"
names=(mysql postgres redis mongo sqlserver oracle)
if [[ "$action" == stop ]]; then
    containers="$(sudo -n docker container ls --all --format '{{.Names}}')"
    for name in "${names[@]}"; do
        if grep -Fxq "poco-nightly-$name" <<< "$containers"; then
            sudo -n docker rm -fv "poco-nightly-$name"
        fi
    done
    sudo -n systemctl stop squid
    exit
fi
bash "$0" stop
sudo -n systemctl start squid
sudo -n docker run -d --name poco-nightly-mysql --memory=1g -p 127.0.0.1:3306:3306 -e MYSQL_ALLOW_EMPTY_PASSWORD=yes -e MYSQL_USER=pocotest -e MYSQL_PASSWORD=pocotest -e MYSQL_DATABASE=pocotest mysql:8.1.0
sudo -n docker run -d --name poco-nightly-postgres --memory=512m -p 127.0.0.1:5432:5432 -p '[::1]:5432:5432' -e POSTGRES_PASSWORD=postgres postgres:16.0
sudo -n docker run -d --name poco-nightly-redis --memory=256m -p 127.0.0.1:6379:6379 -p '[::1]:6379:6379' redis:7
sudo -n docker run -d --name poco-nightly-mongo --memory=1g -p 127.0.0.1:27017:27017 mongo:7 --wiredTigerCacheSizeGB 0.25
sudo -n docker run -d --name poco-nightly-sqlserver --memory=3g -p 127.0.0.1:1433:1433 -e ACCEPT_EULA=Y -e MSSQL_PID=Express -e MSSQL_SA_PASSWORD=Pocopoco1 -e MSSQL_MEMORY_LIMIT_MB=2048 mcr.microsoft.com/mssql/server:2022-latest
sudo -n docker run -d --name poco-nightly-oracle --memory=3g -p 127.0.0.1:1521:1521 -e ORACLE_PWD=poco container-registry.oracle.com/database/free:23.5.0.0-lite
wait_service() {
    local service="$1"
    shift
    for attempt in $(seq 1 180); do
        if "$@" >/dev/null 2>&1; then return; fi
        echo "Waiting for $service ($attempt/180)"
        sleep 5
    done
    echo "Service did not become ready: $service" >&2
    sudo -n docker logs "poco-nightly-$service"
    return 1
}
wait_service mysql sudo -n docker exec poco-nightly-mysql mysqladmin ping
sudo -n docker exec poco-nightly-mysql mysql -uroot -e "GRANT SYSTEM_VARIABLES_ADMIN ON *.* TO 'pocotest'@'%';"
wait_service postgres sudo -n docker exec poco-nightly-postgres pg_isready
wait_service redis sudo -n docker exec poco-nightly-redis redis-cli ping
wait_service mongo sudo -n docker exec poco-nightly-mongo mongosh --quiet --eval 'quit(db.runCommand({ping:1}).ok ? 0 : 1)'
wait_service sqlserver sudo -n docker exec poco-nightly-sqlserver /opt/mssql-tools18/bin/sqlcmd -S localhost -U sa -P Pocopoco1 -C -b -Q 'SELECT 1'
wait_service oracle bash -c 'sudo -n docker logs poco-nightly-oracle 2>&1 | grep -q "DATABASE IS READY TO USE!"'
sudo -n docker exec -i poco-nightly-oracle sqlplus -L -s 'sys/poco@localhost:1521/FREEPDB1 as sysdba' <<'SQL'
WHENEVER SQLERROR EXIT FAILURE
CREATE USER poco IDENTIFIED BY poco;
GRANT CONNECT, RESOURCE, DBA TO poco;
GRANT UNLIMITED TABLESPACE TO poco;
EXIT;
SQL
