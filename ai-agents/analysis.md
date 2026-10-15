# Analysis

Dynamic and static checks for a change. Run them on the components you changed; report a tool that is missing (with the
installation hint below) instead of skipping it silently. Unless asked otherwise, only findings in changed lines count.

## Sanitizers

Sanitizers are passed with `POCO_SANITIZEFLAGS` (flags separated by `;` or spaces). Use a separate build directory per
sanitizer and add `-DENABLE_TRACE=ON` for stack traces in exceptions.

| Sanitizer | Configure option | Runtime environment |
|---|---|---|
| AddressSanitizer | `-DPOCO_SANITIZEFLAGS="-fsanitize=address"` | |
| ASan, hidden visibility | the above plus `-DCMAKE_CXX_VISIBILITY_PRESET=hidden` | `ASAN_OPTIONS=detect_odr_violation=2` |
| UndefinedBehaviorSanitizer | `-DPOCO_SANITIZEFLAGS="-fsanitize=undefined;-fno-sanitize=vptr"` | |
| ThreadSanitizer | `-DPOCO_SANITIZEFLAGS="-fsanitize=thread"` | `TSAN_OPTIONS="suppressions=<repo-root>/tsan.suppress,second_deadlock_stack=1"` |

```bash
cmake -S <repo-root> -B <build-dir>-tsan -DPOCO_MINIMAL_BUILD=ON -DENABLE_TESTS=ON -DENABLE_NET=ON -DENABLE_TRACE=ON -DPOCO_SANITIZEFLAGS="-fsanitize=thread"
```

```bash
TSAN_OPTIONS="suppressions=<repo-root>/tsan.suppress,second_deadlock_stack=1" POCO_BASE=<repo-root> PATH="$PATH:$(pwd)" ./Net-testrunner -all
```

- The vptr check of UBSan is disabled because the Foundation tests do not link with it.
- On macOS, ASan reports a spurious ODR violation for the test plugin loaded with `dlopen`; the CI job
  `macos-clang-cmake-sanitizers` runs ASan with `ASAN_OPTIONS=detect_odr_violation=0`.
- Fix a race instead of adding a suppression. A new entry in `tsan.suppress` gets a comment that explains why the
  reported race is harmless.
- CI jobs: `linux-gcc-cmake-sanitizers`, `macos-clang-cmake-sanitizers`, `macos-x64-clang-cmake-sanitizers` (matrix
  entries `asan`, `ubsan`, `tsan`, and `hidden-asan` on arm64 and Linux).
- Where sanitizers are not available on Linux, run the affected test class under valgrind memcheck (the
  `linux-sqlparser-upstream-tests` job uses valgrind):

  ```bash
  valgrind --leak-check=full --error-exitcode=1 ./<Component>-testrunner <TestClass>
  ```

## Compiler warnings

Build with `-DENABLE_COMPILER_WARNINGS=ON` (see [cmake.md](cmake.md#options)) and fix every new warning in changed code.
CI jobs: `macos-clang-cmake-warnings`, `windows-msvc-cmake-warnings`.

Functions that always throw and should be `[[noreturn]]` are found with Clang's `-Wmissing-noreturn`; GCC does not
report the same cases:

```bash
cmake -S <repo-root> -B <build-dir>-noreturn -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS=-Wmissing-noreturn -DPOCO_MINIMAL_BUILD=ON -DENABLE_<COMPONENT>=ON
```

```bash
cmake --build <build-dir>-noreturn --parallel 2>&1 | grep -- -Wmissing-noreturn
```

## clang-tidy

The repository has no `.clang-tidy` file; pass the checks explicitly. Configure with
`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` to get `<build-dir>/compile_commands.json`.

```bash
run-clang-tidy -p <build-dir> -checks='-*,bugprone-*,concurrency-*,performance-*,cppcoreguidelines-*,modernize-*,misc-include-cleaner' '<Component>/src/<File>.cpp'
```

```bash
clang-tidy -p <build-dir> -checks='-*,bugprone-*,concurrency-*,performance-*,cppcoreguidelines-*,modernize-*,misc-include-cleaner' <Component>/src/<File>.cpp
```

Many `cppcoreguidelines-*` and `modernize-*` findings describe the existing style of the library. Report those only
where they concern changed code and agree with [CODING_STYLE.md](../CODING_STYLE.md).

## cppcheck

```bash
cppcheck --project=<build-dir>/compile_commands.json --enable=warning,performance,portability --inline-suppr --file-filter='*/<Component>/src/*'
```

## Installing missing tools

| Platform | Command |
|---|---|
| Debian, Ubuntu | `sudo apt install clang-tidy cppcheck valgrind` |
| macOS (brew) | `brew install llvm cppcheck` (clang-tidy and run-clang-tidy are in the llvm `bin` directory) |
| Windows | `winget install LLVM.LLVM` (or the LLVM component of Visual Studio); Cppcheck from its Windows installer |

## Fuzzing

- `.github/workflows/cifuzz.yml` builds and runs the OSS-Fuzz targets of the `poco` project on every pull request.
- Fuzz targets live in `<Component>/fuzzing/` (Foundation, JSON, JWT, Net, XML) and are built with `-DENABLE_FUZZING=ON`
  and Clang.
- Parsers of untrusted input (protocols, file formats) get a fuzz target or at least tests with truncated and malformed
  input (see "Extra quality gates").

## Extra quality gates

Changes to transport, concurrency or storage code (sockets, reactors, TLS, thread pools, queues, notification centers,
Data connectors, file and archive writers) also get these checks.

**ThreadSanitizer.** A TSan run of the touched test suites is the objective check of claims about lock discipline and
memory ordering, which a review can only argue about. Run the suites several times; races depend on timing.

**Coverage**, to find branches that no test reaches, in a separate build directory. With GCC or Clang and gcov:

```bash
cmake -S <repo-root> -B <build-dir>-cov -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=--coverage -DCMAKE_C_FLAGS=--coverage -DCMAKE_EXE_LINKER_FLAGS=--coverage -DCMAKE_SHARED_LINKER_FLAGS=--coverage -DPOCO_MINIMAL_BUILD=ON -DENABLE_TESTS=ON -DENABLE_<COMPONENT>=ON
```

After running the test runner:

```bash
gcovr -r <repo-root> <build-dir>-cov --filter '<repo-root>/<Component>/src/'
```

`lcov --capture --directory <build-dir>-cov` with `genhtml` gives the same as HTML. With Clang, compile and link with
`-fprofile-instr-generate -fcoverage-mapping`, run the tests with `LLVM_PROFILE_FILE=<dir>/%p.profraw`, merge the
profiles with `llvm-profdata merge -sparse` and report with
`llvm-cov report <test-runner> -object <library> -instr-profile=<merged.profdata>`.

**Malformed input.** Receive and parse paths are exercised with truncated and malformed input: through the fuzz targets
(see "Fuzzing") or tests that feed such input. Every case must take the error path (an exception or an error result) and
never crash, hang or read out of bounds; run these tests under ASan and UBSan.

**Performance regression check.** `Benchmark/` is a Google Benchmark application for Foundation and Util code
(PatternFormatter, Logger). It is built with `-DENABLE_BENCHMARK=ON` when the Google Benchmark library is found
(`libbenchmark-dev`); its sources are listed in `Benchmark/CMakeLists.txt`. Compare release builds before and after the
change:

```bash
cmake --build <build-dir> --target Benchmark
```

```bash
<build-dir>/bin/Benchmark --filter='<regex>' --repetitions=5 --aggregates-only --format=json --output=<result>.json
```

For code without a benchmark, add one in `Benchmark/src/` (see `Benchmark/README.md`) or state in the pull request how
performance was checked.

## CodeQL

- `.github/workflows/codeql.yml` analyses `cpp` with the `security-and-quality` queries on pushes to `main`, on pull
  requests and once a day. The build is `.github/workflows/codeql-buildscript.sh` on Ubuntu 22.04 (GCC 11).
- The SARIF filter in the workflow drops results in `dependencies/` and the rules `cpp/path-injection`,
  `cpp/world-writable-file-creation`, `cpp/poorly-documented-function`, `cpp/potentially-dangerous-function`,
  `cpp/use-of-goto`, `cpp/integer-multiplication-cast-to-long`, `cpp/comparison-with-wider-type`, `cpp/leap-year/*`,
  `cpp/ambiguously-signed-bit-field`, `cpp/suspicious-pointer-scaling`, `cpp/suspicious-pointer-scaling-void`,
  `cpp/unsigned-comparison-zero`.
- A class that compiles elsewhere fails in the CodeQL build because GCC 11 rejects a standard attribute next to the
  export macro in a class head; see [Rule 94 of CODING_STYLE.md](../CODING_STYLE.md#rule-94).

List open alerts:

```bash
gh api 'repos/pocoproject/poco/code-scanning/alerts?state=open&per_page=100' --paginate --jq '.[] | "\(.number) \(.rule.id) \(.most_recent_instance.location.path):\(.most_recent_instance.location.start_line)"'
```

Triage: every alert gets one verdict after reading the code.

| Verdict | Action |
|---|---|
| Real defect | Fix it in a pull request; the alert closes when the fix reaches the analysed branch |
| False positive | Dismiss with reason `false positive` and a comment that says why the code is safe |
| Won't fix | Dismiss with reason `won't fix` and the reason |
| Test code only | Dismiss with reason `used in tests` |
| Covered by an open pull request | Leave open; check again after that pull request is merged |

```bash
gh api -X PATCH repos/pocoproject/poco/code-scanning/alerts/<N> -f state=dismissed -f dismissed_reason='false positive' -f dismissed_comment='<specific reason>'
```

- Source comments such as `// CodeQL [rule]` do not suppress alerts in this repository.
- An alert raised because of a class name (for example an XML parser class flagged as an external-entity risk) is
  dismissed only after checking that the behavior is safe by default.
- Dismissals need approval (see [outward writes](git-and-pull-requests.md#outward-writes)).

## CodeRabbit

- Before opening a pull request, review the branch with the CodeRabbit CLI where it is installed
  (`coderabbit review --base main`), or with the agent's CodeRabbit integration.
- CodeRabbit comments on a pull request are triaged like CodeQL alerts: verify each one against the code, then fix it or
  answer with the reason. Never apply a suggested change without reading it.

## Known pitfalls

**valgrind reports one error where the code has several.**
Cause: valgrind merges errors whose top stack frames are the same and reports them once.
What to do: fix the reported error and run valgrind again until it reports none.
