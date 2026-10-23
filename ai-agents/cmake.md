# CMake

How to configure POCO and how to write its CMake files. The minimum is CMake 3.26
(`cmake_minimum_required(VERSION 3.26.0)` in `CMakeLists.txt`); the build uses `$<BUILD_LOCAL_INTERFACE:...>`, which
needs 3.26. Do not use a CMake feature that needs a later version unless it is guarded, as `ENABLE_MODULES` is with a
check for 3.28. Placeholders: `<repo-root>` is the checkout, `<build-dir>` a build directory, `<Component>` a CMake
target name such as `Foundation`, `Net` or `DataSQLite`. Compilers, packages and platform notes are in
[building.md](building.md).

## Configure and build

```bash
cmake -S <repo-root> -B <build-dir> -DENABLE_TESTS=ON
```

```bash
cmake --build <build-dir> --parallel
```

Build only the components you work on, and one test runner with the libraries it needs:

```bash
cmake -S <repo-root> -B <build-dir> -DPOCO_MINIMAL_BUILD=ON -DENABLE_TESTS=ON -DENABLE_NET=ON -DENABLE_UTIL=ON
```

```bash
cmake --build <build-dir> --parallel --target <Component>-testrunner
```

- Use one build directory per branch and per configuration (sanitizer, warnings, release), outside tracked directories.
- Components that others depend on are switched on automatically (for example `ENABLE_NETSSL` turns on `ENABLE_CRYPTO`,
  `ENABLE_NET` and `ENABLE_UTIL`).

## Options

| Option | Default | Purpose |
|---|---|---|
| `ENABLE_TESTS` | OFF | Build the CppUnit test runners and register them with CTest |
| `POCO_MINIMAL_BUILD` | OFF | Build Foundation plus the components enabled with `ENABLE_<COMPONENT>` |
| `ENABLE_<COMPONENT>` | per component | Enable one component, for example `ENABLE_NET`, `ENABLE_DATA_SQLITE` |
| `ENABLE_TEST_DEPRECATED` | OFF | Also build tests of deprecated API (defines `POCO_TEST_DEPRECATED`) |
| `ENABLE_COMPILER_WARNINGS` | OFF | Maintainer warnings for POCO code only: `-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wzero-as-null-pointer-constant` (GCC, Clang), `/W4` (MSVC) |
| `ENABLE_TRACE` | OFF | Stack traces in exceptions (defines `POCO_ENABLE_TRACE`) |
| `BUILD_SHARED_LIBS` | ON | Shared or static libraries |
| `POCO_MT` | OFF | MSVC only: static runtime (`/MT`); needs `BUILD_SHARED_LIBS=OFF` |
| `POCO_SOO` | ON | Small object optimization in Foundation |
| `POCO_ENABLE_STD_MUTEX` | OFF | `Poco::Mutex` and `Poco::FastMutex` on `std::recursive_mutex` and `std::mutex` |
| `POCO_NO_FORK_EXEC` | OFF | `Poco::Process` without `fork()` and `exec*()` (Apple mobile platforms) |
| `ENABLE_FASTLOGGER` | ON | Quill-based FastLogger; switched off on unsupported platforms |
| `POCO_UNBUNDLED` | OFF | Use system libraries instead of the bundled ones (see [dependencies.md](dependencies.md)) |
| `POCO_SQLITE_UNBUNDLED` | OFF | Use the system SQLite only |
| `POCO_SANITIZEFLAGS` | empty | Sanitizer flags for compile and link (see [analysis.md](analysis.md)) |
| `POCO_COMPILER` | empty | Compiler by name or path (`gcc-12`, `clang-18`, `clang-cl`) |
| `POCO_CROSS_COMPILE` | empty | Cross build for `arm-linux`, `android` or `emscripten`; not together with `CMAKE_TOOLCHAIN_FILE` |
| `ENABLE_MODULES` | OFF | Build the `Poco.*` C++ modules; needs C++20, CMake 3.28+ and a generator that scans modules: Ninja or Ninja Multi-Config with ninja 1.11+, or Visual Studio 17 2022 (cmake-cxxmodules(7)) |
| `ENABLE_NETSSL_WIN` | OFF | Windows only: NetSSL on SChannel |
| `ENABLE_FUZZING` | OFF | OSS-Fuzz targets; needs Clang |
| `CMAKE_EXPORT_COMPILE_COMMANDS` | OFF | Write `compile_commands.json` for clang-tidy and cppcheck (Makefile and Ninja generators only) |

NOTE: `ENABLE_COMPILER_WARNINGS` does not add `-Werror`. Read the build log for new warnings in the files you changed.

## Source files

- `<Component>/CMakeLists.txt` collects `src/*.cpp` and `include/*.h` with `file(GLOB)` and passes them to
  `POCO_SOURCES_AUTO` and `POCO_HEADERS_AUTO` (`cmake/PocoMacros.cmake`), which put each file into the IDE source group
  named by its `// Package:` banner line.
- A file whose name contains `_` and that has a `// Package:` line is a platform implementation file
  (`Mutex_POSIX.cpp`): it is listed but marked `HEADER_FILE_ONLY`, so it is compiled only through the `#include` in its
  portable source. Name normal source files without `_`.
- Sources built on one platform only are listed with `POCO_SOURCES_AUTO_PLAT(SRCS <platform> <files>)`, where
  `<platform>` is a CMake condition such as `UNIX` or `WIN32` (see `Foundation/CMakeLists.txt`).
- The CMake documentation advises against `file(GLOB)` for sources because the build does not notice an added or removed
  file. POCO uses it on purpose and does not use `CONFIGURE_DEPENDS`: run the configure step again after adding,
  removing or renaming a file. Test sources follow the same rule.

NOTE: The GNU make build (see [Adding a component](#adding-a-component)) compiles only the files named in the `objects`
list of `<Component>/Makefile` and `<Component>/testsuite/Makefile`. Add a new source or test file there too, or the CI
job `linux-gcc-make-build` does not compile it.

## Rules for CMake files

1. **Targets, not directories.** Use `target_link_libraries`, `target_include_directories`,
   `target_compile_definitions`, `target_compile_options` and `target_link_options`. Never use the directory-scope
   commands `link_libraries`, `link_directories`, `include_directories`, `add_definitions`, `add_compile_definitions`,
   `add_compile_options` or `add_link_options` in a component: they apply to every target of the directory and its
   subdirectories. The `target_*` commands are their target-scope equivalents; the documentation of `add_definitions`
   points to `add_compile_definitions`, which is itself a directory command. Existing deviations, not examples to
   follow: `add_compile_definitions(POCO_HAVE_SENDFILE)` in `Net/CMakeLists.txt`, and `include_directories`,
   `add_definitions` and `add_compile_definitions` in `DNSSD/`.
2. **Scope every dependency.** Each `target_link_libraries` call names `PUBLIC`, `PRIVATE` or `INTERFACE`: `PRIVATE` for
   a dependency of the implementation only, `PUBLIC` when public headers need it, `INTERFACE` when only consumers need
   it (cmake-buildsystem(7), "Target Usage Requirements"). The same applies to include directories, definitions and
   options.
3. **Namespaced targets.** Every library has an alias `add_library(Poco::<Component> ALIAS <Component>)` directly after
   `add_library`, and links other POCO libraries as `Poco::<Component>` and external ones through imported targets
   (`OpenSSL::SSL`, `ZLIB::ZLIB`). An alias can be read and linked but not modified, installed or exported; set
   properties on the real target. `DNSSD/` deviates from rules 2 and 3 (alias `PocoDNSSD`, links without a scope
   keyword); do not copy it.
4. **No library type.** Call `add_library(<Component> ${SRCS})` without `STATIC` or `SHARED`; `BUILD_SHARED_LIBS` (an
   option in the root `CMakeLists.txt`, default ON) decides. The Apache module `mod_poco` is `SHARED` because Apache can
   only load a shared object.
5. **Project-wide options in two places only.** Directory-scope compile and link options that apply to every target
   (sanitizer flags, release stripping, `POCO_DLL`/`POCO_STATIC`) are set in `cmake/DefinePlatformSpecific.cmake`, which
   the root `CMakeLists.txt` includes before `add_subdirectory(dependencies)`, so they apply to bundled code too. The
   `ENABLE_COMPILER_WARNINGS` flags are added by `poco_enable_detailed_compiler_warnings()`, called after
   `add_subdirectory(dependencies)` so that bundled code is not affected. Feature definitions that every target needs
   (`POCO_ENABLE_TRACE`, `POCO_NO_FORK_EXEC`) are set in the root `CMakeLists.txt`. Component files change no global
   state.
6. **`CMAKE_<LANG>_FLAGS` belongs to the user** (command line, toolchain file, `CXXFLAGS`). The root file appends
   `/Zc:__cplusplus` for MSVC and `cmake/CXX2x.cmake`/`CXX1x.cmake` append `-stdlib=libc++` when Clang needs it; do not
   add more.
7. **Compiler conditions.** Wrap compiler-specific options in a condition on `CMAKE_CXX_COMPILER_ID`
   (`if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")`, `MATCHES "Clang"`) or a generator expression
   (`$<$<CXX_COMPILER_ID:MSVC>:/W4>`). `CMAKE_COMPILER_IS_GNUCXX` is deprecated (CMake 3.24).
8. **Language standard.** The root `CMakeLists.txt` sets `CMAKE_CXX_STANDARD` to 20 or 17 (see
   [building.md](building.md#language-standard)) and `Foundation` declares
   `target_compile_features(Foundation PUBLIC cxx_std_17)` so that consumers of the installed targets compile with at
   least C++17. Components set no standard of their own and never add `-std=` flags.
9. **Paths.** Use `CMAKE_CURRENT_SOURCE_DIR`, `CMAKE_CURRENT_BINARY_DIR` and `PROJECT_SOURCE_DIR`; POCO can be built as
   a subdirectory of another project, where `CMAKE_SOURCE_DIR` is not the POCO checkout. Public include directories are
   given as `$<BUILD_INTERFACE:...>` and `$<INSTALL_INTERFACE:...>`; build-only dependencies that must not be exported
   as `$<BUILD_LOCAL_INTERFACE:...>`.
10. **Build order comes from linking.** `add_dependencies` is needed only for a target that is used without being
    linked, such as a helper program or plugin a test starts or loads (`Foundation/testsuite/CMakeLists.txt`).
11. **Variables.** A component option is `ENABLE_<COMPONENT>`; any other cache variable starts with `POCO_`. A temporary
    variable starts with `_` and is unset after use (`_poco_san_flags` in `cmake/DefinePlatformSpecific.cmake`). Prefer
    target properties to variables.
12. **Platform checks.** Do not assume a platform or compiler from another one; test `WIN32`, `APPLE`, `ANDROID`,
    `EMSCRIPTEN` or `CMAKE_SYSTEM_NAME`, and test for headers and functions with `check_include_files` and
    `check_symbol_exists`, as `Net/CMakeLists.txt` does for `sendfile`.

## Adding a component

What `Serial` (issue #5449) added for the CMake build and the release archives, in order:

1. `<Component>/include/Poco/<Component>/`, `src/`, `testsuite/src/` and `<Component>/doc/` with an overview page.
2. `<Component>/CMakeLists.txt`, copied from `Serial/CMakeLists.txt`: sources and headers, `DLLVersion.rc` for MSVC
   shared builds, `add_library` with the `Poco::` alias, `set_target_properties` with
   `VERSION`/`SOVERSION ${SHARED_LIBRARY_VERSION}`, `OUTPUT_NAME Poco<Component>` and
   `DEFINE_SYMBOL <Component>_EXPORTS`, scoped links and include directories, `POCO_INSTALL(<Component>)`,
   `POCO_GENERATE_PACKAGE(<Component>)` and `add_subdirectory(testsuite)` under `ENABLE_TESTS`.
3. `<Component>/cmake/Poco<Component>Config.cmake`: one `find_dependency(Poco<Dependency>)` per POCO dependency, then
   `include("${CMAKE_CURRENT_LIST_DIR}/Poco<Component>Targets.cmake")`. `POCO_GENERATE_PACKAGE` installs it with the
   exported targets.
4. `<Component>/testsuite/CMakeLists.txt`, copied from `Serial/testsuite/CMakeLists.txt`:
   `poco_add_executable(<Component>-testrunner ...)`, `add_test` with the `cppignore` file and `POCO_BASE` in the test
   environment.
5. Root `CMakeLists.txt`: `option(ENABLE_<COMPONENT> "..." ${_enable_default})`, the switches that turn on the
   components it needs or turn it off on unsupported platforms, and `add_subdirectory(<Component>)` with
   `list(APPEND Poco_COMPONENTS "<Component>")`.
6. `release/spec/all.release` (release archives).
7. `-DENABLE_<COMPONENT>=ON` in the full-build option lists of `.github/workflows/ci.yml`.
8. `CHANGELOG` and `doc/99100-ReleaseNotes.page` (see [releasing.md](releasing.md)).

NOTE: The GNU make build files still exist on `main`, and the CI job `linux-gcc-make-build` builds with them, so a
component also needs them until GNU make is removed: `<Component>/Makefile` and `<Component>/testsuite/Makefile` with
their `objects` lists, `<Component>/dependencies`, and its entries in the root `Makefile`, in `components` and in
`configure`. `Serial` has each of them.

## Known pitfalls

**The checkout is full of generated files (`CMakeCache.txt`, `CMakeFiles/`, `bin/`, `lib/`) and tracked `Makefile`s show
as modified.**
Cause: CMake was configured with the checkout as the build directory (`cmake .`); with the default Makefile generator it
also overwrites the tracked `Makefile`s.
What to do: always configure with `-S <repo-root> -B <build-dir>`. To recover, list the changes with
`git status --short --ignored`, delete the CMake-generated files and restore the changed tracked files with
`git checkout -- <path>`.

NOTE: Do not use `git clean -x` or `git clean -X` for this: they also delete ignored local files such as
`CLAUDE.local.md` and `ai-agents/local/`.

**A new source file is not compiled, or a deleted one is still expected.**
Cause: sources are collected with `file(GLOB)` when CMake configures, not when it builds.
What to do: run the configure step again (`cmake -S <repo-root> -B <build-dir>`).

**A new source file is in `src/` but is not compiled; the link fails with undefined symbols.**
Cause: the file name contains `_` and the file has a `// Package:` line, so `POCO_SOURCES_AUTO` treats it as a platform
implementation file.
What to do: rename it without `_`, or include it from its portable source.
