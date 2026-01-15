![alt text][logo]

[![poco-ci](https://github.com/pocoproject/poco/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/pocoproject/poco/actions/workflows/ci.yml)
[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/370/badge)](https://bestpractices.coreinfrastructure.org/projects/370)


### POCO (Portable Components) C++ Libraries are:

- A collection of C++ class libraries, conceptually similar to the Java Class Library or the .NET Framework.
- Focused on solutions to frequently-encountered practical problems.
- Focused on "internet-age" network-centric applications.
- Written in efficient, modern, 100% ANSI/ISO Standard C++.
- Based on and complementing the C++ Standard Library/STL.
- Highly portable and available on many different platforms, from embedded to server.
- Open Source, licensed under the [Boost Software License](https://spdx.org/licenses/BSL-1.0).

![alt text][overview]

To start using POCO, see the [Guided Tour](https://docs.pocoproject.org/current/00100-GuidedTour.html)
and [Getting Started](https://docs.pocoproject.org/current/00200-GettingStarted.html) documents.


## Quick Start (with CMake)

### Prerequisites

- CMake 3.26 or newer
- A C++17 compiler (Visual C++ 2017, GCC 8.0, Clang 5, or newer)
- C++20 is supported and recommended; some features are only available with C++20
- OpenSSL 1.1.1 or newer headers and libraries (optional, but recommended)
- MySQL, PostgreSQL, ODBC, and Apache/APR client libraries (optional)
- C++20, CMake 3.28 or later, and Ninja (for C++ modules)

Most Unix/Linux systems already have OpenSSL preinstalled. If your system
does not have OpenSSL, please get it from <https://www.openssl.org> or
another source. You do not have to build OpenSSL yourself - a binary
distribution is fine. For example, via Debian APT:

```
$ sudo apt-get install openssl libssl-dev
```

On macOS, the easiest way to install OpenSSL is via [Homebrew](https://brew.sh):

```
$ brew install openssl
```

The easiest way to install OpenSSL on Windows is to use a binary
(prebuilt) release, for example the one from Shining Light
Productions that comes with a
[Windows installer](https://www.slproweb.com/products/Win32OpenSSL.html).
OpenSSL can also be installed via the `vcpkg` package manager. One may
also use the
[WinGet](https://learn.microsoft.com/en-us/windows/package-manager/winget/)
package.

On Windows, POCO can also use the native Windows TLS APIs (SChannel).

### Installing All Dependencies (Linux and macOS)

All dependencies can be installed with the following commands:

#### Debian Linux (including Ubuntu and Raspbian)

```
$ sudo apt-get -y update && sudo apt-get -y install git g++ cmake libssl-dev libmysqlclient-dev libpq-dev
```

#### RedHat Linux

```
$ sudo dnf install -y git gcc-c++ cmake openssl-devel mysql-devel postgresql-devel
```

#### Arch Linux

```
$ sudo pacman -Syu --noconfirm git gcc make cmake openssl mariadb-libs postgresql-libs
```

#### macOS (with Homebrew)

```
$ brew install cmake openssl mysql-client libpq
```

#### Windows (with WinGet)

```
$ winget install --id -e Git.Git LLVM.LLVM Kitware.CMake ShiningLight.OpenSSL.Dev PostgreSQL.PostgreSQL Oracle.MySQL Microsoft.msodbcsql
```

### Building with CMake (Linux, macOS, Windows)

[CMake](https://cmake.org) (version 3.26 or newer) is the official build system for
building the POCO C++ Libraries.

```
$ git clone -b main https://github.com/pocoproject/poco.git
$ cd poco
$ mkdir cmake-build
$ cd cmake-build
$ cmake ..
$ cmake --build . --config Release
```

On macOS, it's necessary to tell CMake where to find the OpenSSL headers
and libraries by setting the `OPENSSL_ROOT_DIR` CMake variable.
For example, if OpenSSL has been installed with Homebrew,
the `cmake` invocation becomes:

```
$ cmake .. -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3
```

On Intel Macs, use `/usr/local/opt/openssl@3` instead.

Similarly, the locations of other external libraries can be specified:

```
$ cmake .. -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3 -DMYSQL_ROOT_DIR=/opt/homebrew/opt/mysql-client -DPostgreSQL_ROOT_DIR=/opt/homebrew/opt/libpq
```

Other common ways of building with CMake (e.g., `cmake-gui`) will also work.

There are also a number of project-specific CMake variables that can be changed.


#### Cross-Compiling

With a proper CMake toolchain file (specified via the `CMAKE_TOOLCHAIN_FILE` CMake variable),
the POCO C++ Libraries can be cross-compiled for embedded Linux systems:

```
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/mytoolchain.cmake -DCMAKE_INSTALL_PREFIX=/path/to/target
```


#### Installing

The POCO C++ Libraries headers and libraries can optionally be installed by building the `install` target.

```
$ sudo cmake --build . --target install
```

The default install location is `/usr/local/` on Linux and macOS and
`C:\Program Files\` on Windows and can be overridden by setting
the `CMAKE_INSTALL_PREFIX` CMake variable.


#### Building and Installing - Using vcpkg

You can download and install poco using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:

```
$ git clone https://github.com/Microsoft/vcpkg.git
$ cd vcpkg
$ ./bootstrap-vcpkg.sh
$ ./vcpkg integrate install
$ ./vcpkg install poco
```
The poco port in vcpkg is kept up to date by Microsoft team members and community contributors.
If the version is out of date, please [create an issue or pull request](https://github.com/Microsoft/vcpkg)
on the vcpkg repository.


#### Building and Installing - Using Conan

You can download and install poco using the [Conan](https://github.com/conan-io/conan)
package manager. It needs to be [installed](https://conan.io/downloads.html) first.

You can install Poco libraries from [Conan Center](https://conan.io/center.html):

```
$ conan install --requires=poco/1.15.0
```

Or, you can download Poco recipe and build locally:

```
$ conan install --requires=poco/1.15.0 --build=poco
```

The Poco recipe and packages in Conan Center are kept up to date by Conan team members and community contributors.
If the version is out of date, or you detect any wrong behavior, please [create an issue or pull request](https://github.com/conan-io/conan-center-index)
on the Conan Center Index repository.


### Building Without CMake

If you do not want to or cannot use CMake, POCO can also be built with GNU Make 
(Linux, macOS and other supported Unix platforms).

Please refer to the [documentation](https://pocoproject.org/docs) for more information.


### Getting POCO via a Package Manager

POCO can also be obtained via different [package managers](https://pocoproject.org/download.html).


## Community and Contributing

POCO has an active user and contributing community, please visit our [website](https://pocoproject.org) and [blog](https://pocoproject.org/blog).
Answers to POCO-related questions can also be found on [Stack Overflow](https://stackoverflow.com/questions/tagged/poco-libraries).

Please see [CONTRIBUTING](CONTRIBUTING.md) for submitting contributions, bug reports, feature requests or security issues.

### POCO vs. Boost

In regards to Boost, in spite of some functional overlapping,
POCO is best thought of as a Boost complement (rather than replacement).
Side-by-side use of Boost and POCO is a very common occurrence.

[overview]: doc/images/overview.png "Poco Overview"
[logo]: doc/images/logo.png "Poco Logo"
