from pathlib import Path
import socket
import unittest
import xml.etree.ElementTree as ET
from redact_logs import redact


class RedactionTest(unittest.TestCase):
    def test_machine_details_are_removed(self):
        original = f"{Path.home()}/build on {socket.gethostname()}"
        self.assertEqual(redact(original), "$HOME/build on [runner]")

    def test_junit_report_remains_valid_xml(self):
        report = f'<testsuite hostname="{socket.gethostname()}"><testcase name="Net"/></testsuite>'
        self.assertEqual(ET.fromstring(redact(report)).get("hostname"), "[runner]")


if __name__ == "__main__":
    unittest.main()
