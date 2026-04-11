# Third-Party Dependencies

The Poco C++ Libraries include third-party code in two forms:
1. **Bundled dependencies** in this `dependencies/` directory — self-contained libraries that can be updated independently
2. **Embedded code** in the main source tree — algorithms and small components integrated directly into Poco source files

When `POCO_UNBUNDLED` is enabled (CMake) or set in the environment (GNU Make), marked libraries
are replaced with system-installed versions via `find_package()` or `-l` flags.

## Bundled Dependencies

| Library | Version | License | Used by | Unbundleable | Source |
|---------|---------|---------|---------|:------------:|--------|
| [zlib](zlib/) | 1.3.2 | Zlib | Foundation (compression/decompression streams) | Yes | https://github.com/madler/zlib |
| [pcre2](pcre2/) | 10.47 | BSD-3-Clause | Foundation (regular expressions) | Yes | https://github.com/PCRE2Project/pcre2 |
| [utf8proc](utf8proc/) | 2.11.3 | MIT | Foundation (Unicode normalization) | Yes | https://github.com/JuliaStrings/utf8proc |
| [expat](expat/) | 2.7.4 | MIT | XML (SAX/DOM parser) | Yes | https://github.com/libexpat/libexpat |
| [sqlite3](sqlite3/) | 3.53.0 | Public Domain | Data/SQLite (embedded database) | Yes | https://www.sqlite.org |
| [png](png/) | 1.6.56 | libpng License | PDF (PNG image support) | Yes | https://github.com/pnggroup/libpng |
| [v8_double_conversion](v8_double_conversion/) | 3.4.0 | BSD-3-Clause | Foundation (float-to-string conversion) | No | https://github.com/google/double-conversion |
| [pdjson](pdjson/) | — | Public Domain | JSON (streaming parser) | No | https://github.com/skeeto/pdjson |
| [tessil](tessil/) | — | MIT | Foundation (insertion-order-preserving hash containers) | No | https://github.com/Tessil/ordered-map |
| [hpdf](hpdf/) (libharu) | 2.4.6 | Zlib-like | PDF (PDF document generation) | No | https://github.com/libharu/libharu |
| [7zip](7zip/) (LZMA SDK) | 26.00 | Public Domain | SevenZip (7z archive support) | No | https://github.com/ip7z/7zip |
| [cpptrace](cpptrace/) | 1.0.4 | MIT | Foundation (stack trace support, optional) | No | https://github.com/jeremy-rifkin/cpptrace |
| [quill](quill/) | 11.0.2 | MIT | Foundation (high-performance async logging, optional) | No | https://github.com/odygrd/quill |
| [wepoll](wepoll/) | 1.5.8 | BSD-2-Clause | Net (epoll emulation, Windows only) | No | https://github.com/piscisaureus/wepoll |

## Embedded Third-Party Code

The following third-party code is embedded directly in Poco source files. These are algorithm
implementations or minimal compatibility headers that are tightly integrated with Poco classes
and not practical to extract as separate libraries.

| Component | Files | License | Copyright | Notes |
|-----------|-------|---------|-----------|-------|
| Hyrise SQL Parser | `Data/SQLParser/` | MIT | Markus Dreseler, Hasso Plattner Institute | SQL query parsing (diverged fork) |
| CppUnit test framework | `CppUnit/` | CppUnit-old (permissive) | Michael Feathers | Poco's unit test framework |
| BSD random number generator | `Foundation/src/Random.cpp`, `Foundation/include/Poco/Random.h` | BSD-4-Clause | 1983-1993 The Regents of the University of California | FreeBSD libc `random()` algorithm |
| RSA MD4 algorithm | `Foundation/src/MD4Engine.cpp`, `Foundation/include/Poco/MD4Engine.h` | RSA-MD | 1991-1992 RSA Data Security, Inc. | RFC 1320 reference implementation |
| RSA MD5 algorithm | `Foundation/src/MD5Engine.cpp`, `Foundation/include/Poco/MD5Engine.h` | RSA-MD | 1991-1992 RSA Data Security, Inc. | RFC 1321 reference implementation |
| SHA-1 algorithm | `Foundation/src/SHA1Engine.cpp` | Public Domain | Peter C. Gutmann (1992), modified by Carl Ellison | Based on public domain implementation |
| SHA-2 algorithm | `Foundation/src/SHA2Engine.cpp` | Apache-2.0 | mbed TLS project (ARM) | FIPS 180-4 (SHA-224/256/384/512) |
| CMake C++17 detection | `cmake/CXX1x.cmake` | MIT | Nathan Osman (2013) | C++17 compiler feature detection module |
| PostgreSQL large object header | `Data/PostgreSQL/include/libpq/libpq-fs.h` | PostgreSQL License | PostgreSQL Global Development Group | Minimal header for large object definitions |

## Additional Notes
* `tessil/ordered-map` is modified so that `tsl::detail_ordered_hash::numeric_cast()` and `tsl::detail_ordered_hash::deserialize_value()` have external linkage. These functions originally have internal linkage (marked `static`), which prevented the `Poco` module from being built as all exported symbols require external linkage. This should remain until [Tessil/ordered-map#54](https://github.com/Tessil/ordered-map/pull/54) is merged.
* `quill` only supports specific OS and CPU architecture combinations. `ENABLE_FASTLOGGER` is auto-disabled on unsupported platforms via a whitelist in the top-level `CMakeLists.txt` (derived from `ThreadUtilities.h`, `BackendWorkerLock.h`, and `Rdtsc.h`). When updating quill from upstream, re-assess the whitelist against the new version's platform support.
