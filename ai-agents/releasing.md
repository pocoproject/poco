# Releasing

Steps for a release `X.Y.Z`. A patch release is prepared on its release branch `poco-X.Y.Z`. A major or minor release is
prepared on `main`: the release-candidate pull request goes into `main`, the release commit is tagged there, and the
release branch `poco-X.Y.0` starts at the release tag (1.14.0 and 1.15.0 were released this way). Steps that change
GitHub need approval (see [outward writes](git-and-pull-requests.md#outward-writes)).

## 1. Milestone hygiene

In the GitHub milestone `Release X.Y.Z`:

- every issue and pull request has at least one of the labels listed in
  [pull requests](git-and-pull-requests.md#pull-requests);
- every issue has an issue type (Bug, Task, Feature); the GraphQL commands are in
  [git-and-pull-requests.md](git-and-pull-requests.md);
- a pull request that closes an issue has that issue in the same milestone; add a missing issue to the milestone and the
  matching project;
- titles are clear and free of typos.

## 2. Version files

| File | Change |
|---|---|
| `VERSION` | Version string, for example `1.15.4` |
| `libversion` | Shared library SONAME number: a minor release moves to the next multiple of ten (1.14.0 -> 110, 1.15.0 -> 120), a patch release adds one (1.15.4 -> 124); a major release may jump (2.0.0 -> 200) |
| `Foundation/include/Poco/Version.h` | `POCO_VERSION` as `0xAABBCCDD`: AA major, BB minor, CC patch (hexadecimal), DD `00` for a stable release (`Dx`, `Ax`, `Bx` for development, alpha, beta). 1.15.4 is `0x010F0400` |
| `DLLVersion.rc` | `POCO_VERSION` (`1,15,4,0`) and `POCO_VERSION_STR` (`"1.15.4"`) |
| `README.md` | Version in the Conan examples (`conan install --requires=poco/X.Y.Z`) |

## 3. CHANGELOG

Insert the section at the top, after the header line, or complete the `Release X.Y.Z (unreleased)` section that `main`
carries:

```text
Release X.Y.Z (YYYY-MM-DD)
==========================

Summary of Changes:

<short description>
```

The underline is as long as the title. Groups, in this order:

1. **Breaking Changes:** label `breaking`.
2. **Security Fixes:** label `security` (not breaking); advisories as
   `GHSA-xxxx-xxxx-xxxx <Component>: <summary> (reported by <name>)`.
3. **Bundled Third-Party Library Upgrades:** `library old -> new` with CVE references.
4. **Features, Enhancements and Third Party Updates:** labels `enhancement`, `feature`, `third-party`.
5. **Bug Fixes and Improvements:** everything else.

- List the issue (`GH #<N>`) when a closing pull request exists, the pull request (`PR #<N>`) when there is no issue;
  never both. Items within a group are in descending number order.
- Releases may add groups such as "Behaviour changes" in the summary, "API Changes", "Performance Improvements" or, for
  a major release, "Contributors" and "Notable Features". Follow the latest section of `CHANGELOG`.

Draft the lists from the milestone, then compose the section by hand:

```bash
./gh-cli-for-release-notes.sh "Release X.Y.Z"
```

## 4. Release notes page

`doc/99100-ReleaseNotes.page` gets the same content in PocoDoc markup: `!!!Release X.Y.Z` for the release, `!!<Group>`
for each group, items as two spaces, a dash and a space (`  - GH #<N> ...`).

- Name classes and members fully qualified and in italics, as the 2.0.0 section does
  (`<*Poco::Net::HTTPClientSession::sendRequest()*>`), so that PocoDoc can resolve the name and link it to the reference
  documentation.
- Changes that break compatibility or need action from users go under `!!Breaking Changes` with a migration note per
  item (1.15.0 uses `!!Breaking Changes and Migration Notes`). Release notes up to 1.12.0 use
  `!!Incompatible Changes and Possible Transition Issues` for the same purpose.
- Update the platform notes when the release changes what they describe: `doc/99150-WindowsPlatformNotes.page`,
  `doc/99250-VxWorksPlatformNotes.page`, `doc/99300-AndroidPlatformNotes.page`.
- Update `doc/90100-Acknowledgements.page`, which reproduces the licence notices of the bundled third-party code, when a
  bundled library is added or removed or its notice changes (see [dependencies.md](dependencies.md)).

## 5. Contributors

```bash
gh pr list -R pocoproject/poco -S 'milestone:"Release X.Y.Z"' -s all -L 1000 --json author --jq '.[].author.login' | sort -u
```

Check each handle against `CONTRIBUTORS`. For a new one, find the name in `git log`, with `gh api users/<handle>`, or in
the handle's commits elsewhere. Add `Full Name`, or `handle <email>` when no name is known.

## 6. Verification

```bash
grep -n "<old-version>" VERSION libversion Foundation/include/Poco/Version.h DLLVersion.rc README.md
```

```bash
gh issue list -R pocoproject/poco -S 'milestone:"Release X.Y.Z"' -s all -L 1000 --json number,labels --jq '.[] | select(.labels | length == 0)'
```

```bash
gh pr list -R pocoproject/poco -S 'milestone:"Release X.Y.Z"' -s all -L 1000 --json number,labels --jq '.[] | select(.labels | length == 0)'
```

```bash
bash -c 'for n in $(gh issue list -R pocoproject/poco -S "milestone:\"Release X.Y.Z\"" -s all -L 1000 --json number --jq ".[].number"); do t=$(gh api repos/pocoproject/poco/issues/$n --jq ".type.name // \"NONE\""); [ "$t" = NONE ] && echo "Issue #$n has no type"; done'
```

```bash
LC_ALL=C grep -n '[^[:print:][:space:]]' CHANGELOG doc/99100-ReleaseNotes.page CONTRIBUTORS
```

Static analysis, early enough to fix what it finds:

- Triage the open CodeQL alerts of the release-candidate pull request (CodeQL runs on pull requests into every branch,
  see [analysis.md](analysis.md#codeql)):

  ```bash
  gh api 'repos/pocoproject/poco/code-scanning/alerts?ref=refs/pull/<N>/merge&state=open&per_page=100' --paginate --jq '.[] | "\(.number) \(.rule.id) \(.most_recent_instance.location.path):\(.most_recent_instance.location.start_line)"'
  ```

- Run [clang-tidy](analysis.md#clang-tidy) and [cppcheck](analysis.md#cppcheck) on the source files changed since the
  previous release:

  ```bash
  git diff --name-only --diff-filter=AM poco-<previous-version>-release HEAD -- '*.cpp' ':!dependencies'
  ```

## 7. Release candidate, release commit and tag

- Open a release-candidate pull request with the release files into `poco-X.Y.Z` (patch release) or `main` (major or
  minor release); wait for green CI and a review.
- The release commit sets the date in `CHANGELOG` and the release notes and contains `CONTRIBUTORS`.
- Tag it with an annotated tag; for a major or minor release, create `poco-X.Y.0` at the tag:

  ```bash
  git tag -a poco-X.Y.Z-release -m "Release X.Y.Z"
  ```

## 8. Packages and documentation

- `release/script/mkrel` builds the source archives from the specs in `release/spec/` (`mkrel` for the base package,
  `mkrel all` for `all.release`; `-c unix2dos` for the Windows archives). `release/script/mkdoc all` builds the
  reference documentation. Both need `POCO_BASE` set and `release/script` on `PATH`:

  ```bash
  export POCO_BASE=<repo-root> PATH=<repo-root>/release/script:$PATH
  ```

  ```bash
  mkrel all
  ```

- `.github/workflows/packages-qa.yml` builds the archives and the documentation on every push and test-builds the
  archives with CMake.
- `.github/workflows/publish_release.yml` (manual `workflow_dispatch`, input `channel`: `releases-staging` or
  `releases`) builds the archives and documentation, uploads them to the project download server and signs them there.
  Run the staging channel first.
- The GitHub release `Release X.Y.Z` on the tag carries the archives, the documentation archives and `SHASUMS256.txt`
  with its signatures (`.asc`, `.sig`).

## 9. After the release

- Publish the security advisories of the release (see [security.md](security.md)).
- Conan Center: a recipe update pull request in `conan-io/conan-center-index`; vcpkg: a port update where it is
  maintained.
- Website and announcement by the maintainers.
- Forward-port the release-branch fixes to `main` (see [git-and-pull-requests.md](git-and-pull-requests.md)).
