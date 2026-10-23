# Git and Pull Requests

## Branches

| Branch | Purpose |
|---|---|
| `main` | Development of the next major or minor release (2.0) |
| `poco-X.Y.Z` | Patch line of a release, for example `poco-1.15.4` |
| `devel` | Not used for development; kept |

- Releases are tagged with annotated tags `poco-X.Y.Z-release` (see [releasing.md](releasing.md)).
- Never delete `poco-*` release branches or `devel`.
- Feature branches use kebab-case with the issue number first when there is one: `5243-codeql-alerts`,
  `concatenate-namespace-declarations`.
- Check `git status` before switching branches; use a separate build directory per branch (see
  [cmake.md](cmake.md#configure-and-build)).
- Changes in the working tree that you did not make belong to someone else: keep them, mention them, and never revert
  them. Ask when they conflict with your change.
- Before a git sequence of several steps (creating branches, cherry-picks, rebases, pushes, creating a pull request),
  show the plan: branch names, the commits involved, the pull request title and body. Execute it after approval; its
  outward writes still need their own approval (see [outward writes](#outward-writes)).

## Commits

- Conventional format: `type(scope): description` with the types `fix`, `feat`, `enh` and `chore`. The scope is the
  component (`Net`, `Data/SQLite`, `NetSSL`); several are separated by commas.
- The issue number may follow the subject: `fix(Net): handle null socket impl #5243`.
- Keep messages short: the subject line alone when it suffices; at most a short body when the reason is not visible in
  the diff. No bullet lists that restate every changed file.
- One logical change per commit. Security, dependency and audit work keeps one commit per fix or per library.

## Writing style for commits, pull requests, issues and documentation

Applies to commit messages, pull request and issue text, GitHub comments and documentation. Comments in code follow the
section "Comments" of [CODING_STYLE.md](../CODING_STYLE.md#45-comments).

- Clear standard technical English with plain vocabulary. No marketing tone and no filler adjectives. The same term for
  the same concept throughout a text.
- Documentation describes things as they are: no "now", "new" or "since version X". History belongs in `CHANGELOG`.
- Constraints go into short NOTE lines, not into long paragraphs.
- ASCII only ([Rule 86 of CODING_STYLE.md](../CODING_STYLE.md#rule-86)).
- Nothing public contains host names, local paths, user names or IP addresses.
- An issue or pull request comment states the technical problem and cites what its readers can open: tracked paths on
  named branches, issue and pull request numbers.

## Pull requests

- Title in the commit format.
- The description has about 150 words: the link to the issue (`Closes #<N>`), two or three sentences on the problem, one
  bullet per area of change, one line on tests, one line on follow-ups. Details belong in the issue.
- Labels: every issue and pull request has at least one of `bug`, `feature`, `enhancement`, `security`, `third-party`,
  `breaking`, `documentation`, `platform_specific`.
- Milestone `Release X.Y.Z`. A pull request that closes an issue has the issue in the same milestone. Whenever a
  milestone is set, also add the item to the matching GitHub project:

  ```bash
  gh project list --owner pocoproject -L 100
  ```

  ```bash
  gh project item-add <project-number> --owner pocoproject --url <issue-or-pr-url>
  ```

- Issue types (Bug, Task, Feature) are independent of labels and are set with GraphQL, since `gh issue edit` cannot set
  them. Mapping: label `bug` -> Bug, `feature` -> Feature, others -> Task.

  ```bash
  gh api graphql -f query='{ repository(owner:"pocoproject", name:"poco") { issueTypes(first:10) { nodes { id name } } } }'
  ```

  ```bash
  NODE_ID=$(gh api repos/pocoproject/poco/issues/<N> --jq '.node_id')
  ```

  ```bash
  gh api graphql -f query="mutation { updateIssue(input: {id: \"$NODE_ID\", issueTypeId: \"<TYPE_ID>\"}) { issue { number issueType { name } } } }"
  ```

## Merging

- Security, dependency-update, audit and forward-port pull requests keep their commits (merge commit or rebase merge, no
  squash). Contributor pull requests and small housekeeping pull requests may be squashed. When unsure, ask the
  maintainer and name the method.
- Pull requests that change `.github/workflows/*.yml` can be blocked by repository rules for automated merges; the
  maintainer merges them in the web interface.

## Fixes for a release line

- Either open the pull request against `poco-X.Y.Z` and forward-port it to `main`, or open it against `main` and
  cherry-pick it to the release branch after the merge:

  ```bash
  git cherry-pick -x <commit>
  ```

- Record in the pull request which way was taken.
- Forward-port in topic pull requests. Check whether a fix is already on `main` by content (files and tests), not with
  `git cherry`, which is unreliable once the branches diverge. Apply prerequisite fixes first. After all ports, sweep
  the release-branch commits for anything still missing on `main`.

## Contributor pull requests

Fetch a pull request locally:

```bash
git fetch origin +refs/pull/<N>/head:pr/<N>
```

- CI runs of first-time contributors wait for maintainer approval. Approve only after reading the diff.
- To take over a stale contributor branch: rebase it onto `main`, keep the contributor's commits, and push with
  `--force-with-lease` after approval.

## History rewrites

- Before rewriting history, create a backup ref:

  ```bash
  git branch backup/<branch> <branch>
  ```

- Force-push only with `git push --force-with-lease`.

## Outward writes

These actions are visible to others. An agent takes each of them only after the maintainer approved that specific
action:

- pushes, including force-pushes and pushes of tags;
- opening, editing, closing and merging pull requests; approving CI runs;
- comments and reviews on issues, pull requests and commits, and dismissing a review;
- creating, editing and closing issues; setting labels, milestones, issue types and project items;
- dismissing or reopening CodeQL alerts;
- publishing or editing releases and running release workflows;
- creating, editing and publishing security advisories, and CVE requests.

Local operations (creating branches, cherry-picks, rebases) need the shown plan of [Branches](#branches), not a separate
approval each. For an outward write:

1. Draft the text or describe the action, and show it.
2. Act only after an explicit go for this write; an earlier approval does not cover the next one.
3. Read the result back (for example `gh pr view`, `gh api`) and report what was written.

## Cleanup after a merge

Delete the feature branch and its build directory only after confirming that the content is on the target branch:

- merge or rebase merge: the branch tip is an ancestor of the target
  (`git merge-base --is-ancestor <branch> origin/main`);
- squash merge: the files of the branch match the target (`git diff <branch> origin/main -- <paths>` is empty).
