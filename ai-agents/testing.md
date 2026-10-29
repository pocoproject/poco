# Testing

POCO tests use the CppUnit framework in `CppUnit/`. Each component has a test runner `<Component>-testrunner` built from
`<Component>/testsuite/` when `ENABLE_TESTS=ON` (see [cmake.md](cmake.md#options)).

## Structure

- A test class derives from `CppUnit::TestCase` and has a static `suite()` that registers its test methods with
  `CppUnit_addTest(pSuite, Class, method)`. Slow tests use `CppUnit_addLongTest`; they run only with the runner option
  `-long`. CI runs them in the jobs `linux-gcc-cmake-long-tests`, `macos-clang-cmake-long-tests` and
  `windows-2025-msvc-cmake-long-tests` for the Foundation suites listed in `POCO_LONG_TEST_SUITES` in
  `.github/workflows/ci.yml`. A suite that gets a long test is added to that list; a long test in another component
  also needs its test runner in these jobs.
- The class's suite is added to the component suite in `<Component>/testsuite/src/<Component>TestSuite.cpp`. Foundation
  groups its classes in intermediate suites (`CoreTestSuite`, `ThreadingTestSuite`, ...) that `FoundationTestSuite.cpp`
  collects.
- Adding a test file needs the configure step again (see [cmake.md](cmake.md#source-files)).
- Test data lives in `<Component>/testsuite/data/`. The testsuite `CMakeLists.txt` copies it into the build tree; tests
  that read it from the source tree build the path from the `POCO_BASE` environment variable.

Minimal skeleton (the file banner of [CODING_STYLE.md](../CODING_STYLE.md) goes on top of both files):

```cpp
// WidgetTest.h
#ifndef WidgetTest_INCLUDED
#define WidgetTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"


class WidgetTest: public CppUnit::TestCase
{
public:
	WidgetTest() = delete;
	explicit WidgetTest(const std::string& name);
	~WidgetTest() override;

	void setUp() override;
	void tearDown() override;

	void testResizeKeepsContent();

	static CppUnit::Test* suite();
};


#endif // WidgetTest_INCLUDED
```

```cpp
// WidgetTest.cpp
#include "WidgetTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Widget.h"


using Poco::Widget;


WidgetTest::WidgetTest(const std::string& name): CppUnit::TestCase(name)
{
}


WidgetTest::~WidgetTest() = default;


void WidgetTest::setUp()
{
}


void WidgetTest::tearDown()
{
}


void WidgetTest::testResizeKeepsContent()
{
	Widget widget("abc");
	widget.resize(8);
	assertEqual (8, widget.size());
	assertTrue (widget.content() == "abc");
}


CppUnit::Test* WidgetTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("WidgetTest");

	CppUnit_addTest(pSuite, WidgetTest, testResizeKeepsContent);

	return pSuite;
}
```

## Running

From `<build-dir>/bin`, with `POCO_BASE` pointing at the source tree and the directory on `PATH` (Foundation's process
tests start `TestApp` from there):

```bash
POCO_BASE=<repo-root> PATH="$PATH:$(pwd)" ./<Component>-testrunner -all
```

Run one suite, one test class or one test method by name:

```bash
POCO_BASE=<repo-root> PATH="$PATH:$(pwd)" ./Foundation-testrunner ThreadingTestSuite
```

```bash
POCO_BASE=<repo-root> PATH="$PATH:$(pwd)" ./Foundation-testrunner ClockTest::testClock
```

- `-ignore <repo-root>/cppignore.lnx` (`cppignore.win` on Windows) names tests whose failures are not counted: tests
  that depend on timing or on the environment of the CI machines. The listed tests still run; their failures are
  reported as `FAILURE (ignored)` or `ERROR (ignored)`. A listed test that hangs or crashes still hangs or crashes the
  runner.
- The environment variable `CPPUNIT_IGNORE` takes a comma-separated list of test names with the same effect, in the same
  form (`CppUnit::TestCaller<PathTest>.testFind`; on MSVC `class CppUnit::TestCaller<class PathTest>.testFind`).
- `-print` lists the registered tests.

Through CTest, from `<build-dir>`; the test names are the runner names without `-testrunner` (`Foundation`,
`DataSQLite`, `NetSSL`) and pass the `-ignore` file (all except `SSH`):

```bash
ctest --output-on-failure -R "^Foundation$"
```

Build-tree binaries find the shared libraries through their RPATH. Binaries copied elsewhere need `LD_LIBRARY_PATH`
(Linux), `DYLD_LIBRARY_PATH` (macOS) or `PATH` (Windows) set to the library directory.

## Database and server suites

DataMySQL, DataPostgreSQL, DataODBC, Redis and MongoDB tests need a running server. Run them only against local service
containers, never against shared or production servers. The `data-test-*` jobs in `.github/workflows/ci.yml` show the
images (for example `mysql:8.1.0`, `postgres:16.0`, `mongo:8`), ports and credentials the suites expect.

## Test quality

- A test for a fix fails without the fix. Check this by reverting the fix, or by removing each new check, and running
  the test.
- Test names say what is checked (`testResizeKeepsContent`, not `testResize2`).
- Cover error and timeout paths, not only the success path.
- No sleeps for synchronization. Wait on an event, a condition or a deadline loop; waits and lower bounds on elapsed
  time follow [waits and timeouts](writing-code.md#waits-and-timeouts).
- Before calling a flaky test fixed, run it repeatedly (for example 20 times), also in a ThreadSanitizer build
  ([analysis.md](analysis.md)).
- Tests of deprecated API ([Rule 102 of CODING_STYLE.md](../CODING_STYLE.md#rule-102)) are guarded with
  `#ifdef POCO_TEST_DEPRECATED` and run in a build with `-DENABLE_TEST_DEPRECATED=ON` (CI job
  `linux-gcc-cmake-deprecated-tests`).

## Known pitfalls

**A MongoDB query over a time window misses a document that was written just before the bound.**
Cause: `Poco::Timestamp` is stored in BSON as a UTC datetime with millisecond precision (`BSONWriter::write<Timestamp>`
in `MongoDB/include/Poco/MongoDB/Element.h`); microseconds are truncated, so a document and a bound within the same
millisecond compare equal.
What to do: tests set such timestamps explicitly with a clear margin (for example the bound minus one second) instead of
relying on elapsed time.

## CI

- `.github/workflows/ci.yml` runs CTest through `.github/actions/retry-action` (up to three attempts). A pass on a later
  attempt hides a flaky test: check the attempt count in the job log and report the flaky test.
- `.github/path-filters.yml` decides which narrow jobs run for a pull request; the sanitizer jobs always run.
