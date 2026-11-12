# Fuzzing

Fuzz targets are libFuzzer harnesses that run on [OSS-Fuzz](https://google.github.io/oss-fuzz/) as the project `poco`.
Every target follows the libFuzzer and OSS-Fuzz documentation listed under [Sources](#sources); this file states how
POCO applies it. A harness is project code: [CODING_STYLE.md](../CODING_STYLE.md) applies to it as to any other source
file.

## Where things are

| Item | Location |
|---|---|
| Harness, one file per target | `<Component>/fuzzing/<What>Fuzz.cpp` |
| CMake targets | `<Component>/fuzzing/CMakeLists.txt`, added by `<Component>/CMakeLists.txt` under `if(ENABLE_FUZZING)` |
| Seeds | `<Component>/fuzzing/seed/<What>/` |
| Dictionary | `<Component>/fuzzing/<format>.dict` |
| Installation of all targets for OSS-Fuzz | `build/script/oss-fuzz-build.sh` |
| CIFuzz workflow | `.github/workflows/cifuzz.yml` |
| OSS-Fuzz project (`Dockerfile`, `project.yaml` with the sanitizers and fuzzing engines) | [projects/poco](https://github.com/google/oss-fuzz/tree/master/projects/poco) in the OSS-Fuzz repository |

## Structure of a target

- **One target per input format or per way of processing.** Several small targets, not one target with a selector byte
  that chooses between APIs ([libFuzzer][fuzz-target], [split-inputs.md][multiple-options]). Variants that take the same
  format and are cheap run in sequence on the unchanged input, as in `Net/fuzzing/HTTPParse.cpp`; a variant that needs
  the file system gets its own target.
- **The whole input is the plain format** (a JSON document, an HTTP message, a certificate), with no control bytes in
  front. Real files are then seeds, and the corpus of one target is usable by the other targets of that format
  ([split-inputs.md][common-data-format]).
- **A value besides the document** (an option, an index into a fixed table) is taken with `FuzzedDataProvider`, which
  reads integral values from the end of the input, so the document stays at the front
  ([split-inputs.md][fuzzed-data-provider], [FuzzedDataProvider.h][fdp-header]). No length-prefixed fields
  ([split-inputs.md][type-length-value]). A value with a small range is read with `ConsumeIntegralInRange` or
  `PickValueInArray`, so that every input selects one of the values; a plain `ConsumeIntegral` compared with a few
  constants ignores most inputs. `Foundation/fuzzing/MiscFuzzing.cpp` uses `ConsumeIntegralInRange` for the path styles.
- **Deterministic.** The same input gives the same run: no random values, no current time or time zone, no host name,
  DNS, network or environment data. Keys and other fixed data are constants in the source
  ([good-fuzz-target.md][determinism]).
- **No state between runs.** Objects are created for each input. A target does not change global or static state that a
  later run reads ([good-fuzz-target.md][global-state]). One-time initialization is a function-local static object
  ([good-fuzz-target.md][one-time-init]).
- **No file system access and no output.** A target uses in-memory APIs and writes nothing to `stdout` or `stderr`
  ([good-fuzz-target.md][io]). Only where the API under test needs a path, the target uses one private directory below
  `/tmp`, created once per process and emptied after every run; on OSS-Fuzz nothing else is writable
  ([fuzzer environment][file-system]). The input must not be able to name a file that the library opens (include
  directives, external entities).
- **Size check and bounded work.** The first statement of a target returns 0 for an input above a fixed size. The
  target bounds the loops, the recursion and the output that the input controls with limits of the library under test,
  not with a timer or a watchdog thread. The size check is in the harness because the option `-max_len` exists in
  libFuzzer only, not in the other engines of `project.yaml`. OSS-Fuzz reports an input that needs more than about 25
  seconds or 2.5 GB of memory as a defect ([OSS-Fuzz FAQ][limits]); aim for about 1000 runs per second
  ([good-fuzz-target.md][speed]).
- **Return value.** `LLVMFuzzerTestOneInput` returns 0 for every input and never calls `exit()`
  ([libFuzzer][fuzz-target]).
- **Expected exception types only.** A `try` block covers one call, or the calls on one object, and catches only the
  exception types with which that API answers malformed input (for example `Poco::SyntaxException`,
  `Poco::DataFormatException`, `Poco::JSON::JSONException`). Any other exception ends the run and is a finding.
  `catch (const std::exception&)` and `catch (const Poco::Exception&)` also hide `Poco::AssertionViolationException`,
  `Poco::NullPointerException` and `Poco::BugcheckException`, which report a defect in the library. The helper
  `attempt` in `AI/MCP/fuzzing/ClientFuzz.cpp` shows the form of the catch: named types with empty handlers and nothing
  broader. It has one list of types for all its calls; a target whose calls differ in what they throw has one list per
  call. The fuzzer documentation has no rule on exceptions; this one is a decision of the project.
- **A round trip is a check.** A target that writes what it parsed and parses the result again calls `std::abort()` when
  the second parse fails or the two results differ. `assert()` and `poco_assert_dbg()` are compiled out in the build of
  `build/script/oss-fuzz-build.sh` and check nothing there.

NOTE: `Foundation-misc-fuzzer` selects one of four APIs with a value from the input. It is the one exception to the
rule of one target per format; do not copy that structure.

NOTE: Not every target on `main` follows all of these rules; several catch `std::exception` or `Poco::Exception`. Such
a deviation is not a model for another target.

## Files and names

The example is a target of the component Data/SQLite (library `Poco::DataSQLite`) whose input is an SQL script.

| Item | Name | Example |
|---|---|---|
| CMake target | `<Library>-<what>-fuzzer` | `DataSQLite-sql-fuzzer` |
| Binary in `$OUT` | `<library>_<what>_fuzzer` | `datasqlite_sql_fuzzer` |
| Source file | `<Component>/fuzzing/<What>Fuzz.cpp` | `Data/SQLite/fuzzing/SQLFuzz.cpp` |
| Seed directory | `<Component>/fuzzing/seed/<What>/` | `Data/SQLite/fuzzing/seed/SQL/` |
| Dictionary | `<Component>/fuzzing/<format>.dict` | `Data/SQLite/fuzzing/sql.dict` |

- `<Library>` is the name of the CMake library target without `Poco::` (`Net`, `DataSQLite`, `AIMCP`); `<library>` is
  the same name in lower case. `<what>` names the input format or the way of processing.
- A dictionary is named after the format, because several targets can use it (`AI/MCP/fuzzing/mcp.dict`).
- A binary name has lower-case letters, digits and underscores. OSS-Fuzz accepts letters, digits, underscores and
  dashes only ([new project guide][buildsh]).
- Names are final at merge time. OSS-Fuzz keeps the corpus and the reports under the binary name: after a rename it
  closes the open reports of the target and starts again from the seeds ([OSS-Fuzz FAQ][rename]). A change to a target
  also keeps the meaning of its input bytes, so that the stored corpus keeps its value.
- `<Component>/fuzzing/CMakeLists.txt` has three lines per target; see `Net/fuzzing/CMakeLists.txt`.
- The banner of a harness has the form of `AI/MCP/fuzzing/ClientFuzz.cpp`: the file name, what the target feeds to
  which API, the copyright and the `SPDX-License-Identifier` line; no `Library`, `Package` and `Module` lines. Comments
  carry no coverage figures or report dates, and no history of the change
  ([Rule 87 of CODING_STYLE.md](../CODING_STYLE.md#rule-87)).

NOTE: Some targets on `main` do not follow this pattern (`json_parser_fuzzer`, `date_time_fuzzer`, `mcp_client_fuzzer`
and others, with source files such as `Net/fuzzing/HTTPParse.cpp`). Their binaries, CMake targets and source files keep
their names.

## Build script

`build/script/oss-fuzz-build.sh` gets one line per target at the end of each of its three blocks (binaries, seed corpus,
dictionaries; the dictionary line only for a target with a dictionary):

```bash
cp bin/<Library>-<what>-fuzzer $OUT/<library>_<what>_fuzzer
zip -q -r -j $OUT/<library>_<what>_fuzzer_seed_corpus.zip $SRC/poco/<Component>/fuzzing/seed/<What>
cp $SRC/poco/<Component>/fuzzing/<format>.dict $OUT/<library>_<what>_fuzzer.dict
```

- OSS-Fuzz loads the seed corpus archive and the dictionary only under these names: `<binary>_seed_corpus.zip` and
  `<binary>.dict` (new project guide, [seed corpus][seed-corpus] and [dictionaries][oss-fuzz-dict]).
- The seed corpus archive is built without directory names (`zip -j`): a seed directory has no subdirectories.
- A dictionary that several targets use is copied once per target.
- A `-DENABLE_<COMPONENT>=ON` option is added to the `cmake` call only for a component that is off by default; the
  `option(ENABLE_<COMPONENT> ...)` lines of the root `CMakeLists.txt` give the defaults. Where the script switches the
  component off (`-DENABLE_<COMPONENT>=OFF`), remove that option instead of adding `=ON` after it.

## Seeds and dictionaries

- Every target has seeds: small real samples of the format, valid and invalid ones, each the plain input of the target
  ([new project guide][seed-corpus], [good-fuzz-target.md][gft-seed-corpus]). Files written for the test suites can be
  used. A sample that originates outside the project, also one that a test suite already contains, needs a license that
  allows its redistribution; name its origin in the pull request.
- Seeds are kept byte for byte: a `-text` rule in `.gitattributes` excludes seed directories from line-ending
  conversion. `git check-attr text <seed-file>` must print `text: unset` for every seed.
- A format with keywords or magic values gets a dictionary in libFuzzer syntax: one entry in double quotes per line,
  `\xNN` for a byte that is not printable (`"\x0d\x0a"`, not `"\r\n"`), `#` for a comment line
  ([libFuzzer][dictionaries]).
- Every dictionary must parse: libFuzzer exits at start on a line that it cannot parse, and the target then does not
  run at all. Check each target with its dictionary (see [Build and run locally](#build-and-run-locally)):

  ```bash
  <build-dir>-fuzz/bin/<Library>-<what>-fuzzer -dict=<repo-root>/<Component>/fuzzing/<format>.dict -runs=0
  ```

## Build and run locally

`ENABLE_FUZZING` needs LLVM Clang with the libFuzzer runtime; CMake rejects other compilers, Apple Clang included.
The engine is passed in the environment variable `LIB_FUZZING_ENGINE`, which is read when CMake configures the build:

```bash
LIB_FUZZING_ENGINE=-fsanitize=fuzzer cmake -S <repo-root> -B <build-dir>-fuzz -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DBUILD_SHARED_LIBS=OFF -DPOCO_MINIMAL_BUILD=ON -DENABLE_FUZZING=ON -DENABLE_<COMPONENT>=ON -DPOCO_SANITIZEFLAGS="-fsanitize=fuzzer-no-link,address,undefined"
```

```bash
cmake --build <build-dir>-fuzz --target <Library>-<what>-fuzzer --parallel
```

Run the seeds only, then fuzz for a limited time with a scratch corpus directory in front of the seed directory
(libFuzzer writes new inputs to the first directory):

```bash
<build-dir>-fuzz/bin/<Library>-<what>-fuzzer -runs=0 <repo-root>/<Component>/fuzzing/seed/<What>
```

```bash
<build-dir>-fuzz/bin/<Library>-<what>-fuzzer -max_total_time=60 <corpus-dir> <repo-root>/<Component>/fuzzing/seed/<What>
```

## Pull requests that add or change a target

The description of the pull request reports the results of the local OSS-Fuzz test
([new project guide][testing-locally]), run in a checkout of the OSS-Fuzz repository:

| Step | Command | Report |
|---|---|---|
| Build the image | `python3 infra/helper.py build_image poco` | |
| Build, for each sanitizer in `project.yaml` | `python3 infra/helper.py build_fuzzers --sanitizer <sanitizer> poco` | result per sanitizer |
| Check the build, after each build | `python3 infra/helper.py check_build --sanitizer <sanitizer> poco` | result per sanitizer |
| Run each target | `python3 infra/helper.py run_fuzzer --corpus-dir=<corpus-dir> poco <binary>` | number of runs, duration, findings |
| Coverage build | `python3 infra/helper.py build_fuzzers --sanitizer coverage poco` | |
| Coverage of each target | `python3 infra/helper.py coverage poco --fuzz-target=<binary> --corpus-dir=<corpus-dir>` | coverage of the source files under test |

- The image holds the build script of `main`: the `Dockerfile` of the OSS-Fuzz project clones the default branch of POCO
  and copies `build/script/oss-fuzz-build.sh` from that clone when the image is built. For the local test, point the
  `git clone` line of `projects/poco/Dockerfile` in the OSS-Fuzz checkout at the repository and branch of the pull
  request before `build_image`.
- CIFuzz (`.github/workflows/cifuzz.yml`) uses the same image. It builds the sources of the pull request with the build
  script of `main`: a target that the pull request adds is compiled only if that script enables its component, and it
  is neither copied to `$OUT` nor run. A passed CIFuzz job says nothing about such a target.
- A defect that a target finds in the library is fixed in the library, so that the target does not stop on it. When an
  attacker can reach the defect, it is not described in the public pull request; see [security.md](security.md).

## Sources

- libFuzzer: <https://llvm.org/docs/LibFuzzer.html>
- google/fuzzing: [good-fuzz-target.md](https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md),
  [split-inputs.md](https://github.com/google/fuzzing/blob/master/docs/split-inputs.md)
- [FuzzedDataProvider.h][fdp-header]
- OSS-Fuzz: [new project guide](https://google.github.io/oss-fuzz/getting-started/new-project-guide/),
  [ideal integration](https://google.github.io/oss-fuzz/advanced-topics/ideal-integration/),
  [fuzzer environment](https://google.github.io/oss-fuzz/further-reading/fuzzer-environment/),
  [FAQ](https://google.github.io/oss-fuzz/faq/),
  [continuous integration (CIFuzz)](https://google.github.io/oss-fuzz/getting-started/continuous-integration/)

[fuzz-target]: https://llvm.org/docs/LibFuzzer.html#fuzz-target
[dictionaries]: https://llvm.org/docs/LibFuzzer.html#dictionaries
[common-data-format]: https://github.com/google/fuzzing/blob/master/docs/split-inputs.md#common-data-format
[multiple-options]: https://github.com/google/fuzzing/blob/master/docs/split-inputs.md#multiple-options
[fuzzed-data-provider]: https://github.com/google/fuzzing/blob/master/docs/split-inputs.md#fuzzed-data-provider
[type-length-value]: https://github.com/google/fuzzing/blob/master/docs/split-inputs.md#type-length-value
[fdp-header]: https://github.com/llvm/llvm-project/blob/main/compiler-rt/include/fuzzer/FuzzedDataProvider.h
[determinism]: https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md#determinism
[global-state]: https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md#global-state
[one-time-init]: https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md#one-time-initialization
[io]: https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md#io
[speed]: https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md#speed
[gft-seed-corpus]: https://github.com/google/fuzzing/blob/master/docs/good-fuzz-target.md#seed-corpus
[file-system]: https://google.github.io/oss-fuzz/further-reading/fuzzer-environment/#file-system
[limits]: https://google.github.io/oss-fuzz/faq/#how-do-you-handle-timeouts-and-ooms
[rename]: https://google.github.io/oss-fuzz/faq/#what-happens-when-i-rename-a-fuzz-target-
[buildsh]: https://google.github.io/oss-fuzz/getting-started/new-project-guide/#buildsh
[seed-corpus]: https://google.github.io/oss-fuzz/getting-started/new-project-guide/#seed-corpus
[oss-fuzz-dict]: https://google.github.io/oss-fuzz/getting-started/new-project-guide/#dictionaries
[testing-locally]: https://google.github.io/oss-fuzz/getting-started/new-project-guide/#testing-locally
