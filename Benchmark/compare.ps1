#
# compare.ps1 - Run benchmarks and display summary
#
# Usage: .\compare.ps1 [filter] [repetitions]
#   filter: benchmark filter regex (default: ".*")
#   repetitions: number of repetitions (default: 5)
#

param(
    [string]$Filter = ".*",
    [int]$Reps = 5
)

$OutFile = "$env:TEMP\benchmark_results.json"

# Find the benchmark binary
if (-not $env:POCO_BASE) {
    # Try to find it relative to script location
    $ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
    $env:POCO_BASE = Split-Path -Parent $ScriptDir
}

# Detect architecture
$Arch = if ([Environment]::Is64BitOperatingSystem) {
    if ($env:PROCESSOR_ARCHITECTURE -eq "ARM64") { "ARM64" } else { "x64" }
} else { "x86" }

# Look for benchmark binary in cmake build directory or traditional location
$BenchmarkPaths = @(
    "$env:POCO_BASE\..\poco-build\bin\Benchmark.exe",
    "$env:POCO_BASE\..\poco-build\bin\Release\Benchmark.exe",
    "$env:POCO_BASE\..\poco-build\bin\Debug\Benchmark.exe",
    "$env:POCO_BASE\Benchmark\bin\Windows\$Arch\benchmark.exe"
)

$Benchmark = $null
foreach ($path in $BenchmarkPaths) {
    if (Test-Path $path) {
        $Benchmark = $path
        break
    }
}

if (-not $Benchmark) {
    Write-Error "Error: benchmark binary not found. Tried:`n$($BenchmarkPaths -join "`n")"
    exit 1
}

Write-Host "Using benchmark: $Benchmark"

# Run benchmark with JSON output to file
# On Windows, Poco uses / prefix for options
$benchArgs = @(
    "/filter=$Filter",
    "/repetitions=$Reps",
    "/aggregates-only",
    "/output=$OutFile",
    "/output-format=json"
)

& $Benchmark @benchArgs
$BenchStatus = $LASTEXITCODE

Write-Host ""
Write-Host "================================================================================"
Write-Host "BENCHMARK SUMMARY"
Write-Host "================================================================================"
Write-Host ""

if ($BenchStatus -ne 0) {
    Write-Host "Benchmark execution failed with status $BenchStatus"
    exit $BenchStatus
}

if (-not (Test-Path $OutFile) -or (Get-Item $OutFile).Length -eq 0) {
    Write-Host "No benchmark output generated. Check your filter: '$Filter'"
    exit 1
}

# Parse JSON and display results using a temp Python script file
$pythonScriptFile = "$env:TEMP\benchmark_parse.py"

@"
import json
import sys

try:
    with open(sys.argv[1]) as f:
        data = json.load(f)
except json.JSONDecodeError as e:
    print("Error parsing benchmark results: " + str(e))
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
fmt = "{:<" + str(max_name) + "} {:>14} {:>14}"
print(fmt.format("Benchmark", "CPU Time", "Wall Time"))
print("-" * (max_name + 30))

# Print results
for r in results:
    name = r["name"]
    unit = r["time_unit"]
    cpu_str = "{:.1f}{}".format(r["cpu_time"], unit)
    wall_str = "{:.1f}{}".format(r["real_time"], unit)
    print(fmt.format(name, cpu_str, wall_str))

print("")
print("-" * (max_name + 30))
print("CPU time  = time spent in calling thread (latency)")
print("Wall time = total elapsed time")
print("Results saved to: " + sys.argv[1])
"@ | Out-File -FilePath $pythonScriptFile -Encoding utf8

# Run Python with the script file
python $pythonScriptFile $OutFile
