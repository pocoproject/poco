# Instructions for Contributors and AI Agents

Tool-neutral instructions for AI coding agents and the contributors who work with them on the POCO C++ Libraries. The
entry point is [AGENTS.md](../AGENTS.md); the code style is in [CODING_STYLE.md](../CODING_STYLE.md).

| File | Read it when you |
|---|---|
| [building.md](building.md) | check the prerequisites (compiler, packages), build on Windows or Linux, cross-compile |
| [cmake.md](cmake.md) | configure and build with CMake, choose options, change CMake files, add a component |
| [writing-code.md](writing-code.md) | change or add library code: reuse, new files, platform code, deprecation, OpenSSL, protocol checks, documentation |
| [testing.md](testing.md) | write or run CppUnit tests, run database suites, judge a flaky test |
| [analysis.md](analysis.md) | run sanitizers, warnings builds, clang-tidy, cppcheck, fuzzing, or triage CodeQL and CodeRabbit findings |
| [git-and-pull-requests.md](git-and-pull-requests.md) | name a branch, write a commit, open or merge a pull request, port a fix between branches, or write anything to GitHub |
| [reviewing.md](reviewing.md) | review a pull request or your own change |
| [porting.md](porting.md) | port POCO to a new operating system, compiler or C library |
| [releasing.md](releasing.md) | prepare and publish a release |
| [security.md](security.md) | handle a vulnerability report, develop a security fix or publish an advisory |
| [dependencies.md](dependencies.md) | update a bundled third-party library or build against system libraries |

The rules that apply to every task (style, ASCII only, tests, smallest diff, approval of outward actions, security) are
listed in [AGENTS.md](../AGENTS.md).

## Local notes

`ai-agents/local/` is ignored by git. It holds notes that are specific to one machine or one person: build hosts or
virtual machines, build-tree locations, tool paths, personal workflows. Agents load them through an untracked local
instruction file; for Claude Code that is `CLAUDE.local.md` in the repository root, with lines such as:

```text
@ai-agents/local/build-env.md
```

Other agents use their own mechanism for untracked local instructions.

NOTE: An ignored file exists only in the checkout where it was created. To share local notes between several
checkouts or worktrees, keep them in the home directory and import them from `CLAUDE.local.md`
(`@~/.claude/poco-local.md`).

The tracked `CLAUDE.md` contains only `@AGENTS.md`: Claude Code reads `AGENTS.md` on its own only when no `CLAUDE.md`
or `CLAUDE.local.md` exists, so the import keeps `AGENTS.md` loaded in every setup.
