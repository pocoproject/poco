# Nix shell environment for running Poco::Data::ODBC tests with SQL Server.
#
# Install Nix (if not already installed):
#   sh <(curl -L https://nixos.org/nix/install) --daemon
#
# Prerequisites (Debian/Ubuntu):
#   sudo apt-get install uidmap
#
# Usage:
#   nix-shell mssql.nix [--pure]
#
# (--pure is recommended to avoid interference from host environment)
#
# This sets up SQL Server via Podman with the Microsoft ODBC driver,
# and configures the Poco build environment. The database starts
# automatically and stops when the shell exits.
#
# Test database: model (user: sa, password: Pocopoco1)
#
# Notes:
# - On first run, you may be prompted for sudo password to configure
#   subuid/subgid for rootless podman (one-time setup).
# - The Microsoft ODBC driver (msodbcsql18) is unfree software;
#   this shell.nix automatically allows it.
#
# Cleanup/Reset:
# To completely reset the environment (e.g., for a fresh start):
#   podman stop poco-mssql-test && podman rm poco-mssql-test
#   rm -rf Data/ODBC/.nix-mssql-data
# This removes the container and all cached data (ODBC config).

{
  pkgs ? import <nixpkgs> {
    config = {
      allowUnfreePredicate = pkg:
        let name = pkg.pname or (builtins.parseDrvName pkg.name).name;
        in builtins.elem name [ "msodbcsql18" ];
    };
  }
}:

let
  dataDir = builtins.toString ./.nix-mssql-data;
  containerName = "poco-mssql-test";
  mssqlPassword = "Pocopoco1";
  mssqlPort = "1433";
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    # Container runtime
    podman

    # Database tools
    unixODBC
    unixODBCDrivers.msodbcsql18
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
  ODBCINSTINI = "odbcinst.ini";
  ODBCINI = "${dataDir}/odbc/odbc.ini";

  # SQL Server connection info for tests
  POCO_ODBC_TEST_DATABASE_SERVER = "127.0.0.1";

  shellHook = ''
    export DATA_DIR="${dataDir}"
    export CONTAINER_NAME="${containerName}"
    export MSSQL_PASSWORD="${mssqlPassword}"
    export MSSQL_PORT="${mssqlPort}"

    mkdir -p "$DATA_DIR" "$ODBCSYSINI" "$HOME/.config/containers"

    # Add system paths for setuid binaries needed by rootless podman
    export PATH="$PATH:/usr/bin:/usr/sbin"

    # Check for newuidmap (required for rootless podman)
    if ! command -v newuidmap >/dev/null 2>&1; then
      echo "ERROR: newuidmap not found. On Debian/Ubuntu, install with:"
      echo "  sudo apt-get install uidmap"
      echo "Then re-enter this nix-shell."
    fi

    # Check and configure subuid/subgid for rootless podman
    if ! grep -q "^$USER:" /etc/subuid 2>/dev/null; then
      echo "Rootless podman requires subuid/subgid configuration."
      SUDO_CMD=""
      for s in /usr/bin/sudo /run/wrappers/bin/sudo /usr/local/bin/sudo; do
        [ -x "$s" ] && SUDO_CMD="$s" && break
      done
      if [ -n "$SUDO_CMD" ]; then
        echo "Running: $SUDO_CMD usermod --add-subuids 100000-165535 --add-subgids 100000-165535 $USER"
        "$SUDO_CMD" usermod --add-subuids 100000-165535 --add-subgids 100000-165535 "$USER"
      else
        echo "ERROR: sudo not found. Please run outside the nix-shell:"
        echo "  sudo usermod --add-subuids 100000-165535 --add-subgids 100000-165535 $USER"
      fi
    fi

    # Always run podman system migrate to pick up PATH and subuid changes
    podman system migrate 2>/dev/null || true

    # Configure Podman policy for rootless mode if not present
    if [ ! -f "$HOME/.config/containers/policy.json" ]; then
      cat > "$HOME/.config/containers/policy.json" << 'POLICYJSON'
{
    "default": [
        {
            "type": "insecureAcceptAnything"
        }
    ]
}
POLICYJSON
    fi

    # Configure Poco and source environment
    POCO_BASE="$(cd "$DATA_DIR/../../.." && pwd)"
    (cd "$POCO_BASE" && ./configure)
    source "$POCO_BASE/poco_env.bash"

    # Add ODBC driver and its dependencies to library path (after poco_env.bash which sets LD_LIBRARY_PATH)
    export LD_LIBRARY_PATH="${pkgs.unixODBCDrivers.msodbcsql18}/lib:${pkgs.openssl.out}/lib:${pkgs.stdenv.cc.cc.lib}/lib:${pkgs.unixODBC}/lib''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    cat > "$ODBCSYSINI/odbcinst.ini" << EOF
[ODBC Driver 18 for SQL Server]
Description = Microsoft ODBC Driver 18 for SQL Server
Driver = ${pkgs.unixODBCDrivers.msodbcsql18}/lib/libmsodbcsql-18.1.so.1.1
UsageCount = 1
EOF

    cat > "$ODBCINI" << EOF
[PocoDataSQLServerTest]
Description = Poco SQL Server Test Database
Driver = ODBC Driver 18 for SQL Server
Server = 127.0.0.1
Port = 1433
Database = model
TrustServerCertificate = yes
Encrypt = no
EOF

    # Flush files to disk to avoid potential race conditions
    sync

    # Verify ODBC configuration
    echo "Verifying ODBC configuration..."
    odbcinst -j
    echo "Registered drivers:"
    odbcinst -q -d
    echo "Registered DSNs:"
    odbcinst -q -s

    start_mssql() {
      if ! podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
        # Check if container exists but is stopped
        if podman ps -a --format '{{.Names}}' | grep -q "^${containerName}$"; then
          echo "Starting existing SQL Server container..."
          podman start "$CONTAINER_NAME"
        else
          echo "Creating and starting SQL Server container..."
          podman run -d --replace \
            --name "$CONTAINER_NAME" \
            -e "ACCEPT_EULA=Y" \
            -e "MSSQL_SA_PASSWORD=$MSSQL_PASSWORD" \
            -e "MSSQL_PID=Express" \
            -p "$MSSQL_PORT:1433" \
            mcr.microsoft.com/mssql/server:2022-latest
        fi

        echo "Waiting for SQL Server to be ready..."
        for i in $(seq 1 30); do
          if podman exec "$CONTAINER_NAME" /opt/mssql-tools18/bin/sqlcmd \
            -S localhost -U sa -P "$MSSQL_PASSWORD" \
            -C -Q "SELECT 1" > /dev/null 2>&1; then
            echo "SQL Server is ready!"
            break
          fi
          if [ "$i" -eq 30 ]; then
            echo "Warning: SQL Server may not be fully ready yet"
          fi
          sleep 2
        done
      else
        echo "SQL Server container already running"
      fi
    }

    stop_mssql() {
      if podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
        echo "Stopping SQL Server container..."
        podman stop "$CONTAINER_NAME" > /dev/null 2>&1 || true
      fi
    }

    remove_mssql() {
      stop_mssql
      if podman ps -a --format '{{.Names}}' | grep -q "^${containerName}$"; then
        echo "Removing SQL Server container..."
        podman rm "$CONTAINER_NAME" > /dev/null 2>&1 || true
      fi
    }

    mssql_shell() {
      podman exec -it "$CONTAINER_NAME" /opt/mssql-tools18/bin/sqlcmd \
        -S localhost -U sa -P "$MSSQL_PASSWORD" -C
    }

    build_and_test() {
      echo "Building Poco ODBC components..."
      make -s -j$(nproc) -C "$POCO_BASE/CppUnit"
      make -s -j$(nproc) -C "$POCO_BASE/Foundation"
      make -s -j$(nproc) -C "$POCO_BASE/Data"
      make -s -j$(nproc) -C "$POCO_BASE/Data/DataTest"
      make -s -j$(nproc) -C "$POCO_BASE/Data/ODBC"
      make -s -j$(nproc) -C "$POCO_BASE/Data/ODBC/testsuite"

      echo "Running ODBC tests..."
      "$POCO_BASE/Data/ODBC/testsuite/bin/$OSNAME/$OSARCH/testrunner" -all
    }

    trap stop_mssql EXIT

    export -f start_mssql stop_mssql remove_mssql mssql_shell build_and_test

    # Auto-start database
    start_mssql

    # For CI, use: nix-shell mssql.nix --pure --run "build_and_test"
    if [ -t 0 ]; then
      echo ""
      echo "=== Poco SQL Server ODBC Test Environment ==="
      echo "Container: $CONTAINER_NAME"
      echo ""
      echo "Commands:"
      echo "  build_and_test - Build and run tests"
      echo "  start_mssql    - Start the SQL Server container"
      echo "  stop_mssql     - Stop the SQL Server container"
      echo "  remove_mssql   - Stop and remove the SQL Server container"
      echo "  mssql_shell    - Open SQL Server command line"
      echo ""
    fi
  '';
}
