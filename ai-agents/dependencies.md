# Bundled Dependencies

Third-party libraries that POCO ships are in `dependencies/<name>/`. `dependencies/README.md` lists each library with
its version, licence, the components that use it, whether it can be replaced by a system library, and maintenance notes.
Code that is embedded in POCO sources (algorithms, `CppUnit`, `Data/SQLParser`) is listed there too.

## Unbundled builds

- `-DPOCO_UNBUNDLED=ON` uses system libraries instead of the bundled zlib, pcre2, utf8proc, expat, SQLite and png (the
  libraries marked "Unbundleable" in `dependencies/README.md`).
- `-DPOCO_SQLITE_UNBUNDLED=ON` replaces only SQLite.
- The other libraries are always built from the bundled sources.

## Scripts

Compare the bundled versions with the latest upstream releases. The script reads the `# Version:` comment at the top of
`dependencies/<name>/CMakeLists.txt`; it needs `curl` and uses `GITHUB_TOKEN` when set:

```bash
dependencies/check-upstream-versions.sh
```

```bash
dependencies/check-upstream-versions.sh expat sqlite3
```

Update one library: the script downloads the upstream release, overwrites each file that is already bundled in
`dependencies/<name>/` with its upstream counterpart, and updates the version markers in the library's `CMakeLists.txt`
and in `dependencies/README.md`:

```bash
dependencies/update-bundled-dep.sh <name> <new-version>
```

The script's usage text lists the supported libraries and the environment variables for SQLite and 7-Zip download URLs.

## Update procedure

One commit per library; the pull request keeps its commits (see [merging](git-and-pull-requests.md#merging)).

1. Read the notes for the library in `dependencies/README.md` and in the header of
   `dependencies/check-upstream-versions.sh` (for example the local change in `tessil` and the FastLogger platform list
   for `quill`).
2. Check the latest upstream release, its activity, and the CVEs fixed since the bundled version. An upstream project
   may be archived or retired; note that in `dependencies/README.md` instead of updating blindly.
3. Download the official release and verify its signature or checksum.
4. Find local modifications: diff the bundled copy against the upstream release of the bundled version, and carry the
   modifications over to the new version.
5. Copy only the files already bundled (`update-bundled-dep.sh` does this). Keep the POCO-specific configuration
   headers.
6. Check the library's `CMakeLists.txt`: it collects sources with `file(GLOB)` and excludes some files explicitly. Files
   added or removed upstream may need an exclusion or an explicit entry so that the source list matches the files.
7. Check the version in `dependencies/README.md` and in the `# Version:` and `# Source:` comments of
   `dependencies/<name>/CMakeLists.txt`; update them by hand where the script was not used.
8. Review the change:

   ```bash
   git diff --stat dependencies/<name>
   ```

9. Build and run the test runners of every component that uses the library (see [testing.md](testing.md)), also with
   ASan and UBSan (see [analysis.md](analysis.md)).
10. Check the runtime version where the library reports it, for example `XML_ExpatVersion()` or `sqlite3_libversion()`.
11. List the pull request in `CHANGELOG` under "Bundled Third-Party Library Upgrades" as `library old -> new` with the
    CVE references; a security-relevant update also goes under "Security Fixes" (see [releasing.md](releasing.md)).

## Known pitfalls

**The concurrency tests of Data/SQLite hang or fail after an SQLite update.**
Cause: SQLite updates can change internal locking; an API such as `sqlite3_get_autocommit()` may take the connection
mutex.
What to do: run the concurrency tests of `Data/SQLite` (DataSQLite runner) after every update.

**An optional SQLite feature such as FTS5 is missing although the version has it.**
Cause: optional SQLite features are compile options, not part of a version.
What to do: detect them at runtime with `sqlite3_compileoption_used()`, not by version.

**Code that uses `sqlite-vec` fails to build or its tests fail after an update.**
Cause: `sqlite-vec` is pre-1.0 and is bundled from its amalgamation archive; releases can break the API.
What to do: read the upstream release notes and adapt the calling code in the same commit.

**`update-bundled-dep.sh` warns about a bundled file.**
Cause: the script matches files by path and then by base name; a bundled file that it does not find upstream is listed
as a warning.
What to do: decide by hand whether the file was renamed, removed or is a local addition.

**A downloaded release was not verified.**
Cause: `update-bundled-dep.sh` does not verify signatures or checksums.
What to do: verify the download separately (step 3 of the update procedure).
