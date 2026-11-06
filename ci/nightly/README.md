# Nightly Linux

The nightly builds main at 09:15 UTC on the dedicated `poco-nightly` runner. It has no
pull-request trigger. The runner label is reserved for this workflow.

`poco-nightly.sh` configures a clean Clang 21 Release build with the Linux components,
tools, samples, benchmarks, C++ modules, tests and deprecated-API tests, builds it with
four compiler jobs, starts the test services and runs CTest and the standalone SQLParser
tests. A second, minimal build with `POCO_DATA_ODBC_UNICODE` runs the ODBC suites on
the wide-character API of unixODBC, whose Unicode and encoding tests the first build
skips. Its SQL Server suite runs in the `en_US.ISO-8859-1` locale, because the driver
reads and writes narrow strings in the character set of the client locale. For pre-merge
validation, `POCO_REF` selects a branch or commit; scheduled runs build main. GCC 14 can
crash internally when compiling the Data module.

Every suite runs with `-long` and without ignore files; `CPPUNIT_IGNORE` is unset. Tests
run serially without retries. Failures, crashes, timeouts, skipped tests, ignored failures
and missing suites fail the job; a suite whose driver or server is missing counts as
missing. Db2 and Informix are the only excluded database suites.

MySQL, PostgreSQL, Redis, MongoDB, SQL Server Express and Oracle Free run in containers
bound to loopback, with disposable data, and are removed after the run. SQLite ODBC uses
the distribution driver. A local proxy serves the HTTPS proxy tests. Raw-socket tests get
CAP_NET_RAW; all other tests run as the runner account. Fixed passwords are test values.

NOTE: The host also runs another nightly at 03:00 UTC with a 6-hour limit. The 09:15
start keeps the two apart; there is no lock between them.

## Provisioning

Run `bash ci/nightly/provision.sh` on a dedicated Debian 13 x86_64 runner with
trixie-backports enabled. It installs the build dependencies, the Microsoft and Oracle
ODBC drivers, the database images, the proxy configuration and the Latin-1 locale. The runner account needs
passwordless sudo for Docker, the proxy service and setcap. Provisioning accepts
Microsoft's driver license; the database images carry their own license terms.

Register a repository-scoped runner with only the `poco-nightly` label. Set the repository
secrets `NIGHTLY_INTERNAL_HOSTNAME` and `NIGHTLY_INTERNAL_HOME` to the runner's machine
name and home directory, so that GitHub masks them in the job log. The uploaded logs
(`nightly-linux-logs`, 14 days) have both replaced.
