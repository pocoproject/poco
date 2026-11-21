#!/usr/bin/env bash
# Prints the results of the nightly from its logs: every CTest suite with its result, the
# number of tests it ran and its time, the Unicode ODBC suites, the SQLParser tests, the
# number of coverage gaps and the end of the output of each suite that did not pass. With
# GITHUB_STEP_SUMMARY set, it also writes the results as tables to the summary page of the
# run. The summary page holds only suite names and numbers; the test output is in the job
# log only, where secrets are masked.
# Usage: poco-nightly-summary.sh LOG_DIRECTORY
set -euo pipefail
logs="$1"
strip_colors() { sed -E 's/\x1b\[[0-9;]*m//g' "$@"; }

# name|result|tests|seconds for each CTest suite, in the order CTest ran them.
ctest_rows="$(awk '
    match($0, /^[0-9]+: OK \([0-9]+ tests?\)/) {
        split($0, a, /[:( ]+/); tests[a[1]] += a[3]
    }
    match($0, /^[0-9]+: Runs: [0-9]+/) {
        split($0, a, /[: ]+/); tests[a[1]] += a[3]
    }
    match($0, /^ *[0-9]+\/[0-9]+ Test +#[0-9]+: /) {
        line = $0
        sub(/^ *[0-9]+\/[0-9]+ Test +#/, "", line)
        number = line; sub(/:.*/, "", number)
        sub(/^[0-9]+: /, "", line)
        name = line; sub(/ .*/, "", name)
        sub(/^[^ ]+ \.*/, "", line)
        seconds = line; sub(/ sec$/, "", seconds); sub(/.* /, "", seconds)
        result = line; sub(/ +[0-9.]+ sec$/, "", result); sub(/^ *\**/, "", result); sub(/ +$/, "", result)
        order[++count] = number; names[number] = name; results[number] = result; times[number] = seconds
    }
    END {
        for (i = 1; i <= count; i++) {
            n = order[i]
            printf "%s|%s|%s|%s\n", names[n], results[n], (n in tests) ? tests[n] : "-", times[n]
        }
    }' "$logs/tests.log" 2>/dev/null || true)"

# name|result|tests for each Unicode ODBC suite.
unicode_rows="$(awk '
    /^=== .* failed$/ { failed[$2] = 1; next }
    /^=== / { suite = $2 " " $3; key[suite] = $2; order[++count] = suite; next }
    /^OK \([0-9]+ tests?\)/ { split($0, a, /[( ]+/); tests[suite] = a[2]; passed[suite] = 1 }
    /^Runs: [0-9]+/ { tests[suite] = $2 }
    END {
        for (i = 1; i <= count; i++) {
            s = order[i]
            result = (passed[s] && !failed[key[s]]) ? "Passed" : "Failed"
            printf "%s|%s|%s\n", s, result, (s in tests) ? tests[s] : "-"
        }
    }' "$logs/tests-unicode.log" 2>/dev/null || true)"

sqlparser_ok=0
sqlparser_failed=0
sqlparser_checks=""
if [[ -f "$logs/sqlparser.log" ]]; then
    sqlparser_ok="$(strip_colors "$logs/sqlparser.log" | grep -c -E '^\{ +ok\}' || true)"
    sqlparser_failed="$(strip_colors "$logs/sqlparser.log" | grep -c -E '^\{ +failed\}' || true)"
    sqlparser_checks="$(strip_colors "$logs/sqlparser.log" \
        | sed -n -E 's/^(SQL Tests|Memory Leak Check|Grammar Conflict Check): +([A-Za-z]+).*/\1|\2/p')"
fi

gaps=0
[[ -s "$logs/coverage-gaps.txt" ]] && gaps="$(wc -l < "$logs/coverage-gaps.txt")"

sum() { awk -F'|' -v column="$1" '$column ~ /^[0-9]+$/ { s += $column } END { print s + 0 }'; }
count_passed() { awk -F'|' '$2 == "Passed" { n++ } END { print n + 0 }'; }
count_rows() { awk 'NF { n++ } END { print n + 0 }'; }

ctest_suites="$(count_rows <<< "$ctest_rows")"
ctest_passed="$(count_passed <<< "$ctest_rows")"
ctest_tests="$(sum 3 <<< "$ctest_rows")"
unicode_suites="$(count_rows <<< "$unicode_rows")"
unicode_passed="$(count_passed <<< "$unicode_rows")"
unicode_tests="$(sum 3 <<< "$unicode_rows")"

{
    echo "=== Test results"
    printf '%-38s %-20s %7s %8s\n' "CTest suite" "Result" "Tests" "Time"
    while IFS='|' read -r name result tests seconds; do
        [[ -n "$name" ]] || continue
        printf '%-38s %-20s %7s %6.0f s\n' "$name" "$result" "$tests" "$seconds"
    done <<< "$ctest_rows"
    echo "CTest: $ctest_passed of $ctest_suites suites passed, $ctest_tests tests"
    echo
    printf '%-44s %-20s %7s\n' "Unicode ODBC suite" "Result" "Tests"
    if [[ "$unicode_suites" == 0 ]]; then
        echo "not run"
    fi
    while IFS='|' read -r name result tests; do
        [[ -n "$name" ]] || continue
        printf '%-44s %-20s %7s\n' "$name" "$result" "$tests"
    done <<< "$unicode_rows"
    echo "Unicode ODBC: $unicode_passed of $unicode_suites suites passed, $unicode_tests tests"
    echo
    echo "SQLParser: $sqlparser_ok tests passed, $sqlparser_failed failed"
    while IFS='|' read -r check result; do
        [[ -n "$check" ]] || continue
        echo "SQLParser $check: $result"
    done <<< "$sqlparser_checks"
    echo
    echo "Coverage gaps: $gaps"
    # The end of the output of each CTest suite that did not pass.
    while IFS='|' read -r name result tests seconds; do
        [[ -n "$name" && "$result" != Passed ]] || continue
        number="$(sed -n -E "s/^ *[0-9]+\/[0-9]+ Test +#([0-9]+): $name .*/\1/p" "$logs/tests.log" | head -n 1)"
        echo
        echo "=== Last 100 lines of $name ($result)"
        grep -a -E "^$number: " "$logs/tests.log" | tail -n 100 || true
    done <<< "$ctest_rows"
    if [[ "$sqlparser_failed" != 0 ]] || grep -a -q -E 'Failure|failed' <<< "$sqlparser_checks"; then
        echo
        echo "=== Last 40 lines of the SQLParser tests"
        strip_colors "$logs/sqlparser.log" | tail -n 40
    fi
}

if [[ -n "${GITHUB_STEP_SUMMARY:-}" ]]; then
    {
        echo "## Test results"
        echo
        echo "CTest: $ctest_passed of $ctest_suites suites passed, $ctest_tests tests."
        echo "Unicode ODBC: $unicode_passed of $unicode_suites suites passed, $unicode_tests tests."
        echo "SQLParser: $sqlparser_ok tests passed, $sqlparser_failed failed."
        if [[ "$gaps" == 0 ]]; then
            echo "Coverage gaps: none."
        else
            echo "Coverage gaps: $gaps, listed in the job log."
        fi
        echo
        echo "| CTest suite | Result | Tests | Time (s) |"
        echo "|---|---|--:|--:|"
        while IFS='|' read -r name result tests seconds; do
            [[ -n "$name" ]] || continue
            printf '| %s | %s | %s | %.0f |\n' "$name" "$result" "$tests" "$seconds"
        done <<< "$ctest_rows"
        echo
        echo "| Unicode ODBC suite | Result | Tests |"
        echo "|---|---|--:|"
        while IFS='|' read -r name result tests; do
            [[ -n "$name" ]] || continue
            printf '| %s | %s | %s |\n' "$name" "$result" "$tests"
        done <<< "$unicode_rows"
        echo
        echo "| SQLParser check | Result |"
        echo "|---|---|"
        while IFS='|' read -r check result; do
            [[ -n "$check" ]] || continue
            printf '| %s | %s |\n' "$check" "$result"
        done <<< "$sqlparser_checks"
    } >> "$GITHUB_STEP_SUMMARY"
fi
