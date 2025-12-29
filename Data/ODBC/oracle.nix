# Nix shell environment for running Poco::Data::ODBC tests with Oracle.
#
# Install Nix (if not already installed):
#   sh <(curl -L https://nixos.org/nix/install) --daemon
#
# Usage:
#   nix-shell oracle.nix [--pure]
#
# (--pure is recommended to avoid interference from host environment)
#
# This sets up Oracle Free via Podman with the Oracle Instant Client ODBC driver,
# and configures the Poco build environment. The database starts
# automatically and stops when the shell exits.
#
# Test database: FREEPDB1 (user: poco, password: poco)
#
# Notes:
# - On first run, you may be prompted for sudo password to configure
#   subuid/subgid for rootless podman (one-time setup).
# - Oracle Instant Client is downloaded automatically on first run.
# - The Oracle Free container image is publicly available (no login required).
# - The Oracle container image is large (~1.5GB) and may take time to download.
# - Oracle startup takes several minutes; be patient on first run.

{ pkgs ? import <nixpkgs> {} }:

let
  dataDir = builtins.toString ./.nix-oracle-data;
  containerName = "poco-oracle-test";
  oraclePassword = "poco";
  oraclePort = "1521";
  instantClientVersion = "23_6";
  instantClientFullVersion = "23.6.0.0.0";
  instantClientPathVersion = "2360000";
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    # Container runtime
    podman

    # Database tools
    unixODBC
    libtool  # for libltdl
    libaio   # required by Oracle Instant Client

    # Download tools
    curl
    unzip
    cacert  # SSL certificates for curl

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

  # Oracle connection info for tests
  POCO_ODBC_TEST_DATABASE_SERVER = "127.0.0.1";

  shellHook = ''
    export DATA_DIR="${dataDir}"
    export CONTAINER_NAME="${containerName}"
    export ORACLE_PASSWORD="${oraclePassword}"
    export ORACLE_PORT="${oraclePort}"
    export INSTANT_CLIENT_VERSION="${instantClientVersion}"
    export INSTANT_CLIENT_DIR="$DATA_DIR/instantclient_${instantClientVersion}"

    mkdir -p "$DATA_DIR" "$ODBCSYSINI" "$HOME/.config/containers"

    # Set SSL certificate path for curl
    export SSL_CERT_FILE="${pkgs.cacert}/etc/ssl/certs/ca-bundle.crt"

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

    # Show Podman info for debugging CI issues
    echo "=== Podman Environment ==="
    podman --version
    echo "Podman info:"
    podman info --format '{{.Host.OS}} {{.Host.Arch}} {{.Host.MemTotal}} {{.Host.SwapTotal}}' 2>/dev/null || true
    echo "Storage driver: $(podman info --format '{{.Store.GraphDriverName}}' 2>/dev/null || echo 'unknown')"
    echo "==========================="

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

    # Download Oracle Instant Client if not present
    if [ ! -d "$INSTANT_CLIENT_DIR" ]; then
      (
        echo "Downloading Oracle Instant Client..."
        mkdir -p "$DATA_DIR/downloads"
        cd "$DATA_DIR/downloads"

        # Detect architecture and set download URLs
        # Note: ARM64 uses different URL format than x64
        ARCH=$(uname -m)
        if [ "$ARCH" = "x86_64" ]; then
          BASIC_URL="https://download.oracle.com/otn_software/linux/instantclient/${instantClientPathVersion}/instantclient-basic-linux.x64-${instantClientFullVersion}.zip"
          ODBC_URL="https://download.oracle.com/otn_software/linux/instantclient/${instantClientPathVersion}/instantclient-odbc-linux.x64-${instantClientFullVersion}.zip"
          BASIC_ZIP="instantclient-basic-linux.x64-${instantClientFullVersion}.zip"
          ODBC_ZIP="instantclient-odbc-linux.x64-${instantClientFullVersion}.zip"
        elif [ "$ARCH" = "aarch64" ]; then
          # ARM64 uses simplified URLs without version numbers
          BASIC_URL="https://download.oracle.com/otn_software/linux/instantclient/instantclient-basic-linux-arm64.zip"
          ODBC_URL="https://download.oracle.com/otn_software/linux/instantclient/instantclient-odbc-linux-arm64.zip"
          BASIC_ZIP="instantclient-basic-linux-arm64.zip"
          ODBC_ZIP="instantclient-odbc-linux-arm64.zip"
        else
          echo "ERROR: Unsupported architecture: $ARCH"
          exit 1
        fi

        echo "Downloading: $BASIC_URL"
        curl -L -o "$BASIC_ZIP" "$BASIC_URL" || { echo "Failed to download basic package"; exit 1; }

        echo "Downloading: $ODBC_URL"
        curl -L -o "$ODBC_ZIP" "$ODBC_URL" || { echo "Failed to download ODBC package"; exit 1; }

        echo "Extracting Oracle Instant Client..."
        unzip -o "$BASIC_ZIP" -d "$DATA_DIR"
        unzip -o "$ODBC_ZIP" -d "$DATA_DIR"
      )

      # Find the actual extracted directory (version may differ)
      EXTRACTED_DIR=$(ls -d "$DATA_DIR"/instantclient_* 2>/dev/null | head -1)
      if [ -n "$EXTRACTED_DIR" ] && [ "$EXTRACTED_DIR" != "$INSTANT_CLIENT_DIR" ]; then
        echo "Linking $EXTRACTED_DIR to $INSTANT_CLIENT_DIR"
        ln -sfn "$(basename "$EXTRACTED_DIR")" "$INSTANT_CLIENT_DIR"
      fi
    fi

    # Configure Poco and source environment
    POCO_BASE="$(cd "$DATA_DIR/../../.." && pwd)"
    (cd "$POCO_BASE" && ./configure)
    source "$POCO_BASE/poco_env.bash"

    # Add Oracle Instant Client and ODBC dependencies to library path
    export LD_LIBRARY_PATH="$INSTANT_CLIENT_DIR:${pkgs.libaio}/lib:${pkgs.openssl.out}/lib:${pkgs.stdenv.cc.cc.lib}/lib:${pkgs.unixODBC}/lib''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    # Find the ODBC driver library
    echo "Looking for Oracle ODBC driver in: $INSTANT_CLIENT_DIR"
    ls -la "$INSTANT_CLIENT_DIR"/libsqora* 2>/dev/null || echo "No libsqora files found"
    ODBC_DRIVER_LIB=$(ls "$INSTANT_CLIENT_DIR"/libsqora.so* 2>/dev/null | head -1)
    if [ -z "$ODBC_DRIVER_LIB" ]; then
      echo "WARNING: Oracle ODBC driver library not found in $INSTANT_CLIENT_DIR"
      echo "Contents of $INSTANT_CLIENT_DIR:"
      ls -la "$INSTANT_CLIENT_DIR/" 2>/dev/null || echo "(failed to list directory)"
      ODBC_DRIVER_LIB="$INSTANT_CLIENT_DIR/libsqora.so.23.1"
    fi
    echo "Using ODBC driver library: $ODBC_DRIVER_LIB"

    cat > "$ODBCSYSINI/odbcinst.ini" << EOF
[Oracle 23 ODBC driver]
Description = Oracle ODBC driver for Oracle 23
Driver = $ODBC_DRIVER_LIB
UsageCount = 1
EOF
    echo "Wrote odbcinst.ini:"
    cat "$ODBCSYSINI/odbcinst.ini"

    cat > "$ODBCINI" << EOF
[PocoDataOracleTest]
Description = Poco Oracle Test Database
Driver = Oracle 23 ODBC driver
ServerName = //127.0.0.1:${oraclePort}/FREEPDB1
UserID = poco
Password = poco
EOF

    # Flush files to disk to avoid potential race conditions
    sync

    # Verify ODBC configuration is correct
    echo "Verifying ODBC configuration..."
    odbcinst -j
    echo "Registered drivers:"
    odbcinst -q -d
    echo "Registered DSNs:"
    odbcinst -q -s

    start_oracle() {
      if ! podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
        # Check if container exists but is stopped
        if podman ps -a --format '{{.Names}}' | grep -q "^${containerName}$"; then
          echo "Starting existing Oracle container..."
          if ! podman start "$CONTAINER_NAME"; then
            echo "ERROR: Failed to start existing container. Removing and recreating..."
            podman rm -f "$CONTAINER_NAME" 2>/dev/null || true
          fi
        fi

        # Check if we need to create a new container
        if ! podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
          echo "Creating and starting Oracle container (this may take a while on first run)..."
          # Oracle requires at least 1GB of shared memory
          if ! podman run -d --replace \
            --name "$CONTAINER_NAME" \
            --shm-size=1g \
            -e "ORACLE_PWD=$ORACLE_PASSWORD" \
            -p "$ORACLE_PORT:1521" \
            container-registry.oracle.com/database/free:23.5.0.0-lite; then
            echo "ERROR: Failed to create Oracle container"
            echo "Container logs:"
            podman logs "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi
        fi

        # Wait for container to be in running state
        echo "Waiting for container to reach running state..."
        for i in $(seq 1 30); do
          CONTAINER_STATE=$(podman inspect --format '{{.State.Status}}' "$CONTAINER_NAME" 2>/dev/null || echo "unknown")
          echo "Container state: $CONTAINER_STATE"
          if [ "$CONTAINER_STATE" = "running" ]; then
            echo "Container is running"
            break
          elif [ "$CONTAINER_STATE" = "exited" ] || [ "$CONTAINER_STATE" = "dead" ]; then
            echo "ERROR: Container exited unexpectedly"
            echo "Container logs:"
            podman logs "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi
          if [ "$i" -eq 30 ]; then
            echo "ERROR: Container failed to reach running state within 60 seconds"
            echo "Container logs:"
            podman logs "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi
          sleep 2
        done

        echo "Waiting for Oracle to be ready (this may take several minutes)..."
        for i in $(seq 1 180); do
          # First verify container is still running
          CONTAINER_STATE=$(podman inspect --format '{{.State.Status}}' "$CONTAINER_NAME" 2>/dev/null || echo "unknown")
          if [ "$CONTAINER_STATE" != "running" ]; then
            echo "ERROR: Container stopped unexpectedly (state: $CONTAINER_STATE)"
            echo "Container logs:"
            podman logs "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi

          # Check if FREEPDB1 is accessible
          if podman exec "$CONTAINER_NAME" bash -c 'echo "SELECT 1 FROM DUAL;" | sqlplus -s sys/poco@localhost:1521/FREEPDB1 as sysdba 2>&1' | grep -q "^[[:space:]]*1"; then
            echo "Oracle FREEPDB1 is ready!"

            # Wait a bit more to ensure listener is fully stable
            sleep 5

            # Create test user if it doesn't exist
            echo "Creating test user 'poco'..."
            podman exec "$CONTAINER_NAME" bash -c 'sqlplus -s sys/poco@localhost:1521/FREEPDB1 as sysdba <<EOSQL
WHENEVER SQLERROR CONTINUE
CREATE USER poco IDENTIFIED BY poco;
GRANT CONNECT, RESOURCE, DBA TO poco;
GRANT UNLIMITED TABLESPACE TO poco;
EXIT;
EOSQL'

            return 0
          fi
          if [ "$i" -eq 180 ]; then
            echo "ERROR: Oracle did not become ready within 6 minutes"
            echo "Container logs (last 100 lines):"
            podman logs --tail 100 "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi
          # Progress indicator every 30 seconds
          if [ $((i % 15)) -eq 0 ]; then
            echo "Still waiting for Oracle... ($((i * 2)) seconds elapsed)"
          fi
          sleep 2
        done
      else
        echo "Oracle container already running"
      fi
    }

    stop_oracle() {
      if podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
        echo "Stopping Oracle container..."
        podman stop "$CONTAINER_NAME" > /dev/null 2>&1 || true
      fi
    }

    remove_oracle() {
      stop_oracle
      if podman ps -a --format '{{.Names}}' | grep -q "^${containerName}$"; then
        echo "Removing Oracle container..."
        podman rm "$CONTAINER_NAME" > /dev/null 2>&1 || true
      fi
    }

    oracle_shell() {
      podman exec -it "$CONTAINER_NAME" sqlplus poco/poco@localhost:1521/FREEPDB1
    }

    build_and_test() {
      # Pre-flight check: Verify Oracle container is running
      echo "=== Pre-flight checks ==="
      CONTAINER_STATE=$(podman inspect --format '{{.State.Status}}' "$CONTAINER_NAME" 2>/dev/null || echo "not_found")
      echo "Oracle container state: $CONTAINER_STATE"
      if [ "$CONTAINER_STATE" != "running" ]; then
        echo "ERROR: Oracle container is not running (state: $CONTAINER_STATE)"
        echo "Attempting to restart Oracle..."
        if ! start_oracle; then
          echo "ERROR: Failed to start Oracle container"
          return 1
        fi
      fi

      echo "Building Poco ODBC components..."
      make -s -j$(nproc) -C "$POCO_BASE/CppUnit"
      make -s -j$(nproc) -C "$POCO_BASE/Foundation"
      make -s -j$(nproc) -C "$POCO_BASE/Data"
      make -s -j$(nproc) -C "$POCO_BASE/Data/DataTest"
      make -s -j$(nproc) -C "$POCO_BASE/Data/ODBC"
      make -s -j$(nproc) -C "$POCO_BASE/Data/ODBC/testsuite"

      echo ""
      echo "=== ODBC Configuration Debug ==="
      echo "ODBCSYSINI=$ODBCSYSINI"
      echo "ODBCINSTINI=$ODBCINSTINI"
      echo "ODBCINI=$ODBCINI"
      echo ""
      echo "--- odbcinst.ini contents ---"
      cat "$ODBCSYSINI/odbcinst.ini"
      echo ""
      echo "--- odbc.ini contents ---"
      cat "$ODBCINI"
      echo ""
      echo "--- odbcinst verification ---"
      odbcinst -j
      echo ""
      echo "--- Registered drivers (odbcinst -q -d) ---"
      odbcinst -q -d
      echo ""
      echo "--- Registered DSNs (odbcinst -q -s) ---"
      odbcinst -q -s
      echo ""
      echo "--- Driver details ---"
      odbcinst -q -d -n "Oracle 23 ODBC driver" 2>&1 || echo "(failed to query driver details)"
      echo ""
      echo "--- DSN details ---"
      odbcinst -q -s -n "PocoDataOracleTest" 2>&1 || echo "(failed to query DSN details)"
      echo ""
      echo "--- Checking for system ODBC configs ---"
      ls -la /etc/odbcinst.ini 2>/dev/null || echo "/etc/odbcinst.ini not found"
      ls -la /etc/odbc.ini 2>/dev/null || echo "/etc/odbc.ini not found"
      echo ""
      echo "--- Verifying driver library exists ---"
      ls -la "$INSTANT_CLIENT_DIR"/libsqora* 2>/dev/null || echo "No libsqora files found"
      ldd "$INSTANT_CLIENT_DIR"/libsqora.so* 2>&1 | head -20 || echo "(ldd failed)"
      echo "================================"
      echo ""

      # Final container state check before running tests
      CONTAINER_STATE=$(podman inspect --format '{{.State.Status}}' "$CONTAINER_NAME" 2>/dev/null || echo "not_found")
      echo "Oracle container state before tests: $CONTAINER_STATE"
      if [ "$CONTAINER_STATE" != "running" ]; then
        echo "ERROR: Oracle container stopped during build!"
        return 1
      fi

      echo "Running ODBC tests..."
      "$POCO_BASE/Data/ODBC/testsuite/bin/$OSNAME/$OSARCH/testrunner" -all
    }

    trap stop_oracle EXIT

    export -f start_oracle stop_oracle remove_oracle oracle_shell build_and_test

    # Auto-start database
    if ! start_oracle; then
      echo ""
      echo "=== ORACLE CONTAINER FAILED TO START ==="
      echo "The Oracle container could not be started. See errors above."
      echo ""
      # In non-interactive mode (CI), exit with error
      if [ ! -t 0 ]; then
        exit 1
      fi
    fi

    # For CI, use: nix-shell oracle.nix --pure --run "build_and_test"
    if [ -t 0 ]; then
      echo ""
      echo "=== Poco Oracle ODBC Test Environment ==="
      echo "Container: $CONTAINER_NAME"
      echo ""
      echo "Commands:"
      echo "  build_and_test - Build and run tests"
      echo "  start_oracle   - Start the Oracle container"
      echo "  stop_oracle    - Stop the Oracle container"
      echo "  remove_oracle  - Stop and remove the Oracle container"
      echo "  oracle_shell   - Open SQL*Plus command line"
      echo ""
    fi
  '';
}
