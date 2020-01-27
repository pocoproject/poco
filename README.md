![alt text][logo]


[![Travis Build Status](https://travis-ci.org/pocoproject/poco.png?branch=poco-1.10.0)](https://travis-ci.org/pocoproject/poco)
[![Build status](https://ci.appveyor.com/api/projects/status/7iyrx3f233s3akae/branch/poco-1.10.0?svg=true)](https://ci.appveyor.com/project/obiltschnig/poco/branch/poco-1.10.0)
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

To start using POCO, see the [Guided Tour](https://pocoproject.org/docs/00100-GuidedTour.html)
and [Getting Started](https://pocoproject.org/docs/00200-GettingStarted.html) documents.


## Quick Start (with CMake)

### Prerequisites

- CMake 3.5 or newer
- A C++14 compiler (Visual C++ 2015, GCC 5.0, Clang 3.4, or newer)
- OpenSSL headers and libraries (optional, but recommended; on macOS, via Homebrew: `brew install openssl`)

### Building

```
$ git clone -b master https://github.com/pocoproject/poco.git
$ cd poco
$ mkdir cmake-build
$ cd cmake-build
$ cmake .. && cmake --build .
```

You can also run:

```
$ sudo cmake --build . --target install
```

in the `cmake-build` directory to install POCO (default `/usr/local` on Linux or macOS).

If you do not want to or cannot use CMake, POCO can also be built with Visual Studio
(project and solution files included) or GNU Make.

Please refer to the [documentation](https://pocoproject.org/docs) for more information.

POCO can also be obtained via different [package managers](https://pocoproject.org/download.html).

## Community and Contributing

POCO has an active user and contributing community, please visit our [website](https://pocoproject.org) and [blog](https://pocoproject.org/blog).
Answers to POCO-related questions can also be found on [Stack Overflow](https://stackoverflow.com/questions/tagged/poco-libraries).

Please see [CONTRIBUTING](CONTRIBUTING.md) for submitting contributions, bugs reports, feature requests or security issues.

### POCO vs. Boost

In regards to Boost, in spite of some functional overlapping,
POCO is best thought of as a Boost complement (rather than replacement).
Side-by-side use of Boost and POCO is a very common occurrence.

[overview]: doc/images/overview.png "Poco Overview"
[logo]: doc/images/logo.png "Poco Logo"
