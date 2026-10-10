# Nix shell environment for running Poco::Data::PostgreSQL tests.
#
# Install Nix (if not already installed):
#   sh <(curl -L https://nixos.org/nix/install) --daemon
#
# Usage:
#   nix-shell [--pure]
#
# (--pure is recommended to avoid interference from host environment)
#
# This sets up PostgreSQL with ODBC driver and configures the Poco
# build environment. The database starts automatically and stops
# when the shell exits.
#
# Test database: postgres (user: postgres, trust authentication)

{ pkgs ? import <nixpkgs> {} }:

let
  dataDir = builtins.toString ./.nix-data;
  pgDataDir = "${dataDir}/postgres";
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    # Database
    postgresql
    postgresql.lib
    unixODBC
    psqlodbc  # PostgreSQL ODBC driver
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

  # ODBC configuration for Poco build
  POCO_ODBC_INCLUDE = "${pkgs.unixODBC}/include";
  POCO_ODBC_LIB = "${pkgs.unixODBC}/lib";

  ODBCSYSINI = "${dataDir}/odbc";
  ODBCINI = "${dataDir}/odbc/odbc.ini";

  shellHook = ''
    export DATA_DIR="${dataDir}"
    export PGDATA="${pgDataDir}"

    mkdir -p "$DATA_DIR" "$PGDATA" "$ODBCSYSINI"

    # Configure Poco and source environment
    POCO_BASE="$(cd "$DATA_DIR/../../.." && pwd)"
    (cd "$POCO_BASE" && ./configure)
    source "$POCO_BASE/poco_env.bash"

    # Add PostgreSQL ODBC driver library path (after poco_env.bash which sets LD_LIBRARY_PATH)
    export LD_LIBRARY_PATH="${pkgs.psqlodbc}/lib''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    cat > "$ODBCSYSINI/odbcinst.ini" << EOF
[PostgreSQL]
Description = PostgreSQL ODBC driver
Driver = ${pkgs.psqlodbc}/lib/psqlodbcw.so
Setup = ${pkgs.psqlodbc}/lib/psqlodbcw.so
FileUsage = 1

[PostgreSQL ANSI]
Description = PostgreSQL ODBC driver (ANSI)
Driver = ${pkgs.psqlodbc}/lib/psqlodbca.so
Setup = ${pkgs.psqlodbc}/lib/psqlodbca.so
FileUsage = 1
EOF

    cat > "$ODBCINI" << 'EOF'
[poco_postgres]
Description = Poco PostgreSQL Test Database
Driver = PostgreSQL
Servername = localhost
Port = 5432
Database = postgres
Username = postgres
Password = postgres
EOF

    if [ ! -f "$PGDATA/PG_VERSION" ]; then
      echo "Initializing PostgreSQL..."
      initdb -D "$PGDATA" --no-locale --encoding=UTF8 -U postgres -A trust > /dev/null
      echo "unix_socket_directories = '$DATA_DIR'" >> "$PGDATA/postgresql.conf"
      echo "port = 5432" >> "$PGDATA/postgresql.conf"
      echo "PostgreSQL initialized."
    fi

    start_postgres() {
      if ! pg_ctl status -D "$PGDATA" > /dev/null 2>&1; then
        echo "Starting PostgreSQL..."
        pg_ctl start -D "$PGDATA" -l "$DATA_DIR/postgres.log" -o "-k $DATA_DIR"
        sleep 2
        echo "PostgreSQL started"
      else
        echo "PostgreSQL already running"
      fi
    }

    stop_postgres() {
      pg_ctl status -D "$PGDATA" > /dev/null 2>&1 && pg_ctl stop -D "$PGDATA" || true
    }

    trap stop_postgres EXIT

    export -f start_postgres stop_postgres

    # Auto-start database
    start_postgres

    echo ""
    echo "=== Poco PostgreSQL Test Environment ==="
    echo "PostgreSQL auto-started (will stop on shell exit)"
    echo "Connect: psql -h $DATA_DIR -U postgres postgres"
    echo ""
  '';
}
