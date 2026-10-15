# Building POCO

Prerequisites and platform notes for building POCO. The configure and build commands, the CMake options and the rules
for CMake files are in [cmake.md](cmake.md). Placeholders: `<repo-root>` is the checkout, `<build-dir>` a build
directory.

## Language standard

- CMake checks for C++20 support (`cmake/CXX2x.cmake`: GCC 10+, Clang 10+, AppleClang 13+, MSVC) and uses C++20 when it
  is available; otherwise it falls back to C++17 (`cmake/CXX1x.cmake`). Emscripten builds always use C++17. The
  configure output states which standard is used.
- `main` (2.0) targets C++20 and must also build as C++17, so C++20 forms in the code are always guarded (see the
  section "Language Standard and Feature Guards" of
  [CODING_STYLE.md](../CODING_STYLE.md#20-language-standard-and-feature-guards)). The 1.x release branches require C++17
  and use C++20 when it is available.

## Optional components and their packages

In a default build (`POCO_MINIMAL_BUILD=OFF`) Crypto, NetSSL, JWT, the Data connectors and ApacheConnector are disabled
without an error when their development package is not found. SSH, DNSSD and the XSD validator are off by default. An
explicit `-DENABLE_<COMPONENT>=ON` makes the package required, except for SSH and the XSD validator, which are switched
off with a status message. Check the `Building: <component>` lines of the configure output.

| Component | Option | Package (Debian/Ubuntu) |
|---|---|---|
| Crypto, NetSSL, JWT | `ENABLE_CRYPTO`, `ENABLE_NETSSL`, `ENABLE_JWT` | `libssl-dev` |
| Data/MySQL | `ENABLE_DATA_MYSQL` | `libmysqlclient-dev` |
| Data/PostgreSQL | `ENABLE_DATA_POSTGRESQL` | `libpq-dev` |
| Data/ODBC | `ENABLE_DATA_ODBC` | `unixodbc-dev` |
| ApacheConnector | `ENABLE_APACHECONNECTOR` | `apache2-dev`, `libapr1-dev`, `libaprutil1-dev` |
| SSH | `ENABLE_SSH` | `libssh-dev` |
| DNSSD | `ENABLE_DNSSD` with `ENABLE_DNSSD_AVAHI`, `ENABLE_DNSSD_BONJOUR` or `ENABLE_DNSSD_DEFAULT` (Bonjour on Apple platforms, Avahi on all others) | `libavahi-client-dev` (Avahi) |
| XSD validator | `ENABLE_XSD_VALIDATOR` | `libxml2-dev` |

NOTE: An option that CMake switched off is stored as OFF in the cache. After installing the package, pass
`-DENABLE_<COMPONENT>=ON` again.

The union of the packages that the Linux CI jobs in `.github/workflows/ci.yml` install:

```bash
sudo apt install cmake ninja-build libssl-dev libssh-dev libltdl-dev apache2-dev libapr1-dev libaprutil1-dev libavahi-client-dev libsqlite3-dev libxml2-dev unixodbc-dev libmysqlclient-dev libpq-dev redis-server
```

On macOS, point CMake at a non-system OpenSSL with `-DOPENSSL_ROOT_DIR=<openssl-prefix>`.

## Building for Linux from another operating system

Linux-only code (epoll, POSIX process and signal handling, Avahi) is built and tested in a Linux virtual machine or
container with the packages above. Use a separate build directory for it; do not share a build directory between
operating systems.

## Windows

- Compiler: MSVC with the x64 developer environment; CI uses the GitHub `windows-2025` runner with Ninja
  (`-G Ninja -DCMAKE_BUILD_TYPE=Release`). A clang-cl job builds a reduced set of components.
- Dependencies (OpenSSL, libssh, libxml2) come from vcpkg (`x64-windows` triplet); pass
  `-DOPENSSL_ROOT_DIR=<openssl-prefix>` and `-DCMAKE_PREFIX_PATH=<libssh-prefix>`.
- When the source tree is copied to a separate build machine, copy an export of the commit rather than a working tree
  with converted line endings:

  ```bash
  git archive --format=zip -o poco-src.zip HEAD
  ```

- The Windows CI test step excludes the suites that need database or server backends:

  ```bash
  ctest --output-on-failure -E "(DataMySQL)|(DataODBC)|(Redis)|(MongoDB)"
  ```

## Cross-compiling

`-DPOCO_CROSS_COMPILE=arm-linux`, `android` or `emscripten` selects a cross toolchain (see
[cmake.md](cmake.md#options)). `arm-linux` sets only the compilers (prefix `POCO_CROSS_COMPILE_TOOLCHAIN_PREFIX`,
default `aarch64-linux-gnu`), not a sysroot, so `find_package` can pick up host libraries such as OpenSSL. For
components that need target libraries, pass a toolchain file with `-DCMAKE_TOOLCHAIN_FILE=<file>` that sets
`CMAKE_SYSTEM_NAME`, the compilers, `CMAKE_SYSROOT` and `CMAKE_FIND_ROOT_PATH_MODE_LIBRARY`, `_INCLUDE` and `_PACKAGE`
to `ONLY` (cmake-toolchains(7)). Porting to a new platform is described in [porting.md](porting.md).

## Next steps

- Configure, build and CMake options: [cmake.md](cmake.md)
- Running tests: [testing.md](testing.md)
- Sanitizers, warnings and static analysis: [analysis.md](analysis.md)
