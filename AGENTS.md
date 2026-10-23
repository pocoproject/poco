# Instructions for AI Coding Agents

This file is for AI coding agents and the contributors who work with them on the POCO C++ Libraries. It states the rules
that always apply and points to the detailed instructions in [ai-agents/](ai-agents/README.md). The C++ coding style is
defined in [CODING_STYLE.md](CODING_STYLE.md); contribution terms are in [CONTRIBUTING.md](CONTRIBUTING.md).

Personal or machine-specific settings (tool paths, local build hosts, commit trailers) do not belong here; put them in
an untracked local file such as `CLAUDE.local.md`, or in the directory `ai-agents/local/`, which git ignores (see
[local notes](ai-agents/README.md#local-notes)). MCP server registrations that carry credentials (`.mcp.json`) are never
committed.

## Project

POCO (Portable Components) is a set of cross-platform C++ class libraries for network-centric applications, licensed
under the Boost Software License (`BSL-1.0`). `main` is the development branch of POCO 2.0 (C++20); `poco-X.Y.Z`
branches are the maintained release lines.

## Repository layout

| Path | Content |
|---|---|
| `<Component>/` | One directory per library: `Foundation`, `XML`, `JSON`, `Net`, `NetSSL_OpenSSL`, `Crypto`, `Util`, `Data` (with `Data/SQLite`, `Data/MySQL`, `Data/PostgreSQL`, `Data/ODBC`), `MongoDB`, `Redis`, `Zip`, ... |
| `<Component>/include/Poco/` | Public headers; components other than Foundation use a subdirectory that follows the namespace, for example `include/Poco/Net/` in Net, NetSSL_OpenSSL and NetSSL_Win ([Rule 6 of CODING_STYLE.md](CODING_STYLE.md#rule-6)) |
| `<Component>/src/` | Implementation files and private headers |
| `<Component>/testsuite/` | CppUnit test suite of the component (`src/`, `data/`) |
| `<Component>/samples/` | Sample programs |
| `CppUnit/` | The unit test framework used by all test suites |
| `dependencies/` | Bundled third-party libraries (see `dependencies/README.md`) |
| `cmake/` | CMake modules |
| `doc/` | User documentation pages (`*.page`, PocoDoc markup) and release notes |
| `release/` | Release and documentation packaging scripts |
| `.github/workflows/` | CI, CodeQL, fuzzing, packaging and release workflows |

## Build and test (quick reference)

Configure and build one component with its tests:

```bash
cmake -S . -B <build-dir> -DENABLE_TESTS=ON -DPOCO_MINIMAL_BUILD=ON -DENABLE_<COMPONENT>=ON
```

```bash
cmake --build <build-dir> --target <Component>-testrunner --parallel
```

Run the tests from the binary directory. `POCO_BASE` lets tests find their data files, and `bin/` on `PATH` lets tests
start helper programs:

```bash
cd <build-dir>/bin && POCO_BASE=<repo-root> PATH="$PATH:$(pwd)" ./<Component>-testrunner -all
```

Re-run the configure step after adding source files ([source files](ai-agents/cmake.md#source-files)). Details:
[CMake](ai-agents/cmake.md), [building](ai-agents/building.md), [testing](ai-agents/testing.md).

## Rules that always apply

1. **Coding style.** All C++ code follows [CODING_STYLE.md](CODING_STYLE.md). Its *Rules* are mandatory and a review
   finding on a rule is a must-fix; *Recommendations* are the default. Read the guide and the surrounding code before
   writing; new code looks like the code around it.
2. **ASCII only** in every file, as [Rule 86 of CODING_STYLE.md](CODING_STYLE.md#rule-86) states, with its check.
3. **OpenSSL** version checks and the minimum version follow [Rule 103 of CODING_STYLE.md](CODING_STYLE.md#rule-103).
4. **New files** carry the banner of [Example 1 of CODING_STYLE.md](CODING_STYLE.md#example-1); locations are in
   [writing code](ai-agents/writing-code.md#new-files).
5. **Tests.** Every bug fix and every behavior change comes with a CppUnit test that fails without the change.
6. **Smallest diff.** No unrelated refactoring or reformatting; no new third-party dependency without the maintainers'
   agreement.
7. **Definition of done:** before reporting a change as done or opening a pull request, run the checks in
   [reviewing](ai-agents/reviewing.md#definition-of-done) and report any step that was skipped.
8. **Commits and pull requests:** branch names in kebab-case with the issue number first; commit messages and PR titles
   in the form `type(scope): description` with the types `fix`, `feat`, `enh`, `chore` and the component as scope, for
   example `fix(Net): handle null socket impl`. See [git and pull requests](ai-agents/git-and-pull-requests.md).
9. **Outward actions need approval:** an action that others can see on GitHub is taken only after the maintainer
   approved that specific action, as listed in [outward writes](ai-agents/git-and-pull-requests.md#outward-writes).
10. **Security.** Do not discuss an unpublished vulnerability in public issues, pull requests or commit messages. See
    [security](ai-agents/security.md).

## Task index

| Task | Read |
|---|---|
| Prerequisites, packages, platform notes, cross-compiling | [ai-agents/building.md](ai-agents/building.md) |
| Configure and build, CMake options, change CMake files, add a component | [ai-agents/cmake.md](ai-agents/cmake.md) |
| Write or change library code | [ai-agents/writing-code.md](ai-agents/writing-code.md), [CODING_STYLE.md](CODING_STYLE.md) |
| Write and run tests | [ai-agents/testing.md](ai-agents/testing.md) |
| Sanitizers, static analysis, CodeQL, CodeRabbit | [ai-agents/analysis.md](ai-agents/analysis.md) |
| Branches, commits, pull requests, labels, milestones, cherry-picks | [ai-agents/git-and-pull-requests.md](ai-agents/git-and-pull-requests.md) |
| Review a pull request or a change | [ai-agents/reviewing.md](ai-agents/reviewing.md) |
| Port to a new platform | [ai-agents/porting.md](ai-agents/porting.md) |
| Prepare a release | [ai-agents/releasing.md](ai-agents/releasing.md) |
| Handle a security report or advisory | [ai-agents/security.md](ai-agents/security.md) |
| Update a bundled third-party library | [ai-agents/dependencies.md](ai-agents/dependencies.md) |
