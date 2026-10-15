# Security Fixes and Advisories

How vulnerabilities in POCO are reported, fixed and published. Advisory changes, CVE requests and pushes need approval
(see [outward writes](git-and-pull-requests.md#outward-writes)).

## Reporting

- Private vulnerability reporting is enabled for `pocoproject/poco`: report through "Report a vulnerability" on the
  repository's Security tab, which opens a draft security advisory.
- Reports can also go to the security e-mail address given in [CONTRIBUTING.md](../CONTRIBUTING.md).
- Do not file a public issue or pull request for an unpublished vulnerability.

## Developing the fix

- Fixes for unpublished advisories are developed privately: in the advisory's temporary private fork or on a branch that
  is not pushed to the public repository.
- Until the release is available, public pull request titles, descriptions, branch names and commit messages are
  neutral: no advisory ID, no CVE, no description of the exploit.
- Each fix commit for an advisory ends with the line

  ```text
  Fixes GHSA-xxxx-xxxx-xxxx
  ```

  If the line is removed for a public pre-release pull request, restore it on the release branch once CI passes.
- A fix has a regression test that fails without it (see [testing.md](testing.md)), and the touched tests pass under the
  sanitizers (see [analysis.md](analysis.md)).
- Parsers of untrusted input are checked with truncated and malformed input as well.

## Advisory content

Write in a factual tone, without speculation about impact beyond what was verified:

- summary (one line: component and defect);
- description: the defect, the conditions under which it is reachable, the effect;
- affected and patched version ranges;
- CWE;
- CVSS 3.1 vector;
- credits (reporter, finder);
- maintainer notes: configuration workarounds for users who cannot update.

## Release

- Security releases follow [releasing.md](releasing.md). The `CHANGELOG` and the release notes list each advisory under
  "Security Fixes" in the format given in [releasing.md](releasing.md#3-changelog).
- Advisories are published after the release packages are available.

## Known pitfalls

**An advisory cannot be published.**
Cause: a pull request in its temporary private fork is still open.
What to do: close the pull request first.

**A comment cannot be posted in the temporary private fork.**
Cause: GitHub does not offer comments in temporary private forks.
What to do: discuss in the advisory itself.

**The fix commits are gone after the advisory was published.**
Cause: publishing an advisory deletes its temporary private fork.
What to do: keep the fix commits on a branch elsewhere before publishing.

## After publication

- Forward-port each fix to `main` and to the other maintained release branches (see
  [git-and-pull-requests.md](git-and-pull-requests.md)), keeping the `Fixes GHSA-...` line.
- Check that the advisory shows the patched versions and the credits as intended.
