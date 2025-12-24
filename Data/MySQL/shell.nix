# Nix shell environment for running Poco::Data::MySQL tests.
#
# Usage:
#   nix-shell
#
# This sets up MariaDB with ODBC driver, creates a test database,
# and configures the Poco build environment. The database starts
# automatically and stops when the shell exits.
#
# Test database: pocotest (user: pocotest, password: pocotest)

{ pkgs ? import <nixpkgs> {} }:

let
  dataDir = builtins.toString ./.nix-data;
  mysqlDataDir = "${dataDir}/mysql";
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    # Database
    mariadb
    mariadb-connector-c
    unixODBC
    unixODBCDrivers.mariadb
    libtool  # for libltdl

    # Build tools
    cmake
    pkg-config
    gnumake
    gcc
    clang
    openssl
    zlib
    pcre2

    # Standard utilities needed by Poco build
    coreutils
    findutils
    gnugrep
    gnused
    gawk
    hostname
    which
    procps  # ps, pgrep, etc.
    inetutils  # netstat, etc.
  ];

  LIBRARY_PATH = "${pkgs.mariadb-connector-c}/lib";

  # ODBC configuration for Poco build
  POCO_ODBC_INCLUDE = "${pkgs.unixODBC}/include";
  POCO_ODBC_LIB = "${pkgs.unixODBC}/lib";

  ODBCSYSINI = "${dataDir}/odbc";
  ODBCINI = "${dataDir}/odbc/odbc.ini";

  shellHook = ''
    export DATA_DIR="${dataDir}"
    export MYSQL_DATADIR="${mysqlDataDir}"

    mkdir -p "$DATA_DIR/include" "$MYSQL_DATADIR" "$ODBCSYSINI"

    # Create mysql -> mariadb symlink for header compatibility
    ln -sfn ${pkgs.mariadb-connector-c.dev}/include/mariadb "$DATA_DIR/include/mysql"
    export C_INCLUDE_PATH="$DATA_DIR/include''${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"
    export CPLUS_INCLUDE_PATH="$DATA_DIR/include''${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"

    # Configure Poco and source environment
    POCO_BASE="$(cd "$DATA_DIR/../../.." && pwd)"
    (cd "$POCO_BASE" && ./configure)
    source "$POCO_BASE/poco_env.bash"

    # Add MariaDB ODBC driver library path (after poco_env.bash which sets LD_LIBRARY_PATH)
    export LD_LIBRARY_PATH="${pkgs.unixODBCDrivers.mariadb}/lib/mariadb''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    cat > "$ODBCSYSINI/odbcinst.ini" << 'EOF'
[MariaDB]
Description = MariaDB ODBC driver
Driver = ${pkgs.unixODBCDrivers.mariadb}/lib/mariadb/libmaodbc.so
FileUsage = 1
EOF

    cat > "$ODBCINI" << 'EOF'
[poco_mysql]
Description = Poco MySQL Test Database
Driver = MariaDB
Server = 127.0.0.1
Port = 3306
Database = pocotest
User = pocotest
Password = pocotest
EOF

    if [ ! -d "$MYSQL_DATADIR/mysql" ]; then
      echo "Initializing MariaDB..."
      mysql_install_db --datadir="$MYSQL_DATADIR" --auth-root-authentication-method=normal
    fi

    start_mysql() {
      if ! pgrep -f "mariadbd.*datadir=$MYSQL_DATADIR" > /dev/null; then
        echo "Starting MariaDB..."
        mariadbd --datadir="$MYSQL_DATADIR" --socket="$DATA_DIR/mysql.sock" --port=3306 --bind-address=127.0.0.1 &
        sleep 3
        mariadb -S "$DATA_DIR/mysql.sock" -u root << 'EOSQL'
CREATE DATABASE IF NOT EXISTS pocotest;
CREATE USER IF NOT EXISTS 'pocotest'@'localhost' IDENTIFIED BY 'pocotest';
CREATE USER IF NOT EXISTS 'pocotest'@'127.0.0.1' IDENTIFIED BY 'pocotest';
GRANT ALL PRIVILEGES ON pocotest.* TO 'pocotest'@'localhost';
GRANT ALL PRIVILEGES ON pocotest.* TO 'pocotest'@'127.0.0.1';
GRANT SUPER ON *.* TO 'pocotest'@'localhost';
GRANT SUPER ON *.* TO 'pocotest'@'127.0.0.1';
FLUSH PRIVILEGES;
EOSQL
        echo "MariaDB started"
      else
        echo "MariaDB already running"
      fi
    }

    stop_mysql() {
      pgrep -f "mariadbd.*datadir=$MYSQL_DATADIR" > /dev/null && \
        mariadb-admin -S "$DATA_DIR/mysql.sock" -u root shutdown 2>/dev/null || echo "MariaDB not running"
    }

    trap stop_mysql EXIT

    export -f start_mysql stop_mysql

    # Auto-start database
    start_mysql

    echo ""
    echo "=== Poco MySQL Test Environment ==="
    echo "MariaDB auto-started (will stop on shell exit)"
    echo "Connect: mariadb -S $DATA_DIR/mysql.sock -u pocotest -ppocotest pocotest"
    echo ""
  '';
}
