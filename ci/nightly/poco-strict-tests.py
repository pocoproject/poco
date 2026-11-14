import os
import json
from pathlib import Path
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from nightly_coverage import find_coverage_gaps, missing_suites

root = Path.home() / "poco-nightly"
build = root / "build"
fips_config = root / "fipsmodule.cnf"
subprocess.run(["openssl", "fipsinstall", "-module", "/usr/lib/x86_64-linux-gnu/ossl-modules/fips.so", "-out", str(fips_config)], check=True)
openssl_config = build / "Crypto" / "testsuite" / "nightly-openssl.cnf"
openssl_config.write_text(
    ".include /etc/ssl/openssl.cnf\n"
    f".include {fips_config}\n"
    "[openssl_init]\nproviders = nightly_providers\n"
    "[nightly_providers]\ndefault = nightly_default\nfips = fips_sect\n"
    "[nightly_default]\nactivate = 1\n"
)
for test_file in build.rglob("CTestTestfile.cmake"):
    contents = test_file.read_text()
    contents = re.sub(r' "-ignore" "[^"]+"', '', contents)
    contents = re.sub(r'"-all"(?! "-long")', '"-all" "-long"', contents)
    if test_file.parent == build / "Data" / "ODBC" / "testsuite":
        contents = contents.replace('"-all" "-long"', '"-long" "ODBCOracleTest" "ODBCSQLServerTest" "ODBCMySQLTest" "ODBCPostgreSQLTest" "ODBCSQLiteTest"')
    if test_file.parent == build / "Crypto" / "testsuite":
        contents += f'\nset_tests_properties(Crypto PROPERTIES ENVIRONMENT "OPENSSL_CONF={openssl_config}")\n'
    test_file.write_text(contents)

config = build / "bin" / "NetSSL-testrunner.xml"
tree = ET.parse(config)
tree.find("./testsuite/proxy/host").text = "127.0.0.1"
tree.find("./testsuite/proxy/port").text = "3128"
tree.write(config)
environment = os.environ.copy()
environment.pop("CPPUNIT_IGNORE", None)
environment["POCO_BASE"] = str(root / "source")
environment["PATH"] = environment["PATH"] + ":" + str(build / "bin")
environment["LD_LIBRARY_PATH"] = str(build / "lib") + ":" + str(build / "bin") + ":/opt/oracle/instantclient_23_26"
discovery = subprocess.run(["ctest", "--test-dir", str(build), "--show-only=json-v1"], env=environment, capture_output=True, text=True, check=True)
absent = missing_suites(test["name"] for test in json.loads(discovery.stdout)["tests"])
if absent:
    raise RuntimeError("Missing required CTest suites: " + ", ".join(absent))
log_path = root / "logs" / "tests.log"
with log_path.open("w") as log:
    result = subprocess.run(["ctest", "--test-dir", str(build), "--verbose", "--no-tests=error", "--timeout", "7200", "--parallel", "1", "--output-junit", str(root / "logs" / "tests.xml")], env=environment, stdout=log, stderr=subprocess.STDOUT)
contents = log_path.read_text(errors="replace")
missing = find_coverage_gaps(contents)
report = root / "logs" / "tests.xml"
if not report.exists():
    missing.append("CTest did not produce its JUnit report")
else:
    testcases = list(ET.parse(report).iter("testcase"))
    missing.extend("Missing CTest suite: " + name for name in missing_suites(case.get("name") for case in testcases))
    for testcase in testcases:
        if testcase.find("skipped") is not None:
            missing.append("Skipped CTest test: " + testcase.get("name", "unknown"))
if missing:
    print("Missing coverage or ignored failures:")
    print("\n".join(missing))
print(contents[-12000:])
with (root / "logs" / "sqlparser.log").open("w") as log:
    parser = subprocess.run(["make", "-C", str(root / "source" / "Data" / "SQLParser"), "-j4", "test"], env=environment, stdout=log, stderr=subprocess.STDOUT)
sys.exit(1 if result.returncode or missing or parser.returncode else 0)
