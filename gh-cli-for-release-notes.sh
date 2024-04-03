#!/usr/bin/env bash

#
# Helper script to prepare changelog for a release.
#
# It filters GitHub issues and pull requests for the specified milestone and
# prints a list on standard output. The generated list needs to be reviewed by
# a maintainer before including it to the CHANGELOG.
#
# It uses GitHub CLI from https://github.com/cli/cli
#
# Usage: gh-cli-release-notes.sh <GitHub milestone name>
#

if [[ $# -eq 0 ]] ; then
    echo 'Usage: gh-cli-release-notes.sh <GitHub milestone name>'
    exit 1
fi


MILESTONE=$1

echo
echo "${MILESTONE} ($(date "+%Y-%m-%d"))"
echo ===========================
echo
echo "Summary of Changes:"
echo
echo "Breaking Changes:"
echo

gh issue list -S 'milestone:"'"${MILESTONE}"'" label:breaking' -s all -L 500 --json number,title --jq '.[] | "- GH #\(.number) \(.title)"'
gh pr    list -S 'milestone:"'"${MILESTONE}"'" label:breaking' -s all -L 500 --json number,title --jq '.[] | "- PR #\(.number) \(.title)"'


echo
echo "Security Fixes:"
echo

gh issue list -S 'milestone:"'"${MILESTONE}"'" -label:breaking label:security' -s all -L 500 --json number,title --jq '.[] | "- GH #\(.number) \(.title)"'
gh pr    list -S 'milestone:"'"${MILESTONE}"'" -label:breaking label:security' -s all -L 500 --json number,title --jq '.[] | "- PR #\(.number) \(.title)"'

echo
echo "Features, Enhancements and Third Party Updates:"
echo

gh issue list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:security label:enhancement' -s all -L 500 --json number,title --jq '.[] | "- GH #\(.number) \(.title)"'
gh issue list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:security -label:enhancement label:feature' -s all -L 500 --json number,title --jq '.[] | "- GH #\(.number) \(.title)"'
gh issue list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:security -label:enhancement -label:feature label:third-party' -s all -L 500 --json number,title --jq '.[] | "- GH #\(.number) \(.title)"'
gh pr    list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:security label:enhancement' -s all -L 500 --json number,title --jq '.[] | "- PR #\(.number) \(.title)"'
gh pr    list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:security -label:enhancement label:feature' -s all -L 500 --json number,title --jq '.[] | "- PR #\(.number) \(.title)"'
gh pr    list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:security -label:enhancement -label:feature label:third-party' -s all -L 500 --json number,title --jq '.[] | "- PR #\(.number) \(.title)"'

echo
echo "Bug Fixes and Improvements:"
echo

gh issue list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:enhancement -label:feature -label:security -label:third-party' -s all -L 500 --json number,title --jq '.[] | "- GH #\(.number) \(.title)"'
gh pr    list -S 'milestone:"'"${MILESTONE}"'" -label:breaking -label:enhancement -label:feature -label:security -label:third-party' -s all -L 500 --json number,title --jq '.[] | "- PR #\(.number) \(.title)"'

echo
