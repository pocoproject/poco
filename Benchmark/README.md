# Poco Benchmark

Performance benchmarks for Poco components using [Google Benchmark](https://github.com/google/benchmark).

## Dependencies

- Poco Foundation
- Poco Util
- Google Benchmark library

### Installing Google Benchmark

**macOS (Homebrew):**
```bash
brew install google-benchmark
```

**Debian/Ubuntu:**
```bash
apt install libbenchmark-dev
```

**Fedora/RHEL:**
```bash
dnf install google-benchmark-devel
```

**Windows (vcpkg):**
```bash
vcpkg install benchmark:x64-windows
```

**From source (Linux/macOS):**
```bash
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build --config Release
sudo cmake --install build
```

**From source (Windows):**
```powershell
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:\local -S . -B build
cmake --build build --config Release
cmake --install build --config Release
```

## Building

**Using Make (Linux/macOS):**
```bash
cd poco
make -C Benchmark
```

**Using CMake (Linux/macOS):**
```bash
cmake -B build -DENABLE_BENCHMARK=ON
cmake --build build --target Benchmark
```

**Using CMake (Windows):**
```powershell
cmake -B build -G Ninja -DENABLE_BENCHMARK=ON -DCMAKE_PREFIX_PATH=C:\local
cmake --build build --target Benchmark
```

Note: On Windows, use Ninja generator for reliable target builds. The `CMAKE_PREFIX_PATH` should point to where Google Benchmark was installed.

## Usage

```bash
./Benchmark/bin/$(uname)/$(arch)/benchmark [OPTIONS]
```

### Options

| Option | Description |
|--------|-------------|
| `-h, --help` | Display help information |
| `-l, --list` | List all available benchmarks |
| `-f, --filter=<regex>` | Run only benchmarks matching regex |
| `-t, --min-time=<seconds>` | Minimum time to run each benchmark |
| `-r, --repetitions=<num>` | Number of times to repeat each benchmark |
| `-a, --aggregates-only` | Report only aggregates (mean, median, stddev) |
| `--format=<fmt>` | Output format: console, json, or csv |
| `-o, --output=<file>` | Write results to file |
| `--output-format=<fmt>` | Format for output file |
| `--tabular` | Display counters in tabular format |

### Examples

List all benchmarks:
```bash
./benchmark --list
```

Run specific benchmarks:
```bash
./benchmark --filter="PatternFormatter.*"
```

Run with repetitions and show statistics:
```bash
./benchmark --repetitions=5 --aggregates-only
```

Export results to JSON:
```bash
./benchmark --format=json --output=results.json
```

## Writing Benchmarks

Create a new `.cpp` file in `src/` with your benchmarks:

```cpp
// src/MyComponentBench.cpp

#include <benchmark/benchmark.h>
#include "Poco/MyComponent.h"

using Poco::MyComponent;

static void BM_MyOperation(benchmark::State& state)
{
    MyComponent component;

    for (auto _ : state)
    {
        // Code to benchmark - runs many iterations
        auto result = component.doSomething();
        benchmark::DoNotOptimize(result);
    }

    // Optional: report throughput
    state.SetBytesProcessed(state.iterations() * sizeof(result));
}
BENCHMARK(BM_MyOperation);

// Parameterized benchmark
static void BM_MyOperationSized(benchmark::State& state)
{
    int size = state.range(0);
    std::vector<int> data(size);

    for (auto _ : state)
    {
        // Benchmark with different sizes
        processData(data);
    }
}
BENCHMARK(BM_MyOperationSized)->Range(8, 8<<10);
```

Then add the file to `CMakeLists.txt` and `Makefile`:

**CMakeLists.txt:**
```cmake
set(SRCS
    src/BenchmarkApp.cpp
    src/PatternFormatterBench.cpp
    src/MyComponentBench.cpp  # Add this
)
```

**Makefile:**
```make
objects = BenchmarkApp PatternFormatterBench MyComponentBench
```

## Tips

- Use `benchmark::DoNotOptimize()` to prevent the compiler from optimizing away results
- Use `benchmark::ClobberMemory()` if you need to force memory writes to be visible
- Keep setup code outside the `for (auto _ : state)` loop
- Use `state.PauseTiming()` / `state.ResumeTiming()` for expensive setup within the loop
- Report throughput with `state.SetBytesProcessed()` or `state.SetItemsProcessed()`
