# Nix shell environment for running Poco::Data::ODBC tests with Informix.
#
# Install Nix (if not already installed):
#   sh <(curl -L https://nixos.org/nix/install) --daemon
#
# Usage:
#   nix-shell informix.nix [--pure]
#
# (--pure is recommended to avoid interference from host environment)
#
# This sets up IBM Informix Developer Edition via Podman with the Informix
# ODBC driver (extracted from the container), and configures the Poco build
# environment. The database starts automatically and stops when the shell exits.
#
# Test database: poco_test (user: informix, password: poco)
#
# Notes:
# - On first run, you may be prompted for sudo password to configure
#   subuid/subgid for rootless podman (one-time setup).
# - The Informix ODBC driver is extracted from the container on first run.
# - The Informix container image may take time to download on first run.
# - Informix startup takes a minute or two; be patient on first run.
#
# Cleanup/Reset:
# To completely reset the environment (e.g., for a fresh start):
#   podman stop poco-informix-test && podman rm poco-informix-test
#   rm -rf Data/ODBC/.nix-informix-data
# This removes the container and all cached data (ODBC config, Informix client).

{ pkgs ? import <nixpkgs> {} }:

let
  dataDir = builtins.toString ./.nix-informix-data;
  containerName = "poco-informix-test";
  informixPassword = "poco";
  informixPort = "9088";
in
pkgs.mkShell {
  buildInputs = with pkgs; [
    # Container runtime
    podman

    # Database tools
    unixODBC
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

  # Informix connection info for tests
  POCO_ODBC_TEST_DATABASE_SERVER = "127.0.0.1";

  shellHook = ''
    export DATA_DIR="${dataDir}"
    export CONTAINER_NAME="${containerName}"
    export INFORMIX_PASSWORD="${informixPassword}"
    export INFORMIX_PORT="${informixPort}"
    export INFORMIX_CLIENT_DIR="$DATA_DIR/informix_client"

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

    # Add ODBC dependencies to library path (after poco_env.bash which sets LD_LIBRARY_PATH)
    export LD_LIBRARY_PATH="$INFORMIX_CLIENT_DIR/lib:$INFORMIX_CLIENT_DIR/lib/esql:$INFORMIX_CLIENT_DIR/lib/cli:${pkgs.openssl.out}/lib:${pkgs.stdenv.cc.cc.lib}/lib:${pkgs.unixODBC}/lib''${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

    # Find the ODBC driver library.
    # The CSDK (Client SDK) must be installed separately — it is NOT included
    # in the Informix Developer Edition container image.
    ODBC_DRIVER_LIB=""
    for candidate in \
      "$INFORMIX_CLIENT_DIR/lib/cli/libifcli.so" \
      "$INFORMIX_CLIENT_DIR/lib/libifcli.so" \
      ; do
      if [ -f "$candidate" ]; then
        ODBC_DRIVER_LIB="$candidate"
        break
      fi
    done

    if [ -z "$ODBC_DRIVER_LIB" ]; then
      echo "WARNING: Informix ODBC driver library (libifcli.so) not found."
      echo "Install the IBM Informix Client SDK (CSDK) into $INFORMIX_CLIENT_DIR"
      echo "See: https://www.ibm.com/support/pages/download-informix-products"
      ODBC_DRIVER_LIB="$INFORMIX_CLIENT_DIR/lib/cli/libifcli.so"
    else
      echo "Using ODBC driver library: $ODBC_DRIVER_LIB"
    fi

    cat > "$ODBCSYSINI/odbcinst.ini" << EOF
[Informix ODBC Driver]
Description = IBM Informix ODBC Driver
Driver = $ODBC_DRIVER_LIB
UsageCount = 1
EOF

    cat > "$ODBCINI" << EOF
[PocoDataInformixTest]
Description = Poco Informix Test Database
Driver = Informix ODBC Driver
Server = ol_informix
Host = 127.0.0.1
Service = ${informixPort}
Protocol = onsoctcp
Database = poco_test
LogonID = informix
pwd = ${informixPassword}
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

    start_informix() {
      if ! podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
        # Check if container exists but is stopped
        if podman ps -a --format '{{.Names}}' | grep -q "^${containerName}$"; then
          echo "Starting existing Informix container..."
          if ! podman start "$CONTAINER_NAME"; then
            echo "ERROR: Failed to start existing container. Removing and recreating..."
            podman rm -f "$CONTAINER_NAME" 2>/dev/null || true
          fi
        fi

        # Check if we need to create a new container
        if ! podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
          echo "Creating and starting Informix container..."

          # Note: IBM only provides amd64 images for Informix.
          # On ARM hosts, podman uses qemu emulation automatically.
          INFORMIX_IMAGE="icr.io/informix/informix-developer-database:latest"
          ARCH=$(uname -m)
          if [ "$ARCH" = "aarch64" ] || [ "$ARCH" = "arm64" ]; then
            echo "NOTE: Informix image is amd64-only; running under x86 emulation."
          fi

          if ! podman run -d --replace \
            --name "$CONTAINER_NAME" \
            --platform=linux/amd64 \
            -e "LICENSE=accept" \
            -e "INFORMIX_PASSWORD=$INFORMIX_PASSWORD" \
            -p "$INFORMIX_PORT:9088" \
            --privileged \ # Required: Informix needs shared memory (IPC) setup
            "$INFORMIX_IMAGE"; then
            echo "ERROR: Failed to create Informix container"
            echo "Container logs:"
            podman logs "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi
        fi

        echo "Waiting for Informix to be ready..."
        for i in $(seq 1 90); do
          # Verify container is still running
          CONTAINER_STATE=$(podman inspect --format '{{.State.Status}}' "$CONTAINER_NAME" 2>/dev/null || echo "unknown")
          if [ "$CONTAINER_STATE" != "running" ]; then
            echo "ERROR: Container stopped unexpectedly (state: $CONTAINER_STATE)"
            echo "Container logs:"
            podman logs "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi

          # Check if Informix is accepting connections
          if podman exec "$CONTAINER_NAME" bash -c 'echo "SELECT 1 FROM systables WHERE tabid=1;" | dbaccess sysmaster - 2>/dev/null' 2>/dev/null | grep -q "1"; then
            echo "Informix is ready!"

            # Create test database if it does not exist
            echo "Creating test database 'poco_test'..."
            podman exec "$CONTAINER_NAME" bash -c '
              echo "CREATE DATABASE IF NOT EXISTS poco_test WITH LOG;" | dbaccess sysmaster - 2>/dev/null
            ' || true

            return 0
          fi
          if [ "$i" -eq 90 ]; then
            echo "ERROR: Informix did not become ready within 3 minutes"
            echo "Container logs (last 100 lines):"
            podman logs --tail 100 "$CONTAINER_NAME" 2>&1 || echo "(no logs available)"
            return 1
          fi
          # Progress indicator every 30 seconds
          if [ $((i % 15)) -eq 0 ]; then
            echo "Still waiting for Informix... ($((i * 2)) seconds elapsed)"
          fi
          sleep 2
        done
      else
        echo "Informix container already running"
      fi
    }

    stop_informix() {
      if podman ps --format '{{.Names}}' | grep -q "^${containerName}$"; then
        echo "Stopping Informix container..."
        podman stop "$CONTAINER_NAME" > /dev/null 2>&1 || true
      fi
    }

    remove_informix() {
      stop_informix
      if podman ps -a --format '{{.Names}}' | grep -q "^${containerName}$"; then
        echo "Removing Informix container..."
        podman rm "$CONTAINER_NAME" > /dev/null 2>&1 || true
      fi
    }

    informix_shell() {
      podman exec -it "$CONTAINER_NAME" bash -c 'dbaccess sysmaster -'
    }

    build_and_test() {
      # Pre-flight check: Verify Informix container is running
      echo "=== Pre-flight checks ==="
      CONTAINER_STATE=$(podman inspect --format '{{.State.Status}}' "$CONTAINER_NAME" 2>/dev/null || echo "not_found")
      echo "Informix container state: $CONTAINER_STATE"
      if [ "$CONTAINER_STATE" != "running" ]; then
        echo "ERROR: Informix container is not running (state: $CONTAINER_STATE)"
        echo "Attempting to restart Informix..."
        if ! start_informix; then
          echo "ERROR: Failed to start Informix container"
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

      echo "Running ODBC tests..."
      "$POCO_BASE/Data/ODBC/testsuite/bin/$OSNAME/$OSARCH/testrunner" -all
    }

    trap stop_informix EXIT

    export -f start_informix stop_informix remove_informix informix_shell build_and_test

    # Auto-start database
    if ! start_informix; then
      echo ""
      echo "=== INFORMIX CONTAINER FAILED TO START ==="
      echo "The Informix container could not be started. See errors above."
      echo ""
      # In non-interactive mode (CI), exit with error
      if [ ! -t 0 ]; then
        exit 1
      fi
    fi

    # For CI, use: nix-shell informix.nix --pure --run "build_and_test"
    if [ -t 0 ]; then
      echo ""
      echo "=== Poco Informix ODBC Test Environment ==="
      echo "Container: $CONTAINER_NAME"
      echo ""
      echo "Commands:"
      echo "  build_and_test   - Build and run tests"
      echo "  start_informix   - Start the Informix container"
      echo "  stop_informix    - Stop the Informix container"
      echo "  remove_informix  - Stop and remove the Informix container"
      echo "  informix_shell   - Open dbaccess command line"
      echo ""
    fi
  '';
}
