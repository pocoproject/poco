#!/usr/bin/env bash
# Full nightly build and test of POCO on Linux: every Linux component with tests, samples,
# tools, benchmarks and modules, and every test suite with -long. Nothing is ignored or
# retried; only the Db2 and Informix ODBC suites may be absent. A second build runs the ODBC
# suites on the wide-character ODBC API, whose tests the first build skips.
set -euo pipefail
nightly_root="$HOME/poco-nightly"
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source_ref="${POCO_REF:-main}"
build="$nightly_root/build"
unicode_build="$nightly_root/build-unicode"
logs="$nightly_root/logs"
trap 'bash "$script_dir/poco-test-services.sh" stop' EXIT
trap 'exit 143' TERM
trap 'exit 130' INT
bash "$script_dir/poco-test-services.sh" stop
mkdir -p "$nightly_root"
cd "$nightly_root"
if [[ ! -d source/.git ]]; then
    git clone --depth 1 --branch main https://github.com/pocoproject/poco.git source
fi
git -C source fetch --depth 1 -- origin "$source_ref"
git -C source reset --hard FETCH_HEAD
git -C source clean -ffdx
rm -rf "$logs" "$build" "$unicode_build" "$nightly_root/tmp"
mkdir -p "$logs" "$nightly_root/tmp"
# Tests create files with fixed names in the temporary directory; a shared /tmp can hold
# files of another account with the same names.
export TMPDIR="$nightly_root/tmp"
git -C source rev-parse HEAD > "$logs/commit.txt"
cmake -S source -B "$build" -G Ninja \
    -DCMAKE_C_COMPILER=clang-21 -DCMAKE_CXX_COMPILER=clang++-21 \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_TESTS=ON -DENABLE_TEST_DEPRECATED=ON -DENABLE_SAMPLES=ON \
    -DENABLE_MODULES=ON -DENABLE_BENCHMARK=ON \
    -DENABLE_CRYPTO=ON -DENABLE_NETSSL=ON -DENABLE_JWT=ON \
    -DENABLE_DATA_MYSQL=ON -DENABLE_DATA_POSTGRESQL=ON -DENABLE_DATA_ODBC=ON \
    -DENABLE_DATA_SQL_SERVER_BIG_STRINGS=ON \
    -DENABLE_DATA_SQLITE_VEC=ON -DENABLE_DATA_SQLITE_FTS5=ON -DENABLE_APACHECONNECTOR=ON \
    -DENABLE_SSH=ON -DENABLE_PDF=ON -DENABLE_SEVENZIP=ON \
    -DENABLE_CPPPARSER=ON -DENABLE_POCODOC=ON -DENABLE_ENCODINGS_COMPILER=ON \
    -DENABLE_DNSSD=ON -DENABLE_DNSSD_DEFAULT=ON -DENABLE_XSD_VALIDATOR=ON \
    -DENABLE_AI=ON -DENABLE_AI_MCP=ON -DENABLE_AI_MCP_HTTP=ON \
    2>&1 | tee "$logs/configure.log"
for feature in SSH XSD_VALIDATOR MODULES BENCHMARK DATA_SQLITE_FTS5 DATA_SQL_SERVER_BIG_STRINGS; do
    if ! grep -Fxq "ENABLE_${feature}:BOOL=ON" "$build/CMakeCache.txt"; then
        echo "Required feature was disabled: $feature" >&2
        exit 1
    fi
done
cmake --build "$build" --parallel 4 2>&1 | tee "$logs/build.log"
# A failed Unicode build fails the job after the other tests have run.
unicode_built=0
cmake -S source -B "$unicode_build" -G Ninja \
    -DCMAKE_C_COMPILER=clang-21 -DCMAKE_CXX_COMPILER=clang++-21 \
    -DCMAKE_BUILD_TYPE=Release -DPOCO_MINIMAL_BUILD=ON -DENABLE_TESTS=ON \
    -DENABLE_DATA_ODBC=ON -DENABLE_DATA_SQL_SERVER_BIG_STRINGS=ON -DPOCO_DATA_ODBC_UNICODE=ON \
    > "$logs/configure-unicode.log" 2>&1 \
    && cmake --build "$unicode_build" --target DataODBC-testrunner --parallel 4 > "$logs/build-unicode.log" 2>&1 \
    && unicode_built=1
test -x "$build/bin/Benchmark"
test -x "$build/bin/sample-json-benchmark"
sudo -n /usr/local/sbin/poco-nightly-root setcap-net-testrunner
bash "$script_dir/poco-test-services.sh" start 2>&1 | tee "$logs/services.log"

# Every suite runs with -long and without ignore lists; DataODBC runs each database suite.
find "$build" -name CTestTestfile.cmake -exec sed -i -E \
    's/ "-ignore" "[^"]+"//g; s/"-all" "-long"/"-all"/g; s/"-all"/"-all" "-long"/g' {} +
sed -i 's/"-all" "-long"/"-long" "ODBCOracleTest" "ODBCSQLServerTest" "ODBCMySQLTest" "ODBCPostgreSQLTest" "ODBCSQLiteTest"/' \
    "$build/Data/ODBC/testsuite/CTestTestfile.cmake"
# Crypto runs with the FIPS provider available; NetSSL uses the local proxy.
openssl fipsinstall -module /usr/lib/x86_64-linux-gnu/ossl-modules/fips.so -out "$nightly_root/fipsmodule.cnf"
cat > "$build/Crypto/testsuite/nightly-openssl.cnf" <<EOF
.include /etc/ssl/openssl.cnf
.include $nightly_root/fipsmodule.cnf
[openssl_init]
providers = nightly_providers
[nightly_providers]
default = nightly_default
fips = fips_sect
[nightly_default]
activate = 1
EOF
echo "set_tests_properties(Crypto PROPERTIES ENVIRONMENT \"OPENSSL_CONF=$build/Crypto/testsuite/nightly-openssl.cnf\")" \
    >> "$build/Crypto/testsuite/CTestTestfile.cmake"
sed -i -e 's|<host>proxy.aon.at</host>|<host>127.0.0.1</host>|' -e 's|<port>8080</port>|<port>3128</port>|' \
    "$build/bin/NetSSL-testrunner.xml"

unset CPPUNIT_IGNORE
export POCO_BASE="$nightly_root/source"
export PATH="$PATH:$build/bin"
export LD_LIBRARY_PATH="$build/lib:$build/bin:/opt/oracle/instantclient_23_26"
status=0
: > "$logs/coverage-gaps.txt"
required="Foundation Encodings XML JSON Util Net MongoDB Redis SSH Prometheus AIMCPHTTP AIMCP AI PDF JWT NetSSL
    Crypto Crypto-legacy-provider-unavailable Data DataSQLite DataMySQL DataPostgreSQL DataODBC ActiveRecord Zip
    CppParser CodeGeneration RemotingNGTCP RemotingNGHTTP RemotingNGJSONRPC RemotingNGREST RemotingNGSOAP RemotingNG
    XSDTypes XSDParser XSDGen-ElementNameInjection XSDGen-SoapActionInjection XSDValidator Geo Serial PageCompiler
    Modules"
listed="$(ctest --test-dir "$build" -N | sed -n -E 's/^ *Test +#[0-9]+: //p')"
for suite in $required; do
    if ! grep -Fxq "$suite" <<< "$listed"; then
        echo "Missing CTest suite: $suite" >> "$logs/coverage-gaps.txt"
    fi
done
ctest --test-dir "$build" --verbose --no-tests=error --timeout 7200 --parallel 1 \
    --output-junit "$logs/tests.xml" > "$logs/tests.log" 2>&1 || status=1
# These suites register no tests and pass when they cannot connect to their server, so the
# banner each prints after connecting must be in the log.
for banner in '*** Connected to [MySQL] test database.' '*** Connected to [PostgreSQL] test database.' \
    'Connected to [127.0.0.1:27017]'; do
    if ! grep -aFq "$banner" "$logs/tests.log"; then
        echo "Server missing, no \"$banner\" in tests.log" >> "$logs/coverage-gaps.txt"
    fi
done
# SQL Server reads and writes narrow strings in the character set of the client locale, which
# testEncoding expects to be Latin-1.
if [[ "$unicode_built" != 1 ]]; then
    echo "The Unicode ODBC build failed; see configure-unicode.log and build-unicode.log" >> "$logs/coverage-gaps.txt"
fi
if ! locale -a | grep -qix 'en_US.iso88591'; then
    echo "Missing locale en_US.ISO-8859-1 for the Unicode SQL Server suite" >> "$logs/coverage-gaps.txt"
fi
if [[ "$unicode_built" == 1 ]]; then
    for suite in ODBCOracleTest ODBCSQLServerTest ODBCMySQLTest ODBCPostgreSQLTest ODBCSQLiteTest; do
        locale=C.UTF-8
        [[ "$suite" == ODBCSQLServerTest ]] && locale=en_US.ISO-8859-1
        echo "=== $suite (LC_ALL=$locale)" >> "$logs/tests-unicode.log"
        (cd "$unicode_build/Data/ODBC/testsuite" && \
            LC_ALL="$locale" LD_LIBRARY_PATH="$unicode_build/lib:$LD_LIBRARY_PATH" \
            timeout 7200 "$unicode_build/bin/DataODBC-testrunner" -long "$suite") \
            >> "$logs/tests-unicode.log" 2>&1 || { echo "=== $suite failed" >> "$logs/tests-unicode.log"; status=1; }
    done
fi
# A suite whose driver or server is missing drops its tests without failing; only Db2 and Informix may.
# The first build skips the tests of the wide-character API, which the second one runs.
grep -a -i -E '(FAILURE|ERROR) \(ignored\)|\btests? (skipped|not available|not executed)\b|\bskipping\b|no pseudo terminal available|SQL Server extensions not enabled' \
    "$logs/tests.log" "$logs/tests-unicode.log" \
    | grep -a -i -v -E '(IBM DB2 ODBC DRIVER - DB2COPY1|Informix ODBC Driver) driver NOT found, tests not available\.' \
    | grep -a -v -E ': NOTICE: |testGetLoggerCollision: Skipping configuration for logger' \
    | grep -a -v -E '^[^:]*tests\.log:[0-9]+: ODBC[A-Za-z]+Test::test(Unicode|Encoding): Not an UNICODE build, skipping\.$' \
    >> "$logs/coverage-gaps.txt" || true
if [[ ! -f "$logs/tests.xml" ]]; then
    echo "CTest did not produce its JUnit report" >> "$logs/coverage-gaps.txt"
elif grep -q '<skipped' "$logs/tests.xml"; then
    echo "CTest skipped tests" >> "$logs/coverage-gaps.txt"
fi
make -C source/Data/SQLParser -j4 test > "$logs/sqlparser.log" 2>&1 || status=1
# Without valgrind the SQLParser tests skip their leak check and still pass.
if ! sed -E 's/\x1b\[[0-9;]*m//g' "$logs/sqlparser.log" | grep -a -q -E '^Memory Leak Check: +Success'; then
    echo "The SQLParser memory leak check did not run or did not pass" >> "$logs/coverage-gaps.txt"
fi
bash "$script_dir/poco-nightly-summary.sh" "$logs" || { echo "The test summary failed"; status=1; }
if [[ -s "$logs/coverage-gaps.txt" ]]; then
    echo "Missing coverage or ignored failures:"
    cat "$logs/coverage-gaps.txt"
    status=1
fi
exit "$status"
