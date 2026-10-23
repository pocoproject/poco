# Writing Code

Rules for changing library code. The style itself (layout, naming, formatting, comments, error handling) is defined in
[CODING_STYLE.md](../CODING_STYLE.md); this file does not repeat it.

## Before writing

- Read [CODING_STYLE.md](../CODING_STYLE.md) and the code around the place you change. New code looks like the code next
  to it.
- Reuse before writing: the C++ standard library first, then existing Poco classes and the helpers of the component.
  Search the component (`git grep`) before adding a function.
- When the change, or the code it touches, duplicates logic, extract a helper instead of adding a second copy. Make it
  generic (a template or a free function) where the uses allow it, and place it in the component that owns the concept.
- Make the smallest diff that solves the problem. No unrelated refactoring, renaming or reformatting.
- Do not reformat lines whose content does not change, comments included. An unchanged comment keeps its line breaks,
  also when the code around it moves.

## Public API

- Changes to public headers are deliberate, documented in the `///` reference comments and consistent with the existing
  API of the component (names, parameter order, error reporting).
- Visibility or ownership is never widened only to make something compile: do not make a private member public, add a
  `friend`, or turn a reference into an owning pointer to work around a design problem.
- A change that breaks source or binary compatibility is labelled `breaking` and listed under "Breaking Changes" in
  `CHANGELOG`; see also the deprecation rules below.

## New files

| Kind | Location |
|---|---|
| Public header | `<Component>/include/Poco/<Namespace>/<Class>.h`, in a directory that follows the namespace ([Rule 6 of CODING_STYLE.md](../CODING_STYLE.md#rule-6)): `Foundation/include/Poco/<Class>.h`, `Data/SQLite/include/Poco/Data/SQLite/`, and `include/Poco/Net/` in NetSSL_OpenSSL and NetSSL_Win |
| Source | `<Component>/src/<Class>.cpp` |
| Test | `<Component>/testsuite/src/<Class>Test.h`, `.cpp` (see [testing.md](testing.md)) |

- Every file starts with the banner and every header has the include guard defined in the sections "Comments" and
  "Header Files" of [CODING_STYLE.md](../CODING_STYLE.md) (`Library:`, `Package:`, `Module:` lines, copyright,
  `SPDX-License-Identifier: BSL-1.0`; guard `<Component>_<Class>_INCLUDED`).
- Exported classes carry the component's export macro (`Foundation_API`, `Net_API`, `SQLite_API`), defined in the
  component's main header such as `Poco/Net/Net.h`.
- Adding a file needs the configure step again; the naming rule for `_`, platform-only sources and the make build are in
  [cmake.md](cmake.md#source-files).
- The `// Package:` line of the banner names the source group in IDE projects.

## Platform-specific code

Platform implementations live in separate files that the portable source includes, for example
`Foundation/src/Mutex.cpp`:

```cpp
#if defined(POCO_ENABLE_STD_MUTEX)
#include "Mutex_STD.cpp"
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "Mutex_WIN32.cpp"
#elif defined(POCO_VXWORKS)
#include "Mutex_VX.cpp"
#else
#include "Mutex_POSIX.cpp"
#endif
```

The same pattern uses the suffixes `_POSIX`, `_UNIX`, `_WIN32`, `_WIN32U`, `_VX`, `_STD`, `_Android`, `_C99`, `_DEC`,
`_HPUX`, `_QNX`, `_SUN` and `_DUMMY`, with matching headers in `include/` (`Timezone` has no implementation headers).
The classes that use it and the order in which to choose an implementation are in [porting.md](porting.md).

## Language level and feature macros

C++20 forms are guarded as described in the section "Language Standard and Feature Guards" of
[CODING_STYLE.md](../CODING_STYLE.md#20-language-standard-and-feature-guards); `[[nodiscard]]` and `POCO_NODISCARD_CTOR`
follow [Rule 94](../CODING_STYLE.md#rule-94).

## Deprecation

- Mark API for removal as [Rule 102 of CODING_STYLE.md](../CODING_STYLE.md#rule-102) describes; its tests are run as
  described in [testing.md](testing.md#test-quality). Users can define `POCO_SILENCE_DEPRECATED` to hide the warnings.
- Deprecate first, remove in a later major release; list the removal under "Breaking Changes" in `CHANGELOG`.

## Errors

Use Poco exceptions (`Foundation/include/Poco/Exception.h`, `POCO_DECLARE_EXCEPTION`) and the assertion macros of
`Foundation/include/Poco/Bugcheck.h` (`poco_assert`, `poco_assert_dbg`, `poco_check_ptr`, `poco_bugcheck`) as described
in the section "Error Handling" of [CODING_STYLE.md](../CODING_STYLE.md).

## OpenSSL

- Version checks and the minimum version follow [Rule 103 of CODING_STYLE.md](../CODING_STYLE.md#rule-103).
- When a failing call is ignored on purpose, bracket it with `ERR_set_mark()` and `ERR_pop_to_mark()`.
  `ERR_clear_error()` would also drop errors that the caller queued.
- Throwing `Poco::Crypto::OpenSSLException` reads and empties the OpenSSL error queue into the exception message.

## Protocol and parser code

- Data the library sends is validated strictly: sizes, counts, structure.
- Data received from a peer is checked where a missing check would cause a memory or stack safety problem or silently
  wrong data: lengths, offsets, terminators, nesting depth.
- State the policy in a comment where the check is made, so that a reader does not add or remove checks by mistake.

## Waits and timeouts

A wait that can return before its deadline (spurious wake-up, timer resolution on Windows) is written as a loop against
a deadline, not as one timed call. On Windows a single OS wait can end slightly before its timeout as measured by
`Poco::Stopwatch`, so a test that asserts a lower bound on elapsed time also uses a deadline loop or a tolerance. Here
`done()` stands for the awaited condition and `event` is a `Poco::Event`:

```cpp
const auto deadline = std::chrono::steady_clock::now() + timeout;
while (!done())
{
	const auto remaining = std::chrono::ceil<std::chrono::milliseconds>(deadline - std::chrono::steady_clock::now());
	if (remaining.count() <= 0) break;
	event.tryWait(static_cast<long>(remaining.count()));
}
```

## Documentation

- Reference documentation is written as `///` comments after the declaration, as shown in the section "Appendix:
  Documentation" of [CODING_STYLE.md](../CODING_STYLE.md). PocoDoc builds the reference from them.
- User guides are `doc/*.page` and `<Component>/doc/*.page` in PocoDoc markup: first line title, second line category,
  then `!!!` (level 1), `!!` (level 2) and `!` (level 3) headings, `<*italic*>`, `<!bold!>`, `<[code]>`, `[[url text]]`
  links and indented code blocks. `PocoDoc/pages/samplePage.page` shows every element.
- A change in behavior updates the reference comment and, where one exists, the guide.

## Before handing over

- Every behavior change has a test: [testing.md](testing.md).
- Every changed line is ASCII; run the check of [Rule 86 of CODING_STYLE.md](../CODING_STYLE.md#rule-86).
- Build with `ENABLE_COMPILER_WARNINGS=ON` and run the checks of [analysis.md](analysis.md) on the changed files.
- Complete the [definition of done](reviewing.md#definition-of-done).

## Known pitfalls

**A header fails with the static assertion "A min() or max() macro is defined, which breaks this header".**
Cause: `<windows.h>` was included without `NOMINMAX` before a POCO header that uses `std::numeric_limits<>::min()` or
`max()`; the header checks this with `POCO_CHECK_MINMAX_MACROS` (`Poco/Foundation.h`).
What to do: include `<windows.h>` only through `Poco/UnWindows.h`, or include a POCO header first. A new header that
uses `std::numeric_limits<>::min()` or `max()` adds `POCO_CHECK_MINMAX_MACROS` after its includes, as `Poco/Clock.h`
does.
