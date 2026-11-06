import os
from pathlib import Path
import re
import subprocess
import sys
import xml.etree.ElementTree as ET

root = Path.home() / "poco-nightly"
build = root / "build"
for test_file in build.rglob("CTestTestfile.cmake"):
    contents = test_file.read_text()
    contents = re.sub(r' "-ignore" "[^"]+"', '', contents)
    contents = re.sub(r'"-all"(?! "-long")', '"-all" "-long"', contents)
    if test_file.parent == build / "Data" / "ODBC" / "testsuite":
        contents = contents.replace('"-all" "-long"', '"-long" "ODBCOracleTest" "ODBCSQLServerTest" "ODBCMySQLTest" "ODBCPostgreSQLTest" "ODBCSQLiteTest"')
    test_file.write_text(contents)

config = build / "bin" / "NetSSL-testrunner.xml"
tree = ET.parse(config)
tree.find("./testsuite/proxy/host").text = "127.0.0.1"
tree.find("./testsuite/proxy/port").text = "3128"
tree.write(config)
environment = os.environ.copy()
environment.pop("CPPUNIT_IGNORE", None)
environment["POCO_BASE"] = str(root / "source")
environment["PATH"] = str(build / "bin") + ":" + environment["PATH"]
environment["LD_LIBRARY_PATH"] = str(build / "lib") + ":" + str(build / "bin") + ":/opt/oracle/instantclient_23_26"
log_path = root / "logs" / "tests.log"
with log_path.open("w") as log:
    result = subprocess.run(["ctest", "--test-dir", str(build), "--verbose", "--no-tests=error", "--timeout", "7200", "--parallel", "1", "--output-junit", str(root / "logs" / "tests.xml")], env=environment, stdout=log, stderr=subprocess.STDOUT)
contents = log_path.read_text(errors="replace")
missing = re.findall(r"^.*(?:FAILURE \(ignored\)|ERROR \(ignored\)|tests not available|tests skipped|no pseudo terminal available).*$", contents, re.MULTILINE)
missing = [line for line in missing if "DB2" not in line and "Informix" not in line]
report = root / "logs" / "tests.xml"
if not report.exists():
    missing.append("CTest did not produce its JUnit report")
else:
    for testcase in ET.parse(report).iter("testcase"):
        if testcase.find("skipped") is not None:
            missing.append("Skipped CTest test: " + testcase.get("name", "unknown"))
if missing:
    print("Missing coverage or ignored failures:")
    print("\n".join(missing))
print(contents[-12000:])
sys.exit(1 if result.returncode or missing else 0)
