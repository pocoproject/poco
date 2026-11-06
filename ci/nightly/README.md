# Nightly Linux

The nightly builds main at 06:17 UTC on the dedicated `poco-nightly` runner.
It has no pull-request trigger. The runner label is reserved for this workflow.

The Release build enables Linux components, tools, samples, benchmarks, C++
modules, tests, and deprecated-API tests. Four compiler jobs limit memory use.
Each run starts with a clean build tree.

Generated CTest commands gain `-long` and lose ignore-file arguments.
CPPUNIT_IGNORE is removed from the environment. Tests run serially without
retries. Failures, crashes, timeouts, skipped CTest cases, ignored failures, and
missing required database suites fail the job. Db2 and Informix are the only
excluded database suites. Source assertions are not changed.

MySQL, PostgreSQL, SQLite ODBC, Redis, MongoDB, SQL Server Express, and Oracle
Free provide the database fixtures. Server containers bind only to loopback,
use disposable data, and are removed during cleanup. A local proxy supports
HTTPS proxy tests. Raw-socket tests receive CAP_NET_RAW; tests otherwise run as
the unprivileged runner account. Fixed passwords are test-fixture values.

## Provisioning

Run `bash ci/nightly/provision.sh` on a dedicated Debian 13 runner. It installs
build dependencies, Microsoft and Oracle ODBC drivers, database images, the
local proxy configuration, and the host-lock hook. The runner account requires
passwordless sudo for Docker, the proxy service, and setcap. Provisioning
accepts Microsoft's driver license. Database images include their own license
requirements.

Register a separate repository-scoped runner with only the `poco-nightly`
label. Configure ACTIONS_RUNNER_HOOK_JOB_STARTED in every runner registration
on the machine to point to the installed job-lock.sh, outside all runner
application directories. Restart those runner services after configuration.
Use KillMode=control-group for systemd services so stopping a service also
stops its listener and workers.

Every job-start hook acquires the same host lock before workflow steps run.
The lock remains owned until that job's Runner.Worker exits, including cleanup.
A persistent worker identity prevents a killed lock-holder process from
admitting another job while the original worker is alive. Missing hooks fail
the workflow's verification step. This serializes jobs across repositories;
GitHub workflow concurrency groups alone do not provide that guarantee.

Run `python3 ci/nightly/test_job_lock.py` on Linux to verify normal completion,
cancellation, holder failure, and invalid lock-state behavior. Direct build
invocations outside a runner must also acquire the host lock or run while all
runner services are stopped.

Build and test logs, the source revision, and a JUnit report are uploaded as
`nightly-linux-logs` with 14-day retention. Repair environment problems or fix
actual library/test defects; do not add ignore lists or retry failing tests.
