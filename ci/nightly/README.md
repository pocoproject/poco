# Nightly Linux

The nightly builds main at 09:15 UTC on the dedicated `poco-nightly` runner. It has no
pull-request trigger. Only this workflow on main can use the runner (see Provisioning).

`poco-nightly.sh` configures a clean Clang 21 Release build with the Linux components,
tools, samples, benchmarks, C++ modules, tests and deprecated-API tests, builds it with
four compiler jobs, starts the test services and runs CTest and the standalone SQLParser
tests. A second, minimal build with `POCO_DATA_ODBC_UNICODE` runs the ODBC suites on
the wide-character API of unixODBC, whose Unicode and encoding tests the first build
skips. Its SQL Server suite runs in the `en_US.ISO-8859-1` locale, because the driver
reads and writes narrow strings in the character set of the client locale. The workflow
builds the commit it runs for: main for the scheduled run, the chosen branch for a run
started by hand. When the script is run by hand on the runner host, `POCO_REF` selects the
branch or commit; it defaults to main. GCC 14 can crash internally when compiling the Data module.

Every suite runs with `-long` and without ignore files; `CPPUNIT_IGNORE` is unset. Tests
run serially without retries. Failures, crashes, timeouts, skipped tests, ignored failures
and missing suites fail the job; a suite whose driver or server is missing counts as
missing. Db2 and Informix are the only excluded database suites.

MySQL, PostgreSQL, Redis, MongoDB, SQL Server Express and Oracle Free run in containers
bound to loopback, with disposable data, and are removed after the run. SQLite ODBC uses
the distribution driver. A local proxy serves the HTTPS proxy tests. Raw-socket tests get
CAP_NET_RAW; all other tests run as the runner account. Fixed passwords are test values.
The runner account runs nothing else as root: starting and stopping the proxy and the
containers and setting the capability go through `poco-nightly-root.sh`.

NOTE: The host also runs another nightly at 03:00 UTC with a 6-hour limit. The 09:15
start keeps the two apart; there is no lock between them.

## Provisioning

The runner runs as an account of its own, without other sudo rights and outside the
`docker` group. An administrator runs `bash ci/nightly/provision.sh RUNNER_USER` on a
dedicated Debian 13 x86_64 host with trixie-backports enabled. It installs the build
dependencies, the Microsoft and Oracle ODBC drivers, the database images, the proxy
configuration and the Latin-1 locale, and it installs `poco-nightly-root.sh` as
`/usr/local/sbin/poco-nightly-root` and `poco-nightly.sudoers` as
`/etc/sudoers.d/poco-nightly`: the runner account may run that helper with one of its
three fixed actions and nothing else as root. Changes to either file take effect when
the host is provisioned again. Provisioning accepts Microsoft's driver license; the
database images carry their own license terms.

Register the runner with only the `poco-nightly` label in an organization runner group
that is limited to this repository and to `.github/workflows/nightly-linux.yml` on
`refs/heads/main`, so that no other workflow, and no pull request, can use it. Workflow
runs from forks need approval for all outside contributors. Set the repository secrets
`NIGHTLY_INTERNAL_HOSTNAME` and `NIGHTLY_INTERNAL_HOME` to the runner's machine name and
home directory, so that GitHub masks them in the job log. The uploaded logs
(`nightly-linux-logs`, 14 days) have both replaced.
