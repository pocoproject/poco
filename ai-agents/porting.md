# Porting POCO to a New Platform

What a port to a new operating system, compiler or C library changes. Most of the code is portable and builds on the
abstraction layer of Foundation; platform code is concentrated in Foundation, Net, Util, the bundled expat of XML, and
the CMake files.

## Requirements

- A C++17 compiler and standard library. CMake uses C++20 when the compiler supports it (see
  [building.md](building.md#language-standard)); C++20 forms in the code are guarded (see
  [CODING_STYLE.md](../CODING_STYLE.md#20-language-standard-and-feature-guards)).
- Threads with a mutex and a condition variable or an equivalent event primitive.
- A file system API.
- A BSD or POSIX socket API for Net and everything above it.
- CMake 3.26 or later with a working toolchain for the platform (see [cmake.md](cmake.md)).

A platform with a POSIX API needs few changes: the `_POSIX` and `_UNIX` implementations usually build after small
adaptations.

## Choosing an implementation

Use the first of these that preserves the documented behavior of the POCO class:

1. Standard C++: a `_STD` implementation file, or portable code without a split.
2. POSIX: the existing `_POSIX` or `_UNIX` file, adapted with `#if` where the platform differs.
3. The platform's own API: a new implementation file.

The standard library does not cover every POCO class. Read the class documentation before replacing an implementation:

| POCO class | Standard facility | Difference to check |
|---|---|---|
| `Mutex`, `FastMutex` | `std::recursive_mutex`, `std::mutex` | `Mutex_STD.cpp` (`POCO_ENABLE_STD_MUTEX`) exists; its `tryLock(milliseconds)` polls, because `std::mutex` has no timed lock |
| `Event` | `std::mutex` with `std::condition_variable` | Auto-reset and manual-reset modes; no `_STD` file exists |
| `RWLock` | `std::shared_mutex` (C++17) | `RWLock::unlock()` releases either mode; a second `lock_shared()` by the same thread is undefined behavior, while POSIX read locks may be held several times |
| `Semaphore` | `std::counting_semaphore` (C++20, guarded) | The maximum is a template argument; `Semaphore(n, max)` takes it at run time and `set()` throws above it, where `release()` above `max()` violates a precondition |
| `Thread` | `std::thread`, `std::jthread` (C++20, guarded by `POCO_HAVE_JTHREAD`) | No name, priority, stack size or affinity; these stay platform code |
| `File`, `Path`, `DirectoryIterator` | `std::filesystem` (C++17) | `File::canRead()` and related calls answer for the effective user (`File_UNIX.cpp` uses `stat()` and `geteuid()`); permission bits alone do not |
| `Timezone` | `std::chrono::current_zone()` (C++20, guarded) | Only where the standard library ships the time zone database |
| `Environment` | `std::getenv` | Reading only; setting needs `setenv` or the platform API |

`Process`, `Pipe`, `SharedLibrary`, `SharedMemory`, `NamedMutex` and `NamedEvent` have no standard library equivalent.

## Foundation

### Platform identification

`Foundation/include/Poco/Platform.h` defines `POCO_OS` from compiler macros (`__linux__`, `__APPLE__`, `_WIN32`, ...) or
from a macro set by the build (`POCO_VXWORKS`), and one of `POCO_OS_FAMILY_UNIX`, `POCO_OS_FAMILY_WINDOWS` or
`POCO_OS_FAMILY_VMS`; BSD systems and macOS also get `POCO_OS_FAMILY_BSD`. A new platform gets a `POCO_OS_<NAME>`
constant with an unused value and a branch in that chain; an unknown platform stops with `#error "Unknown Platform."`.
The same file defines `POCO_ARCH` (`POCO_ARCH_AMD64`, `POCO_ARCH_AARCH64`, `POCO_ARCH_RISCV64`, ...),
`POCO_ARCH_LITTLE_ENDIAN` or `POCO_ARCH_BIG_ENDIAN`, and the `POCO_COMPILER_*` macros.

`Poco/Foundation.h` then includes `Poco/Platform_WIN32.h`, `Poco/Platform_VX.h` or `Poco/Platform_POSIX.h` for settings
of the platform family. Features a platform lacks are switched off with the `POCO_NO_*` macros listed in
`Foundation/include/Poco/Config.h`, for example `POCO_NO_SHAREDMEMORY`, `POCO_NO_FPENVIRONMENT`,
`POCO_NO_SYSLOGCHANNEL`.

### Fixed-size types

`Foundation/include/Poco/Types.h` defines `Poco::Int8` to `Poco::UInt64`, `IntPtr` and `UIntPtr` as aliases of the
`<cstdint>` types. A new compiler needs no change there.

### Implementation files

The platform code of a class is usually in a class `<Class>Impl`, declared in `<Class>_<SUFFIX>.h` and implemented in
`<Class>_<SUFFIX>.cpp`. The portable header and source select the implementation with `#if` and include it (see
[writing-code.md](writing-code.md#platform-specific-code)). The class uses its implementation in one of these ways:

- private inheritance from `<Class>Impl`: `Event`, `File`, `FileStreamRWLock`, `FPEnvironment`, `Mutex`, `NamedMutex`,
  `RWLock`, `Semaphore`, `SharedLibrary`, `Thread`;
- public inheritance from `<Class>Impl`: `NamedEvent`, `Process`;
- a pointer to a `<Class>Impl` object: `DirectoryIterator`, `Pipe` (`PipeImpl_<SUFFIX>.h`), `SharedMemory`;
- calls of static functions of `<Class>Impl`: `Environment`, `Path`;
- no `Impl` class: `FileStream` selects its stream buffer `FileStreamBuf` from `FileStream_<SUFFIX>.h`, and the
  `Timezone_<SUFFIX>.cpp` files implement the member functions of `Timezone` directly.

All implementations of a class have the same member functions, named with an `Impl` suffix; there is no virtual
interface. Read `Thread_POSIX.h` for the full set of `ThreadImpl` functions (`startImpl`, `joinImpl`, `setPriorityImpl`,
`setNameImpl`, `setAffinityImpl`, ...).

| Class | Implementations in `Foundation/src/` |
|---|---|
| `DirectoryIterator`, `Path` | `_UNIX`, `_WIN32U` |
| `Environment`, `File`, `Process` | `_UNIX`, `_VX`, `_WIN32U` |
| `Event`, `Semaphore`, `Thread` | `_POSIX`, `_VX`, `_WIN32` |
| `FileStream`, `FileStreamRWLock` | `_POSIX`, `_WIN32` |
| `FPEnvironment` | `_C99`, `_DEC`, `_QNX`, `_SUN`, `_WIN32`, `_DUMMY` |
| `Mutex` | `_POSIX`, `_STD`, `_VX`, `_WIN32` |
| `NamedEvent`, `NamedMutex` | `_Android`, `_UNIX`, `_WIN32U` |
| `Pipe` (`PipeImpl`), `SharedMemory` | `_POSIX`, `_WIN32`, `_DUMMY` |
| `RWLock` | `_Android`, `_POSIX`, `_VX`, `_WIN32` |
| `SharedLibrary` | `_HPUX`, `_UNIX`, `_VX`, `_WIN32U` |
| `Timezone` | `_UNIX`, `_VX`, `_WIN32` (source files only) |

`_WIN32U` files use the wide-character Windows API through `Poco::UnicodeConverter`; `_DUMMY` files are empty
implementations for platforms without the facility. Classes with short platform code keep it inline under `#if`, for
example `Timestamp`, `Clock`, `LocalDateTime`, `Debugger` and `DirectoryWatcher`.

## Net

- `Net/include/Poco/Net/SocketDefs.h` maps the socket API of the platform family (Windows, VxWorks, Unix) to POCO names
  and includes the system headers; add the platform's headers and differences there.
- `SocketImpl`, `NetworkInterface`, `SocketAddress`, `HostEntry`, `DNS` and `IPAddressImpl` contain platform branches.
  `NetworkInterface` is compiled only where `Net/include/Poco/Net/Net.h` defines `POCO_NET_HAS_INTERFACE`.
- `PollSet` uses epoll (`POCO_HAVE_FD_EPOLL`: Linux, Android, and Windows through the bundled `wepoll`) or `poll()`
  (`POCO_HAVE_FD_POLL`: the BSD family in `Net.h`, other systems in `Foundation/CMakeLists.txt`).

## Util

- `POCO_APP_MAIN` (`Util/include/Poco/Util/Application.h`) and `POCO_SERVER_MAIN` (`ServerApplication.h`) define the
  program entry point; a platform without `main()` gets its own branch, as VxWorks has.
- `ServerApplication` runs as a daemon on Unix and as a service on Windows.
- `SystemConfiguration` omits `system.pid` where the platform has no separate processes (`#if !defined(POCO_VXWORKS)` in
  `Util/src/SystemConfiguration.cpp`).

## XML

The bundled expat (`dependencies/expat/src/xmlparse.c`) seeds its hash salt in `generate_hash_secret_salt()` from a
random source and falls back to `gather_time_entropy()` and the process ID. `dependencies/expat/CMakeLists.txt` selects
the random source per platform (`random_rand_s.c`, `random_arc4random_buf.c`, `random_getrandom.c`,
`random_dev_urandom.c`) and defines the matching `HAVE_*` macro; add the new platform there.

## CMake

- `Foundation/CMakeLists.txt` sets the public compile definitions and link libraries per platform (`CMAKE_SYSTEM_NAME`,
  `APPLE`, `ANDROID`, `EMSCRIPTEN`, `IOS`, ...), for example `POCO_NO_*` switches, `POCO_HAVE_FD_POLL` and system
  libraries. Add a branch for the platform there.
- The root `CMakeLists.txt` switches off components and features that a platform cannot build (Serial on Emscripten,
  FastLogger outside the supported systems and processors).
- Sources that exist for one platform only are listed with `POCO_SOURCES_AUTO_PLAT` (see
  [cmake.md](cmake.md#source-files)); cross builds are described in [building.md](building.md#cross-compiling).

## Testing a port

- Build all components with `-DENABLE_TESTS=ON` and run every test runner ([testing.md](testing.md)). Report the tests
  that fail on the platform in the pull request. `cppignore.lnx` and `cppignore.win` list tests whose failures the test
  runners report as ignored and do not count under CTest on every Unix or every Windows build; the tests still run (see
  [testing.md](testing.md#running)). Ask the maintainers before adding to them.
- Run the sanitizers and the warnings build where the toolchain supports them ([analysis.md](analysis.md)).

NOTE: The core team keeps Linux, macOS and Windows with GCC, Clang and Visual C++ building and passing; other platforms
rely on contributors who "own" them (`CONTRIBUTING.md`, "Contributing Mini-FAQ"). Ask the maintainers before adding a CI
job for a new platform.
