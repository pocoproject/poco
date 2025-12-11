#!/bin/bash
#
# compare.sh - Run benchmarks and display summary
#
# Usage: ./compare.sh [filter] [repetitions]
#   filter: benchmark filter regex (default: ".*")
#   repetitions: number of repetitions (default: 5)
#

FILTER="${1:-.*}"
REPS="${2:-5}"
OUTFILE="/tmp/benchmark_results.json"

# Detect OS and architecture
OSNAME=$(uname)
case $(uname -m) in
    x86_64)  OSARCH="x86_64" ;;
    aarch64) OSARCH="aarch64" ;;
    arm64)   OSARCH="arm64" ;;
    *)       OSARCH=$(uname -m) ;;
esac

# Find the benchmark binary
if [[ -z "$POCO_BASE" ]]; then
    echo "Error: POCO_BASE not set" >&2
    exit 1
fi

BENCHMARK="$POCO_BASE/Benchmark/bin/$OSNAME/$OSARCH/benchmark"

if [[ ! -x "$BENCHMARK" ]]; then
    echo "Error: benchmark binary not found at $BENCHMARK" >&2
    exit 1
fi

# Run benchmark with JSON output to file
"$BENCHMARK" --filter="$FILTER" --repetitions="$REPS" --aggregates-only --output="$OUTFILE" --output-format=json
BENCH_STATUS=$?

echo ""
echo "================================================================================"
echo "BENCHMARK SUMMARY"
echo "================================================================================"
echo ""

if [[ $BENCH_STATUS -ne 0 ]]; then
    echo "Benchmark execution failed with status $BENCH_STATUS"
    exit $BENCH_STATUS
fi

if [[ ! -s "$OUTFILE" ]]; then
    echo "No benchmark output generated. Check your filter: '$FILTER'"
    exit 1
fi

# Parse JSON and display results
python3 - "$OUTFILE" << 'PYTHON'
import json
import sys

try:
    with open(sys.argv[1]) as f:
        data = json.load(f)
except json.JSONDecodeError as e:
    print(f"Error parsing benchmark results: {e}")
    sys.exit(1)

benchmarks = data.get("benchmarks", [])

# Collect mean results
results = []
for b in benchmarks:
    name = b["name"]
    if "_mean" not in name:
        continue
    # Strip _mean suffix for display
    display_name = name.replace("_mean", "")
    results.append({
        "name": display_name,
        "cpu_time": b["cpu_time"],
        "real_time": b["real_time"],
        "time_unit": b.get("time_unit", "ns")
    })

if not results:
    print("No benchmark results found.")
    sys.exit(0)

# Find max name length for formatting
max_name = max(len(r["name"]) for r in results)

# Print header
print(f"{'Benchmark':<{max_name}} {'CPU Time':>14} {'Wall Time':>14}")
print("-" * (max_name + 30))

# Print results
for r in results:
    name = r["name"]
    unit = r["time_unit"]
    print(f"{name:<{max_name}} {r['cpu_time']:>12.1f}{unit} {r['real_time']:>12.1f}{unit}")

print("")
print("-" * (max_name + 30))
print("CPU time  = time spent in calling thread (latency)")
print("Wall time = total elapsed time")
print(f"Results saved to: {sys.argv[1]}")
PYTHON
