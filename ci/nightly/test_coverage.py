import unittest
from nightly_coverage import EXPECTED_TESTS, find_coverage_gaps, missing_suites


class CoverageTest(unittest.TestCase):
    def test_missing_suite_fails(self):
        self.assertEqual(missing_suites(EXPECTED_TESTS - {"Modules"}), ["Modules"])
        self.assertEqual(missing_suites(EXPECTED_TESTS), [])

    def test_missing_coverage_fails(self):
        messages = [
            "Not connected, test skipped.",
            "ICMP sockets not available (requires elevated privileges), tests skipped.",
            "SQLite FTS not enabled, test not executed.",
            "SQL Server extensions not enabled.",
            "Oracle 23 ODBC driver driver NOT found, tests not available.",
            "no pseudo terminal available, skipping",
            "FAILURE (ignored)",
            "ERROR (ignored)",
        ]
        for message in messages:
            with self.subTest(message=message):
                self.assertEqual(find_coverage_gaps(message), [message])

    def test_only_approved_missing_drivers_are_excluded(self):
        messages = "\n".join([
            "IBM DB2 ODBC DRIVER - DB2COPY1 driver NOT found, tests not available.",
            "Informix ODBC Driver driver NOT found, tests not available.",
        ])
        self.assertEqual(find_coverage_gaps(messages), [])
        self.assertEqual(find_coverage_gaps("DB2 FAILURE (ignored)"), ["DB2 FAILURE (ignored)"])

    def test_normal_output_is_not_a_gap(self):
        self.assertEqual(find_coverage_gaps("Test passed\n100% tests passed"), [])


if __name__ == "__main__":
    unittest.main()
