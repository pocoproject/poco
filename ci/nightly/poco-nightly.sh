#!/usr/bin/env bash
set -euo pipefail
nightly_root="$HOME/poco-nightly"
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$nightly_root"
exec 9>"$nightly_root/run.lock"
flock 9
trap 'bash "$script_dir/poco-test-services.sh" stop' EXIT
trap 'exit 143' TERM
trap 'exit 130' INT
bash "$script_dir/poco-test-services.sh" stop
cd "$nightly_root"
if [[ ! -d source/.git ]]; then
    git clone --depth 1 --branch main https://github.com/pocoproject/poco.git source
fi
git -C source fetch --depth 1 origin main
git -C source reset --hard origin/main
git -C source clean -ffdx
rm -rf logs
mkdir -p logs
git -C source rev-parse HEAD > logs/commit.txt
rm -rf build
cmake -S source -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_TESTS=ON -DENABLE_TEST_DEPRECATED=ON -DENABLE_SAMPLES=ON \
    -DENABLE_MODULES=ON -DENABLE_BENCHMARK=ON \
    -DENABLE_CRYPTO=ON -DENABLE_NETSSL=ON -DENABLE_JWT=ON \
    -DENABLE_DATA_MYSQL=ON -DENABLE_DATA_POSTGRESQL=ON -DENABLE_DATA_ODBC=ON \
    -DENABLE_DATA_SQLITE_VEC=ON -DENABLE_APACHECONNECTOR=ON \
    -DENABLE_SSH=ON -DENABLE_PDF=ON -DENABLE_SEVENZIP=ON \
    -DENABLE_CPPPARSER=ON -DENABLE_POCODOC=ON -DENABLE_ENCODINGS_COMPILER=ON \
    -DENABLE_DNSSD=ON -DENABLE_DNSSD_DEFAULT=ON -DENABLE_XSD_VALIDATOR=ON \
    -DENABLE_AI=ON -DENABLE_AI_MCP=ON -DENABLE_AI_MCP_HTTP=ON \
    2>&1 | tee logs/configure.log
cmake --build build --parallel 4 2>&1 | tee logs/build.log
sudo -n setcap cap_net_raw+ep build/bin/Net-testrunner
bash "$script_dir/poco-test-services.sh" start 2>&1 | tee logs/services.log
python3 "$script_dir/poco-strict-tests.py"
