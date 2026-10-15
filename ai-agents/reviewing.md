# Reviewing

How to review a pull request or a local change. Tool commands are in [analysis.md](analysis.md); the style rules are in
[CODING_STYLE.md](../CODING_STYLE.md).

## Inputs

```bash
gh pr view <N> -R pocoproject/poco
```

```bash
gh pr diff <N> -R pocoproject/poco
```

- Read the linked issue, the whole diff and the code around each hunk.
- Fetch the branch to build and test it locally (see [git-and-pull-requests.md](git-and-pull-requests.md)).

## Tools first

- Use the review skills or sub-agents of the agent where they exist: general code review; security review for changes to
  input parsing, network, cryptography, file-system and credential code; simplification and code-quality review;
  error-handling and silent-failure review; test-coverage review.
- Run the checks of [analysis.md](analysis.md) on the changed files: compiler warnings, clang-tidy, cppcheck, the
  touched test suites under ASan, UBSan and TSan, and CodeRabbit.
- A missing tool is reported with its installation hint from [analysis.md](analysis.md), not skipped silently.
- Tool output is input to the review, not the verdict. Verify each finding against the code.

## Dimensions

1. **Correctness and regressions:** behavior changes for existing callers, object lifetime and ownership (use after
   free), data races and lock order, deadlocks and hangs at teardown, error and timeout paths, cleanup after a partial
   failure, durability (data is synced before it is reported as persisted), integer overflow and size types, platform
   differences (Windows, macOS, Linux, 32-bit).
2. **Design and pattern fit:** the pattern matches the problem (builder, producer and consumer with state, strategy,
   command or task queue, observer or events); no second way to do one thing where one would do.
3. **Simplification and deduplication:** reuse of the standard library, Poco classes and project helpers. Duplicated
   logic is extracted into a helper as described in [writing-code.md](writing-code.md#before-writing).
4. **Dead code and redundant tests:** vestigial parameters and members, unreachable branches, an API without a
   production caller, tests that assert nothing.
5. **Performance:** regressions on hot paths (copies, allocations in loops, extra system calls, locking) and
   improvements the change makes possible.
6. **Tests:** a test exists for each behavior change and fails without it (see [testing.md](testing.md)). Missing tests
   are named: error, abort and timeout paths, paths that cross a buffer or message boundary, and concurrent use.
7. **Clarity, comments and naming:** names, function length, simple control flow. Comments explain why, not what, are
   still accurate after a refactoring, and carry no ticket numbers.
8. **Language idioms:** `[[nodiscard]]` on the declaration of every function whose result must not be ignored (C++17, so
   it needs no guard; mandatory in new code, [Rule 94 of CODING_STYLE.md](../CODING_STYLE.md#rule-94)). C++20 forms
   where they apply: `std::span` instead of a pointer and a length, `std::jthread` with `std::stop_token` and
   `std::condition_variable_any` for prompt shutdown, ranges, `std::bit_cast` instead of `reinterpret_cast`; they need
   the feature guards of the section "Language Standard and Feature Guards" of
   [CODING_STYLE.md](../CODING_STYLE.md#20-language-standard-and-feature-guards), because `main` must still build as
   C++17.
9. **Style:** conformance to [CODING_STYLE.md](../CODING_STYLE.md). Its rules are must-fix in new and changed code, also
   where existing code on `main` does not follow them.
10. **API and ABI:** changes to public headers are deliberate and documented; removed API is deprecated first
    (`POCO_DEPRECATED`); a breaking change is labelled `breaking` and listed under "Breaking Changes" in `CHANGELOG`.
11. **Text:** ASCII only; documentation and `CHANGELOG` entries match the code.

## Findings

- Grade each finding:
  - **must-fix:** defect, regression, missing test for a behavior change, style rule violation, non-ASCII text;
  - **should-fix:** readability, naming, a missed simplification;
  - **note:** an observation that needs no change.
- Each finding names `file:line` and a concrete fix.
- Prefer fewer, verified findings to many uncertain ones.
- Report in the chat or session by default. Post a review on GitHub only when the maintainer asks for it (see
  [outward writes](git-and-pull-requests.md#outward-writes)).

## Posting a review

A posted review is one review with a summary and inline comments. One-line fixes go in suggestion blocks. Create it with
a JSON payload:

```bash
gh api -X POST repos/pocoproject/poco/pulls/<N>/reviews --input review.json
```

`review.json` holds `commit_id`, `body`, `event` (`COMMENT`, `REQUEST_CHANGES` or `APPROVE`) and `comments` (each with
`path`, `line`, `side` and `body`).

- A POST that returns an error may still have created the review. List the reviews before retrying:

  ```bash
  gh api repos/pocoproject/poco/pulls/<N>/reviews --paginate --jq '.[] | "\(.id) \(.state) \(.user.login)"'
  ```

- Read the posted review back and report what was posted.

## Follow-up rounds

- On each new head, check every earlier finding and state which are resolved and which remain.
- A head that does not compile is returned without further review.
- Only the reviewer who requested changes dismisses that review.

## Own pull requests

When reviewing your own change, fix the findings directly instead of writing comments, then run the checks again.

## Definition of done

A change is done, and a pull request may be opened, only after all of these steps ran. The same list applies to your own
changes and to changes you review.

1. A clean build of the affected components, configured from scratch with `ENABLE_COMPILER_WARNINGS=ON` (see
   [cmake.md](cmake.md#options)), with no new warnings.
2. The full test suites of the affected components, run again from scratch (see [testing.md](testing.md)).
3. The analysers of [analysis.md](analysis.md) on the changed files: clang-tidy, cppcheck, and ASan, UBSan and TSan when
   tests cover the changed code. Concurrency, transport or storage code also gets the remaining
   [extra quality gates](analysis.md#extra-quality-gates): coverage, fuzzing or malformed-input tests of receive paths,
   and the performance regression check.
4. The review skills or sub-agents of the agent, as listed under "Tools first".
5. The whole diff, read hunk by hunk against [CODING_STYLE.md](../CODING_STYLE.md) and the dimensions above, with the
   tool findings beside it. Each hunk gets a verdict.

A step that was skipped is reported as skipped, with the reason (for example a tool that is not installed). "Done" means
that all steps ran, their results are reported, and no must-fix finding is open.
