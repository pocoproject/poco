import re


def find_coverage_gaps(contents):
    pattern = re.compile(
        r"(?:FAILURE|ERROR) \(ignored\)|\btests? (?:skipped|not available|not executed)\b|\bskipping\b|no pseudo terminal available|SQL Server extensions not enabled",
        re.IGNORECASE,
    )
    excluded = re.compile(
        r"(?:IBM DB2 ODBC DRIVER - DB2COPY1|Informix ODBC Driver) driver NOT found, tests not available\.",
        re.IGNORECASE,
    )
    return [line for line in contents.splitlines() if pattern.search(line) and not excluded.search(line)]
